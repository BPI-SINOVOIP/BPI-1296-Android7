LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(USE_RT_MEDIA_PLAYER),true)
LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER
endif
LOCAL_CFLAGS += -DRTK_PLATFORM
#LOCAL_CFLAGS += -DRTK_NET_ASYNC_PLAYBACK_FLOW

LOCAL_SRC_FILES:=                       \
        GenericSource.cpp               \
        HTTPLiveSource.cpp              \
        NuPlayer.cpp                    \
        NuPlayerCCDecoder.cpp           \
        NuPlayerDecoder.cpp             \
        NuPlayerDecoderBase.cpp         \
        NuPlayerDecoderPassThrough.cpp  \
        NuPlayerDriver.cpp              \
        NuPlayerRenderer.cpp            \
        NuPlayerStreamListener.cpp      \
        RTSPSource.cpp                  \
        StreamingSource.cpp             \
        audio_table.cpp                 \
        video_table.cpp                 \

LOCAL_C_INCLUDES := \
	$(TOP)/frameworks/av/media/libstagefright                     \
	$(TOP)/frameworks/av/media/libstagefright/httplive            \
	$(TOP)/frameworks/av/media/libstagefright/include             \
	$(TOP)/frameworks/av/media/libstagefright/mpeg2ts             \
	$(TOP)/frameworks/av/media/libstagefright/rtsp                \
	$(TOP)/frameworks/av/media/libstagefright/timedtext           \
	$(TOP)/frameworks/av/media/libmediaplayerservice              \
	$(TOP)/frameworks/av/media/libstagefright		      \
	$(TOP)/frameworks/native/include/media/openmax        \
	$(TOP)/device/realtek/proprietary/libs

LOCAL_CFLAGS += -Werror -Wall

# enable experiments only in userdebug and eng builds
ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DENABLE_STAGEFRIGHT_EXPERIMENTS
endif

LOCAL_CLANG := true

LOCAL_MODULE:= libstagefright_nuplayer

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

