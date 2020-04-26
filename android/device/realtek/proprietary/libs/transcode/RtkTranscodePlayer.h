/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _RTK_TRANSCODE_PLAYER_H_
#define _RTK_TRANSCODE_PLAYER_H_

#include <media/mediarecorder.h>
#include "RtkTranscodeCommon.h"
#include "RtkIpc/IPlayerListener.h"

#define BUFFER_LEN  4096

namespace android {

struct ABuffer;
struct ALooper;
struct AHandler;
struct IGraphicBufferProducer;
struct NativeWindowWrapper;
struct RtkTranscodeDecode;
struct RtkTranscodeReader;
struct RtkTranscodeEncode;
struct RtkTranscodeWriter;
struct RtkTranscodePostProcessor;
struct RtkTranscodeMP4Writer;
class MediaExtractor;
class MetaData;
class ICamera;

struct RtkFilter;
class AudioReader;

typedef void(*STATUS_CALLBACK)(sp<AMessage> &msg);

class RtkTranscodePlayer : public AHandler
{
public:
    RtkTranscodePlayer(bool createLooper = false, int32_t onlyTrack = 0);
    void registerStatusHandler(sp<AHandler> id) { mStatusId = id; };
    void registerStatusCallback(STATUS_CALLBACK callback) { mStatusCb = callback; };

    status_t initiate(int transcodeType = TranscodeTypeFile, output_format outputFormat = OUTPUT_FORMAT_MPEG2TS); //outputFormat is already obsolete, use setOutputFormat() instead
    status_t setDataSource(const char *path);
    status_t setDataSource(int32_t fileId, int64_t offset, int64_t length);
    status_t setCameraSource(int cameraId, String16 &clientName);
    status_t setCameraSource(/*int cameraId, String16 &clientName,*/ const sp<::android::hardware::ICamera> &camera);
    status_t setAudioSource(int audioSource);
    status_t setSurface(const sp<IGraphicBufferProducer> &bufferProducer);
    status_t setOutputFormat(RtkTranscodeConf &conf);
    status_t start();
    status_t stop();
    bool EOSReached();
    status_t setPlayback(bool video, bool audio);
    status_t setTranscode(bool transcode);
    status_t setDebug(bool controlDebug);
    status_t setDecode();
    bool getTranscodeEnabled() { return mTranscode; };
    status_t getCameraSnapshot(CAMERA_SNAPSHOT *snapshot);
    String8 getCameraParameters();
    status_t configureRxPreviewResolution(int32_t width, int32_t height);

    //for DvdPlayer
    sp<AHandler> getDvdPlayerAgentID();
    status_t setAudioDecoderAgentID(const uint32_t id);
    status_t configureTargetFormat(const AV_FORMAT_CONFIG &config);

    // for reset record format
    status_t resetTargetFormat(const AV_FORMAT_CONFIG &config);

    // for service mode.
    void setListener(sp<IPlayerListener> &l) { mListener = l; };
    void cleanup();

    status_t notifyHDMIRxAudioHotplug(int32_t state);

    enum {
        /* App --> Player */
        kWhatInitiate = 0x100,
        kWhatSetDataSourceByPath,
        kWhatSetDataSourceByFileHandle,
        kWhatSetPlayback,
        kWhatSetTranscodeStart,
        kWhatSetDecode,
        kWhatSetDebug,
        kWhatSetSurface = WHAT_SET_SURFACE,
        kWhatStop = WHAT_STOP,

        /* Decoder --> Player */
        kWhatStopDecodeDone = 0x110,
        kWhatDecodeGotEOS,

        /* Encoder --> Player */
        kWhatSetTranscodeDone = 0x120,
        kWhatStopTranscodeDone,

        /* CameraReader --> Player */
        kWhatCameraError = 0x130,
    };

    enum {
        DuringSetupTranscodeSetting = 1 << 0,
        DuringStopTranscodeSetting = 1 << 1,
        DuringHandleDecoder = 1 << 2,
        DuringHandleEncoder = 1 << 3,
    };

    // move the enum to RtkTranscodeCommon.h
#if 0
    enum {
        TranscodeTypeFile = 0,
        TranscodeTypeDvdPlayer,
        TranscodeTypeHdmiRx,
        TranscodeTypeUsbCamera,
        TranscodeTypeMax,
    };
#endif

    // TODO: review me later, not sure if it cause memory leak.
    sp<IMemory> mBuffer;

#define CHECK_TRANSCODE_STATUS_CHANGE       (mTranscodeChangeState & (DuringSetupTranscodeSetting | DuringStopTranscodeSetting))
#define CHECK_CHANGE_TRANSCODE_DONE         (!(mTranscodeChangeState & (DuringHandleEncoder | DuringHandleDecoder)))
#define START_CHANGE_ENCODER_TRANSCODE      (mTranscodeChangeState |= DuringHandleEncoder)
#define START_CHANGE_DECODER_TRANSCODE      (mTranscodeChangeState |= DuringHandleDecoder)
#define START_SETUP_PLAYER_TRANSCODE        (mTranscodeChangeState |= DuringSetupTranscodeSetting)
#define START_STOP_PLAYER_TRANSCODE         (mTranscodeChangeState |= DuringStopTranscodeSetting)
#define DONE_CHANGE_ENCODER_TRANSCODE       (mTranscodeChangeState &= ~DuringHandleEncoder)
#define DONE_CHANGE_DECODER_TRANSCODE       (mTranscodeChangeState &= ~DuringHandleDecoder)

#define DONE_SETUP_PLAYER_TRANSCODE         (mTranscodeChangeState &= ~DuringSetupTranscodeSetting)
#define DONE_STOP_PLAYER_TRANSCODE          (mTranscodeChangeState &= ~DuringStopTranscodeSetting)

#define OP(x)                          ((x & DuringSetupTranscodeSetting) ? "Setup" : "Stop")

protected:
    virtual ~RtkTranscodePlayer();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:

    enum State {
        UNINITIALIZED,
        INITIALIZED,
        WAIT_SURFACE,
        PREPARED,
        STARTED,
        STOPPING,
    };

    bool mEOSReached;
    bool mTranscode;
    bool mRenderVideo;
    bool mRenderAudio;
    bool mDebug;
    uint32_t mTranscodeChangeState;
    sp<AReplyToken> mTranscodeChangeReplyId;

    State mState;
    int32_t mTranscodeType;
    //int32_t mOutputFormat;
    AString mInputFile;
    int32_t mFileId;
    RtkTranscodeConf *mTranscodeConfig;
    const AV_FORMAT_CONFIG *mTargetFormat;
    uint32_t mAudioDecoderAgentId;
    String16 mClientName;

    sp<AHandler> mStatusId;
    STATUS_CALLBACK mStatusCb;
    int32_t mOnlyTrack;

    bool mHasPlayerLooper;
    sp<ALooper> mPlayerLooper;

    sp<RtkFilter> mWriter;
    sp<ALooper> mWriterLooper;

    sp<RtkFilter> mEncode;
    sp<ALooper> mEncoderLooper;

    sp<RtkFilter> mReader;
    sp<ALooper> mReaderLooper;

    sp<RtkFilter> mDecode;
    sp<ALooper> mDecoderLooper;

    sp<RtkFilter> mPostProcessor;
    sp<ALooper> mPostProcessorLooper;

    sp<RtkFilter> mAudioReader;
    sp<ALooper> mAudioReaderLooper;

    /* For create Decoder */
    sp<AMessage> mVideoMsg;
    sp<AMessage> mAudioMsg;
    int32_t mVideoTrackIndex;
    int32_t mAudioTrackIndex;
    /* For create Encoder */
    sp<MetaData> mVideoMetaData;
    sp<MetaData> mAudioMetaData;

    int64_t mStartTimeRealUs;

    status_t onInitiate();
    status_t onStart();
    status_t onStop();
    void stopDecode();

    status_t setupTranscoding();
    void stopTranscoding();
    status_t createEncodeMsg(bool isVideo, const sp<MetaData> srcMeta, sp<AMessage> *format);
    void resetVariables();

    sp<IPlayerListener> mListener;

    // fix binder call problem.
    RtkTranscodeConf mTConfig;
    AV_FORMAT_CONFIG mAVConfig;

    char mFileName[BUFFER_LEN];
    char mUdpIp[BUFFER_LEN];

    int mFd;

    void closeFd();

    DISALLOW_EVIL_CONSTRUCTORS(RtkTranscodePlayer);
};

} // namespace android
#endif /* end of not define _RTK_TRANSCODE_PLAYER_H_ */
