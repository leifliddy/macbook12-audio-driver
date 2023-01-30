snd-hda-codec-cirrus-objs :=	patch_cirrus.o
obj-$(CONFIG_SND_HDA_CODEC_CIRRUS) += snd-hda-codec-cirrus.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	strip --strip-debug snd-hda-codec-cirrus.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

install:
	cp snd-hda-codec-cirrus.ko /lib/modules/$(shell uname -r)/updates/snd-hda-codec-cirrus.ko_speaker
	ln -srf /lib/modules/$(shell uname -r)/updates/snd-hda-codec-cirrus.ko_speaker /lib/modules/$(shell uname -r)/updates/snd-hda-codec-cirrus.ko
	depmod -a
	@#uncomment these lines if /etc/pulse/default.pa has been modified with the changes listed in pulse_audio_configs/default.pa
	@#killall alsactl &> /dev/null
	@#modprobe -r snd_hda_intel
	@#modprobe -r snd_hda_codec_cirrus
	@#modprobe snd_hda_codec_cirrus
	@#modprobe snd_hda_intel
	@#sleep 2
	@#killall pulseaudio &> /dev/null
	@#sleep 2
