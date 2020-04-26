LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS := \
  lib/lwip \

MODULE_SRCS += \
  $(LOCAL_DIR)/ping.c \

include make/module.mk
