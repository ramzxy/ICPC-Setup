# Current NWERC guide

Edit `notebook.tex`; print `notebook.pdf`. Numbering starts at reference page 1
after the cover. Page 1 contains the separate Python, C++ and algorithm indexes
and workspace setup. Each reference part starts on a fresh page; content flows
across columns within that part. Each setup source file stays in one column.

The target is 25 reference pages plus the cover. The index follows labels
automatically, and the build rejects more than 25 reference pages. Pale blue,
green and peach page headers match the Python, C++ and algorithm index headings.

## Sources

| Path | Purpose |
| --- | --- |
| `notebook.tex` | Shared layout, colors and page-separated reading order |
| `sections/index.tex` | Separate topic indexes with automatic page references |
| `sections/workspace.tex` | Printed setup guide and actual template listings |
| `../tmpl/`, `../setup.sh` | Current starter files and terminal functions |
| `sections/python-language.tex` | Python syntax and standard-library APIs |
| `sections/cpp-language.tex` | C++17 syntax and standard-library APIs |
| `sections/math/` | Everyday formulas, number theory, graphs and probability |
| `sections/math/SOURCES.md` | Mathematics sources and verification notes |
| `sections/algorithm-notes.tex` | Technique selection and pitfalls |

Reusable implementations are in [../reference/](../reference/README.md), with
checks in [../tests/](../tests/README.md). Older chapters and print snapshots
are in [../archive/](../archive/README.md); they are not active notebook inputs.

## Build

From the repository root:

```sh
bash notebook/build.sh
```

Requires TeX Live or MiKTeX with `latexmk` and the packages listed in the main
source. Build files and visual checks go in the ignored `.build/` directory.
The exported `notebook.pdf` is the print copy; refresh it after source edits.
An optional output directory passed to `build.sh` is relative to `notebook/`.

The printed setup reads the actual templates and the marked function block in
`../setup.sh`. If that block moves, update its main and optional-test listing line ranges in
`sections/workspace.tex`. Keep examples compatible with C++17 and Python 3.10,
then check compilation, page boundaries and the rendered PDF after changes.
