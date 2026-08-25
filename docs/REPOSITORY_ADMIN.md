# Repository maintainer guide

## Source of truth

Package manifests and payload files under `packages/` are the source of truth. `index.kuro` is generated metadata and must stay committed because Anvil fetches it directly.

## Before merge

Run:

```bash
python3 tools/validate_repo.py
python3 tools/build_index.py --check
```

If the index is stale:

```bash
python3 tools/build_index.py --write
```

## What CI protects

The validator checks current Anvil capacity/format constraints, required fields, exact payload size, dependency availability, catalog/manifest agreement and the current HTTP payload ceiling.

It does not prove a binary payload is safe, non-malicious or correctly licensed. Human review is still required.

## Review priorities

1. provenance and redistribution rights;
2. destination path and overwrite risk;
3. dependency graph;
4. payload behavior/security;
5. compatibility with the currently released KuroganeOS ABI;
6. reproducibility from source where practical.

## Catalog capacity

Current Anvil retains at most 12 packages. Do not silently exceed this limit. When the official repository grows beyond it, the KuroganeOS client must gain pagination/larger catalog support first.

## Format evolution

Never change `KIDX1`/`KPKG1` incompatibly on `main`. Client and repository changes must land in a coordinated migration.
