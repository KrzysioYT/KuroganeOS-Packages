# Official KuroganeOS Anvil mirror

The canonical package source remains this GitHub repository. KuroganeOS also exposes an HTTPS mirror from the project VPS so the operating system does not need to depend on GitHub raw URLs as its long-term public service endpoint.

## Public endpoint

```text
https://packages.kuroganeos.147-79-62-37.sslip.io
```

Catalog:

```text
https://packages.kuroganeos.147-79-62-37.sslip.io/index.kuro
```

Health check:

```text
https://packages.kuroganeos.147-79-62-37.sslip.io/healthz
```

The mirror serves the same repository-relative paths as `main`, including `index.kuro`, `packages/<name>/manifest.kpkg`, and package payload files. It also accepts the compatibility prefix `/KrzysioYT/KuroganeOS-Packages/main/` so existing Anvil base-path configuration can be moved between `raw.githubusercontent.com` and the VPS without changing manifest paths.

The mirror is synchronized automatically from `main`. GitHub remains the source of truth and recovery origin.

## Anvil configuration

Current KuroganeOS builds can point Anvil at the mirror with:

```text
HOST=packages.kuroganeos.147-79-62-37.sslip.io
BASE=/KrzysioYT/KuroganeOS-Packages/main
```

The project may switch release builds to this endpoint once the VPS health gate is green. Development builds can continue to use GitHub directly when needed.

## Custom domain

The `sslip.io` hostname is intentionally temporary and costs nothing. When a permanent KuroganeOS domain is available, only the service hostname changes; package paths and the KIDX1/KPKG1 contract remain stable.
