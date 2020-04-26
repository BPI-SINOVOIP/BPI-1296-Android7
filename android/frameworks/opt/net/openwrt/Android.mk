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

# This will install the file in /system/etc/permissions
#
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := com.rtk.net.ubusrpc.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/permissions
LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := ubus
LOCAL_SRC_FILES := bin/ubus
LOCAL_MODULE_CLASS := EXECUTABLES
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := ubus.sh
LOCAL_INIT_RC := openwrt.rc
LOCAL_SRC_FILES := bin/ubus.sh
LOCAL_MODULE_CLASS := EXECUTABLES
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := update_hosts.sh
LOCAL_INIT_RC := openwrt.rc
LOCAL_SRC_FILES := bin/update_hosts.sh
LOCAL_MODULE_CLASS := EXECUTABLES
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := init_xen_data_channel.sh
LOCAL_INIT_RC := openwrt.rc
LOCAL_SRC_FILES := bin/init_xen_data_channel.sh
LOCAL_MODULE_CLASS := EXECUTABLES
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := iface_du.sh
LOCAL_INIT_RC := openwrt.rc
LOCAL_SRC_FILES := bin/iface_du.sh
LOCAL_MODULE_CLASS := EXECUTABLES
include $(BUILD_PREBUILT)

#=== build java jar =====
include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under, java)
LOCAL_STATIC_JAVA_LIBRARIES := \
	jackson-core jackson-annotations jackson-databind \
	slf4j json-smart json-path
LOCAL_MODULE_TAGS := optional
# This is the target being built.
LOCAL_MODULE:= com.rtk.net.ubusrpc
LOCAL_REQUIRED_MODULES := \
	curl \
	ubus \
	ubus.sh \
	update_hosts.sh \
	init_xen_data_channel.sh \
	Dom0tcpService \
	iface_du.sh

include $(BUILD_STATIC_JAVA_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := \
	jackson-core:libs/jackson-core-2.3.3.jar \
	jackson-annotations:libs/jackson-annotations-2.3.3.jar \
	jackson-databind:libs/jackson-databind-2.3.3.jar \
	slf4j:libs/slf4j-api-1.7.16.jar \
	json-smart:libs/json-smart-2.2.1.jar \
	json-path:libs/json-path-2.2.0.jar
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := Dom0tcpService.c
LOCAL_LDLIBS := -llog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= Dom0tcpService
include $(BUILD_EXECUTABLE)