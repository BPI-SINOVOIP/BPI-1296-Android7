LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test-allocator
LOCAL_MODULE_TAGS := optional tests
LOCAL_CFLAGS := -DLOG_TAG=\"TestAllocator\"

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_SRC_FILES := main.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include

include $(BUILD_EXECUTABLE)
