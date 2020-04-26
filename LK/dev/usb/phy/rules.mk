LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS += \
	$(LOCAL_DIR)/phy-rtk-usb2.c \
	$(LOCAL_DIR)/phy-rtk-usb3.c \


include make/module.mk
