#
# Makefile for CEC linux kernel module.
#

ccflags-y += -I$(srctree)/drivers/video/fbdev/rtk
ccflags-y += -I$(srctree)/drivers/media/platform/rtk_mhltx
ccflags-y += -I$(srctree)/drivers/staging/android

EXTRA_CFLAGS += -I ../

obj-$(CONFIG_RTD1295_CEC) += cec_rtd1295.o
