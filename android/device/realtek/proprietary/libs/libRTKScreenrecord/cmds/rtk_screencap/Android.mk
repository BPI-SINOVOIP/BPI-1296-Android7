LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    screencap.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \
    libskia \
    libui \
    libgui

LOCAL_CFLAGS += -DUSE_RTK_RECORD
LOCAL_SHARED_LIBRARIES += libRtkScreenRecord
LOCAL_SHARED_LIBRARIES += libRTKAllocator
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES += rtk-record.cpp

LOCAL_MODULE:= rtk_screencap

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wall -Werror -Wunused -Wunreachable-code

include $(BUILD_EXECUTABLE)
