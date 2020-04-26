LOCAL_PATH := $(call my-dir)

ifeq ($(PLATFORM_VERSION),4.2.2)
WWW_ROOT := $(TARGET_OUT_DATA)/www
else
WWW_ROOT := $(TARGET_OUT)/data/www
endif

CGI_PATH := $(WWW_ROOT)/cgi-bin
DOC_PATH := $(WWW_ROOT)/htdocs

# =============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
				libfcgi_2.4.1/fcgiapp.c \
				libfcgi_2.4.1/fcgi_stdio.c \
				libfcgi_2.4.1/os_unix.c \
				libfcgi_2.4.1/fcgio.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += bionic

LOCAL_MODULE := libfcgi

include $(BUILD_STATIC_LIBRARY)

# =============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := read.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += system/core/include/cutils
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include

LOCAL_MODULE_TAGS := optional

LOCAL_STATIC_LIBRARIES := libfcgi
LOCAL_SHARED_LIBRARIES += libstagefright
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_SHARED_LIBRARIES += libstagefright_foundation
LOCAL_SHARED_LIBRARIES += libRtkIpc

LOCAL_MODULE_PATH := $(CGI_PATH)

LOCAL_MODULE := read.fcgi

include $(BUILD_EXECUTABLE)
# =============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
				remote.cpp \
				libcgi.cpp \
				response.cpp

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH) \
		$(LOCAL_PATH)/include \
		system/core/include/cutils \
		external/jansson/src \
		external/jansson/android
			
LOCAL_CFLAGS += $(DEFINES)
LOCAL_CFLAGS += -DANDROID	

LOCAL_MODULE_TAGS := optional eng

LOCAL_STATIC_LIBRARIES := libfcgi
LOCAL_SHARED_LIBRARIES := libcutils	libjansson

LOCAL_MODULE_PATH := $(CGI_PATH)

LOCAL_MODULE := remote.fcgi

include $(BUILD_EXECUTABLE)
# =============================================================
include $(CLEAR_VARS)

ifeq ($(PLATFORM_VERSION),4.2.2)
include hardware/realtek/realtek_omx/rtk_libs/system/include/MakeConfig
else
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
endif

LOCAL_SRC_FILES := \
		rtCGI.cpp \
		libcgi.cpp \
		info.cpp \
		setting.cpp \
		util.cpp \
		fs.cpp \
		wifi.cpp \
		response.cpp

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH) \
		$(LOCAL_PATH)/include \
		system/core/include/cutils \
		external/jansson/src \
		external/jansson/android
	    
LOCAL_C_INCLUDES += bionic

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_C_INCLUDES += device/realtek/libs/Include \
		hardware/realtek/realtek_omx/rtk_libs/common \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/generate/include/system \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/include \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/include/xdr \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/src/xdr \
		hardware/realtek/realtek_omx/rtk_libs/OSAL/include \
		hardware/realtek/realtek_omx/rtk_libs/OSAL/include/OSAL
else
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include \
	    device/realtek/proprietary/libs/rtk_libs/common \
	    device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system \
	    device/realtek/proprietary/libs/rtk_libs/common/IPC/include \
	    device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr \
	    device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr \
    	device/realtek/proprietary/libs/rtk_libs/OSAL/include \
    	device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL
endif

LOCAL_CFLAGS += $(DEFINES)
LOCAL_CFLAGS += -DANDROID


LOCAL_MODULE_TAGS := optional eng

LOCAL_STATIC_LIBRARIES := libfcgi
LOCAL_SHARED_LIBRARIES := libcutils libjansson libRTKSetupClass libMediaQuery

ifeq ($(USE_TOGO_MANAGER), YES)
LOCAL_SRC_FILES += togo.cpp
LOCAL_CFLAGS += -DUSE_TOGO_MANAGER

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_C_INCLUDES += device/realtek/libs/Include/Application/AppClass
else
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include/Application/AppClass
endif

LOCAL_SHARED_LIBRARIES += libToGoService libcutils
endif

LOCAL_MODULE_PATH := $(CGI_PATH)

LOCAL_MODULE := rtCGI.fcgi

include $(BUILD_EXECUTABLE)

# =============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := uploadCGI.cpp ccgi-1.1/ccgi.c  
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/include $(LOCAL_PATH)/ccgi-1.1
ifeq ($(PLATFORM_VERSION),4.2.2)
include hardware/realtek/realtek_omx/rtk_libs/system/include/MakeConfig
LOCAL_C_INCLUDES += device/realtek/libs/Include \
		hardware/realtek/realtek_omx/rtk_libs/common \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/generate/include/system \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/include \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/include/xdr \
		hardware/realtek/realtek_omx/rtk_libs/common/IPC/src/xdr \
		hardware/realtek/realtek_omx/rtk_libs/OSAL/include \
		hardware/realtek/realtek_omx/rtk_libs/OSAL/include/OSAL
else
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include \
		device/realtek/proprietary/libs/rtk_libs/common \
		device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system \
		device/realtek/proprietary/libs/rtk_libs/common/IPC/include \
		device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr \
		device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr \
		device/realtek/proprietary/libs/rtk_libs/OSAL/include \
		device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL
endif
LOCAL_CFLAGS += $(DEFINES) -DANDROID

LOCAL_MODULE_TAGS := optional eng

LOCAL_SHARED_LIBRARIES := libcutils libRTKSetupClass

LOCAL_STATIC_LIBRARIES := libfcgi

LOCAL_MODULE_PATH := $(CGI_PATH)

LOCAL_MODULE := uploadCGI.fcgi

include $(BUILD_EXECUTABLE)

# =============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := togoRead.fcgi

LOCAL_MODULE_PATH := $(CGI_PATH)

LOCAL_MODULE_TAGS := optional eng

LOCAL_SRC_FILES := togoRead.cpp libcgi.cpp util.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/include \
	device/realtek/proprietary/libs/Include \
	frameworks/native/include \
	frameworks/av/include

ifeq ($(ENABLE_1195_DTV_FLOW), YES)
LOCAL_CFLAGS += -DENABLE_1195_DTV_FLOW
endif

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_C_INCLUDES += device/realtek/libs/Include/Application/AppClass
else
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include/Application/AppClass
endif

LOCAL_STATIC_LIBRARIES := libfcgi

LOCAL_SHARED_LIBRARIES := liblog \
	librealtek_runtime \
	libbinder \
	libutils \
	libRtkIpc \
	libToGoService

include $(BUILD_EXECUTABLE)

# =============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := togoOutCGI.cgi

LOCAL_MODULE_PATH := $(CGI_PATH)

LOCAL_MODULE_TAGS := optional eng

LOCAL_SRC_FILES := togoOutCGI.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	device/realtek/proprietary/libs/Include \
	frameworks/native/include \
	frameworks/av/include

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_C_INCLUDES += device/realtek/libs/Include/Application/AppClass
else
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include/Application/AppClass
endif

LOCAL_SHARED_LIBRARIES := librealtek_runtime \
	libbinder \
	libutils \
	libcutils \
	libRtkIpc \
	libToGoService

include $(BUILD_EXECUTABLE)

# =============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := AirFun.apk

LOCAL_SRC_FILES := apps/$(LOCAL_MODULE)

LOCAL_MODULE_PATH := $(DOC_PATH)

LOCAL_MODULE_CLASS := EXECUTABLES

LOCAL_MODULE_TAGS := optional eng

include $(BUILD_PREBUILT)

# =============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := AirFunInstall.exe

LOCAL_SRC_FILES := apps/$(LOCAL_MODULE)

LOCAL_MODULE_PATH := $(DOC_PATH)

LOCAL_MODULE_CLASS := EXECUTABLES

LOCAL_MODULE_TAGS := optional eng

include $(BUILD_PREBUILT)

