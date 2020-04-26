LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS :=

MODULE_SRCS += \
	$(LOCAL_DIR)/tar.c
	
include make/module.mk
