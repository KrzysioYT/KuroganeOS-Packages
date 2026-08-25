# Security policy for Anvil packages

## Current trust model

Anvil v1 downloads repository metadata and payloads through KuroganeOS HTTPS/TLS and checks exact payload byte count. It does **not** yet verify a package SHA-256 digest or cryptographic package/repository signature.

Therefore:

- the configured repository is part of the trusted computing base;
- a compromised repository branch can serve a different same-sized payload;
- third-party repositories must not be treated as cryptographically authenticated package sources;
- do not put secrets or GitHub tokens into `anvil.cfg` or package URLs.

## Reporting a malicious or vulnerable package

Do not publish exploit details in a package PR when coordinated disclosure is appropriate. Contact the KuroganeOS maintainer through the security/contact mechanism configured for the main project, and identify the package name/version and affected payload.

## Package review rules

Official packages should be reproducible from source where practical, document third-party licensing, avoid unnecessary privileged destinations and stay within current Anvil capabilities.

Package signing and hashes are high-priority roadmap items, not features that exist today.
