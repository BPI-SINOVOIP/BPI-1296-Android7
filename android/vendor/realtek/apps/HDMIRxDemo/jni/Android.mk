LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include \
		device/realtek/proprietary/libs/rtk_libs/common/IPC/include \
		device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system

LOCAL_SRC_FILES := jni_funs.cpp

LOCAL_SHARED_LIBRARIES := liblog \
		libnativehelper \
		libcutils

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libhdmidemoutils
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)

