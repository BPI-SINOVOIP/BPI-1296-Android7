LOCAL_PATH := $(call my-dir)

# Building the vp9api
include $(CLEAR_VARS)


LOCAL_SRC_FILES := \
	common/bytestream_parser.c \
	common/command_line_parser.c \
	common/error_simulator.c \
	common/file_sink.c \
	common/md5_sink.c \
	common/null_sink.c \
	common/null_trace.c \
	common/swhw/md5.c \
	common/swhw/tb_cfg.c \
	common/swhw/tb_md5.c \
	common/swhw/tb_params.c \
	common/swhw/tb_stream_corrupt.c \
	common/swhw/tb_tiled.c \
	common/vpxfilereader.c \
	common/yuvfilters.c \
	common/dectestbench.c


LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_VE3_PATH) \
					$(LOCAL_VE3_PATH)/software/linux/memalloc \
					$(LOCAL_VE3_PATH)/software/linux/ldriver \
					$(LOCAL_VE3_PATH)/software/source/inc \
					$(LOCAL_VE3_PATH)/software/source/config \
					$(LOCAL_VE3_PATH)/software/source/common \
					$(LOCAL_VE3_PATH)/software/source/vp9 \
					$(LOCAL_VE3_PATH)/software/test/common \
					$(LOCAL_VE3_PATH)/software/test/common/swhw \
					$(LOCAL_VE3_PATH)/software/test/vp9

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := vp9dec

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
		libvp9 \
		liblog



include $(BUILD_EXECUTABLE)

