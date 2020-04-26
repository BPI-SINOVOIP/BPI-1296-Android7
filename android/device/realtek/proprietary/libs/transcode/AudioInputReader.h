/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _AUDIOINPUTREADER_H_
#define _AUDIOINPUTREADER_H_

#include <OSAL_RTK.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "AudioInbandAPI.h"
#include "RtkTranscodeCommon.h"
#include "AudioSourceFilter.h"
#include "media/stagefright/AudioSource.h"

// use AudioReader under raw mode
#include "AudioReader.h"

#define AIN_CHANNEL_NUM     2
#define DEC_CHANNEL_NUM     8
#define RINGBUFFER_SIZE     (64*1024)
#define NON_PCM_BUF_SIZE    (36 * 1024)
#define DEC_INBAND_SIZE     (512)
#define RING_SAMPLE_SIZE    (16 * 1024)
#define RING_BYTE_SIZE      (RING_SAMPLE_SIZE << 2)
#define MAX_AUDIO_SIZE      8192 //Audio encoder max buffer size is 8192
#define INSTANCE(inst, pin) (((inst) & 0xFFFFF000)|((pin) & 0xFFF))

#define PTS_CAPTURE_BUF     (10 * 1024)

namespace android {

struct ALooper;
struct AHandler;

typedef struct _PTS_RECORD {
    nsecs_t mRefPTS;
    uint32_t mWPStart;
    uint32_t mWPEnd;
    ssize_t mSize;
    uint64_t mIndex;
    //uint64_t mRawPTSVal;
} PTS_RECORD;

// new added struct for batch mode capture
typedef struct pts_info_2 {
    nsecs_t mRefPTS;
    ssize_t mLen;
    uint8_t mData[PTS_CAPTURE_BUF];
} pts_info_2_t;

class AudioInputReader : public AudioSourceFilter
{
public:
    AudioInputReader();

    enum {
        /* Self */
        kWhatCapture = 0x510,
    };

protected:
    virtual ~AudioInputReader();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        INITIALIZED,
        CONFIGURED,
        PREPARED,
        STARTED,
        STOPPED,
    };

    enum {
        kMaxBufferSize = 2048,
    };

    State mState;

    bool mRenderAudio;
    bool mEncodeAudio;

    long mAudioInId;
    long mAudioDecId;
    long mAudioPpId;
    long mAudioOutId;
    long mPpPinId;      //PP pin id
    long mAoutPinId;    //AO flash pin id

    int mIonFd;
    ion_user_handle_t mRefClkHdl;

    OSAL_RTKALLOCATOR *mCaptureAllocator;
    OSAL_RTKALLOCATOR *mAIInAllocator[AIN_CHANNEL_NUM];
    OSAL_RTKALLOCATOR *mDecInAllocator;
    OSAL_RTKALLOCATOR *mDecIcqAllocator;
    OSAL_RTKALLOCATOR *mDecOutAllocator[DEC_CHANNEL_NUM];

    // added for pts
    OSAL_RTKALLOCATOR *mPTSAllocator;
    uint32_t mLastWp;
    uint32_t mPCMCapturRingBufpAddr;
    uint32_t mPCMCaptureRingBufSize;
    // for accessing hw 90k
    int32_t rbus_fd;
    char*   rbus_mmap_base;

    bool    mRxReady;
    bool    mRxLPCM;
    int32_t mSrcSampleRate;
    int32_t mChannelCount;
    int32_t mSampleRate;
    int32_t mSampleSize;
    int32_t mAudioSource;
    bool mAudioRedirect;
    bool mRawAudio;
    int32_t mPTSAvail;

    sp<AudioSource> mRxSource;
    String16 mOpPackageName;
    int64_t mCaptureStartTimeUs;
    char *mPrevBuf[4096];
    FILE *mFd;
    uint64_t mCapturePtsIdx;
    Vector<PTS_RECORD *> mPTSRecQueue;

    // service under RAW mode record
    sp<RtkFilter> mAudioReader;
    sp<ALooper> mAudioReaderLooper;

    bool            mNotifyAudioHotplug;

    status_t createAudioInstance(uint32_t instanceType, long *intanceId);
    status_t destroyAudioInstance(long instanceId);
    status_t runAudioInstance(long instanceId);
    status_t pauseAudioInstance(long instanceId);
    status_t stopAudioInstance(long instanceId);
    status_t getAOInstance();
    status_t getAOPinId();
    status_t releaseAOPinId(long instanceId, long pinId);
    status_t getPPPinId();
    status_t releasePPPinId(long instanceId, long pinId);
    status_t initPCMRingBuffer();
    status_t setAIRxSource(long instanceId);
    status_t uninitPCMRingBuffer();
    status_t initRAWRingBuffer();
    status_t uninitRAWRingBuffer();
    status_t initRefClock();
    status_t uninitRefClock();
    status_t configMaxVolumeForTesting();
    status_t configRAWInput();
    status_t switchPpFocus(long instanceId, long pinId);
    status_t configADC();
    status_t configAiCapture();
    status_t flushCaptureBuffer();

    status_t startCapture();
    status_t startCapturePCM();
    status_t startCaptureRAW();

    status_t stopCapture();
    status_t stopCapturePCM();
    status_t stopCaptureRAW();

    status_t captureAudio();
    status_t startAudioRender(long instanceId);
    status_t stopAudioRender(long instanceId);
    status_t enableAudioInput();
    status_t disableAudioInput();
    status_t setupAudioRedirect();
    status_t stopAudioRedirect();
    status_t retrieveHdmiRxAduioInfo();

    // new API -> get PTS info by myself
    status_t initPTSRingBuffer();
    status_t flushPTSBuffer();
    status_t uninitPTSRingBuffer();
    status_t capturePTS(AUDIO_DEC_PTS_INFO *pInfo);
    status_t captureAudioBase(uint32_t reqSize,
            uint8_t *dstBuf,
            uint32_t *readSize);

    status_t captureAudio3();
    status_t captureAudio3Batch();
    nsecs_t convertPTS(AUDIO_DEC_PTS_INFO *pInfo);
    int64_t getHW90k();
    int pli_init();
    int pli_close();
    int* getIOAddress(unsigned int addr);
    status_t muteInputInstance();
    status_t unmuteInputInstance();
    void clearPTSRecordQueue();

    status_t setPlayback(int renderAudio);
    status_t setEncode(int encodeAudio);
    status_t start();
    status_t stop();
    status_t prepare();

    status_t initReaderHelper();
    status_t uninitReaderHelper();

    status_t notifyHDMIRxAudioHotplug(int32_t state);


    DISALLOW_EVIL_CONSTRUCTORS(AudioInputReader);
};

} // namespace android

#endif //_AUDIOINPUTREADER_H_
