LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CLANG := true

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SRC_FILES:= \
	Client.cpp  \
	SBConsumer.cpp  \
	SBLayer.cpp  \
	SBRefClock.cpp \
	Sideband.cpp \
 	main_sideband.cpp

LOCAL_CFLAGS:= -DLOG_TAG=\"Sideband\"

LOCAL_CFLAGS += -fvisibility=hidden -Werror=format
LOCAL_CFLAGS += -std=c++11

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	liblog \
	libutils\
	libbinder \
	libui \
	libgui

LOCAL_MODULE:= libsideband
ifdef TARGET_32_BIT_SURFACEFLINGER
LOCAL_32_BIT_ONLY := true
endif

LOCAL_SHARED_LIBRARIES += libhardware
LOCAL_SHARED_LIBRARIES += libsync

ANDROID_VERSION_5_0_0 = true
ifeq ($(ANDROID_VERSION_5_0_0), true)
LOCAL_CFLAGS += -DANDROID_VERSION_5_0_0
LOCAL_C_INCLUDES += system/core/libsync/
LOCAL_C_INCLUDES += system/core/libsync/include/
endif

#+-----------------------#
#|	For HWLayer			|#
#+-----------------------#
RVSD_VORPC = true

ifeq ($(RVSD_VORPC), true)
LOCAL_CFLAGS += -DUSE_RT_ION
LOCAL_CFLAGS += -DRVSD_RPC_VO_ENABLED
LOCAL_CFLAGS += -DRVSD_RPC_VO_PLOCK
LOCAL_CFLAGS += -DRVSD_RPC_RVSD_HANDLE
LOCAL_CFLAGS += -DRVSD_RPC_VO_ACODEC

LOCAL_SRC_FILES += Display/VOLayer/VOLayer.cpp
LOCAL_SRC_FILES += Display/VOLayer/VOWindowRPC.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/RingBufferBase.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/LayerBase.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/PLockBase.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/BufferBase.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/IonBuffer.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/GraphicBufferInfo.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/RefClockBase.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/SWFence.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/utils/HDCPMessage.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/video/VideoLayer.cpp
LOCAL_SRC_FILES += Display/VOLayer/hwlayer/video/VideoBuffer.cpp
#LOCAL_SRC_FILES += Display/VOLayer/hwlayer/cursor/ColorLookUpTable.cpp
#LOCAL_SRC_FILES += Display/VOLayer/hwlayer/cursor/CursorTexture.cpp
#LOCAL_SRC_FILES += Display/VOLayer/hwlayer/cursor/CursorBuffer.cpp
#LOCAL_SRC_FILES += Display/VOLayer/hwlayer/cursor/CursorLayer.cpp

LOCAL_SHARED_LIBRARIES += libRTK_lib
LOCAL_SHARED_LIBRARIES += libRTKOMX_OSAL_RTK
LOCAL_SHARED_LIBRARIES += libion

LOCAL_C_INCLUDES += $(LOCAL_PATH)/Display/VOLayer/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Display/VOLayer/hwlayer/

LOCAL_C_INCLUDES += system/core/include/ion/
LOCAL_C_INCLUDES += hardware/realtek/realtek_omx/

LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include/Application/AppClass/
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/Include/
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include
LOCAL_C_INCLUDES += device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL
endif

ifeq ($(USE_RT_MEDIA_PLAYER),true)
LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER

ifeq ($(USE_RTK_VOUT_UTIL),YES)
include device/realtek/proprietary/libs/rtk_libs/system/include/MakeConfig
LOCAL_CFLAGS += $(DEFINES) -DSYS_UNIX=1 -DUSE_RTK_VOUT_UTIL
LOCAL_SHARED_LIBRARIES += libRTKVoutUtil
LOCAL_C_INCLUDES += \
	device/realtek/proprietary/libs/Include/Application/AppClass/\
	device/realtek/proprietary/libs/Include/\
	device/realtek/proprietary/libs/rtk_libs/common\
	device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system\
	device/realtek/proprietary/libs/rtk_libs/common/IPC/include\
	device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr\
	device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr\
	device/realtek/proprietary/libs/rtk_libs/OSAL/include\
	device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL

ifeq ($(USE_SETTINGS_SERVICE), true)
LOCAL_CFLAGS += -DUseSetupClassAndroid
else
LOCAL_C_INCLUDES += external/sqlite/dist
endif
endif
endif

LOCAL_32_BIT_ONLY := true

GIT_SHA_NAME := $(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)
$(shell test -e $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).cpp || echo "#include <RTKRevision.h>" > $(LOCAL_PATH)/.RTKRevision.$(GIT_SHA_NAME).cpp)
LOCAL_CFLAGS           += -D'GIT_VERSION="$(GIT_SHA_NAME)"'
LOCAL_SRC_FILES        += .RTKRevision.$(GIT_SHA_NAME).cpp
LOCAL_C_INCLUDES       += device/realtek/proprietary/libs/RTKRevision/include
include $(BUILD_SHARED_LIBRARY)

###############################################################
# build Sideband's executable
include $(CLEAR_VARS)

LOCAL_LDFLAGS := -Wl,--version-script,art/sigchainlib/version-script.txt -Wl,--export-dynamic
LOCAL_CFLAGS:= -DLOG_TAG=\"Sideband\"
LOCAL_CPPFLAGS:= -std=c++11

LOCAL_INIT_RC := sideband.rc

LOCAL_SRC_FILES:= \
	main_sideband.cpp

LOCAL_SHARED_LIBRARIES := \
	libsideband \
	libcutils \
	liblog \
	libbinder \
	libutils

LOCAL_SHARED_LIBRARIES += libRTK_lib libRTKOMX_OSAL_RTK libion
LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/Display/VOLayer/ \
					$(LOCAL_PATH)/Display/VOLayer/hwlayer/ \
					system/core/include/ion/ \
					hardware/realtek/realtek_omx/ \
					device/realtek/proprietary/libs/Include/Application/AppClass/ \
					device/realtek/proprietary/libs/Include/ \
					device/realtek/proprietary/libs/rtk_libs/common \
					device/realtek/proprietary/libs/rtk_libs/common/IPC/generate/include/system \
					device/realtek/proprietary/libs/rtk_libs/common/IPC/include \
					device/realtek/proprietary/libs/rtk_libs/common/IPC/include/xdr \
					device/realtek/proprietary/libs/rtk_libs/common/IPC/src/xdr \
					device/realtek/proprietary/libs/rtk_libs/OSAL/include \
					device/realtek/proprietary/libs/rtk_libs/OSAL/include/OSAL


LOCAL_WHOLE_STATIC_LIBRARIES := libsigchain

LOCAL_MODULE:= sideband

ifdef TARGET_32_BIT_SURFACEFLINGER
LOCAL_32_BIT_ONLY := true
endif
LOCAL_32_BIT_ONLY := true

include $(BUILD_EXECUTABLE)
