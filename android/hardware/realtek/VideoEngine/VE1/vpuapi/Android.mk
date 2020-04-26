LOCAL_PATH := $(call my-dir)

# FOR C&M FPGA Environment
CNM_FPGA_FLAGS := -DCNM_FPGA_PLATFORM -DCNM_FPGA_USB_INTERFACE -DSUPPORT_USB_TRANSFER -DSUPPORT_CONF_TEST
ifeq ($(PRODUCT), 7Q)
CNM_CFLAGS := -DCODA7Q $(CNM_FPGA_FLAGS)
endif
ifeq ($(PRODUCT), 960)
CNM_CFLAGS := -DCODA960 $(CNM_FPGA_FLAGS)
endif
ifeq ($(PRODUCT), 980)
CNM_CFLAGS := -DCODA980 $(CNM_FPGA_FLAGS)
endif
ifeq ($(PRODUCT), 410)
CNM_CFLAGS := -DWAVE410 $(CNM_FPGA_FLAGS)
endif
ifeq ($(PRODUCT), 412)
CNM_CFLAGS := -DWAVE412 $(CNM_FPGA_FLAGS)
endif
ifeq ($(PRODUCT), 420)
CNM_CFLAGS := -DWAVE420 $(CNM_FPGA_FLAGS)
endif
#

# Building the vpuapi
include $(CLEAR_VARS)

# workaround avoid HEVC decode error
#LOCAL_CLANG := false

LOCAL_SRC_FILES :=			\
 	../vdi/linux/vdi.c 		\
   	../vdi/linux/vdi_osal.c

LOCAL_SRC_FILES += \
 	vpuapi.c \
	product.c \
	coda9/coda9.c \
	wave/common/common.c \
	wave/coda7q/coda7q.c \
	wave/wave4/wave4.c \
	vpuapifunc.c

LOCAL_SRC_FILES += \
	../aes/aes.c \

LOCAL_SRC_FILES += \
	../sample/helper/vpuhelper.c


LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvpu

LOCAL_CFLAGS := -DCONFIG_DEBUG_LEVEL=255 $(CNM_CFLAGS)

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := \
		libutils \
		libdl \
	   	liblog	

ifeq ($(ENABLE_TEE_DRM_FLOW),true)
LOCAL_SRC_FILES += wave/common/common_tee.c
LOCAL_CFLAGS += -DENABLE_TEE_DRM_FLOW
LOCAL_C_INCLUDES += \
    $(TOP)/device/realtek/proprietary/libs/Include/TEEapi
	LOCAL_SHARED_LIBRARIES += libTEEapi
endif

LOCAL_SHARED_LIBRARIES += libion
LOCAL_C_INCLUDES += \
	$(TOP)/system/core/include

GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include

LOCAL_SHARED_LIBRARIES += libRTK_lib
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include/
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr
include $(BUILD_SHARED_LIBRARY)

