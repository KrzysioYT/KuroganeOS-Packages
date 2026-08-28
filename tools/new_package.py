#!/usr/bin/env python3
"""Create a KPKG1 package directory from an existing payload file."""
from __future__ import annotations

import argparse
import hashlib
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
NAME_RE = re.compile(r"^[a-z0-9][a-z0-9._-]{0,30}$")


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(128 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description="Create an Anvil KPKG1 package")
    parser.add_argument("--name", required=True)
    parser.add_argument("--version", required=True)
    parser.add_argument("--description", required=True)
    parser.add_argument("--destination", required=True)
    parser.add_argument("--payload", required=True, type=Path)
    parser.add_argument("--depends", default="")
    parser.add_argument("--peer", default="")
    parser.add_argument("--conflicts", default="")
    parser.add_argument("--license", default="UNSPECIFIED")
    args = parser.parse_args()

    if not NAME_RE.fullmatch(args.name):
        parser.error("--name must be lowercase ASCII [a-z0-9._-] and <=31 chars")
    if len(args.version) > 19:
        parser.error("--version exceeds current 19-character client capacity")
    if len(args.description) > 63 or "|" in args.description:
        parser.error("--description must be <=63 chars and may not contain |")
    if not args.destination.startswith("/") or len(args.destination) > 127:
        parser.error("--destination must be absolute and <=127 chars")

    source = args.payload.expanduser().resolve()
    if not source.is_file():
        parser.error(f"payload does not exist: {source}")

    package_dir = ROOT / "packages" / args.name
    if package_dir.exists():
        parser.error(f"package already exists: {package_dir}")
    payload_dir = package_dir / "payload"
    payload_dir.mkdir(parents=True)
    destination_file = payload_dir / source.name
    shutil.copy2(source, destination_file)

    request_path = "/" + str(destination_file.relative_to(ROOT)).replace("\\", "/")
    if len(request_path) > 111:
        shutil.rmtree(package_dir)
        parser.error("generated payload request path exceeds current 111-character manifest capacity")

    payload_sha256 = sha256_file(destination_file)
    manifest = package_dir / "manifest.kpkg"
    manifest.write_text(
        "\n".join([
            "KPKG1",
            f"name={args.name}",
            f"version={args.version}",
            f"description={args.description}",
            f"destination={args.destination}",
            f"payload={request_path}",
            f"bytes={destination_file.stat().st_size}",
            f"sha256={payload_sha256}",
            f"depends={args.depends}",
            f"peer={args.peer}",
            f"conflicts={args.conflicts}",
            f"license={args.license}",
            "",
        ]),
        encoding="utf-8",
        newline="\n",
    )
    (package_dir / "README.md").write_text(
        f"# {args.name}\n\n{args.description}\n\n"
        f"Installs `{args.destination}`.\n\n"
        f"Payload SHA-256: `{payload_sha256}`.\n\n"
        "Document source, provenance, license and KuroganeOS testing here.\n",
        encoding="utf-8",
        newline="\n",
    )

    subprocess.run([sys.executable, str(ROOT / "tools" / "build_index.py"), "--write"], check=True)
    subprocess.run([sys.executable, str(ROOT / "tools" / "validate_repo.py")], check=True)
    print(f"created packages/{args.name} and regenerated index.kuro")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
