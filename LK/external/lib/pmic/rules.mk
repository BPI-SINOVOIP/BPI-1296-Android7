LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	external/lib/i2c

MODULE_SRCS += \
	$(LOCAL_DIR)/pmic-core.c \
	$(LOCAL_DIR)/pmic-virt.c \
	$(LOCAL_DIR)/cmd_pmic.c

ifeq ($(USE_PMIC_G2227),1)
MODULE_SRCS += \
	$(LOCAL_DIR)/pmic-g2227.c
endif

ifeq ($(USE_PMIC_G2237),1)
MODULE_SRCS += \
	$(LOCAL_DIR)/pmic-g2237.c
endif

include make/module.mk
