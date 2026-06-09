# CS4208 audio driver for the 12-inch MacBook

Out-of-tree audio driver for the **Cirrus Logic CS4208** codec in the 12-inch
MacBook (**MacBook9,1** / 2016 and **MacBook10,1** / 2017).

The mainline kernel has no working configuration for this codec on these
machines: the codec is detected, but the speaker amplifier is never enabled, so
the built-in speakers stay silent. This module replays the initialization the
codec needs (reverse-engineered from macOS) and adds proper headphone/speaker
routing on top.

Based on davidjo's [snd_hda_macbookpro](https://github.com/davidjo/snd_hda_macbookpro)
and forked from leifliddy's [macbook12-audio-driver](https://github.com/leifliddy/macbook12-audio-driver).

## What works

- Internal speakers
- Headphone jack
- Automatic speaker / headphone switching on plug / unplug
- Volume control — needs one extra config step, see [Speaker volume](#3-speaker-volume-required)
- Internal microphone

## Supported hardware & kernels

|               |                                                       |
| ------------- | ----------------------------------------------------- |
| Models        | MacBook9,1 (2016), MacBook10,1 (2017)                 |
| Not supported | MacBook8,1 (2015)                                     |
| Kernels       | ≥ 5.0 (including the 6.17+ `sound/hda/codecs` layout) |
| Audio server  | PipeWire or PulseAudio                                 |

## Installation

### 1. Install build prerequisites

**Ubuntu / elementary / Debian**
```bash
sudo apt install dkms gcc make git wget linux-headers-$(uname -r)
```
**Fedora**
```bash
sudo dnf install dkms gcc make git wget kernel-devel
```
**Arch**
```bash
sudo pacman -S dkms gcc make git wget linux-headers
```

### 2. Build & install the module (DKMS — recommended)

DKMS rebuilds the module automatically whenever you install a new kernel, so
audio keeps working across kernel updates.

```bash
git clone https://github.com/breitburg/macbook12-audio-driver.git
cd macbook12-audio-driver
sudo ./install.cirrus.driver.sh -i
sudo reboot
```

> [!NOTE]
> The build downloads the matching kernel source tarball to obtain the
> HD-audio codec headers, so **an internet connection is required at build
> time** — including when DKMS rebuilds after a kernel update.

> [!IMPORTANT]
> Keep the cloned directory in place. The DKMS install links to it, and removing
> it will break the automatic rebuild on the next kernel update.

To uninstall:
```bash
sudo ./install.cirrus.driver.sh -u
```

<details>
<summary>Manual build (fallback — does <b>not</b> survive kernel updates)</summary>

```bash
git clone https://github.com/breitburg/macbook12-audio-driver.git
cd macbook12-audio-driver
sudo ./install.cirrus.driver.sh
sudo reboot
```
</details>

### 3. Speaker volume (required)

The internal speaker has **no usable hardware volume control** — the codec's only
analog amplifier is wired to the headphone path. PipeWire therefore has to apply
volume in software for this card. **Without this step the volume slider appears
to do nothing on the speakers.**

Check your WirePlumber version first:
```bash
wireplumber --version
```

**WirePlumber 0.5+** (most current distros) — create
`/etc/wireplumber/wireplumber.conf.d/51-macbook-cs4208-softvol.conf`:
```
monitor.alsa.rules = [
  {
    matches = [ { device.name = "alsa_card.pci-0000_00_1f.3" } ]
    actions = { update-props = { api.alsa.soft-mixer = true } }
  }
]
```

**WirePlumber 0.4** — create
`/etc/wireplumber/main.lua.d/51-macbook-cs4208-softvol.lua`:
```lua
table.insert(alsa_monitor.rules, {
  matches = {
    { { "device.name", "equals", "alsa_card.pci-0000_00_1f.3" } },
  },
  apply_properties = {
    ["api.alsa.soft-mixer"] = true,
  },
})
```

Copy-paste-ready versions of both live in [`contrib/wireplumber/`](contrib/wireplumber).

Then pin the hardware amps to full, persist them, and restart WirePlumber:
```bash
amixer -c0 sset Master 100% unmute
sudo alsactl store
systemctl --user restart wireplumber
```

> [!NOTE]
> On these MacBooks the audio controller is at PCI `00:1f.3`, which is what the
> config matches. If yours differs, find the card name with
> `wpctl status` or `pactl list cards short`.

## Verifying

```bash
# confirm this module (not the in-tree stub) is loaded:
modinfo snd_hda_codec_cs420x | grep filename     # should point at .../updates/...

# then play a test sound and plug/unplug headphones — audio should follow:
wpctl status
```

## How it works & limitations

The CS4208 in these MacBooks drives the **speakers over a digital link**
(converter `0x0a` → pin `0x1d`, with the amplifier enabled by a GPIO) and the
**headphones over the analog DAC** (converter `0x02` → pin `0x10`). The module
replays the macOS init for both paths and, on a jack interrupt, re-points the
active stream onto the correct converter (disabling the speaker pin so it
doesn't bleed while headphones are in).

- Switching is done at the codec level and cooperates with PipeWire's own jack
  handling.
- Because the speaker relies on software volume, **very low headphone levels can
  sound slightly compressed** (reduced effective bit depth). Normal listening
  levels are unaffected.

## Credits

- **davidjo** — original [snd_hda_macbookpro](https://github.com/davidjo/snd_hda_macbookpro) reverse-engineering this is built on
- **leifliddy** — [macbook12-audio-driver](https://github.com/leifliddy/macbook12-audio-driver), the base for this fork
