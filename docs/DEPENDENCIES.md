# Dependencies, peers and conflicts

Anvil v1 supports three name lists in `KPKG1`.

## `depends=`

Comma-separated package names that Anvil installs automatically when they are not already present in `/home/anvil.db`.

```text
depends=libkuro-ui,kuro-protocols
```

Dependencies must exist in the currently loaded catalog. Recursive installation is bounded to **6 levels**.

## `peer=`

Packages that must already be installed. Anvil does not install peers automatically.

```text
peer=kuro-runtime
```

A peer may come from an earlier repository/configuration because current Anvil checks the installed database rather than requiring the peer in the current catalog.

## `conflicts=`

Installation fails when any named package is already installed.

```text
conflicts=my-tool-legacy
```

## Important v1 limitations

- no version operators (`>=`, `~`, ranges, etc.);
- no provides/virtual packages;
- no dependency alternatives;
- no cycle solver beyond the recursion bound;
- installed state is package-name based;
- no full upgrade/remove transaction model yet.

Keep dependency graphs small and acyclic.
