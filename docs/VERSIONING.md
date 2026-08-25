# Versioning

## Package versions

Anvil treats `version` as an opaque display/storage string with a maximum of 19 characters. The official repository recommends SemVer-like versions:

```text
1.0.0
1.2.3
2.0.0-beta1
```

Current dependency fields contain package **names only**. A value such as `depends=libfoo>=2` is not supported and will be interpreted as a package name, so do not use version operators yet.

## Repository format versions

The first line of the catalog and manifest are protocol/version identifiers:

```text
KIDX1
KPKG1
```

They are compatibility contracts with the KuroganeOS client. Do not redefine their syntax incompatibly.

When the OS gains a new incompatible package format, introduce a new identifier such as `KPKG2` only together with client support and a migration policy.

## Updating existing packages

Current KuroganeOS development Anvil records `name|version|destination` in `/home/anvil.db` and uses the **latest record for that package name** as its installed state.

After a catalog refresh:

- `GET` means the package is not installed;
- `INST` means the installed version exactly matches the catalog;
- `UPD` means the package exists locally but the catalog version is different.

Selecting an `UPD` package and choosing **UPDATE SELECTED** downloads the new payload and uses the same transactional `.new` / `.old` replacement path as a first install. A successful update appends the new version to the installed database, making the newest record authoritative.

Dependencies are still name-only. Installing a package automatically installs a missing dependency, but it does not currently express or enforce minimum/maximum dependency versions.
