/*
 * Copyright (c) 2015 Realtek Semiconductor Corp.
 */

#ifndef _FFMPEG_MP4_WRITER_H_
#define _FFMPEG_MP4_WRITER_H_

#include <media/stagefright/MediaWriter.h>

namespace android {

class MediaBuffer;
class MediaSource;
class MetaData;
class IMemory;
struct AHandler;

class FFmpegMP4Writer : public MediaWriter {
public:
    FFmpegMP4Writer(const char *filename);
    FFmpegMP4Writer(int fd);
    FFmpegMP4Writer(FILE *f_fd);
    FFmpegMP4Writer(void *cookie, ssize_t (*write)(void *cookie, const void *data, size_t size));
#if !defined(__LINARO_SDK__)
    FFmpegMP4Writer(sp<IMemory> data, sp<IMemory> info);
#endif

    virtual status_t addSource(const sp<MediaSource> &source);
    virtual bool reachedEOS();
    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop();
    virtual status_t pause();
    //virtual status_t dump(int fd, const Vector<String16>& args);

    void onMessageReceived(const sp<AMessage> &msg);
    int writePacket(uint8_t *buf, int buf_size); //must be public for callback

protected:
    virtual ~FFmpegMP4Writer();

private:
    class EsTrack : public AHandler {
    public:
        EsTrack(const sp<MediaSource> &source);
        void start(const sp<AMessage> &notify);
        void stop();
        bool isAudio() const;
        bool isEOSReached();
        int getTrackIndex();
        int64_t getStartTime();
        sp<MetaData> getFormat();
        sp<ABuffer> getCodecSpecificData();
        void readMore();

        enum {
            kNotifyStartFailed,
            kNotifyCodecSpecificData,
            kNotifyBuffer,
            kNotifyReachedEOS,
        };

    protected:
        virtual ~EsTrack();
        virtual void onMessageReceived(const sp<AMessage> &msg);

    private:
        void extractCodecSpecificData();

        enum {
            kWhatStart = 'strt',
            kWhatRead  = 'read',
        };

        sp<MediaSource> mSource;
        sp<ALooper> mLooper;
        sp<AMessage> mNotify;
        bool mIsAudio;
        bool mEOSReached;
        int mTrackIndex;
        int64_t mPrevTimeUs;
        int64_t mStartTime;
        sp<ABuffer> mCsd;

        DISALLOW_EVIL_CONSTRUCTORS(EsTrack);
    }; //end of class EsTrack

    enum {
        kWhatSourceNotify = 'noti',
    };

    void resetVariables();
    void init();
    status_t startMP4Muxer(sp<EsTrack> audioTrack, sp<EsTrack> videoTrack);
    status_t stopMP4Muxer();
    status_t writeFrameToMP4Muxer(int trackIndex, MediaBuffer *buffer);


    bool mStarted;          //indicate the state of FFmpegMP4Writer
    bool mMP4MuxerStarted;  //indicate the state of internal FFmpeg MP4 muxer
    Vector< sp<EsTrack> > mTracks;

    FILE *mFile;
    void *mWriteCookie;
    ssize_t (*mWriteFunc)(void *cookie, const void *data, size_t size);
#if !defined(__LINARO_SDK__)
    sp<IMemory> mBufferData;
    sp<IMemory> mBufferInfo;
#endif

    sp<ALooper> mLooper;
    sp<AHandlerReflector<FFmpegMP4Writer> > mReflector;

    void *mFormatContext;
    unsigned char *mAvioBuf;
    int64_t mStartTime;

    int mWidth;
    int mHeight;
    int mSampleRate;
    int mChannelCount;
};

}  // namespace android

#endif //_FFMPEG_MP4_WRITER_H_


