LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../../chip.mk

LOCAL_SRC_FILES:=                 \
	ISocket.cpp CInputSocket.cpp TS.cpp rtp.cpp tsDebug.cpp

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/../../../ \
	$(LOCAL_PATH)/../../../Include \
	$(LOCAL_PATH)/../../../Include/IPC \
	$(LOCAL_PATH)/../../../OSAL/include \
	$(LOCAL_PATH)/../../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../../common/IPC/include \
	$(LOCAL_PATH)/../../../common/IPC/generate/include/system

LOCAL_CFLAGS += -Wno-multichar
LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)

LOCAL_CFLAGS += -Wno-error=format-security
LOCAL_CFLAGS += -Wno-error=non-virtual-dtor
LOCAL_CFLAGS += -Wno-error=address
LOCAL_CFLAGS += -Wno-error=sequence-point
LOCAL_CFLAGS += -Wno-error=return-type

LOCAL_MODULE:= libRTKInputSocket

include $(BUILD_STATIC_LIBRARY)

