# DOMjudge on a fresh k3s VM

This walkthrough uses the default k3s Traefik and local-path storage, with
`domjudge.rickvm.dev` as the public hostname. Run the Linux commands on the k3s
VM, from a copy of this repository. No commands have been run on your server by
this repository. The manifests have only been checked locally.

## k3s in a minute

K3s packages Kubernetes, its container runtime, cluster networking, DNS and some
useful defaults. One server VM is already a complete cluster; Docker Engine is
not required for its pods. You describe the desired applications in YAML, and
Kubernetes starts them, checks them and replaces failed containers.

| Term | Meaning here |
| --- | --- |
| Node | Your k3s VM |
| Pod | A running application container, sometimes with helper containers |
| Deployment | Keeps the DOMserver pod running |
| StatefulSet | Manages the MariaDB pod and its stable storage claim |
| Service | A stable internal address such as `domserver` or `mariadb` |
| Ingress | Routes a public hostname through Traefik to the web Service |
| PVC | Persistent database storage, separate from the container |
| Secret | Database credentials consumed by the pods |
| Namespace | The `domjudge` group of resources |

`kubectl apply` installs or updates the declared resources. Editing a local YAML
file alone does not update the cluster. One VM has no failover if that VM or its
disk fails. Local-path storage survives pod restarts, but is not a backup.

Sources: [k3s quick start](https://docs.k3s.io/quick-start),
[networking defaults](https://docs.k3s.io/networking/networking-services),
[Kubernetes components](https://kubernetes.io/docs/concepts/overview/components/).

## 1. Check the node

This shell helper lasts only for the current terminal:

```sh
k() { sudo k3s kubectl "$@"; }
k get nodes -o wide
k get storageclass
k -n kube-system get pods
k get ingressclass
```

Expect a Ready amd64 Linux node, a default `local-path` StorageClass, and running
Traefik. The cert-manager manifest pins v1.21.3, documented for Kubernetes
1.33-1.36. Check the node version before installing; choose a supported matching
release if it falls outside that range. The DOMjudge base requests an 8 GiB
database volume and can use up to 4 GiB RAM across its two application containers;
leave room for k3s, judging and imports. Start with at least 4 vCPUs and 8 GiB RAM
for a small combined practice VM, then measure actual use.

## 2. Start the private web server and database

For a fresh installation, create a local credential file without overwriting an
existing one. Install `openssl` if it is not already available:

```sh
(
  set -eC
  umask 077
  db_root_password=$(openssl rand -hex 32)
  db_password=$(openssl rand -hex 32)
  printf 'DB_ROOT_PASSWORD=%s\nDB_PASSWORD=%s\n' \
    "$db_root_password" "$db_password" \
    > deploy/domjudge/secrets.env
)
k apply -f deploy/domjudge/namespace.yaml
k -n domjudge create secret generic domjudge-database \
  --from-env-file=deploy/domjudge/secrets.env
k apply -k deploy/domjudge
k -n domjudge rollout status statefulset/mariadb --timeout=10m
k -n domjudge rollout status deployment/domserver --timeout=10m
```

Stop if credential creation fails unexpectedly. If the Secret already exists,
keep its existing passwords rather than regenerating them. Changing the Secret
does not rotate an initialized database password.

The initial deployment uses DOMjudge 9.0.1 and creates demo data. Retrieve the
admin password privately:

```sh
k -n domjudge exec deployment/domserver -- \
  cat /opt/domjudge/domserver/etc/initial_admin_password.secret
k -n domjudge port-forward service/domserver 8080:80
```

Leave port-forward running. On your own computer, open a second terminal:

```sh
ssh -N -L 8080:127.0.0.1:8080 YOUR_USER@YOUR_K3S_VM
```

Visit `http://localhost:8080`. Sign in as `admin`, save/change the admin password,
set a new random password for the `judgehost` account, and disable/delete demo
accounts you will not use before publishing. Set
`judgehost_activated_by_default` to false while configuring the worker. Save
credentials outside the web pod; it can be replaced.

## 3. DNS and public HTTPS

1. Create a DNS **A** record for `domjudge.rickvm.dev` pointing at your public IPv4
   address. Start with DNS-only if your DNS provider offers a proxy toggle.
2. Forward TCP **80 and 443** on your router to the k3s VM's LAN address. Allow
   those ports in any VM/Proxmox firewall. Port 80 is used for certificate
   validation; port 443 serves the site.
3. Publish an AAAA record only if IPv6 actually reaches this same ingress.
   Check that the hostname resolves correctly from inside the VM too; HTTP-01
   self-checks may require working NAT loopback or suitable split DNS.

If inbound connections are impossible (for example CGNAT), this HTTP-01 recipe
needs a different access plan; repeatedly applying manifests will not fix it.

Install cert-manager through k3s's built-in Helm controller; no separate Helm
client is needed. Do not install a second copy if the cluster already has one:

```sh
k apply -f deploy/domjudge/k3s/cert-manager.yaml
# The Helm job creates the deployments asynchronously. Check until they exist:
k -n kube-system get jobs
k -n cert-manager get deployments
# Once those deployments exist:
k -n cert-manager rollout status deployment/cert-manager --timeout=5m
k -n cert-manager rollout status deployment/cert-manager-webhook --timeout=5m
k -n cert-manager rollout status deployment/cert-manager-cainjector --timeout=5m
k apply -f deploy/domjudge/public/https.yaml
k -n domjudge get issuer,certificate,ingress
k -n domjudge wait --for=condition=Ready certificate/domserver-tls --timeout=10m
```

Open `https://domjudge.rickvm.dev`. Traefik redirects ordinary HTTP requests to
HTTPS; cert-manager serves the more specific HTTP-01 challenge path separately
and renews certificates automatically. An email address can optionally be added
to the Issuer's ACME settings. Do not publish until private account setup is
complete.

These two optional manifests are deliberately outside the base kustomization,
so `kubectl apply -k deploy/domjudge` alone still exposes no public service.
Behind Traefik, configure DOMserver `TRUSTED_PROXIES` for the actual proxy
addresses/CIDRs if correct forwarded scheme/client IP reporting is needed; do
not blindly trust every internet address. See
[DOMjudge proxy settings](https://www.domjudge.org/docs/manual/9.0/install-domserver.html#running-behind-a-proxy-or-loadbalancer).

Sources: [k3s Helm controller](https://docs.k3s.io/add-ons/helm),
[cert-manager installation](https://cert-manager.io/docs/installation/helm/),
[supported versions](https://cert-manager.io/docs/releases/),
[HTTP-01 configuration](https://cert-manager.io/docs/configuration/acme/http01/),
[Traefik Ingress](https://doc.traefik.io/traefik/reference/routing-configuration/kubernetes/ingress/).

## 4. Add actual judging

The web server can accept submissions but cannot execute them until a judgehost
is connected. The existing worker file is `deploy/domjudge/judgehost.compose.yaml`;
it is a Docker Compose service, not a Kubernetes workload.

A separate worker VM is the existing documented approach, not a requirement of
the HTTP API. Keeping everything on the current dedicated VM is an option, but
the official worker needs privileged mode, the host cgroup namespace and writable
host cgroups. A writable mount alone does not provide the host cgroup namespace.
Do not claim an ordinary privileged pod is equivalent without verifying the
runtime and actually judging submissions. Co-located judging also competes with
web/database activity, so timing will need calibration.

For now complete web bootstrap and HTTPS, then inspect the node's OS, CPU count
and cgroup setup before choosing a tested worker integration. The separate-VM
Compose route remains available if that proves simpler. Follow the worker,
Python/C++ toolchain, problem import and backup steps in [the full hosting
guide](domjudge.md). A healthy website alone is not a functioning judge, and the
stock judgehost image is not automatically an exact NWERC GCC/PyPy environment.

## Useful checks

```sh
k -n domjudge get pods,pvc,svc,ingress
k -n domjudge logs deployment/domserver --tail=80
k -n domjudge logs statefulset/mariadb --tail=80
k -n domjudge get events --sort-by=.metadata.creationTimestamp
k -n domjudge describe certificate domserver-tls
k -n domjudge get orders,challenges
```

Keep the database PVC and back it up before importing a contest or upgrading.
Deleting a namespace can delete its PVCs and local-path data. Import the original
individual problem ZIPs, not the outer `solutions.zip`; the multi-pass problem
still requires a compatible tested judge version. See the full guide for those
limitations.
