LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_PACKAGE_NAME := DMR

LOCAL_STATIC_JAVA_LIBRARIES := android-support-v4
LOCAL_REQUIRED_MODULES := libDLNADMRClass

LOCAL_SRC_FILES := $(call all-java-files-under, src)

#LOCAL_CERTIFICATE := shared
LOCAL_CERTIFICATE := platform
# Link against the current Android SDK.
LOCAL_SDK_VERSION := current
LOCAL_PROGUARD_FLAG_FILES := proguard.flags

include $(BUILD_PACKAGE)
