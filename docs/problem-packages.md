# Released contest packages and local checks

The practice tools use the Python standard library (Python 3.10+), on Windows
or Linux. Run commands from this repository root. They read archive metadata as
data; they never use its API endpoints, run package scripts, or automatically
compile or execute included jury solutions or validators.

## What is in solutions.zip?

The supplied `solutions.zip` is a **NWERC 2025 contest bundle**, not just a folder
of sample solutions: `contest.yaml`, `problems.yaml`, twelve problem ZIPs
(A-L), `samples.zip`, statements, editorials and slides. The problem packages
contain 33 sample and 1,398 secret input files. Individual problems
contain sample and secret `.in`/`.ans` pairs, metadata, and reference submissions.
**Imported packages expose secret tests, answers and jury code.** Keep them separate
from your solution workspace; avoid browsing them during blind practice. For
team contests, retain these files on the judge server and use DOMjudge team
accounts instead of sharing the imported directory. For setup-only rehearsal,
start with an empty workspace and do not mount/import the problem data there.
Many specify custom output validation; Hasty Haul specifies a multipass protocol.

Keep downloaded archives and imported test data out of Git. The original ZIP
is left untouched and is also useful for importing problems into DOMjudge.

## Inspect and import

```sh
python3 -m practice inspect /path/to/solutions.zip
python3 -m practice inspect /path/to/solutions.zip --problem lastchristmas
python3 -m practice import /path/to/solutions.zip --dest practice-data/nwerc2025
```

For a quick first try, import one problem:

```sh
python3 -m practice import /path/to/solutions.zip \
    --dest practice-data/first-try --problem lastchristmas
```

The result is `practice-data/first-try/lastchristmas/`, with its original
`problem.yaml`, `data/`, submissions and statement files. Contest metadata and
PDFs are retained alongside problem directories. `samples.zip` has no
`problem.yaml` and is skipped; its samples are already in the problem packages.
A `practice-import.json` report records the source and inspected settings.
For an individual problem ZIP, omit `--problem`; its contents go directly into
the destination (an enclosing problem directory, if present, is preserved).

In Windows PowerShell, replace the archive path with, for example:

```powershell
python -m practice inspect C:/Users/Rick/Downloads/solutions.zip --problem lastchristmas
```

Import requires a **new destination** and does not overwrite existing work.
Nested ZIPs are spooled through temporary disk files, rather than loaded into
RAM. Extraction rejects traversal, absolute/Windows special paths, symlinks,
special files and case-colliding names. Limits are 100,000 ZIP members, 4 GiB
of declared total data (including nested ZIP files), and 1 GiB per member.
Allow disk space for the imported files and temporary nested archives. Archive
instructions are never treated as shell commands. This importer is deliberately
narrower than the complete package specification; symlink-based packages must be
handled with official tools in an appropriate isolated Linux environment.

## Run your solution

```sh
python3 -m practice run practice-data/first-try/lastchristmas \
    --solution /path/to/contest/L/py.py --python pypy3 --compare tokens
python3 -m practice run practice-data/first-try/lastchristmas \
    --solution /path/to/contest/L/cpp.cpp --data all --compare tokens
```

The default is sample data only. `--data secret` selects secret cases, and
`--data all` selects both. `--case substring` filters data-relative paths.
Python defaults to the interpreter running the tool; use `--python` explicitly
for the contest interpreter. C++ uses `g++ -std=gnu++20 -O2 -DONLINE_JUDGE
-DDOMJUDGE`; `--cxx /path/to/g++` and `--std gnu++17` override the compiler and
standard. The child receives `ONLINE_JUDGE=1`, `DOMJUDGE=1`, with `LOCAL` unset.

Inside the practice container, the repository is read-only at `/repo`, and
your writable files are under `/workspace`. Its `PYTHONPATH` exposes the driver.
Use CPython for the driver and select PyPy for the solution:

```sh
/usr/bin/python3 -m practice run /workspace/practice-data/first-try/lastchristmas \
    --solution /workspace/contest/L/py.py --python pypy3 --compare tokens \
    --artifacts /workspace/.build/practice
```

The runner executes **only the solution you explicitly select**. To use an
already compiled program or a more specific interpreter invocation:

```sh
python3 -m practice run practice-data/first-try/lastchristmas \
    --compare tokens --command /absolute/path/to/solution
```

`--command` must be the last option. Use absolute paths for executable/script
arguments: each test runs in a fresh artifact directory, not your current
working directory. Source paths passed through `--solution` are resolved for you.

## Smoke comparison versus judging

This tool is a local smoke checker, **not a submission sandbox or DOMjudge**.

- `MATCH`/`DIFF` compare outputs locally. They are not Accepted/Wrong Answer.
  Token comparison is case-sensitive and ignores whitespace; `--compare exact`
  compares bytes, including line endings. Neither handles float tolerances.
- A validator directory triggers the conservative custom/advanced check even
  if `problem.yaml` declares `default` (as in this bundle's Last Christmas).
  Custom or advanced validation requires an explicit `--compare tokens`,
  `--compare exact` or `--compare none`. This acknowledges a smoke check that
  does not execute the custom validator and may disagree with official judging.
- Interactive and multipass packages are refused, even with that override.
  Their input/output protocols require an actual compatible judge.
- `--compare none` checks execution only (`RUN`) and does not require `.ans`.
- `--time-limit 5` is a local wall-clock timeout per test, independent of the
  package/judge CPU limits. `--output-limit 16` caps stdout and stderr in MiB
  (combined, checked while running). Memory and CPU usage are not isolated.
  Run only code you trust; a container or this runner is not a judging sandbox.
- Each run gets a new directory under `.build/practice/`, with compiler output,
  per-case stdout/stderr and `report.json`. Input packages are not modified.
  `--artifacts /path` overrides that location.

The exit status is 0 if all selected cases match (or run successfully with
comparison disabled), 1 for a local comparison/runtime/timeout/output-limit
failure, and 2 for unsupported packages, invalid inputs or compilation failures.
Missing answers are reported before execution when comparison is enabled.

For example, a custom-output package can be checked for an exact jury-like
output during development, but that does not validate other correct outputs:

```sh
python3 -m practice run practice-data/nwerc2025/arcadecrane \
    --solution /path/to/my-solution.cpp --compare tokens
```

Use [DOMjudge](domjudge.md), with the problem's real validator and limits,
for practice submissions and final verdicts. See the official
[ICPC problem-package specification](https://icpc.io/problem-package-format/spec/2023-07-draft.html)
and [Kattis problemtools](https://github.com/Kattis/problemtools) for the full
format, validators, inheritance and package verification.

## Verify the tools

```sh
python3 -m unittest discover -s tests/practice -v
```

The tests use synthetic packages: nesting, traversal and symlink rejection,
case collisions, no-clobber imports, bounded resources, pairing, custom-validator
refusal, explicit commands, and output/runtime/timeout results. Real archives
remain local practice data rather than test fixtures committed to the repository.

The initial integration check inspected all twelve packages in the supplied ZIP,
imported Last Christmas, and explicitly selected its included Python and C++
jury solutions. Both matched the three sample outputs; the Python solution also
matched one selected secret case. These were host-runtime smoke checks, not a
verification of every submission, validator or contest time limit.
