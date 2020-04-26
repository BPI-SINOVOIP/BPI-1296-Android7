LOCAL_PATH:= $(call my-dir)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX_Core.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX_Core

include $(BUILD_PREBUILT)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libRTKOMX_OSAL_RTK.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libRTKOMX_OSAL_RTK

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.audio.dec.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.audio.dec

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.video.dec.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.video.dec

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.image.dec.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.image.dec

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.video.dec.avc.secure.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.video.dec.avc.secure

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.video.dec.vp9.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.video.dec.vp9

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.video.enc.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.video.enc

include $(BUILD_PREBUILT)
################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := libOMX.realtek.image.enc.so
LOCAL_MODULE_TAGS:= optional eng
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := libOMX.realtek.image.enc

include $(BUILD_PREBUILT)
