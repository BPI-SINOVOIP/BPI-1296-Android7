LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

USE_ION = yes

RTK_TOP := hardware/realtek/realtek_omx
RTK_INC := $(RTK_TOP)/include
RTK_RPC := $(TOP)/device/realtek/proprietary/libs/rtk_libs/common
RTK_PLI := $(TOP)/device/realtek/proprietary/libs/rtk_libs/OSAL

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES :=

# use system/core/libion/ion.c instead
#ifeq ($(USE_ION), yes)
#LOCAL_SRC_FILES += ion.c
#endif

LOCAL_SRC_FILES += \
	basecomp.c \
	msgque.c \
	OSAL_RTK.c \
	port.c \
	util.c \
	android_ext.cpp \
	system_mem.c

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libRTKOMX_OSAL_RTK

LOCAL_CFLAGS :=
LOCAL_CFLAGS += -DRTK_AUDIO_EXTRACTOR_EN

ifeq ($(USE_ION), yes)
LOCAL_CFLAGS += -DENABLE_ION
endif

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libRTK_lib
LOCAL_SHARED_LIBRARIES += libui
LOCAL_SHARED_LIBRARIES += libion
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_C_INCLUDES := \
	$(RTK_INC)/khronos \
	$(RTK_INC)/imagination \
	. \
	system/core/include/cutils \
	frameworks/native/include/media/hardware \
	frameworks/native/include/media/openmax \
	$(RTK_RPC)/include \
	$(RTK_RPC)/IPC/include \
	$(RTK_RPC)/IPC/generate/include/system \
	$(RTK_PLI)/include \
	$(RTK_PLI)/include/OSAL
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include

GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include
include $(BUILD_SHARED_LIBRARY)
