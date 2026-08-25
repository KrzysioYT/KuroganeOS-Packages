# Build a Ring-3 application package

Anvil does not compile source code. A package payload that is an application must already be a KuroganeOS Ring-3 ELF.

## Build with the KuroganeOS repository

Clone/build `KrzysioYT/KuroganeOS` and use its application SDK tooling. The host-specific helper currently includes `scripts/build-app-macos.sh` on macOS; Windows/Linux use the corresponding KuroganeOS SDK/build flow documented in the main repository.

The important rule is that the output must target **KuroganeOS x86-64 Ring-3**, not the host operating system.

## Move the ELF into this repository

Example:

```text
packages/kurofetch/
  manifest.kpkg
  payload/
    kurofetch
```

Manifest:

```text
KPKG1
name=kurofetch
version=1.0.0
description=Small KuroganeOS system information tool
destination=/home/kurofetch
payload=/packages/kurofetch/payload/kurofetch
bytes=<exact ELF byte size>
depends=
peer=
conflicts=
license=<license>
```

Then:

```bash
python3 tools/build_index.py --write
python3 tools/validate_repo.py
```

## Executable permissions

The current KuroganeOS VFS/loader identifies executable ELF content through the OS execution path rather than relying on Unix host file mode semantics. Git's executable bit is useful for host tooling but is not a replacement for producing a valid KuroganeOS ELF.

## Size

Current Anvil downloads a package payload through a bounded 512 KiB Ring-3 HTTP response buffer. Keep application payloads comfortably below that value until streaming package downloads are implemented.
