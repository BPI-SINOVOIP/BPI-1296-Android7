
ifneq ($(KERNELRELEASE),)
	obj-m := rtk_btusb.o
	rtk_btusb-y = rtk_coex.o rtk_bt.o
else
	KSRC := $(LINUX_KERNEL_PATH)

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE="$(CROSS_COMPILE)" -C $(KSRC) M=$(shell pwd) modules

clean:
	rm -rf *.o *.mod.c *.mod.o *.ko *.symvers *.order *.a

endif
