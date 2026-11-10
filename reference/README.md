# Reusable reference code

| Directory | Contents |
| --- | --- |
| `cpp/` | C++17 snippets: containers, search, DP, graphs, trees, math, geometry and strings |
| `python/` | Python big-integer and exact-fraction example |
| `shell/` | Standalone stress-test script |

Most C++ files are snippets, not complete programs. They use the aliases and
helpers in `cpp/template.cpp`; tests build a shared prelude from it.
That file supports this reference collection. The short templates for creating
contest solutions are in [../tmpl/](../tmpl/).

The current notebook contains selected APIs and technique notes. The original
chapters showing these snippets are in `../archive/sections/`.

Run the checks from the repository root:

```sh
bash tests/reference/run.sh
bash tests/reference/run.sh graphs     # one topic
```

The stress script is an existing reference example: copy `shell/stress.sh` next
to `sol.cpp`, `brute.cpp` and `gen.cpp`, then run it there. It compiles those
programs and compares their output on generated inputs.
