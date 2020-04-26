LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := libRtkRtsp.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libRtkRtsp

include $(BUILD_PREBUILT)

###########################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := RtspServer
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS := debug
LOCAL_MODULE := RtspServer

include $(BUILD_PREBUILT)

