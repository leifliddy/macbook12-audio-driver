# Ubuntu HWE source selection

PR #66 addresses the private-HDA-header mismatch for ordinary Ubuntu kernels
by using the distribution's `linux-source` binary package. HWE kernels can lack
that package; they need the source corresponding to their installed headers,
not a same-numbered vanilla kernel or a source package from a different ABI.

## Failure observed on hardware

MacBook10,1 / CS4208, Linux Mint on Ubuntu 24.04 HWE, kernel
`7.0.0-43-generic`, source `linux-hwe-7.0 7.0.0-43.43~24.04.1`.
A stale installer used the vanilla Linux 7.0 private headers. This produced
UBSAN array-index-out-of-bounds reports in the generic HDA parser, then a page
fault in `cs_4208_build_pcms_explicit`; ALSA exposed no cards.

The Ubuntu source diff adds `share_spdif_kctl` to `struct hda_multi_out` in
`sound/hda/common/hda_local.h`. Its size changes by eight bytes, shifting the
subsequent members of the enclosing `hda_gen_spec`. Matching `vermagic` and
successful DKMS compilation did not detect this private ABI mismatch.

A locally repaired build using the matching Ubuntu HDA header patches restored
ALSA enumeration and audible internal-speaker playback after reboot. That local
module predates this PR and retains unrelated local microphone modifications;
it is not evidence that the exact PR module has been runtime-tested.

## Verification of this patch

- Offline tests: `python3 -m unittest discover -s tests -v`.
  Covers modern/legacy extraction layouts, cached checksum verification,
  corrupt archive/diff rejection, failed patch handling, unsafe paths and
  unsupported source formats. Test fixtures are synthetic, not hardware tests.
- Syntax: `bash -n prepare.cirrus.driver.sh`.
- Real preparation on `7.0.0-43-generic`: the exact Ubuntu orig archive and
  source diff were downloaded; 29 HDA file patches applied successfully.
- Real build against the installed target headers:

  ```sh
  MACBOOK12_AUDIO_KERNEL_CACHE="$PWD/.kernel-cache" \
    ./prepare.cirrus.driver.sh 7.0.0-43-generic
  make -j4 KERNELRELEASE=7.0.0-43-generic KCFLAGS=-g
  ```

- Debug information from the resulting `cs420x.o` agrees with the loaded
  distribution `snd_hda_codec_generic` module's BTF:

  | Layout | Bytes |
  | --- | ---: |
  | `sizeof(struct hda_gen_spec)` | 6008 |
  | `sizeof(struct hda_multi_out)` | 104 |
  | `offsetof(hda_gen_spec, pcm_rec)` | 2120 |
  | `offsetof(hda_gen_spec, autocfg)` | 2144 |

This branch was built but deliberately not installed or loaded over the working
local driver. Full runtime verification of the exact PR module, suspend/resume,
other HWE releases and subsequent kernel upgrades remain untested. The legacy
layout is covered by a fixture, not a real 6.x HWE build.

## Boundaries

The new helper is selected only for headers whose source package is
`linux-hwe-*`; ordinary Debian/Ubuntu source selection and kernel.org source
selection are unchanged. The target package is queried using the requested
kernel release, never the running kernel.

The helper currently supports Ubuntu format-1.0 source packaging and downloads
from the public Ubuntu archive over HTTPS. SHA-256 is checked against the DSC;
this is not independent OpenPGP signature verification. If a source version is
removed from that archive, downloading fails rather than silently substituting
a newer or vanilla source tree. The existing cache override can be used to
prepopulate verified source files for offline builds.
