# NWERC preparation

University of Twente, team **Nth time's the charm**. Python is our main language;
C++17 is the fallback when its performance or containers help.

## Start a practice workspace

In Bash (or zsh), from this repository:

```sh
mkdir -p ../contest
source ./setup.sh ../contest
cd ../contest
tmpl M                  # A/ through M/, both A.py and A.cpp, etc.
rp A                    # Python, normal terminal input
rp A A/1.in             # Python with a saved input file
rc A A/1.in             # compile and run C++ with that input
```

`tmpl` defaults to Z if its last letter is omitted. Lowercase letters are
accepted too. Repeating it keeps existing solutions. `rp` and `rc` accept an
input filename, relative to the workspace or absolute; omit it for terminal
input or ordinary `< file` redirection.

Everything lives in the workspace. `tmpl/` contains only `template.py` and
`template.cpp`. The three functions belong to the current terminal; no shell
startup files or home-directory templates are required. In a new terminal,
source the setup again, or paste the command block from the guide.

The Python template uses ordinary `input()`, returning strings. Its optional
string-returning line-reader optimization is commented out.

For the full contest-machine setup guide, including typing the setup from
paper without this repository, see [SETUP.md](SETUP.md). It explains samples,
stderr debugging, interpreter/compiler selection and submission.

## One reference document

- [Unified PDF](notebook/notebook.pdf): index, setup, Python APIs, C++17 APIs,
  mathematics and compact algorithm notes, in that order.
- [Main LaTeX source](notebook/notebook.tex): the entry point for all edits.
- [Notebook maintenance](notebook/README.md): layout, build and source map.

The guide focuses on language lookup and practical pitfalls. Tested algorithm
implementations remain in `notebook/code/` and `notebook/tests/` for practice;
the old BAPC print material is preserved in `notebook/legacy/`.

```sh
bash notebook/build.sh        # rebuild notebook/notebook.pdf
bash tests/setup.sh           # isolated workspace/command checks
bash notebook/tests/run.sh    # original algorithm checks; needs GCC
```

NWERC 2026 allows up to 25 single-sided reference pages, with the university
at the upper left and page numbers at the upper right. The front of the folder
must name the institution. The guide includes a separate cover and enforces
the 25-page reference limit. Check the current official rules before printing:
[NWERC 2026 rules](https://2026.nwerc.eu/rules).
