# Copyright 2006 The Android Open Source Project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= main.c xml_util.c

LOCAL_MODULE:= multilan

include $(BUILD_HOST_EXECUTABLE)
