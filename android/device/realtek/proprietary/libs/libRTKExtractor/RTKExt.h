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

#ifndef RTK_EXTRACTOR_H_

#define RTK_EXTRACTOR_H_
#include <utils/RefBase.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaErrors.h>
#include <media/IMediaSource.h>
#include <utils/Vector.h>
#include <utils/threads.h>

#include <StreamClass/EType.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/PrivateInfo.h> // for RM
#include <Filters/NavigationFilter/CNavigationFilter.h>
#include <Filters/DummyDecoder/CDummyDecoder.h>

#include <system/si4/SITypes.h>	// for SI

#include <LiveSession.h>	//android httplive

//#define ENABLE_USE_M3U8
#define REDUCE_LOADMEDIA_VERSION // Reduce loadMedia count form 2 to 1
#define REDUCE_CODE_FLOW    // Disable unused code FLOW

typedef enum _SCAN_FILE_STATE {
    SCAN_FILE_STATE_OFF = 0,
    SCAN_FILE_STATE_ON,
    SCAN_FILE_STATE_STOPPING,
    SCAN_FILE_STATE_COMPLETED,
} SCAN_FILE_STATE;

typedef enum _RTKEXT_PARAM_ENUM{
    RTKEXT_PARAM_UNSUPPORT = 0,
    RTKEXT_PARAM_FILE_MODE = 1,
    RTKEXT_PARAM_NET_MODE = RTKEXT_PARAM_FILE_MODE<<1,
    RTKEXT_PARAM_THUMBNAIL_MODE = RTKEXT_PARAM_FILE_MODE<<2,
    RTKEXT_PARAM_SINGLE_TRACK_MODE = RTKEXT_PARAM_FILE_MODE<<3,
    RTKEXT_PARAM_ENUM_MAX = RTKEXT_PARAM_FILE_MODE<<31,
}RTKEXT_PARAM_ENUM ;

namespace android {

struct AMessage;
class DataSource;
class String8;
class IMediaSource;
class MetaData;

class RTKExt : public MediaExtractor {
public:
    // Extractor assumes ownership of "source".
    RTKExt(sp<DataSource> source);
    virtual int RTKExt_rest_init(void);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();
    //virtual ssize_t IOreadAt2(off64_t offset, void *data, size_t size);
#ifdef ENABLE_USE_M3U8
    virtual ssize_t IOreadAtM3U8(off64_t offset, void *data, size_t size);
#endif
    virtual int64_t getCachedDurationUs(status_t *finalStatus);
    virtual size_t approxDataRemaining(status_t *finalStatus);
    virtual void FlowStop(void);

    uint32_t flags() const;

    int32_t thread_mode;
    CNavigationFilter *m_pSource;
    status_t check_init();

    NAVMEDIAINFO m_mediaInfo;
    CFlowManager *m_pFManager;
    int video_src_retry_cnts;
    long long video_src_last_pts;
    Mutex       mVideolock ;
    Condition   mVideoCondition;

    sp<ALooper> looper;
    sp<LiveSession> liveSession;

    int         is_seek;
    int         wait_seek;
    int         wait_seek_time;
    Mutex       mSeeklock ;
    Condition   mSeekCondition;
    int         mReqSeeking; 
    int         mRefCount;

    Mutex       mFlushlock ;
    Condition   mFlushCondition;

    int audio_src_retry_cnts;
    long long audio_src_last_pts;
    Mutex        mAudiolock ;
    Condition   mAudioCondition;

	SI *m_siHandle;
	SCAN_FILE_STATE m_scanFileState;
	bool m_scanFile_StopByOthers;
	void ScanFileStop();
    bool mUsedInband;   /* for remove inband command used */
    int mis_audio;
    int mis_video;
    CDummyDecoder *m_pVDummyDecoder;
    CDummyDecoder *m_pADummyDecoder;
    int mOneTrackOnlyPauseFlg;

protected:
    virtual ~RTKExt();

private:

    struct Track {
        int32_t rtk_track;
        sp<MetaData> mMeta;
        uint32_t timescale;
        Track *next;
        bool skipTrack;
    };
    Track *mFirstTrack, *mLastTrack;
    int32_t track_sz;
    bool mCreateSource;

    IReferenceClock *m_pRefClock;

    sp<DataSource> mDataSource;
    sp<DataSource> mDSM3U8;
    status_t mInitCheck;

    sp<MetaData> mFileMetaData;

    status_t init();

    pthread_t mThread;
    static void *ThreadWrapper(void *);
    status_t threadFunc();
	static void fileScan_thread(void *);
	HRESULT Lock();
	HRESULT Unlock();
	osal_mutex_t            m_mutex;
	osal_event_t            m_event;
	osal_thread_t           m_fileScan_thread_id;

    void track_check(int container);
    const char * GetMIMETypeForHandler(uint32_t handler);

    void DemuxMP4Create(void);
    void DemuxTSCreate(void);
    void DemuxPSCreate(void);
    void DemuxMKVCreate(void);
    void DemuxFLVCreate(void);
    void DemuxASFCreate(void);
    void DemuxAVICreate(void);
    void DemuxRMCreate(void);
    void DemuxSocketCreate(void);
    void DemuxAudioCreate(void);

    int mFd;
    
    RTKExt(const RTKExt &);
    RTKExt &operator=(const RTKExt &);
};

}  // namespace android

#define setUseRTKInBand(void) \
{ \
 track->mMeta->setInt32(kKeyRTKExtr, mUsedInband); \
}
            

#endif  // RTK_EXTRACTOR_H_
