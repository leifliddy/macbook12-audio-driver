#!/bin/bash

switch_audio_driver='sudo /usr/local/bin/switch.audio.driver.sh'

change_active_sink() {
  make_active_sink=$1

  pacmd set-default-sink $make_active_sink
  for playing in $(pacmd list-sink-inputs | awk '$1 == "index:" {print $2}')
  do
        pacmd move-sink-input $playing $make_active_sink &> /dev/null
  done
}

alsa_sink=$(pacmd list-sinks | grep alsa | grep name: | cut -d' ' -f2 | sed 's/<//' | sed 's/>//')  #ie alsa_output.hw_0_0
dummy_sink='mydummysink'

[[ -z $(pacmd list-sinks | grep $dummy_sink) ]] && pacmd load-module module-null-sink sink_name=$dummy_sink

change_active_sink $dummy_sink
$switch_audio_driver
change_active_sink $alsa_sink
