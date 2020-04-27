#
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
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_PACKAGE_NAME := RealtekEncoder

# Only compile source java files in this apk.
LOCAL_SRC_FILES := $(call all-java-files-under, src)
# AIDL
LOCAL_SRC_FILES += src/com/realtek/transcode/IRealtekTranscodeService.aidl
LOCAL_STATIC_JAVA_LIBRARIES := libencodersupportv4_tsb
LOCAL_CERTIFICATE := platform
#LOCAL_CERTIFICATE := shared
# Link against the current Android SDK.
# LOCAL_SDK_VERSION := current
LOCAL_PROGUARD_FLAG_FILES := proguard.flags

include $(BUILD_PACKAGE)

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := libencodersupportv4_tsb:libs/android-support-v4.jar
include $(BUILD_MULTI_PREBUILT)
include $(call all-makefiles-under,$(LOCAL_PATH))

include $(CLEAR_VARS)
#LOCAL_SRC_FILES := $(call all-java-files-under, cmd/src))
LOCAL_SRC_FILES += cmd/src/com/realtek/commands/transcode/Transcode.java
LOCAL_SRC_FILES += cmd/src/com/realtek/transcode/TranscodeManager.java
LOCAL_SRC_FILES += src/com/realtek/transcode/IRealtekTranscodeService.aidl
LOCAL_MODULE := transcode
include $(BUILD_JAVA_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := transcmd
LOCAL_SRC_FILES := cmd/transcmd
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS := optional
include $(BUILD_PREBUILT)
