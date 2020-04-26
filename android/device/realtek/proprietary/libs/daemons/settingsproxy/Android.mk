LOCAL_PATH := $(call my-dir)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := settingsproxy
LOCAL_MODULES_TAGS := eng optional
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE := settingsproxy

include $(BUILD_PREBUILT)
