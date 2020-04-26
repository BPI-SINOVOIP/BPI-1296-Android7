# Copyright 2011 The Android Open Source Project

#AUDIO_POLICY_TEST := true
#ENABLE_AUDIO_DUMP := true

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    AudioHardwareInterface.cpp \
    audio_hw_hal.cpp

LOCAL_MODULE := libaudiohw_legacy
LOCAL_STATIC_LIBRARIES := libmedia_helper
LOCAL_CFLAGS := -Wno-unused-parameter -Wno-gnu-designator
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/../include

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    AudioPolicyManagerBase.cpp \
    AudioPolicyCompatClient.cpp \
    audio_policy_hal.cpp

ifeq ($(AUDIO_POLICY_TEST),true)
  LOCAL_CFLAGS += -DAUDIO_POLICY_TEST
endif

LOCAL_STATIC_LIBRARIES := libmedia_helper
LOCAL_MODULE := libaudiopolicy_legacy
LOCAL_CFLAGS += -Wno-unused-parameter
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/../include

#rtk begin
ifeq ($(USE_RTK_AOUT_UTIL), YES)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1
LOCAL_CFLAGS += -DUSE_RTK_AOUT_UTIL
LOCAL_C_INCLUDES += \
    device/realtek/proprietary/libs/Include \
    device/realtek/proprietary/libs/Include/Application/AppClass \
    device/realtek/proprietary/libs/rtk_libs/common \
    device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system \
    device/realtek/proprietary/libs/rtk_libs/common/IPC/include \
    device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr \
    device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr \
    device/realtek/proprietary/libs/rtk_libs/OSAL/include \
    device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL

#LOCAL_SHARED_LIBRARIES += libRTKAoutUtil
endif

LOCAL_C_INCLUDES += device/realtek/proprietary/libs

#rtk end

include $(BUILD_STATIC_LIBRARY)

# The default audio policy, for now still implemented on top of legacy
# policy code
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    AudioPolicyManagerDefault.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    liblog \
    libbinder

LOCAL_STATIC_LIBRARIES := \
    libmedia_helper

LOCAL_WHOLE_STATIC_LIBRARIES := \
    libaudiopolicy_legacy

LOCAL_MODULE := audio_policy.default
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_CFLAGS := -Wno-unused-parameter
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

#rtk begin
ifeq ($(USE_RTK_AOUT_UTIL), YES)
LOCAL_SHARED_LIBRARIES += libRTKAoutUtil
endif

LOCAL_C_INCLUDES += device/realtek/proprietary/libs
LOCAL_SHARED_LIBRARIES += libstagefright_foundation
LOCAL_SHARED_LIBRARIES += libRtkAudioRedirect
#rtk end

include $(BUILD_SHARED_LIBRARY)

#ifeq ($(ENABLE_AUDIO_DUMP),true)
#  LOCAL_SRC_FILES += AudioDumpInterface.cpp
#  LOCAL_CFLAGS += -DENABLE_AUDIO_DUMP
#endif
#
#ifeq ($(strip $(BOARD_USES_GENERIC_AUDIO)),true)
#  LOCAL_CFLAGS += -D GENERIC_AUDIO
#endif

#ifeq ($(BOARD_HAVE_BLUETOOTH),true)
#  LOCAL_SRC_FILES += A2dpAudioInterface.cpp
#  LOCAL_SHARED_LIBRARIES += liba2dp
#  LOCAL_C_INCLUDES += $(call include-path-for, bluez)
#
#  LOCAL_CFLAGS += \
#      -DWITH_BLUETOOTH \
#endif
#
#include $(BUILD_SHARED_LIBRARY)

#    AudioHardwareGeneric.cpp \
#    AudioHardwareStub.cpp \
