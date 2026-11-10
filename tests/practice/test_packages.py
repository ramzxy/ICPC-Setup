import argparse
from contextlib import redirect_stderr, redirect_stdout
import io
from pathlib import Path
import stat
import sys
import tempfile
import unittest
from unittest.mock import patch
from zipfile import ZipFile, ZipInfo

from practice.cli import PracticeError, import_archive, main, process_archive


def write_zip(path, files):
    with ZipFile(path, 'w') as archive:
        for name, content in files.items():
            archive.writestr(name, content)


def package_bytes(validation='default', extras=None):
    content = io.BytesIO()
    with ZipFile(content, 'w') as archive:
        files = {'problem.yaml': 'name: Increment\nvalidation: ' + validation + '\n',
                 'data/sample/1.in': '41\n', 'data/sample/1.ans': '42\n',
                 'data/secret/group/2.in': '-3\n', 'data/secret/group/2.ans': '-2\n'}
        files.update(extras or {})
        for name, text in files.items():
            archive.writestr(name, text)
    return content.getvalue()


class PackageTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cache = Path('.build/practice-tests').resolve()
        cls.cache.mkdir(parents=True, exist_ok=True)
        cls.old_tempdir = tempfile.tempdir
        tempfile.tempdir = str(cls.cache)

    @classmethod
    def tearDownClass(cls):
        tempfile.tempdir = cls.old_tempdir

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(dir=self.cache)
        self.root = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def bundle(self, validation='default', extras=None):
        path = self.root / 'contest.zip'
        write_zip(path, {'increment.zip': package_bytes(validation, extras),
                         'samples.zip': self.sample_zip(), 'contest.yaml': 'name: Example\n'})
        return path

    def sample_zip(self):
        b = io.BytesIO()
        with ZipFile(b, 'w') as z:
            z.writestr('A/1.in', '41\n')
        return b.getvalue()

    def import_problem(self, validation='default', extras=None):
        target = self.root / 'problems'
        import_archive(self.bundle(validation, extras), target)
        return target / 'increment'

    def invoke(self, *args):
        out, err = io.StringIO(), io.StringIO()
        with redirect_stdout(out), redirect_stderr(err):
            result = main(list(args))
        return result, out.getvalue(), err.getvalue()

    def test_nested_contest_inspection_and_import(self):
        archive = self.bundle()
        report = process_archive(archive)
        self.assertEqual(report['problems'][0]['sample_inputs'], 1)
        self.assertEqual(report['problems'][0]['secret_inputs'], 1)
        self.assertEqual(report['skipped_nonproblem_zips'], ['samples.zip'])
        target = self.root / 'problems'
        import_archive(archive, target)
        self.assertEqual((target / 'increment/data/sample/1.in').read_text(), '41\n')
        self.assertTrue((target / 'practice-import.json').is_file())
        with self.assertRaises(PracticeError):
            import_archive(archive, target)
        self.assertEqual((target / 'increment/data/sample/1.in').read_text(), '41\n')

    def test_select_one_problem_and_handle_wrapper(self):
        b = io.BytesIO()
        with ZipFile(b, 'w') as inner:
            inner.writestr('wrapped/problem.yaml', 'name: Wrapped\n')
            inner.writestr('wrapped/data/sample/1.in', '1\n')
            inner.writestr('wrapped/data/sample/1.ans', '2\n')
        path = self.root / 'contest.zip'
        write_zip(path, {'one.zip': b.getvalue(), 'two.zip': package_bytes()})
        target = self.root / 'one-only'
        report = import_archive(path, target, selected='one')
        self.assertEqual([p['id'] for p in report['problems']], ['one'])
        self.assertFalse((target / 'two').exists())
        self.assertTrue((target / 'one/wrapped/problem.yaml').is_file())

    def test_reject_traversal_absolute_windows_and_links(self):
        for name in ('../escape', '/absolute', 'a/../../escape', 'C:/escape',
                     'a\\..\\escape', 'file:stream', 'NUL', 'name. '):
            with self.subTest(name=name):
                archive = self.root / 'unsafe.zip'
                write_zip(archive, {'problem.yaml': 'name: Unsafe', name: 'x'})
                target = self.root / 'must-not-appear'
                with self.assertRaises(PracticeError):
                    import_archive(archive, target)
                self.assertFalse(target.exists())
        archive = self.root / 'link.zip'
        link = ZipInfo('data/link.in')
        link.create_system = 3
        link.external_attr = (stat.S_IFLNK | 0o777) << 16
        with ZipFile(archive, 'w') as z:
            z.writestr('problem.yaml', 'name: Bad link')
            z.writestr(link, '../outside')
        with self.assertRaises(PracticeError):
            import_archive(archive, self.root / 'links')

    def test_reject_case_collision_and_nested_traversal(self):
        archive = self.root / 'collision.zip'
        write_zip(archive, {'problem.yaml': '', 'Data/x.in': '', 'data/x.in': ''})
        with self.assertRaises(PracticeError):
            import_archive(archive, self.root / 'collision')
        inner = io.BytesIO()
        with ZipFile(inner, 'w') as z:
            z.writestr('problem.yaml', '')
            z.writestr('../escape.txt', 'x')
        write_zip(archive, {'bad.zip': inner.getvalue()})
        with self.assertRaises(PracticeError):
            import_archive(archive, self.root / 'nested')
        self.assertFalse((self.root / 'escape.txt').exists())

    def test_declared_archive_size_limit_and_output_limit(self):
        archive = self.bundle()
        with patch('practice.cli.MAX_BYTES', 10):
            with self.assertRaises(PracticeError):
                import_archive(archive, self.root / 'oversized')
        self.assertFalse((self.root / 'oversized').exists())
        problem = self.import_problem()
        solution = self.root / 'verbose.py'
        solution.write_text('print("x" * 100000)')
        code, out, _ = self.invoke('run', str(problem), '--solution', str(solution),
                                   '--output-limit', '.01', '--artifacts', str(self.root / 'runs'))
        self.assertEqual(code, 1)
        self.assertIn('OUTPUT_LIMIT', out)

    def test_python_samples_secret_data_and_artifacts(self):
        problem = self.import_problem()
        solution = self.root / 'my solution.py'
        solution.write_text('print(int(input()) + 1)\n')
        code, out, err = self.invoke('run', str(problem), '--solution', str(solution),
                                    '--data', 'all', '--artifacts', str(self.root / 'runs'))
        self.assertEqual((code, err), (0, ''))
        self.assertEqual(out.count('MATCH '), 2)
        self.assertEqual(len(list((self.root / 'runs').rglob('report.json'))), 1)
        self.assertFalse(list(problem.rglob('stdout.txt')))

    def test_diff_runtime_error_and_timeout(self):
        problem = self.import_problem()
        solution = self.root / 'solution.py'
        for body, expected in [('print(0)', 'DIFF'), ('raise ValueError()', 'RUNTIME_ERROR'),
                               ('import time; time.sleep(5)', 'TIMEOUT')]:
            with self.subTest(expected=expected):
                solution.write_text(body)
                code, out, _ = self.invoke('run', str(problem), '--solution', str(solution),
                                           '--time-limit', '.15', '--artifacts', str(self.root / 'runs'))
                self.assertEqual(code, 1)
                self.assertIn(expected, out)

    def test_custom_requires_explicit_smoke_and_multipass_refused(self):
        problem = self.import_problem('custom', {'answer_validators/check/main.cpp': 'not executed'})
        solution = self.root / 'solution.py'
        solution.write_text('print(int(input())+1)')
        common = ['run', str(problem), '--solution', str(solution), '--artifacts', str(self.root / 'runs')]
        code, _, err = self.invoke(*common)
        self.assertEqual(code, 2)
        self.assertIn('Custom/advanced', err)
        self.assertEqual(self.invoke(*common, '--compare', 'tokens')[0], 0)
        (problem / 'problem.yaml').write_text('validation: custom multi-pass\n')
        code, _, err = self.invoke(*common, '--compare', 'tokens')
        self.assertEqual(code, 2)
        self.assertIn('Interactive/multipass', err)

    def test_missing_answer_and_explicit_command(self):
        problem = self.import_problem()
        (problem / 'data/sample/1.ans').unlink()
        code, _, err = self.invoke('run', str(problem), '--command', sys.executable, '-c', 'print(42)')
        self.assertEqual(code, 2)
        self.assertIn('without .ans', err)
        code, out, _ = self.invoke('run', str(problem), '--compare', 'none', '--artifacts', str(self.root / 'runs'),
                                   '--command', sys.executable, '-c', 'print(42)')
        self.assertEqual(code, 0)
        self.assertIn('RUN ', out)


if __name__ == '__main__':
    unittest.main()
