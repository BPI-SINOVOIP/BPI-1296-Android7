#
# Makefile for HDMITX linux kernel module.
#
ccflags-y += -I$(srctree)/drivers/video/fbdev/rtk
ccflags-y += -I$(srctree)/drivers/staging/android

#ccflags-y += -DAO_ON_SCPU

hdmitx-$(CONFIG_RTK_HDMITX):= hdmitx_core.o switch_hdmitx.o hdmitx_api.o rtk_edid.o compat_hdmitx.o hdmitx_info.o \
	hdmitx_scdc.o rtk_edid_filter.o rtk_mute_gpio.o

hdmitx-$(CONFIG_RTK_HDMITX) += hdmitx_dbg.o

obj-$(CONFIG_RTK_HDMITX)+= hdmitx.o

