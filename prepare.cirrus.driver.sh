#!/bin/bash

set -euo pipefail

kernel_release="${1:-$(uname -r)}"
kernel_version="${kernel_release%%-*}"
kernel_version="${kernel_version%%_*}"

if [[ ! $kernel_version =~ ^[0-9]+\.[0-9]+(\.[0-9]+)?([.-]rc[0-9]+)?$ ]]; then
  echo "Unsupported kernel release: $kernel_release" >&2
  exit 2
fi

major_version="${kernel_version%%.*}"
version_tail="${kernel_version#*.}"
minor_version="${version_tail%%.*}"
kernel_short_version="$major_version.$minor_version"

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir"

build_dir="$script_dir/build"
patch_dir="$script_dir/patch_cirrus"
hda_dir="$build_dir/hda"
base_url="https://cdn.kernel.org/pub/linux/kernel/v${major_version}.x"

if [[ -n ${MACBOOK12_AUDIO_KERNEL_CACHE:-} ]]; then
  cache_dir="$MACBOOK12_AUDIO_KERNEL_CACHE"
elif (( EUID == 0 )); then
  cache_dir="/var/cache/macbook12-audio-driver"
else
  cache_dir="$build_dir"
fi

download() {
  local url="$1"
  local destination="$2"

  if command -v curl >/dev/null; then
    curl --fail --location --retry 3 --silent --show-error \
      --output "$destination" "$url"
  elif command -v wget >/dev/null; then
    wget --quiet --tries=3 --output-document="$destination" "$url"
  else
    echo "Install curl or wget to download the matching kernel source." >&2
    return 127
  fi
}

refresh_checksums() {
  local partial="$checksums.part"
  rm -f "$partial"
  download "$base_url/sha256sums.asc" "$partial"
  mv "$partial" "$checksums"
}

checksum_for() {
  local archive_name="$1"
  awk -v filename="$archive_name" '$2 == filename { print $1; exit }' "$checksums"
}

mkdir -p "$build_dir" "$cache_dir"
rm -rf "$hda_dir"

checksums="$cache_dir/sha256sums-v${major_version}.asc"
[[ -f $checksums ]] || refresh_checksums

archive=""
for candidate_version in "$kernel_version" "$kernel_short_version"; do
  archive_name="linux-${candidate_version}.tar.xz"
  expected_sha256="$(checksum_for "$archive_name")"

  if [[ -z $expected_sha256 ]]; then
    refresh_checksums
    expected_sha256="$(checksum_for "$archive_name")"
  fi
  [[ -n $expected_sha256 ]] || continue

  candidate_archive="$cache_dir/$archive_name"
  if [[ -f $candidate_archive ]]; then
    actual_sha256="$(sha256sum "$candidate_archive" | awk '{ print $1 }')"
  else
    actual_sha256=""
  fi

  if [[ $actual_sha256 != "$expected_sha256" ]]; then
    partial_archive="$candidate_archive.part"
    rm -f "$partial_archive"
    echo "Downloading $archive_name"
    if ! download "$base_url/$archive_name" "$partial_archive"; then
      rm -f "$partial_archive"
      continue
    fi

    actual_sha256="$(sha256sum "$partial_archive" | awk '{ print $1 }')"
    if [[ $actual_sha256 != "$expected_sha256" ]]; then
      rm -f "$partial_archive"
      echo "SHA-256 verification failed for $archive_name" >&2
      exit 3
    fi
    mv "$partial_archive" "$candidate_archive"
  fi

  archive="$candidate_archive"
  kernel_version="$candidate_version"
  break
done

if [[ -z $archive ]]; then
  echo "No verified kernel.org source archive found for $kernel_release" >&2
  exit 4
fi

if (( major_version > 6 || (major_version == 6 && minor_version >= 17) )); then
  makefile_name="Makefile_cs420x"
  tar --strip-components=2 -xf "$archive" --directory="$build_dir" \
    "linux-${kernel_version}/sound/hda"
  mv "$hda_dir/codecs/cirrus/Makefile" "$hda_dir/codecs/cirrus/Makefile.orig"
  mv "$hda_dir/codecs/cirrus/cs420x.c" "$hda_dir/codecs/cirrus/cs420x.c.orig"
  cp "$patch_dir/cs420x.c" \
    "$patch_dir/patch_cirrus_a1534_setup.h" \
    "$patch_dir/patch_cirrus_a1534_pcm.h" \
    "$hda_dir/codecs/cirrus"
  cp "$patch_dir/$makefile_name" "$hda_dir/codecs/cirrus/Makefile"
else
  makefile_name="Makefile_cirrus"
  tar --strip-components=3 -xf "$archive" --directory="$build_dir" \
    "linux-${kernel_version}/sound/pci/hda"
  mv "$hda_dir/Makefile" "$hda_dir/Makefile.orig"
  mv "$hda_dir/patch_cirrus.c" "$hda_dir/patch_cirrus.c.orig"
  cp "$patch_dir/patch_cirrus.c" \
    "$patch_dir/patch_cirrus_a1534_setup.h" \
    "$patch_dir/patch_cirrus_a1534_pcm.h" \
    "$hda_dir"
  cp "$patch_dir/$makefile_name" "$hda_dir/Makefile"
fi

if (( major_version > 6 || (major_version == 6 && minor_version >= 17) )); then
  sed -i 's/\.free/.remove/' "$hda_dir/codecs/cirrus/patch_cirrus_a1534_pcm.h"
fi

if (( major_version == 6 && minor_version >= 12 && minor_version < 17 )); then
  sed -i 's/snd_pci_quirk/hda_quirk/' "$hda_dir/patch_cirrus.c"
  sed -i 's/SND_PCI_QUIRK\b/HDA_CODEC_QUIRK/' "$hda_dir/patch_cirrus.c"
fi

if (( major_version == 6 && minor_version <= 11 )); then
  sed -i 's/hda_quirk/snd_pci_quirk/' "$hda_dir/patch_cirrus.c"
fi

if (( major_version < 5 || (major_version == 5 && minor_version < 6) )); then
  sed -i 's/timespec64/timespec/' "$hda_dir/patch_cirrus.c"
  sed -i 's/timespec64/timespec/' "$hda_dir/patch_cirrus_a1534_pcm.h"
  sed -i 's/ktime_get_real_ts64/getnstimeofday/' "$hda_dir/patch_cirrus_a1534_pcm.h"
fi

cp "$script_dir/$makefile_name" "$script_dir/Makefile"
