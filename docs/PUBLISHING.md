# Publishing packages

Before every publish:

```bash
python3 tools/validate_repo.py
python3 tools/build_index.py --check
```

If the catalog is stale:

```bash
python3 tools/build_index.py --write
```

Then commit both the package and generated `index.kuro`.

## Pull request checklist

- package name/version are intentional;
- payload is redistributable;
- `destination` is safe and expected;
- `bytes` equals the real payload size;
- dependencies exist or are intentionally peers;
- conflicts are justified;
- `index.kuro` was regenerated;
- no credentials, private keys or tokens are included;
- payload is below the current transport size limit.

GitHub Actions runs the same structural validation for pushes and pull requests.

## Updating an existing package

Current Anvil v1 does not implement a complete upgrade command. Merely replacing a package payload/version in the repository does not make already-installed clients upgrade it automatically. Treat repository updates as availability for clean/new installs until upgrade semantics are added to the OS.
