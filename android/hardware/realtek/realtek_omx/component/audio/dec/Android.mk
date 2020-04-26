LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

RTK_TOP := hardware/realtek/realtek_omx
RTK_INC := $(RTK_TOP)/include

LOCAL_SRC_FILES := \
    decoder.c \
    codec_rtk.c \
    AAC_Utility.c \
    sideband_audio.cpp

LOCAL_CFLAGS += -DRTK_AUDIO_EXTRACTOR_EN

## OMX_AUDIO_CFG ###################################
# OMX_AUDIO_CFG_1  : without MS12                  #
# OMX_AUDIO_CFG_2  : with MS12 entirely            #
# OMX_AUDIO_CFG_3  : DDP self test with conveter   #
# OMX_AUDIO_CFG_4  : without MS12+Audio on SCPU    #
####################################################

OMX_AUDIO_CFG = OMX_AUDIO_CFG_2

LOCAL_PRELINK_MODULE := false
AUDIO_ON_SCPU_EN = true
AUDIO_DWNSTRM_INBAND_QUEUE_EN = false
AUDIO_RAWOUTINFO_INBAND_QUEUE_EN = false
SUPPORT_MS12_FLOW = false
AUDIO_ON_SCPU_MS12_EN = false
NEW_CHANNEL_INDEX_TYPE_EN = true
MS12_SWITCH_FILE_EN = false

ifeq ($(OMX_AUDIO_CFG), OMX_AUDIO_CFG_2)
AUDIO_DWNSTRM_INBAND_QUEUE_EN = true
SUPPORT_MS12_FLOW = true
AUDIO_ON_SCPU_MS12_EN = true
#LOCAL_CFLAGS += -DHACK_FOR_NON_MS12PLAYER
#LOCAL_CFLAGS += -DMS12OMX_RETURN_AO_PTS
endif

ifeq ($(OMX_AUDIO_CFG), OMX_AUDIO_CFG_3)
#AUDIO_ON_SCPU_EN = true
AUDIO_DWNSTRM_INBAND_QUEUE_EN = true
AUDIO_RAWOUTINFO_INBAND_QUEUE_EN = true
AUDIO_ON_SCPU_MS12_EN = true
LOCAL_CFLAGS += -DUDC_PLAYER_EN
LOCAL_SRC_FILES += codec_rtk_ms12.c decoder_ms12.c codec_ms12_gbl_var.c codec_rtk_ms12_filewrite.c
endif

ifeq ($(OMX_AUDIO_CFG), OMX_AUDIO_CFG_4)
AUDIO_DWNSTRM_INBAND_QUEUE_EN = true
AUDIO_RAWOUTINFO_INBAND_QUEUE_EN = true
AUDIO_ON_SCPU_EN = true
endif

ifeq ($(MS12_SWITCH_FILE_EN),true)
LOCAL_CFLAGS += -DMS12_SWITCH_FILE_ENABLED
LOCAL_SRC_FILES += ms12_switch_file.c
endif

ifeq ($(AUDIO_DWNSTRM_INBAND_QUEUE_EN),true)
LOCAL_CFLAGS += -DMS12_DEC_PTSQ_TO_PP
endif

ifeq ($(AUDIO_RAWOUTINFO_INBAND_QUEUE_EN), true)
LOCAL_CFLAGS += -DDEC_RAWOUTINFO_TO_PP
endif

ifeq ($(AUDIO_ON_SCPU_EN),true)
LOCAL_CFLAGS += -DAUDIO_ON_SCPU
LOCAL_SRC_FILES += fw_misc.c
LOCAL_SHARED_LIBRARIES += libfw_socket_client
endif

ifeq ($(AUDIO_ON_SCPU_MS12_EN),true)
LOCAL_CFLAGS += -DAUDIO_ON_SCPU_MS12
LOCAL_SRC_FILES += fw_misc_ms12.c

ifeq ($(AUDIO_ON_SCPU_EN),false)
LOCAL_SRC_FILES += fw_misc.c
LOCAL_SHARED_LIBRARIES += libfw_socket_client
endif

endif


ifeq ($(SUPPORT_MS12_FLOW),true)
LOCAL_CFLAGS += -DMS12_FLOW_EN
LOCAL_CFLAGS += -DMS12_FUTURE_USE
LOCAL_CFLAGS += -DMS12_SINGLE_PLAYER
LOCAL_CFLAGS += -DMS12_FILEWRITE_WITH_ENC
LOCAL_CFLAGS += -DMS12_FILEWRITE_WITH_DD_CNVTR
LOCAL_CFLAGS += -DMS12_CNVTR_RAWOUT_EN
#LOCAL_CFLAGS += -DMS12_DMX_LISTENING_EN
LOCAL_SRC_FILES += codec_rtk_ms12.c decoder_ms12.c codec_ms12_gbl_var.c codec_rtk_ms12_filewrite.c
endif


LOCAL_MODULE := libOMX.realtek.audio.dec
LOCAL_32_BIT_ONLY := true

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES += libRTKOMX_OSAL_RTK
LOCAL_SHARED_LIBRARIES += libRTK_lib
LOCAL_SHARED_LIBRARIES += libaudioutils

LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libz
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libstdc++
LOCAL_SHARED_LIBRARIES += libm
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libion
LOCAL_SHARED_LIBRARIES += libRTKAllocator
LOCAL_SHARED_LIBRARIES += libgui

LOCAL_C_INCLUDES := \
    . \
    $(RTK_INC)/khronos \
    $(RTK_INC)/imagination \
    system/core/include/cutils \
    $(RTK_TOP)/osal_rtk \
    $(RTK_TOP)/core \

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL
LOCAL_C_INCLUDES += system/core/include/ion
LOCAL_C_INCLUDES += device/realtek/firmware/include
LOCAL_C_INCLUDES += system/media/audio_utils/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/fw_socket


ifeq ($(ENABLE_TEE_DRM_FLOW),true)
LOCAL_CFLAGS += -DENABLE_TEE_DRM_FLOW
LOCAL_C_INCLUDES += \
	$(TOP)/device/realtek/proprietary/libs/Include/TEEapi
LOCAL_SHARED_LIBRARIES += libTEEapi
endif

#LOCAL_LDLIBS += -lpthread
#
# create symbolic links to each decoders
#
DECODER_FORMATS += raw
DECODER_FORMATS += secure

SYMLINKS := $(addsuffix .so,$(addprefix $(TARGET_OUT)/lib/$(LOCAL_MODULE).,$(DECODER_FORMATS)))

LOCAL_POST_INSTALL_CMD = \
	$(hide) $(foreach t,$(SYMLINKS),ln -sf $(LOCAL_MODULE).so $(t);)

include $(BUILD_SHARED_LIBRARY)
