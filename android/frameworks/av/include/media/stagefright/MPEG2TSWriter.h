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

#ifndef MPEG2TS_WRITER_H_

#define MPEG2TS_WRITER_H_

#include <arpa/inet.h>
#include <sys/socket.h>

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/foundation/AHandlerReflector.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/MediaWriter.h>
#include <binder/MemoryBase.h>

#define SUPPORT_MUXER_RTP
#ifdef SUPPORT_MUXER_RTP
#define WRITE_DATA_FROM_LARGE_BUFFER	// For muxer max 7 ts during one RTP
#endif /* end of SUPPORT_MUXER_RTP */

namespace android {

struct ABuffer;

#if 1
enum {
    MuxInPtsOrder,
    MuxInIncomingOrder,
    MuxBySystemTime,
    MuxDirectOutput,
    MuxInPtsOrderQueue,
    MuxMaxNumber,
};
#endif

struct MPEG2TSWriter : public MediaWriter {
    MPEG2TSWriter(int fd);
    MPEG2TSWriter(const char *filename);
    MPEG2TSWriter(void *cookie, ssize_t (*write)(void *cookie, const void *data, size_t size));
#if 1
    MPEG2TSWriter(int port, const char *ip, bool enableRtp);
    MPEG2TSWriter(FILE *f_fd);
#if 1//!defined(__LINARO_SDK__)
    MPEG2TSWriter(sp<IMemory> data, sp<IMemory> info, bool enableRtp);
    MPEG2TSWriter(sp<IMemory> data, sp<IMemory> info, bool enableRtp, int port, const char *ip);
#endif
    void setMuxerMode(int muxerMode);
	void setVideoFrameFps(int32_t videoFrameFps);
#endif

    virtual status_t addSource(const sp<IMediaSource> &source);
    virtual status_t start(MetaData *param = NULL);
    virtual status_t stop() { return reset(); }
    virtual status_t pause();
    virtual bool reachedEOS();
    virtual status_t dump(int fd, const Vector<String16>& args);

    void onMessageReceived(const sp<AMessage> &msg);

protected:
    virtual ~MPEG2TSWriter();

private:
    enum {
        kWhatSourceNotify = 'noti',
        kWhatMuxerModeChange = 'muxr',
    };

    struct SourceInfo;

    FILE *mFile;

    void *mWriteCookie;
    ssize_t (*mWriteFunc)(void *cookie, const void *data, size_t size);

    sp<ALooper> mLooper;
    sp<AHandlerReflector<MPEG2TSWriter> > mReflector;

    bool mStarted;

    Vector<sp<SourceInfo> > mSources;
    size_t mNumSourcesDone;

#if 1
	/* Add for testing UDP */
	int mPort;
	AString mIp;
	int mSocket;
	struct sockaddr_in mUDPAddr;
#if 1//!defined(__LINARO_SDK__)
	sp<IMemory> mBufferData;
	sp<IMemory> mBufferInfo;
#endif
	bool mRtp;
	uint32_t mRTPSeqNo;
	int mMuxerMode;
    int64_t mTsStartTime; //in ns
	int64_t mStartPts; //in us
	int32_t mVideoFrameFps;
#endif

    int64_t mNumTSPacketsWritten;
    int64_t mNumTSPacketsBeforeMeta;
    int mPATContinuityCounter;
    int mPMTContinuityCounter;
    uint32_t mCrcTable[256];
    Vector<sp<ABuffer>> mFirstTrack;
    Vector<sp<ABuffer>> mSecondTrack;

#if 1
    void muxToTsStream(int32_t sourceIndex, sp<ABuffer> buffer, bool readMore);
    void muxInIncomingOrder(int32_t sourceIndex, sp<ABuffer> buffer, int64_t timeUs);
    void muxBySystemTime(const sp<AMessage> &msg, int32_t sourceIndex, sp<ABuffer> buffer, int64_t timeUs);
    void muxInPtsOrderQueue(int32_t sourceIndex, sp<ABuffer> buffer, int64_t timeUs);
    void flushTrackQueue();
#endif
    void muxInPtsOrder(int32_t sourceIndex, sp<ABuffer> buffer);

    void init();
    void initMembers();

    void writeTS();
    void writeProgramAssociationTable();
    void writeProgramMap();
    void writeAccessUnit(int32_t sourceIndex, const sp<ABuffer> &buffer);
    void initCrcTable();
    uint32_t crc32(const uint8_t *start, size_t length);

    ssize_t internalWrite(const void *data, size_t size);
    status_t reset();

	ssize_t convertRtp(const sp<ABuffer> &tsBuffer, size_t size);

    DISALLOW_EVIL_CONSTRUCTORS(MPEG2TSWriter);
};

}  // namespace android

#endif  // MPEG2TS_WRITER_H_
