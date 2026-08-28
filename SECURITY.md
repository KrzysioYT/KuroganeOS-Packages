# Security policy for Anvil packages

## Current trust model

Anvil downloads repository metadata and payloads through KuroganeOS HTTPS/TLS. Before an install or update is committed, the client verifies both:

- the exact response body length declared by `bytes=`;
- the SHA-256 digest declared by `sha256=`.

The official repository builder generates the digest from the produced Ring-3 ELF and the repository validator independently recomputes it before accepting the catalog.

This protects payload integrity relative to the manifest and catches corruption or a payload changed without a corresponding manifest update. It is **not** an independent package/repository signature.

Therefore:

- the configured repository and its manifest content remain part of the trusted computing base;
- HTTPS/TLS and the configured repository identity provide transport/source authentication for the current model;
- a repository compromise that changes both manifest and payload is not stopped by SHA-256 alone;
- third-party repositories must not be described as independently signed/authenticated package sources;
- do not put secrets or GitHub tokens into `anvil.cfg` or package URLs.

## Reporting a malicious or vulnerable package

Do not publish exploit details in a package PR when coordinated disclosure is appropriate. Contact the KuroganeOS maintainer through the security/contact mechanism configured for the main project, and identify the package name/version and affected payload.

## Package review rules

Official packages should be reproducible from source where practical, document third-party licensing, avoid unnecessary privileged destinations and stay within current Anvil capabilities. Generated Ring-3 payloads must pass the repository ELF checks, exact-size validation and SHA-256 validation before publication.

Independent cryptographic package/repository signatures remain future work; SHA-256 payload verification exists today and must not be confused with signing.
