-- MacBook9,1 / MacBook10,1 CS4208 - force software volume (WirePlumber 0.4)
--
-- The internal speaker has no usable hardware volume control (the codec's only
-- analog amp is wired to the headphone path), so PipeWire must apply volume in
-- software for this card. Without this the volume slider does nothing on the
-- speakers. Applied at the device level so it takes effect before the card
-- profile's mixer paths are set up.
--
-- Install to: /etc/wireplumber/main.lua.d/51-macbook-cs4208-softvol.lua
-- then: systemctl --user restart wireplumber
--
-- If your audio controller is not at PCI 00:1f.3, adjust device.name
-- (find it with `wpctl status` or `pactl list cards short`).

table.insert(alsa_monitor.rules, {
  matches = {
    { { "device.name", "equals", "alsa_card.pci-0000_00_1f.3" } },
  },
  apply_properties = {
    ["api.alsa.soft-mixer"] = true,
  },
})
