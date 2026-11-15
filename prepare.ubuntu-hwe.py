#!/usr/bin/env python3
"""Extract the exact Ubuntu HWE HDA sources, including distribution patches."""
import argparse
import gzip
import hashlib
from pathlib import Path
import re
import subprocess
import tarfile


POOL = "https://archive.ubuntu.com/ubuntu/pool/main/l"


def fetch(url, destination, expected_sha256=None):
    if not destination.exists():
        partial = destination.with_name(destination.name + ".part")
        try:
            subprocess.run([
                "curl", "--fail", "--location", "--retry", "3",
                "--connect-timeout", "15", "--max-time", "600",
                "--silent", "--show-error", "--output", str(partial), url,
            ], check=True)
            partial.replace(destination)
        finally:
            partial.unlink(missing_ok=True)
    if expected_sha256:
        digest = hashlib.sha256()
        with destination.open("rb") as stream:
            for chunk in iter(lambda: stream.read(1024 * 1024), b""):
                digest.update(chunk)
        actual = digest.hexdigest()
        if actual != expected_sha256:
            destination.unlink()
            raise RuntimeError(f"SHA-256 verification failed: {destination.name}; rerun to download again")
    return destination


def source_files(dsc):
    section = re.search(r"(?m)^Checksums-Sha256:\n((?:[ \t].*\n)+)", dsc)
    if not section:
        raise RuntimeError("Missing source SHA-256 checksums in Ubuntu DSC")
    files = {}
    for line in section[1].splitlines():
        digest, size, name = line.split()
        if not re.fullmatch(r"[a-f0-9]{64}", digest) or not size.isdigit() or Path(name).name != name:
            raise RuntimeError("Invalid Ubuntu source checksum entry")
        files[name] = digest
    orig = [name for name in files if ".orig.tar." in name]
    diff = [name for name in files if name.endswith(".diff.gz")]
    if len(orig) != 1 or len(diff) != 1:
        raise RuntimeError("Expected Ubuntu format-1.0 source (one orig archive and one diff.gz)")
    return orig[0], diff[0], files


def prepare(source, version, kernel, build_dir, cache_dir):
    if not re.fullmatch(r"linux-hwe-[0-9.]+", source):
        raise RuntimeError(f"Not an Ubuntu HWE source package: {source}")
    if not re.fullmatch(r"[0-9][A-Za-z0-9.+~:-]*", version):
        raise RuntimeError("Invalid Ubuntu source version")
    major, minor = map(int, kernel.split("-")[0].split(".")[:2])
    modern = (major, minor) >= (6, 17)
    subtree = "sound/hda" if modern else "sound/pci/hda"
    strip = 2 if modern else 3
    cache_dir.mkdir(parents=True, exist_ok=True)
    build_dir.mkdir(parents=True, exist_ok=True)
    url = f"{POOL}/{source}"
    dsc_name = f"{source}_{version}.dsc"
    dsc = fetch(f"{url}/{dsc_name}", cache_dir / dsc_name).read_text()
    orig_name, diff_name, checksums = source_files(dsc)
    archive = fetch(f"{url}/{orig_name}", cache_dir / orig_name, checksums[orig_name])
    delta = fetch(f"{url}/{diff_name}", cache_dir / diff_name, checksums[diff_name])
    with tarfile.open(archive) as tar:
        first = tar.next()
        if first is None:
            raise RuntimeError("Empty Ubuntu source archive")
        root = first.name.split("/")[0]
    if not root or root.startswith("-") or root in (".", ".."):
        raise RuntimeError("Invalid Ubuntu source archive root")
    subprocess.run([
        "tar", "-xf", str(archive), f"--strip-components={strip}",
        "-C", str(build_dir), f"{root}/{subtree}",
    ], check=True)
    # Apply every change in the extracted subtree, not a hardcoded struct edit.
    # Fail closed if packaging/layout changes; never silently use vanilla headers.
    diff = gzip.decompress(delta.read_bytes()).decode()
    count = 0
    for block in re.split(r"(?m)(?=^--- )", diff):
        lines = block.splitlines()
        if len(lines) < 2 or not lines[0].startswith("--- ") or not lines[1].startswith("+++ "):
            continue
        old, new = (line[4:].split("\t", 1)[0] for line in lines[:2])
        marker = f"/{subtree}/"
        if marker not in old and marker not in new:
            continue
        # Ensure both sides of each patch stay inside the HDA subtree.
        for name in (old, new):
            if name != "/dev/null" and (marker not in name or ".." in Path(name).parts):
                raise RuntimeError("Unsafe HDA patch path")
        subprocess.run([
            "patch", "--batch", "--forward", "--fuzz=0",
            f"-p{strip}", "-d", str(build_dir),
        ], input=block, text=True, check=True)
        count += 1
    (build_dir / "ubuntu-hwe-source.txt").write_text(
        f"Kernel: {kernel}\nSource: {source} {version}\nHDA patches: {count}\n")
    print(f"Prepared exact Ubuntu HWE source: {source} {version} ({count} HDA patches)")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("kernel_release")
    parser.add_argument("build_dir", type=Path)
    parser.add_argument("cache_dir", type=Path)
    args = parser.parse_args()
    metadata = subprocess.run([
        "dpkg-query", "-W", "-f=${source:Package}\n${source:Version}",
        f"linux-headers-{args.kernel_release}",
    ], check=True, capture_output=True, text=True).stdout.splitlines()
    if len(metadata) != 2:
        raise RuntimeError("Cannot identify target kernel source from installed headers")
    prepare(metadata[0], metadata[1], args.kernel_release,
            args.build_dir.resolve(), args.cache_dir.resolve())


if __name__ == "__main__":
    try:
        main()
    except (RuntimeError, OSError, subprocess.CalledProcessError) as error:
        raise SystemExit(f"Ubuntu HWE source preparation failed: {error}")
