LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(ENABLE_RTK_HDMIRX_SERVICE), true)
LOCAL_SRC_FILES += com_realtek_hardware_RtkHDMIRxManager.cpp
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/transcode
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include
LOCAL_SHARED_LIBRARIES += libcamera_client
LOCAL_SHARED_LIBRARIES += libRtkTranscodePlayer
LOCAL_SHARED_LIBRARIES += libRtkIpc
LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libgui
LOCAL_SHARED_LIBRARIES += libnativehelper
LOCAL_SHARED_LIBRARIES += libandroid_runtime
LOCAL_CFLAGS += -DENABLE_RTK_HDMIRX_MANAGER
endif

ifeq ($(ENABLE_RT_WIFIDISPLAY),true)
LOCAL_SRC_FILES += com_realtek_app_RtkMiracastSinkJNI.cpp

LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs/wfdisplay
LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs/wfdisplay/inband
LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs/libRTMediaPlayer

LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libstdc++
LOCAL_SHARED_LIBRARIES += libcrypto
LOCAL_SHARED_LIBRARIES += libhdcp2
LOCAL_SHARED_LIBRARIES += libwfdisplay
LOCAL_SHARED_LIBRARIES += libRTMediaPlayer
LOCAL_SHARED_LIBRARIES += libRT_MediaPlayClient
LOCAL_SHARED_LIBRARIES += libgui
LOCAL_SHARED_LIBRARIES += libstagefright_foundation
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libinput
LOCAL_SHARED_LIBRARIES += libinputservice
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libwfdinband

LOCAL_CFLAGS += -DENABLE_RTK_MIRACAST_SINK_SERVICE
LOCAL_CFLAGS += -DHDCP_REPEATER=0
LOCAL_CFLAGS += -DHDCP2_KEY_PROTECT_WITH_KIMG
LOCAL_CFLAGS += -DHDCP2_1
#                         -DREALTEK_WFDISPLAY_SIGMA
endif

LOCAL_SRC_FILES += onload.cpp

LOCAL_MODULE := librealtek_jni
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

