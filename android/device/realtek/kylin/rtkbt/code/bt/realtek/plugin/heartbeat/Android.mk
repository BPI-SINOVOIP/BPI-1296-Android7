LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES += heartbeat.c \
                   rtk_poll.c  \

LOCAL_C_INCLUDES := $(rtk_local_C_INCLUDES)
LOCAL_CONLYFLAGS += $(bluetooth_CONLYFLAGS)
LOCAL_CFLAGS += $(rtk_local_CFLAGS)
LOCAL_MODULE_RELATIVE_PATH := rtkbt
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := libcutils libc
LOCAL_MODULE := heartbeat
LOCAL_MULTILIB := 32

include $(BUILD_SHARED_LIBRARY)
