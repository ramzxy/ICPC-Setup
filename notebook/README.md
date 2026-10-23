# BAPC team notebook

The team reference document (TRD) for BAPC 2026, written in LaTeX. It is
landscape A4 with three columns, in C++ only. Every algorithm comes with a short
"when to use it" box and tested code.

| Path | Contents |
| --- | --- |
| `notebook.tex` | Layout, styling and chapter order. Put your university and team name at the top |
| `sections/` | The text of each chapter |
| `code/cpp/` | The code snippets that get printed, one file per snippet |
| `code/sh/`, `code/py/` | The terminal setup, the stress test and the Python big-number example |
| `tests/` | One `test_<chapter>.cpp` per chapter; checks every snippet against brute force |

## Build and check

```sh
./build.sh             # writes notebook.pdf (needs TeX Live: pdflatex + latexmk)
./preview.sh graphs    # PDF of one chapter only, to check its layout
./tests/run.sh         # compiles and tests all snippets (needs g++)
./tests/run.sh graphs  # only tests/test_graphs.cpp
```

Run the tests after every code change. A snippet only belongs in the notebook
once its test passes.

## Adding a snippet

1. Put the code in `code/cpp/name.cpp`. Keep lines to 68 characters or fewer,
   or they wrap in the PDF.
2. Print it from a chapter: `\topic{Name}{$O(n \log n)$}`, then `\use{one line:
   when to use it}`, then `\code{name}`.
3. Add a check to that chapter's `tests/test_<chapter>.cpp`: `#include` the
   snippet and compare it with a slow, obviously correct version on random input.

## Contest rules for the TRD (BAPC 2026)

- At most 25 single-sided A4 pages.
- University name top left, page number top right.
- Readable from half a metre away.
- Handwritten notes are allowed on the front of the pages.
- Up to 3 copies, handed in at registration.
