
################################################################################

LOCAL_PATH:= $(call my-dir)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := camera.kylin.so
LOCAL_MODULE_TAGS:= optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := camera3

include $(BUILD_PREBUILT)

