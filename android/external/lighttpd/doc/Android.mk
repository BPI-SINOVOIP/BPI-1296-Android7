LOCAL_PATH := $(call my-dir)

#####################################################
include $(CLEAR_VARS)

LOCAL_MODULE := lighttpd.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/lighttpd
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/lighttpd.conf

include $(BUILD_PREBUILT)

######################################################
include $(CLEAR_VARS)

LOCAL_MODULE := modules.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/lighttpd
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/modules.conf

include $(BUILD_PREBUILT)

######################################################
include $(CLEAR_VARS)

LOCAL_MODULE := mime.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/lighttpd/conf.d
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/conf.d/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

######################################################
include $(CLEAR_VARS)

LOCAL_MODULE := cgi.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/lighttpd/conf.d
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/conf.d/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

######################################################
include $(CLEAR_VARS)

LOCAL_MODULE := fastcgi.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/lighttpd/conf.d
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/conf.d/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)
######################################################
include $(CLEAR_VARS)

LOCAL_MODULE := access_log.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/lighttpd/conf.d
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/conf.d/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

######################################################

include $(CLEAR_VARS)

LOCAL_MODULE := init.lighttpd.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := config/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

######################################################

include $(CLEAR_VARS)

LOCAL_MODULE := startlighttpd.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS := eng optional
LOCAL_SRC_FILES := scripts/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

######################################################
