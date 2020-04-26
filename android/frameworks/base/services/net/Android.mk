LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := services.net

LOCAL_SRC_FILES += \
    $(call all-java-files-under,java)

LOCAL_AIDL_INCLUDES += \
    system/netd/server/binder

LOCAL_STATIC_JAVA_LIBRARIES := com.rtk.net.ubusrpc

include $(BUILD_STATIC_JAVA_LIBRARY)
