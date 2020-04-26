LOCAL_PATH := $(call my-dir)

#########################

#########################

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := UDPserver.c
LOCAL_MODULE := UDPserver
LOCAL_CFLAGS := -Os -Wall $(CFLAGS)
LOCAL_C_INCLUDES:=$(LOCAL_PATH)/../include

include $(BUILD_EXECUTABLE)

