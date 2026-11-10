# DOMjudge deployment groundwork

Read [the deployment guide](../../docs/domjudge.md) before applying anything.

- `kustomization.yaml`: k3s namespace, persistent MariaDB, DOMserver, private services.
- `secrets.env.example`: database credential names; real `secrets.env` is ignored.
- `judgehost.compose.yaml`: separate, privileged Linux VM judgehost example.
- `judgehost.env.example`: judge-to-server URL and CPU selection.

Render locally without contacting Kubernetes:

```sh
kubectl kustomize deploy/domjudge
```

No default passwords or public ingress are installed by these manifests.
The release tags are explicit; image digests and runtime behavior still need
verification on the deployment host. This is a starting point, not a tested
production installation.
