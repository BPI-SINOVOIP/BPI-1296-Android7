LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	dev/usb/phy

GLOBAL_INCLUDES += \
        $(LOCAL_DIR)/include

MODULE_SRCS += \
	$(LOCAL_DIR)/cmd_usb.c \
	$(LOCAL_DIR)/cmd_fat.c \
	$(LOCAL_DIR)/fat.c \
	$(LOCAL_DIR)/usb_storage.c \
	$(LOCAL_DIR)/usb.c \
	$(LOCAL_DIR)/ehci-hcd.c \
	$(LOCAL_DIR)/ehci-rtk.c \
	$(LOCAL_DIR)/usb_hub.c \
	$(LOCAL_DIR)/part.c \
	$(LOCAL_DIR)/part_dos.c \
	$(LOCAL_DIR)/xhci.c \
	$(LOCAL_DIR)/xhci-mem.c \
	$(LOCAL_DIR)/xhci-plat.c \
	$(LOCAL_DIR)/xhci-ring.c \
	$(LOCAL_DIR)/host-rtk.c \

include make/module.mk
