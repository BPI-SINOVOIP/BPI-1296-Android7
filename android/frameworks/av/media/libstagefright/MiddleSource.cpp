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
#include <fcntl.h>

#define LOG_TAG "MiddleSource"
#include <utils/Log.h>

#include <media/stagefright/MiddleSource.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/foundation/ADebug.h>
#include "include/AwesomePlayer.h"
#include "include/SoftwareRenderer.h"
#define USED_NATIVEWINDOW_METHOD
#ifdef USED_NATIVEWINDOW_METHOD
#define USED_NATIVE_WINDOWS_BY_OMXCODEC
#endif /* end of USED_NATIVEWINDOW_METHOD */
#ifdef USED_NATIVE_WINDOWS_BY_OMXCODEC
#include <ui/GraphicBuffer.h>
#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>
#define USED_LOCK_ONLY
#endif /* end of USED_NATIVE_WINDOWS_BY_OMXCODEC */
#define USED_AUDIO_RENDER
#ifdef USED_AUDIO_RENDER
#define USED_AUDIO_CALLBACK		// Audio render used callback method
#include <media/AudioTrack.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <utils/Thread.h>
#define DEFAULT_TIMEOUT	200	// 200 * 0.01 => 2sec
#endif /* end of USED_AUDIO_RENDER */
#include <utils/Statistics.h>

#define HANDLE_YV12_TO_NV12
#ifdef HANDLE_YV12_TO_NV12
#define ROUNDUP(x, base)        ((x + base - 1) & ~(base - 1))
#define ROUNDUP16(x)            ROUNDUP(x, 16)
#endif /* end of HANDLE_YV12_TO_NV12 */

namespace android {

struct MiddleSource::AwesomeNativeWindowRenderer : public AwesomeRenderer {

public:
    AwesomeNativeWindowRenderer(
            const sp<ANativeWindow> &nativeWindow, const sp<MetaData> &meta, int32_t rotationDegrees, bool usedNativeWindow)
		: mNativeWindow(nativeWindow),
		  mUsedNativeWindow(usedNativeWindow),
		  mTarget(new SoftwareRenderer(nativeWindow, meta)) {
		applyRotation(rotationDegrees);
    }

    virtual void render(MediaBuffer *buffer) {
		int64_t timeUs;
		CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));

		if (mUsedNativeWindow)
		{
			native_window_set_buffers_timestamp(mNativeWindow.get(), timeUs * 1000);
			//ALOGV("timeUs = %.3f  mVideoBuffer->range_length()=%d\n", (float)timeUs/1000000.0, buffer->range_length());

			status_t err = mNativeWindow->queueBuffer(mNativeWindow.get(), buffer->graphicBuffer().get(), -1);
			if (err != 0) {
				ALOGE("queueBuffer failed with error %s (%d)", strerror(-err),
						-err);
				return;
			}

			sp<MetaData> metaData = buffer->meta_data();
			metaData->setInt32(kKeyRendered, 1);
		}
		else
		{
			render((const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
		}
    }

    void render(const void *data, size_t size) {
        mTarget->render(data, size, NULL);
    }

protected:
    virtual ~AwesomeNativeWindowRenderer() {
        delete mTarget;
        mTarget = NULL;
    }

private:
    sp<ANativeWindow> mNativeWindow;
	bool mUsedNativeWindow;
    SoftwareRenderer *mTarget;

    void applyRotation(int32_t rotationDegrees) {
        uint32_t transform;
        switch (rotationDegrees) {
            case 0: transform = 0; break;
            case 90: transform = HAL_TRANSFORM_ROT_90; break;
            case 180: transform = HAL_TRANSFORM_ROT_180; break;
            case 270: transform = HAL_TRANSFORM_ROT_270; break;
            default: transform = 0; break;
        }

        if (transform) {
            CHECK_EQ(0, native_window_set_buffers_transform(
                        mNativeWindow.get(), transform));
        }
    }

    AwesomeNativeWindowRenderer(const AwesomeNativeWindowRenderer &);
    AwesomeNativeWindowRenderer &operator=(
            const AwesomeNativeWindowRenderer &);
};

#ifdef USED_AUDIO_RENDER
struct MiddleSource::AudioTrackRenderer : public AudioTrack {

private:
	uint32_t mNumFramesWritten;
	uint32_t mPreQueueSize;
	sp<AudioTrack> mAudioTrack;
	sp<ABuffer>	mAudioBuffer;
#ifdef USED_AUDIO_CALLBACK
	static void AudioCallback(int event, void *user, void *info);
	void AudioCallback(int event, void *info);
	size_t fillBuffer(void *data, size_t size);
	Mutex mLock;
	bool mNeedBuffering;
#endif /* end of USED_AUDIO_CALLBACK */
public:
    AudioTrackRenderer(int SampleRate, int AudioChannel, int QueueBufferSize, int PreQueueSize)
		:mNumFramesWritten(0),
		 mPreQueueSize(PreQueueSize)
	{

		audio_stream_type_t mStreamType = AUDIO_STREAM_MUSIC;
		uint32_t afFrameCount = 0;
		uint32_t afSampleRate = 0;
		int frameCount = 0;
		int mSessionId = AudioSystem::newAudioSessionId();
		AudioSystem::acquireAudioSessionId(mSessionId);
		if(AudioSystem::getOutputFrameCount(&afFrameCount, mStreamType) != NO_ERROR) {
			ALOGI("[Audio]getOutputFrameCount ERROR\n");
		}
		if(AudioSystem::getOutputSamplingRate(&afSampleRate, mStreamType) != NO_ERROR) {
			ALOGI("[Audio]getOutputSamplingRate ERROR\n");
		}
		frameCount = (SampleRate * afFrameCount * 8)/afSampleRate;
		ALOGV("---- afFrameCount[%d] afSampleRate [%d] frameCount[%d]-----", afFrameCount, afSampleRate, frameCount);

#ifdef USED_AUDIO_CALLBACK
        mAudioTrack = new AudioTrack(
                AUDIO_STREAM_MUSIC, SampleRate, AUDIO_FORMAT_PCM_16_BIT, audio_channel_out_mask_from_count(AudioChannel),
                frameCount, AUDIO_OUTPUT_FLAG_NONE, &AudioCallback, this, 0);
		mNeedBuffering = true;
#else /* else of USED_AUDIO_CALLBACK */
        mAudioTrack = new AudioTrack(AUDIO_STREAM_MUSIC, SampleRate, AUDIO_FORMAT_PCM_16_BIT, audio_channel_out_mask_from_count(AudioChannel), frameCount);
#endif /* end of USED_AUDIO_CALLBACK */
		mAudioBuffer = new ABuffer(QueueBufferSize);
		mAudioBuffer->setRange(0, 0);
    }

    virtual void render(MediaBuffer *buffer) {
		CHECK(mAudioTrack != NULL);
		if (mAudioTrack->stopped())
			mAudioTrack->start();

#ifdef USED_AUDIO_CALLBACK
        int64_t timeUs;
        CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));
		size_t copy = buffer->range_length();
		size_t availabledSpace = mAudioBuffer->capacity() - mAudioBuffer->size();
		int sleep_count = DEFAULT_TIMEOUT;

		while((availabledSpace == 0 || availabledSpace < copy) && sleep_count > 0)
		{
			//usleep(1000000);	// 1sec
			usleep(10000); // 0.01 sec, 10 ms
			sleep_count--;
			availabledSpace = mAudioBuffer->capacity() - mAudioBuffer->size();
		}
		if (sleep_count != DEFAULT_TIMEOUT && sleep_count < 90) ALOGI("--- Sleep wait [%d] count total sec [%.3f] sec" , DEFAULT_TIMEOUT-sleep_count, (float)(DEFAULT_TIMEOUT-sleep_count) / 100.0);
		if (availabledSpace > 0 && copy <= availabledSpace)
		{
			Mutex::Autolock autoLock(mLock);
			/* Have space to store Audio */
			if (mAudioBuffer->offset() + mAudioBuffer->size() + copy >= mAudioBuffer->capacity())
			{
				ALOGV("#### Remaining space [%d] sample size [%d] offset[%d] size[%d] Total[%d] #####", availabledSpace, buffer->range_length(), mAudioBuffer->offset(),  mAudioBuffer->size(), mAudioBuffer->capacity());
				if (mAudioBuffer->offset() == mAudioBuffer->capacity())
				{
					/* It mean  buffer empty */
					memcpy(mAudioBuffer->base(), (uint8_t *)buffer->data() + buffer->range_offset(), copy);
				}
				else
				{
					/* Need coall memmv to head */
					memmove(mAudioBuffer->base(), mAudioBuffer->data(), mAudioBuffer->size());
				}
				memcpy(mAudioBuffer->base() + mAudioBuffer->size(), (uint8_t *)buffer->data() + buffer->range_offset(), copy);
				mAudioBuffer->setRange(0, mAudioBuffer->size() + copy);
			}
			else
			{
				/* append data */
				memcpy(mAudioBuffer->data() + mAudioBuffer->size(), (uint8_t *)buffer->data() + buffer->range_offset(), copy);
				mAudioBuffer->setRange(mAudioBuffer->offset(), mAudioBuffer->size() + copy);
			}
		}
		else
		{
			ALOGI("--- Lost audio sample [%d] capacity[%d] size[%d] offset[%d] sleep_count[%d]---", buffer->range_length(), mAudioBuffer->capacity(), mAudioBuffer->size(), mAudioBuffer->offset(), sleep_count);
		}

		if (mNeedBuffering && mAudioBuffer->size() >= mPreQueueSize)
			mNeedBuffering = false;
#else /* else of USED_AUDIO_CALLBACK */
		uint32_t numFramesPlayed;
		CHECK_EQ(mAudioTrack->getPosition(&numFramesPlayed), (status_t)OK);

		uint32_t numFramesAvailableToWrite = mAudioTrack->frameCount() - (mNumFramesWritten - numFramesPlayed);

		size_t numBytesAvailableToWrite = numFramesAvailableToWrite * mAudioTrack->frameSize();

        int64_t timeUs;
        CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));
		//ALOGI("---- pts :%.3f ramge_length %d numBytesAvailableToWrite:%d ----", (float)timeUs/1000000.0, buffer->range_length(), numBytesAvailableToWrite);
		size_t copy = buffer->range_length();
		/* TODO, we don't known the Output available space */
		if (copy > numBytesAvailableToWrite) {
			copy = numBytesAvailableToWrite;
		}

		if (copy == 0 || (mAudioBuffer->size() > 0 && copy > 0))
		{
			/* Mean we need store data to mAdioBuffer */
			size_t availabledSpace = mAudioBuffer->capacity() - mAudioBuffer->size();
			if (availabledSpace > 0)
			{
				ALOGV("#### Remaining space [%d] sample size [%d] offset[%d] size[%d] Total[%d] #####", availabledSpace, buffer->range_length(), mAudioBuffer->offset(),  mAudioBuffer->size(), mAudioBuffer->capacity());
				/* Have space to store Audio */
				if (mAudioBuffer->offset() + mAudioBuffer->size() == mAudioBuffer->capacity())
				{
					if (mAudioBuffer->offset() == mAudioBuffer->capacity())
					{
						/* It mean  buffer empty */
						memcpy(mAudioBuffer->base(), (uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
					}
					else
					{
						/* Need coall memmv to head */
						memmove(mAudioBuffer->base(), mAudioBuffer->data(), mAudioBuffer->size());
					}
					memcpy(mAudioBuffer->base() + mAudioBuffer->size(), (uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
					mAudioBuffer->setRange(0, mAudioBuffer->size() + buffer->range_length());
				}
				else
				{
					/* append data */
					memcpy(mAudioBuffer->data()+ mAudioBuffer->size(), (uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
					mAudioBuffer->setRange(mAudioBuffer->offset(), mAudioBuffer->size() + buffer->range_length());
				}
			}
			else
			{
				ALOGI("--- Lost audio sample [%d] capacity[%d] size[%d] offset[%d] ---", buffer->range_length(), mAudioBuffer->capacity(), mAudioBuffer->size(), mAudioBuffer->offset());
				//return;
			}
		}

		int64_t startTimeUs = ALooper::GetNowUs();

		ssize_t nbytes = 0;
		ssize_t write_size = 0;

		//ALOGI("#### part 2 numBytesAvailableToWrite[%d] sample size [%d] AudioBuffer->size[%d] #####", numBytesAvailableToWrite, buffer->range_length(), mAudioBuffer->size());
		if (numBytesAvailableToWrite > 0)
		{
			if (mAudioBuffer->size() > 0)
			{
				if (numBytesAvailableToWrite > mAudioBuffer->size())
					write_size = mAudioBuffer->size();
				else
					write_size = numBytesAvailableToWrite;
				//if (write_size != 4096) ALOGI("=== write_size is [%d] ==", write_size);
				nbytes = mAudioTrack->write(mAudioBuffer->data(), write_size);
				mAudioBuffer->setRange(mAudioBuffer->offset()+nbytes, mAudioBuffer->size()-nbytes);
				copy = write_size;
			}
			else
			{
				nbytes = mAudioTrack->write((uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
			}

			ALOGV("$$$$ Write data to render $$$$ pts [%.3f] nbytes[%d] size[%d] rangelength[%d] mAudioBuffer[%d] numBytesAvailableToWrite[%d]$$$$", (float)timeUs/1000000.0, nbytes, mAudioBuffer->size(), buffer->range_length(), mAudioBuffer->size(), numBytesAvailableToWrite);
			CHECK_EQ(nbytes, (ssize_t)copy);

		}

		int64_t delayUs = ALooper::GetNowUs() - startTimeUs;

		uint32_t numFramesWritten = nbytes / mAudioTrack->frameSize();

		if (delayUs > 2000ll) {
			ALOGW("AudioTrack::write took %lld us, numFramesAvailableToWrite=%u, "
					"numFramesWritten=%u",
					delayUs, numFramesAvailableToWrite, numFramesWritten);
		}
		mNumFramesWritten += numFramesWritten;
#endif /* end of USED_AUDIO_CALLBACK */
    }

protected:
    virtual ~AudioTrackRenderer() {
#ifdef USED_AUDIO_CALLBACK
		mAudioTrack->stop();
#endif /* end of USED_AUDIO_CALLBACK */
    }

private:
    AudioTrackRenderer(const AudioTrackRenderer&);
    AudioTrackRenderer&operator=(const AudioTrackRenderer&);
};

#ifdef USED_AUDIO_CALLBACK
// static
void MiddleSource::AudioTrackRenderer::AudioCallback(int event, void *user, void *info) {
	static_cast<MiddleSource::AudioTrackRenderer *>(user)->AudioCallback(event, info);
}

void MiddleSource::AudioTrackRenderer::AudioCallback(int event, void *info)
{
	switch (event) {
		case AudioTrack::EVENT_MORE_DATA:
			{
				AudioTrack::Buffer *buffer = (AudioTrack::Buffer *)info;
				size_t numBytesWritten = fillBuffer(buffer->raw, buffer->size);
				buffer->size = numBytesWritten;
			}
			break;

		case AudioTrack::EVENT_STREAM_END:	/* TODO, Fixed me */
//			mReachedEOS = true;
//			notifyAudioEOS();
			break;
	}
}

size_t MiddleSource::AudioTrackRenderer::fillBuffer(void *data, size_t size)
{
	size_t size_done = 0;
	size_t size_remaining = size;
	Mutex::Autolock autoLock(mLock);
	if (mAudioBuffer->size() > 0 && mNeedBuffering == false)
	{
		if (size > mAudioBuffer->size())
			size_remaining = mAudioBuffer->size();
		if (size_remaining > 0)
		{
			memcpy((char *)data, mAudioBuffer->data(), size_remaining);
			mAudioBuffer->setRange(mAudioBuffer->offset()+size_remaining, mAudioBuffer->size()-size_remaining);
		}
	}
	else
	{
		size_remaining = 0;
		mNeedBuffering = true;
	}
	return size_remaining;
}
#endif /* end of USED_AUDIO_CALLBACK */
#endif /* end of USED_AUDIO_RENDER */

MiddleSource::MiddleSource(const sp<MediaSource> &inputSource, const struct middle_parameter inputParameter, const sp<ANativeWindow> *gSurface)
	: mStarted(false),
	  mRender(true),
	  mInputSource(inputSource),
	  mMiddleParameter(inputParameter),
	  mVideoRenderer(NULL),
	  mAudioRenderer(NULL),
	  mVideoConvertBuffer(NULL)
{
	mInitCheck = OK;
	if (mMiddleParameter.mIsVideo)
	{
		if (gSurface != NULL)
		{
			sp<MetaData> meta = inputSource->getFormat();
			mVideoRenderer =  new MiddleSource::AwesomeNativeWindowRenderer(*gSurface, meta, 0, mMiddleParameter.data.video.usedNativeWindow);
			native_window_set_scaling_mode((*gSurface).get(), NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
		}
		mVideoConvertBuffer = new ABuffer(mMiddleParameter.data.video.inputWidth * mMiddleParameter.data.video.inputHeight / 2);
		mVideoConvertBuffer->setRange(0, 0);
#ifdef HOLD_ENCODER_RATE
		if (mMiddleParameter.data.video.fps > 120 || mMiddleParameter.data.video.fps <= 0)
			mMiddleParameter.data.video.fps = 30;
		mMiddleParameter.data.video.fps += 1;
		mRenderFrameRate = 1000000000 / mMiddleParameter.data.video.fps;
		mRenderFrameRate /= 1000;
		mLastPts = 0;
#endif /* end of HOLD_ENCODER_RATE */
	}
#ifdef USED_AUDIO_RENDER
	else
	{
		ALOGV("=== mMiddleParameter.data.audio.preQueueSize[%d] mMiddleParameter.data.audio.queueBuferSize[%d] ==", mMiddleParameter.data.audio.preQueueSize, mMiddleParameter.data.audio.queueBuferSize);
		if (!mMiddleParameter.data.audio.preQueueSize)
			mMiddleParameter.data.audio.preQueueSize = kMaxAudioPreQueueSize;
		if (!mMiddleParameter.data.audio.queueBuferSize)
			mMiddleParameter.data.audio.queueBuferSize = kMaxAudioQueueBufferSize;
		ALOGV("--[%d] new AudioTrack SampleRate [%d] Channel num [%d] ----", __LINE__, mMiddleParameter.data.audio.inputSampleRate, mMiddleParameter.data.audio.inputChannel);
		mAudioRenderer = new MiddleSource::AudioTrackRenderer(mMiddleParameter.data.audio.inputSampleRate, mMiddleParameter.data.audio.inputChannel, mMiddleParameter.data.audio.queueBuferSize, mMiddleParameter.data.audio.preQueueSize);
	}
#endif /* end of USED_AUDIO_RENDER */
}

MiddleSource::~MiddleSource() {
    if (mStarted) {
        reset();
    }
	mInputSource.clear();
}

status_t MiddleSource::initCheck() const {
    return mInitCheck;
}

status_t MiddleSource::start(MetaData *params) {
	
    Mutex::Autolock autoLock(mLock);
    if (mStarted) {
        return UNKNOWN_ERROR;
    }

    if (mInitCheck != OK) {
        return NO_INIT;
    }

	status_t err = mInputSource->start();
    if (err == OK) {
        mStarted = true;
    } else {
        mInputSource.clear();
    }
	ALOGI("%d: start %s, source is :%s", __LINE__, (err == OK)? "OK" : "Failed", mMiddleParameter.mIsVideo ? "Video" : "Audio");

    return err;
}

status_t MiddleSource::reset() {
    Mutex::Autolock autoLock(mLock);
    if (!mStarted) {
        return UNKNOWN_ERROR;
    }

    if (mInitCheck != OK) {
        return NO_INIT;
    }

    mStarted = false;
    mInputSource->stop();

    return OK;
}

sp<MetaData> MiddleSource::getFormat() {
    Mutex::Autolock autoLock(mLock);
    if (mInitCheck != OK) {
        return 0;
    }
	sp<MetaData> meta = mInputSource->getFormat();

    return meta;
}

status_t MiddleSource::read(
        MediaBuffer **out, const ReadOptions *options) {
    Mutex::Autolock autoLock(mLock);
    *out = NULL;
	MediaBuffer *outBuffer = NULL; 

    if (mInitCheck != OK) {
        return NO_INIT;
    }

	status_t err = OK;
	for(;;)
	{
		MediaBuffer *srcBuffer;
		err = mInputSource->read(&srcBuffer);
        if (err == INFO_FORMAT_CHANGED && !mMiddleParameter.mIsVideo) {
            continue;
        } else if (err != OK) {
			if (err == -110)
				err = ERROR_END_OF_STREAM;	// guess can't read anymore
            break;
        } 
		else
		{
			if (mMiddleParameter.mIsVideo)
			{
				if (mRender)
					mVideoRenderer->render(srcBuffer);

				handleVideoBuf(&outBuffer, srcBuffer);
				CHECK(outBuffer != NULL);
				*out = outBuffer;
				/* This is sample code: do scaler. The width muse be the same. By YUV420 Semi Planer*/
				*out = outBuffer;
				srcBuffer->release();
				srcBuffer = NULL;
#ifdef HOLD_ENCODER_RATE
				int64_t currentTimeUs = ALooper::GetNowUs();
				if (mLastPts == 0)
					mLastPts = currentTimeUs;
				else
				{
					if (mRenderFrameRate > (currentTimeUs - mLastPts))
						usleep(mRenderFrameRate - ((currentTimeUs - mLastPts)));
					mLastPts = ALooper::GetNowUs();
				}
#if 1
				static Statistics *middle_st= new Statistics("MiddleVideo", 3);
				middle_st->rate(1);
#endif
#endif /* end of HOLD_ENCODER_RATE */
				break;

			}
			else
			{
				/* TODO, Audio downmix in here */
				outBuffer = srcBuffer->clone();
				CHECK(outBuffer != NULL);
				/* TODO, implement DownMix API in handleAudioBuf */
				handleAudioBuf(outBuffer);
				*out = outBuffer;
#ifdef USED_AUDIO_RENDER
				if (mRender)
					mAudioRenderer->render(srcBuffer);
#endif /* end of USED_AUDIO_RENDER */
				srcBuffer->release();
				srcBuffer= NULL;
				break;
			}
		}
		
		*out = srcBuffer;
		break;
	}
    return err;
}

void MiddleSource::signalBufferReturned(MediaBuffer *buffer) {
    ALOGV("signalBufferReturned: %p", buffer->data());
    Mutex::Autolock autoLock(mLock);
    buffer->setObserver(0);
    buffer->release();
    return;
}

static int ALIGN(int x, int y) {
    // y must be a power of 2.
    return (x + y - 1) & ~(y - 1);
}
status_t MiddleSource::handleVideoBuf(MediaBuffer **dstDataBuffer, MediaBuffer *srcDataBuffer)
{

	if (mMiddleParameter.data.video.usedNativeWindow)
	{
		/* Original raw Data is YUV 420 SemiPlaner */
		size_t UVSourceSize = mMiddleParameter.data.video.inputWidth * mMiddleParameter.data.video.inputHeight;
		size_t ScalerDataSize = mMiddleParameter.data.video.outputWidth * mMiddleParameter.data.video.outputHeight;

		size_t UVScalerSize = ScalerDataSize / 2;
		sp<GraphicBuffer> graphicBuffer = srcDataBuffer->graphicBuffer();
		int usage = GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN;
		void *dst = NULL;
		graphicBuffer->lock(usage, &dst);

		if (dst != NULL)
		{
			*dstDataBuffer = new MediaBuffer(dst, srcDataBuffer->range_length(), srcDataBuffer->meta_data());
			//convertYV12toNV12((uint8_t *)dst, (uint8_t *)(*dstDataBuffer)->data() + (*dstDataBuffer)->range_offset(), mMiddleParameter.data.video.outputWidth, mMiddleParameter.data.video.outputHeight, srcDataBuffer->range_length());
		}

		CHECK(UVSourceSize >= ScalerDataSize);
	}
	else
	{
		*dstDataBuffer = srcDataBuffer->clone();
		//convertYV12toNV12((uint8_t *)(*dstDataBuffer)->data() + (*dstDataBuffer)->range_offset(), (uint8_t *)srcDataBuffer->data() + srcDataBuffer->range_offset(), mMiddleParameter.data.video.outputWidth, mMiddleParameter.data.video.outputHeight, srcDataBuffer->range_length());
	}
	/* Resize buffer */
	(*dstDataBuffer)->set_range(0, srcDataBuffer->range_length());
	return OK;
}

status_t MiddleSource::handleAudioBuf(MediaBuffer *dstDataBuffer)
{
	/* Audio Downmix API */
	return OK;
}

void MiddleSource::convertYV12toNV12(uint8_t *dst, uint8_t *src, int width, int height, int size) {
#ifdef HANDLE_YV12_TO_NV12
#ifdef DUMP_VIDEO_FRAMES
    static int cnt = 0;
    int fd = open(StringPrintf("/storage/sda1/yuv/YV12-%04d.yuv", cnt++).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd >= 0){
        write(fd, src, size);
        close(fd);
        fd = -1;
    }
#endif
    width = ROUNDUP16(width);
    height = ROUNDUP16(height);
    CHECK(width * height * 3 / 2 <= size);
    ALOGV("widht:%d height:%d size:%d", width, height, size);
	uint32_t uv_size = width * height / 2;
	memcpy(mVideoConvertBuffer->data(), src + width * height, mVideoConvertBuffer->capacity());
	mVideoConvertBuffer->setRange(0, mVideoConvertBuffer->capacity());
    const uint8_t *src_v = (const uint8_t *)mVideoConvertBuffer->data();
    const uint8_t *src_u = (const uint8_t *)src_v +  mVideoConvertBuffer->size() / 2;

    uint8_t *dst_y = (uint8_t *)dst;
    uint8_t *dst_u = dst_y + width * height;
    uint8_t *dst_v = dst_u + 1;

	//Y
	//memcpy(dst, src, width * height);

	//VU
	for(int i = 0; i < (width / 2 * height / 2); i++){
		*dst_v = *src_v;
		*dst_u = *src_u;
		src_v++;
		src_u++;
		dst_v+=2;
		dst_u+=2;
	}
	mVideoConvertBuffer->setRange(0, 0);
#else /* else of HANDLE_YV12_TO_NV12 */
#endif /* end of HANDLE_YV12_TO_NV12 */
}





}  // namespace android
