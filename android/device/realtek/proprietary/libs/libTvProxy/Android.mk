LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libTvProxy
LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := liblog libutils libbinder

LOCAL_SRC_FILES := TvProxy.cpp DvbChannelInfo.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Include \
	$(TOP)/frameworks/native/include/utils

include $(BUILD_SHARED_LIBRARY)