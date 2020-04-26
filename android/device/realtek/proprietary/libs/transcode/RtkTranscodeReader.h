/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */

#ifndef __RTKTRANSCODEREADER_H__
#define __RTKTRANSCODEREADER_H__

#include <binder/IMemory.h>

#include "RtkTranscodeCommon.h"

namespace android {

struct ABuffer;
struct ALooper;
struct AHandler;
struct IMediaSource;
class IMediaExtractor;
class MetaData;

struct RtkFilter;

struct RtkTranscodeReader : public RtkFilter
{
public:
    RtkTranscodeReader();
    bool EOSReached();

    enum {
        /* Self */

        /* Player --> Reader */
        kWhatSetDataSourceByPath = 0x500,
        kWhatSetDataSourceByFileHandle,
        kWhatStartSource,
        kWhatStop = WHAT_STOP,

        /* Decoder --> Reader */
        kWhatFillBuffer,        //notify output data arrives
        kWhatGetMeta,           // Decoder request Meta 
    };

protected:
    virtual ~RtkTranscodeReader();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        CONFIGURED,
        PREPARED,
        STARTED,
        STOPPED,
    };
    
    enum {
        FILE_PATH,
        FILE_HANDLE,
    };

    State mState;
    bool mEOSReached;
    bool mDebug;
    size_t mTotalSourceDone;

    AString mInputFile;
    int32_t mFileId;

    sp<IMediaExtractor> mExtractor;

    struct ReaderSource
    {
        int32_t type;   // CODEC_AUDIO or CODEC_VIDEO
        sp<IMediaSource> mSource;
        sp<AMessage> mFormat; /* For ACodec from extractor */
        sp<MetaData> mMetaData;
        Vector< sp<ABuffer> > mCSD; /* For First decode, need SPS and PPS */
        size_t mCSDIndex;  
        bool mSourceDone; /* For check MediaSource status */
    };

    Vector< ReaderSource *> mSourceList;

    void resetVariables();
    void onSetupDataSource(const sp<AMessage> &msg, int32_t FileSourceType);
    void onFillThisBuffer(const sp<AMessage> &msg);
    status_t onStartSource();

    DISALLOW_EVIL_CONSTRUCTORS(RtkTranscodeReader);
};

} // namespace android

#endif //__RTKTRANSCODEREADER_H__
