ifneq ($(TARGET_PRODUCT), rtk_darwin_tv010)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := \
        $(call all-java-files-under, src)
		
LOCAL_PACKAGE_NAME := DTV
#LOCAL_JNI_SHARED_LIBRARIES :=
LOCAL_CERTIFICATE := platform

LOCAL_PROGUARD_ENABLED := disabled

#LOCAL_JAVA_LIBRARIES := realtek

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))

endif
