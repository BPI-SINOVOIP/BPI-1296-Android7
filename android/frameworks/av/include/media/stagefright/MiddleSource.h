/*
 * Copyright (C) 2009 The Android Open Source Project
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

#ifndef MIDDLE_SOURCE_H_

#define MIDDLE_SOURCE_H_

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MediaBuffer.h>
#include <utils/Mutex.h>

#include <android/native_window.h>

#define HOLD_ENCODER_RATE

namespace android {

struct MiddleSource : public MediaSource, public MediaBufferObserver {
public:
	struct middle_parameter {
		bool mIsVideo;
		union {
			struct {
				uint32_t inputSampleRate;
				uint32_t inputChannel;
				uint32_t outputSampleRate;
				uint32_t outputChannel;
				uint32_t preQueueSize;
				uint32_t queueBuferSize;
			} audio;
			struct {
				uint32_t inputWidth;
				uint32_t inputHeight;
				uint32_t outputWidth;
				uint32_t outputHeight;
				uint32_t fps;
				bool usedNativeWindow;
			} video;
		} data;
	};

    MiddleSource(
            const sp<MediaSource> &inputSource,
			const struct middle_parameter inputParameter,
			const sp<ANativeWindow> *gSurface
			);

    status_t initCheck() const;

    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop() { return reset(); }
    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options = NULL);

    virtual void signalBufferReturned(MediaBuffer *buffer);

	struct AwesomeNativeWindowRenderer;
	struct AudioTrackRenderer;
	void setPreview(bool previewOption) {mRender = previewOption; };


protected:
    virtual ~MiddleSource();

private:
	enum {
		kMaxBufferSize = 2048,
		kMaxAudioQueueBufferSize = 512*1024,
		kMaxAudioPreQueueSize = 32768*4,
	};

    Mutex mLock;

    status_t mInitCheck;
    bool mStarted;
	bool mRender;

	sp<MediaSource> mInputSource;
	struct middle_parameter mMiddleParameter;
	sp<AwesomeNativeWindowRenderer> mVideoRenderer;
	sp<AudioTrackRenderer> mAudioRenderer;
	sp<ABuffer> mVideoConvertBuffer;
#ifdef HOLD_ENCODER_RATE
	int64_t mLastPts;
	int64_t mRenderFrameRate;
#endif /* end of HOLD_ENCODER_RATE */

    status_t reset();
	status_t handleVideoBuf(MediaBuffer **dstDataBuffer, MediaBuffer *srcDataBuffer);
	status_t handleAudioBuf(MediaBuffer *rawDataBuffer);
	void convertYV12toNV12(uint8_t *dst, uint8_t *src, int width, int height, int size);

    MiddleSource(const MiddleSource &);
    MiddleSource &operator=(const MiddleSource &);
};

}  // namespace android

#endif  // MIDDLE_SOURCE_H_
