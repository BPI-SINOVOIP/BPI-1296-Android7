LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
    external/lib/i2c

MODULE_SRCS += \
    $(LOCAL_DIR)/cmd_eeprom.c

include make/module.mk

