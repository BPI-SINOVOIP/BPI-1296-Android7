LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS := \

ifeq ($(PLATFORM_RTD1295),1)
MODULE_SRCS += \
  $(LOCAL_DIR)/sd_test.c \
  $(LOCAL_DIR)/mem_test.c \
  $(LOCAL_DIR)/ft_test.c
else
MODULE_SRCS += \
  $(LOCAL_DIR)/sd_test.c \
  $(LOCAL_DIR)/mem_test.c \
  $(LOCAL_DIR)/ft_test.c

endif

include make/module.mk
