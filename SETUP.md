# Contest workspace setup

Use Bash or zsh in one writable contest folder. Only the two language templates
belong in `tmpl/`; no shell helper file or shell startup-file access is needed.

## Set up once at the contest

```sh
mkdir -p contest/tmpl
cd contest
```

Type the Python and C++ templates from the printed guide into:

```text
contest/
  tmpl/
    template.py
    template.cpp
```

Check `python3 --version`, `g++ --version`, and the judge's supported versions.
Then enter the printed terminal command block directly into the terminal. It
starts with `CONTEST_ROOT=$PWD`, which remembers this workspace. Repeat that
block in every new terminal, from the workspace directory. Nothing needs to go
in `.bashrc`, `.zshrc`, or your home folder.

When copying from print, treat hooked arrows as line-wrap markers and join
the displayed fragments into one shell line.

If executable names differ, change `PYTHON=python3` or `CXX=g++` in the block.
For example, use an installed Homebrew GCC name on macOS if the default compiler
does not provide `<bits/stdc++.h>`. These variables each name one executable;
compiler flags belong in `rc`.

## Create the problem folders

```sh
tmpl C       # create A/, B/, C/ with both language files
tmpl         # default: create A/ through Z/
```

`tmpl c` also means A through C. Every directory gets both files, for example
`A/A.py` and `A/A.cpp`. Existing files are preserved, so running `tmpl` again
cannot erase solutions. The final folder layout is:

```text
contest/
  tmpl/
    template.py
    template.cpp
  A/
    A.py
    A.cpp
    1.in
    1.ans
  B/
    B.py
    B.cpp
```

## Run Python or C++

```sh
rp a                 # run A/A.py; input from the terminal
rc a                 # compile A/A.cpp and run it
rp A A/1.in          # literal input filename
rc A A/1.in
rp a < A/1.in        # ordinary shell redirection also works
rp a A/1.in > A/1.out
diff -u A/1.ans A/1.out
```

`rc` and `rp` accept one problem letter in either case. Choose the language with
its command: Python uses `rp`, C++ uses `rc`. There is no automatic language
choice, filename guessing, or special sample number. `rp a 1` reads the literal
file named `1`, not `A/1.in`.

Relative input filenames passed as arguments are resolved from the remembered
workspace. Absolute filenames work too. The commands continue to find the
workspace after `cd` elsewhere and do not change the current directory. Shell
redirections such as `< A/1.in` or `> A/1.out` are interpreted by your shell from
its current directory, so run those examples from the workspace.

With no input argument or redirection, paste the input into the terminal. Use
Ctrl-D on a new line when the program expects input until end-of-file. Submit
`A/A.py` or `A/A.cpp`, not the compiled executable.

## Practice machine: use the repository installer

From inside a new workspace, source the repository's setup file:

```sh
mkdir -p contest
cd contest
source /path/to/ICPC-Setup/setup.sh
```

Or select the workspace explicitly:

```sh
source /path/to/ICPC-Setup/setup.sh /path/to/contest
```

Sourcing installs the two templates and enables `tmpl`, `rc`, and `rp` in the
current terminal. It preserves existing files and does not change shell options,
your current directory, or any startup files. Repeat the source command in each
new terminal. To switch workspaces, source it again with the other directory.

Executing `bash /path/to/ICPC-Setup/setup.sh /path/to/contest` only copies the
templates; functions defined in a child shell do not persist in the parent. It
prints the source command needed to enable them. The installer does not install
Python, a compiler, or editor software.

At the contest, the printed command block is all you need after entering the
two templates; the practice installer itself does not need to be typed.

## Minimum workflow without functions

The same folder layout works with ordinary commands:

```sh
mkdir -p A
cp tmpl/template.py A/A.py     # only if A/A.py does not exist yet
python3 A/A.py < A/1.in

cp tmpl/template.cpp A/A.cpp   # only if A/A.cpp does not exist yet
g++ -std=c++17 -O2 -Wall -Wextra -g -DLOCAL A/A.cpp -o A/A
./A/A < A/1.in
```

## Input, debugging, and a quick check

Python uses the normal built-in `input()`: it returns a string and removes the
trailing newline. Use `int(input())` or `map(int, input().split())` for numbers.
An optional commented `sys.stdin.readline` shortcut also returns strings but
keeps the newline. Change input handling only when necessary.

Both templates have `dbg` on standard error. `rp` enables Python debugging with
`LOCAL=1`; `rc` enables C++ debugging with `-DLOCAL`. Redirecting ordinary output
to a file leaves debugging visible in the terminal. Normal judge execution leaves
template debugging disabled. For direct Python execution, enable it explicitly:

```sh
LOCAL=1 python3 A/A.py < A/1.in > A/1.out
```

The C++ command uses C++17, optimization, warnings, and debug symbols. Optionally
add `-fsanitize=address,undefined` for a memory bug if the compiler supports it.
Before practice, run each template on input `42` and check output `42`, sample
redirection, debug messages, and the editor.
