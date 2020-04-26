# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_SHARED_LIBRARIES := liblog \
						  libutils \
						  libbinder \
						  libhardware \
						  libRTK_lib


LOCAL_SRC_FILES := hwm_api.c

LOCAL_MODULE := libRTKHWM

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += $(DEFINES)
LOCAL_CFLAGS += -fPIC 
LOCAL_C_INCLUDES += \
    system/core/include/cutils \
	hardware/realtek \
    $(LOCAL_PATH)/../rtk_libs/common \
    $(LOCAL_PATH)/../rtk_libs/common/IPC/generate/include/system \
    $(LOCAL_PATH)/../rtk_libs/common/IPC/include \
    $(LOCAL_PATH)/../rtk_libs/common/IPC/include/xdr \
    $(LOCAL_PATH)/../rtk_libs/common/IPC/src/xdr \
    $(LOCAL_PATH)/../rtk_libs/OSAL/include \
    $(LOCAL_PATH)/../rtk_libs/OSAL/include/OSAL
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

