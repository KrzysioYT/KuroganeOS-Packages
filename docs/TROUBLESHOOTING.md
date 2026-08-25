# Troubleshooting Anvil repositories

## `ANVIL / HTTPS FETCH FAILED`

Check NIC/DHCP/DNS first, then TLS trust and the configured `HOST`/`BASE`. The repository must be public and reachable without authentication.

## `ANVIL / EMPTY OR INVALID CATALOG`

Verify `/index.kuro` starts with `KIDX1` and package records use:

```text
pkg|name|version|description|/manifest/path
```

## `ANVIL / INVALID PACKAGE MANIFEST`

The manifest must start with `KPKG1` and include valid `name`, `version`, `destination`, `payload` and positive numeric `bytes`.

## `ANVIL / INDEX MANIFEST MISMATCH`

The catalog name/version differs from the referenced manifest. Regenerate `index.kuro`.

## `ANVIL / PAYLOAD SIZE MISMATCH`

`bytes=` does not equal the downloaded HTTP body size. Recalculate the payload size and republish the manifest.

## `ANVIL / DEPENDENCY FAILED`

A normal dependency is missing from the current catalog or failed to install.

## `ANVIL / MISSING PEER DEPENDENCY`

A package listed in `peer=` is not present in `/home/anvil.db`.

## `ANVIL / PACKAGE CONFLICT`

A package listed in `conflicts=` is already installed.

## `ANVIL / PACKAGE ALREADY INSTALLED`

Current v1 installed-state detection is name based. Upgrade/remove semantics are not implemented yet.

## Validate the repository outside KuroganeOS

```bash
python3 tools/validate_repo.py
python3 tools/build_index.py --check
```
