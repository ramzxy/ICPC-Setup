# NWERC CLI practice environment

The main practice route is now the [Proxmox desktop VM](../docs/proxmox-practice.md).
These container files are an optional CLI fallback; they do not provide a desktop.

Read [the setup and limitations](../docs/practice-environment.md) first.
Requires a running Docker Linux engine and Docker Compose v2.

From the repository root, in Linux or WSL Bash:

```sh
mkdir -p workspaces
export PRACTICE_UID=$(id -u) PRACTICE_GID=$(id -g)
docker compose -f environment/compose.yaml build
docker compose -f environment/compose.yaml run --rm practice
```

The persistent host directory `workspaces/` appears as `/workspace`. The repo
is read-only at `/repo`. Nothing automatically copies templates, defines
functions or changes startup files. Start a session with an empty problem folder
and use the printed guide; for convenience during coding practice:

```sh
check-versions
mkdir -p contest
cd contest
source /repo/setup.sh .
tmpl
printf '42\n' | rp A
printf '42\n' | rc A
```

The existing `rp` uses `python3`, which intentionally resolves to the judge's
PyPy/Python 3.9 here. The host driver for the archive tools requires Python
3.10+: invoke it as `/usr/bin/python3 -m practice`, then select `--python pypy3`
for the solution child. Run the driver from `/workspace` so its artifacts are
writable. See [problem-package practice](../docs/problem-packages.md) for commands.

This is a practice shell, not a DOMjudge worker or an untrusted-code sandbox.
Network access is disabled for sessions; Docker still downloads packages when
building the image. The Dockerfile checks the targeted compiler and interpreter
versions at build time. Ubuntu base layers and apt patch revisions are not digest
locked; after a successful build, save the resulting image digest for a stable
team-wide image. Use an x86-64 host for timing practice.
