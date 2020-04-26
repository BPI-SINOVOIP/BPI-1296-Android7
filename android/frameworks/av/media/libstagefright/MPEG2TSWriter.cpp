/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "MPEG2TSWriter"
#if defined(__LINARO_SDK__)
#include "ADebug.h"
#include "ABuffer.h"
#include "AMessage.h"
#include "hexdump.h"
#else
#include <media/stagefright/foundation/ADebug.h>

#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/AMessage.h>
#endif
#include <media/stagefright/MPEG2TSWriter.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>
#include <utils/Statistics.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define CHECK_RINGBUFFER_OVERWRITE

#define MAX_DURATION_DIFF   500000000  //in ns, 0.5sec
//#define NO_SPS_PPS_WORKAROUND	/* Only for no sps and pps encoder Used */
#define USED_VIDEO_PCRPID
//#define ADD_PCR_IN_AF	/* VLC can't playback when open this defined, not finished */
//#define SUPPORT_VIDEO_BITRATE_CONTROL

#define CHECK_EQ_2(x,y) \
    do {    \
        if((ssize_t)(x) != (ssize_t)(y)){ \
            ALOGE("%s != %s strerr:%s",#x,#y,strerror(errno));    \
        }   \
    } while(0)

#define DEFAULT_PID	0x1e0	// PMT used
#define USED_ALONE_PCRPID
#ifdef USED_ALONE_PCRPID
#define kPID_PCR (DEFAULT_PID - 1)
#else /* else of USED_ALONE_PCRPID */
#define kPID_PCR (DEFAULT_PID + 1)
#endif /* end of USED_ALONE_PCRPID */

#include <RtkIpc/IRecordBuffer.h>
#ifdef WRITE_DATA_FROM_LARGE_BUFFER
#define MAX_WRITE2SOCKET_COUNTER	7	// 188 * 7 = 1316
#define MAX_WRITE2FILE_LENGTH		64000 
#define WRITE_MODE_FILE				0	// write to file
#define WRITE_MODE_SOCKET_UDP		1	// UDP only
#define WRITE_MODE_SOCKET_RTP		2	// RTP + UDP
#endif /* end of WRITE_DATA_FROM_LARGE_BUFFER */
#define MAX_WRITE2SOCKET_LENGTH		1500	// max MTU
#define RTP_OVER_ASHMEM_LENGTH		(MAX_WRITE2SOCKET_LENGTH + 4 )	// 1504
static const uint32_t kSourceID = 0xdeadbeef;

#include "include/ESDS.h"

namespace android {

struct MPEG2TSWriter::SourceInfo : public AHandler {
    SourceInfo(const sp<IMediaSource> &source, bool transcodeFile, int32_t VideoFrameFps);

    void start(const sp<AMessage> &notify);
    void stop();

    unsigned streamType() const;
    unsigned incrementContinuityCounter();

#if 1
    int64_t getInitialReadTime();
#endif

    void readMore();

    enum {
        kNotifyStartFailed,
        kNotifyBuffer,
        kNotifyReachedEOS,
    };

    sp<ABuffer> lastAccessUnit();
    int64_t lastAccessUnitTimeUs();
    void setLastAccessUnit(const sp<ABuffer> &accessUnit);

    void setEOSReceived();
    bool eosReceived() const;

protected:
    virtual void onMessageReceived(const sp<AMessage> &msg);

    virtual ~SourceInfo();

private:
    enum {
        kWhatStart = 'strt',
        kWhatRead  = 'read',
    };

    sp<IMediaSource> mSource;
    sp<ALooper> mLooper;
    sp<AMessage> mNotify;

    sp<ABuffer> mAACCodecSpecificData;

    size_t      mAACBps;
    sp<ABuffer> mAACBuffer;
	sp<ABuffer> mAVCSpsBuffer;
	sp<ABuffer> mAC3Buffer;

    sp<ABuffer> mLastAccessUnit;
    bool mEOSReceived;

#if 1
    int64_t mInitialReadTime;
#endif

    unsigned mStreamType;
    unsigned mContinuityCounter;

	int64_t mLastPts;
	int64_t mRenderFrameRate;

    void extractCodecSpecificData();

    bool appendAACFrames(MediaBuffer *buffer);
    bool flushAACFrames();

    void postAVCFrame(MediaBuffer *buffer);
    void postAC3Frame(MediaBuffer *buffer);

    DISALLOW_EVIL_CONSTRUCTORS(SourceInfo);
};

MPEG2TSWriter::SourceInfo::SourceInfo(const sp<IMediaSource> &source, bool transcodeFile, int32_t VideoFrameFps)
    : mSource(source),
      mLooper(new ALooper),
      mAACBps(0),
      mEOSReceived(false),
      mStreamType(0),
      mContinuityCounter(0) {
    mLooper->setName("MPEG2TSWriter source");

    sp<MetaData> meta = mSource->getFormat();
    //meta->dumpToLog();
    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        mStreamType = 0x0f;
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
        mStreamType = 0x1b;
		if (transcodeFile)
		{
			if (VideoFrameFps > 120 || VideoFrameFps <= 0)
				VideoFrameFps = 30;
			VideoFrameFps += 1; // Avoid encoder or decoder delay
			mRenderFrameRate = 1000000000 / VideoFrameFps;
			mRenderFrameRate = mRenderFrameRate / 1000;
			ALOGI("======transcodeFile [%d] new VideoFrameFps [%d]  mRenderFrameRate [%lld]======", transcodeFile, VideoFrameFps, (long long)mRenderFrameRate);
		}
		else
			mRenderFrameRate = 0;
		mLastPts = 0;
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AC3)) {
        mStreamType = 0x81; //AC3
    } else {
        TRESPASS();
    }
}

MPEG2TSWriter::SourceInfo::~SourceInfo() {
}

#if 1
int64_t MPEG2TSWriter::SourceInfo::getInitialReadTime() {
	if(mInitialReadTime == 0){
	    sp<MetaData> meta = mSource->getFormat();
	    if(meta->findInt64(kKeyInitialReadTime, &mInitialReadTime)){
	        ALOGD("SourceInfo got %s initial read time:%f", (mStreamType == 0x1b) ? "video" : "audio", mInitialReadTime / 1000000.0);
	    }
	}
	return mInitialReadTime;
}
#endif

unsigned MPEG2TSWriter::SourceInfo::streamType() const {
    return mStreamType;
}

unsigned MPEG2TSWriter::SourceInfo::incrementContinuityCounter() {
    if (++mContinuityCounter == 16) {
        mContinuityCounter = 0;
    }

    return mContinuityCounter;
}

void MPEG2TSWriter::SourceInfo::start(const sp<AMessage> &notify) {
    mLooper->registerHandler(this);
    mLooper->start();

    mNotify = notify;

#if 1
    mInitialReadTime = 0;
#endif

    (new AMessage(kWhatStart, this))->post();
}

void MPEG2TSWriter::SourceInfo::stop() {
    mSource->stop();

    mLooper->unregisterHandler(id());
    mLooper->stop();

}

void MPEG2TSWriter::SourceInfo::extractCodecSpecificData() {
    sp<MetaData> meta = mSource->getFormat();
    //meta->dumpToLog();

    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        uint32_t type;
        const void *data;
        size_t size;
        if (!meta->findData(kKeyESDS, &type, &data, &size)) {
            // Codec specific data better be in the first data buffer.
            return;
        }

        ESDS esds((const char *)data, size);
        CHECK_EQ(esds.InitCheck(), (status_t)OK);

        const uint8_t *codec_specific_data;
        size_t codec_specific_data_size;
        esds.getCodecSpecificInfo(
                (const void **)&codec_specific_data, &codec_specific_data_size);

        CHECK_GE(codec_specific_data_size, 2u);

        mAACCodecSpecificData = new ABuffer(codec_specific_data_size);

        memcpy(mAACCodecSpecificData->data(), codec_specific_data,
               codec_specific_data_size);

        return;
    }

    /* Handle AC3 buffer */
    if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AC3)) {
        ALOGD("\033[0;32m ---- In %s:%d received ac3 packet need check audio pts and framesize-----\033[m", __FUNCTION__, __LINE__);
        return;
    }

    if (strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
        return;
    }


    uint32_t type;
    const void *data;
    size_t size;
    if (!meta->findData(kKeyAVCC, &type, &data, &size)) {
        // Codec specific data better be part of the data stream then.
        return;
    }

    sp<ABuffer> out = new ABuffer(1024);
    out->setRange(0, 0);

    const uint8_t *ptr = (const uint8_t *)data;

    size_t numSeqParameterSets = ptr[5] & 31;

    ptr += 6;
    size -= 6;

    for (size_t i = 0; i < numSeqParameterSets; ++i) {
        CHECK(size >= 2);
        size_t length = U16_AT(ptr);

        ptr += 2;
        size -= 2;

        CHECK(size >= length);

        CHECK_LE(out->size() + 4 + length, out->capacity());
        memcpy(out->data() + out->size(), "\x00\x00\x00\x01", 4);
        memcpy(out->data() + out->size() + 4, ptr, length);
        out->setRange(0, out->size() + length + 4);

        ptr += length;
        size -= length;
    }

    CHECK(size >= 1);
    size_t numPictureParameterSets = *ptr;
    ++ptr;
    --size;

    for (size_t i = 0; i < numPictureParameterSets; ++i) {
        CHECK(size >= 2);
        size_t length = U16_AT(ptr);

        ptr += 2;
        size -= 2;

        CHECK(size >= length);

        CHECK_LE(out->size() + 4 + length, out->capacity());
        memcpy(out->data() + out->size(), "\x00\x00\x00\x01", 4);
        memcpy(out->data() + out->size() + 4, ptr, length);
        out->setRange(0, out->size() + length + 4);

        ptr += length;
        size -= length;
    }

    out->meta()->setInt64("timeUs", -1ll /*0ll*/); //CSD doesn't have pts

    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", kNotifyBuffer);
    notify->setBuffer("buffer", out);
    notify->setInt32("oob", true);
    notify->post();
}

void MPEG2TSWriter::SourceInfo::postAVCFrame(MediaBuffer *buffer) {

#if 0
    static int fd = -1;
    if(fd == -1)
        fd = open("/tmp/dump.h264", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd >= 0)
        write(fd, (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
#endif

    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", kNotifyBuffer);

    const uint8_t startcode[] = { 0x00, 0x00, 0x00, 0x01 };
    const uint32_t *sc = (const uint32_t *)startcode;
    const uint32_t *token = (const uint32_t *)buffer->data();
    //const uint8_t *d = (const uint8_t *)buffer->data();
    //ALOGI("postAVCFrame %02x %02x %02x %02x %02x len:%d", d[0], d[1], d[2], d[3], d[4], buffer->range_length());
#ifdef NO_SPS_PPS_WORKAROUND
    const uint8_t spsStartCode[] = { 0x00, 0x00, 0x00, 0x01, 0x67 };
	const uint32_t *sps = (const uint32_t *)spsStartCode;
	const uint8_t *bufferPtr = (const uint8_t *)buffer->data();
	bool IFramesAddSps= false;
	size_t allocLength = buffer->range_length();
	bool NeedAddStartCode = false;
	size_t copyOffset = 0;

    sp<ABuffer> copy;
    //if(d[0] == 0x00 && d[1] == 0x00 && d[2] == 0x00 && d[3] == 0x01)
    if(*token == *sc){ //already has start code
		if (bufferPtr[4] == 0x65 && mAVCSpsBuffer != NULL)
			IFramesAddSps = true;
        //copy = new ABuffer(buffer->range_length());
        //memcpy(copy->data(), (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
    }else{ //need to prepend start code manually
		allocLength += 4;
		NeedAddStartCode = true;
		if (bufferPtr[0] == 0x65 && mAVCSpsBuffer != NULL)
			IFramesAddSps = true;
    }

	if (mAVCSpsBuffer == NULL)
	{
		if (*token == *sps)
		{
			size_t alloc = buffer->range_length();
			mAVCSpsBuffer = new ABuffer(alloc);
			memcpy(mAVCSpsBuffer->data(), (const uint8_t *)buffer->data(), buffer->range_length());
		}
	}

	if (IFramesAddSps)	
	{
		allocLength += mAVCSpsBuffer->size();
	}

	copy = new ABuffer(allocLength);
	/* copy SPS / PPS */
	if (IFramesAddSps)
	{
		memcpy(copy->data(), mAVCSpsBuffer->data(), mAVCSpsBuffer->size());
		copyOffset += mAVCSpsBuffer->size();
	}
	/* copy start code */
	if (NeedAddStartCode)
	{
        memcpy(copy->data() + copyOffset, startcode, 4);
		copyOffset += 4;
	}
	/* copy data */
	memcpy(copy->data() + copyOffset, (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());

#else /* else of NO_SPS_PPS_WORKAROUND */
    sp<ABuffer> copy;
    //if(d[0] == 0x00 && d[1] == 0x00 && d[2] == 0x00 && d[3] == 0x01)
    if(*token == *sc){ //already has start code
        copy = new ABuffer(buffer->range_length());
        memcpy(copy->data(), (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
    }else{ //need to prepend start code manually
        copy = new ABuffer(buffer->range_length() + 4);
        memcpy(copy->data(), startcode, 4);
        memcpy(copy->data() + 4, (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
    }
#endif /* end of NO_SPS_PPS_WORKAROUND */
    int64_t timeUs;
    CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));
    copy->meta()->setInt64("timeUs", timeUs);

    int32_t isSync;
    if (buffer->meta_data()->findInt32(kKeyIsSyncFrame, &isSync)
            && isSync != 0) {
        copy->meta()->setInt32("isSync", true);
    }

    notify->setBuffer("buffer", copy);
    notify->post();
#ifdef SUPPORT_VIDEO_BITRATE_CONTROL
	if (mRenderFrameRate > 0)
	{
		/* It mean transcode, we need control video output fps */
		int64_t currentTimeUs = ALooper::GetNowUs();
		if (mLastPts == 0)
			mLastPts = currentTimeUs;
		else
		{
			if (mRenderFrameRate > (currentTimeUs - mLastPts))
				usleep(mRenderFrameRate - ((currentTimeUs - mLastPts)));
			mLastPts = ALooper::GetNowUs();
		}
	}
#endif /* end of SUPPORT_VIDEO_BITRATE_CONTROL */
#if 0
	static Statistics *ts_st= new Statistics("MPEG2TSVideo", 3);
	ts_st->rate(1);
#endif
}

void MPEG2TSWriter::SourceInfo::postAC3Frame(MediaBuffer *buffer) {
#if 0
    static int fd = -1;
    if(fd == -1)
        fd = open("/tmp/es.ac3", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd >= 0)
        write(fd, (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
#endif

    int64_t timeUs;
    CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));
    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", kNotifyBuffer);

    ALOGV("\033[0;32m ---- In %s:%d timeUs:%.3f, size:%d -----\033[m", __FUNCTION__, __LINE__, (double)timeUs / 1000000000.0, (int)buffer->range_length());
    if (mAC3Buffer == NULL) {
        size_t alloc = buffer->range_length() + 10;
        mAC3Buffer = new ABuffer(alloc);
        mAC3Buffer->meta()->setInt64("timeUs", timeUs);
        mAC3Buffer->meta()->setInt32("isSync", true);

        mAC3Buffer->setRange(0, 0);
    }
    memcpy(mAC3Buffer->data(), (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());

    mAC3Buffer->setRange(0, buffer->range_length());
    notify->setBuffer("buffer", mAC3Buffer);
    notify->post();
    mAC3Buffer.clear();
#if 0
	static Statistics *ts_st= new Statistics("MPEG2TSAC3", 3);
	ts_st->rate(1);
#endif
}

bool MPEG2TSWriter::SourceInfo::appendAACFrames(MediaBuffer *buffer) {
    bool accessUnitPosted = false;

    int64_t timeUs;
    CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));

    if (mAACBuffer != NULL) {
        if(mAACBuffer->size() + 7 + buffer->range_length() > mAACBuffer->capacity())
        {
            accessUnitPosted = flushAACFrames();
        }
#if 1
        else
        {
            int64_t beginTime;
            CHECK(mAACBuffer->meta()->findInt64("timeUs", &beginTime));
            if(timeUs - beginTime > 250000) //flush AAC frames every 0.25sec
                accessUnitPosted = flushAACFrames();
        }
#endif
    }

    if (mAACBuffer == NULL) {
        size_t alloc;

#if 0
        if(mAACBps == 0){
            ALOGV("No bitrate avail, trying to find it...");
            int32_t bps;
            sp<MetaData> meta = mSource->getFormat();
            if(meta->findInt32(kKeyBitRate, &bps) && bps > 0)
                mAACBps = bps;
        }
        if(mAACBps)
            alloc = mAACBps / 8 / 4; //flush AAC Frames every 0.25sec
#endif
            alloc = 4096; //flush AAC every 2 sec if bitrate is 16000 bit/s

        if (buffer->range_length() + 7 > alloc) {
            alloc = 7 + buffer->range_length();
        }

        mAACBuffer = new ABuffer(alloc);

        mAACBuffer->meta()->setInt64("timeUs", timeUs);
        mAACBuffer->meta()->setInt32("isSync", true);

        mAACBuffer->setRange(0, 0);
    }

    const uint8_t *codec_specific_data = mAACCodecSpecificData->data();

    unsigned profile = (codec_specific_data[0] >> 3) - 1;

    unsigned sampling_freq_index =
        ((codec_specific_data[0] & 7) << 1)
        | (codec_specific_data[1] >> 7);

    unsigned channel_configuration =
        (codec_specific_data[1] >> 3) & 0x0f;

    uint8_t *ptr = mAACBuffer->data() + mAACBuffer->size();

    const uint32_t aac_frame_length = buffer->range_length() + 7;

    *ptr++ = 0xff;
    *ptr++ = 0xf1;  // b11110001, ID=0, layer=0, protection_absent=1

    *ptr++ =
        profile << 6
        | sampling_freq_index << 2
        | ((channel_configuration >> 2) & 1);  // private_bit=0

    // original_copy=0, home=0, copyright_id_bit=0, copyright_id_start=0
    *ptr++ =
        (channel_configuration & 3) << 6
        | aac_frame_length >> 11;
    *ptr++ = (aac_frame_length >> 3) & 0xff;
    *ptr++ = (aac_frame_length & 7) << 5;

    // adts_buffer_fullness=0, number_of_raw_data_blocks_in_frame=0
    *ptr++ = 0;

    memcpy(ptr,
           (const uint8_t *)buffer->data() + buffer->range_offset(),
           buffer->range_length());

    ptr += buffer->range_length();

    mAACBuffer->setRange(0, ptr - mAACBuffer->data());

    return accessUnitPosted;
}

bool MPEG2TSWriter::SourceInfo::flushAACFrames() {
    if (mAACBuffer == NULL) {
        return false;
    }

    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", kNotifyBuffer);
    notify->setBuffer("buffer", mAACBuffer);
    notify->post();

    mAACBuffer.clear();

    return true;
}

void MPEG2TSWriter::SourceInfo::readMore() {
    (new AMessage(kWhatRead, this))->post();
}

void MPEG2TSWriter::SourceInfo::onMessageReceived(const sp<AMessage> &msg) {
    switch (msg->what()) {
        case kWhatStart:
        {
            status_t err = mSource->start();
            if (err != OK) {
                sp<AMessage> notify = mNotify->dup();
                notify->setInt32("what", kNotifyStartFailed);
                notify->post();
                break;
            }

            extractCodecSpecificData();

            readMore();
            break;
        }

        case kWhatRead:
        {
            MediaBuffer *buffer;
#if 0
            static nsecs_t t1, t2;
            if (mStreamType == 0x0f){
                t1 = systemTime();
                ALOGD("before kWhatRead: %.3fs", (t1 - t2)/1000000000.0);
                //if(t1 - t2 > 100000000)	ALOGD("current time: %f", t1 / 1000000000.0);
            }
#endif
            status_t err = mSource->read(&buffer);

            if (err != OK && err != INFO_FORMAT_CHANGED) {
                if (mStreamType == 0x0f) {
                    flushAACFrames();
                }

                sp<AMessage> notify = mNotify->dup();
                notify->setInt32("what", kNotifyReachedEOS);
                notify->setInt32("status", err);
                notify->post();
                break;
            }

            if (err == OK) {
                if (mStreamType == 0x0f && mAACCodecSpecificData == NULL) {
                    // The first buffer contains codec specific data.

                    CHECK_GE(buffer->range_length(), 2u);

                    //const uint8_t *d = (const uint8_t *)buffer->data();
                    //ALOGV("AACCodecSpecificData %02x %02x %02x %02x len:%d", d[0], d[1], d[2], d[3], buffer->range_length());

                    mAACCodecSpecificData = new ABuffer(buffer->range_length());

                    memcpy(mAACCodecSpecificData->data(),
                           (const uint8_t *)buffer->data()
                            + buffer->range_offset(),
                           buffer->range_length());

                    //20131017 charleslin - AAC track blocks forever because next read event is not scheduled here
                    //https://code.google.com/p/android/issues/detail?id=37714
                    readMore();

                } else if (buffer->range_length() > 0) {
                    if (mStreamType == 0x0f) {
                        //ALOGV("AACFrame %02x %02x %02x %02x len:%d", d[0], d[1], d[2], d[3], buffer->range_length());
                        //nsecs_t t1 = systemTime();
                        if (!appendAACFrames(buffer)) {
                            msg->post(); //kWhatRead i.e., readMore
                        }
                        //nsecs_t t2 = systemTime();
                        //ALOGD("appendAACFrames: %.3fs", (t2 - t1)/1000000000.0);
                    } else if (mStreamType == 0x81) { // Audio AC3
                            postAC3Frame(buffer);
                    } else {
                        postAVCFrame(buffer);
                    }
                } else {
                    readMore();
                }

                buffer->release();
                buffer = NULL;
            }
#if 0
            if (mStreamType == 0x0f){
                t2 = systemTime();
                ALOGD("after kWhatRead: %.3fs", (t2 - t1)/1000000000.0);
            }
#endif
            // Do not read more data until told to.
            break;
        }

        default:
            TRESPASS();
    }
}

sp<ABuffer> MPEG2TSWriter::SourceInfo::lastAccessUnit() {
    return mLastAccessUnit;
}

void MPEG2TSWriter::SourceInfo::setLastAccessUnit(
        const sp<ABuffer> &accessUnit) {
    mLastAccessUnit = accessUnit;
}

int64_t MPEG2TSWriter::SourceInfo::lastAccessUnitTimeUs() {
    if (mLastAccessUnit == NULL) {
        return -1;
    }

    int64_t timeUs;
    CHECK(mLastAccessUnit->meta()->findInt64("timeUs", &timeUs));

    return timeUs;
}

void MPEG2TSWriter::SourceInfo::setEOSReceived() {
    CHECK(!mEOSReceived);
    mEOSReceived = true;
}

bool MPEG2TSWriter::SourceInfo::eosReceived() const {
    return mEOSReceived;
}

////////////////////////////////////////////////////////////////////////////////

MPEG2TSWriter::MPEG2TSWriter(int fd) {
    initMembers();
    mFile = fdopen(dup(fd), "wb");
    init();
}

MPEG2TSWriter::MPEG2TSWriter(const char *filename) {
    initMembers();
    mFile = fopen(filename, "wb");
    init();
}

MPEG2TSWriter::MPEG2TSWriter(void *cookie, ssize_t (*write)(void *cookie, const void *data, size_t size)) {
    initMembers();
    mWriteCookie = cookie;
    mWriteFunc = write;
    init();
}

#if 1
MPEG2TSWriter::MPEG2TSWriter(FILE *f_fd) {
    initMembers();
    mFile = f_fd;
    init();
}

MPEG2TSWriter::MPEG2TSWriter(int port, const char *ip, bool enableRTP) {
    initMembers();
    mRtp = enableRTP;
    mPort = port;
    mIp.setTo(ip);
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	CHECK_GE(mSocket, 0);
	/* set socket value */
	memset(mUDPAddr.sin_zero, 0, sizeof(mUDPAddr.sin_zero));
	mUDPAddr.sin_family = AF_INET;
	mUDPAddr.sin_addr.s_addr = inet_addr(mIp.c_str());
	mUDPAddr.sin_port = htons(mPort);
    init();
}

#if 1//!defined(__LINARO_SDK__)
MPEG2TSWriter::MPEG2TSWriter(sp<IMemory> data, sp<IMemory> info, bool enableRTP) {
    initMembers();
    mRtp = enableRTP;
    mBufferData = data;
    mBufferInfo = info;
    ALOGD("data:%p:%d info:%p:%d", mBufferData->pointer(), (int)mBufferData->size(), mBufferInfo->pointer(), (int)mBufferInfo->size());
    struct BufferInfo_t *bufInfo = (struct BufferInfo_t *)mBufferInfo->pointer();
#if 0
    bufInfo->magic = ASHMEM_RTK_MAGIC_NUMBER;
    bufInfo->size = mBufferData->size();
    bufInfo->writePtr = 0;
#else
    //Don't reset writePtr, otherwise readPtr may get uninitialized data
    CHECK(bufInfo->magic == ASHMEM_RTK_MAGIC_NUMBER);
#endif
    bufInfo->endOfStream = 0;
	if (enableRTP)
		bufInfo->packet_size = RTP_OVER_ASHMEM_LENGTH;	 // 1500 Max RTP size and 4 bytes is described length
	else
		bufInfo->packet_size = 0;

	memset(mUDPAddr.sin_zero, 0, sizeof(mUDPAddr.sin_zero));
	mUDPAddr.sin_family = AF_INET;
	mUDPAddr.sin_addr.s_addr = inet_addr(mIp.c_str());
	mUDPAddr.sin_port = htons(mPort);
	ALOGI("[%s][%d] mRtp is [%s] ", __func__, __LINE__, mRtp ? "true" : "false" );
    init();
}

MPEG2TSWriter::MPEG2TSWriter(sp<IMemory> data, sp<IMemory> info, bool enableRTP, int port, const char *ip) {
    initMembers();
    mBufferData = data;
    mBufferInfo = info;
    mRtp = enableRTP;
    mPort = port;
    mIp.setTo(ip);
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ALOGD("data:%p info:%p", mBufferData->pointer(), mBufferInfo->pointer());

    struct BufferInfo_t *bufInfo = (struct BufferInfo_t *)mBufferInfo->pointer();
#if 0
    bufInfo->magic = ASHMEM_RTK_MAGIC_NUMBER;
    bufInfo->size = mBufferData->size();
    bufInfo->writePtr = 0;
#else
    //Don't reset writePtr, otherwise readPtr may get uninitialized data
    CHECK(bufInfo->magic == ASHMEM_RTK_MAGIC_NUMBER);
#endif
    bufInfo->endOfStream = 0;
	if (enableRTP)
		bufInfo->packet_size = RTP_OVER_ASHMEM_LENGTH;	 // 1500 Max RTP size and 4 bytes is described length
	else
		bufInfo->packet_size = 0;

	memset(mUDPAddr.sin_zero, 0, sizeof(mUDPAddr.sin_zero));
	mUDPAddr.sin_family = AF_INET;
	mUDPAddr.sin_addr.s_addr = inet_addr(mIp.c_str());
	mUDPAddr.sin_port = htons(mPort);
	ALOGI("[%s][%d] mRtp is [%s] ", __func__, __LINE__, mRtp ? "true" : "false" );
    init();
}
#endif

#if 1
void MPEG2TSWriter::setMuxerMode(int muxerMode) {
    sp<AMessage> msg = new AMessage(kWhatMuxerModeChange, mReflector);
    msg->setInt32("muxerMode", muxerMode);
    msg->post();
}

void MPEG2TSWriter::setVideoFrameFps(int32_t videoFrameFps) {
	mVideoFrameFps = videoFrameFps;
	ALOGV("==== set mVideoFrameFps %d ===", mVideoFrameFps);
}
#endif
#endif

void MPEG2TSWriter::initMembers() {
    mFile = NULL;
    mWriteCookie = NULL;
    mWriteFunc = NULL;
    mStarted = false;
    mNumSourcesDone = 0;
    mNumTSPacketsWritten = 0;
    mNumTSPacketsBeforeMeta = 0;
    mPATContinuityCounter = 0;
    mPMTContinuityCounter = 0;
#if 1
    mPort = 0;
    mIp.clear();
    mSocket = -1;
#if 1//!defined(__LINARO_SDK__)
    mBufferData = NULL;
    mBufferInfo = NULL;
#endif
    mRtp = false;
    mRTPSeqNo = 0;
    mMuxerMode = MuxInPtsOrder;
	mVideoFrameFps = 30;
#endif
}

void MPEG2TSWriter::init() {

#if 0//defined(__LINARO_SDK__)
	CHECK(mFile != NULL || mSocket != -1 || mWriteFunc != NULL);
#else
    CHECK(mFile != NULL || mSocket != -1 || mWriteFunc != NULL || (mBufferData != NULL && mBufferInfo != NULL));
#endif

    initCrcTable();

    mLooper = new ALooper;
    mLooper->setName("MPEG2TSWriter");

    mReflector = new AHandlerReflector<MPEG2TSWriter>(this);

    mLooper->registerHandler(mReflector);
    mLooper->start();
}

MPEG2TSWriter::~MPEG2TSWriter() {

    if(mMuxerMode == MuxInPtsOrderQueue)
    {
        flushTrackQueue();
        mFirstTrack.clear();
        mSecondTrack.clear();
    }

    if (mStarted) {
        reset();
    }

    mLooper->unregisterHandler(mReflector->id());
    mLooper->stop();

    if (mFile != NULL) {
        fclose(mFile);
        mFile = NULL;
    }

	if (mSocket > 0) {
		close(mSocket);
		mSocket = -1;
	}
}

status_t MPEG2TSWriter::addSource(const sp<IMediaSource> &source) {
    CHECK(!mStarted);

    sp<MetaData> meta = source->getFormat();
    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)
            && strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AC3)
            && strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        return ERROR_UNSUPPORTED;
    }

    //sp<SourceInfo> info = new SourceInfo(source);
    sp<SourceInfo> info = new SourceInfo(source, mMuxerMode == MuxInPtsOrder, mVideoFrameFps);
    mSources.push(info);

    return OK;
}

status_t MPEG2TSWriter::start(MetaData * /* param */) {
    CHECK(!mStarted);

    mStarted = true;
    mNumSourcesDone = 0;
    mNumTSPacketsWritten = 0;
    mNumTSPacketsBeforeMeta = 0;
#if 1
    mTsStartTime = 0;
    mStartPts = -1;
#endif

    for (size_t i = 0; i < mSources.size(); ++i) {
        sp<AMessage> notify =
            new AMessage(kWhatSourceNotify, mReflector);

        notify->setInt32("source-index", i);

        mSources.editItemAt(i)->start(notify);
    }

    return OK;
}

status_t MPEG2TSWriter::reset() {
    CHECK(mStarted);

    for (size_t i = 0; i < mSources.size(); ++i) {
        mSources.editItemAt(i)->stop();
    }
    mStarted = false;

#if 1//!defined(__LINARO_SDK__)
    if (mBufferData != NULL && mBufferInfo != NULL) {
        struct BufferInfo_t *info = (struct BufferInfo_t *)mBufferInfo->pointer();
        info->endOfStream = true;
    }
#endif

    return OK;
}

status_t MPEG2TSWriter::pause() {
    CHECK(mStarted);

    return OK;
}

bool MPEG2TSWriter::reachedEOS() {
    return !mStarted || (mNumSourcesDone == mSources.size() ? true : false);
}

status_t MPEG2TSWriter::dump(
        int /* fd */, const Vector<String16> & /* args */) {
    return OK;
}

void MPEG2TSWriter::onMessageReceived(const sp<AMessage> &msg) {
    switch (msg->what()) {
        case kWhatSourceNotify:
        {
            int32_t sourceIndex;
            CHECK(msg->findInt32("source-index", &sourceIndex));

            int32_t what;
            CHECK(msg->findInt32("what", &what));

            if (what == SourceInfo::kNotifyReachedEOS
                    || what == SourceInfo::kNotifyStartFailed) {
                sp<SourceInfo> source = mSources.editItemAt(sourceIndex);
                //source->setEOSReceived(); //move below

                sp<ABuffer> buffer = source->lastAccessUnit();
                source->setLastAccessUnit(NULL);

                if (buffer != NULL) {
                    writeTS();
                    writeAccessUnit(sourceIndex, buffer);
                }

                //kNotifyReachedEOS may be sent more than once
                if(!source->eosReceived()){
                    ++mNumSourcesDone;
                    source->setEOSReceived();
                }
                ALOGV("setEOSReceived: idx:%d what:%d mNumSourcesDone:%d", sourceIndex, what, (int)mNumSourcesDone);
				/* Fixed transcode file, audio or Video received EOS, not keep do remaining track */
				if (mNumSourcesDone != mSources.size())
				{
					if (mMuxerMode == MuxInPtsOrder)
					{
						int32_t ii = 0;
						for (ii = 0; ii < (int32_t)mSources.size(); ++ii) 
						{
							if (ii != sourceIndex)
								break;
						}
						const sp<SourceInfo> &source = mSources.editItemAt(ii);

						buffer = source->lastAccessUnit();
						source->setLastAccessUnit(NULL);
				
						/* 
						 * Fixed received Null Buffer case. Audio called flushAACFrames when Audio received EOS. After muxer maybe send Video to mxuer.
						 * It caused Video and Audio are both NULL. So we need change buffer is equal to NULL or not.
						 */
						if (buffer != NULL) 
						{	
							writeTS();
							writeAccessUnit(ii, buffer);
						}

						source->readMore();
					}
				}
            } else if (what == SourceInfo::kNotifyBuffer) {
                sp<ABuffer> buffer;
                CHECK(msg->findBuffer("buffer", &buffer));
                int64_t timeUs;
                CHECK(buffer->meta()->findInt64("timeUs", &timeUs));
#if 0
                //if(sourceIndex == 1) //audio
                {
                    static int64_t prev = 0;
                    ALOGV("idx:%d %s timeUs:%f diff:%f", sourceIndex, sourceIndex ? "audio" : "video", ptsUs / 1000000.0, (ptsUs - prev) / 1000000.0);
                    prev = ptsUs;
                }
#endif
#if 0
                if(sourceIndex == 1){ //audio
                    static Statistics *st = new Statistics("TSabps", 5);
                    st->rate(buffer->size() * 8);
                }else{
                    static Statistics *st = new Statistics("TSvbps", 5);
                    st->rate(buffer->size() * 8);
                }
#endif
                int32_t oob;
                if (msg->findInt32("oob", &oob) && oob) {
                    // This is codec specific data delivered out of band.
                    // It can be written out immediately.
                    writeTS();
                    writeAccessUnit(sourceIndex, buffer);
                    break;
                }

#if 1
                if(timeUs == -1){ //no pts available
                    writeTS();
                    writeAccessUnit(sourceIndex, buffer);
                    //if(sourceIndex) ALOGD("readMore:%d", __LINE__);
                    mSources.editItemAt(sourceIndex)->readMore();
                    break;
                }

                if(mMuxerMode == MuxDirectOutput)
                    muxToTsStream(sourceIndex, buffer, true);
                else if(mMuxerMode == MuxBySystemTime)
                    muxBySystemTime(msg, sourceIndex, buffer, timeUs);
                else if(mMuxerMode == MuxInIncomingOrder)
                    muxInIncomingOrder(sourceIndex, buffer, timeUs);
                else if(mMuxerMode == MuxInPtsOrderQueue)
                    muxInPtsOrderQueue(sourceIndex, buffer, timeUs);
                else if(mMuxerMode == MuxInPtsOrder)
#endif
                    muxInPtsOrder(sourceIndex, buffer);
            }
            break;
        }

        case kWhatMuxerModeChange:
        {
            int muxerMode;
            CHECK(msg->findInt32("muxerMode", &muxerMode));

            if(muxerMode == MuxDirectOutput)
                ALOGI("MuxerModeChange: direct output");
            else if(muxerMode == MuxBySystemTime)
                ALOGI("MuxerModeChange: by system time");
            else if(muxerMode == MuxInIncomingOrder)
                ALOGI("MuxerModeChange: in incoming order");
            else if(muxerMode == MuxInPtsOrder)
                ALOGI("MuxerModeChange: in pts order");
            else
                CHECK(muxerMode >= 0 && muxerMode < MuxMaxNumber);

            if(muxerMode != mMuxerMode){
                mMuxerMode = muxerMode;
                if(mMuxerMode == MuxBySystemTime){
                    mStartPts = -1;
                    mTsStartTime = 0;
                }
            }
            break;
        }

        default:
            TRESPASS();
    }
}

void MPEG2TSWriter::muxToTsStream(int32_t sourceIndex, sp<ABuffer> buffer, bool readMore)
{
    sp<SourceInfo> source = mSources.editItemAt(sourceIndex);

    writeTS();
    writeAccessUnit(sourceIndex, buffer);
    //if(sourceIndex) ALOGD("readMore:%d", __LINE__);

    if(readMore)
        source->readMore();
}

void MPEG2TSWriter::muxInPtsOrder(int32_t sourceIndex, sp<ABuffer> buffer)
{
    // We don't just write out data as we receive it from
    // the various sources. That would essentially write them
    // out in random order (as the thread scheduler determines
    // how the messages are dispatched).
    // Instead we gather an access unit for all tracks and
    // write out the one with the smallest timestamp, then
    // request more data for the written out track.
    // Rinse, repeat.
    // If we don't have data on any track we don't write
    // anything just yet.

    sp<SourceInfo> source = mSources.editItemAt(sourceIndex);

    CHECK(source->lastAccessUnit() == NULL);
    source->setLastAccessUnit(buffer);

    ALOGV("lastAccessUnitTimeUs[%d] = %.2f secs",
         sourceIndex, source->lastAccessUnitTimeUs() / 1E6);

    int64_t minTimeUs = -1;
    size_t minIndex = 0;

    for (size_t i = 0; i < mSources.size(); ++i) {
        const sp<SourceInfo> &source = mSources.editItemAt(i);

        if (source->eosReceived()) {
            continue;
        }

        int64_t timeUs = source->lastAccessUnitTimeUs();
        ALOGV("index:%d timeUs:%lld minTimeUs:%lld", (int)i, (long long)timeUs, (long long)minTimeUs);
        if (timeUs < 0) { //no last access unit
            minTimeUs = -1;
            break;
        } else if (minTimeUs < 0 || timeUs < minTimeUs) {
            minTimeUs = timeUs;
            minIndex = i;
        }
    }

    if (minTimeUs < 0) {
        ALOGV("not a all tracks have valid data.");
        return;
    }

    ALOGV("writing access unit at time %.2f secs (index %d)",
         minTimeUs / 1E6, (int)minIndex);

    source = mSources.editItemAt(minIndex);

    buffer = source->lastAccessUnit();
    source->setLastAccessUnit(NULL);

    muxToTsStream(minIndex, buffer, true);
}

void MPEG2TSWriter::muxInPtsOrderQueue(int32_t sourceIndex, sp<ABuffer> buffer, int64_t timeUs)
{
    if(mStartPts == -1){
        mStartPts = timeUs;
        mTsStartTime = systemTime();
    }

    if(mSources.size() == 1)
    {
        muxToTsStream(sourceIndex, buffer, true);
        return;
    }

    sp<ABuffer> bufferTmp = new ABuffer(buffer->size());
    int videoLastIdx, audioLastIdx;
    videoLastIdx = mFirstTrack.size();
    audioLastIdx = mSecondTrack.size();
    if(sourceIndex == 0)
    {
        mFirstTrack.add(bufferTmp);
        mFirstTrack[videoLastIdx]->meta()->setInt64("timeUs", timeUs);
        memset(mFirstTrack[videoLastIdx]->data(), 0xff, mFirstTrack[videoLastIdx]->size());
        memcpy(mFirstTrack[videoLastIdx]->data(), buffer->data(), buffer->size());
        mFirstTrack[videoLastIdx]->setRange(0, buffer->size());
    }
    else
    {
        mSecondTrack.add(bufferTmp);
        mSecondTrack[audioLastIdx]->meta()->setInt64("timeUs", timeUs);
        memset(mSecondTrack[audioLastIdx]->data(), 0xff, mSecondTrack[audioLastIdx]->size());
        memcpy(mSecondTrack[audioLastIdx]->data(), buffer->data(), buffer->size());
        mSecondTrack[audioLastIdx]->setRange(0, buffer->size());
    }

    while(mFirstTrack.size() > 0 && mSecondTrack.size() > 0)
    {
        int64_t firstTimeUs, secondTimeUs;
        CHECK(mFirstTrack[0]->meta()->findInt64("timeUs", &firstTimeUs));
        CHECK(mSecondTrack[0]->meta()->findInt64("timeUs", &secondTimeUs));
        if(secondTimeUs <= firstTimeUs)
        {
            muxToTsStream(1, mSecondTrack[0], false);
            mSecondTrack.erase(mSecondTrack.begin());
        }
        else
        {
            muxToTsStream(0, mFirstTrack[0], false);
            mFirstTrack.erase(mFirstTrack.begin());
        }
    }
//    ALOGD(" remain first %zu second %zu", mFirstTrack.size(), mSecondTrack.size());
    sp<SourceInfo> source = mSources.editItemAt(sourceIndex);
    source->readMore();
}

void MPEG2TSWriter::flushTrackQueue()
{
    if(mFirstTrack.size() > 0)
    {
        while(mFirstTrack.size() > 0)
        {
            muxToTsStream(0, mFirstTrack[0], false);
            mFirstTrack.erase(mFirstTrack.begin());
        }
    }
    else if(mSecondTrack.size() > 0)
    {
        while(mSecondTrack.size() > 0)
        {
            muxToTsStream(1, mSecondTrack[0], false);
            mSecondTrack.erase(mSecondTrack.begin());
        }
    }
}

//here we don't control the output order by timestamp
//instead, we just write access unit directly
void MPEG2TSWriter::muxInIncomingOrder(int32_t sourceIndex, sp<ABuffer> buffer, int64_t timeUs)
{

#if 0
    static Statistics *alatency = new Statistics("alatency", 5);
    static Statistics *vlatency = new Statistics("vlatency", 5);
    int64_t tcurr = systemTime() / 1000;

    if(sourceIndex == 0){ //video
        ALOGD("video %f - %f = latency:%f", tcurr / 1000000.0, ptsUs / 1000000.0, (tcurr - ptsUs) / 1000000.0);
        vlatency->count(tcurr - ptsUs);
    }else{
        ALOGD("audio latency:%f", (tcurr - ptsUs) / 1000000.0);
        alatency->count(tcurr - ptsUs);
    }
#endif

#if 1
    //here we try to find out the smallest initial read time among sources
    //if the source initial read time is larger than mTsStartTime,
    //we will add the difference to the buffer pts
    sp<SourceInfo> source = mSources.editItemAt(sourceIndex);
    int64_t initialReadTime = source->getInitialReadTime();
    if(mTsStartTime == 0 || initialReadTime < mTsStartTime){ //update the smallest initial read time
        if(initialReadTime) ALOGD("got smaller initial read time %f < %f", initialReadTime / 1000000.0, mTsStartTime / 1000000.0);
        mTsStartTime = initialReadTime;
    }else if(initialReadTime > mTsStartTime){ //modify buffer pts
        int64_t newTimeUs;
        newTimeUs = timeUs + (initialReadTime - mTsStartTime);
        buffer->meta()->setInt64("timeUs", newTimeUs);
        //ALOGD("shift %s pts %.4f => %.4f", (source->streamType() == 0x0f) ? "audio" : "video", oriTimeUs / 1000000.0, newTimeUs / 1000000.0);
    }
#endif
    muxToTsStream(sourceIndex, buffer, true);
}

void MPEG2TSWriter::muxBySystemTime(const sp<AMessage> &msg, int32_t sourceIndex, sp<ABuffer> buffer, int64_t timeUs)
{
    if(mStartPts == -1){
        mStartPts = timeUs;
        mTsStartTime = systemTime();
    }

    int64_t sysDuration = systemTime() - mTsStartTime;
    int64_t ptsDuration = (timeUs - mStartPts) * 1000;

    if(ptsDuration - sysDuration < MAX_DURATION_DIFF){ //diff < 0.5sec, write immediately
        muxToTsStream(sourceIndex, buffer, true);
    }else{
        ALOGV("[%d] sysDur:%.3f - ptsDur:%.3f = %.3f", sourceIndex, sysDuration / 1000000000.0, ptsDuration / 1000000000.0, (ptsDuration - sysDuration) / 1000000000.0);
        msg->post((ptsDuration - sysDuration - MAX_DURATION_DIFF)/1000); //post again
    }
}

void MPEG2TSWriter::writeProgramAssociationTable() {
    // 0x47
    // transport_error_indicator = b0
    // payload_unit_start_indicator = b1
    // transport_priority = b0
    // PID = b0000000000000 (13 bits)
    // transport_scrambling_control = b00
    // adaptation_field_control = b01 (no adaptation field, payload only)
    // continuity_counter = b????
    // skip = 0x00
    // --- payload follows
    // table_id = 0x00
    // section_syntax_indicator = b1
    // must_be_zero = b0
    // reserved = b11
    // section_length = 0x00d
    // transport_stream_id = 0x0000
    // reserved = b11
    // version_number = b00001
    // current_next_indicator = b1
    // section_number = 0x00
    // last_section_number = 0x00
    //   one program follows:
    //   program_number = 0x0001
    //   reserved = b111
    //   program_map_PID = 0x01e0 (13 bits!)
    // CRC = 0x????????

    static const uint8_t kData[] = {
        0x47,
        0x40, 0x00, 0x10, 0x00,  // b0100 0000 0000 0000 0001 ???? 0000 0000
        0x00, 0xb0, 0x0d, 0x00,  // b0000 0000 1011 0000 0000 1101 0000 0000
        0x00, 0xc3, 0x00, 0x00,  // b0000 0000 1100 0011 0000 0000 0000 0000
        0x00, 0x01, 0xe1, 0xe0,  // b0000 0000 0000 0001 1110 0001 1110 0000
        0x00, 0x00, 0x00, 0x00   // b???? ???? ???? ???? ???? ???? ???? ????
    };

    sp<ABuffer> buffer = new ABuffer(188);
    memset(buffer->data(), 0xff, buffer->size());
    memcpy(buffer->data(), kData, sizeof(kData));

    if (++mPATContinuityCounter == 16) {
        mPATContinuityCounter = 0;
    }
    buffer->data()[3] |= mPATContinuityCounter;

    uint32_t crc = htonl(crc32(&buffer->data()[5], 12));
    memcpy(&buffer->data()[17], &crc, sizeof(crc));

	if (mRtp)
		convertRtp(buffer, buffer->size());
	else
		CHECK_EQ_2(internalWrite(buffer->data(), buffer->size()), buffer->size());
}

void MPEG2TSWriter::writeProgramMap() {
    // 0x47
    // transport_error_indicator = b0
    // payload_unit_start_indicator = b1
    // transport_priority = b0
    // PID = b0 0001 1110 0000 (13 bits) [0x1e0]
    // transport_scrambling_control = b00
    // adaptation_field_control = b01 (no adaptation field, payload only)
    // continuity_counter = b????
    // skip = 0x00
    // -- payload follows
    // table_id = 0x02
    // section_syntax_indicator = b1
    // must_be_zero = b0
    // reserved = b11
    // section_length = 0x???
    // program_number = 0x0001
    // reserved = b11
    // version_number = b00001
    // current_next_indicator = b1
    // section_number = 0x00
    // last_section_number = 0x00
    // reserved = b111
    // PCR_PID = b? ???? ???? ???? (13 bits)
    // reserved = b1111
    // program_info_length = 0x000
    //   one or more elementary stream descriptions follow:
    //   stream_type = 0x??
    //   reserved = b111
    //   elementary_PID = b? ???? ???? ???? (13 bits)
    //   reserved = b1111
    //   ES_info_length = 0x000
    // CRC = 0x????????

    static const uint8_t kData[] = {
        0x47,
        0x41, 0xe0, 0x10, 0x00,  // b0100 0001 1110 0000 0001 ???? 0000 0000
        0x02, 0xb0, 0x00, 0x00,  // b0000 0010 1011 ???? ???? ???? 0000 0000
        0x01, 0xc3, 0x00, 0x00,  // b0000 0001 1100 0011 0000 0000 0000 0000
        0xe0, 0x00, 0xf0, 0x00   // b111? ???? ???? ???? 1111 0000 0000 0000
    };

    sp<ABuffer> buffer = new ABuffer(188);
    memset(buffer->data(), 0xff, buffer->size());
    memcpy(buffer->data(), kData, sizeof(kData));

    if (++mPMTContinuityCounter == 16) {
        mPMTContinuityCounter = 0;
    }
    buffer->data()[3] |= mPMTContinuityCounter;

    size_t section_length = 5 * mSources.size() + 4 + 9;
    buffer->data()[6] |= section_length >> 8;
    buffer->data()[7] = section_length & 0xff;

    //static const unsigned kPCR_PID = 0x1e1;
    static const unsigned kPCR_PID = kPID_PCR;
    buffer->data()[13] |= (kPCR_PID >> 8) & 0x1f;
    buffer->data()[14] = kPCR_PID & 0xff;

    uint8_t *ptr = &buffer->data()[sizeof(kData)];
    for (size_t i = 0; i < mSources.size(); ++i) {
        *ptr++ = mSources.editItemAt(i)->streamType();

        const unsigned ES_PID = DEFAULT_PID + i + 1;
        *ptr++ = 0xe0 | (ES_PID >> 8);
        *ptr++ = ES_PID & 0xff;
        *ptr++ = 0xf0;
        *ptr++ = 0x00;
    }

    uint32_t crc = htonl(crc32(&buffer->data()[5], 12+mSources.size()*5));
    memcpy(&buffer->data()[17+mSources.size()*5], &crc, sizeof(crc));

	if (mRtp)
		convertRtp(buffer, buffer->size());
	else
		CHECK_EQ_2(internalWrite(buffer->data(), buffer->size()), buffer->size());
}

void MPEG2TSWriter::writeAccessUnit(
        int32_t sourceIndex, const sp<ABuffer> &accessUnit) {
    // 0x47
    // transport_error_indicator = b0
    // payload_unit_start_indicator = b1
    // transport_priority = b0
    // PID = b0 0001 1110 ???? (13 bits) [0x1e0 + 1 + sourceIndex]
    // transport_scrambling_control = b00
    // adaptation_field_control = b??
    // continuity_counter = b????
    // -- payload follows
    // packet_startcode_prefix = 0x000001
    // stream_id = 0x?? (0xe0 for avc video, 0xc0 for aac audio)
    // PES_packet_length = 0x????
    // reserved = b10
    // PES_scrambling_control = b00
    // PES_priority = b0
    // data_alignment_indicator = b1
    // copyright = b0
    // original_or_copy = b0
    // PTS_DTS_flags = b10  (PTS only)
    // ESCR_flag = b0
    // ES_rate_flag = b0
    // DSM_trick_mode_flag = b0
    // additional_copy_info_flag = b0
    // PES_CRC_flag = b0
    // PES_extension_flag = b0
    // PES_header_data_length = 0x05
    // reserved = b0010 (PTS)
    // PTS[32..30] = b???
    // reserved = b1
    // PTS[29..15] = b??? ???? ???? ???? (15 bits)
    // reserved = b1
    // PTS[14..0] = b??? ???? ???? ???? (15 bits)
    // reserved = b1
    // the first fragment of "buffer" follows

#ifdef WRITE_DATA_FROM_LARGE_BUFFER
    sp<ABuffer> buffer = new ABuffer(MAX_WRITE2SOCKET_LENGTH);
#else /* else of WRITE_DATA_FROM_LARGE_BUFFER */
    sp<ABuffer> buffer = new ABuffer(188);
#endif /* end of WRITE_DATA_FROM_LARGE_BUFFER */
    memset(buffer->data(), 0xff, buffer->size());

    const unsigned PID = DEFAULT_PID + sourceIndex + 1;


    // XXX if there are multiple streams of a kind (more than 1 audio or
    // more than 1 video) they need distinct stream_ids.
    const unsigned stream_id =
        mSources.editItemAt(sourceIndex)->streamType() == 0x1b ? 0xe0 : 0xc0;

#ifdef USED_ALONE_PCRPID
	unsigned pcr_continuity_counter  = 0;
#endif /* end of (defined(USED_VIDEO_PCRPID) || defined(USED_ALONE_PCRPID)) */
    unsigned continuity_counter =
        mSources.editItemAt(sourceIndex)->incrementContinuityCounter();
    int64_t timeUs;
    CHECK(accessUnit->meta()->findInt64("timeUs", &timeUs));
    ALOGV("%s %.3f sz:%d", mSources.editItemAt(sourceIndex)->streamType() == 0x1b ? "video": "audio", timeUs/1000000.0, (int)accessUnit->size());

    //uint32_t PTS = (timeUs * 9ll) / 100ll;
    uint64_t PTS = (timeUs * 9ll) / 100ll;
    bool hasPts = (timeUs != -1);

    size_t PES_packet_length = accessUnit->size() + 8;
    bool padding = (accessUnit->size() < (188 - 18));
#ifdef ADD_PCR_IN_AF
	if (!padding && stream_id == 0xe0) 	// video 
		PES_packet_length += 8;
#endif /* end of ADD_PCR_IN_AF */

    if (PES_packet_length >= 65536) {
        // This really should only happen for video.
        CHECK_EQ(stream_id, 0xe0u);

        // It's valid to set this to 0 for video according to the specs.
        PES_packet_length = 0;
    }

    uint8_t *ptr = buffer->data();
    *ptr++ = 0x47;
    *ptr++ = 0x40 | (PID >> 8);
    *ptr++ = PID & 0xff;
#ifdef ADD_PCR_IN_AF
	unsigned int adaptation_field_control = 0;
	if (!padding && stream_id == 0xe0) 	// video 
		adaptation_field_control = 0x20;
    *ptr++ = (adaptation_field_control | (padding ? 0x30 : 0x10)) | continuity_counter;
	if (!padding && stream_id == 0xe0) 	// video 
	{
		uint64_t PCR = timeUs * 27;  // PCR based on a 27MHz clock
		uint64_t PCR_base = PCR / 300;
		uint32_t PCR_ext = PCR % 300;
		*ptr++ = 0x07;  // adaptation_field_length
		*ptr++ = 0x10;
		*ptr++ = (PCR_base >> 25) & 0xff;
		*ptr++ = (PCR_base >> 17) & 0xff;
		*ptr++ = (PCR_base >> 9) & 0xff;
		*ptr++ = ((PCR_base & 1) << 7) | 0x7e | ((PCR_ext >> 8) & 1);
		*ptr++ = (PCR_ext & 0xff);
		*ptr++ = 0x00;
	}
#else /* else of ADD_PCR_IN_AF */
    *ptr++ = (padding ? 0x30 : 0x10) | continuity_counter;
#endif /* end of ADD_PCR_IN_AF */
    if (padding) {
        int paddingSize = 188 - accessUnit->size() - 18;
        *ptr++ = paddingSize - 1;
        if (paddingSize >= 2) {
            *ptr++ = 0x00;
            ptr += paddingSize - 2;
        }
    }
    *ptr++ = 0x00;
    *ptr++ = 0x00;
    *ptr++ = 0x01;
    *ptr++ = stream_id;
    *ptr++ = PES_packet_length >> 8;
    *ptr++ = PES_packet_length & 0xff;
    *ptr++ = 0x84;
    *ptr++ = hasPts ? 0x80 : 0x00;
    *ptr++ = hasPts ? 0x05 : 0x00;
    if(hasPts){
        *ptr++ = 0x20 | (((PTS >> 30) & 7) << 1) | 1;
        *ptr++ = (PTS >> 22) & 0xff;
        *ptr++ = (((PTS >> 15) & 0x7f) << 1) | 1;
        *ptr++ = (PTS >> 7) & 0xff;
        *ptr++ = ((PTS & 0x7f) << 1) | 1;
    }

#if (defined(USED_VIDEO_PCRPID) || defined(USED_ALONE_PCRPID))
	if (hasPts && (stream_id == 0xe0 || mSources.size() < 2)) 	// video or only audio
	{
		sp<ABuffer> pcrBuffer = new ABuffer(188);
		//uint64_t PCR = systemTime() * 27000000LL / 1000000000LL;
		uint64_t PCR = timeUs * 27000000LL / 1000000LL; // PCR based on a 27MHz clock
		uint64_t PCR_base = PCR / 300;
		uint32_t PCR_ext = PCR % 300;

		memset(pcrBuffer->data(), 0xff, pcrBuffer->size());
		uint8_t *ptr = pcrBuffer->data();
		*ptr++ = 0x47;
		*ptr++ = 0x40 | (kPID_PCR >> 8);
		*ptr++ = kPID_PCR & 0xff;
		*ptr++ = 0x20 | pcr_continuity_counter;
		*ptr++ = 0xb7;  // adaptation_field_length
		*ptr++ = 0x10;
		*ptr++ = (PCR_base >> 25) & 0xff;
		*ptr++ = (PCR_base >> 17) & 0xff;
		*ptr++ = (PCR_base >> 9) & 0xff;
		*ptr++ = ((PCR_base & 1) << 7) | 0x7e | ((PCR_ext >> 8) & 1);
		*ptr++ = (PCR_ext & 0xff);
		if (mRtp)
			convertRtp(pcrBuffer, pcrBuffer->size());
		else
			CHECK_EQ_2(internalWrite(pcrBuffer->data(), pcrBuffer->size()), pcrBuffer->size());
	}
#endif /* end of (defined(USED_VIDEO_PCRPID) || defined(USED_ALONE_PCRPID)) */

#ifdef WRITE_DATA_FROM_LARGE_BUFFER
    size_t sizeLeft = buffer->data() + 188 - ptr;
#else /* else of WRITE_DATA_FROM_LARGE_BUFFER */
    size_t sizeLeft = buffer->data() + buffer->size() - ptr;
#endif /* end of WRITE_DATA_FROM_LARGE_BUFFER */
    size_t copy = accessUnit->size();
    if (copy > sizeLeft) {
        copy = sizeLeft;
    }

    memcpy(ptr, accessUnit->data(), copy);
#ifdef WRITE_DATA_FROM_LARGE_BUFFER
    size_t offset = copy;
	size_t needReturnSize = 188;
	size_t currentNumber = 1;
	
    while (offset < accessUnit->size()) {
        bool lastAccessUnit = ((accessUnit->size() - offset) < 184);
        // for subsequent fragments of "buffer":
        // 0x47
        // transport_error_indicator = b0
        // payload_unit_start_indicator = b0
        // transport_priority = b0
        // PID = b0 0001 1110 ???? (13 bits) [0x1e0 + 1 + sourceIndex]
        // transport_scrambling_control = b00
        // adaptation_field_control = b??
        // continuity_counter = b????
        // the fragment of "buffer" follows.
        const unsigned continuity_counter =
            mSources.editItemAt(sourceIndex)->incrementContinuityCounter();

        ptr = buffer->data() + currentNumber*188;
        *ptr++ = 0x47;
        *ptr++ = 0x00 | (PID >> 8);
        *ptr++ = PID & 0xff;
        *ptr++ = (lastAccessUnit ? 0x30 : 0x10) | continuity_counter;


        if (lastAccessUnit) {
            // Pad packet using an adaptation field
            // Adaptation header all to 0 execpt size
            uint8_t paddingSize = (uint8_t)184 - (accessUnit->size() - offset);
            *ptr++ = paddingSize - 1;
            if (paddingSize >= 2) {
                *ptr++ = 0x00;
                ptr += paddingSize - 2;
            }
			copy = accessUnit->size() - offset;
        }
		else
			copy = 184;

		currentNumber++;
		needReturnSize += 188;

		memcpy(ptr, accessUnit->data() + offset, copy);
        offset += copy;
		if (currentNumber == MAX_WRITE2SOCKET_COUNTER || lastAccessUnit)
		{
			// over MAX_WRITE2SOCKET_COUNTER write data or lastAccessUnit
			if (mRtp)
				convertRtp(buffer, needReturnSize);
			else
				CHECK_EQ_2(internalWrite(buffer->data(), needReturnSize), needReturnSize);
			currentNumber = 0;
			needReturnSize = 0;
			memset(buffer->data(), 0xff, buffer->size());
		}
    }
	
	if (needReturnSize > 0)
	{
		if (mRtp)
			convertRtp(buffer, needReturnSize);
		else
			CHECK_EQ_2(internalWrite(buffer->data(), needReturnSize), needReturnSize);
	}
#else /* else of WRITE_DATA_FROM_LARGE_BUFFER */

	if (mRtp)
		convertRtp(buffer, buffer->size());
	else
		CHECK_EQ_2(internalWrite(buffer->data(), buffer->size()), buffer->size());

    size_t offset = copy;
    while (offset < accessUnit->size()) {
        bool lastAccessUnit = ((accessUnit->size() - offset) < 184);
        // for subsequent fragments of "buffer":
        // 0x47
        // transport_error_indicator = b0
        // payload_unit_start_indicator = b0
        // transport_priority = b0
        // PID = b0 0001 1110 ???? (13 bits) [0x1e0 + 1 + sourceIndex]
        // transport_scrambling_control = b00
        // adaptation_field_control = b??
        // continuity_counter = b????
        // the fragment of "buffer" follows.

        memset(buffer->data(), 0xff, buffer->size());

        const unsigned continuity_counter =
            mSources.editItemAt(sourceIndex)->incrementContinuityCounter();

        ptr = buffer->data();
        *ptr++ = 0x47;
        *ptr++ = 0x00 | (PID >> 8);
        *ptr++ = PID & 0xff;
        *ptr++ = (lastAccessUnit ? 0x30 : 0x10) | continuity_counter;

        if (lastAccessUnit) {
            // Pad packet using an adaptation field
            // Adaptation header all to 0 execpt size
            uint8_t paddingSize = (uint8_t)184 - (accessUnit->size() - offset);
            *ptr++ = paddingSize - 1;
            if (paddingSize >= 2) {
                *ptr++ = 0x00;
                ptr += paddingSize - 2;
            }
        }

        size_t sizeLeft = buffer->data() + buffer->size() - ptr;
        size_t copy = accessUnit->size() - offset;
        if (copy > sizeLeft) {
            copy = sizeLeft;
        }

        memcpy(ptr, accessUnit->data() + offset, copy);
		if (mRtp)
			convertRtp(buffer, buffer->size());
		else
			CHECK_EQ_2(internalWrite(buffer->data(), buffer->size()),
					buffer->size());

        offset += copy;
    }
#endif /* end of WRITE_DATA_FROM_LARGE_BUFFER */
}

void MPEG2TSWriter::writeTS() {
	mNumTSPacketsWritten++; //20131017 charleslin
	//ALOGV("mNumTSPacketsWritten:%lld mNumTSPacketsBeforeMeta:%lld", mNumTSPacketsWritten, mNumTSPacketsBeforeMeta);
    if (mNumTSPacketsWritten >= mNumTSPacketsBeforeMeta) {
        writeProgramAssociationTable();
        writeProgramMap();

        mNumTSPacketsBeforeMeta = mNumTSPacketsWritten + 10; //2500 //20131017 charleslin
    }
}

void MPEG2TSWriter::initCrcTable() {
    uint32_t poly = 0x04C11DB7;

    for (int i = 0; i < 256; i++) {
        uint32_t crc = i << 24;
        for (int j = 0; j < 8; j++) {
            crc = (crc << 1) ^ ((crc & 0x80000000) ? (poly) : 0);
        }
        mCrcTable[i] = crc;
    }
}

/**
 * Compute CRC32 checksum for buffer starting at offset start and for length
 * bytes.
 */
uint32_t MPEG2TSWriter::crc32(const uint8_t *p_start, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t *p;

    for (p = p_start; p < p_start + length; p++) {
        crc = (crc << 8) ^ mCrcTable[((crc >> 24) ^ *p) & 0xFF];
    }

    return crc;
}

ssize_t MPEG2TSWriter::internalWrite(const void *data, size_t size) {
    if (mFile != NULL) {
        return fwrite(data, 1, size, mFile);
    }

	if (mSocket > 0)
	{
		ssize_t n = sendto(
				mSocket, data, size, 0,
				(const struct sockaddr *)(&mUDPAddr),
				sizeof(mUDPAddr));
		CHECK_EQ(n, size);
#if 0//defined(__LINARO_SDK__)
		return n;
#else
		if (mBufferData == NULL && mBufferInfo == NULL)
			return n;
#endif
	}

#if 1//!defined(__LINARO_SDK__)
	if (mBufferData != NULL && mBufferInfo != NULL) {
        struct BufferInfo_t *info = (struct BufferInfo_t *)mBufferInfo->pointer();
        char *buf = (char *)mBufferData->pointer();
        size_t remain = info->size - info->writePtr;

#ifdef CHECK_RINGBUFFER_OVERWRITE
        size_t space = (info->size + info->readPtr - info->writePtr - 1) % info->size;
        //ALOGD("space:%x size:%x", space, size);
        if(size > space)
            ALOGW("ringBuffer overwrite!! writePtr:%x readPtr:%x frame:%x space:%x size:%x", info->writePtr, info->readPtr, size, space, info->size);
#endif

		if (mRtp)
		{
			int *newbuf = (int *)(buf+info->writePtr);
			newbuf[0] = size ;
			
			if(remain < size){
				memcpy(buf + info->writePtr + 4 , data, remain);
				memcpy(buf, (char *)data + remain, size - remain);
			}else{
				memcpy(buf + info->writePtr + 4, data, size);
			}
			info->writePtr = (info->writePtr + RTP_OVER_ASHMEM_LENGTH) % info->size;
		}
		else
		{
			if(remain < size){
				memcpy(buf + info->writePtr, data, remain);
				memcpy(buf, (char *)data + remain, size - remain);
			}else{
				memcpy(buf + info->writePtr, data, size);
			}
			info->writePtr = (info->writePtr + size) % info->size;
		}
        ALOGV("size:%d writePtr:%u", (int)size, info->writePtr);
        return size;
	}

#endif
    return (*mWriteFunc)(mWriteCookie, data, size);
}

ssize_t MPEG2TSWriter::convertRtp(const sp<ABuffer> &tsBuffer, size_t size) {

	sp<ABuffer> buffer = new ABuffer( 12 + size);
	uint8_t *rtp = buffer->data();
	//static const bool kMarkerBit = false;
	rtp[0] = 0x80;
	rtp[1] = 33 ;  // M-bit, | (kMarkerBit ? (1 << 7) : 0);
	rtp[2] = (mRTPSeqNo >> 8) & 0xff;
	rtp[3] = mRTPSeqNo & 0xff;
	rtp[4] = 0x00;  // rtp time to be filled in later.
	rtp[5] = 0x00;
	rtp[6] = 0x00;
	rtp[7] = 0x00;
	rtp[8] = kSourceID >> 24;
	rtp[9] = (kSourceID >> 16) & 0xff;
	rtp[10] = (kSourceID >> 8) & 0xff;
	rtp[11] = kSourceID & 0xff;

	++mRTPSeqNo;

	memcpy(buffer->data() + 12, tsBuffer->data(), size);

	CHECK_EQ_2(internalWrite(buffer->data(), buffer->size()), buffer->size());
	return buffer->size();
}

}  // namespace android

