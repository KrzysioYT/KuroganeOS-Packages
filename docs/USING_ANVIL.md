# Using Anvil

Anvil is the Ring-3 package manager shipped with KuroganeOS. Official packages are installed **from inside the operating system**; normal users do not compile, copy or edit package files on the host.

## Official repository

If `/etc/anvil.cfg` is absent, the current client falls back to:

```text
HOST=raw.githubusercontent.com
BASE=/KrzysioYT/KuroganeOS-Packages/main
```

Anvil refreshes `/index.kuro` over HTTPS and presents the packages in its own system window.

## Normal installation

1. boot KuroganeOS with working networking;
2. open **Anvil**;
3. wait for the catalog refresh;
4. select the package;
5. choose **INSTALL SELECTED**.

The package is downloaded, integrity-checked and installed entirely by KuroganeOS.

For the standard utility set, install `kuro-toolkit`. Its dependency graph makes Anvil install the core tools automatically.

## Package state

Current development Anvil displays:

```text
GET   package is not installed
INST  installed version matches the repository
UPD   repository contains a different version
```

For `UPD`, choose **UPDATE SELECTED**. To refresh every stale package in the catalog, choose **UPDATE ALL** or press `U`. Updates use the same transactional replacement path as installation.

## What Anvil does internally

1. download and parse the catalog;
2. download the selected manifest;
3. verify index name/version match the manifest;
4. read the latest installed version from `/home/anvil.db`;
5. reject conflicts and verify peers;
6. recursively install or refresh stale normal dependencies;
7. download the payload over HTTPS;
8. require the response body length to match `bytes=` exactly;
9. recompute SHA-256 and require it to match `sha256=`;
10. write `<destination>.new`;
11. move an existing destination to `<destination>.old`;
12. rename `.new` into the final destination;
13. remove the backup after success;
14. append `name|version|destination` to `/home/anvil.db`.

A failed size/hash check stops before the destination transaction. SHA-256 validates payload integrity relative to the manifest; it is not an independent package signature.

## Running installed tools

Official command-line packages install under `/apps`. Kurosh resolves installed `/apps` programs as commands, so after installing `kurofetch` you can run:

```text
kurofetch
```

The explicit form remains available:

```text
run kurofetch
```

## Current limits

The current catalog supports at most **12 packages** in one repository view. Dependency recursion is bounded to **6 levels**. Packages are currently single-payload transactions and independent cryptographic package signatures are still future work.

See `docs/ROADMAP.md` for planned evolution.
