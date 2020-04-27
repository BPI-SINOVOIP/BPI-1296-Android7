LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)


LOCAL_SRC_FILES := \
    OnLoad.cpp  \
    rtksmb_jni.cpp \
    rtksmblib.cpp \
    rtkini/rtk_ini.cpp
    

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) \
    $(LOCAL_PATH)/rtkini

LOCAL_SHARED_LIBRARIES := libcutils libnativehelper

LOCAL_MODULE := librtksmb
#LOCAL_32_BIT_ONLY := true

LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
