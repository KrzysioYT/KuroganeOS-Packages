# Create an Anvil package

## 1. Choose a package name

Use a short stable lowercase identifier, for example:

```text
kurofetch
hello-text
my-tool
```

Current Anvil stores package names in a 32-byte C buffer, so this repository limits names to **31 ASCII characters**.

## 2. Create the package directory

```text
packages/my-tool/
  manifest.kpkg
  payload/
    my-tool
```

Anvil v1 installs one payload file. For an executable, the payload may be a KuroganeOS Ring-3 ELF built with the KuroganeOS SDK. For data, it may be a text/binary file that fits the current HTTP transport limit.

## 3. Write the manifest

```text
KPKG1
name=my-tool
version=1.0.0
description=Small example tool
destination=/home/my-tool
payload=/packages/my-tool/payload/my-tool
bytes=12345
depends=
peer=
conflicts=
license=MIT
```

Fields understood by current Anvil are documented in `PACKAGE_FORMAT.md`. `description` and `license` are repository metadata and are ignored safely by current clients if they are not consumed.

## 4. Set exact bytes

Use Python:

```bash
python3 -c "from pathlib import Path; print(Path('packages/my-tool/payload/my-tool').stat().st_size)"
```

or regenerate/check through the repository tooling.

## 5. Add it to the catalog

Do not hand-maintain the index unless necessary:

```bash
python3 tools/build_index.py --write
```

## 6. Validate

```bash
python3 tools/validate_repo.py
python3 tools/build_index.py --check
```

## 7. Test in KuroganeOS

Refresh Anvil, select the package and install it. Verify the destination exists and, for an ELF payload, that KuroganeOS can execute it.

Do not publish payloads you do not have permission to redistribute.
