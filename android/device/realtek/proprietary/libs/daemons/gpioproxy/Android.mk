LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULES_TAGS := eng optional

LOCAL_MODULE := gpioproxy

LOCAL_SRC_FILES += CommandListener.cpp
LOCAL_SRC_FILES += ResponseCode.cpp
LOCAL_SRC_FILES += main.cpp

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/RtkGPIO  

LOCAL_SHARED_LIBRARIES += libsysutils
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libRTKGPIO

include $(BUILD_EXECUTABLE)

