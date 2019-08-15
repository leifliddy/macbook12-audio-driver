snd-hda-codec-cirrus-objs :=	patch_cirrus.o
obj-$(CONFIG_SND_HDA_CODEC_CIRRUS) += snd-hda-codec-cirrus.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	cp snd-hda-codec-cirrus.ko /lib/modules/$(shell uname -r)/updates/snd-hda-codec-cirrus.ko_speaker
	ln -sf /lib/modules/$(shell uname -r)/updates/snd-hda-codec-cirrus.ko_speaker /lib/modules/$(shell uname -r)/updates/snd-hda-codec-cirrus.ko
	#symlinks -c /lib/modules/$(shell uname -r)/updates 	#uncomment to use relative path links 
	depmod -a
	killall -9 alsactl &> /dev/null
	modprobe -r snd_hda_intel
	modprobe -r snd_hda_codec_cirrus
	modprobe snd_hda_codec_cirrus
	modprobe snd_hda_intel
	sleep 2
	killall pulseaudio    #ensure autospawn=yes and pulseaudio will auto-restart (default setting)
