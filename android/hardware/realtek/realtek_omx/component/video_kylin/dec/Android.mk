LOCAL_PATH := $(call my-dir)
RTK_TOP	:= hardware/realtek/realtek_omx
RTK_INC := $(RTK_TOP)/include
RTK_RPC := $(TOP)/device/realtek/proprietary/libs/rtk_libs/common
RTK_VE1:= $(TOP)/hardware/realtek/VideoEngine/VE1
RTK_VE3:= $(TOP)/hardware/realtek/VideoEngine/VE3
RTK_JPG:= $(TOP)/hardware/realtek/VideoEngine/JPEG

DECODER_TYPE = VIDEO
OMX_DECODER_TUNNELING_SUPPORT = true
# build video decoder
include $(CLEAR_VARS)

ifeq ($(DECODER_TYPE), VIDEO)
LOCAL_PRELINK_MODULE := false

LOCAL_SRC_FILES := \
	decoder.c \
	codec.c \
	codec_vdec.c \
	codec_jdec.c

LOCAL_MODULE := libOMX.realtek.video.dec
LOCAL_32_BIT_ONLY := true

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES += libRTKOMX_OSAL_RTK
LOCAL_SHARED_LIBRARIES += libRTK_lib

LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libz
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libstdc++
LOCAL_SHARED_LIBRARIES += libm
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libhardware
LOCAL_SHARED_LIBRARIES += libhwse.$(TARGET_BOARD_PLATFORM)
LOCAL_SHARED_LIBRARIES += libvpu
LOCAL_SHARED_LIBRARIES += libjpu
LOCAL_SHARED_LIBRARIES += libRtkIpc
LOCAL_SHARED_LIBRARIES += libion
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_CFLAGS = -DOMX_DECODER_VIDEO_DOMAIN -DUSE_OUTPUT_CROP
LOCAL_CFLAGS += -DUSE_RT_ION

ifeq ($(OMX_DECODER_TUNNELING_SUPPORT), true)
LOCAL_SRC_FILES += sidebandOutput.c Sideband_C.cpp
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libskia
LOCAL_SHARED_LIBRARIES += libgui
LOCAL_SHARED_LIBRARIES += libui
LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_CFLAGS += -DOMX_DECODER_TUNNELING_SUPPORT
endif

LOCAL_SRC_FILES += codec_HEVCHdrParameterParser.c
LOCAL_C_INCLUDES := \
	. \
	$(RTK_INC)/khronos \
	$(RTK_INC)/imagination \
	system/core/include \
	system/core/include/cutils \
	$(RTK_RPC)/IPC/include \
	$(RTK_TOP)/osal_rtk \
	$(RTK_TOP)/core \
	$(RTK_VE1)/vpuapi \
	$(RTK_JPG)/include \
	$(RTK_JPG)/jpuapi \
	$(RTK_JPG) \
	hardware/realtek/StreamingEngine2
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include

#CODEC_VO_WRITEBACK
LOCAL_SRC_FILES += vo_writeback/codec_VORPC.c
LOCAL_C_INCLUDES += $(RTK_RPC)/IPC/generate/include/system

ifeq ($(ENABLE_TEE_DRM_FLOW),true)
LOCAL_CFLAGS += -DENABLE_TEE_DRM_FLOW
LOCAL_C_INCLUDES += \
	$(TOP)/device/realtek/proprietary/libs/Include/TEEapi
LOCAL_SHARED_LIBRARIES += libTEEapi
else
# For secureplayback workaround
LOCAL_CFLAGS += -DDISABLE_SECURE_PATH
endif

#LOCAL_LDLIBS += -lpthread
#
# create symbolic links to each decoders
#
DECODER_FORMATS += avc
DECODER_FORMATS += mpeg4
DECODER_FORMATS += vp8
DECODER_FORMATS += 3gpp
DECODER_FORMATS += mpeg2
DECODER_FORMATS += raw
DECODER_FORMATS += vc1
DECODER_FORMATS += rv
DECODER_FORMATS += divx3
DECODER_FORMATS += avs
DECODER_FORMATS += mjpg
DECODER_FORMATS += hevc
DECODER_FORMATS += wvc1
DECODER_FORMATS += wmv3
DECODER_FORMATS += wmv
DECODER_FORMATS += mp43
DECODER_FORMATS += flv
DECODER_FORMATS += rv30
DECODER_FORMATS += rv40
DECODER_FORMATS += hevc.secure
DECODER_FORMATS += avc.secure

SYMLINKS := $(addsuffix .so,$(addprefix $(TARGET_OUT)/lib/$(LOCAL_MODULE).,$(DECODER_FORMATS)))

LOCAL_POST_INSTALL_CMD = \
	$(hide) $(foreach t,$(SYMLINKS),ln -sf $(LOCAL_MODULE).so $(t);)

GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include
LOCAL_C_INCLUDES += system/core/libsync/
LOCAL_SHARED_LIBRARIES += libsync

include $(BUILD_SHARED_LIBRARY)
endif # end of DECODER_TYPE

###########################################################################

DECODER_TYPE = IMAGE
OMX_DECODER_TUNNELING_SUPPORT = false
# build image decoder
include $(CLEAR_VARS)

ifeq ($(DECODER_TYPE), IMAGE)
LOCAL_PRELINK_MODULE := false

LOCAL_SRC_FILES := \
	decoder.c \
	codec.c \
	codec_jdec.c

LOCAL_MODULE := libOMX.realtek.image.dec
LOCAL_32_BIT_ONLY := true

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES += libRTKOMX_OSAL_RTK
LOCAL_SHARED_LIBRARIES += libRTK_lib

LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libz
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libstdc++
LOCAL_SHARED_LIBRARIES += libm
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libhardware
LOCAL_SHARED_LIBRARIES += libhwse.$(TARGET_BOARD_PLATFORM)
LOCAL_SHARED_LIBRARIES += libjpu
LOCAL_SHARED_LIBRARIES += libRtkIpc
LOCAL_SHARED_LIBRARIES += libion
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_CFLAGS = -DOMX_DECODER_IMAGE_DOMAIN -DUSE_OUTPUT_CROP
LOCAL_CFLAGS += -DUSE_RT_ION

LOCAL_C_INCLUDES := \
	. \
	$(RTK_INC)/khronos \
	$(RTK_INC)/imagination \
	system/core/include \
	system/core/include/cutils \
	$(RTK_RPC)/IPC/include \
	$(RTK_TOP)/osal_rtk \
	$(RTK_TOP)/core \
	$(RTK_JPG)/include \
	$(RTK_JPG)/jpuapi \
	$(RTK_JPG) \
	hardware/realtek/StreamingEngine2
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include
#LOCAL_LDLIBS += -lpthread

LOCAL_SRC_FILES += vo_writeback/codec_VORPC.c
LOCAL_C_INCLUDES += $(RTK_RPC)/IPC/generate/include/system

GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include
LOCAL_C_INCLUDES += system/core/libsync/
LOCAL_SHARED_LIBRARIES += libsync

include $(BUILD_SHARED_LIBRARY)
endif # end of DECODER_TYPE

DECODER_TYPE = VP9
OMX_DECODER_TUNNELING_SUPPORT = true
# build video decoder
include $(CLEAR_VARS)

ifeq ($(DECODER_TYPE), VP9)
LOCAL_PRELINK_MODULE := false

LOCAL_SRC_FILES := \
	decoder.c \
	codec.c \
	codec_vp9.c

LOCAL_MODULE := libOMX.realtek.video.dec.vp9
LOCAL_32_BIT_ONLY := true

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES += libRTKOMX_OSAL_RTK
LOCAL_SHARED_LIBRARIES += libRTK_lib

LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libz
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libstdc++
LOCAL_SHARED_LIBRARIES += libm
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libhardware
LOCAL_SHARED_LIBRARIES += libhwse.$(TARGET_BOARD_PLATFORM)
LOCAL_SHARED_LIBRARIES += libvp9
LOCAL_SHARED_LIBRARIES += libRtkIpc
LOCAL_SHARED_LIBRARIES += libion
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libRTKAllocator

LOCAL_CFLAGS = -DOMX_DECODER_VIDEO_DOMAIN -DOMX_DECODER_VP9_DOMAIN -DUSE_OUTPUT_CROP
LOCAL_CFLAGS += -DUSE_RT_ION

LOCAL_CFLAGS += -DFIFO_DATATYPE=void*
LOCAL_CFLAGS += -DDOWN_SCALER
LOCAL_CFLAGS += -D_ASSERT_USED
LOCAL_CFLAGS += -DDWL_DISABLE_REG_PRINTS
LOCAL_CFLAGS += -DENABLE_VP9_SUPPORT
LOCAL_CFLAGS += -DDWL_USE_DEC_IRQ
LOCAL_CFLAGS += -DUSE_64BIT_ENV
LOCAL_CFLAGS += -DUSE_EXTERNAL_BUFFER

ifeq ($(OMX_DECODER_TUNNELING_SUPPORT), true)
LOCAL_SRC_FILES += sidebandOutput.c Sideband_C.cpp
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libskia
LOCAL_SHARED_LIBRARIES += libgui
LOCAL_SHARED_LIBRARIES += libui
LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_CFLAGS += -DOMX_DECODER_TUNNELING_SUPPORT
endif

LOCAL_C_INCLUDES := \
	. \
	$(RTK_INC)/khronos \
	$(RTK_INC)/imagination \
	system/core/include \
	system/core/include/cutils \
	$(RTK_RPC)/IPC/include \
	$(RTK_TOP)/osal_rtk \
	$(RTK_TOP)/core \
	$(RTK_VE3) \
	$(RTK_VE3)/software/source/inc \
	hardware/realtek/StreamingEngine2
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/libRTKAllocator/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include
ifeq ($(ENABLE_TEE_DRM_FLOW),true)
LOCAL_CFLAGS += -DENABLE_TEE_DRM_FLOW
LOCAL_C_INCLUDES += \
	$(TOP)/device/realtek/proprietary/libs/Include/TEEapi
LOCAL_SHARED_LIBRARIES += libTEEapi
else
# For secureplayback workaround
LOCAL_CFLAGS += -DDISABLE_SECURE_PATH
endif

LOCAL_SRC_FILES += vo_writeback/codec_VORPC.c
LOCAL_C_INCLUDES += $(RTK_RPC)/IPC/generate/include/system

DECODER_FORMATS = secure
SYMLINKS := $(addsuffix .so,$(addprefix $(TARGET_OUT)/lib/$(LOCAL_MODULE).,$(DECODER_FORMATS)))
LOCAL_POST_INSTALL_CMD = \
	$(hide) $(foreach t,$(SYMLINKS),ln -sf $(LOCAL_MODULE).so $(t);)

GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include
LOCAL_C_INCLUDES += system/core/libsync/
LOCAL_SHARED_LIBRARIES += libsync

include $(BUILD_SHARED_LIBRARY)
endif # end of DECODER_TYPE
