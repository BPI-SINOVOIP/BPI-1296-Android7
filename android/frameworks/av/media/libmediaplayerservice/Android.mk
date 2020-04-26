LOCAL_PATH:= $(call my-dir)

#
# libmediaplayerservice
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
    ActivityManager.cpp         \
    HDCP.cpp                    \
    MediaPlayerFactory.cpp      \
    MediaPlayerService.cpp      \
    MediaRecorderClient.cpp     \
    MetadataRetrieverClient.cpp \
    RemoteDisplay.cpp           \
    StagefrightRecorder.cpp     \
    TestPlayerStub.cpp          \

LOCAL_SHARED_LIBRARIES :=       \
    libbinder                   \
    libcamera_client            \
    libcrypto                   \
    libcutils                   \
    libdrmframework             \
    liblog                      \
    libdl                       \
    libgui                      \
    libmedia                    \
    libmediautils               \
    libmemunreachable           \
    libsonivox                  \
    libstagefright              \
    libstagefright_foundation   \
    libstagefright_httplive     \
    libstagefright_omx          \
    libstagefright_wfd          \
    libutils                    \
    libvorbisidec               \

ifeq ($(USE_RT_MEDIA_PLAYER),true)

ifeq ($(PLATFORM_VERSION),4.2.2)
LOCAL_CFLAGS += -DAndroid_JB
endif
ifeq ($(PLATFORM_VERSION),4.4)
LOCAL_CFLAGS += -DAndroid_KitKate
else ifeq ($(PLATFORM_VERSION),4.4.4)
LOCAL_CFLAGS += -DAndroid_KitKate
endif

ifeq ($(USE_RTK_VOUT_UTIL), YES)
LOCAL_CFLAGS += -DUSE_RTK_VOUT_UTIL
endif
LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER
LOCAL_SHARED_LIBRARIES +=       \
    libRTMediaPlayer            \
    libRT_IPC                   \
    libRTDtvMediaPlayer         \
    libaudioParser
endif

ifeq ($(ENABLE_RT_WIFIDISPLAY),true)
LOCAL_CFLAGS += -DENABLE_RT_WIFIDISPLAY
LOCAL_SHARED_LIBRARIES +=       \
    libhdcp2                    \
    libwfdisplay
endif

LOCAL_STATIC_LIBRARIES :=       \
    libstagefright_nuplayer     \
    libstagefright_rtsp         \
    libstagefright_timedtext    \

LOCAL_C_INCLUDES :=                                                 \
    $(TOP)/frameworks/av/media/libstagefright/include               \
    $(TOP)/frameworks/av/media/libstagefright/rtsp                  \
    $(TOP)/frameworks/av/media/libstagefright/wifi-display          \
    $(TOP)/frameworks/av/media/libstagefright/webm                  \
    $(TOP)/frameworks/av/include/media                              \
    $(TOP)/frameworks/av/include/camera                             \
    $(TOP)/frameworks/native/include/media/openmax                  \
    $(TOP)/frameworks/native/include/media/hardware                 \
    $(TOP)/external/tremolo/Tremolo                                 \
    libcore/include                                                 \

LOCAL_CFLAGS += -Werror -Wno-error=deprecated-declarations -Wall
LOCAL_CFLAGS += -DRTK_PLATFORM
LOCAL_CLANG := true
ifeq ($(USE_RT_MEDIA_PLAYER),true)
LOCAL_C_INCLUDES +=                                                 \
    $(TOP)/device/realtek/proprietary/libs/libRTMediaPlayer         \
    $(TOP)/device/realtek/proprietary/libs/Include  \
    $(TOP)/device/realtek/proprietary/libs/RT_IPC                   \
    $(TOP)/device/realtek/proprietary/libs/libRTDtvMediaPlayer      \
    $(TOP)/device/realtek/proprietary/libs/libaudioParser           \
    $(TOP)/device/realtek/proprietary/libs
endif

ifeq ($(ENABLE_RT_WIFIDISPLAY),true)
LOCAL_C_INCLUDES +=                                                 \
    bionic                                                          \
    external/stlport/stlport                                        \
    $(TOP)/frameworks/av/media/libmediaplayerservice/wfdisplay      \
    $(TOP)/frameworks/av/media/libmediaplayerservice/wfdisplay/spu  \
    $(TOP)/frameworks/av/media/libmediaplayerservice/wfdisplay/spu/core     \
    $(TOP)/frameworks/av/media/libmediaplayerservice/wfdisplay/spu/include
endif

LOCAL_MODULE:= libmediaplayerservice

LOCAL_32_BIT_ONLY := true

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
