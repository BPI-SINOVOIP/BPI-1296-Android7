LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../../chip.mk

LOCAL_SRC_FILES:=                 \
	CDtvBitRateEst.cpp CDtvContextInfo.cpp CDtvFileEditor.cpp \
	CDtvFileReader.cpp CDtvFileWriter.cpp CDtvSIInterface.cpp \
	CDtvTunerMode.cpp CInputDTV.cpp
#CDtvSubPath.cpp ==> for 3D

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/../../../ \
	$(LOCAL_PATH)/../../../Include \
	$(LOCAL_PATH)/../../../Include/IPC \
	$(LOCAL_PATH)/../../../OSAL/include \
	$(LOCAL_PATH)/../../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../../common/IPC/include \
	$(LOCAL_PATH)/../../../common/IPC/generate/include/system \
	$(LOCAL_PATH)/../../../Include/Filters/NavigationFilter/SiDvb/include \
	$(LOCAL_PATH)/../../../Include/Filters/NavigationFilter \
	$(LOCAL_PATH)/../../../Filters/NavigationFilter/SiDvb/librtd/si4 \
	$(LOCAL_PATH)/../../../Filters/NavigationFilter

#	$(TOP)/hardware/realtek/realtek_omx/rtk_libs
#        $(TOP)/frameworks/native/include/media/openmax \

LOCAL_CFLAGS += -Wno-multichar
LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)

LOCAL_CFLAGS += -Wno-error=format-security
LOCAL_CFLAGS += -Wno-error=non-virtual-dtor
LOCAL_CFLAGS += -Wno-error=address
LOCAL_CFLAGS += -Wno-error=sequence-point
LOCAL_CFLAGS += -Wno-error=return-type

LOCAL_CFLAGS += -DTEST_TS -DDTV_FILE_PLAYBACK_RESYNC_THRESHOLD=962560

LOCAL_MODULE:= libRTKInputDTV

include $(BUILD_STATIC_LIBRARY)

