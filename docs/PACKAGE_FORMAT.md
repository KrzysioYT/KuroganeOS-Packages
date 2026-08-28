# KPKG1 package manifest

A package manifest is UTF-8/ASCII-compatible line-oriented text beginning with:

```text
KPKG1
```

Current Anvil reads `key=value` pairs.

## Required client fields

| Field | Current limit | Meaning |
| --- | ---: | --- |
| `name` | 31 chars | Stable package identifier |
| `version` | 19 chars | Version string displayed/stored by Anvil |
| `destination` | 127 chars | Absolute VFS destination path |
| `payload` | 111 chars | Absolute repository-relative request path |
| `bytes` | positive uint64 | Exact HTTP response body payload size |
| `sha256` | 64 lowercase hex chars | SHA-256 digest of the exact payload body |

`destination` and `payload` must begin with `/`.

Anvil requires both `bytes=` and `sha256=` to match the downloaded payload before it starts the transactional `.new/.old` replacement. The official repository validator independently recomputes the digest for every published payload.

## Optional client fields

| Field | Current limit | Meaning |
| --- | ---: | --- |
| `depends` | 95 chars | Comma-separated dependencies Anvil may install/update automatically |
| `peer` | 95 chars | Comma-separated packages that must already be installed |
| `conflicts` | 95 chars | Comma-separated installed package names that block installation |

Dependency entries are **names only** in KPKG1. Version constraint expressions are not implemented; the client instead compares the repository package version with the latest installed record and refreshes stale normal dependencies when necessary.

## Repository metadata extensions

This official repository also permits metadata such as:

```text
description=Human readable package summary
license=SPDX-or-human-readable-license-id
homepage=https://example.invalid/
```

Current Anvil ignores unknown keys. Do not rely on such fields for installation behavior until the client explicitly implements them.

## Payload limit

The current Ring-3 HTTP response buffer is **512 KiB including HTTP response bytes**. A package body equal to the entire 512 KiB capacity cannot fit together with headers. Keep payloads comfortably below the limit; this repository validator warns above 500 KiB and rejects payloads at or above 512 KiB.

## Example

```text
KPKG1
name=kurofetch
version=1.0.0
description=Compact KuroganeOS system and network overview
destination=/apps/kurofetch
payload=/packages/kurofetch/payload/kurofetch
bytes=9576
sha256=86cb49bf768fcf447d0fe87b3c89ac1af3e3ee137f866ac2874807fbe3e5b06b
depends=
peer=
conflicts=
license=KSAL-2.0
```

`sha256=` provides payload integrity relative to the fetched manifest. It is not an independent package signature; repository/transport authenticity remains part of the trust model described in `SECURITY.md`.
