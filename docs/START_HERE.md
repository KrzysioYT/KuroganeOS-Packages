# Start here

Use this page as the entry point for the Anvil package ecosystem.

## I only want to install packages

Read [USING_ANVIL.md](USING_ANVIL.md).

## I want KuroganeOS to use my repository

Read [ADDING_REPOSITORY.md](ADDING_REPOSITORY.md). Current Anvil v1 uses one `HOST` + `BASE` source from `/etc/anvil.cfg`.

## I want to create a public package repository

Read [CREATING_REPOSITORY.md](CREATING_REPOSITORY.md), then copy the structure of this repository and keep `index.kuro` at the repository root.

## I want to publish one package

Read [CREATING_PACKAGE.md](CREATING_PACKAGE.md), [PACKAGE_FORMAT.md](PACKAGE_FORMAT.md) and [DEPENDENCIES.md](DEPENDENCIES.md).

The fastest local workflow is:

```bash
python3 tools/new_package.py \
  --name my-tool \
  --version 1.0.0 \
  --description "My KuroganeOS tool" \
  --destination /home/my-tool \
  --payload /path/to/my-tool

python3 tools/validate_repo.py
python3 tools/build_index.py --check
```

## I want to package a Ring-3 executable

Read [BUILDING_RING3_PACKAGE.md](BUILDING_RING3_PACKAGE.md). The package repository stores the resulting KuroganeOS ELF payload; compilation is performed with the KuroganeOS SDK/toolchain.

## I maintain this repository

Read [REPOSITORY_ADMIN.md](REPOSITORY_ADMIN.md), [PUBLISHING.md](PUBLISHING.md) and [SECURITY.md](../SECURITY.md).
