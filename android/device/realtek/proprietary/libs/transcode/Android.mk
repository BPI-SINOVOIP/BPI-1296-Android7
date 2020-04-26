LOCAL_PATH:= $(call my-dir)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libRtkTranscodePlayer.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libRtkTranscodePlayer

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := RtkTranscodeDemo
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS := debug
LOCAL_MODULE := RtkTranscodeDemo

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := TranscodeDemo6
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS := debug eng
LOCAL_MODULE := TranscodeDemo6

include $(BUILD_PREBUILT)





