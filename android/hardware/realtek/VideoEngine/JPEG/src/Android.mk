
LOCAL_PATH := $(call my-dir)

#Building jpurun binary which will be placed in the /system/bin folder
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	main.c \
	jpurun.c \
	mixer.c

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := jpurun


# LOCAL_CFLAGS +=  -DCNM_FPGA_PLATFORM

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(TOP)/hardware/realtek/VideoEngine/JPEG/include \
                    $(TOP)/hardware/realtek/VideoEngine/JPEG/jpuapi



LOCAL_SHARED_LIBRARIES :=       \
        libjpu					\
        libutils


include $(BUILD_EXECUTABLE)

