# Automated official packages

Official source packages in this repository are built automatically by GitHub Actions.

For end users the workflow is intentionally simple:

1. boot KuroganeOS with working Internet;
2. open **Anvil**;
3. select a package;
4. choose **INSTALL SELECTED**.

There is no host-side build/copy step for installing official packages.

## One-click toolkit

Install `kuro-toolkit` in Anvil to pull the core utility set through `depends=` automatically.

## Maintainer pipeline

Source packages contain:

```text
package.meta
src/main.c
```

The `Build KuroganeOS Ring-3 packages` workflow clones the configured KuroganeOS SDK revision, builds a real x86-64 Ring-3 ELF, writes `payload/<name>`, generates `manifest.kpkg` with exact `bytes=`, regenerates `index.kuro`, validates the repository and commits generated artifacts to `main`.

The published `main` branch therefore remains directly consumable by `raw.githubusercontent.com` and Anvil.
