#!/bin/bash

set -euo pipefail

module_name="macbook12-audio-driver"
module_version="0.1"
source_dir="/usr/src/${module_name}-${module_version}"
cache_dir="/var/cache/macbook12-audio-driver"
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
action="${1:-install}"

if (( EUID != 0 )); then
  echo "Run this script as root." >&2
  exit 1
fi

remove_driver() {
  dkms remove -m "$module_name" -v "$module_version" --all 2>/dev/null || true
  rm -rf -- "$source_dir" "$cache_dir"
  depmod -a
}

install_driver() {
  if dkms status -m "$module_name" -v "$module_version" 2>/dev/null | grep -q .; then
    dkms remove -m "$module_name" -v "$module_version" --all
  fi

  rm -rf -- "$source_dir"
  install -d -m 0755 "$source_dir"
  install -m 0644 \
    "$script_dir/dkms.conf" \
    "$script_dir/Makefile_cirrus" \
    "$script_dir/Makefile_cs420x" \
    "$source_dir"
  install -m 0755 "$script_dir/prepare.cirrus.driver.sh" "$source_dir"
  cp -a "$script_dir/patch_cirrus" "$source_dir"

  dkms install -m "$module_name" -v "$module_version" --force
}

case "$action" in
  install|-i|--install)
    install_driver
    ;;
  remove|-r|-u|--remove|--uninstall)
    remove_driver
    ;;
  *)
    echo "Usage: $0 [install|remove]" >&2
    exit 2
    ;;
esac
