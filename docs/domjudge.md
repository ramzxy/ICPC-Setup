# DOMjudge practice hosting

Research checked 10 November 2026. These files are deployment groundwork; nothing
has been applied to your cluster. The current stable release is DOMjudge **9.0.1**.
The examples pair its server and judgehost images with MariaDB **11.4.13**.
These are explicit release tags, not immutable digests; verify image availability,
architecture and digests before deployment. Registry access and a running server
were unavailable in this session. [DOMjudge downloads](https://www.domjudge.org/download),
[MariaDB maintenance releases](https://mariadb.org/mariadb-server-12-3-11-8-11-4-and-10-11-q3-2026-maintenance-releases-and-goodbye-10-6/).

## Deployment shape

Use k3s for the DOMserver web/API and persistent database. Run the actual judgehost
on a dedicated Linux VM with at least two vCPUs, separately from shared cluster
workloads. The judgehost needs only HTTP(S) access to the DOMserver; it does not
need a database connection. This separates the practice website from the machine
executing submissions. [DOMjudge architecture](https://www.domjudge.org/docs/manual/9.0/install-domserver.html).

The official judgehost container requires privileged mode, the host cgroup
namespace, and a writable `/sys/fs/cgroup` mount. A normal Kubernetes Deployment
does not reproduce that contract. This repository deliberately provides the
upstream Docker-style judgehost approach for a dedicated VM, rather than an
untested privileged pod on your k3s server. For bare-metal installation, use the
[official judgehost manual](https://www.domjudge.org/docs/manual/9.0/install-judgehost.html).
The [official container instructions](https://github.com/DOMjudge/domjudge-packaging/blob/main/docker/README.md#judgehost-container)
explain the container requirements.

## k3s web and database

The manifests use `linux/amd64` node selectors, one DOMserver replica, and an 8 GiB
MariaDB PVC. Before first deployment, choose a suitable StorageClass and capacity
in `mariadb.yaml`. An omitted `storageClassName` uses the cluster default.
K3s normally supplies local-path storage; that is local node storage, not a
replicated database or a backup. [K3s storage](https://docs.k3s.io/storage/),
[Kubernetes volumes](https://kubernetes.io/docs/concepts/storage/persistent-volumes/).

From the repository root in Bash, after selecting the intended kubeconfig/context:

```sh
kubectl kustomize deploy/domjudge                    # render only, offline
cp deploy/domjudge/secrets.env.example deploy/domjudge/secrets.env
chmod 600 deploy/domjudge/secrets.env
# Edit secrets.env: set two different random passwords, replacing both examples.
# The following commands actually deploy; run only when ready.
kubectl apply -f deploy/domjudge/namespace.yaml
kubectl -n domjudge create secret generic domjudge-database \
  --from-env-file=deploy/domjudge/secrets.env
kubectl apply -k deploy/domjudge
kubectl -n domjudge rollout status statefulset/mariadb
kubectl -n domjudge rollout status deployment/domserver
kubectl -n domjudge port-forward service/domserver 8080:80
```

Open `http://localhost:8080`. Both services are ClusterIP only. Port-forward is
for the first admin setup; it exposes localhost on the machine running kubectl,
not a URL reachable by an external judgehost. Before attaching that worker,
provide a stable private/VPN address or a TLS ingress reachable from the judge VM.
No public hostname, ingress, certificate issuer or load balancer is assumed here.
[Kubernetes port forwarding](https://kubernetes.io/docs/tasks/access-application-cluster/port-forward-access-application-cluster/).

The official DOMserver entrypoint creates or upgrades the database. Keep a single
replica and use matching server/worker versions. First-start demo data are enabled
for a simple smoke test. Once it is running, retrieve the initial admin password
privately and change it in the UI:

```sh
kubectl -n domjudge exec deployment/domserver -- \
  cat /opt/domjudge/domserver/etc/initial_admin_password.secret
```

The database PVC persists accounts and contest data. Generated credential files
inside the web container are ephemeral: after pod replacement, an initial-password
file can say `[unknown]` while the real account password remains in the database.
Save the admin password in your password manager. Keep new judgehosts paused by
setting `judgehost_activated_by_default` to false before connecting a worker.
For the worker, set a known random password on an account with the `judgehost` role through the admin UI;
use that same value on the judge VM. Do not assume a newly generated `restapi.secret`
file matches an existing database. [Official startup logic](https://github.com/DOMjudge/domjudge-packaging/blob/main/docker/domserver/scripts/start.d/50-domjudge.sh).

Changing the environment secret does not change an initialized MariaDB account
password. Rotate it with SQL and update both applications together. The official
container initialization variables only apply to a new data directory.
[MariaDB image behavior](https://hub.docker.com/_/mariadb).

## Dedicated Linux judgehost

Use a recent Linux kernel with working cgroups. For cgroup v2, DOMjudge 9 documents
Linux 5.19/6.0 or newer for peak-memory reporting. Check CPU IDs with `lscpu` and
`cat /sys/devices/system/cpu/online`; the example uses daemon/CPU 1. Run only one
judgehost initially, then calibrate time limits on this hardware.
[Judgehost requirements](https://www.domjudge.org/docs/manual/9.0/install-judgehost.html#linux-control-groups).

On that VM, with Docker Engine and Compose already installed:

```sh
cp deploy/domjudge/judgehost.env.example deploy/domjudge/judgehost.env
chmod 600 deploy/domjudge/judgehost.env
# Edit the URL (no /api suffix), account password and available CPU ID.
# This real credential file is ignored by Git; do not commit it.
docker compose --env-file deploy/domjudge/judgehost.env \
  -f deploy/domjudge/judgehost.compose.yaml config --quiet
docker compose --env-file deploy/domjudge/judgehost.env \
  -f deploy/domjudge/judgehost.compose.yaml up -d
```

The password is passed using the official image's `JUDGEDAEMON_PASSWORD` setting.
Keep the real env file private; use `config --quiet` when validating real secrets.
The privileged container can affect its VM, so give it a dedicated VM rather
than sharing a k3s/control-plane host. Verify that the worker appears in the admin
Judgehosts page and run Config Checker. The packaging README's language list is
incomplete for Python: check the interpreters **inside `/chroot`**, not just in
the outer container. Enable C++ and the desired Python/PyPy compile scripts in the
UI only after those runtimes are available. Language support is configured through
DOMjudge executables; installing Python in the outer image alone is insufficient.
[Chroot setup](https://www.domjudge.org/docs/manual/9.0/install-judgehost.html#creating-a-chroot-environment),
[language configuration](https://www.domjudge.org/docs/manual/9.0/config-advanced.html#programming-languages).

The official 9.0.1 worker image is not a pinned NWERC toolchain. The published
NWERC 2026 targets are GCC **13.3.0** with `-std=gnu++20`, and **PyPy 7.3.15**
running Python **3.9.18**, with 2 GiB for the whole submitted program/runtime.
[Official NWERC systems](https://2026.nwerc.eu/system).

Before allowing practice submissions, pause the worker in the admin UI and inspect
its actual chroot with the installed `dj_run_chroot` helper:

```sh
docker compose --env-file deploy/domjudge/judgehost.env \
  -f deploy/domjudge/judgehost.compose.yaml exec judgehost \
  /opt/domjudge/judgehost/bin/dj_run_chroot
# Inside the chroot shell:
g++ --version
pypy3 --version
python3 --version
exit
```

For the first smoke test, enable only C++ and the interpreter actually installed.
For NWERC parity, a custom judgehost chroot/image must install the exact GCC and
PyPy versions; configure its C++ compile executable with the published static
GNU++20 flags and its Python executable to use `pypy3` for compilation/execution.
CPython `python3` does not give PyPy parity. Set the contest memory limit to 2 GiB
using the UI's displayed units. Disable unused languages, test accepted/rejected
submissions, then enable the worker. Use the local practice container from
[the environment guide](practice-environment.md) for matching compiler/interpreter
practice now; building and testing an equally pinned judgehost is a separate step.
Hardware timing still requires calibration on the judge VM.

## Using the supplied archive

The supplied `solutions.zip` contains a complete NWERC 2025 problem set: 12 nested
problem ZIPs, test data, jury solutions, validators, statements and metadata.
Use [the package tooling](problem-packages.md) to inspect/import it locally.
For DOMjudge, upload the **individual original problem ZIPs** under Problems and
associate them with a practice contest. Keep their custom output validators and
flags intact; do not replace them with plain answer-file comparison.
[Supported problem ZIP format](https://www.domjudge.org/docs/manual/9.0/problem-format.html).

The supplied outer ZIP is about 426 MiB; its largest individual problem ZIP is
about 167 MiB. Before uploading, check PHP, Nginx and any ingress request-size
and timeout limits against those sizes. Upload individual problems, not the
entire contest bundle. The twelve problems expand to about 1.2 GiB before database
and backup overhead; size the database volume accordingly.

There is an important version gate: `hastyhaul` uses multi-pass validation.
The 9.0 stable manual does not provide that workflow; the upstream enhancement is
[still tracked separately](https://github.com/DOMjudge/domjudge/issues/2307), while
[multi-pass instructions are in the 10.0 development manual](https://www.domjudge.org/snapshot/manual/config-advanced.html#multi-pass-problems).
Treat that problem as blocked on a tested compatible DOMjudge release/build.
Do not strip the multi-pass flag or treat an ordinary run as an accepted verdict.

Create an admin-associated jury team if you want bundled jury solutions submitted
automatically. Before using the set for practice, inspect the import warnings,
verify validator behavior, and use Judging Verifier to check expected accepted,
wrong-answer and time-limit submissions. A green plain-output smoke check alone
is not equivalent to correct custom-validator judging.
[Jury-solution verification](https://www.domjudge.org/docs/manual/9.0/config-basic.html#testing-jury-solutions).

## Backups and remaining deployment choices

Before importing a large archive or changing versions, take a database dump and
store it outside the PVC. For example, from Bash on the admin machine:

```sh
umask 077
kubectl -n domjudge exec statefulset/mariadb -- sh -c \
  'MYSQL_PWD="$MARIADB_ROOT_PASSWORD" mariadb-dump -u root --single-transaction --routines --events domjudge' \
  > deploy/domjudge/domjudge-backup.sql
```

This dump contains contest data and account hashes; store it privately and test a
restore into a separate database. A PVC or a k3s namespace deletion is not a backup.
Custom web assets/configuration require separate persistence/backup if you add them.

Before real deployment, resolve the cluster architecture, persistent storage and
backup location, judge VM placement/cgroup support, HTTPS/private routing, and
which tested backend version handles the archive's multi-pass problem. Image
pulls, bootstrap, submission judging and restarts must still be tested on that host.
