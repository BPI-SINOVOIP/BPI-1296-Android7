LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	lib/sysparam

MODULE_SRCS += \
	$(LOCAL_DIR)/fdt_main.c \
	$(LOCAL_DIR)/fdt_cmd.c
	
include make/module.mk
