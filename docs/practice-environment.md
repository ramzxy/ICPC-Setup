# Practice environment: Proxmox desktop and optional CLI

The team's main practice route is a full desktop VM on Proxmox, accessible from
Windows and Linux. Follow the [Proxmox provisioning guide](proxmox-practice.md)
to import the official ICPC image, verify NWERC's tool versions and keep a clean
baseline for setup rehearsals. The Docker files remain an optional CLI fallback.
Both routes are separate from the DOMjudge service and judge workers.

## Published NWERC 2026 setup

Checked on 2026-11-10 against the [official NWERC system page](https://2026.nwerc.eu/system):

| Item | Published configuration |
| --- | --- |
| Workstation OS / desktop | Ubuntu 24.04.3 LTS / Xfce 4.18 |
| C++ judge | GCC 13.3.0, GNU C++20 |
| Python judge | PyPy 7.3.15 implementing Python 3.9.18 |
| Judge memory | 2 GiB, including the interpreter/runtime |
| Available editors | Includes Vim, Nano, VS Code, CLion and PyCharm |

The published C++ command uses `-std=gnu++20 -Wall -O2 -static -pipe` and defines
`ONLINE_JUDGE` and `DOMJUDGE`. Python runs with `pypy3`, with those two environment
variables set to `1`. The site also publishes convenient compiler/interpreter
aliases. Recheck the site shortly before the contest; organizer changes take
precedence over this snapshot.

C++17 solutions remain valid within GNU C++20. More importantly, the current
notebook was written for Python 3.10+, whereas the announced judge implements
3.9. Check newer APIs before relying on them; for example, `bin(x).count("1")`
is a portable replacement when `int.bit_count()` is unavailable. A successful
run under the host's newer CPython alone does not establish judge compatibility.

## Choose the right level of rehearsal

| Route | What it gives you | Important difference |
| --- | --- | --- |
| Proxmox VM with the official ICPC image | Full desktop/IDEs, browser or SPICE access from Windows/Linux, repeatable setup rehearsals | Verify NWERC-specific versions/session after boot; VM performance differs from contest hardware |
| This repo's Docker container | Ubuntu CLI, GCC, pinned PyPy, Bash, terminal editors, persistent files | No Xfce/IDE desktop; shared host kernel and hardware |
| WSL Ubuntu alone | A convenient Linux terminal on Windows | Installed Ubuntu/Python/GCC versions depend on that distro |
| Official ICPC image in a VM or on USB | Closest available desktop/editor baseline | NWERC modifies this image; the public image is not an exact NWERC clone |

The container checks GCC **13.3.0** and PyPy **7.3.15 / Python 3.9.18** when it is
built. Its Ubuntu base and apt package revisions can still change between builds.
For repeatable team sessions, keep the successfully built image and record its
image ID/digest. Timing inside WSL, Docker or a VM cannot reproduce the contest
hardware or DOMjudge's per-submission resource accounting.

## Optional container: Windows prerequisites

For the optional container, use Docker Desktop and a WSL2 Ubuntu distro. Check first
in PowerShell:

```powershell
wsl --list --verbose
docker version
docker compose version
```

If needed, follow [Microsoft's WSL installation guide](https://learn.microsoft.com/en-us/windows/wsl/install).
On this machine, the read-only inventory showed `Ubuntu` and `docker-desktop`
already installed as WSL2 distributions, both stopped. Docker's client is present,
but its Linux daemon was not running during this work. Start Docker Desktop,
select the WSL2 engine, and enable integration with the Ubuntu distro in
**Settings > Resources > WSL Integration**. See [Docker's WSL2 guide](https://docs.docker.com/desktop/features/wsl/).
We did not install or start services, change distro settings, or enable integration.

Open Ubuntu from Windows Terminal, or use `wsl -d Ubuntu`. Keep the repo and
practice files under the Linux filesystem, such as `~/projects/ICPC-Setup`, for
normal Linux file ownership and faster build access. You can edit those files
from Windows using a WSL-aware editor. Use Docker Desktop's integration rather
than independently installing a second Docker Engine inside the same WSL distro.

## Optional container: Linux prerequisites

Use a running Docker Linux engine with Compose v2. Ubuntu users can follow
[Docker's official installation instructions](https://docs.docker.com/engine/install/ubuntu/).
Check `docker version` and `docker compose version` before continuing. Docker
must work as your current user, because the commands below preserve the invoking
user's UID/GID for the writable host folder.

## Build and enter the practice shell

Run these commands from the repository root in Linux or WSL Bash:

```sh
mkdir -p workspaces
export PRACTICE_UID=$(id -u) PRACTICE_GID=$(id -g)
docker compose -f environment/compose.yaml config --quiet
docker compose -f environment/compose.yaml build
docker compose -f environment/compose.yaml run --rm practice
```

Use a regular user with UID/GID at least 1000. Keep those exported build values
when rebuilding. The service runs as a non-root user; `/workspace` is the writable
host `workspaces/` directory, and `/repo` is a read-only repository mount. Exiting
removes the session container while preserving all files in `workspaces/`.
No template or shell function is installed automatically. Session networking is
disabled; the image build itself needs network access for Ubuntu/PyPy downloads.

Inside the shell:

```sh
check-versions
```

`python3` intentionally points to the pinned **PyPy** in this container, so the
existing `rp` command exercises the judge's Python language level. The Ubuntu
CPython interpreter remains available separately as `/usr/bin/python3` for tools.
This aliasing is our practice choice, not a claim about NWERC's `/usr/bin/python3`.

### Rehearse setup from the printed guide

Start a fresh session and choose a new, empty contest folder:

```sh
mkdir -p rehearsal-01/tmpl
cd rehearsal-01
```

Now type the two templates into `tmpl/py.py` and `tmpl/cpp.cpp`, then
type `tmpl`, `rc` and `rp` from reference page 1 (plus the optional sample-test
block for `tp`/`tc`). Use `nano` or `vim` if
you want a terminal-only rehearsal. Do not source `/repo/setup.sh` in this mode.
Finally, check both languages:

```sh
tmpl
printf '42\n' | rp A
printf '42\n' | rc A
printf '42\n' > A/1.in
rp A 1
rc A 1
```

Each should print `42`. Open a second container session to rehearse entering the
terminal functions again; files persist, functions belong only to each shell.
For a new rehearsal, use another folder name instead of deleting old solutions.

### Convenient coding practice

When setup typing is not the exercise:

```sh
mkdir -p contest
cd contest
source /repo/setup.sh .
tmpl
```

The ZIP/package tool requires Python 3.10+ for its driver. In this container,
invoke it with `/usr/bin/python3 -m practice`; select `--python pypy3` when running
a Python solution. `PYTHONPATH=/repo` is already set. Run
`/usr/bin/python3 -m practice --help` for its commands. Imported test data can be
read through the read-only repo mount; write generated results under `/workspace`.
Run the driver from `/workspace` and see [problem-package practice](problem-packages.md)
for the inspect/import/run commands.

This container is for trusted local practice code. It is not the DOMjudge
sandbox, does not apply per-problem verdict/time/memory rules itself, and is not
intended for running arbitrary hostile submissions.

## Full desktop on Proxmox

Use the [Proxmox guide](proxmox-practice.md) for VM settings, raw-disk import,
toolchain verification, desktop access and clean snapshots. Proxmox imports the
decompressed `.img` directly; an intermediate qcow2 conversion is unnecessary.
The official image has not been downloaded or booted here. Confirm a working,
persistent Xfce desktop and the listed versions before freezing the base VM.

## Validation status

Compose configuration resolves successfully with the installed Docker client.
The Dockerfile includes version and standard-library/static-C++ smoke checks.
The official PyPy archive was downloaded, its SHA256 verified, and the bundled
module-builder paths and CFFI sources inspected before writing the Dockerfile.
Building/running the container remains unverified because the Docker daemon was
stopped. The official desktop image has not been downloaded or booted either.
Run the container build-time guards and verify the VM tools before relying on
toolchain parity.

The official PyPy archive is pinned to the SHA256 from
[PyPy's published checksum list](https://pypy.org/checksums.html):
`f062be307200bde434817e1620cebc13f563d6ab25309442c5f4d0f0d68f0912`.
The [PyPy 7.3.15 release notes](https://doc.pypy.org/release-v7.3.15.html) identify
its Python 3.9.18 language/runtime baseline.
