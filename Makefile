obj-m := hello_world.o

KDIR := /home/gvigelet/nvidia-orin/Linux_for_Tegra/source/kernel/kernel-jammy-src
CROSS_COMPILE := /opt/aarch64--glibc--stable-2022.08-1/bin/aarch64-buildroot-linux-gnu-
ARCH := arm64

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	