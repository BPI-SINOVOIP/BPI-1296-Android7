LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../../chip.mk

LOCAL_SRC_FILES:= \
	AAC_Utility.c ID3.c SocketAVData_Utility.c TS_Utility.cpp
	

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../../../Include \
	$(LOCAL_PATH)/../../../Include/IPC \
	$(LOCAL_PATH)/../../../OSAL/include \
        $(TOP)/hardware/realtek/realtek_omx/rtk_libs/OSAL/include \
	$(TOP)/hardware/realtek/realtek_omx/rtk_libs/common/IPC/include \
	$(TOP)/hardware/realtek/realtek_omx/rtk_libs/common/IPC/generate/include/system \

LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type

LOCAL_MODULE:= libRTKCommonUtility

LOCAL_SHARED_LIBRARIES := liblog

#LOCAL_STATIC_LIBRARIES := libRTK_lib

include $(BUILD_STATIC_LIBRARY)
