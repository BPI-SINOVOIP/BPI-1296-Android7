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

ifeq ($(USE_RTK_VOUT_UTIL), YES)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig

#LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_SRC_FILES := test_VoutUtil.cpp \
				   VoutUtilWrapper.cpp \
				   vout_util_cmdline.c \
				   prepare_method_parameters.cpp

LOCAL_MODULE := test_VoutUtil
LOCAL_CFLAGS:= -DLOG_TAG=\"test_VoutUtil\"
LOCAL_CFLAGS+= -DLOG_NDEBUG=0
LOCAL_MODULE_TAGS := optional


LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libRTK_lib \
						  libRTKSetupClass \
						  libRTKHDMIControl \
						  libRTKVoutUtil

LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1
LOCAL_CFLAGS += -fPIC
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../../../Include \
    system/core/include/cutils \
    $(LOCAL_PATH)/../../../rtk_libs/common \
    $(LOCAL_PATH)/../../../rtk_libs/common/IPC/generate/include/system \
    $(LOCAL_PATH)/../../../rtk_libs/common/IPC/include \
    $(LOCAL_PATH)/../../../rtk_libs/common/IPC/include/xdr \
    $(LOCAL_PATH)/../../../rtk_libs/common/IPC/src/xdr \
    $(LOCAL_PATH)/../../../rtk_libs/OSAL/include \
    $(LOCAL_PATH)/../../../rtk_libs/OSAL/include/OSAL


include $(BUILD_EXECUTABLE)

endif # USE_RTK_VOUT_UTIL
