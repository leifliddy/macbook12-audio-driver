ifneq ($(KERNELRELEASE),)
	KERNELDIR ?= /lib/modules/$(KERNELRELEASE)/build
else
	## KERNELRELEASE not set.
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(shell pwd)/build/hda modules

clean:
	make -C $(KERNELDIR) M=$(shell pwd)/build/hda clean
	
ifeq ($(KERNELRELEASE),)
install:
	cp $(shell pwd)/build/hda/snd-hda-codec-cirrus.ko /lib/modules/$(shell uname -r)/updates
	depmod -a
endif
