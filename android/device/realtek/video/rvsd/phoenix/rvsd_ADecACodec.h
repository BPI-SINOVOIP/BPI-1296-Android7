#ifndef __RVSD_ADEC_ACODEC_H__
#define __RVSD_ADEC_ACODEC_H__

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/IMediaPlayerService.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AMessage.h>

#include "include/avc_utils.h"

#include <media/stagefright/ACodec.h>
#include <media/stagefright/AudioPlayer.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/OMXClient.h>
#include <media/mediametadataretriever.h>
#ifndef ANDROID_6
#include <media/stagefright/NativeWindowWrapper.h>
#endif

#include <media/stagefright/MPEG2TSWriter.h>
#include <media/stagefright/MPEG4Writer.h>

#include <media/AudioTrack.h>
#include <system/audio.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MediaBufferGroup.h>

#include <include/ESDS.h>

#include <utils/RefBase.h>

#include "rvsd_vdec.h"
#include "rvsd_InBandAPI.h"
#include "pthread-macro.h"
#include "rvsd_OSAL.h"

class RVSD_INFO;

namespace android {
void *ADec_ACodec_pthread(void *data);

struct AORender : public AHandler {
    AORender(RVSD_INFO* pRvsd_info, const sp<AudioTrack> &audioTrack);
    void startAsync();
    void stopAsync();
    void SetStopStatus(ENUM_SERVICE_STATUS arg) {
        mStop = arg;
    }
    ENUM_SERVICE_STATUS GetStopStatus() {
        return mStop;
    }

    enum {
        kWhatEOS				 = 'eos ',
        kWhatFlushComplete		 = 'fluC',
        kWhatPosition			 = 'posi',
        kWhatVideoRenderingStart = 'vdrd',
    };

protected:
    virtual ~AORender();
    virtual void onMessageReceived(const sp<AMessage> &msg);
private:
    enum {
        kWhatStart             = 'strt',
        kWhatStop              = 'stop',
        kWhatCodecNotify       = 'noti',
        kWhatSeek              = 'seek',
        kWhatAudioNotify       = 'audr',
        kWhatRendererNotify    = 'renN',
        kWhatRenderAOBuffer    = 'raob',
        kWhatQueueAO	       = 'qaoN',
        kWhatDrainAudioQueue   = 'draA',
        kWhatQueueBuffer       = 'queB',

    };

    struct QueueEntry {
        sp<ABuffer> mBuffer;
        sp<AMessage> mNotifyConsumed;
        size_t mOffset;
        status_t mFinalResult;
    };

    sp<AudioTrack> mAudioTrack;
    sp<AMessage> mNotify;
    List<QueueEntry> mAudioQueue;

    enum SeekState {
        SEEK_NONE,
        SEEK_FLUSHING,
        SEEK_FLUSH_COMPLETED,
    };
    SeekState mSeekState;
    int64_t mSeekTimeUs;

    uint32_t mNumFramesWritten;
    bool mDrainAudioQueuePending;
    int32_t mAudioQueueGeneration;

    int64_t mAnchorTimeMediaUs;
    int64_t mAnchorTimeRealUs;

    void postDrainAudioQueue(int64_t delayUs);
    bool onDrainAudioQueue();
    void notifyEOS(bool audio, status_t finalResult);

    void queueBuffer(
        const sp<ABuffer> &buffer,
        const sp<AMessage> &notifyConsumed);

    int64_t aRCD;
    void onFillThisBuffer(const sp<AMessage> &msg);
    ENUM_SERVICE_STATUS mStop;

    RVSD_INFO* pRvsdInfo;
};

struct ADecController : public AHandler {
#ifdef ANDROID_6
    ADecController(RVSD_INFO* pRvsd_info, const sp<AudioTrack> &audioTrack, const sp<MediaSource> &source, sp<AHandler> AOid);
#else
    ADecController(RVSD_INFO* pRvsd_info, const sp<AudioTrack> &audioTrack, const sp<MediaSource> &source, ALooper::handler_id AOid);
#endif
    void startAsync();
    void stopAsync();
    void signalFlush();
    void SetStopStatus(ENUM_SERVICE_STATUS arg) {
        mStop = arg;
    }
    ENUM_SERVICE_STATUS GetStopStatus() {
        return mStop;
    }

protected:
    virtual ~ADecController();
    virtual void onMessageReceived(const sp<AMessage> &msg);
private:
    enum {
        kWhatStart             = 'strt',
        kWhatStop              = 'stop',
        kWhatCodecNotify       = 'noti',
        kWhatSeek              = 'seek',
        kWhatQueueBuffer       = 'queB',
        kWhatAudioNotify	   = 'audr',
        kWhatRendererNotify    = 'renN',
        kWhatQueueAO		   = 'qaoN',
        kWhatEOS               = 'eos ',
    };

    struct QueueEntry {
        sp<ABuffer> mBuffer;
        sp<AMessage> mNotifyConsumed;
        size_t mOffset;
        status_t mFinalResult;
    };

    sp<ALooper> mADecodeLooper;
    sp<MediaSource> mSource;
    sp<AudioTrack> mAudioTrack;
    int32_t nNumFramesWritten;
    sp<ACodec> mCodec;
    sp<AMessage> mNotify;
    bool mIsVorbis;
#ifdef ANDROID_6
    sp<AHandler> mAOid;
#else
    ALooper::handler_id mAOid;
#endif
    Vector<sp<ABuffer> > mCSD;
    size_t mCSDIndex;

    MediaBuffer *mLeftOverBuffer;
    status_t mFinalResult;

    int64_t mStartTimeUs;
    int32_t mNumOutputBuffersReceived;
    int64_t mTotalBytesReceived;

    enum SeekState {
        SEEK_NONE,
        SEEK_FLUSHING,
        SEEK_FLUSH_COMPLETED,
    };
    SeekState mSeekState;
    int64_t mSeekTimeUs;

    sp<AMessage> makeFormat(const sp<MetaData> &meta);
    void onFillThisBuffer(const sp<AMessage> &msg);
    ENUM_SERVICE_STATUS mStop;

    RVSD_INFO* pRvsdInfo;
};

class ACodecAudioSource : public MediaSource {
public:
    int isEOS;
    int isReading;

    ACodecAudioSource(RVSD_INFO* pRvsd_info,int sampleRate, int channels, int bufsize , int audioType);
    virtual sp<MetaData> getFormat();
    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual status_t read(
        MediaBuffer **buffer, const MediaSource::ReadOptions *options);

protected:
    virtual ~ACodecAudioSource() {pRvsdInfo = NULL;}

private:
    MediaBufferGroup mGroup;
    int mSampleRate;
    int mChannels;
    int mAudioType;
    size_t mSize;

//	sp<AudioTrack> mAudioTrack;

    ACodecAudioSource(const ACodecAudioSource &);
    ACodecAudioSource &operator=(const ACodecAudioSource &);

    RVSD_INFO* pRvsdInfo;
};

class ADecACodec : public RefBase {
public:
    ADecACodec(RVSD_INFO* pRvsd_info,pthread_t *pThreadAudio);
    void ADecInit(void);
    void ADecUninit(void);
    void * ADec_decode_loop(void);

//protected:
    virtual ~ADecACodec();
protected:

private:
    sp<MediaSource> mediaSource;
    sp<MediaSource> mAudioSource;
    sp<AudioTrack> mAudioTrack;

    int mSignalFlush;

    sp<ALooper> mADecLooper;
    sp<ADecController> mADecController;
    sp<ALooper> mAORenderLooper;
    sp<AORender> mAORender;

    RVSD_INFO* pRvsdInfo;
};

}  // namespace android

#endif
