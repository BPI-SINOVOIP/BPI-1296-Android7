LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

GLOBAL_INCLUDES += \
        $(LOCAL_DIR)/uartwrite/inc/ 

MODULE_SRCS += \
	$(LOCAL_DIR)/uartwrite/uart_write.c 

MODULE_EXTRA_OBJS += \
	$(LOCAL_DIR)/keyset/keyset.o \
	$(LOCAL_DIR)/keyset/sha1.o

include make/module.mk
