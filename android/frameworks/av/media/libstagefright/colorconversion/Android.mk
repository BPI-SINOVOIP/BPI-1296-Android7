LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=                     \
        ColorConverter.cpp            \
        SoftwareRenderer.cpp

#LOCAL_CFLAGS_32 += -DLIBYUV_NEON
LOCAL_SRC_FILES += third_party/libyuv/source/row_neon.cpp
LOCAL_SRC_FILES += third_party/libyuv/source/row_any.cpp
LOCAL_SRC_FILES += third_party/libyuv/source/row_common.cpp
LOCAL_SRC_FILES += third_party/libyuv/source/convert_wrapper.cpp


LOCAL_C_INCLUDES := \
        $(TOP)/frameworks/native/include/media/openmax \
        $(TOP)/hardware/msm7k \
        $(TOP)/frameworks/av/media/libstagefright/colorconversion/third_party/libyuv/include 
        #$(TOP)/external/libyuv/files/include

LOCAL_STATIC_LIBRARIES := \
        libyuv_static \

LOCAL_CFLAGS += -Werror
LOCAL_CLANG := true
LOCAL_SANITIZE := signed-integer-overflow

LOCAL_MODULE:= libstagefright_color_conversion

include $(BUILD_STATIC_LIBRARY)
