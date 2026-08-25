# Versioning

## Package versions

Anvil v1 treats `version` as an opaque display/storage string with a maximum of 19 characters. The official repository recommends SemVer-like versions:

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

Publishing a newer version to the repository does not currently upgrade an already-installed package. Anvil v1's installed database is name based. Full upgrade semantics belong to a future client/package format revision.
