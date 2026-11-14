# Private DOMjudge deployment

Verified on 11 November 2026. This is the first working installation, not a claim
that every cluster configuration or problem validator has been tested.

## Access and resources

- Dedicated Proxmox VM: Tailscale hostname `k3s-1`, guest hostname `k3s`.
- Website: `http://100.103.219.70:30080` over Tailscale, or
  `http://192.168.1.117:30080` on the LAN. Login username: `admin`.
- Initial admin password: `/root/domjudge-admin-password.txt` on that VM,
  mode 0600. Read it with `sudo cat`; its value is not stored in this repository.
- Public DNS/forwarding/certificates are deliberately not configured yet.
- k3s `v1.37.2+k3s1`, Debian 13, kernel `6.12.116+deb13-amd64`, containerd
  `2.3.7-k3s1`; 8 vCPUs and 16 GiB RAM.
- Helm release/namespace `domjudge`, chart 0.1.1, installed by the k3s Helm
  controller from an embedded chart package. Active config:
  `HelmChartConfig/kube-system/domjudge`, sourced from `domjudge-private.yaml`.
- One web pod, one MariaDB pod and one judgehost pod, all Ready with no restarts
  at verification. The MariaDB PVC is 8 GiB on `local-path`.
- Worker `domjudge-judgehost-1` is enabled, bound to CPU 1. Its privileged host
  PID/cgroup access was explicitly approved for this dedicated VM. New worker
  auto-activation remains disabled; an existing registered worker keeps its state.

The source for that configuration is
[`domjudge-private.yaml`](../deploy/domjudge/k3s/domjudge-private.yaml).
Do not also apply the raw Kustomize deployment against the same intended data.

## Language configuration and checks

The actual sandbox contains GCC **13.3.0** and PyPy **7.3.15** implementing Python
**3.9.18**. The default Python executable uses `pypy3`; `python3` is its DOMjudge
language ID, not evidence of a CPython runner.

The stock C++ compile script did not select GNU++20 or define the judge macros.
These database-persisted executable settings were changed through the authenticated
DOMjudge editor, with the original ZIPs backed up under `/root`:

- Executables → `cpp` → `run`: compile with
  `g++ -x c++ -std=gnu++20 -Wall -O2 -static -pipe -DONLINE_JUDGE -DDOMJUDGE`.
- Executables → `py3` → `run`: enable `export ONLINE_JUDGE=1 DOMJUDGE=1`
  in the generated runner script.

Those edits live in MariaDB, not in chart values. Repeat them for a new database,
or restore the SQL backup. The global memory limit is 2 GiB; individual problem
limits can override it. Time limits still require calibration on this VM.

A private **Installation check (C++ / Python)** contest and one addition problem
were added for verification. It contains eight intentionally labelled submissions:

| Check | Result |
| --- | --- |
| C++ addition across sample/secret cases | AC |
| Python addition asserting PyPy 3.9.18 | AC |
| Deliberately wrong output | WA |
| Python infinite loop | TLE |
| Deliberate C++ compile error | CE |
| Allocation exceeding a 128 MiB problem limit | RTE |
| C++ static assertion for C++20 and both judge macros | AC |
| Python assertions for both judge environment variables | AC |

For the memory case, the sandbox metadata reported exactly **134217728 bytes**
and exit 137/SIGKILL, demonstrating the 128 MiB cap. DOMjudge reports this as RTE,
not a separate MLE verdict. The fixture's 128 MiB override is deliberate; it is
not the global contest limit. The test contest is private, has no team login
password, and is separate from any real practice contest. It can be removed or
kept as deployment evidence.

The login page returned HTTP 200 from the node and from the Windows computer over
Tailscale. Admin API authentication passed. Config Checker reported no errors;
remaining warnings were connection/log-size tuning for larger contests, password
hash timing and a missing balloon color in the test fixture. The upstream web
image also prints a PHP `ds.so` startup warning; the UI, API, config check and
tested judging paths worked despite it.

## Recovery and next steps

- A verified initial SQL dump is saved outside the PVC at
  `/root/domjudge-verified-initial.sql.gz`, mode 0600. It includes account hashes,
  compiler-script edits and test data. This VM-local file is not an off-machine
  disaster-recovery backup; copy it privately elsewhere before serious use.
- Database Secret: `domjudge-database`; worker Secret: `domjudge-judgehost`.
  Retain the database Secret with the PVC. Regenerating it does not rotate the
  initialized MariaDB password.
- The admin password file and initial compiler ZIP backups are root-only VM files.
  A replacement web pod can show `[unknown]` in its generated initial-password
  file; the saved password and database account remain valid.
- Keep Helm controller management for this release; do not also run Helm CLI
  upgrades against it. Rebuild/reapply the embedded chart to update its source.
- Before enabling public HTTPS, update the active HelmChartConfig carefully:
  preserve `judgehost.enabled: true`, and keep or deliberately remove private
  NodePort access. The separate public example initially disables its worker and
  must not blindly replace this verified private configuration.
- The [NWERC 2025 practice set](nwerc2025-practice.md) was subsequently imported
  and checked with original jury solutions, including Hasty Haul's multi-pass
  protocol. The eight initial smoke checks above alone did not establish that.
  Other contest archives and untested languages still need their own verification.

Deployed image digests, recorded for repeatability:

| Image | Digest |
| --- | --- |
| `domjudge/domserver:9.0.1` | `sha256:7585ee1f265091d1e75bff571454b359a4413056bc3694ac88fe27bfa7e41560` |
| `domjudge/judgehost:9.0.1` | `sha256:8b0ff952445f80e6e7ec01ec050bf41b759dad3c153a5011179196ffe75c81de` |
| `mariadb:11.4.13` | `sha256:d1891ede5a559f1086f55053bb8cc1569b0298338c4d72d71725a7f74f6ba620` |
