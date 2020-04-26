LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	dev/usb/phy

MODULE_SRCS += \
	$(LOCAL_DIR)/udc-core.c \
	$(LOCAL_DIR)/udc-common.c \
	$(LOCAL_DIR)/gadget_fastboot.c \
	$(LOCAL_DIR)/udc-rtk.c \
	$(LOCAL_DIR)/otg-rtk.c \
	$(LOCAL_DIR)/dwc3-rtk.c \
	$(LOCAL_DIR)/dwc3-core.c \
	$(LOCAL_DIR)/dwc3-core-intr.c \
	$(LOCAL_DIR)/dwc3-device.c \
	$(LOCAL_DIR)/dwc3-device-intr.c \
	$(LOCAL_DIR)/dwc3-ep0.c \
	$(LOCAL_DIR)/dwc3-gadget.c \
	$(LOCAL_DIR)/dwc3-debug.c \


include make/module.mk
