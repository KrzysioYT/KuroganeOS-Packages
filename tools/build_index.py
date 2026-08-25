#!/usr/bin/env python3
"""Generate/check index.kuro from packages/*/manifest.kpkg."""
from __future__ import annotations

import argparse
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PACKAGES = ROOT / "packages"
INDEX = ROOT / "index.kuro"


def parse_manifest(path: Path) -> dict[str, str]:
    lines = path.read_text(encoding="utf-8").splitlines()
    if not lines or lines[0] != "KPKG1":
        raise SystemExit(f"{path}: not a KPKG1 manifest")
    result: dict[str, str] = {}
    for line in lines[1:]:
        if not line or line.startswith("#"):
            continue
        if "=" in line:
            key, value = line.split("=", 1)
            result[key] = value
    return result


def generated() -> str:
    records: list[str] = ["KIDX1"]
    for directory in sorted(PACKAGES.iterdir() if PACKAGES.exists() else []):
        if not directory.is_dir() or directory.name.startswith("_"):
            continue
        manifest = directory / "manifest.kpkg"
        if not manifest.is_file():
            continue
        data = parse_manifest(manifest)
        name = data.get("name", "")
        version = data.get("version", "")
        description = data.get("description", name)
        relative = "/" + str(manifest.relative_to(ROOT)).replace("\\", "/")
        if "|" in name + version + description + relative:
            raise SystemExit(f"{manifest}: catalog fields may not contain |")
        records.append(f"pkg|{name}|{version}|{description}|{relative}")
    return "\n".join(records) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--write", action="store_true", help="replace index.kuro")
    group.add_argument("--check", action="store_true", help="fail when index.kuro is stale")
    args = parser.parse_args()
    expected = generated()
    if args.write:
        INDEX.write_text(expected, encoding="utf-8", newline="\n")
        print(f"wrote {INDEX.relative_to(ROOT)}")
        return 0
    actual = INDEX.read_text(encoding="utf-8") if INDEX.exists() else ""
    if actual != expected:
        print("index.kuro is stale; run: python3 tools/build_index.py --write")
        return 1
    print("index.kuro: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
