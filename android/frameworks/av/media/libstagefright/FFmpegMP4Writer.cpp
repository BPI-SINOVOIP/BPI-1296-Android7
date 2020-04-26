/*
 * Copyright (c) 2015 Realtek Semiconductor Corp.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "FFmpegMP4Writer"
#include <utils/Log.h>

#if defined(__LINARO_SDK__)
#include "ADebug.h"
#include "ABuffer.h"
#include "AMessage.h"
#include "hexdump.h"
// for NAS toolchain
#ifndef   UINT64_C
#define   UINT64_C(value)__CONCAT(value,ULL)
#endif
#else
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/AMessage.h>
#include <RtkIpc/IRecordBuffer.h>
#endif

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/foundation/AHandlerReflector.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaWriter.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <binder/MemoryBase.h>
#include <utils/CallStack.h>
#include "include/ESDS.h"

extern "C" {
//#include <ffmpeg.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include <media/stagefright/FFmpegMP4Writer.h>

#define IO_BUFFER_SIZE 32768

#define FCC2ASCII(fmt)  ((char *)(&fmt))[3], ((char *)(&fmt))[2], ((char *)(&fmt))[1], ((char *)(&fmt))[0]

namespace android {

FFmpegMP4Writer::EsTrack::EsTrack(const sp<MediaSource> &source)
    : mSource(source),
      mLooper(new ALooper),
      mEOSReached(true),
      mTrackIndex(-1),
      mStartTime(-1)
{
    ALOGV("EsTrack:%d", __LINE__);
    sp<MetaData> meta = mSource->getFormat();
    //meta->dumpToLog();
    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        mIsAudio = true;
        mLooper->setName("EsTrack-audio");
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
        mIsAudio = false;
        mLooper->setName("EsTrack-video");
    } else {
        TRESPASS();
    }
}

FFmpegMP4Writer::EsTrack::~EsTrack()
{
    ALOGV("~EsTrack:%d", __LINE__);

}

void FFmpegMP4Writer::EsTrack::start(const sp<AMessage> &notify)
{
    ALOGV("EsTrack::start:%d", __LINE__);
    mEOSReached = false;
    mLooper->registerHandler(this);
    mLooper->start();

    CHECK(notify != NULL);
    mNotify = notify;
    CHECK(mNotify->findInt32("track-index", &mTrackIndex));

    sp<AMessage> msg = new AMessage(kWhatStart, this);
    msg->post();
}

void FFmpegMP4Writer::EsTrack::stop()
{
    ALOGV("EsTrack::stop:%d", __LINE__);
    mEOSReached = true;
    mLooper->unregisterHandler(id());
    mLooper->stop();

    mSource->stop();
}

bool FFmpegMP4Writer::EsTrack::isAudio() const
{
    return mIsAudio;
}

bool FFmpegMP4Writer::EsTrack::isEOSReached()
{
    return mEOSReached;
}

int FFmpegMP4Writer::EsTrack::getTrackIndex()
{
    return mTrackIndex;
}

int64_t FFmpegMP4Writer::EsTrack::getStartTime()
{
    return mStartTime;
}

sp<MetaData> FFmpegMP4Writer::EsTrack::getFormat()
{
    return mSource->getFormat();
}

sp<ABuffer> FFmpegMP4Writer::EsTrack::getCodecSpecificData()
{
    return mCsd;
}

void FFmpegMP4Writer::EsTrack::readMore()
{
    sp<AMessage> msg = new AMessage(kWhatRead, this);
    msg->post();
}

void FFmpegMP4Writer::EsTrack::onMessageReceived(const sp<AMessage> &msg)
{
    unsigned int what = msg->what();
    ALOGV("EsTrack::onMessageReceived:%d what:%c%c%c%c", __LINE__, FCC2ASCII(what));
    switch (what) {
    case kWhatStart:
    {
        status_t err = mSource->start();
        if (err != OK) {
            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("event", kNotifyStartFailed);
            notify->post();
            break;
        }

        extractCodecSpecificData();

        readMore(); //start reading data

        break;
    }
    case kWhatRead:
    {
        int isCodecConfig = 0;
        int isSyncFrame = 0;
        MediaBuffer *buffer = NULL;

        status_t err = mSource->read(&buffer);

        if(err != OK || buffer == NULL){
            mEOSReached = true;
            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("event", kNotifyReachedEOS);
            notify->setInt32("status", err);
            notify->post();
            break;
        }

        buffer->meta_data()->findInt32(kKeyIsCodecConfig, &isCodecConfig);
        buffer->meta_data()->findInt32(kKeyIsSyncFrame, &isSyncFrame);
        //if(!mIsAudio) ALOGD("read %s buffer len:%u isSyncFrame:%d isCodecConfig:%d mCsd:%p", mIsAudio ? "audio" : "video", buffer->range_length(), isSyncFrame, isCodecConfig, mCsd.get());
        if(isCodecConfig){
            CHECK(mCsd == NULL);
            //ALOGD("found %s CSD", mIsAudio ? "audio" : "video");
            //buffer->meta_data()->dumpToLog();
            //hexdump(buffer->data(), buffer->size(), 3, NULL);

            if(mIsAudio){
                //FIXME: what's the real foramt?
                CHECK_GE(buffer->range_length(), 2u); //at least 2 bytes
            }else{
                //expect start with 00 00 00 01
            }

            mCsd = new ABuffer(buffer->range_length());
            memcpy(mCsd->data(), buffer->data(), buffer->range_length());
            buffer->release();
            buffer = NULL;

            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("event", kNotifyCodecSpecificData);
            notify->post();

            //don't readMore(), waiting FFmpegMP4Writer to startMP4Muxer

        }else if(buffer->range_length() > 0){  //got normal frame
            int64_t timeUs = -1;
            CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));
            if(mStartTime == -1 && timeUs != -1){
                mStartTime = timeUs;
                ALOGD("%s got start time: %.3f", mIsAudio ? "audio" : "video", mStartTime / 1000000.0);
            }
#if 0
            if(timeUs < mPrevTimeUs){
                ALOGE("\033[31mnon monotonically increasing timeUs:%.3f mPrevTimeUs:%.3f", timeUs, mPrevTimeUs);
                buffer->release();
                buffer = NULL;
            }else
#endif
            {
                sp<AMessage> notify = mNotify->dup();
                notify->setInt32("event", kNotifyBuffer);
                notify->setPointer("buffer", buffer);   //FFmpegMP4Writer need to release this buffer
                notify->post();
            }
            readMore();
        }

        break;
    }
    default:
        ALOGE("invalid msg:%s", msg->debugString().c_str());
        TRESPASS();
    }
}

void FFmpegMP4Writer::EsTrack::extractCodecSpecificData()
{
    ALOGV("EsTrack::extractCodecSpecificData:%d", __LINE__);
    const char *mime;
    sp<MetaData> meta = mSource->getFormat();
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {  //expect start with 00 00 00 01
        uint32_t type;
        const void *data;
        size_t size;
        if (meta->findData(kKeyAVCC, &type, &data, &size)) {
            //ALOGD("extractCodecSpecificData for %s", mIsAudio ? "audio" : "video");
            //meta->dumpToLog();
            mCsd = new ABuffer(size);
            memcpy(mCsd->data(), data, size);
        }
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        uint32_t type;
        const void *data;
        size_t size;
        if (meta->findData(kKeyESDS, &type, &data, &size)) {
            //ALOGD("extractCodecSpecificData for %s", mIsAudio ? "audio" : "video");
            //meta->dumpToLog();
            ESDS esds(data, size);
            if (esds.getCodecSpecificInfo(&data, &size) == OK) {
                mCsd = new ABuffer(size);
                memcpy(mCsd->data(), data, size);
            }
        }
    }
}

//=======================================================================
FFmpegMP4Writer::FFmpegMP4Writer(const char *filename)
{
    ALOGV("FFmpegMP4Writer:%d", __LINE__);
    resetVariables();
    mFile = fopen(filename, "wb");
    init();
}

FFmpegMP4Writer::FFmpegMP4Writer(int fd)
{
    ALOGV("FFmpegMP4Writer:%d", __LINE__);
    resetVariables();
    mFile = fdopen(dup(fd), "wb");
    init();
}

FFmpegMP4Writer::FFmpegMP4Writer(FILE *f_fd)
{
    ALOGV("FFmpegMP4Writer:%d", __LINE__);
    resetVariables();
    mFile = f_fd;
    init();
}

FFmpegMP4Writer::FFmpegMP4Writer(void *cookie, ssize_t (*write)(void *cookie, const void *data, size_t size))
{
    ALOGV("FFmpegMP4Writer:%d", __LINE__);
    resetVariables();
    mWriteCookie = cookie;
    mWriteFunc = write;
    init();
}

#if !defined(__LINARO_SDK__)
FFmpegMP4Writer::FFmpegMP4Writer(sp<IMemory> data, sp<IMemory> info)
{
    ALOGV("FFmpegMP4Writer:%d", __LINE__);
    resetVariables();
    mBufferData = data;
    mBufferInfo = info;
    ALOGD("data:%p:%d info:%p:%d", mBufferData->pointer(), mBufferData->size(), mBufferInfo->pointer(), mBufferInfo->size());
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
    init();
}
#endif

FFmpegMP4Writer::~FFmpegMP4Writer()
{
    ALOGV("~FFmpegMP4Writer:%d", __LINE__);
    if(mStarted){
        stop();
        mStarted = false;
    }
}

void FFmpegMP4Writer::resetVariables()
{
    ALOGV("resetVariables:%d", __LINE__);
    mStarted = false;

    mFile = NULL;
    mWriteCookie = NULL;
    mWriteFunc = NULL;
#if !defined(__LINARO_SDK__)
    mBufferData = NULL;
    mBufferInfo = NULL;
#endif
    mLooper = NULL;
    mReflector = NULL;
    mFormatContext = NULL;
    mMP4MuxerStarted = false;
    mStartTime = -1;

    mWidth = 0;
    mHeight = 0;
    mSampleRate = 0;
    mChannelCount = 0;
}

void FFmpegMP4Writer::init()
{
    ALOGV("init:%d", __LINE__);
#if defined(__LINARO_SDK__)
    CHECK(mFile != NULL || mWriteFunc != NULL);
#else
    CHECK(mFile != NULL || mWriteFunc != NULL || (mBufferData != NULL && mBufferInfo != NULL));
#endif

    mLooper = new ALooper;
    mLooper->setName(LOG_TAG);

    mReflector = new AHandlerReflector<FFmpegMP4Writer>(this);

    mLooper->registerHandler(mReflector);
    mLooper->start();
}

extern "C" int write_packet(void *opaque, uint8_t *buf, int size)
{
    ALOGV("write_packet:%d buf:%p size:%d", __LINE__, buf, size);
    FFmpegMP4Writer *writer = (FFmpegMP4Writer *)opaque;
    return writer->writePacket(buf, size);
}

int FFmpegMP4Writer::writePacket(uint8_t *data, int size)
{
    //ALOGV("writePacket:%d", __LINE__);
    if(mFile != NULL)
    {
        return fwrite(data, size, 1, mFile);
    }
    else if(mWriteFunc)
    {
        return (*mWriteFunc)(mWriteCookie, data, size);
    }
#if !defined(__LINARO_SDK__)
    else if (mBufferData != NULL && mBufferInfo != NULL)
    {
        struct BufferInfo_t *info = (struct BufferInfo_t *)mBufferInfo->pointer();
        char *buf = (char *)mBufferData->pointer();
        int remain = info->size - info->writePtr;

#ifdef CHECK_RINGBUFFER_OVERWRITE
        size_t space = (info->size + info->readPtr - info->writePtr - 1) % info->size;
        //ALOGD("space:%x size:%x", space, size);
        if(size > space)
            ALOGW("ringBuffer overwrite!! writePtr:%x readPtr:%x frame:%x space:%x size:%x", info->writePtr, info->readPtr, size, space, info->size);
#endif

        if(remain < size){
            memcpy(buf + info->writePtr, data, remain);
            memcpy(buf, (char *)data + remain, size - remain);
        }else{
            memcpy(buf + info->writePtr, data, size);
        }
        info->writePtr = (info->writePtr + size) % info->size;
        ALOGV("size:%d writePtr:%u", size, info->writePtr);
        return size;
    }
#endif
    return 0;
}

status_t FFmpegMP4Writer::addSource(const sp<MediaSource> &source)
{
    ALOGV("addSource:%d", __LINE__);
    CHECK(mStarted == false);

    sp<MetaData> meta = source->getFormat();
    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)
            && strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        return ERROR_UNSUPPORTED;
    }

    sp<EsTrack> track = new EsTrack(source);
    mTracks.push(track);
    return OK;
}

status_t FFmpegMP4Writer::start(MetaData * /*params*/)
{
    ALOGV("start:%d", __LINE__);
    CHECK(mStarted == false);
    mStarted = true;

    //start all tracks
    for (size_t i = 0; i < mTracks.size(); ++i) {
        //prepare msg for EsTrack notifying FFmpegMP4Writer
        sp<AMessage> notify = new AMessage(kWhatSourceNotify, mReflector);
        notify->setInt32("track-index", i);
        mTracks.editItemAt(i)->start(notify);
    }

    return OK;
}

status_t FFmpegMP4Writer::stop()
{
    ALOGV("stop:%d", __LINE__);
    CHECK(mStarted == true);
    mStarted = false;

    for (size_t i = 0; i < mTracks.size(); ++i) {
        mTracks.editItemAt(i)->stop();
    }

    mLooper->unregisterHandler(mReflector->id());
    mLooper->stop();

    stopMP4Muxer();

    if(mFile != NULL){
        fclose(mFile);
        mFile = NULL;
    }

#if !defined(__LINARO_SDK__)
    if (mBufferData != NULL && mBufferInfo != NULL) {
        struct BufferInfo_t *info = (struct BufferInfo_t *)mBufferInfo->pointer();
        info->endOfStream = true;
    }
#endif

    return OK;
}

status_t FFmpegMP4Writer::pause()
{
    ALOGV("pause:%d", __LINE__);
    CHECK(mStarted == true);
    return INVALID_OPERATION;
}

bool FFmpegMP4Writer::reachedEOS()
{
    if(mStarted == false)
        return true;

    bool allEOSReached = true;
    for(size_t i = 0; i < mTracks.size(); i++){
        sp<EsTrack> &track = mTracks.editItemAt(i);
        if(track->isEOSReached() == false){
            allEOSReached = false;
            break;
        }
    }
    return allEOSReached;
}

#if 0
status_t FFmpegMP4Writer::dump(int fd, const Vector<String16>& args)
{
    ALOGV("dump:%d", __LINE__);
    return OK;
}
#endif

void FFmpegMP4Writer::onMessageReceived(const sp<AMessage> &msg)
{
    //ALOGV("onMessageReceived:%d", __LINE__);
    unsigned int what = msg->what();
    switch (what) {
        case kWhatSourceNotify:
        {
            int trackIndex;
            CHECK(msg->findInt32("track-index", &trackIndex));

            int event;
            CHECK(msg->findInt32("event", &event));
            ALOGV("onMessageReceived:%d what:%c%c%c%c what:%d", __LINE__, FCC2ASCII(what), event);
            switch(event){
                case EsTrack::kNotifyReachedEOS:
                case EsTrack::kNotifyStartFailed:
                {
                    if(reachedEOS()){
                        //status_t ret = stopMP4Muxer();    //don' stop MP4 muxer here, or it may be called twice
                    }
                    break;
                }
                case EsTrack::kNotifyCodecSpecificData:
                {
                    bool allCsdReady = true;
                    sp<EsTrack> videoTrack;
                    sp<EsTrack> audioTrack;

                    for(size_t i = 0; i < mTracks.size(); i++){
                        sp<EsTrack> &track = mTracks.editItemAt(i);
                        if(track->getCodecSpecificData() == NULL){
                            allCsdReady = false;
                            break;
                        }
                        if(track->isAudio())
                            audioTrack = track;
                        else
                            videoTrack = track;
                    }

                    //Tracks are waiting start MP4 Muxer, now tell them to read more
                    if(allCsdReady){
                        startMP4Muxer(audioTrack, videoTrack);
                        for(size_t i = 0; i < mTracks.size(); i++){
                            sp<EsTrack> &track = mTracks.editItemAt(i);
                            track->readMore();
                        }
                    }
                    break;
                }
                case EsTrack::kNotifyBuffer:
                {
                    MediaBuffer *buffer = NULL;
                    CHECK(msg->findPointer("buffer", (void **)&buffer));
                    writeFrameToMP4Muxer(trackIndex, buffer);
                    break;
                }
                default:
                    ALOGE("invalid notification:%s", msg->debugString().c_str());
                    TRESPASS();
            }
            break;
        }
        default:
            ALOGE("invalid msg:%s", msg->debugString().c_str());
            TRESPASS();
    }
}

status_t FFmpegMP4Writer::writeFrameToMP4Muxer(int trackIndex, MediaBuffer *buffer)
{
    CHECK(mMP4MuxerStarted == true);

    while(mStartTime == -1){
        bool allStarted = true;
        int64_t minStartTime = -1;
        for (size_t i = 0; i < mTracks.size(); ++i) {
            int64_t startTime = mTracks[i]->getStartTime();
            if(startTime == -1){
                allStarted = false;
                break;
            }
            if(minStartTime == -1 || startTime < minStartTime)
                minStartTime = startTime;
        }
        if(allStarted){
            mStartTime = minStartTime;
            break;
        }
        ALOGV("waiting for all tracks started");
        usleep(2000);
    }

    sp<MetaData> meta = buffer->meta_data();
    int isSyncFrame = 0;
    meta->findInt32(kKeyIsSyncFrame, &isSyncFrame);
    int64_t timeUs = -1;
    CHECK(meta->findInt64(kKeyTime, &timeUs));
    if(timeUs != -1)
        timeUs -= mStartTime;

    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.stream_index = trackIndex;
    pkt.data = (uint8_t *)buffer->data();
    pkt.size = buffer->range_length();
    if(isSyncFrame)
        pkt.flags |= AV_PKT_FLAG_KEY;
    if(timeUs == -1)
        pkt.pts = AV_NOPTS_VALUE;
    else if(mTracks[trackIndex]->isAudio())
        pkt.pts = timeUs * mSampleRate / 1000000;  //convert from microsecond to sample rate
    else
        pkt.pts = timeUs;
    pkt.dts = AV_NOPTS_VALUE;

    //if(trackIndex == 0)
    //    ALOGD("writeFrameToMP4Muxer: index:%d data:%p size:%d flags:%x time:%.3f pts:%lld", pkt.stream_index, pkt.data, pkt.size, pkt.flags, timeUs/1000000.0, pkt.pts);

    int ret = av_interleaved_write_frame((AVFormatContext *)mFormatContext, &pkt);
    //int ret = av_write_frame((AVFormatContext *)mFormatContext, &pkt);
    //CHECK(ret == 0);
    if(ret != 0)
        ALOGE("\033[33mfailed to write frame: ret:%d\033[m", ret);

    buffer->release();
    buffer = NULL;
    return OK;
}

status_t FFmpegMP4Writer::startMP4Muxer(sp<EsTrack> audioTrack, sp<EsTrack> videoTrack)
{
    ALOGV("startMP4Muxer:%d", __LINE__);
    int ret = OK;
    //unsigned char *buf = NULL;
    AVFormatContext *ctx = NULL;
    AVStream *vst = NULL;
    AVStream *ast = NULL;
    AVCodecContext *codec;
    AVDictionary *opt = NULL;
    sp<ABuffer> csd;
    sp<MetaData> meta;

    CHECK(mMP4MuxerStarted == false);

    av_register_all();

    ctx = avformat_alloc_context();
    CHECK(ctx != NULL);

    ctx->oformat = av_guess_format("mp4", NULL, NULL);
    CHECK(ctx->oformat != NULL);

    if(ctx->oformat->priv_data_size > 0){
        //priv_data
        ctx->priv_data = av_mallocz(ctx->oformat->priv_data_size);
        CHECK(ctx->priv_data != NULL);

        //priv_class
        if(ctx->oformat->priv_class){
            *(const AVClass**)ctx->priv_data = ctx->oformat->priv_class;
            av_opt_set_defaults(ctx->priv_data);
        }
    }else{
        ctx->priv_data = NULL;
    }

    //make sure the audio/video stream index & track index are sync
    if(audioTrack->getTrackIndex() == 0){
        ast = avformat_new_stream(ctx, NULL);
        vst = avformat_new_stream(ctx, NULL);
    }else{
        vst = avformat_new_stream(ctx, NULL);
        ast = avformat_new_stream(ctx, NULL);
    }

    CHECK(vst != NULL);
    CHECK(ast != NULL);

    //vst->id = 1;
    codec = vst->codec;
    codec->codec_type = AVMEDIA_TYPE_VIDEO;
    codec->codec_id = AV_CODEC_ID_H264;
    //codec->codec_tag = MKTAG('a','v','c','1');
    codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    codec->pix_fmt =  AV_PIX_FMT_NV12;
    meta = videoTrack->getFormat();
    CHECK(meta != NULL);
    //ALOGD("startMP4Muxer: current %s metadata:", "video");
    //meta->dumpToLog();
    CHECK(meta->findInt32(kKeyWidth, &mWidth));
    CHECK(meta->findInt32(kKeyHeight, &mHeight));
    codec->width = mWidth;
    codec->height = mHeight;
    codec->time_base.num = 1;
    codec->time_base.den = 1000000; //microseconds
    //codec->bit_rate = 1000000;
    //codec->rc_buffer_size = 4096;
    //codec->rc_max_rate = 1500000;
    //codec->sample_aspect_ratio.num = 0;
    //codec->sample_aspect_ratio.den = 0;
    csd = videoTrack->getCodecSpecificData();
    CHECK(csd != NULL);
    codec->extradata = (uint8_t *)av_malloc(csd->size());
    CHECK(codec->extradata != NULL);
    memcpy(codec->extradata, csd->data(), csd->size());
    codec->extradata_size = csd->size();

    //ast->id = 1;
    codec = ast->codec;
    codec->codec_type = AVMEDIA_TYPE_AUDIO;
    codec->codec_id = AV_CODEC_ID_AAC;
    //codec->codec_tag = MKTAG('m', 'p', '4', 'a');
    codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    meta = audioTrack->getFormat();
    CHECK(meta != NULL);
    //ALOGD("startMP4Muxer: current %s metadata:", "audio");
    //meta->dumpToLog();
    CHECK(meta->findInt32(kKeySampleRate, &mSampleRate));
    CHECK(meta->findInt32(kKeyChannelCount, &mChannelCount));
    codec->sample_rate = mSampleRate;
    codec->channels = mChannelCount;
    codec->frame_size = 1024; //FIXME: according to SoftAACEncoder2::kNumSamplesPerFrame
    codec->time_base.num = 1;
    codec->time_base.den = codec->sample_rate; //samplerate
    //codec->bit_rate = 50000;
    //codec->frame_size = 0;
    //codec->block_align = 0;
    csd = audioTrack->getCodecSpecificData();
    CHECK(csd != NULL);
    codec->extradata = (uint8_t *)av_malloc(csd->size());
    CHECK(codec->extradata != NULL);
    memcpy(codec->extradata, csd->data(), csd->size());
    codec->extradata_size = csd->size();

    mAvioBuf = (unsigned char *)av_malloc(IO_BUFFER_SIZE);
    CHECK(mAvioBuf);

    ctx->pb = avio_alloc_context(mAvioBuf, IO_BUFFER_SIZE, AVIO_FLAG_WRITE, this, NULL, write_packet, NULL);
    CHECK(ctx->pb != NULL);

    //ctx->pb->direct = 1;  //setting direct flag will flushes buffer frequently, may cause seek fail
    ctx->pb->seekable = 0;
    //ctx->pb->max_packet_size = 0;
    //ctx->pb->read_pause = NULL;
    //ctx->pb->read_seek = NULL;
    //ctx->pb->av_class = NULL;

    //av_dict_set(&opt, "avioflags", "direct", 0);
    //av_dict_set(&opt, "frag_keyframe", "", 0);
    av_dict_set(&opt, "movflags", "frag_keyframe", 0);
#if defined(__LINARO_SDK__)//tandy: don't know why, in linux, without set these opt, the bit rate of mp4 always be 1kb and need too long time to load and play.
    av_dict_set(&opt, "frag_duration", "2000000", 0);
    av_dict_set(&opt, "min_frag_duration", "1000000", 0);
    //av_dict_set(&opt, "movflags", "faststart", 0);//conflict with frag_keyframe
    //av_dict_set(&opt, "movflags", "empty_moov", 0);
    //av_dict_set(&opt, "movflags", "separate_moof", 0);
    //av_dict_set(&opt, "movflags", "isml", 0);
#endif

    ret = avformat_write_header(ctx, &opt);
    CHECK(ret == 0);

    mFormatContext = ctx;
    mMP4MuxerStarted = true;
    return ret;
}

status_t FFmpegMP4Writer::stopMP4Muxer()
{
    ALOGV("stopMP4Muxer:%d", __LINE__);
    mMP4MuxerStarted = false;

    AVFormatContext *ctx = (AVFormatContext *)mFormatContext;
    if(ctx != NULL){
        av_write_trailer(ctx);
        //for(unsigned int i = 0; i < ctx->nb_streams; i++)
        //    av_freep(&ctx->streams[i]);
        av_freep(&ctx->pb);
        avformat_free_context(ctx);
    }

    if(mAvioBuf != NULL)
        av_freep(&mAvioBuf);

    return OK;
}

}  // namespace android
