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

`destination` and `payload` must begin with `/`.

## Optional client fields

| Field | Current limit | Meaning |
| --- | ---: | --- |
| `depends` | 95 chars | Comma-separated dependencies Anvil may install automatically |
| `peer` | 95 chars | Comma-separated packages that must already be installed |
| `conflicts` | 95 chars | Comma-separated installed package names that block installation |

Dependency entries are **names only** in Anvil v1. Version constraints are not implemented.

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
name=hello-text
version=1.0.0
description=Example Anvil text payload
destination=/home/hello.txt
payload=/packages/hello-text/payload/hello.txt
bytes=29
depends=
peer=
conflicts=
license=CC0-1.0
```
