# Copyright (C) 2014 The Android Open Source Project
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

# Make the JNI part
# ============================================================
# include $(CLEAR_VARS)

# LOCAL_CFLAGS += -Wno-unused-parameter -Wno-int-to-pointer-cast
# LOCAL_CFLAGS += -Wno-maybe-uninitialized -Wno-parentheses
# LOCAL_CPPFLAGS += -Wno-conversion-null

# LOCAL_C_INCLUDES += \
# 	$(JNI_H_INCLUDE) 

# LOCAL_SHARED_LIBRARIES += \
# 	libnativehelper \

# LOCAL_CXX_STL := libstdc++

# LOCAL_LDLIBS := -llog

# LOCAL_SRC_FILES := \
# 	jni/android_net_pppoe_natvie.cpp \

# LOCAL_MODULE := libpppoe-native

# include $(BUILD_SHARED_LIBRARY)

# This will install the file in /system/etc/permissions
#
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := com.android.server.pppoe.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/permissions
LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)

#=== build java jar =====
include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under, java)
LOCAL_REQUIRED_MODULES := com.android.server.pppoe.xml
LOCAL_MODULE_TAGS := optional
# This is the target being built.
LOCAL_MODULE:= com.android.server.pppoe
include $(BUILD_JAVA_LIBRARY)