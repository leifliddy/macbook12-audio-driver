"""Offline tests: python3 -m unittest discover -s tests -v."""
import gzip
import hashlib
import importlib.util
import io
import os
from pathlib import Path
import shutil
import subprocess
import tarfile
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location(
    "ubuntu_hwe", Path(__file__).resolve().parents[1] / "prepare.ubuntu-hwe.py")
assert SPEC is not None and SPEC.loader is not None
hwe = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(hwe)


class HweSourceTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.cache = self.root / "cache"
        self.cache.mkdir()
        self.build = self.root / "build"

    def fixture(self, modern=True, bad_context=False, unsafe=False):
        source = "linux-hwe-7.0" if modern else "linux-hwe-6.8"
        base = "7.0.0" if modern else "6.8.0"
        version = base + "-43.43~24.04.1"
        kernel = base + "-43-generic"
        subtree = "sound/hda" if modern else "sound/pci/hda"
        orig = self.cache / f"{source}_{base}.orig.tar.gz"
        member = f"{source}-{base}/{subtree}/common/hda_local.h"
        with tarfile.open(orig, "w:gz") as tar:
            top = tarfile.TarInfo(f"{source}-{base}")
            top.type = tarfile.DIRTYPE
            top.mode = 0o755
            tar.addfile(top)
            for name in [subtree, subtree + "/common"]:
                directory = tarfile.TarInfo(f"{source}-{base}/{name}")
                directory.type = tarfile.DIRTYPE
                directory.mode = 0o755
                tar.addfile(directory)
            data = b"struct test {\n    int value;\n};\n"
            info = tarfile.TarInfo(member)
            info.size = len(data)
            tar.addfile(info, io.BytesIO(data))
        delta = self.cache / f"{source}_{version}.diff.gz"
        old = member.replace(f"{source}-{base}/", f"{source}-{base}.orig/", 1)
        new = member if not unsafe else f"{source}-{base}/{subtree}/../../escape"
        context = "    int wrong;" if bad_context else "    int value;"
        diff = f"--- {old}\n+++ {new}\n@@ -1,3 +1,4 @@\n struct test {{\n {context}\n+    void *extra;\n }};\n"
        delta.write_bytes(gzip.compress(diff.encode()))
        dsc = "Format: 1.0\nChecksums-Sha256:\n"
        for file in [orig, delta]:
            dsc += f" {hashlib.sha256(file.read_bytes()).hexdigest()} {file.stat().st_size} {file.name}\n"
        dsc += "Files:\n"
        (self.cache / f"{source}_{version}.dsc").write_text(dsc)
        return source, version, kernel

    def test_exact_modern_source_is_patched_offline(self):
        metadata = self.fixture()
        with patch.object(hwe, "POOL", "https://invalid.example"):
            hwe.prepare(*metadata, self.build, self.cache)
        self.assertIn("void *extra;", (self.build / "hda/common/hda_local.h").read_text())
        self.assertIn(metadata[1], (self.build / "ubuntu-hwe-source.txt").read_text())

    def test_legacy_hda_layout(self):
        hwe.prepare(*self.fixture(modern=False), self.build, self.cache)
        self.assertTrue((self.build / "hda/common/hda_local.h").exists())

    def test_corrupted_source_is_rejected_and_removed(self):
        metadata = self.fixture()
        archive = next(self.cache.glob("*.orig.tar.gz"))
        archive.write_bytes(b"broken")
        with self.assertRaisesRegex(RuntimeError, "SHA-256"):
            hwe.prepare(*metadata, self.build, self.cache)
        self.assertFalse(archive.exists())
        self.assertFalse((self.build / "hda").exists())

    def test_corrupted_diff_is_rejected(self):
        metadata = self.fixture()
        next(self.cache.glob("*.diff.gz")).write_bytes(b"broken")
        with self.assertRaisesRegex(RuntimeError, "SHA-256"):
            hwe.prepare(*metadata, self.build, self.cache)

    def test_failed_patch_is_not_silently_accepted(self):
        with self.assertRaises(subprocess.CalledProcessError):
            hwe.prepare(*self.fixture(bad_context=True), self.build, self.cache)
        self.assertFalse((self.build / "ubuntu-hwe-source.txt").exists())

    def test_unsafe_patch_path_rejected(self):
        with self.assertRaisesRegex(RuntimeError, "Unsafe HDA patch path"):
            hwe.prepare(*self.fixture(unsafe=True), self.build, self.cache)

    def test_reject_unsupported_packaging(self):
        digest = "0" * 64
        with self.assertRaisesRegex(RuntimeError, "format-1.0"):
            hwe.source_files(f"Checksums-Sha256:\n {digest} 10 source.debian.tar.xz\nFiles:\n")

    def test_reject_unsafe_checksum_filename(self):
        digest = "0" * 64
        with self.assertRaisesRegex(RuntimeError, "Invalid Ubuntu source"):
            hwe.source_files(f"Checksums-Sha256:\n {digest} 10 ../escape.diff.gz\nFiles:\n")

    def test_dkms_staging_copies_helper_without_touching_system(self):
        repo = Path(__file__).resolve().parents[1]
        fixture_repo = self.root / "repo"
        fixture_repo.mkdir()
        for name in ["dkms.conf", "Makefile_cirrus", "Makefile_cs420x",
                     "prepare.cirrus.driver.sh", "prepare.ubuntu-hwe.py"]:
            shutil.copy2(repo / name, fixture_repo / name)
        shutil.copytree(repo / "patch_cirrus", fixture_repo / "patch_cirrus")
        staged = self.root / "usr-src"
        script = (repo / "dkms.sh").read_text()
        replacements = {
            'source_dir="/usr/src/${module_name}-${module_version}"': f'source_dir="{staged}"',
            'cache_dir="/var/cache/macbook12-audio-driver"': f'cache_dir="{self.cache}"',
            'if (( EUID != 0 )); then': 'if false; then',
        }
        for old, new in replacements.items():
            self.assertIn(old, script)
            script = script.replace(old, new)
        (fixture_repo / "dkms.sh").write_text(script)
        bin_dir = self.root / "bin"
        bin_dir.mkdir()
        fake_dkms = bin_dir / "dkms"
        fake_dkms.write_text('#!/bin/sh\n[ "$1" != status ]\n')
        fake_dkms.chmod(0o755)
        env = dict(os.environ, PATH=str(bin_dir) + os.pathsep + os.environ["PATH"])
        subprocess.run(["bash", str(fixture_repo / "dkms.sh"), "install"], env=env, check=True)
        self.assertEqual((staged / "prepare.ubuntu-hwe.py").read_bytes(),
                         (repo / "prepare.ubuntu-hwe.py").read_bytes())
        self.assertTrue((staged / "prepare.cirrus.driver.sh").exists())

    def test_reject_non_hwe_source(self):
        with self.assertRaisesRegex(RuntimeError, "Not an Ubuntu HWE"):
            hwe.prepare("linux", "7.0.0-43.43", "7.0.0-43-generic", self.build, self.cache)


if __name__ == "__main__":
    unittest.main()
