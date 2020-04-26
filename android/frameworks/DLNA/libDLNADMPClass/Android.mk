LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)


LOCAL_SRC_FILES := \
    OnLoad.cpp  \
    DLNA_DMP_1p5.cpp \
    RTK_MmsCp_1p5.cpp   \
    DLNA_DMP_1p5_jni.cpp
    


LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/../libupnp3/include/

LOCAL_CFLAGS := \
    -DIS_TV_CHIP \
    -DMMSCP_LEAN_AND_MEAN -D_ANDROID

LOCAL_SHARED_LIBRARIES := libupnp3 libcutils libnativehelper

LOCAL_MODULE := libDLNADMPClass

LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)