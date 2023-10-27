ifdef KERNELRELEASE
	KERNELDIR := /lib/modules/$(KERNELRELEASE)
else
	KERNELDIR := /lib/modules/$(shell uname -r)
endif

KERNELBUILD := $(KERNELDIR)/build

all:
	make -C $(KERNELBUILD) M=$(shell pwd)/build/hda modules

clean:
	make -C $(KERNELBUILD) M=$(shell pwd)/build/hda clean

ifndef KERNELRELEASE
install:
	cp $(shell pwd)/build/hda/snd-hda-codec-cirrus.ko $(KERNELDIR)/updates
	depmod -a
endif
