# Anvil repository roadmap

The repository intentionally begins with the exact feature set the current KuroganeOS client can consume.

## v1 — current

- KIDX1 catalog;
- KPKG1 manifests;
- public HTTPS repositories;
- one payload -> one destination;
- exact payload byte-count validation;
- dependencies, peers and conflicts by package name;
- transactional `.new` / `.old` destination replacement;
- installed package database;
- repository validator and generated index.

## Next format/client work

Planned only after matching KuroganeOS support exists:

- SHA-256 payload hashes;
- signed repository metadata and package signatures;
- multiple configured repositories;
- multi-file/archive packages;
- install/remove/upgrade transactions;
- version constraints and dependency solver improvements;
- package channels such as stable/testing;
- mirrors;
- package provenance/license metadata surfaced in Anvil UI;
- delta/update downloads where useful;
- larger streaming downloads instead of a single bounded response buffer.

Do not add incompatible syntax to KIDX1/KPKG1 before the OS client understands a new format version.
