LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../chip.mk

LOCAL_SRC_FILES:= \
	CNavigationFilter.cpp \
	NavPluginFactory.cpp \
	NavDebug.cpp \
	DemuxMPEGProgram/CDemuxMPEGProgram.cpp \
	DemuxMPEGTransport/CDemuxMPEGTransport.cpp DemuxMPEGTransport/TsDebug.cpp \
	DemuxPassthrough/CDemuxPassthrough.cpp \
	IOPlugins/FilePlugin/FileSetIOPlugin.c \
	IOPlugins/Custom1/Custom1.c IOPlugins/Custom2/Custom2.c IOPlugins/Custom3/Custom3.c \
	IOPlugins/DataSourcePlugin/DataSourcePlugin.cpp
# IOPlugins/FilePlugin/aio.c   #with aio, need also fix semop & semctl issue

LOCAL_C_INCLUDES:= \
	$(TOP)/frameworks/av/include \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/DemuxMPEGProgram \
	$(LOCAL_PATH)/DemuxMPEGTransport \
	$(LOCAL_PATH)/DemuxPassthrough \
	$(LOCAL_PATH)/IOPlugins/FilePlugin \
	$(LOCAL_PATH)/../../Include \
	$(LOCAL_PATH)/../../Include/IPC \
	$(LOCAL_PATH)/../../OSAL/include \
	$(LOCAL_PATH)/../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../common/IPC/include \
	$(LOCAL_PATH)/../../common/IPC/generate/include/system \
	$(LOCAL_PATH)/../../Include/Filters/NavigationFilter \
	$(LOCAL_PATH)/../../Include/Filters/NavigationFilter/SiDvb/include


LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type
LOCAL_CFLAGS += -DTEST_TS

LOCAL_MODULE:= libRTKNavigationFilter

LOCAL_SHARED_LIBRARIES := liblog

#LOCAL_STATIC_LIBRARIES := libRTK_lib libRTKStreamClass
##LOCAL_STATIC_LIBRARIES += libRTKInputMP4 libRTKMCP libRTKCommonUtility

include $(BUILD_STATIC_LIBRARY)


include $(call all-makefiles-under,$(LOCAL_PATH))
