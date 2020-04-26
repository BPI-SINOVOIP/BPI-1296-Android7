LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../chip.mk

LOCAL_SRC_FILES:= \
	EditingInfo.cpp GeneralIFO.cpp GeneralIFO_Persist.cpp

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/DemuxMPEGProgram \
	$(LOCAL_PATH)/DemuxMPEGTransport \
	$(LOCAL_PATH)/DemuxPassthrough \
	$(LOCAL_PATH)/IOPlugins/FilePlugin \
	$(LOCAL_PATH)/../../Include \
	$(LOCAL_PATH)/../../Include/IPC \
	$(LOCAL_PATH)/../../OSAL/include\
	$(LOCAL_PATH)/../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../common/IPC/include \
	$(LOCAL_PATH)/../../common/IPC/generate/include/system

LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type

LOCAL_MODULE:= libRTKGeneralIFO


include $(BUILD_STATIC_LIBRARY)


include $(call all-makefiles-under,$(LOCAL_PATH))
