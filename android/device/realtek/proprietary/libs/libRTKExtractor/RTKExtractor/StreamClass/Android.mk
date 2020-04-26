LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../chip.mk

LOCAL_SRC_FILES:= \
	CEnumFilters.cpp CEnumPins.cpp CFilter.cpp CFlowManager.cpp CMediaSample.cpp \
	CMediaType.cpp CMemListAllocator.cpp CMemRingAllocator.cpp CPin.cpp \
	CReadPointerHandle.cpp CReferenceClock.cpp 
#IOCache_MS.cpp
	

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../Include \
	$(LOCAL_PATH)/../Include/IPC \
	$(LOCAL_PATH)/../OSAL/include \
	$(LOCAL_PATH)/../OSAL/include/OSAL \
	$(LOCAL_PATH)/../Include/IPC/include/ \
	$(LOCAL_PATH)/../common/IPC/include \

LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type
#LOCAL_CFLAGS += -frtti

LOCAL_MODULE:= libRTKStreamClass

LOCAL_SHARED_LIBRARIES := liblog

#LOCAL_STATIC_LIBRARIES := libRTK_lib

include $(BUILD_STATIC_LIBRARY)

