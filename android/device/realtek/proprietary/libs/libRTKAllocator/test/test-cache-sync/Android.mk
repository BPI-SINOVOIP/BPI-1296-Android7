LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test-cache-sync
LOCAL_MODULE_TAGS := optional tests
LOCAL_CFLAGS := -DLOG_TAG=\"TestCacheSync\"
LOCAL_CFLAGS += -DMODULE_NAME=\"test-cache-sync\"

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_SRC_FILES := main.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include


LOCAL_SHARED_LIBRARIES += libhwse.$(TARGET_BOARD_PLATFORM)
LOCAL_SHARED_LIBRARIES += libRtkIpc     # For SeMemcpy
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include
LOCAL_C_INCLUDES += hardware/realtek/StreamingEngine2

include $(BUILD_EXECUTABLE)
