LOCAL_PATH := $(call my-dir)

# Building the vp9api
include $(CLEAR_VARS)


LOCAL_SRC_FILES := \
	common/decapi.c \
	common/bqueue.c \
	common/commonconfig.c \
	common/fifo.c \
	common/raster_buffer_mgr.c \
	common/regdrv.c \
	common/sw_stream.c \
	common/sw_util.c \
	vp9/vp9_entropymode.c \
	vp9/vp9_entropymv.c \
	vp9/vp9_modecont.c \
	vp9/vp9_modecontext.c \
	vp9/vp9_treecoder.c \
	vp9/vp9decapi.c \
	vp9/vp9hwd_asic.c \
	vp9/vp9hwd_bool.c \
	vp9/vp9hwd_decoder.c \
	vp9/vp9hwd_buffer_queue.c \
	vp9/vp9hwd_headers.c \
	vp9/vp9hwd_output.c \
	vp9/vp9hwd_probs.c

LOCAL_SRC_FILES += \
	../linux/dwl/dwl_activity_trace.c \
	../linux/dwl/dwl_linux.c \
	../linux/dwl/dwl_buf_protect.c \
	../linux/dwl/dwl_linux_hw.c

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_VE3_PATH) \
					$(LOCAL_VE3_PATH)/software/linux/memalloc \
					$(LOCAL_VE3_PATH)/software/linux/ldriver \
					$(LOCAL_VE3_PATH)/software/source/inc \
					$(LOCAL_VE3_PATH)/software/source/config \
					$(LOCAL_VE3_PATH)/software/source/common \
					$(LOCAL_VE3_PATH)/software/source/vp9

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvp9

LOCAL_CFLAGS := -DCONFIG_DEBUG_LEVEL=255
LOCAL_CFLAGS += -DFIFO_DATATYPE=void*
LOCAL_CFLAGS += -D_ASSERT_USED
LOCAL_CFLAGS += -DDOWN_SCALER
#LOCAL_CFLAGS += -DCLEAR_OUT_BUFFER
LOCAL_CFLAGS += -DDEC_X170_OUTPUT_FORMAT=0
LOCAL_CFLAGS += -DDEC_X170_TIMEOUT_LENGTH=-1
LOCAL_CFLAGS += -DDWL_DISABLE_REG_PRINTS
LOCAL_CFLAGS += -DENABLE_VP9_SUPPORT
LOCAL_CFLAGS += -DDWL_USE_DEC_IRQ
LOCAL_CFLAGS += -DUSE_64BIT_ENV
LOCAL_CFLAGS += -DUSE_EXTERNAL_BUFFER


LOCAL_STATIC_LIBRARIES :=

LOCAL_SHARED_LIBRARIES := \
		libutils \
		libdl \
		liblog


GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include

include $(BUILD_SHARED_LIBRARY)

