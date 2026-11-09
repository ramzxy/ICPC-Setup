# Unified NWERC guide

Build and edit `notebook.tex`; print `notebook.pdf`. It starts at reference page
1 after a cover. Content flows continuously across landscape A4 columns:
index, local workspace setup, Python, C++17, mathematics, then algorithm notes.
The index follows labels automatically. The PDF build rejects more than 25
reference pages; the cover is separate.

## Source map

| Path | Purpose |
| --- | --- |
| `notebook.tex` | Shared layout and reading order |
| `sections/index.tex` | Automatic topic/page index |
| `sections/workspace.tex` | Setup guide, printed templates and commands |
| `../tmpl/`, `../setup.sh` | Actual starter files and terminal functions |
| `sections/python-language.tex` | Python syntax and standard-library APIs |
| `sections/cpp-language.tex` | C++17 syntax and standard-library APIs |
| `sections/math-basics.tex`, `python/` math sources | Formula lookup |
| `sections/algorithm-notes.tex` | Technique selection and pitfalls |
| `code/`, `tests/` | Preserved algorithm implementations and checks |
| `legacy/` | Frozen BAPC PDFs and original main source |

Older algorithm chapters in `sections/` remain available for practice. They
are not all printed in the current guide. The former `python/` supplement is
superseded as a separate document; its mathematics sources are shared here.
Do not concatenate old PDFs to make the NWERC print copy.

## Build and validate

```sh
bash notebook/build.sh             # TeX Live or MiKTeX with latexmk
bash tests/setup.sh                # run from the repository root
bash notebook/tests/run.sh         # original algorithm tests; GCC required
```

Build files and visual checks go in the ignored `.build/` directory. For an
alternate PDF destination, pass it to `notebook/build.sh` (relative paths are
resolved from `notebook/`). `python/build.sh` forwards to the unified build for
compatibility. To preview an original algorithm chapter, use `preview.sh`.

The printed setup reads the real templates and command block in `../setup.sh`.
If that block moves, update its listing line range in `sections/workspace.tex`.
Keep C++17 and Python 3.10-compatible examples, and check compilation and the
rendered PDF after changes. Algorithm code has meaningful tests; language
examples should stay short enough to scan quickly.

Rules: [NWERC 2026](https://2026.nwerc.eu/rules). Python and mathematics source
references are in `python/SOURCES.md`.
