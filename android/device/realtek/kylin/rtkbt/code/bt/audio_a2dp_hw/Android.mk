LOCAL_PATH := $(call my-dir)

# Audio A2DP shared library for target
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	audio_a2dp_hw.c

LOCAL_C_INCLUDES += \
	. \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../utils/include

LOCAL_MODULE := audio.a2dp.default
LOCAL_MODULE_RELATIVE_PATH := hw

LOCAL_SHARED_LIBRARIES := liblog
ifeq ($(BOARD_HAVE_BLUETOOTH_RTK), true)
LOCAL_STATIC_LIBRARIES := libosi_rtk
else
LOCAL_STATIC_LIBRARIES := libosi
endif

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += $(bluetooth_CFLAGS)
LOCAL_CONLYFLAGS += $(bluetooth_CONLYFLAGS)
LOCAL_CPPFLAGS += $(bluetooth_CPPFLAGS)

include $(BUILD_SHARED_LIBRARY)
