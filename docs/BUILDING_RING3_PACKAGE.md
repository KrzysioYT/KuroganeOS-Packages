# Build a Ring-3 application package

Anvil does not compile source code inside KuroganeOS. Application payloads published by the official repository are already KuroganeOS x86-64 Ring-3 ELF executables.

## Official source-package flow

For official source packages, keep the source in the package repository:

```text
packages/kurofetch/
  package.meta
  src/
    main.c
```

GitHub Actions runs `tools/build_ring3_packages.sh`. The builder:

1. checks out the configured KuroganeOS SDK source;
2. builds the SDK/sysroot;
3. compiles the package as a freestanding x86-64 Ring-3 ELF;
4. rejects a non-`ET_EXEC`, wrong architecture, or writable+executable segment/stack;
5. writes `payload/<name>`;
6. calculates the exact byte size and SHA-256 digest;
7. generates `manifest.kpkg`;
8. regenerates `index.kuro` and runs repository validation;
9. publishes generated payloads/manifests back to `main`.

The publish step detects if `main` advanced while the SDK/package build was running and rebuilds on the newest tree before pushing, so generated payloads are not committed from a stale package definition.

## Resulting manifest

A generated package looks like:

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

Anvil verifies both `bytes=` and `sha256=` before committing the install/update transaction.

## Packaging an already-built payload

`tools/new_package.py` remains available for payloads that are already valid KuroganeOS artifacts. It copies the payload, calculates `bytes=` and `sha256=`, creates the manifest, regenerates `index.kuro`, and runs validation.

Do not compile a normal Linux/macOS/Windows executable and put it into Anvil. The payload must target **KuroganeOS x86-64 Ring-3**.

## Executable permissions

The KuroganeOS VFS/loader identifies executable ELF content through the OS execution path rather than relying on Unix host file mode semantics. Git's executable bit is useful for host tooling but is not a replacement for producing a valid KuroganeOS ELF.

## Size

Current Anvil downloads a package payload through a bounded 512 KiB Ring-3 HTTP response buffer. Keep application payloads comfortably below that value until streaming package downloads are implemented.
