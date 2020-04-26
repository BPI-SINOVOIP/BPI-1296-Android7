LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/chip.mk

LOCAL_SRC_FILES:= \
	Wrapper.cpp \
	Utility/Util.cpp \
	Utility/md5.c \
	Utility/ResourceMgr/ioResource.cpp \
	Flows/PlayBack/CPlaybackFlowManager.cpp

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/Include \
	$(LOCAL_PATH)/Include/IPC \
	$(LOCAL_PATH)/OSAL/include \
	$(LOCAL_PATH)/OSAL/include/OSAL \
	$(LOCAL_PATH)/Include/IPC/include/ \
	$(LOCAL_PATH)/common/IPC/generate/include/system \
	$(LOCAL_PATH)/common/IPC/include \
	bionic \

#        $(TOP)/hardware/realtek/realtek_omx/rtk_libs/OSAL/include \
#	$(TOP)/hardware/realtek/realtek_omx/rtk_libs/common/IPC/include \
#	$(TOP)/hardware/realtek/realtek_omx/osal/ \
#	$(TOP)/hardware/realtek/realtek_omx/rtk_libs/common/IPC/generate/include/system \

LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type
#LOCAL_CFLAGS += -frtti

#LOCAL_MODULE:= libRTKExtractor
LOCAL_MODULE:= libRTKExtWrapper

LOCAL_SHARED_LIBRARIES := liblog libdl

#LOCAL_STATIC_LIBRARIES := libRTKStreamClass
#LOCAL_STATIC_LIBRARIES += libRTKOSAL
#LOCAL_WHOLE_STATIC_LIBRARIES := libRTKNavigationFilter libRTKGeneralFileAccess
#LOCAL_STATIC_LIBRARIES += libRTKInputMP4 libRTKMCP libRTKCommonUtility 

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))

