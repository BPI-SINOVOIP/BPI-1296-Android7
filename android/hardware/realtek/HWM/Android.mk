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

#ifeq ($(USE_RTK_CEC_CONTROL),YES)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := rtk_hwm.c

#LOCAL_CFLAGS += -std=c99

LOCAL_CFLAGS += -std=c99 -Wall -Wno-unused-parameter -Wno-missing-field-initializers -Werror=unused-variable

LOCAL_C_INCLUDES += \
	hardware/realtek/realtek_omx/rtk_libs/common \
	hardware/realtek/realtek_omx/rtk_libs/common/IPC/generate/include/system \
	hardware/realtek/realtek_omx/rtk_libs/common/IPC/include \
	device/realtek/proprietary/libs/libMCPControl/inc \
	#device/realtek/proprietary/libs/Include/TEEapi \
	#device/realtek/proprietary/libs/Include/Platform_Lib/MCPControl

#LOCAL_CFLAGS := -DCEC_HAL_TRACE
LOCAL_32_BIT_ONLY := true
	
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libRTK_lib
#LOCAL_SHARED_LIBRARIES += libTEEapi
		

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE := rtk_hwm.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional	

	
include $(BUILD_SHARED_LIBRARY)

#endif
