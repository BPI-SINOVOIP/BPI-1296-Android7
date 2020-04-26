LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../../chip.mk

LOCAL_SRC_FILES:=                 \
	InputMP4.c mp4atom.c mp4_bits.c mp4.c \
	mp4ff.c mp4_getinfo.c \
	mp4_parse.c mp4_progconfig.c mp4_stream.c \
	mp4util.c Security.c
#Ibcti.cpp

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/../../../ \
	$(LOCAL_PATH)/../../../Include \
	$(LOCAL_PATH)/../../../Include/IPC \
	$(LOCAL_PATH)/../../../OSAL/include \
	$(LOCAL_PATH)/../../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../../common/IPC/include \
	$(LOCAL_PATH)/../../../common/IPC/generate/include/system

#	$(TOP)/hardware/realtek/realtek_omx/rtk_libs
#        $(TOP)/frameworks/native/include/media/openmax \

LOCAL_CFLAGS += -Wno-multichar
LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)

LOCAL_CFLAGS += -Wno-error=format-security
LOCAL_CFLAGS += -Wno-error=non-virtual-dtor
LOCAL_CFLAGS += -Wno-error=address
LOCAL_CFLAGS += -Wno-error=sequence-point
LOCAL_CFLAGS += -Wno-error=return-type

LOCAL_MODULE:= libRTKInputMP4

include $(BUILD_STATIC_LIBRARY)

