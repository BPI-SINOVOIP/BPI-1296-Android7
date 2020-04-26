LOCAL_PATH := $(call my-dir)

# Building the jpuapi
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
 	jpuapi.c \
	jpuapifunc.c \
	../src/jpuhelper.c \
	../src/jpulog.c

 LOCAL_SRC_FILES += \
	../jdi/linux/jdi.c \


LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libjpu

# LOCAL_CFLAGS +=  -DCNM_FPGA_PLATFORM


LOCAL_SHARED_LIBRARIES :=       \
		libutils		\
		libcutils		\
		libdl  			\
		liblog



LOCAL_C_INCLUDES := $(LOCAL_PATH)/src		\
					$(TOP)/hardware/realtek/VideoEngine/JPEG	\
					$(TOP)/hardware/realtek/VideoEngine/JPEG/include




include $(BUILD_SHARED_LIBRARY)

