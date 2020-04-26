
LOCAL_PATH := $(call my-dir)

BUILD_MULTI_INSTANCE_TEST = 0


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


ifeq ($(BUILD_MULTI_INSTANCE_TEST), 1)
    CNM_CFLAGS += -DMULTI_INSTANCE_TEST
endif

#Building vpurun binary which will be placed in the /system/bin folder

SRC_FILES := helper/main_helper.c \
          helper/vpuhelper.c \
          helper/bitstream/bitstreamfeeder.c \
          helper/bitstream/bitstreamreader.c \
          helper/bitstream/bsfeeder_fixedsize_impl.c \
          helper/bitstream/bsfeeder_framesize_impl.c \
          helper/bitstream/bsfeeder_size_plus_es_impl.c \
          helper/comparator/bin_comparator_impl.c \
          helper/comparator/comparator.c \
          helper/comparator/md5_comparator_impl.c \
          helper/comparator/yuv_comparator_impl.c \
          helper/display/fbdev_impl.c \
          helper/display/hdmi_impl.c \
          helper/display/simplerenderer.c \
          helper/misc/cfgParser.c \
          helper/misc/cnm_fpga.c \
          helper/misc/cnm_video_helper.c \
          helper/misc/container.c \
          helper/misc/datastructure.c \
          helper/misc/platform.c \
          helper/yuv/yuvfeeder.c \
          helper/yuv/yuvLoaderfeeder.c

ifeq ($(BUILD_MULTI_INSTANCE_TEST), 1)
    SRC_FILES += main_w4_dec_test.c
endif




C_INCLUDES := $(LOCAL_PATH) \
		$(TOP)/hardware/realtek/VideoEngine/VE1/vpuapi \
		$(TOP)/hardware/realtek/VideoEngine/VE1/sample \
		$(TOP)/hardware/realtek/VideoEngine/VE1/sample/helper



SHARED_LIBRARIES := \
        libvpu \
        libutils

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(C_INCLUDES)
LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)
LOCAL_SRC_FILES := main_coda980_dec_test.c
LOCAL_SRC_FILES += $(SRC_FILES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := coda980_dec_test
LOCAL_CFLAGS := $(CNM_CFLAGS)
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(C_INCLUDES)
LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)
LOCAL_SRC_FILES := main_coda980_enc_test.c
LOCAL_SRC_FILES += $(SRC_FILES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := coda980_enc_test
LOCAL_CFLAGS := $(CNM_CFLAGS)
include $(BUILD_EXECUTABLE)

ifeq ($(BUILD_MULTI_INSTANCE_TEST), 0)
    include $(CLEAR_VARS)
    LOCAL_C_INCLUDES := $(C_INCLUDES)
    LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)
    LOCAL_SRC_FILES := main_w4_dec_test.c
    LOCAL_SRC_FILES += $(SRC_FILES)
    LOCAL_MODULE_TAGS := optional
    LOCAL_MODULE := w4_dec_test
    LOCAL_CFLAGS := $(CNM_CFLAGS)
    include $(BUILD_EXECUTABLE)
else
    include $(CLEAR_VARS)
    LOCAL_C_INCLUDES := $(C_INCLUDES)
    LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)
    LOCAL_SRC_FILES := main_multi_instance_test.c
    LOCAL_SRC_FILES += $(SRC_FILES)
    LOCAL_MODULE_TAGS := optional
    LOCAL_MODULE := multi_instance_test
    LOCAL_CFLAGS := $(CNM_CFLAGS)
    include $(BUILD_EXECUTABLE)
endif

#include $(CLEAR_VARS)
#LOCAL_C_INCLUDES := $(C_INCLUDES)
#LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)
#LOCAL_SRC_FILES := main_w4_enc_test.c
#LOCAL_SRC_FILES += $(SRC_FILES)
#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := w4_enc_test
#LOCAL_CFLAGS := $(CNM_CFLAGS)
#include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(C_INCLUDES)
LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)
LOCAL_SRC_FILES := main_vpuinit.c
LOCAL_SRC_FILES += $(SRC_FILES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := vpuinit
LOCAL_CFLAGS := $(CNM_CFLAGS)
include $(BUILD_EXECUTABLE)

