LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

RVSD_TOP := $(LOCAL_PATH)

ifeq ($(TARGET_BOARD_PLATFORM), phoenix)
include $(RVSD_TOP)/phoenix/Android.mk
endif

ifeq ($(TARGET_BOARD_PLATFORM), kylin)
include $(RVSD_TOP)/kylin/Android.mk
endif
