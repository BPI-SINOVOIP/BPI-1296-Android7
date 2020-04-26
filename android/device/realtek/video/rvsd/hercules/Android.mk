LOCAL_PATH:= $(call my-dir)

################################################################################
## To pre-build static-library, then build rvsd and link the static-library

RVSD_RTK_PRIVATE = false
ifeq ($(RVSD_RTK_PRIVATE),true)
LOCAL_SRC_FILES := rvsd_RtkPrivate.cpp
LOCAL_C_INCLUDES := \
    $(TOP)/hardware/realtek/realtek_omx/component/video/cnm_video_api/include \
    $(TOP)/hardware/realtek/realtek_omx/component/video/cnm_video_api/vpuapi
LOCAL_MODULE := librvsd_RtkPrivate
include $(BUILD_STATIC_LIBRARY)
endif

################################################################################

RVSD_VORPC = true
RVSD_ENABLE_ENCODE = true
RVSD_ENABLE_SHARE_ICQ = true
RVSD_VO_WRITEBACK = true
#RVSD_USE_VOUTIL = true
RVSD_USE_1295_VE = true
RVSD_USE_1295_VE3 = true
RVSD_USE_JPU_API = true
ENABLE_DOLBY_METADATA_PARSER = false
RVSD_USE_VO_SCALE_CMD = false
RVSD_USE_VO_SCALE_AUTO = false
RVSD_USE_FW_BIT_CODE_TABLE = true
RVSD_DBG_DUMP_BS = false

COMMON_SRC_FILES := \
        rvsd_maincore.cpp rvsd_ringbuffer.cpp rvsd_OSAL.cpp rvsd_InBandCMD.cpp \
        rvsd_VDecACodec.cpp \
        rvsd_ptsMsgHandle.cpp \
        rvsd_entry.cpp \
        socket_client.cpp\
        dvbsp_decoder/DVBSP_Decoder_HL.cpp dvbsp_decoder/DVBSP_Decoder.cpp \
        sp_common/sp_common.cpp sp_common/sp_rpc.cpp dvdsp_decoder/SP.cpp \
        bdsp_decoder/BDSP_Decoder.cpp bdsp_decoder/BDSP_Decoder_HL.cpp rvsd_dbg.cpp \
        rvsd_VDecRM.cpp \
        rvsd_BsValidate.cpp \
        rvsd_CommonFunc.cpp \
        rvsd_VDecMpeg2.cpp \
        rvsd_VDecHdrParameterParser.cpp \
        rvsd_VdecBitstreamFeed.cpp \
        rvsd_dbg_md5.c \
        rvsd_BitReader.cpp

ifeq ($(ENABLE_DOLBY_METADATA_PARSER), true)
COMMON_SRC_FILES += rvsd_DolbyMetadataParser.cpp
endif

include $(CLEAR_VARS)

ifeq ($(PLATFORM_VERSION), 7.1.1)
LOCAL_CFLAGS += -DANDROID_6
#LOCAL_CFLAGS += -DANDROID7_HACK_AVOID_SPU    ## remove me later
endif
ifeq ($(PLATFORM_VERSION), 7.0)
LOCAL_CFLAGS += -DANDROID_6
#LOCAL_CFLAGS += -DANDROID7_HACK_AVOID_SPU    ## remove me later
endif
ifeq ($(PLATFORM_VERSION), 6.0.1)
LOCAL_CFLAGS += -DANDROID_6
endif
ifeq ($(PLATFORM_VERSION), 6.0)
LOCAL_CFLAGS += -DANDROID_6
endif
#else ifeq ($(PLATFORM_VERSION), 5.1.1)
#LOCAL_CFLAGS += -DANDROID_5.1.1
#else ifeq ($(PLATFORM_VERSION), 4.4)
#LOCAL_CFLAGS += -DANDROID_4.4
#else
#LOCAL_CFLAGS += -DANDROID_6

LOCAL_CFLAGS += -DRVSD_ENABLE_SET_CRA_AS_BLA

LOCAL_CFLAGS += -DRVSD_MPEG2_CC
LOCAL_CFLAGS += -DVPU_GET_CC
LOCAL_CFLAGS += -DRVSD_SUPPORT_TECHNICOLOR
LOCAL_CFLAGS += -DRVSD_H264_CC
#LOCAL_CFLAGS += -DRVSD_H265_CC

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_CFLAGS += -DAndroid_JB
endif
#ifeq ($(TARGET_BOARD_PLATFORM), phoenix)
LOCAL_CFLAGS += -DPLATFORM_RTK_PHOENIX
#LOCAL_CFLAGS += -DUSE_AUDIO_OMX
#LOCAL_CFLAGS += -DUSE_FAKE_CLK
LOCAL_CFLAGS += -DSPU_DECODE_ENABLE
LOCAL_CFLAGS += -DDVBSPDEC_ENABLED
LOCAL_CFLAGS += -DBDSPDEC_ENABLED
#endif

ifeq ($(RVSD_RTK_PRIVATE),true)
LOCAL_CFLAGS += -DRVSD_RTK_PRIVATE_EN
endif

ifeq ($(USE_RT_ION),true)
LOCAL_CFLAGS += -DUSE_RT_ION
endif
ifeq ($(USE_RT_EXTRACTOR),true)
LOCAL_CFLAGS += -DUSE_RTK_EXTRACTOR
endif


ifeq ($(RVSD_USE_VOUTIL),true)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_CFLAGS += $(DEFINES)
LOCAL_CFLAGS += -DRVSD_USE_VOUTIL
endif

LOCAL_SRC_FILES := $(COMMON_SRC_FILES)

LOCAL_SRC_FILES += rvsd_VDecRtk.cpp

LOCAL_SRC_FILES += rvsd_VDecRtk_ProcICQ.cpp

ifeq ($(RVSD_USE_JPU_API), true)
LOCAL_SRC_FILES += rvsd_JDecRtk.cpp
LOCAL_SRC_FILES += jpu/rvsd_CodecJdec.cpp
endif

ifeq ($(RVSD_VORPC),true)
#LOCAL_CFLAGS += -DRVSD_RPC_RVSD_HANDLE
#LOCAL_CFLAGS += -DRVSD_RPC_VO_ACODEC
LOCAL_SRC_FILES +=  rvsd_VORPC.cpp
endif
ifeq ($(RVSD_ENABLE_ENCODE),true)
LOCAL_CFLAGS += -DENABLE_ENCODE
ifeq ($(RVSD_VO_WRITEBACK),true)
LOCAL_CFLAGS += -DRVSD_VO_WRITEBACK
#LOCAL_CFLAGS += -DSET_TV_SYSTEM_720P30
endif
endif

ifeq ($(RVSD_ENABLE_SHARE_ICQ),true)
LOCAL_CFLAGS += -DRVSD_NEW_VIDEO_FLOW_EN
LOCAL_SRC_FILES +=  rvsd_VDecRtk_NewFlow.cpp
endif

ifeq ($(RVSD_USE_1295_VE), true)
LOCAL_CFLAGS += -DRVSD_USE_1295_VE
endif

ifeq ($(RVSD_USE_1295_VE3), true)
LOCAL_CFLAGS += -DRVSD_USE_1295_VE3
LOCAL_CFLAGS += -DRVSD_ACODEC_FLOW_SIDEBAND_ENABLED
endif

ifeq ($(RVSD_USE_JPU_API), true)
LOCAL_CFLAGS += -DRVSD_USE_JPU_API
endif

ifeq ($(RVSD_USE_VO_SCALE_CMD), true)
LOCAL_CFLAGS += -DRVSD_USE_VO_SCALE_CMD
endif

ifeq ($(RVSD_USE_VO_SCALE_AUTO), true)
LOCAL_CFLAGS += -DRVSD_USE_VO_SCALE_AUTO
endif

ifeq ($(RVSD_USE_FW_BIT_CODE_TABLE), true)
LOCAL_CFLAGS += -DRVSD_USE_FW_BIT_CODE_TABLE
endif

LIBVERSION:=\"librvsd_version_$(shell cd $(LOCAL_PATH);git rev-parse HEAD )_$(shell date +\"%Y%m%d%H%M%S\")\\n\"
$(warning "the librvsd_version_ is$(LIBVERSION)")
LOCAL_CFLAGS += -DLIBRVSDVERSION=$(LIBVERSION)

#LOCAL_SRC_FILES += eventListener.cpp

LOCAL_SHARED_LIBRARIES := \
    libstagefright libmedia libutils libbinder libstagefright_foundation \
        libgui libion liblog libcutils
LOCAL_SHARED_LIBRARIES += libRtkIpc
LOCAL_SHARED_LIBRARIES += libvpu
LOCAL_SHARED_LIBRARIES += libjpu
LOCAL_SHARED_LIBRARIES += librtk_ion

ifeq ($(ENABLE_DOLBY_METADATA_PARSER), true)
LOCAL_SHARED_LIBRARIES += librpu
endif

ifeq ($(RVSD_VORPC),true)
LOCAL_SHARED_LIBRARIES += libRTK_lib libRTKOMX_OSAL_RTK libion
endif
ifeq ($(RVSD_USE_VOUTIL),true)
LOCAL_SHARED_LIBRARIES += libRTKVoutUtil
endif
ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_SHARED_LIBRARIES += libcutils
endif

#LOCAL_SHARED_LIBRARIES += libRTMediaPlayer

LOCAL_C_INCLUDES:= \
    frameworks/av/media/libstagefright \
    frameworks/av/media/libstagefright/include \
    $(TOP)/frameworks/native/include/media/openmax

LOCAL_C_INCLUDES += \
    $(TOP)/hardware/realtek/VideoEngine/VE1 \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vdi \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vpuapi

ifeq ($(RVSD_USE_JPU_API), true)
LOCAL_C_INCLUDES += \
    $(TOP)/hardware/realtek/VideoEngine/JPEG	\
	$(TOP)/hardware/realtek/VideoEngine/JPEG/include \
	$(TOP)/hardware/realtek/VideoEngine/JPEG/jpuapi
endif

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/bdsp_decoder \
    $(LOCAL_PATH)/dvbsp_decoder \
    $(LOCAL_PATH)/dvdsp_decoder \
    $(LOCAL_PATH)/sp_common

ifeq ($(RVSD_VORPC),true)
LOCAL_C_INCLUDES += .
LOCAL_C_INCLUDES += system/core
LOCAL_C_INCLUDES += hardware/realtek/realtek_omx/osal_rtk
LOCAL_C_INCLUDES += hardware/realtek/realtek_omx/core
LOCAL_C_INCLUDES += hardware/realtek/realtek_omx/component
LOCAL_C_INCLUDES += hardware/realtek/realtek_omx/include/khronos
LOCAL_C_INCLUDES += hardware/realtek/realtek_omx/include/imagination
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_ion/

ifeq ($(ENABLE_DOLBY_METADATA_PARSER), true)
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/dolby_vision/dolby/libs/vdr_rpu/include
endif

#$(RTK_PLI)/include
#$(RTK_PLI)/include/OSAL
endif

LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs/libRTMediaPlayer
LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs/Include
LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs/transcode
LOCAL_C_INCLUDES += external/stlport/stlport
LOCAL_C_INCLUDES += bionic

LOCAL_CFLAGS += -Wno-multichar

ifeq ($(RVSD_DBG_DUMP_BS), true)
LOCAL_CFLAGS += -D_DBG_RVSD_DUMP_DATA
LOCAL_C_INCLUDES += \
    device/realtek/firmware/include/main
LOCAL_SHARED_LIBRARIES += libfw_socket_client
endif

LOCAL_MODULE_TAGS := optional

ifeq ($(RVSD_RTK_PRIVATE),true)
LOCAL_STATIC_LIBRARIES := librvsd_RtkPrivate
endif

LOCAL_MODULE:= librvsd

include $(BUILD_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=       \
    client_main.cpp

LOCAL_SRC_FILES += rvsd_surface.cpp

LOCAL_SHARED_LIBRARIES := \
    libstagefright libmedia libutils libbinder libstagefright_foundation \
        libgui libion liblog

LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER


LOCAL_SHARED_LIBRARIES += libRT_IPC libmediaplayerservice

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_CFLAGS += -DAndroid_JB
endif

ifeq ($(RVSD_VO_WRITEBACK),true)
LOCAL_CFLAGS += -DRVSD_VO_WRITEBACK
endif

LOCAL_C_INCLUDES:= \
    frameworks/av/media/libstagefright \
    frameworks/av/media/libstagefright/include \
    $(TOP)/frameworks/native/include/media/openmax \
    bionic  \
    external/stlport/stlport    \
    frameworks/native/include/gui \
    frameworks/av/media/libmediaplayerservice \
    $(TOP)/device/realtek/proprietary/libs/libRTMediaPlayer/ \
    $(TOP)/device/realtek/proprietary/libs/Include \
    $(TOP)/device/realtek/proprietary/libs/RT_IPC \
    $(TOP)/device/realtek/proprietary/libs

LOCAL_C_INCLUDES += \
    $(TOP)/hardware/realtek/VideoEngine/VE1 \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vdi \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vpuapi

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/transcode
LOCAL_SHARED_LIBRARIES += libRTMediaPlayer

LOCAL_SHARED_LIBRARIES += librvsd
LOCAL_SHARED_LIBRARIES += libRtkIpc

ifeq ($(USE_RTK_VOUT_UTIL),YES)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1 -DUSE_RTK_VOUT_UTIL
endif

LOCAL_CFLAGS += -Wno-multichar

LOCAL_MODULE_TAGS := optional eng

LOCAL_32_BIT_ONLY := true

LOCAL_MODULE:= mpclient

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=       \
    client_main_2.cpp

LOCAL_SRC_FILES += rvsd_surface.cpp

LOCAL_SHARED_LIBRARIES := \
    libstagefright libmedia libutils libbinder libstagefright_foundation \
        libgui libion liblog

LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER


LOCAL_SHARED_LIBRARIES += libRT_IPC
#LOCAL_SHARED_LIBRARIES += libmediaplayerservice

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_CFLAGS += -DAndroid_JB
endif

LOCAL_C_INCLUDES:= \
    frameworks/av/media/libstagefright \
    frameworks/av/media/libstagefright/include \
    $(TOP)/frameworks/native/include/media/openmax \
    bionic  \
    external/stlport/stlport    \
    frameworks/native/include/gui \
    frameworks/av/media/libmediaplayerservice \
    $(TOP)/device/realtek/proprietary/libs/libRTMediaPlayer/ \
    $(TOP)/device/realtek/proprietary/libs/Include \
    $(TOP)/device/realtek/proprietary/libs/RT_IPC \
    $(TOP)/device/realtek/proprietary/libs

LOCAL_C_INCLUDES += \
    $(TOP)/hardware/realtek/VideoEngine/VE1 \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vdi \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vpuapi

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/transcode
LOCAL_C_INCLUDES += $(TOP)/frameworks/av/include/media

LOCAL_SHARED_LIBRARIES += librvsd
LOCAL_SHARED_LIBRARIES += libRtkIpc

ifeq ($(USE_RTK_VOUT_UTIL),YES)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1 -DUSE_RTK_VOUT_UTIL
endif

LOCAL_CFLAGS += -Wno-multichar

LOCAL_MODULE_TAGS := optional eng

LOCAL_MODULE:= mpclient2

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=       \
    client_main_dtv.cpp

LOCAL_SRC_FILES += rvsd_surface.cpp
LOCAL_SRC_FILES += TvManagerService.cpp

LOCAL_SHARED_LIBRARIES := \
    libstagefright libmedia libutils libbinder libstagefright_foundation \
        libgui libion liblog

LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER

LOCAL_SHARED_LIBRARIES += libRT_IPC libmediaplayerservice

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_CFLAGS += -DAndroid_JB
endif

LOCAL_C_INCLUDES:= \
    frameworks/av/media/libstagefright \
    frameworks/av/media/libstagefright/include \
    $(TOP)/frameworks/native/include/media/openmax \
    bionic  \
    external/stlport/stlport    \
    frameworks/native/include/gui \
    frameworks/av/media/libmediaplayerservice \
    $(TOP)/device/realtek/proprietary/libs/libRTMediaPlayer/ \
    $(TOP)/device/realtek/proprietary/libs/Include \
    $(TOP)/device/realtek/proprietary/libs/RT_IPC \
    $(TOP)/device/realtek/proprietary/libs

LOCAL_C_INCLUDES += \
    $(TOP)/hardware/realtek/VideoEngine/VE1 \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vdi \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vpuapi

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/transcode
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTDtvMediaPlayer
LOCAL_C_INCLUDES += device/realtek/frameworks/services/jni/tv
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/RtkRpcClientServer
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/RtkRpcClientServer/rpc
#LOCAL_SHARED_LIBRARIES += libRTMediaPlayer
##LOCAL_SHARED_LIBRARIES += libTvProxy
LOCAL_SHARED_LIBRARIES += libRTDtvMediaPlayer
LOCAL_CFLAGS += -DAndroid_KitKate

LOCAL_SHARED_LIBRARIES += librvsd
LOCAL_SHARED_LIBRARIES += libRtkIpc
LOCAL_SHARED_LIBRARIES += libRtkRpcClientServer

ifeq ($(USE_RTK_VOUT_UTIL),YES)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1 -DUSE_RTK_VOUT_UTIL
endif

LOCAL_CFLAGS += -Wno-multichar

LOCAL_MODULE_TAGS := optional eng

LOCAL_32_BIT_ONLY := true

LOCAL_MODULE:= mpDTVclient

include $(BUILD_EXECUTABLE)


###############################################
###############################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=       \
    DumpFWProxy.cpp

LOCAL_SHARED_LIBRARIES := \
    libion liblog
LOCAL_SHARED_LIBRARIES += libRTK_lib	## RPC
LOCAL_SHARED_LIBRARIES += librvsd

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system

LOCAL_C_INCLUDES += \
    $(TOP)/hardware/realtek/VideoEngine/VE1 \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vdi \
    $(TOP)/hardware/realtek/VideoEngine/VE1/vpuapi

LOCAL_C_INCLUDES += system/core/include


LOCAL_MODULE:= DumpFWProxy

include $(BUILD_EXECUTABLE)


