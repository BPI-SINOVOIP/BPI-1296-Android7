LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS := libion.a
LOCAL_MODULE_TAGS:= optional eng

include $(BUILD_MULTI_PREBUILT)




