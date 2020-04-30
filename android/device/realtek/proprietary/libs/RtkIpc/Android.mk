LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := libRtkIpc.so
LOCAL_MODULE_TAGS:= optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libRtkIpc

include $(BUILD_PREBUILT)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES := libRtkTranscodePlayerService.so
LOCAL_MODULE_TAGS:= optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libRtkTranscodePlayerService

include $(BUILD_PREBUILT)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := rtktranscodeplayerserver
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS := debug
LOCAL_MODULE := rtktranscodeplayerserver

include $(BUILD_PREBUILT)

