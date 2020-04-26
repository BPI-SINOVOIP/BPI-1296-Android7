LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

# MODULE_DEPS +=

MODULE_SRCS += \
     $(LOCAL_DIR)/rtk_i2c-lib.c \
     $(LOCAL_DIR)/rtk_i2c-pow.c

	
include make/module.mk
