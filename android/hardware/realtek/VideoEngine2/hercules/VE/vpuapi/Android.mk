LOCAL_PATH := $(call my-dir)

# Building the vpuapi
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
 	../vdi/linux/vdi.c \
   	../vdi/linux/vdi_osal.c

LOCAL_SRC_FILES += \
 	vpuapi.c \
	product.c \
	coda9/coda9.c \
	wave/wave5.c \
	vpuapifunc.c 

#LOCAL_SRC_FILES += \
#	../sample/helper/vpuhelper.c


LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvpu

LOCAL_CFLAGS := -DCONFIG_DEBUG_LEVEL=255 $(CNM_CFLAGS)


LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES :=       \
		libutils      \
		libdl  		 \
	   	liblog	

		
GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).c)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).c
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include


include $(BUILD_SHARED_LIBRARY)

