LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test-screen-record
LOCAL_MODULE_TAGS := optional tests
LOCAL_CFLAGS := -DLOG_TAG=\"TestScreenRecord\"

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libRtkScreenRecord
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include

LOCAL_SRC_FILES := main.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include
include $(BUILD_EXECUTABLE)
