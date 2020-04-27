LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SDK_VERSION := 19

LOCAL_SRC_FILES := \
	$(call all-java-files-under, src) \
	$(call all-renderscript-files-under, src)

#LOCAL_JAVA_LIBRARIES := realtek
LOCAL_JAVA_LIBRARIES := framework services.realtek

#LOCAL_CERTIFICATE := platform

LOCAL_PACKAGE_NAME := ToGo

include $(BUILD_PACKAGE)
