# ICPC-Setup

Our contest setup for BAPC 2026: University of Twente, team "Nth time's the charm".

| Path | What it is |
| --- | --- |
| `notebook/notebook.pdf` | The team reference document. Print it 3 times, single-sided |
| `setup.sh` | Adds the `mk` / `c` / `r` commands to your shell and copies the template |
| `notebook/` | LaTeX source of the notebook, its code snippets and their tests |

## Setting up a practice machine

```sh
bash setup.sh
```

Then open a new terminal. The script copies the template to `~/template.cpp`
and adds the commands below to `~/.zshrc` or `~/.bashrc`. On a Mac it uses
Homebrew's `g++-NN`, because the default `g++` there has no `<bits/stdc++.h>`.

At the contest there is no internet: type in the template ("Template" in
chapter 1 of the notebook) as `~/template.cpp`, and the "Terminal setup" block
into the terminal.

## Commands

Work in a separate folder, one subfolder per problem:

```sh
mk a      # folder a/ with a/a.cpp copied from the template
c a       # compile a/a.cpp: dbg() on, checks for out-of-range and overflow
r a       # compile and run: paste the input, then Ctrl-D
r a 1     # compile and run on a/1.in
```

`dbg(x)` in the template prints `x = ...` only when compiled with `c`
(`-DLOCAL`). On the judge it does nothing, so it can stay in the code.

## Changing the notebook

```sh
notebook/build.sh       # rebuilds notebook/notebook.pdf (needs TeX Live)
notebook/tests/run.sh   # tests every code snippet against brute force
```

Details are in [notebook/README.md](notebook/README.md).

## TRD rules (BAPC 2026)

- At most 25 single-sided A4 pages.
- University name top left, page number top right.
- Up to 3 copies, handed in at registration.
- Handwritten notes are allowed on the front of the pages.
