LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_PACKAGE_NAME := runrunscript
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_TAGS := optional
#LOCAL_STATIC_JAVA_LIBRARIES += com.rtk.net.ubusrpc.wifi
include $(BUILD_PACKAGE)
