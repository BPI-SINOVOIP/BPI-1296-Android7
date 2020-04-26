ifeq ($(USE_RTK_SETUP_CLASS), YES)

LOCAL_PATH := $(call my-dir)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libRTKSetupClass.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libRTKSetupClass

include $(BUILD_PREBUILT)

endif # USE_RTK_SETUP_CLASS
