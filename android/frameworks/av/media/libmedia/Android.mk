LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    AudioParameter.cpp
LOCAL_MODULE:= libmedia_helper
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Werror -Wno-error=deprecated-declarations -Wall
LOCAL_CLANG := true

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    AudioTrack.cpp \
    AudioTrackShared.cpp \
    IAudioFlinger.cpp \
    IAudioFlingerClient.cpp \
    IAudioTrack.cpp \
    IAudioRecord.cpp \
    ICrypto.cpp \
    IDataSource.cpp \
    IDrm.cpp \
    IDrmClient.cpp \
    IHDCP.cpp \
    AudioRecord.cpp \
    AudioSystem.cpp \
    mediaplayer.cpp \
    IMediaCodecList.cpp \
    IMediaCodecService.cpp \
    IMediaDrmService.cpp \
    IMediaHTTPConnection.cpp \
    IMediaHTTPService.cpp \
    IMediaLogService.cpp \
    IMediaExtractor.cpp           \
    IMediaExtractorService.cpp \
    IMediaPlayerService.cpp \
    IMediaPlayerClient.cpp \
    IMediaRecorderClient.cpp \
    IMediaPlayer.cpp \
    IMediaRecorder.cpp \
    IMediaSource.cpp \
    IRemoteDisplay.cpp \
    IRemoteDisplayClient.cpp \
    IResourceManagerClient.cpp \
    IResourceManagerService.cpp \
    IStreamSource.cpp \
    MediaCodecInfo.cpp \
    MediaUtils.cpp \
    Metadata.cpp \
    mediarecorder.cpp \
    IMediaMetadataRetriever.cpp \
    mediametadataretriever.cpp \
    MidiIoWrapper.cpp \
    ToneGenerator.cpp \
    JetPlayer.cpp \
    IOMX.cpp \
    IAudioPolicyService.cpp \
    IAudioPolicyServiceClient.cpp \
    MediaScanner.cpp \
    MediaScannerClient.cpp \
    CharacterEncodingDetector.cpp \
    IMediaDeathNotifier.cpp \
    MediaProfiles.cpp \
    MediaResource.cpp \
    MediaResourcePolicy.cpp \
    IEffect.cpp \
    IEffectClient.cpp \
    AudioEffect.cpp \
    Visualizer.cpp \
    MemoryLeakTrackUtil.cpp \
    StringArray.cpp \
    AudioPolicy.cpp

ifeq ($(ENABLE_TEE_DRM_FLOW),true)
LOCAL_CFLAGS += -DENABLE_TEE_DRM_FLOW
endif

LOCAL_SHARED_LIBRARIES := \
	libui liblog libcutils libutils libbinder libsonivox libicuuc libicui18n libexpat \
        libcamera_client libstagefright_foundation \
        libgui libdl libaudioutils libnbaio

ifeq ($(USE_RT_MEDIA_PLAYER),true)
LOCAL_CFLAGS += -DUSE_RT_MEDIA_PLAYER
endif

LOCAL_WHOLE_STATIC_LIBRARIES := libmedia_helper

# for memory heap analysis
LOCAL_STATIC_LIBRARIES := libc_malloc_debug_backtrace libc_logging

LOCAL_MODULE:= libmedia

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_C_INCLUDES := \
    $(TOP)/frameworks/native/include/media/openmax \
    external/icu4c/common \
    $(TOP)/frameworks/av/include/media/ \
    $(TOP)/frameworks/av/media/libstagefright \
    $(call include-path-for, audio-effects) \
    $(call include-path-for, audio-utils)

LOCAL_CFLAGS += -Werror -Wno-error=deprecated-declarations -Wall
LOCAL_CLANG := true
LOCAL_SANITIZE := unsigned-integer-overflow signed-integer-overflow

LOCAL_C_INCLUDES += $(TOP)/device/realtek/proprietary/libs

include $(BUILD_SHARED_LIBRARY)

