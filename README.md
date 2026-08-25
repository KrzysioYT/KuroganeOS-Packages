# KuroganeOS Packages

Official **Anvil** package repository for KuroganeOS.

Anvil consumes this repository directly over HTTPS from:

```text
raw.githubusercontent.com/KrzysioYT/KuroganeOS-Packages/main
```

## Installing packages

Normal users do **not** build packages manually.

Open **Anvil** inside KuroganeOS, select a package and choose **INSTALL SELECTED**. Anvil downloads the manifest/payload, resolves dependencies and installs transactionally.

For the standard utility set install only:

```text
kuro-toolkit
```

It pulls the core tools automatically.

## Current utility family

- `kurofetch` — compact system/network overview;
- `kuro-netinfo` — NIC, DHCP, addressing and traffic;
- `kuro-uptime` — uptime and scheduler load;
- `kuro-meminfo` — total/used/free memory;
- `kuro-webcheck` — real HTTPS/TLS connectivity test;
- `kuro-audioinfo` — audio/mixer status;
- `kuro-apps` — list `/apps` entries;
- `kuro-health` — one-shot health summary;
- `kuro-toolkit` — dependency meta-package installing the core set.

## Repository automation

Official Ring-3 packages are built from source by GitHub Actions. CI generates the ELF payloads, `manifest.kpkg`, exact `bytes=` values and `index.kuro`. See [docs/AUTOMATED_PACKAGES.md](docs/AUTOMATED_PACKAGES.md).

## Documentation

Start with [docs/START_HERE.md](docs/START_HERE.md). The docs include using Anvil, adding another repository, creating repositories/packages, `KIDX1`/`KPKG1`, dependencies, versioning, publishing, security, troubleshooting and maintainer workflows.

The package protocol is intentionally kept compatible with the current KuroganeOS Anvil client. Format evolution must be coordinated with the OS repository.
