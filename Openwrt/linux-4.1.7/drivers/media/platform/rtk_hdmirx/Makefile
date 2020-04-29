

OBJNAME := v4l2_hdmi_rx
ccflags-y += -I$(srctree)/drivers/media/platform/rtk_hdmirx/reg_include
ccflags-y += -I$(srctree)/drivers/media/platform/rtk_hdmirx/rx_drv
ccflags-y += -I$(srctree)/drivers/media/platform/rtk_hdmirx/
ccflags-y += -I$(srctree)/drivers/video/fbdev/rtk
ccflags-y += -I$(srctree)/drivers/staging/android
$(OBJNAME)-objs := v4l2_hdmi_dev.o hdmirx_video_dev.o mipi_wrapper.o hdmirx_wrapper.o hdmirx_reg.o hdmirx_hdcp.o hdmirx_repeater.o hdmirx_sysfs.o

ifdef CONFIG_RTK_HDCP1x_REPEATER
$(OBJNAME)-objs += cipher.lib
endif

ifdef CONFIG_RTK_HDCPRX_2P2
$(OBJNAME)-objs += hdmirx_hdcp2p2.o
endif

#obj-m	+= v4l2_hdmi_rx.o
obj-$(CONFIG_RTK_HDMIRX)	+= v4l2_hdmi_rx.o
obj-$(CONFIG_RTK_HDMIRX)	+= rx_drv/

