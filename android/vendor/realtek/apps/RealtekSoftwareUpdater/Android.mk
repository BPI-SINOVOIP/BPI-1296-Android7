LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
$(info $(shell ($(LOCAL_PATH)/multilan.sh $(LOCAL_PATH) )))

LOCAL_JAVA_LIBRARIES := bouncycastle telephony-common
LOCAL_STATIC_JAVA_LIBRARIES := guava android-support-v4 jsr305

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_PACKAGE_NAME := RealtekSoftwareUpdater
#LOCAL_CERTIFICATE := shared
LOCAL_CERTIFICATE := platform

LOCAL_PROGUARD_FLAG_FILES := proguard.flags

include $(BUILD_PACKAGE)

# Use the folloing include to make our test apk.
include $(call all-makefiles-under,$(LOCAL_PATH))
