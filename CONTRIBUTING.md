# Contributing packages

Contributions are welcome when they are compatible with the current KuroganeOS/Anvil package contract.

## Package PR

1. create `packages/<name>/`;
2. add `manifest.kpkg` and payload;
3. document source/license/provenance in the package README;
4. run `python3 tools/build_index.py --write`;
5. run `python3 tools/validate_repo.py`;
6. commit the updated `index.kuro`;
7. open a pull request.

Do not submit malware, credential stealers, hidden network beacons, pirated binaries, payloads you cannot redistribute, or packages designed to bypass KuroganeOS security boundaries.

Because the repository does not yet declare a single repository-wide software license, package authors must clearly document the license/provenance of contributed payloads. A future project-wide contribution/license policy may supersede this note.
