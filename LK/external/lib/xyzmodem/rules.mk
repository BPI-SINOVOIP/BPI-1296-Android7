LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	lib/cksum

MODULE_SRCS += \
	$(LOCAL_DIR)/main.c \
	$(LOCAL_DIR)/xyzModem.c \
	$(LOCAL_DIR)/crc16.c
	
include make/module.mk
