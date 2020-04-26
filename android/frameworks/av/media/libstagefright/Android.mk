LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)


ifeq ($(USE_RT_ION),true)
LOCAL_CFLAGS += -DUSE_RT_ION
endif
LOCAL_CFLAGS += -DCHANGE_ALLOC_BUFFER_LOCATION
LOCAL_CFLAGS += -DRTK_AUDIO_EXTRACTOR_EN
LOCAL_CFLAGS += -DSUPPORT_OMX_VIDEO_LOW_DELAY_MODE
LOCAL_CFLAGS += -DSUPPORT_OMX_VIDEO_SEARCH_I_FRAME
LOCAL_CFLAGS += -DMS12_FLOW_EN
LOCAL_CFLAGS += -DRTK_PLATFORM
# For normal case using adpative playback, notified video component and do not reallocate output buffer
LOCAL_CFLAGS += -DSUPPORT_ADPATIVEPLAYBACK_MODE


LOCAL_SRC_FILES:=                         \
        ACodec.cpp                        \
        AACExtractor.cpp                  \
        AACWriter.cpp                     \
        AMRExtractor.cpp                  \
        AMRWriter.cpp                     \
        AudioPlayer.cpp                   \
        AudioSource.cpp                   \
        CallbackDataSource.cpp            \
        CameraSource.cpp                  \
        CameraSourceTimeLapse.cpp         \
        CodecBase.cpp                     \
        DataConverter.cpp                 \
        DataSource.cpp                    \
        DataURISource.cpp                 \
        DRMExtractor.cpp                  \
        ESDS.cpp                          \
        FileSource.cpp                    \
        FLACExtractor.cpp                 \
        FrameRenderTracker.cpp            \
        HTTPBase.cpp                      \
        HevcUtils.cpp                     \
        JPEGSource.cpp                    \
        MP3Extractor.cpp                  \
        MPEG2TSWriter.cpp                 \
        MPEG4Extractor.cpp                \
        MPEG4Writer.cpp                   \
        MediaAdapter.cpp                  \
        MediaClock.cpp                    \
        MediaCodec.cpp                    \
        MediaCodecList.cpp                \
        MediaCodecListOverrides.cpp       \
        MediaCodecSource.cpp              \
        MediaDefs.cpp                     \
        MediaExtractor.cpp                \
        MediaSync.cpp                     \
        MidiExtractor.cpp                 \
        http/MediaHTTP.cpp                \
        MediaMuxer.cpp                    \
        MediaSource.cpp                   \
        NuCachedSource2.cpp               \
        NuMediaExtractor.cpp              \
        OMXClient.cpp                     \
        OggExtractor.cpp                  \
        ProcessInfo.cpp                   \
        SampleIterator.cpp                \
        SampleTable.cpp                   \
        SimpleDecodingSource.cpp          \
        SkipCutBuffer.cpp                 \
        StagefrightMediaScanner.cpp       \
        StagefrightMetadataRetriever.cpp  \
        SurfaceMediaSource.cpp            \
        SurfaceUtils.cpp                  \
        ThrottledSource.cpp               \
        Utils.cpp                         \
        VBRISeeker.cpp                    \
        VideoFrameScheduler.cpp           \
        WAVExtractor.cpp                  \
        WVMExtractor.cpp                  \
        XINGSeeker.cpp                    \
        avc_utils.cpp                     \
        hevc_utils.cpp                    \
        RTK/RTKAudioExtractor.cpp         \
        RTK/RTKAudioALAC.cpp              \
        AVIExtractor.cpp                  \
        RTKASFExtractor.cpp               \
        RTKFLVExtractor.cpp               \
        RTKAIFFExtractor.cpp               \
        RTKDSDExtractor.cpp               \
        RTKAPEExtractor.cpp               \
        RTKHEIFExtractor.cpp



LOCAL_C_INCLUDES:= \
        $(TOP)/frameworks/av/include/media/ \
        $(TOP)/frameworks/av/include/media/stagefright/timedtext \
        $(TOP)/frameworks/native/include/media/hardware \
        $(TOP)/frameworks/native/include/media/openmax \
        $(TOP)/external/flac/include \
        $(TOP)/external/tremolo \
        $(TOP)/hardware/libhardware/include \
        $(TOP)/device/realtek/proprietary/libs/Include \
        $(TOP)/external/libvpx/libwebm \
        $(TOP)/system/netd/include \
        $(call include-path-for, audio-utils)

ifeq ($(ENABLE_TEE_DRM_FLOW),true)
LOCAL_CFLAGS += -DENABLE_TEE_DRM_FLOW
LOCAL_C_INCLUDES += $(TOP)/system/core/include
endif

LOCAL_SHARED_LIBRARIES := \
        libaudioutils \
        libbinder \
        libcamera_client \
        libcutils \
        libdl \
        libdrmframework \
        libexpat \
        libgui \
        libicui18n \
        libicuuc \
        liblog \
        libmedia \
        libmediautils \
        libnetd_client \
        libopus \
        libsonivox \
        libssl \
        libstagefright_omx \
        libstagefright_yuv \
        libsync \
        libui \
        libutils \
        libvorbisidec \
        libz \
        libpowermanager \
		libion \
		libhardware \

LOCAL_STATIC_LIBRARIES := \
        libstagefright_color_conversion \
        libyuv_static \
        libstagefright_aacenc \
        libstagefright_matroska \
        libstagefright_mediafilter \
        libstagefright_webm \
        libstagefright_timedtext \
        libvpx \
        libwebm \
        libstagefright_mpeg2ts \
        libstagefright_id3 \
        libFLAC \
        libmedia_helper \

ifeq ($(ENABLE_FFMPEG),true)
LOCAL_SRC_FILES += FFmpegMP4Writer.cpp
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES += $(TOP)/external/ffmpeg
LOCAL_C_INCLUDES += $(TOP)/external/ffmpeg/android/include
LOCAL_SHARED_LIBRARIES += libavformat
LOCAL_SHARED_LIBRARIES += libavcodec
LOCAL_SHARED_LIBRARIES += libavutil
endif

ifeq ($(USE_RTK_EXTRACTOR),true)
LOCAL_CFLAGS += -DUSE_RTK_EXTRACTOR
LOCAL_SRC_FILES += RTKExtractor.cpp
endif

#LOCAL_SRC_FILES += \
#        chromium_http_stub.cpp
#LOCAL_CPPFLAGS += -DCHROMIUM_AVAILABLE=1

LOCAL_SHARED_LIBRARIES += \
        libstagefright_enc_common \
        libstagefright_avc_common \
        libstagefright_foundation \
        libdl \
        libRScpp \

LOCAL_CFLAGS += -Wno-multichar -Werror -Wno-error=deprecated-declarations -Wall

# enable experiments only in userdebug and eng builds
ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DENABLE_STAGEFRIGHT_EXPERIMENTS
endif

LOCAL_CLANG := true
LOCAL_SANITIZE := unsigned-integer-overflow signed-integer-overflow

LOCAL_CFLAGS += -Wno-multichar
LOCAL_CFLAGS += -DMEDIA_SCAN_ALONE

LOCAL_MODULE:= libstagefright

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
