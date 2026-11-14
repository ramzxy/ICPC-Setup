# Contest setup: the short version

Use Bash. Work from the contest workspace root whenever you run these commands.

1. Run `mkdir -p contest/tmpl`, then `cd contest`.
2. Type the two short templates from the guide into `tmpl/py.py` and
   `tmpl/cpp.cpp`.
3. Save the combined terminal-functions block, including the sample-test
   helpers, in `contest/s.sh`. Run `source s.sh` from `contest/` in each
   Bash terminal. Nothing goes in `.bashrc`.
4. Run `tmpl` to create A through N with both language files. Existing
   solutions are preserved.

```sh
rp A                  # Python, pasted input
rc A                  # compile and run C++
rp A 1           # saved input file
rc A 1
rp A < A/1.in         # ordinary redirection also works
tp A                  # all A/*.in against matching A/*.out
tc A                  # compile once, then test every case
rp A 1 > A/1.got       # optional manual capture
diff -bB A/1.out A/1.got
```

Either case is accepted for letters. Directories are uppercase; copied filenames
stay unchanged: `A/py.py`, `A/cpp.cpp`. `tmpl` copies the entire
template directory with `cp -rn tmpl/. "$p"`; `-n` preserves existing files.
The optional input is a basename: `rp A 1` reads `A/1.in`, and `rp A sample`
reads `A/sample.in`. With no input argument, input comes from the terminal or a pipe. Use Ctrl-D
when the program expects EOF.

The functions use the current directory. Stay at the workspace root; run
`source s.sh` in a new Bash terminal. Hooked arrows
in the PDF mark visual wraps: type those fragments as one logical line.
If needed, replace `python3` or `g++` in the functions with the judge's executable.

The Python template contains only `dbg` and `ints()`. Add imports as needed
with `from module import name`:

| Module | Useful imports |
| --- | --- |
| `collections` | `Counter`, `defaultdict`, `deque` |
| `operator` | `itemgetter`, `attrgetter` |
| `heapq` | `heappush`, `heappop`, `heapify` |
| `bisect` | `bisect_left`, `bisect_right` |
| `functools` | `cache`, `cmp_to_key` |
| `itertools` | `accumulate`, `combinations`, `permutations`, `product` |
| `math` | `gcd`, `lcm`, `isqrt`, `comb`, `inf`, `sqrt`, `hypot`, `atan2`, `pi`, `factorial`, `prod` |

Python uses normal string-returning `input()`. The one-line `ints()` helper reads
one line as a list of decimal integers: use `n, m = ints()` or `a = ints()`.
Python aliases `dbg = print`; remove debug calls before testing or submitting,
since they print to stdout along with answers. C++ `dbg(x)` also prints to stdout
using `cout`; remove those calls too. The small C++ debug macro handles streamable
values; loop over vectors when debugging them. Compilation failure stops `rc`
before it can run an old executable.

C++ `main()` starts with only the fast-I/O setup. It disables synchronization
with C stdio and automatic flushing of `cout` before `cin` reads. Use C++
streams consistently and flush explicitly for interactive problems.

For large input/output, you can add these optional aliases:

```python
import sys
input = sys.stdin.readline
write = sys.stdout.write
```

Use either when useful for large, non-interactive input/output. Fast line
input still returns `str`, including the trailing newline; use
`input().rstrip("\r\n")` for text comparisons. It returns `""` at EOF instead
of raising `EOFError`. Integer parsing and `ints()` work with either reader.
For many output lines, collect strings in `out`, then replace the repeated
`print` calls with `write("\n".join(out) + "\n")`. `write` adds no newline of
its own. Normal `print()` remains available.

## Test all provided samples

Save each expected output alongside its input: `A/1.in` with `A/1.out`,
`A/2.in` with `A/2.out`, and so on. `tp A` runs Python; `tc A` compiles C++
**once**, then runs all cases. Lowercase problem letters also work.

The optional block uses one shared helper and two short commands. Each case
prints its filename and `OK` or `FAIL`, with a diff on mismatches. It continues
through every case and returns a nonzero status if any case fails. Runtime errors,
missing expected files and an empty test directory also fail. Remove all
`dbg(...)` calls before comparing output. Expected
`.out` files are read only; use `.got` for manual captures.

Comparison ignores changes in spacing and blank lines, while preserving token
boundaries and line structure. It is a quick check of the provided examples;
problems with custom output validation or numeric tolerances need separate checking.

## Practice shortcut

With this repository available, install the two templates and define the
commands without typing them:

```sh
source /path/to/ICPC-Setup/setup.sh /path/to/contest
cd /path/to/contest
tmpl
```

Sourcing preserves existing templates, current directory and shell options.
It never writes startup files. Executing `bash setup.sh /path/to/contest`
only copies templates and prints the source command. The practice installer
is not part of what you type at the contest.

## Without terminal functions

```sh
python3 A/py.py < A/1.in
g++ -std=c++17 -O2 A/cpp.cpp -o A/A
./A/A < A/1.in
```

The guide has the full Python and C++ syntax/API reference, including optional
faster input and numeric formatting when needed.
