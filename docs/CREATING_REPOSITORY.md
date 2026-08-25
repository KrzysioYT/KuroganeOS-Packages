# Create your own Anvil repository

The easiest supported backend is a **public GitHub repository** served by `raw.githubusercontent.com`.

## Minimal layout

```text
index.kuro
packages/
  my-package/
    manifest.kpkg
    payload/
      my-file
```

`index.kuro`:

```text
KIDX1
pkg|my-package|1.0.0|My first package|/packages/my-package/manifest.kpkg
```

`manifest.kpkg`:

```text
KPKG1
name=my-package
version=1.0.0
description=My first package
destination=/home/my-file
payload=/packages/my-package/payload/my-file
bytes=123
depends=
peer=
conflicts=
```

The `bytes` value must be the **exact payload size in bytes**.

## Recommended workflow

1. fork/copy this repository structure;
2. put package directories under `packages/`;
3. run `python3 tools/validate_repo.py`;
4. run `python3 tools/build_index.py --write`;
5. commit generated `index.kuro`;
6. publish the repository;
7. point `/etc/anvil.cfg` to it.

## GitHub configuration

For `OWNER/REPO`, branch `main`:

```text
HOST=raw.githubusercontent.com
BASE=/OWNER/REPO/main
```

Keep it public. Current Anvil does not implement GitHub authentication.

## Compatibility rule

Do not change `KIDX1` or `KPKG1` syntax casually. Those strings are wire-format version identifiers parsed by released KuroganeOS clients. Incompatible changes require a new format version and client support first.
