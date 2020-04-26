LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS += \
	external/lib/pmic

MODULE_SRCS += \
	$(LOCAL_DIR)/cpufreq.c

include make/module.mk
