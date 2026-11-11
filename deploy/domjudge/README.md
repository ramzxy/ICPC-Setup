# DOMjudge deployment groundwork

Read [the deployment guide](../../docs/domjudge.md) before applying anything.
For the default k3s installation and `domjudge.rickvm.dev`, start with
[the k3s walkthrough](../../docs/k3s-domjudge.md).

- `kustomization.yaml`: k3s namespace, persistent MariaDB, DOMserver, private services.
- `secrets.env.example`: database credential names; real `secrets.env` is ignored.
- `judgehost.compose.yaml`: separate, privileged Linux VM judgehost example.
- `judgehost.env.example`: judge-to-server URL and CPU selection.
- `k3s/cert-manager.yaml`: optional cert-manager install through k3s's Helm controller.
- `public/https.yaml`: optional Traefik ingress and Let's Encrypt issuer for
  `domjudge.rickvm.dev`; apply separately after private account setup.

Render locally without contacting Kubernetes:

```sh
kubectl kustomize deploy/domjudge
```

No default passwords or public ingress are installed by the base kustomization.
The release tags are explicit; image digests and runtime behavior still need
verification on the deployment host. This is a starting point, not a tested
production installation.
