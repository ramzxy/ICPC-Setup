# NWERC preparation

University of Twente, team **Nth time's the charm**. Python is our main language;
C++17 is the fallback when its performance or containers help.

## Start a practice workspace

In Bash, from this repository:

```sh
source ./setup.sh ../contest
cd ../contest
tmpl                    # A/ through N/, both templates in every folder
rp A                    # Python, terminal input
rp A 1                  # Python with A/1.in
rc A 1                  # compile and run C++ with A/1.in
tp A                    # check Python against all A/*.in + A/*.out
tc A                    # compile once and check all C++ samples
```

`tmpl` preserves existing files. Any input basename works: `rp A sample` reads
`A/sample.in`. Omit the input argument for terminal input, or use `< file`.
Run the commands from the workspace root. In a new Bash terminal, source the
setup again; no shell startup files are required. `tp`/`tc` report every case
and fail overall if any case fails; expected `.out` files stay untouched.

For manually typing the setup at a contest, see [the setup guide](docs/setup.md).
The Python template uses normal `input()` and includes `ints()` for integer lines.

## Repository layout

| Directory | Contents |
| --- | --- |
| [notebook/](notebook/README.md) | Current LaTeX guide and printable PDF |
| [tmpl/](tmpl/) | The two current contest templates |
| [reference/](reference/README.md) | Reusable C++/Python code and a stress-test script |
| [tests/](tests/README.md) | Setup, reference-code and mathematics checks |
| [docs/](docs/setup.md) | Setup and practice guides |
| [practice/](docs/problem-packages.md) | Inspect/import problem ZIPs and run local checks |
| [environment/](docs/practice-environment.md) | Optional CLI container; full desktop practice uses Proxmox |
| [deploy/domjudge/](docs/domjudge.md) | k3s DOMjudge web/database and external judgehost groundwork |
| [deploy/charts/domjudge/](deploy/charts/domjudge/README.md) | Helm chart, optional HTTPS and experimental k3s worker |
| [archive/](archive/README.md) | Previous BAPC PDFs and sources |

`setup.sh` installs the current templates and defines the terminal functions.
The current print copy is [notebook/notebook.pdf](notebook/notebook.pdf), built
from [notebook/notebook.tex](notebook/notebook.tex). Page 1 contains the indexes
and setup; Python, C++ and algorithms/mathematics then have separate page ranges.
The target is **25 reference pages plus the cover**. The index follows page labels
automatically; the build rejects more than 25 reference pages.

## Practice environment and judging

These are complementary parts of the practice setup:

- [Proxmox desktop](docs/proxmox-practice.md): the main practice workstation,
  using the official ICPC image with NWERC checks and clean setup rehearsals.
  Access the same desktop from Windows or Linux.
- [Environment guide](docs/practice-environment.md): published tool versions
  and the optional Docker/WSL2 CLI fallback.
- [Problem packages](docs/problem-packages.md): import old contest ZIPs and
  check your solutions against sample or secret tests locally.
- [DOMjudge on k3s](deploy/charts/domjudge/README.md): a Helm deployment for the
  persistent web/database, optional HTTPS and an experimental worker pod.
  [The hosting guide](docs/domjudge.md) covers external workers and problem imports.
- [DOMjudge operator's manual](docs/domjudge-manual.md): create competitions from
  ZIP bundles, issue team logins, start/finish contests and check the server.
- [NWERC 2025 open practice](docs/nwerc2025-practice.md): the deployed A-L problem
  set, verified jury checks, long-running contest settings and team login guide.

NWERC 2026 lists GCC 13.3.0 and PyPy 7.3.15 (Python 3.9.18) for judging.
The notebook's current Python 3.10+ API assumptions need a compatibility pass
before contest use. Local output comparisons are smoke checks, not official
verdicts, especially for custom validators and multi-pass problems.
See the [official system specification](https://2026.nwerc.eu/system).

Imported archives and tests belong in ignored `practice-data/`; contestant files
belong in ignored `workspaces/`. Neither is baked into the practice image.
Deployment credentials are local files, excluded from Git.

## Build and check

```sh
bash notebook/build.sh                 # rebuild the current PDF
bash tests/setup.sh                    # isolated workspace/command checks
bash tests/reference/run.sh            # reference-code checks; needs GCC
python3 tests/math/check_graph_formulas.py
python3 tests/math/check_number_formulas.py
python3 -m unittest discover -s tests/practice -v
```

See [notebook maintenance](notebook/README.md) for editing and building, and
[test documentation](tests/README.md) for individual checks. Generated files
stay in ignored `.build/` directories. Historical material belongs in `archive/`;
there is one active notebook and one pair of contest templates.

Official competition information: [NWERC 2026 rules](https://2026.nwerc.eu/rules).
