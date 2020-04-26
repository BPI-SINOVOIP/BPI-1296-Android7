LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS := \
	lib/sysparam \
	dev/usb/udc \
	external/lib/tar \
	external/lib/rtk_storage_layout

MODULE_SRCS += \
	$(LOCAL_DIR)/fastboot.c \
	$(LOCAL_DIR)/fastboot_usb.c \
	$(LOCAL_DIR)/fastboot_cmd.c \
	$(LOCAL_DIR)/sparse_img.c \
	$(LOCAL_DIR)/tar_img.c

ifeq ($(USB_FASTBOOT_SETTING),1)
GLOBAL_DEFINES += \
	FASTBOOT_BUFFER_ADDR=$(FASTBOOT_BUFFER_ADDR) \
	FASTBOOT_BUFFER_MAX_SIZE=$(FASTBOOT_BUFFER_MAX_SIZE)
endif

include make/module.mk
