LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    DLNA_DMR.cpp \
    DLNA_DMR_jni.cpp \
    DMR_MM.cpp \
    DMR_Playlist.cpp
    
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/../libupnp3/include \
    $(LOCAL_PATH)/../libneon/src
    
LOCAL_CFLAGS := \
    -DMMSCP_LEAN_AND_MEAN -DINCLUDE_FEATURE_VOLUME -D_POSIX -D_ANDROID

ifeq ($(USE_DTCPIP_SIK), YES)
LOCAL_CFLAGS += -DENABLE_DTCP_IP
endif

LOCAL_STATIC_LIBRARIES := libneon

LOCAL_SHARED_LIBRARIES := libupnp3 libcutils libnativehelper 

ifeq ($(DLNADMRCTT), YES)
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../libDLNADMPClass/include
  LOCAL_CFLAGS += -DDLNADMRCTT
  LOCAL_SHARED_LIBRARIES += libDLNADMPClass
endif

LOCAL_MODULE := libDLNADMRClass

LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
