LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libGLES_mali
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_TAGS := optional

ifdef TARGET_2ND_ARCH
LOCAL_MULTILIB := both
LOCAL_MODULE_PATH_64 := $(TARGET_OUT)/lib64/egl
LOCAL_MODULE_PATH_32 := $(TARGET_OUT)/lib/egl
LOCAL_SRC_FILES_64 := lib64/egl/libGLES_mali.so
LOCAL_SRC_FILES_32 := lib/egl/libGLES_mali.so

$(shell mkdir -p $(TARGET_OUT)/lib64/)
$(shell mkdir -p $(TARGET_OUT)/lib/)
$(shell mkdir -p $(TARGET_OUT_VENDOR)/lib64/)
$(shell mkdir -p $(TARGET_OUT_VENDOR)/lib/)
$(shell mkdir -p $(TARGET_OUT)/bin/)

$(shell cp -rf $(LOCAL_PATH)/lib64/* $(TARGET_OUT)/lib64/)
$(shell cp -rf $(LOCAL_PATH)/lib/* $(TARGET_OUT)/lib/)
$(shell cp -rf $(LOCAL_PATH)/vendor/lib64/* $(TARGET_OUT_VENDOR)/lib64/)
$(shell cp -rf $(LOCAL_PATH)/vendor/lib/* $(TARGET_OUT_VENDOR)/lib/)
$(shell cp -rf $(LOCAL_PATH)/bin64/* $(TARGET_OUT)/bin/)
else
LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/egl
LOCAL_SRC_FILES := lib/egl/libGLES_mali.so

$(shell mkdir -p $(TARGET_OUT)/lib/)
$(shell mkdir -p $(TARGET_OUT_VENDOR)/lib/)
$(shell cp -rf $(LOCAL_PATH)/lib/* $(TARGET_OUT)/lib/)
$(shell cp -rf $(LOCAL_PATH)/vendor/lib/* $(TARGET_OUT_VENDOR)/lib/)
endif
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := gralloc.kylin
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_TAGS := optional

ifdef TARGET_2ND_ARCH
LOCAL_MULTILIB := both
LOCAL_MODULE_PATH_64 := $(TARGET_OUT)/lib64/hw
LOCAL_MODULE_PATH_32 := $(TARGET_OUT)/lib/hw
LOCAL_SRC_FILES_64 := lib64/hw/gralloc.kylin.so
LOCAL_SRC_FILES_32 := lib/hw/gralloc.kylin.so
else
LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/hw
LOCAL_SRC_FILES := lib/hw/gralloc.kylin.so
endif

include $(BUILD_PREBUILT)
