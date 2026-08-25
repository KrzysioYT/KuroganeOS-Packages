# KIDX1 repository catalog

Anvil fetches `/index.kuro` from the configured repository base.

The first line must be exactly:

```text
KIDX1
```

Each package line has this shape:

```text
pkg|NAME|VERSION|DESCRIPTION|/PATH/TO/MANIFEST
```

Example:

```text
KIDX1
pkg|hello-text|1.0.0|Example Anvil text payload|/packages/hello-text/manifest.kpkg
```

## Current client limits

- at most 12 catalog packages are retained by Anvil v1;
- package name: 31 characters max;
- version: 19 characters max;
- description: 63 characters max;
- manifest request path: 95 characters max in the catalog record;
- manifest path must start with `/`;
- `BASE + manifest path` must also fit the network request path capacity.

The official repository generates this file from package manifests with:

```bash
python3 tools/build_index.py --write
```

CI verifies the committed file is exactly what the generator would produce.
