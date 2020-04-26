#ifndef __C_NAVIGATION_FILTER_H__
#define __C_NAVIGATION_FILTER_H__

#include <utils/RefBase.h>

#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/MediaExtractor.h>  
#include <media/stagefright/MediaSource.h>     
#include <media/stagefright/DataSource.h>    
#include <utils/Vector.h>
#include <utils/threads.h>

#include <OSAL.h>
#include <CStream.h>
#include <Filters/INavControl.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <Util.h>
#include <Utility/ResourceMgr/ioResource.h>
#include <Filters/NavigationFilter/NavPrintfMacro.h>
#include <Filters/PrivateInfo.h>

//#define WRITE_HEADER_ENABLE
#define NAV_USER_CMD_SIZE 64 // maximal size of user command block
#define NAV_MAX_USER_CMDS 32 // maximal number of queued user commands
#define NAV_MAX_CHANNELS  2  // maximal number of decoding channels
#ifdef WRITE_HEADER_ENABLE
#define VIDEO_PAYLOAD_TAG_SIZE  3
#define VC1_EXTRA_TAG_SIZE      128
#define NAV_MAX_SEQ_HEADER_SIZE     1024
#define NAV_MAX_PIC_HEADER_SIZE     4096


#define NAV_RM_MAX_FORMAT       4
#define NAV_RM_MAX_FRAME        1024
#define NAV_RM_MAX_SEGMENT      4096
#endif /* end of WRITE_HEADER_ENABLE */

#define REMOVE_BETTERSNIFFDATASOURCE
//#define RTK_ENABLE_DROP_RING_BUFFER_DATA

namespace android {

#ifdef WRITE_HEADER_ENABLE
////////////////////////////////////////////////////////////////////////////////
// RV_RINGBUF and RV_INFO for 1195 OMX decode RM/RMVB thumbnail
typedef struct _RV_RINGBUF_ {
    long    limit;
    long    readIndex;
    long    writeIndex;
    long    num;
} RV_RINGBUF;

typedef struct _RV_INFO_ {
    VIDEORMFORMAT      formatInfo[NAV_RM_MAX_FORMAT];
    RV_RINGBUF         formatInfoRing;

    VIDEORMFRAMEINFO   frameInfo[NAV_RM_MAX_FRAME];
    RV_RINGBUF         frameInfoRing;

    VIDEORMSEGMENTINFO segmentInfo[NAV_RM_MAX_SEGMENT];
    RV_RINGBUF         segmentInfoRing;
} RV_INFO;

#endif //WRITE_HEADER_ENABLE
////////////////////////////////////////////////////////////////////////////////
// CNavCmd - local helper for packing nav commands.
class CNavCmd
{
    static unsigned int m_latestCmdID;

public:

    unsigned int m_cmdID;
    unsigned int m_size;
    unsigned char m_buffer[NAV_USER_CMD_SIZE];

    inline CNavCmd(unsigned int* cmdID = NULL) : m_cmdID(0), m_size(0)
    {
        if(cmdID != NULL) {

            m_latestCmdID ++;
            m_cmdID = *cmdID = m_latestCmdID;
        }
    }

    inline void Pack(unsigned int data)
    {
        Pack((void*)&data, sizeof(unsigned int));
    }

    inline void Pack(void* data, unsigned int size)
    {
        assert(m_size+size <= NAV_USER_CMD_SIZE);
        memcpy(m_buffer+m_size, data, size);
        m_size += size;
    }
};

////////////////////////////////////////////////////////////////////////////////
// CNavCmdQueue - nav command queue

typedef ulCircularQueue<CNavCmd> CNavCmdQueue;

////////////////////////////////////////////////////////////////////////////////
// CNavigationPinInfo - information of filter pin connection

struct CNavigationPinInfo
{
    IMemRingAllocator* pAllocator;
    BYTE*              pBufferLower; // lower bound of buffer
    BYTE*              pBufferUpper; // upper bound of buffer
    int                cachedAddrOffset;
};

////////////////////////////////////////////////////////////////////////////////
// CNavigationFilter - stream class filter implementation.
class CNavigationFilter :
            public CBaseFilter,    // base filter implementation
            public IFilterSeeking, // filter seeking interface
            public INavControl     // navigator control interface
{
public:

    enum { VIDEO_PIN = 0,
           AUDIO_PIN,
           SPIC_PIN,
           TELETEXT_PIN,
           NUMBER_OF_PINS };

    CNavigationFilter();
    virtual ~CNavigationFilter();

    HRESULT Stop();
    HRESULT Pause(FILTER_STATE state);
    HRESULT Run();
    HRESULT FilterType(FILTER_TYPE* pFilterType);
    HRESULT SetFilterProperty(FILTER_PROPERTY propId, void* propData, unsigned int propSize);
    HRESULT ReadFrame(int *buftype);
    HRESULT SetFD(int newfd);
    HRESULT SetFD(int newfd, void *func_ptr);
	
protected:
	bool			   m_bEOS;
    CBaseOutputPin     m_outputPin[NAV_MAX_CHANNELS][NUMBER_OF_PINS];
    CBaseOutputPin     m_infoPin;
    CBaseOutputPin     m_passThruPin;
    IReferenceClock*   m_pRefClock;

    IOPLUGIN           m_ioPlugin;
    INPUTPLUGIN        m_inputPlugin;
    DEMUXPLUGIN        m_demuxPlugin[NAV_MAX_CHANNELS];
    int                m_currentDemuxIndex[NAV_MAX_CHANNELS];
    NAVDEMUXIN         m_demuxIn;
    NAVDEMUXIN         m_demuxInReserved[NAV_MAX_CHANNELS];
    NAVDEMUXCONFIG     m_demuxConfig;
    NAVDEMUXOUT        m_demuxOut;
    NAVDEMUXOUT        m_demuxOutReserved[NAV_MAX_CHANNELS];
    NAV_STREAM_TYPE    m_channelFullnessInfo[NAV_MAX_CHANNELS];
    DATA_MEDIA_TYPE    m_inputType;
    NAV_AVSYNC_MODE    m_AVSyncMode;
    long               m_AVSyncStartupFullness;
    NAVTIMEOUTLIMITS   m_timeoutLimits;
    NAVDEMUXPTSINFO    m_demuxPTSInfo;
    bool               m_bDeliverPassThru;
    bool               m_bDeliverPassThru_redeliver;
    unsigned char*     m_bDeliverPassThru_data;
    unsigned int       m_bDeliverPassThru_size;
    bool               m_bStopDelivered[NAV_MAX_CHANNELS][NUMBER_OF_PINS];
    bool               m_bResetAVSync;
    bool               m_bPendingOnFullness;
    bool               m_bCheckingAFModeStatus;
    bool               m_bCheckCmd;
    bool               m_bSmoothReverseCapable;
    long               m_minForwardSkip;
    long               m_maxForwardSkip;
    long               m_minReverseSkip;
    long               m_maxReverseSkip;
    bool               m_bUseDDRCopy;
    bool               m_bFlushBeforeDelivery;
    uint64_t           m_latestDDRCopyHandle;
    int                m_bytesReadSinceStart;
    int64_t            m_AFModeStatusCheckingTimer;
    bool               m_bFlushState;
    
    #ifndef DONTFIXBUG17880
    int64_t			   m_startAudioPts;
    int64_t			   m_startVideoPts;
    #endif

    // used only when __NAV_USE_DLL_PLUGIN__ is defined
    void*              m_pInputPluginLibrary;
    void*              m_pDemuxPluginLibrary[NAV_MAX_CHANNELS];

    CNavCmdQueue       m_navCmdQueue;
    NAVCMDSTATUS       m_latestCmdStatus;
    NAVMEDIAINFO       m_mediaInfo;
#ifdef WRITE_HEADER_ENABLE
    unsigned char      m_pSeqHeader[NAV_MAX_SEQ_HEADER_SIZE];
    unsigned char      m_pPicHeader[NAV_MAX_PIC_HEADER_SIZE];

    bool               m_bSeqHeaderDelivered;
    int                m_nSeqHeaderSize;
    int                m_nPicHeaderSize;
    int                m_nGarbageSize;

    unsigned int       m_codecProfile;
    unsigned int       m_vc1ExtraData[VC1_EXTRA_TAG_SIZE];
    int                m_frameWidth;
    int                m_frameHeight;

    RV_INFO            m_rmInfo;

    unsigned char      m_videoPayloadTag[VIDEO_PAYLOAD_TAG_SIZE];
    int                m_videoPayloadSize;

    BYTE*              m_pPicHeaderWritePtr;
    unsigned char     *m_pDemuxVideoData;
#endif // WRITE_HEADER_ENABLE

    int                m_speed;
    int                m_skip;

    osal_thread_t      m_thread;
    osal_mutex_t       m_streamingMutex;
    int				   m_navThreadPriority;

    CNavigationPinInfo m_pinInfo[NAV_MAX_CHANNELS][NUMBER_OF_PINS];

    // resource management
    RESOURCE_NAME      m_ioDevice;

	// error concealment level
    int                m_videoErrConcealmentLevel;
    // move static value from CNavigationFilter.cpp to here.
    bool               m_bCollectingDebugInfo;
    int                m_debug_ring_buffer_full_count;
    int64_t            m_dbgRateReadCount[NAV_MAX_CHANNELS];
    int64_t            m_dbgRateClock;
    int64_t            m_dbgRatePinCount[NAV_MAX_CHANNELS][NUMBER_OF_PINS];
    int                m_thread_Proc_init_flag;

    static void ThreadProcEntry(
        void* pInstance);

    void ThreadProc();

    void Read();

    void StartStreaming();
    void StopStreaming();

    void CheckPin(
        CBaseOutputPin*     pPin,
        CNavigationPinInfo* pPinInfo);
#ifdef WRITE_HEADER_ENABLE
    void InitRmInfo(void);
    void FlushRmInfo(void);

    int IsUsingShareBSRingFormat(unsigned int format);
    int BuildSeqHeader(unsigned char* pSeqHeader);
    int GetPicHeaderSize(unsigned char* pInputBuffer);
    int BuildPicHeader(unsigned char* pbHeader, unsigned char* pInputBuffer, unsigned int pInputBufferLen);
#endif // WRITE_HEADER_ENABLE

    bool HandleUserCmd(CNavCmd* pCmd);
    HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired = NULL);
    HRESULT DeliverUserCmd(CNavCmd* pCmd);

    HRESULT DeliverData(NAVDEMUXOUT* pDemuxOut);
    HRESULT SyncWritePointers(int channelIndex, unsigned int WP_update_flags, BYTE* WP[]);
    HRESULT DeliverPrivateInfo(NAVBUF* pNavBuffer);
    HRESULT DeliverNavBufCommands(NAVBUF* pNavBuffer);

/* ------------ the following lines are to be deleted after new interface adopted --------------*/
#define BYTEptr BYTE*
    void  DeliverDV(CNavigationPinInfo* pPinInfo,BYTE*   pBufferCurr,
        BYTEptr&  pBufferNext,NAVBUF * pNavBuffer);
/* ---------------------------------------------------------------------------------------------*/

    HRESULT DeliverStop(int channelIndex);
    HRESULT DeliverFlush(PRESENTATION_POSITIONS* pLastPosition = NULL);
    HRESULT DeliverEndOfStream(int channelIndex, bool bPostEvent);
    HRESULT DeliverEndOfStream(int channelIndex, unsigned int pinIndex, bool bPostEvent);
    HRESULT DeliverNewSegment(int channelIndex);

    void Sleep(long milliseconds, bool bCancelFullnessPending = true);
    void UpdatePlaybackPosition();
    void ResetAVSync(NAVBUF* pNavBuffer);

    HRESULT SelectInputPlugin(char* org_path, ENUM_MEDIA_TYPE majorType, ENUM_MEDIA_TYPE subType, bool* pbContinue, bool bForcedIdentification, NAVLOADFAILURE* pFailure,fpGetAuthInfo getAuthInfo, bool bRetryIOPlugin);
    HRESULT SelectDemuxPlugin(int channelIndex, ENUM_MEDIA_TYPE type);
    HRESULT OpenInputPlugin(int index, ENUM_MEDIA_TYPE majorType, ENUM_MEDIA_TYPE subType);
    HRESULT OpenDemuxPlugin(int channelIndex, int index, ENUM_MEDIA_TYPE type);
    HRESULT CloseInputPlugin(bool bUnloadMedia = true);
    HRESULT CloseDemuxPlugin(int channelIndex);
    HRESULT OpenIOPlugin(int index);

    HRESULT DispatchProperty(
        NAV_PROP_ID id,
        void* inData,
        unsigned int inDataSize,
        void* outData,
        unsigned int outDataSize,
        unsigned int* returnedSize);

    bool IsVideoConnected();
    bool IsAudioConnected();
    bool IsIndexGenConnected();

    void FlushPin(int channelIndex, unsigned int pinIndex);

    
    inline HRESULT DeliverPrivateInfo(CBaseOutputPin* pPin, int infoId, BYTE* pInfo, int length)
    {
    	HRESULT res = S_OK;
    	while((res = pPin->DeliverPrivateInfo(infoId, pInfo, length)) == E_TIMEOUT && m_State == State_Running)
    	{
            if (m_bFlushState) { ALOGD("\033[0;32m $$$ In %s:%d detect m_bFlushState exit while loop $$$$\033[m", __FUNCTION__, __LINE__); break;}
    		osal_MutexUnlock(&m_streamingMutex);
    		osal_Sleep(100);
    		osal_MutexLock(&m_streamingMutex);
    	}
    	
    	return res;
    }

	void CheckUnderflowStatus();

	HRESULT IdentifyIOPlugin(char *path, ENUM_MEDIA_TYPE orgMajorType, bool bRetry, bool *pbContinue, void** ppFH);

#if defined(NAV_LOG_TO_FILE)
	void OpenLogFile();
	void CloseLogFile();	
#endif
#if defined(DUMP_DEMUX_AUDIO_DATA) || defined(DUMP_DEMUX_VIDEO_DATA)
	void OpenDumpFile(unsigned int pinType);
	void CloseDumpFile(unsigned int pinType);
	void DumpFile(unsigned int pinType, unsigned char* pBuffer, unsigned int size);	
#endif
#if defined(CALCU_WFD_ACCU_DATA_BYTES) && !defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
	void CalculateAccuDemuxBytes();
#endif
#if defined(CALCU_WFD_ACCU_DATA_BYTES) || defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
	void PrepareSharedMemory();
#endif
#if defined(CALCU_WFD_ACCU_DATA_BYTES)
	void AccuDemuxBytes(unsigned int pinType, unsigned int bytes);
	void ClearAccuDemuxBytesVariables();
#endif
#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
	void SavePtsWritePointerAfterSync(bool isSendVideoFrameStartDone, unsigned int frameStartSlot, bool isSendPossibleVideoFrameEndDone, CMemRingAllocator* pAllocator);
	void ClearCheckLatencyVariables();	
	void SavePacketPos(unsigned int rtp_seq, unsigned int offset, packet_pos* pPos, char showForVideoFrameEnd, unsigned long phyWPAfterMemcpy);
	void CalculateLatency(unsigned int slot);
	void CheckVideoFrameStartEnd(NAVBUF* pNavBuffer, NAVBUF* pNextNavBuffer, unsigned long phyWPAfterMemcpy, bool *pIsSendVideoFrameStartDone=NULL, unsigned int *pFrameStartSlot=NULL, bool* pIsSendPossibleVideoFrameEndDone=NULL);
#endif
#if defined(SEND_RTP_HEADER_MARKER_BIT_TO_VIDEO_FW)
	void PrintRtpMbitInfo(unsigned long phyWP, unsigned short rtp_seq_number);
#endif

public:

	bool	getEosFlag();
    // IFilterSeeking interface
    virtual HRESULT FlushData(int channelIndex, unsigned int pinIndex);

    HRESULT SetSeekingInfo    (long Speed, long Skip);
    HRESULT GetSeekingInfo    (long* pSpeed, long* pSkip);
    HRESULT CheckCapabilities (FILTER_SEEKING_CAP* pSeekingCap);

    // INavControl interface

    virtual HRESULT LoadMedia(
        char* path,
        NAVMEDIAINFO* pMediaInfo=NULL,
        ENUM_MEDIA_TYPE preDeterminedType=MEDIATYPE_None,
        bool* pbContinue=NULL,
        bool bForcedIdentification = false,
        NAVLOADFAILURE* pFailure = NULL,
        fpGetAuthInfo getAuthInfo = NULL,
        bool bRetryIOPlugin = false);

    virtual HRESULT UnloadMedia();

    virtual HRESULT SetGetProperty(
        NAV_PROP_ID id,
        void* inData,
        unsigned int inDataSize,
        void* outData,
        unsigned int outDataSize,
        unsigned int* returnedSize);

    virtual HRESULT GetPlaybackStatus(
        NAVPLAYBACKSTATUS* status);

    virtual HRESULT GetVideoStatus(
        NAVVIDEOSTATUS* status);

    virtual HRESULT GetAudioStatus(
        NAVAUDIOSTATUS* status);

    virtual HRESULT GetSubpictureStatus(
        NAVSPICSTATUS* status);

    virtual HRESULT GetMenuStatus(
        NAVMENUSTATUS* status);

    virtual HRESULT GetDiscStatus(
        NAVDISCSTATUS* status);

    virtual HRESULT GetTitleStatus(
        int titleNum,
        NAVTITLESTATUS* status);

    virtual HRESULT GetLatestCmdStatus(
        NAVCMDSTATUS* status);

    virtual HRESULT GetNavState(
        void* state,
        unsigned int size,
        unsigned int* returnedSize);

    virtual HRESULT SetNavState(
        void* state,
        unsigned int size,
        bool bForced=false);

    virtual HRESULT PlayTitle(
        int titleNum,
        unsigned int* cmdID);

    virtual HRESULT PlayChapter(
        int titleNum,
        int chapterNum,
        unsigned int* cmdID);

    virtual HRESULT PlayNextChapter(
        unsigned int* cmdID);

    virtual HRESULT PlayPrevChapter(
        unsigned int* cmdID);

    virtual HRESULT PlayAtTime(
        int titleNum,
        unsigned int startSeconds,
        unsigned int startFrameIdx,
        unsigned int* cmdID);

	virtual HRESULT PlayAutoChapter(
        int titleNum,
        int chapterNum,
        unsigned int* cmdID);
        
#ifndef DISABLEPLAYATPOS
	virtual HRESULT PlayAtPos(
		int titleNum,
		long long pos,
		unsigned int* cmdID);
#endif        

    virtual HRESULT PlaySegment(
        int titleNum,
        unsigned int startSeconds,
        unsigned int startFrameIdx,
        unsigned int endSeconds,
        unsigned int endFrameIdx,
        bool bRepeat,
        unsigned int* cmdID);

    virtual HRESULT MenuShow(
        unsigned int menuID,
        unsigned int* cmdID);

    virtual HRESULT MenuEscape(
        unsigned int* cmdID);

    virtual HRESULT ButtonSelectNumeric(
        int buttonNum,
        unsigned int* cmdID);

    virtual HRESULT ButtonSelectPoint(
        unsigned int x,
        unsigned int y,
        unsigned int* cmdID);

    virtual HRESULT ButtonActivateNumeric(
        int buttonNum,
        unsigned int* cmdID);

    virtual HRESULT ButtonActivatePoint(
        unsigned int x,
        unsigned int y,
        unsigned int* cmdID);

    virtual HRESULT ButtonActivate(
        unsigned int* cmdID);

    virtual HRESULT ButtonMoveSelection(
        NAV_BTNDIR_ID direction,
        unsigned int* cmdID);

    virtual HRESULT GoUp(
        unsigned int* cmdID);

    virtual HRESULT StillEscape(
        unsigned int* cmdID);

    virtual HRESULT SetAudioStream(
        int streamNum,
        unsigned int* cmdID);

    virtual HRESULT SetSubpictureStream(
        int streamNum,
        NAV_DISPLAY_STATE displayState,
        bool bDummyStream,
        unsigned int* cmdID);

    virtual HRESULT SetAngle(
        int angleNum,
        unsigned int* cmdID);

    virtual HRESULT SetGetProperty_Async(
        NAV_PROP_ID id,
        void* inData,
        unsigned int inDataSize,
        void* outData,
        unsigned int outDataSize,
        unsigned int* returnedSize,
        unsigned int* cmdID);

    virtual HRESULT SendNavCommand(
        NAV_CMD_ID id,
        void* cmdData,
        unsigned int cmdDataSize,
        unsigned int* cmdID);

	virtual HRESULT GetBufferFullness(
		int* pFullness, 
		FILTER_TYPE filterType = FILTER_DECODE_VIDEO, 
		int channelIndex = 0);

	virtual HRESULT GetBuffer(
		int channelIndex, int trackNo, void *pNAVPkt);

	virtual HRESULT GetPinPktCnt(
		int channelIndex, int trackNo, void *pNAVPkt);

	virtual HRESULT GetAudioFormatPkt(
		int channelIndex, int trackNo, void *pNAVPkt);

// for IO plugin buffer fullness requirement
	virtual HRESULT GetIOBufferFullness(int *pFullness, int *pFlag = NULL);

//#ifndef NDEBUG

    // IFilterControl interface overriding
    virtual HRESULT ExecuteCommandLine(
        char* command);

//#endif
    HRESULT SetDataSourceHandler(sp<DataSource> source);
    HRESULT SetInputFlowInsterface(void);

	bool m_bKeepRCD;
	
	struct {
		bool bPending;
		unsigned int vContext;
		unsigned int aContext;
		int			 vChIndex;
		int			 aChIndex;
	} m_avContext;
	
	long m_offsetToPhyAddr;
	long m_bUnderflow;
	
	bool m_bLenientUnderflow;
	bool m_bCheckVideoMasterFirstState;
	
	char* m_path;
	int use_fd;
    ssize_t (*IOreadAt_func)(off64_t offset, void *data, size_t size);
    //        virtual ssize_t readAt(off64_t offset, void *data, size_t size);
    int last_navbuf_type;
    int last_AUDIO_PIN_PktCnt;
    int last_VIDEO_PIN_PktCnt;
    int m_PktCntR, m_PktCntW;
    sp<DataSource> mSource;
    int64_t assume_audio_only_pts;
    int is_audio_only;
    int is_video_only;

};

}  // namespace android

#endif //__C_NAVIGATION_FILTER_H__
