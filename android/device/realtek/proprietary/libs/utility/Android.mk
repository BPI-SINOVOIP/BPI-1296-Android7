LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES += chfb.cpp

LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcutils

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= chfb

include $(BUILD_EXECUTABLE)

