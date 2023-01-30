#!/bin/bash

hda_dir="/lib/modules/$(uname -r)/kernel/sound/pci/hda"
update_dir="/lib/modules/$(uname -r)/updates"

cirrus_headphones=$(find $hda_dir -type f | grep snd-hda-codec-cirrus.ko)  	#module from stock kernel (headphones)  #will find .ko or .ko.xz module
cirrus_speaker="$update_dir/snd-hda-codec-cirrus.ko_speaker"  			#speaker module
cirrus_active="$update_dir/snd-hda-codec-cirrus.ko"  				#active module (symlink)

speaker_module_active=$(readlink -f $cirrus_active | grep '_speaker$')

reload_driver() {
   depmod -a
   killall alsactl &> /dev/null
   modprobe -r snd_hda_intel
   modprobe -r snd_hda_codec_cirrus
   modprobe snd_hda_codec_cirrus
   modprobe snd_hda_intel
   sleep 2
   killall pulseaudio &> /dev/null 
   sleep 2
}

if [[ $speaker_module_active ]]; then
   echo 'switching to headphones'
   #echo '**if volume seems low, use alsa mixer to increase master volume for HDA Intel PCH card'
   ln -srf $cirrus_headphones $cirrus_active
   reload_driver
else
   echo 'switching to speakers'
   ln -srf $cirrus_speaker $cirrus_active
   reload_driver
fi
