#define LOG_TAG "RTKAudioExt"
#include <utils/Log.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <utils/String8.h>

#include "RTKAudioALAC.h"
#include "RTKAudioExtractor.h"

namespace android {

#ifdef TRACE_EN
#define TRACE_PRINT(fmt, args...)   ALOGD("@ %d %s %d:"fmt,gettid(), __FUNCTION__, __LINE__, ##args);
#else
#define TRACE_PRINT(fmt, args...)   do{ }while(0)
#endif

RTKAudioExtractor::~RTKAudioExtractor() 
{
    TRACE_PRINT("\n");
    if(mimeType == MEDIA_MIMETYPE_AUDIO_ALAC_CAFF)
    {
        delete mALACInfo;
    }
}

RTKAudioExtractor::RTKAudioExtractor(const sp<DataSource> &source)
    : mDataSource(source),
      mInitCheck(NO_INIT)
{
    TRACE_PRINT("[+]\n");
    float confidence;
    int64_t duration;   // unit: ms
    if (!SniffRTKAudio(mDataSource, &mimeType, &confidence, NULL)) {
        TRACE_PRINT("[-] failed\n");
        return;
    }

    if(mimeType == MEDIA_MIMETYPE_AUDIO_ALAC_CAFF)
    {
        mALACInfo = new RTK_ALAC(mDataSource);
        mALACInfo->ALAC_init();

        mMeta = new MetaData;
        mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_ALAC_CAFF);
        mMeta->setInt32(kKeyChannelCount,mALACInfo->mConfig.numChannels);
        mMeta->setInt32(kKeySampleRate, mALACInfo->mConfig.sampleRate);
        mMeta->setInt32(kKeyBitRate, mALACInfo->mConfig.avgBitRate);
        mMeta->setPointer(kKeyALAC_CAFFConfig, &mALACInfo->mConfig);

        duration = mALACInfo->EstimateDuration();
        mMeta->setInt64(kKeyDuration, duration);

        TRACE_PRINT("[ALAC ch %d sr %d dur %lld]\n", mALACInfo->mConfig.numChannels
            , mALACInfo->mConfig.sampleRate, duration);

        mInitCheck = OK;
    }
    TRACE_PRINT("[-] ok\n");
}

size_t RTKAudioExtractor::countTracks() 
{
    return mInitCheck == OK ? 1 : 0;
}

sp<MetaData> RTKAudioExtractor::getMetaData() 
{
    sp<MetaData> meta = new MetaData;

    if (mInitCheck != OK) {
        return meta;
    }

    if(mimeType == MEDIA_MIMETYPE_AUDIO_ALAC_CAFF)
        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_ALAC_CAFF);

    return meta;
}

sp<IMediaSource> RTKAudioExtractor::getTrack(size_t index) 
{
    TRACE_PRINT(" # \n");
    if (mInitCheck != OK || index != 0) {
        return NULL;
    }

    return new ALACSource(mDataSource, mMeta,
        mALACInfo->mOffsetTable, mALACInfo->mPktSizeOffsetTable, mALACInfo->mOffsetTableLength, (off64_t)mALACInfo->mInputDataPos, (off64_t)mALACInfo->mPacketTablePos, mALACInfo->us_per_frame);
}

sp<MetaData> RTKAudioExtractor::getTrackMetaData(size_t index, uint32_t /*flags*/)
{
    if (mInitCheck != OK || index != 0) {
        return NULL;
    }

    return mMeta;
}

bool SniffRTKAudio(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) 
{
    TRACE_PRINT("[+]\n");
    char header[16];

    if (source->readAt(0, header, sizeof(header)) != sizeof(header)) {
        TRACE_PRINT("[-] %f\n", *confidence);
        return false;
    }

    if (!memcmp(header, RTK_ALAC_HEADER, RTK_ALAC_HEADER_LEN)) {
        *mimeType = MEDIA_MIMETYPE_AUDIO_ALAC_CAFF;
        *confidence = 0.6;

        TRACE_PRINT("[-] find %f\n", *confidence);
        return true;
    }

    TRACE_PRINT("[-] %f\n", *confidence);
    return false;
}

}  // namespace android


