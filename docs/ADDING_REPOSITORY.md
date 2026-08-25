# Point KuroganeOS at another Anvil repository

Current Anvil v1 supports **one configured repository at a time**. This is a repository switch, not an `apt`-style list of many sources.

Anvil reads:

```text
/etc/anvil.cfg
```

The file contains two lines:

```text
HOST=raw.githubusercontent.com
BASE=/OWNER/REPOSITORY/main
```

For a GitHub repository `alice/my-kurogane-packages` on branch `main`:

```text
HOST=raw.githubusercontent.com
BASE=/alice/my-kurogane-packages/main
```

The repository must expose `index.kuro` at its base and all manifest/payload paths referenced by the catalog.

## When building a KuroganeOS image

Place the desired configuration at the root filesystem path that becomes `/etc/anvil.cfg`. The official KuroganeOS build already stages this file for the official repository.

## On a running installation

Anvil reloads repository configuration when the application starts. Replace `/etc/anvil.cfg` using the writable VFS/file-management tooling available in that KuroganeOS build, close Anvil and launch it again.

Do not invent additional keys: current Anvil only consumes `HOST` and `BASE`.

## Limits

- `HOST` must fit the current 64-byte network host field.
- the final `BASE + relative path` must fit the current 160-byte network path field;
- the repository must be reachable with the current HTTPS/TLS trust store;
- private GitHub repositories are not supported because Anvil has no GitHub authentication/token transport.
