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

The package is downloaded and installed entirely by KuroganeOS.

For the standard utility set, install `kuro-toolkit`. Its `depends=` list makes Anvil install the core tools automatically.

## Package state

Current development Anvil displays:

```text
GET   package is not installed
INST  installed version matches the repository
UPD   repository contains a different version
```

For `UPD`, choose **UPDATE SELECTED**. Updates use the same transactional replacement path as installation.

## What Anvil does internally

1. download and parse the catalog;
2. download the selected manifest;
3. verify index name/version match the manifest;
4. read the latest installed version from `/home/anvil.db`;
5. reject conflicts and verify peers;
6. recursively install missing normal dependencies;
7. download the payload over HTTPS;
8. require the response body length to match `bytes=` exactly;
9. write `<destination>.new`;
10. move an existing destination to `<destination>.old`;
11. rename `.new` into the final destination;
12. remove the backup after success;
13. append `name|version|destination` to `/home/anvil.db`.

## Running installed tools

Official command-line packages install under `/apps`. On the current KuroganeOS development branch Kurosh resolves installed `/apps` programs as commands, so after installing `kurofetch` you can run:

```text
kurofetch
```

The explicit form remains available:

```text
run kurofetch
```

## Current limits

The current catalog supports at most **12 packages** in one repository view. Dependency recursion is bounded to **6 levels**. Packages are currently single-payload transactions and cryptographic package signatures are still future work.

See `docs/ROADMAP.md` for planned evolution.
