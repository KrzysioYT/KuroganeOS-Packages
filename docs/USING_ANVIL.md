# Using Anvil

Anvil is the Ring-3 package manager shipped with KuroganeOS.

## Official repository

If `/etc/anvil.cfg` is absent, the current client falls back to:

```text
HOST=raw.githubusercontent.com
BASE=/KrzysioYT/KuroganeOS-Packages/main
```

On refresh Anvil requests:

```text
/index.kuro
```

from that base over HTTPS, parses `KIDX1`, then fetches the selected `KPKG1` manifest and payload.

## Installation flow

Current Anvil performs these steps:

1. download and parse the catalog;
2. download the selected manifest;
3. verify index name/version match the manifest;
4. reject installed conflicts;
5. require all peer dependencies to already be installed;
6. recursively install missing normal dependencies;
7. download the payload over HTTPS;
8. require the response body length to match `bytes=` exactly;
9. write `<destination>.new`;
10. move an existing destination to `<destination>.old`;
11. rename `.new` into the final destination;
12. remove the backup after success;
13. append `name|version|destination` to `/home/anvil.db`.

## Current behavior to know

Anvil v1 treats a package name as installed once it appears in `/home/anvil.db`. It does not yet provide full upgrade, remove, signature or multi-file package semantics.

The current catalog supports at most **12 packages** in one repository view. Dependency recursion is bounded to **6 levels**.

See `docs/ROADMAP.md` for planned evolution.
