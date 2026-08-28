#!/usr/bin/env python3
"""Validate a KuroganeOS Anvil KIDX1/KPKG1 repository."""
from __future__ import annotations

import hashlib
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PACKAGES = ROOT / "packages"
INDEX = ROOT / "index.kuro"
MAX_PACKAGES = 12
HTTP_CAP = 512 * 1024
WARN_PAYLOAD = 500 * 1024
NAME_RE = re.compile(r"^[a-z0-9][a-z0-9._-]{0,30}$")
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")


def parse_manifest(path: Path) -> dict[str, str]:
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines()
    if not lines or lines[0] != "KPKG1":
        raise ValueError(f"{path}: first line must be KPKG1")
    data: dict[str, str] = {}
    for lineno, line in enumerate(lines[1:], start=2):
        if not line or line.startswith("#"):
            continue
        if "=" not in line:
            raise ValueError(f"{path}:{lineno}: expected key=value")
        key, value = line.split("=", 1)
        if not key:
            raise ValueError(f"{path}:{lineno}: empty key")
        data[key] = value
    return data


def split_names(value: str) -> list[str]:
    return [x.strip() for x in value.split(",") if x.strip()]


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(128 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> int:
    errors: list[str] = []
    warnings: list[str] = []
    manifests: dict[str, tuple[Path, dict[str, str]]] = {}

    for directory in sorted(PACKAGES.iterdir() if PACKAGES.exists() else []):
        if not directory.is_dir() or directory.name.startswith("_"):
            continue
        manifest_path = directory / "manifest.kpkg"
        if not manifest_path.is_file():
            if (directory / "package.meta").is_file() and (directory / "src" / "main.c").is_file():
                warnings.append(f"{directory}: source-only package awaiting Ring-3 build")
                continue
            errors.append(f"{directory}: missing manifest.kpkg")
            continue
        try:
            data = parse_manifest(manifest_path)
        except Exception as exc:
            errors.append(str(exc))
            continue

        for key in ("name", "version", "destination", "payload", "bytes", "sha256"):
            if not data.get(key):
                errors.append(f"{manifest_path}: missing required field {key}")

        name = data.get("name", "")
        if not NAME_RE.fullmatch(name):
            errors.append(f"{manifest_path}: invalid name {name!r}; max 31 lowercase ASCII chars")
        if directory.name != name:
            errors.append(f"{manifest_path}: directory name must equal package name")
        if name in manifests:
            errors.append(f"duplicate package name: {name}")
        if len(data.get("version", "")) > 19:
            errors.append(f"{manifest_path}: version exceeds 19 chars")
        if len(data.get("description", "")) > 63:
            errors.append(f"{manifest_path}: description exceeds 63 chars")
        if len(data.get("destination", "")) > 127 or not data.get("destination", "").startswith("/"):
            errors.append(f"{manifest_path}: destination must be absolute and <=127 chars")
        if len(data.get("payload", "")) > 111 or not data.get("payload", "").startswith("/"):
            errors.append(f"{manifest_path}: payload must start with / and be <=111 chars")
        declared_sha256 = data.get("sha256", "")
        if declared_sha256 and not SHA256_RE.fullmatch(declared_sha256):
            errors.append(f"{manifest_path}: sha256 must be exactly 64 lowercase hexadecimal characters")
        for key in ("depends", "peer", "conflicts"):
            if len(data.get(key, "")) > 95:
                errors.append(f"{manifest_path}: {key} exceeds 95 chars")
            for dep in split_names(data.get(key, "")):
                if not NAME_RE.fullmatch(dep):
                    errors.append(f"{manifest_path}: invalid package name in {key}: {dep!r}")

        payload_rel = data.get("payload", "").lstrip("/")
        payload_path = ROOT / payload_rel
        if payload_rel and payload_path.is_file():
            actual = payload_path.stat().st_size
            try:
                declared = int(data.get("bytes", ""), 10)
            except ValueError:
                declared = -1
                errors.append(f"{manifest_path}: bytes must be a decimal integer")
            if declared != actual:
                errors.append(f"{manifest_path}: bytes={declared}, actual payload size={actual}")
            if declared_sha256 and SHA256_RE.fullmatch(declared_sha256):
                actual_sha256 = sha256_file(payload_path)
                if declared_sha256 != actual_sha256:
                    errors.append(
                        f"{manifest_path}: sha256={declared_sha256}, actual payload sha256={actual_sha256}"
                    )
            if actual >= HTTP_CAP:
                errors.append(f"{manifest_path}: payload {actual} bytes reaches/exceeds 512 KiB transport capacity")
            elif actual > WARN_PAYLOAD:
                warnings.append(f"{manifest_path}: payload {actual} bytes is close to the 512 KiB response capacity")
        elif payload_rel:
            errors.append(f"{manifest_path}: payload file not found: /{payload_rel}")

        manifests[name] = (manifest_path, data)

    if len(manifests) > MAX_PACKAGES:
        errors.append(f"catalog has {len(manifests)} packages; current Anvil retains at most {MAX_PACKAGES}")

    for name, (path, data) in manifests.items():
        for dep in split_names(data.get("depends", "")):
            if dep not in manifests:
                errors.append(f"{path}: dependency {dep!r} is not present in this catalog")

    if not INDEX.is_file():
        errors.append("missing index.kuro")
    else:
        lines = INDEX.read_text(encoding="utf-8").splitlines()
        if not lines or lines[0] != "KIDX1":
            errors.append("index.kuro: first line must be KIDX1")
        catalog: dict[str, tuple[str, str, str]] = {}
        for lineno, line in enumerate(lines[1:], start=2):
            if not line:
                continue
            parts = line.split("|")
            if len(parts) != 5 or parts[0] != "pkg":
                errors.append(f"index.kuro:{lineno}: invalid package record")
                continue
            _, name, version, description, manifest = parts
            if len(name) > 31 or len(version) > 19 or len(description) > 63 or len(manifest) > 95:
                errors.append(f"index.kuro:{lineno}: field exceeds current client capacity")
            if not manifest.startswith("/"):
                errors.append(f"index.kuro:{lineno}: manifest path must start with /")
            catalog[name] = (version, description, manifest)
        if set(catalog) != set(manifests):
            errors.append(f"index.kuro package set differs from manifests: index={sorted(catalog)}, manifests={sorted(manifests)}")
        for name, (path, data) in manifests.items():
            expected = (
                data.get("version", ""),
                data.get("description", name),
                "/" + str(path.relative_to(ROOT)).replace("\\", "/"),
            )
            if catalog.get(name) != expected:
                errors.append(f"index.kuro: {name} record is stale; run tools/build_index.py --write")

    for warning in warnings:
        print(f"WARN: {warning}")
    if errors:
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        print(f"validation failed: {len(errors)} error(s)", file=sys.stderr)
        return 1
    print(f"Anvil repository validation: PASS ({len(manifests)} published package(s))")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
