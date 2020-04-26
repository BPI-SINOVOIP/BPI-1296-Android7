#ifeq ($(ENABLE_RT_WIFIDISPLAY),true)

LOCAL_PATH:= $(call my-dir)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libwfdisplay.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libwfdisplay

include $(BUILD_PREBUILT)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libhdcp2.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libhdcp2

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libwfdinband.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libwfdinband

include $(BUILD_PREBUILT)
#######################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := rtk_wfd
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE := rtk_wfd

include $(BUILD_PREBUILT)

