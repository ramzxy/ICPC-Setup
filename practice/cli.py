"""Small, conservative helpers for released ICPC problem packages."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import signal
import stat
import subprocess
import sys
import tempfile
import time
from zipfile import BadZipFile, ZipFile

MAX_FILES = 100_000
MAX_BYTES = 4 * 1024 ** 3
MAX_FILE_BYTES = 1024 ** 3


class PracticeError(Exception):
    pass


class Budget:
    def __init__(self):
        self.files = self.bytes = 0

    def add(self, info):
        self.files += 1
        self.bytes += info.file_size
        if self.files > MAX_FILES or self.bytes > MAX_BYTES or info.file_size > MAX_FILE_BYTES:
            raise PracticeError("Archive exceeds import limits (100,000 members, 4 GiB total, 1 GiB/member)")


def member_path(info):
    name = info.filename
    parts = name.rstrip('/').split('/')
    reserved = {'con', 'prn', 'aux', 'nul'} | {f'{p}{i}' for p in ('com', 'lpt') for i in range(1, 10)}
    if (not name or name.startswith('/') or '\\' in name or ':' in name
            or any(p in ('', '.', '..') or p.endswith(('.', ' ')) for p in parts)
            or any(p.split('.')[0].lower() in reserved for p in parts)):
        raise PracticeError(f"Unsafe ZIP member: {name!r}")
    mode = info.external_attr >> 16
    if stat.S_IFMT(mode) not in (0, stat.S_IFREG, stat.S_IFDIR):
        raise PracticeError(f"Links or special files are not imported: {name!r}")
    return PurePosixPath(*parts)


def checked_members(archive, budget):
    seen = set()
    infos = archive.infolist()
    for info in infos:
        path = member_path(info)
        key = str(path).casefold()
        if key in seen:
            raise PracticeError(f"Duplicate/case-colliding ZIP path: {info.filename!r}")
        seen.add(key)
        budget.add(info)
    return infos


def package_prefix(infos):
    roots = [str(member_path(i).parent) for i in infos if member_path(i).name == 'problem.yaml']
    if len(roots) != 1:
        return None
    return '' if roots[0] == '.' else roots[0] + '/'


def small_text(archive, name):
    try:
        info = archive.getinfo(name)
    except KeyError:
        return ''
    if info.file_size > 1024 * 1024:
        raise PracticeError(f"Metadata file is too large: {name}")
    return archive.read(info).decode('utf-8-sig', errors='replace')


def validation_info(text, names):
    # This deliberately does not claim to parse arbitrary YAML. Unknown or
    # advanced settings require an explicit smoke-comparison choice.
    match = re.search(r'^validation\s*:\s*([^\n]*)', text, re.M)
    raw = match.group(1).split('#', 1)[0].strip().strip("\"'") if match else 'default'
    special = re.search(r'\b(interactive|multi[-_ ]?pass)\b', text, re.I)
    validators = sorted({p.split('/')[0] for p in names if p.split('/')[0] in ('output_validators', 'answer_validators')})
    flags = bool(re.search(r'^\s*(validator_flags|output_validator_flags)\s*:', text, re.M))
    testdata = any(PurePosixPath(p).name == 'testdata.yaml' for p in names)
    complex_validation = raw != 'default' or validators or flags or testdata
    return {'declared': raw or '(mapping/unknown)', 'validators': validators,
            'explicit_comparison_required': bool(complex_validation),
            'interactive_or_multipass': bool(special)}


def describe(archive, infos, prefix, identifier):
    names = [i.filename[len(prefix):] for i in infos if not i.is_dir() and i.filename.startswith(prefix)]
    inputs = [n for n in names if n.startswith('data/') and n.endswith('.in')]
    available = set(names)
    result = {'id': identifier, 'sample_inputs': sum(n.startswith('data/sample/') for n in inputs),
              'secret_inputs': sum(n.startswith('data/secret/') for n in inputs),
              'missing_answers': [n for n in inputs if n[:-3] + '.ans' not in available]}
    result['validation'] = validation_info(small_text(archive, prefix + 'problem.yaml'), names)
    result['time_limit_text'] = small_text(archive, prefix + '.timelimit').strip()
    return result


def copy_member(archive, info, root):
    target = root.joinpath(*member_path(info).parts)
    if info.is_dir():
        target.mkdir(parents=True, exist_ok=True)
    else:
        target.parent.mkdir(parents=True, exist_ok=True)
        with archive.open(info) as src, target.open('xb') as dst:
            shutil.copyfileobj(src, dst, length=1024 * 1024)


def process_archive(source, output=None, selected=None):
    """Read nested ZIPs through temporary disk files, never unbounded RAM."""
    budget, problems, skipped = Budget(), [], []
    with ZipFile(source) as outer:
        infos = checked_members(outer, budget)
        prefix = package_prefix(infos)
        if prefix is not None:
            identifier = Path(source).stem
            if selected and selected != identifier:
                raise PracticeError(f"This package is {identifier!r}, not {selected!r}")
            problems.append(describe(outer, infos, prefix, identifier))
            if output is not None:
                for info in infos:
                    copy_member(outer, info, output)
        else:
            for info in infos:
                if info.is_dir():
                    continue
                rel = member_path(info)
                identifier = rel.stem
                if rel.suffix.lower() != '.zip':
                    if output is not None:
                        copy_member(outer, info, output)
                    continue
                if selected and identifier != selected:
                    continue
                with tempfile.TemporaryFile() as temp:
                    with outer.open(info) as src:
                        shutil.copyfileobj(src, temp, length=1024 * 1024)
                    temp.seek(0)
                    with ZipFile(temp) as inner:
                        inner_infos = checked_members(inner, budget)
                        inner_prefix = package_prefix(inner_infos)
                        if inner_prefix is None:
                            skipped.append(info.filename)
                            continue
                        problems.append(describe(inner, inner_infos, inner_prefix, identifier))
                        if output is not None:
                            # A nested package becomes one directory; original ZIPs
                            # remain in the downloaded bundle for DOMjudge import.
                            package_output = output.joinpath(*rel.with_suffix('').parts)
                            package_output.mkdir(parents=True, exist_ok=False)
                            for item in inner_infos:
                                copy_member(inner, item, package_output)
    if not problems:
        raise PracticeError("No problem.yaml package found (or selected problem does not exist)")
    return {'source': str(Path(source).resolve()), 'problems': problems,
            'skipped_nonproblem_zips': skipped, 'expanded_bytes_including_nested_zips': budget.bytes}


def import_archive(source, destination, selected=None):
    destination = Path(destination).absolute()
    if destination.exists() or destination.is_symlink():
        raise PracticeError(f"Destination already exists; use a new directory: {destination}")
    destination.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='.practice-import-', dir=destination.parent) as staging:
        payload = Path(staging) / 'payload'
        payload.mkdir()
        report = process_archive(source, payload, selected)
        (payload / 'practice-import.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
        if destination.exists() or destination.is_symlink():
            raise PracticeError(f"Destination appeared during import: {destination}")
        payload.rename(destination)
    return report


def problem_root(path):
    path = Path(path).resolve()
    if (path / 'problem.yaml').is_file():
        return path
    roots = list(path.glob('*/problem.yaml'))
    if len(roots) == 1:
        return roots[0].parent
    raise PracticeError(f"Select one problem directory containing problem.yaml: {path}")


def kill_process(process):
    if process.poll() is not None:
        return
    if os.name == 'nt':
        subprocess.run(['taskkill', '/PID', str(process.pid), '/T', '/F'],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)
    else:
        try:
            os.killpg(process.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass
    process.kill() if process.poll() is None else None
    process.wait()


def execute(command, input_path, directory, timeout, output_limit):
    directory.mkdir(parents=True)
    out_path, err_path = directory / 'stdout.txt', directory / 'stderr.txt'
    started = time.monotonic()
    verdict = 'RUN'
    with input_path.open('rb') as stdin, out_path.open('wb') as out, err_path.open('wb') as err:
        environment = os.environ.copy()
        environment.update(ONLINE_JUDGE='1', DOMJUDGE='1')
        environment.pop('LOCAL', None)
        process = subprocess.Popen(command, stdin=stdin, stdout=out, stderr=err,
                                   cwd=directory, env=environment, start_new_session=os.name != 'nt')
        try:
            while process.poll() is None:
                if time.monotonic() - started > timeout:
                    verdict = 'TIMEOUT'
                    break
                if out_path.stat().st_size + err_path.stat().st_size > output_limit:
                    verdict = 'OUTPUT_LIMIT'
                    break
                time.sleep(.01)
        finally:
            if process.poll() is None:
                kill_process(process)
        if verdict == 'RUN' and process.returncode:
            verdict = 'RUNTIME_ERROR'
    if verdict == 'RUN' and out_path.stat().st_size + err_path.stat().st_size > output_limit:
        verdict = 'OUTPUT_LIMIT'
    return verdict, out_path, time.monotonic() - started


def solution_command(args, directory):
    if args.command:
        return args.command
    solution = Path(args.solution).resolve()
    if not solution.is_file():
        raise PracticeError(f"Solution not found: {solution}")
    if solution.suffix.lower() == '.py':
        return [args.python, str(solution)]
    if solution.suffix.lower() == '.cpp':
        executable = directory / ('solution.exe' if os.name == 'nt' else 'solution')
        command = [args.cxx, '-std=' + args.std, '-O2', '-DONLINE_JUDGE', '-DDOMJUDGE',
                   str(solution), '-o', str(executable)]
        with (directory / 'compile.stdout').open('wb') as out, (directory / 'compile.stderr').open('wb') as err:
            result = subprocess.run(command, stdout=out, stderr=err, timeout=60, check=False)
        if result.returncode:
            raise PracticeError(f"Compilation failed; see {directory / 'compile.stderr'}")
        return [str(executable)]
    raise PracticeError("Use --solution for .py/.cpp files, or --command for an explicit executable command")


def run_problem(args):
    root = problem_root(args.problem)
    names = [str(p.relative_to(root)).replace(os.sep, '/') for p in root.rglob('*') if p.is_file()]
    metadata = (root / 'problem.yaml').read_text(encoding='utf-8-sig')
    validation = validation_info(metadata, names)
    if validation['interactive_or_multipass']:
        raise PracticeError("Interactive/multipass packages need their judge/validator protocol; use DOMjudge")
    if validation['explicit_comparison_required'] and args.compare is None:
        raise PracticeError("Custom/advanced validation: use DOMjudge, or explicitly choose --compare tokens|exact|none for smoke checks only")
    comparison = args.compare or 'tokens'
    inputs = sorted((root / 'data' / args.data).rglob('*.in')) if args.data != 'all' else sorted((root / 'data').rglob('*.in'))
    if args.case:
        inputs = [p for p in inputs if args.case in str(p.relative_to(root / 'data'))]
    if not inputs:
        raise PracticeError("No matching .in cases found")
    if comparison != 'none':
        missing = [str(p) for p in inputs if not p.with_suffix('.ans').is_file()]
        if missing:
            raise PracticeError(f"Cases without .ans files: {missing[:5]}")
    output_limit = int(args.output_limit * 1024 ** 2)
    if not args.time_limit > 0 or not 0 < args.output_limit <= 1024:
        raise PracticeError("Time limit must be positive; output limit must be in (0, 1024] MiB")
    artifacts = Path(args.artifacts).resolve()
    artifacts.mkdir(parents=True, exist_ok=True)
    directory = Path(tempfile.mkdtemp(prefix=root.name + '-', dir=artifacts))
    command = solution_command(args, directory)
    results = []
    print('Local smoke checks only: MATCH/DIFF are not judge verdicts. No memory or CPU isolation.')
    print('Artifacts:', directory)
    for index, input_path in enumerate(inputs):
        name = input_path.relative_to(root / 'data').as_posix()
        status, output, elapsed = execute(command, input_path, directory / f'case-{index:04d}', args.time_limit, output_limit)
        if status == 'RUN' and comparison != 'none':
            answer = input_path.with_suffix('.ans')
            if answer.stat().st_size > output_limit:
                status = 'ANSWER_TOO_LARGE'
            else:
                actual, expected = output.read_bytes(), answer.read_bytes()
                same = actual == expected if comparison == 'exact' else actual.split() == expected.split()
                status = 'MATCH' if same else 'DIFF'
        results.append({'case': name, 'status': status, 'wall_seconds': round(elapsed, 4)})
        print(f'{status:16} {elapsed:.3f}s {name}')
    (directory / 'report.json').write_text(json.dumps({'command': command, 'comparison': comparison, 'results': results}, indent=2) + '\n', encoding='utf-8')
    return 0 if all(r['status'] in ('RUN', 'MATCH') for r in results) else 1


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='action', required=True)
    inspect = sub.add_parser('inspect', help='inspect a problem ZIP or released contest bundle without executing anything')
    inspect.add_argument('archive')
    inspect.add_argument('--problem', help='select one nested problem by ZIP stem')
    extract = sub.add_parser('import', help='extract into a new directory; never overwrite existing files')
    extract.add_argument('archive')
    extract.add_argument('--dest', required=True, help='new destination, normally under practice-data/')
    extract.add_argument('--problem', help='select one nested problem by ZIP stem')
    run = sub.add_parser('run', help='run an explicitly selected solution on sample/secret data (smoke checks only)')
    run.add_argument('problem')
    source = run.add_mutually_exclusive_group(required=True)
    source.add_argument('--solution', help='your Python or C++ source path')
    source.add_argument('--command', nargs=argparse.REMAINDER, help='explicit command, must be the last option')
    run.add_argument('--python', default=sys.executable, help='solution interpreter; e.g. pypy3')
    run.add_argument('--cxx', default='g++')
    run.add_argument('--std', default='gnu++20')
    run.add_argument('--data', choices=('sample', 'secret', 'all'), default='sample')
    run.add_argument('--case', help='substring of the data-relative case path')
    run.add_argument('--compare', choices=('tokens', 'exact', 'none'), help='explicit local comparison; never runs package validators')
    run.add_argument('--time-limit', type=float, default=5, help='local wall seconds per case, NOT jury CPU time')
    run.add_argument('--output-limit', type=float, default=16, help='combined stdout/stderr MiB per case')
    run.add_argument('--artifacts', default='.build/practice')
    args = parser.parse_args(argv)
    try:
        if args.action == 'run':
            if not args.solution and not args.command:
                raise PracticeError('--command requires an executable')
            return run_problem(args)
        if args.action == 'import':
            result = import_archive(args.archive, args.dest, args.problem)
        else:
            result = process_archive(args.archive, selected=args.problem)
        print(json.dumps(result, indent=2))
        return 0
    except (PracticeError, OSError, BadZipFile, subprocess.TimeoutExpired) as error:
        print(f'practice: {error}', file=sys.stderr)
        return 2
