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
$(info $(shell ($(LOCAL_PATH)/multilan.sh $(LOCAL_PATH) )))

LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_PACKAGE_NAME := MediaBrowser
LOCAL_REQUIRED_MODULES := libDLNADMPClass

# Only compile source java files in this apk.
LOCAL_SRC_FILES := $(call all-java-files-under, src/com)

LOCAL_STATIC_JAVA_LIBRARIES := \
	android-support-v4 librecommendation libglide libbcprov_mb libslf4j_mb libasn_mb libjcifs_mb

LOCAL_CERTIFICATE := platform
# Link against the current Android SDK.
# LOCAL_SDK_VERSION := current
LOCAL_PROGUARD_FLAG_FILES := proguard.flags

include $(BUILD_PACKAGE)

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := \
    librecommendation:libs/android-support-recommendation.jar \
	libglide:libs/glide-3.7.0.jar \
	libslf4j_mb:libs/slf4j-api-1.7.25.jar \
	libasn_mb:libs/asn-one-0.4.0.jar \
	libbcprov_mb:libs/bcprov-jdk15on-161.jar \
	libjcifs_mb:libs/jcifs-ng-2.1.3.jar

include $(BUILD_MULTI_PREBUILT)
include $(call all-makefiles-under,$(LOCAL_PATH))
