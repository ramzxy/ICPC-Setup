# Repository checks

Run these commands from the repository root. This directory checks the setup,
reference implementations, printed formulas and local problem-package tooling.
The local runner is not a replacement for DOMjudge.

| Command | Checks |
| --- | --- |
| `python3 -m unittest discover -s tests/practice -v` | ZIP import safety, package layouts, local output checks and execution failures |
| `bash tests/setup.sh` | A-N generation, preservation of solutions, stdin/sample files, debug output, all-case sample testing and compile-failure handling |
| `bash tests/reference/run.sh` | C++ reference snippets against assertions and brute-force results, plus the Python numeric example |
| `python3 tests/math/check_graph_formulas.py` | Graph, tree, partition, permutation and orbit-counting formulas on small exhaustive cases |
| `python3 tests/math/check_number_formulas.py` | Number theory, sums, modular arithmetic and probability formulas |

Setup checks require Bash, Python 3 and optionally GCC for the C++ checks.
Reference checks require GCC and Python 3; an individual topic can be selected
with `bash tests/reference/run.sh graphs` (or `dp`, `math`, `strings`, etc.).
Formula checks use only the Python standard library and require Python 3.10+.

The reference runner writes executables and its generated prelude into
`reference/.build/` inside this directory. Setup checks use an isolated temporary
workspace and remove it afterward. Generated files are ignored by Git.

Problem-package tooling checks use only the Python standard library (Python
3.10+). See [the package guide](../docs/problem-packages.md) for real archives.
