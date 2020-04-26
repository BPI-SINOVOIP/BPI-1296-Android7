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

ifeq ($(USE_RTK_SETUP_CLASS), YES)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include hardware/realtek/realtek_omx/rtk_libs/system/include/MakeConfig

#LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SRC_FILES := setupClassTool.cpp

LOCAL_MODULE := setupClassTool
LOCAL_CFLAGS:= -DLOG_TAG=\"setupClassTool\"
LOCAL_CFLAGS+= -DLOG_NDEBUG=0
LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES += libRTKSetupClass \
						  libcutils
LOCAL_SHARED_LIBRARIES += libsettingsclient

LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1
LOCAL_CFLAGS += -DANDROID

#LOCAL_CFLAGS += -fPIC
LOCAL_C_INCLUDES += \
    device/realtek/libs/Include \
	bionic \
	external/stlport/stlport \
    system/core/include/cutils \
    hardware/realtek/realtek_omx/rtk_libs/common \
    hardware/realtek/realtek_omx/rtk_libs/common/IPC/generate/include/system \
    hardware/realtek/realtek_omx/rtk_libs/common/IPC/include \
    hardware/realtek/realtek_omx/rtk_libs/common/IPC/include/xdr \
    hardware/realtek/realtek_omx/rtk_libs/common/IPC/src/xdr \
    hardware/realtek/realtek_omx/rtk_libs/OSAL/include \
    hardware/realtek/realtek_omx/rtk_libs/OSAL/include/OSAL \

include $(BUILD_EXECUTABLE)

endif # USE_RTK_SETUP_CLASS
