# DOMjudge Helm chart

Local chart for DOMjudge **9.0.1**, MariaDB **11.4.13**, persistent database storage,
optional Traefik HTTPS and an **experimental, disabled-by-default judgehost pod**.
There are no chart dependencies and no default database passwords. The default
installation is private and has no demo contest data.

The web/database and opt-in worker were deployed and tested on the dedicated
Debian 13 k3s VM on 11 November 2026. See [the deployment record](../../../docs/domjudge-deployment.md)
for the tested versions, verdicts and database-level language configuration.
Other hosts still need their own runtime checks. Use a fresh installation. Do not deploy it alongside the old
`deploy/domjudge` Kustomize resources and expect it to adopt their database.
Back up/migrate existing data deliberately if you already deployed those.

## First installation

For **k3s without installing a Helm CLI on the server**, use the packaged route
below. Otherwise use the Helm CLI steps in this section. Pick one route; do not
manage the same release with both Helm CLI and a k3s HelmChart resource.

Run from the repository root on the k3s VM. Install the Helm CLI if necessary
using [the official instructions](https://helm.sh/docs/v3/intro/install/):

```sh
curl -fsSL https://raw.githubusercontent.com/helm/helm/main/scripts/get-helm-3 | bash
```

Create the namespace and credentials once (requires `openssl`). Keep existing
Secrets on later upgrades; do not generate replacement passwords for an existing
database:

```sh
sudo k3s kubectl create namespace domjudge
(
  set -e
  db_root_password=$(openssl rand -hex 32)
  db_password=$(openssl rand -hex 32)
  sudo k3s kubectl -n domjudge create secret generic domjudge-database \
    --from-literal=DB_ROOT_PASSWORD="$db_root_password" \
    --from-literal=DB_PASSWORD="$db_password"
)
sudo helm upgrade --install domjudge ./deploy/charts/domjudge \
  --kubeconfig /etc/rancher/k3s/k3s.yaml \
  --namespace domjudge --wait --timeout 10m
```

The credentials stay outside Helm values/releases. Retrieve and save the initial
admin password, then establish a private first login:

```sh
sudo k3s kubectl -n domjudge exec deployment/domjudge-domserver -- \
  cat /opt/domjudge/domserver/etc/initial_admin_password.secret
sudo k3s kubectl -n domjudge port-forward service/domjudge-domserver 8080:80
```

For an SSH-only VM, forward that port from your own computer in another terminal:
`ssh -N -L 8080:127.0.0.1:8080 rick@k3s-1`. Visit `http://localhost:8080`, log in
as `admin`, save/change the password and set a known random password on the
`judgehost` account. Set `judgehost_activated_by_default` to false until worker
checks pass. If a pod replacement makes the initial password file say `[unknown]`,
the existing database password still applies; the file is not a password reset.

## Packaged k3s installation (no Helm CLI on the server)

On the development computer, with Helm installed, build the chart and embedded
installer:

```sh
python3 deploy/package-domjudge.py
```

This writes `.build/domjudge-release/domjudge-0.1.1.tgz` and
`.build/domjudge-release/domjudge-k3s.yaml`. Copy the YAML installer onto the k3s
VM. Create the namespace and database Secret using the same commands above, then:

```sh
sudo k3s kubectl apply -f domjudge-k3s.yaml
sudo k3s kubectl -n kube-system get jobs
sudo k3s kubectl -n domjudge get pods -w
```

The bundled Helm controller installs the embedded chart; no chart repository,
Docker Engine or separate Helm CLI is needed on the VM. The pods appear after
the Helm job starts. Wait for both database and DOMserver to be Running/Ready,
then Ctrl+C and retrieve the admin password/port-forward as above.

For HTTPS after DNS, private account setup and the Traefik ACME configuration:

```sh
sudo k3s kubectl apply -f deploy/domjudge/k3s/traefik-acme.yaml
sudo k3s kubectl apply -f deploy/domjudge/k3s/domjudge-public.yaml
```

Both files must be copied from the repository if only the installer was uploaded.
Edit the public HelmChartConfig's `valuesContent` to retain/change settings. Enable
the worker there only after creating its matching Secret as described below.
On chart updates rebuild and reapply `domjudge-k3s.yaml`; the separate config and
credentials stay in the cluster. `failurePolicy: abort` preserves a failed install
for inspection. Check the Helm install Job's logs and HelmChart status before
retrying; do not delete the database or regenerate its passwords.

## Public HTTPS on domjudge.rickvm.dev

This route uses **Traefik's own ACME resolver**, avoiding a separate cert-manager
installation. The older cert-manager example in `deploy/domjudge/k3s` is an
alternative, not a prerequisite. Do not use both public recipes for the same
hostname. With Kubernetes 1.37, do not blindly install that older cert-manager
pin whose documented support only reaches Kubernetes 1.36.

1. Set DNS A `domjudge.rickvm.dev` to your public IPv4 (DNS-only initially).
2. Forward TCP 80/443 to the k3s VM and allow them through its firewalls.
3. Publish AAAA only when IPv6 actually routes to the same ingress.
4. Check the actual pod CIDR with `sudo k3s kubectl get nodes -o wide` and
   `sudo k3s kubectl get nodes -o jsonpath='{.items[*].spec.podCIDR}'`.
   Adjust `domserver.trustedProxies` in `values-rickvm.yaml` if it is not the
   default k3s `10.42.0.0/16`.

After private account setup:

```sh
sudo k3s kubectl apply -f deploy/domjudge/k3s/traefik-acme.yaml
# The k3s Helm controller updates Traefik asynchronously; inspect its Helm job:
sudo k3s kubectl -n kube-system get jobs,pods,pvc
sudo k3s kubectl -n kube-system rollout status deployment/traefik --timeout=5m
sudo helm upgrade --install domjudge ./deploy/charts/domjudge \
  --kubeconfig /etc/rancher/k3s/k3s.yaml --namespace domjudge \
  -f deploy/charts/domjudge/values-rickvm.yaml --wait --timeout 10m
```

Visit `https://domjudge.rickvm.dev`. Certificate issuance is asynchronous; check
`sudo k3s kubectl -n kube-system logs deployment/traefik --tail=80` if it still
serves a default certificate. HTTP-01 requires externally reachable port 80;
CGNAT or incorrect DNS needs a different network plan. The Traefik configuration
adds a persistent certificate store and keeps one replica. Its Recreate rollout
briefly interrupts other sites using that same proxy; it is intended for this
fresh single-node cluster. Back up its certificate PVC as well as MariaDB.

The HTTP redirect is scoped to this app. No public database Service is created.
The web pod trusts the configured proxy CIDR, which should contain only trusted
cluster proxies/workloads. Inspect PHP/nginx upload limits before importing the
large problem archives; ingress alone does not change those limits.

## Optional worker on this dedicated k3s VM

`judgehost.enabled=true` starts one worker pod. This opt-in integration has passed
live C++/Python submissions on the recorded dedicated k3s VM. It uses privileged mode, host PID access,
`nsenter` into the host cgroup namespace, and writable host cgroups. It can affect
the whole node. Use a dedicated practice VM, not a cluster containing unrelated
workloads. The default daemon binds CPU 1, so the node needs at least two usable
logical CPUs and working cgroup v2. The image must contain `nsenter` and the
upstream `/scripts/start.sh`; startup checks fail explicitly if prerequisites are
missing. Do not enable multiple releases' workers with the same CPU ID on one node.

Set the `judgehost` account password in DOMjudge first. Then create its matching
Secret without placing the password in shell history (Bash):

```sh
read -rsp 'Judgehost account password: ' judge_password; echo
printf '%s' "$judge_password" | sudo k3s kubectl -n domjudge create secret generic \
  domjudge-judgehost --from-file=JUDGEDAEMON_PASSWORD=/dev/stdin
unset judge_password
```

Add the worker setting to your chosen values file so subsequent upgrades retain
it. For `values-rickvm.yaml`, add:

```yaml
judgehost:
  enabled: true
  daemonId: 1
```

Run the same Helm upgrade as above, then:

```sh
sudo k3s kubectl -n domjudge logs deployment/domjudge-judgehost --tail=100
sudo k3s kubectl -n domjudge exec -it deployment/domjudge-judgehost -- \
  /opt/domjudge/judgehost/bin/dj_run_chroot
# Inside the judge chroot:
g++ --version
python3 --version
pypy3 --version
exit
```

Check that the worker registers in the admin UI, run Config Checker, configure
only the available C++/Python languages, and check actual accepted, wrong-answer,
time-limit and memory-limit submissions before activating it. Kubernetes Ready
does not prove correct judging; no synthetic readiness probe pretends otherwise.
The stock image is not guaranteed to contain NWERC's exact GCC/PyPy versions.
Co-located web/database work also affects timings. The existing dedicated-VM
Docker Compose worker remains a fallback if the cluster worker fails its checks.

For private LAN/Tailscale access without public ingress, the controller route can
use `deploy/domjudge/k3s/domjudge-private.yaml` after its worker Secret exists.
It enables the privileged worker and exposes the website on NodePort 30080.
The database remains internal. No router forwarding or certificate setup is
needed. In this mode the web Service is reachable on both the VM's LAN and
Tailscale addresses; access is governed by the respective network/firewall rules.

## Storage, upgrades and checks

- Default PVC: 8 GiB, cluster default StorageClass, retained on Helm uninstall.
  An existing PVC can be selected with `database.persistence.existingClaim`.
- Keep the external database Secret whenever retaining the database PVC.
  Changing a Secret does not rotate an initialized MariaDB password.
- Take and test an external SQL backup before importing problems or upgrading.
  Database upgrades are not automatically undone by a Helm rollback.
- Server and worker image versions must match. Keep one server replica during
  database initialization/upgrades. Do not change the default database user/name
  on an existing volume without a deliberate SQL migration.
- Helm uninstall retains the database PVC, but namespace deletion can still
  delete it. The chart does not supply backups or high availability.

```sh
helm lint deploy/charts/domjudge --strict
helm template domjudge deploy/charts/domjudge --namespace domjudge
helm template domjudge deploy/charts/domjudge --namespace domjudge \
  -f deploy/charts/domjudge/values-rickvm.yaml --set judgehost.enabled=true
```

See [the hosting guide](../../../docs/domjudge.md) for original problem ZIP imports,
custom validators and language parity checks. The installed build has passed
NWERC 2025 Hasty Haul's actual multi-pass protocol; see
[the practice record](../../../docs/nwerc2025-practice.md). The chart is not an
official DOMjudge Helm distribution.

Sources: [official images and worker contract](https://github.com/DOMjudge/domjudge-packaging/tree/main/docker),
[k3s Helm configuration](https://docs.k3s.io/add-ons/helm),
[Traefik ACME](https://doc.traefik.io/traefik/reference/install-configuration/tls/certificate-resolvers/acme/),
[Traefik chart values](https://github.com/traefik/traefik-helm-chart/blob/master/traefik/values.yaml).
