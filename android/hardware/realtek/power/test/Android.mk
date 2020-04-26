LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test-power
LOCAL_MODULE_TAGS := optional tests
LOCAL_CFLAGS := -DLOG_TAG=\"PowerTest\"

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_SHARED_LIBRARIES += libhardware
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libpowermanager


LOCAL_SRC_FILES := main.cpp

include $(BUILD_EXECUTABLE)
