#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
os_repo="${KUROGANE_OS_REPO:-https://github.com/KrzysioYT/KuroganeOS.git}"
os_ref="${KUROGANE_OS_REF:-gpt/kuroganeos-5-gui}"
work="${RUNNER_TEMP:-$root/.build}/kurogane-anvil-builder"

rm -rf -- "$work"
mkdir -p "$work"
git clone --depth 1 --branch "$os_ref" "$os_repo" "$work/os"
(
  cd "$work/os"
  bash ./scripts/build-sdk.sh
)

sysroot="$work/os/build/sdk/sysroot"
lib="$sysroot/usr/lib"
include="$sysroot/usr/include"
cc="${CC:-gcc}"
readelf="${READELF:-readelf}"

for tool in "$cc" "$readelf" sha256sum; do
  command -v "$tool" >/dev/null 2>&1 || { echo "missing build tool: $tool" >&2; exit 1; }
done

common=(
  -ffreestanding -fno-stack-protector -m64 -mno-red-zone
  -mno-mmx -mno-sse -msoft-float -fno-pic -fno-pie
  -mcmodel=large -fno-builtin -ffunction-sections -fdata-sections
  -Wa,--noexecstack -O2 -Wall -Wextra -Wpedantic -Werror
  -I "$include" -I "$root/packages/_common"
)

meta_value() {
  local file="$1" key="$2"
  sed -n "s/^${key}=//p" "$file" | head -n 1
}

built=0
for dir in "$root"/packages/*; do
  [[ -d "$dir" ]] || continue
  [[ "$(basename "$dir")" == _* ]] && continue
  meta="$dir/package.meta"
  source="$dir/src/main.c"
  [[ -f "$meta" && -f "$source" ]] || continue

  name="$(meta_value "$meta" name)"
  version="$(meta_value "$meta" version)"
  description="$(meta_value "$meta" description)"
  destination="$(meta_value "$meta" destination)"
  depends="$(meta_value "$meta" depends)"
  peer="$(meta_value "$meta" peer)"
  conflicts="$(meta_value "$meta" conflicts)"
  license="$(meta_value "$meta" license)"
  [[ -n "$name" && "$name" == "$(basename "$dir")" ]] || { echo "$dir: invalid name" >&2; exit 1; }
  [[ -n "$version" && -n "$description" && "$destination" == /* ]] || { echo "$dir: incomplete package.meta" >&2; exit 1; }

  obj="$work/$name.o"
  mkdir -p "$dir/payload"
  elf="$dir/payload/$name"

  "$cc" "${common[@]}" -std=c11 -c "$source" -o "$obj"
  "$cc" -nostdlib -static -no-pie \
    -Wl,--fatal-warnings -Wl,--build-id=none -Wl,-z,noexecstack \
    -Wl,-z,separate-code -Wl,--gc-sections \
    -T "$lib/kurogane-user.ld" -o "$elf" \
    "$lib/crt0.o" "$obj" -L "$lib" \
    -Wl,--start-group -lc -lkurogane -lui -lgcc -Wl,--end-group

  "$readelf" -hW "$elf" | grep -Eq 'Type:[[:space:]]+EXEC' || { echo "$name: not ET_EXEC" >&2; exit 1; }
  "$readelf" -hW "$elf" | grep -Eq 'Machine:[[:space:]]+Advanced Micro Devices X86-64' || { echo "$name: not x86-64" >&2; exit 1; }
  if "$readelf" -lW "$elf" | grep -Eq '^[[:space:]]*(LOAD|GNU_STACK).*RWE'; then
    echo "$name: executable writable segment/stack" >&2
    exit 1
  fi

  bytes="$(wc -c < "$elf" | tr -d ' ')"
  sha256="$(sha256sum "$elf" | awk '{print $1}')"
  [[ "$sha256" =~ ^[0-9a-f]{64}$ ]] || { echo "$name: invalid SHA-256 result" >&2; exit 1; }
  cat > "$dir/manifest.kpkg" <<EOF
KPKG1
name=$name
version=$version
description=$description
destination=$destination
payload=/packages/$name/payload/$name
bytes=$bytes
sha256=$sha256
depends=$depends
peer=$peer
conflicts=$conflicts
license=$license
EOF
  echo "[package] $name $version -> $destination ($bytes bytes, sha256=$sha256)"
  built=$((built + 1))
done

(( built > 0 )) || { echo 'no source packages found' >&2; exit 1; }
python3 "$root/tools/build_index.py" --write
python3 "$root/tools/validate_repo.py"
echo "[package] Ring-3 build complete: $built package(s)"
