# KuroganeOS Packages

Official package repository and reference implementation for **Anvil**, the KuroganeOS package manager.

This repository is intentionally separate from the KuroganeOS kernel/source tree. Anvil downloads the catalog, package manifests and payloads over HTTPS from this repository.

## Current compatibility

The repository targets the current Anvil v1 wire formats:

- catalog: `KIDX1`
- package manifest: `KPKG1`
- default host: `raw.githubusercontent.com`
- default base: `/KrzysioYT/KuroganeOS-Packages/main`

The formats are deliberately small and deterministic. Current Anvil installs **one payload file per package**, resolves name-only dependencies recursively, verifies exact payload byte count and performs a `.new` / `.old` transactional replacement.

## Start here

- [Using Anvil](docs/USING_ANVIL.md)
- [Pointing KuroganeOS at another repository](docs/ADDING_REPOSITORY.md)
- [Creating your own Anvil repository](docs/CREATING_REPOSITORY.md)
- [Creating a package](docs/CREATING_PACKAGE.md)
- [KIDX1 repository format](docs/REPOSITORY_FORMAT.md)
- [KPKG1 package format](docs/PACKAGE_FORMAT.md)
- [Dependencies, peers and conflicts](docs/DEPENDENCIES.md)
- [Publishing and validation](docs/PUBLISHING.md)
- [Current limitations and roadmap](docs/ROADMAP.md)

## Repository layout

```text
index.kuro
packages/
  hello-text/
    manifest.kpkg
    payload/
      hello.txt
  _template/
    manifest.kpkg
examples/
  anvil.cfg
tools/
  build_index.py
  validate_repo.py
docs/
.github/workflows/
```

## Validate locally

```bash
python3 tools/validate_repo.py
python3 tools/build_index.py --check
```

To regenerate the root catalog from package manifests:

```bash
python3 tools/build_index.py --write
```

Every pull request is validated by GitHub Actions.

## Important security note

Anvil currently trusts the configured HTTPS repository and validates transport success plus payload byte count. **Package signing and payload hashes are not implemented yet.** Do not treat third-party repositories as cryptographically authenticated package sources. See [SECURITY.md](SECURITY.md).

## KuroganeOS

Main operating-system repository: `KrzysioYT/KuroganeOS`.
