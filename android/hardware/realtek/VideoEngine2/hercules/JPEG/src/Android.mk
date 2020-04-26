
LOCAL_PATH := $(call my-dir)

#Building jpurun binary which will be placed in the /system/bin folder
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	main.c \
	jpurun.c \
	mixer.c

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := jpurun

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(TOP)/hardware/realtek/VideoEngine2/hercules/JPEG/include \
                    $(TOP)/hardware/realtek/VideoEngine2/hercules/JPEG/jpuapi



LOCAL_SHARED_LIBRARIES :=       \
        libjpu					\
        libutils


include $(BUILD_EXECUTABLE)

