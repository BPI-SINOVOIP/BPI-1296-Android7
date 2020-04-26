#ifndef __C_DTV_INPUT_PLUGIN_H__
#define __C_DTV_INPUT_PLUGIN_H__

#include "CDtvFileReader.h"
#include "CDtvFileWriter.h"

#include <Util.h>
#include <IStream.h>
#include <InbandAPI.h>
#include <AudioInbandAPI.h>
#include "VideoRPCBaseDS_data.h"
#include <Filters/PrivateInfo.h>

#if defined(ENABLE_DTV_3D)
#include "CDtvSubPath.h"
#endif

#if defined(DVB_T)
#include <Filters/NavigationFilter/SiDvb/siApi.h>
#include <SITypes_local.h>
#endif

#if AP_CONFIG_WITH(APCFG_HW_DTV)
#include <Platform_Lib/DTVFrontendControl/DTVTunerControl.h>

	#define TUNER_CONTROL_CONFIG 					TunerControlConfig
	#define TUNER_CONTROL_INIT	 					TunerControlInit
	#define TUNER_CONTROL_GET_BUFFER_PADDING_SIZE 	TunerControlGetBufferPaddingSize
	#define TUNER_CONTROL_SET_RING_BUFFER 			TunerControlSetRingBuffer
	#define TUNER_CONTROL_STOP			  			TunerControlStop
	#define TUNER_CONTROL_SET_PID_FILTERS 			TunerControlSetPIDFilters
	#define TUNER_CONTROL_UNINIT		  			TunerControlUnInit
	#define TUNER_CONTROL_READ_DATA		  			TunerControlReadData
	#define TUNER_CONTROL_RELEASE_DATA	  			TunerControlReleaseData
	#define TUNER_CONTROL_START			  			TunerControlStart
	#define TUNER_CONTROL_FLUSH			  			TunerControlFlush
	
	#define TUNER_CTRL_RETVALUE_BUFFER_FULL		  TUNER_CTRL_BUFFER_FULL
	#define TUNER_CTRL_RETVALUE_OK				  TUNER_CTRL_OK
	#define TUNER_CTRL_RETVALUE_ENODEV			  	TUNER_CTRL_ENODEV
	
#else
	static inline int RETURN_FALL(unsigned char, ...) {return 0;}
	
	#define TUNER_CONTROL_CONFIG					RETURN_FALL
	#define TUNER_CONTROL_INIT	 					RETURN_FALL
	#define TUNER_CONTROL_GET_BUFFER_PADDING_SIZE 	RETURN_FALL
	#define TUNER_CONTROL_SET_RING_BUFFER 			RETURN_FALL
	#define TUNER_CONTROL_READ_DATA		  			RETURN_FALL
	#define TUNER_CONTROL_STOP(...)		  			;
	#define TUNER_CONTROL_SET_PID_FILTERS(...) 		;
	#define TUNER_CONTROL_UNINIT(...)				;
	#define TUNER_CONTROL_RELEASE_DATA(...)			;	  			
	#define TUNER_CONTROL_START(...)				;			  			
	#define TUNER_CONTROL_FLUSH(...)				;			  			
	
	#define TUNER_CTRL_RETVALUE_BUFFER_FULL		  0
	#define TUNER_CTRL_RETVALUE_OK				  1
	#define TUNER_CTRL_RETVALUE_ENODEV			  2
#endif

// FILE BUFFER LAYOUT
//
// [--- reserved_bytes ---] [-------------------- read_bytes --------------------]
//
//     * io plugin always read data into the "read_bytes" region
//     * "reserved_bytes" region (actually only the last few bytes of it) is used to store a partial
//       TS packet to form a complete TS packet with the leading bytes of the "read_bytes" region

#define DEFAULT_READ_WRITE_SIZE		(204*1024)
#define FILE_BUFFER_RSVR_BYTES   (4096)			//for devices with sector size 4k(refer to bug 18414)            
#define MAX_READ_ERROR_ALLOWED   (256)
#define MAX_NO_SIGNAL_TOLERANCE	 (64)
#define TS_FAST_PARSING_BUF_SIZE	(188*1024)	// for m_bIOHttpOrHLS > 0 case (HTTP/HLS/UDP/RTP)
#define TUNER_RECORD_TS_IFO_SIZE	(188*1024)

#define ABS(x)  ((x < 0)? -x : x)

////////////////////////////////////////////////////////////////////////////////

#define IS_FILE_MODE(_this)  ((_this)->m_pIOPlugin != NULL)
#define IS_SCANNING(_this)   (IS_FILE_MODE(_this) && (_this)->m_pFileReader->m_speed != 256)
#define IS_TRANSPORT(_this)  ((_this)->m_mediaType == MEDIASUBTYPE_MPEG2_TRANSPORT)
#define IS_DECODING_THUMBNAIL(_this)	((_this)->m_pThumbDecDone != NULL)

#define HAS_VIDEO(_this)  (IS_TRANSPORT(_this)? ((_this)->m_activeTarget[TARGET_VIDEO].pid != -1) : true)
#define HAS_AUDIO(_this)  (IS_TRANSPORT(_this)? ((_this)->m_activeTarget[TARGET_AUDIO].pid != -1) : !IS_SCANNING(_this))
#define HAS_SUB_T(_this)  (IS_TRANSPORT(_this)? ((_this)->m_activeTarget[TARGET_SUBTITLE].pid != -1) : false)
#define HAS_TEL_T(_this)  (IS_TRANSPORT(_this)? ((_this)->m_activeTarget[TARGET_TELETEXT].pid != -1) : false)

#define HAS_PRESENTATION(_this)  (HAS_VIDEO(_this) || HAS_AUDIO(_this) || HAS_SUB_T(_this) || HAS_TEL_T(_this))

////////////////////////////////////////////////////////////////////////////////

#define DTV_WARNING(x) { printf x; }
#define DTV_INFO(x)    { printf x; }

#define DTV_MEMCPY(_dst, _src, _len)  memcpy(_dst, _src, _len)
#define DTV_FLUSH_CACHE(_ptr, _size)  pli_flushMemory(_ptr, _size)

////////////////////////////////////////////////////////////////////////////////

class CInputDTV
{
    enum { 						
           EVENT_UPDATE_PACKET_SIZE			= 1 << 0,							
           EVENT_NEW_SEGMENT       			= 1 << 1,							
           EVENT_VIDEO_BLEND_CMD   			= 1 << 2,							
           EVENT_VIDEO_DECODE_CMD  			= 1 << 3,							
           EVENT_AUDIO_DECODE_CMD  			= 1 << 4,							
           EVENT_SPU_CONFIG        			= 1 << 5,							
           EVENT_VOBU_INFO         			= 1 << 6,							
           EVENT_FLUSH             			= 1 << 7,							
           EVENT_FLUSH_AUDIO       			= 1 << 8,							
           EVENT_FLUSH_SPIC        			= 1 << 9,							
           EVENT_FLUSH_TT          			= 1 << 10,							
           EVENT_EOS               			= 1 << 11,							
           EVENT_NEW_POSITION      			= 1 << 12,				
           EVENT_RESET_SPEED       			= 1 << 13,							
           EVENT_VIDEO_VC1_SEQ_INFO			= 1 << 14,		
           EVENT_NEW_DEMUX_TYPE    			= 1 << 15,							
           EVENT_VOLUME_FULL       			= 1 << 16,							
           EVENT_WRITE_ERROR       			= 1 << 17,							
           EVENT_UNSUPPORTED_FORMAT 		= 1 << 18,
           EVENT_UPDATE_DEMUX_AUDIO_TARGET	= 1 << 19,
           EVENT_NEW_VIDEO_TYPE				= 1 << 20,
           EVENT_DECODE_THUMBNAIL			= 1 << 21,
           EVENT_SEGMENT_END				= 1 << 22,   
           EVENT_PLAYBACK_NO_SIGNAL			= 1 << 23,
           EVENT_MP4_VIDEO_FORMAT           = 1 << 24,
           EVENT_ERROR_CONCEALMENT_LEVEL	= 1 << 25,
           EVENT_UPDATE_AVSYNC_MODE			= 1 << 26,
           EVENT_DCICCI						= 1 << 27,
           EVENT_APS_STATUS					= 1 << 28,
		   
    };

	enum {
		STATUS_RE_SYNC                      = 1 << 0,
		STATUS_READ_ERROR                   = 1 << 1,
		STATUS_BITSTREAM_ERROR              = 1 << 2,
		STATUS_WAIT_SEGMENT_END             = 1 << 3,
		STATUS_WAIT_PRES_END                = 1 << 4,
		STATUS_WAIT_SCAN_STEP               = 1 << 5,
		STATUS_PAUSE_RECORD                 = 1 << 6,
		STATUS_RESUME_RECORD                = 1 << 7,
		STATUS_GOTO_NEXT_THUMBNAIL          = 1 << 8,
		STATUS_SI_RESET_POSITION            = 1 << 9,
		STATUS_RESYNC_FAIL                  = 1 << 10,
		STATUS_PRINT_VIDEO_PID              = 1 << 11,
		STATUS_PRINT_AUDIO_PID              = 1 << 12,
		STATUS_WAIT_MAIN_PATH_COMPLETE      = 1 << 13,
		STATUS_WAIT_SUB_PATH_COMPLETE       = 1 << 14,
		STATUS_ABREPEAT_CANCELED            = 1 << 15,
    };

    enum { TARGET_VIDEO,
           TARGET_AUDIO,
           TARGET_SUBTITLE,
           TARGET_TELETEXT,
           TARGET_CHECK_SCRAMBLE,
           TARGET_VIDEO_2ND,
           NUM_OF_TARGETS,
           
    };

    enum { BITRATE_ESTIMATED_BY_PCR,
           BITRATE_ESTIMATED_BY_PTS
    };

public:

    CInputDTV()  {}
    ~CInputDTV() { Unload(); }

    // input plugin initialization
    HRESULT Open(ENUM_MEDIA_TYPE majorType, ENUM_MEDIA_TYPE subType, osal_mutex_t* mutex, INPUTPLUGIN* pInputPlugin);

    // input plugin functions
    static HRESULT loadMedia(void* pInstance, char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
    static HRESULT unloadMedia(void* pInstance);
    static HRESULT getMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
    static HRESULT read(void* pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    static HRESULT updatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    static HRESULT privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    static HRESULT execUserCmd(void* pInstance, NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    static HRESULT propertySetGet(void* pInstance, NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    static HRESULT identify(void* pInstance, char* path, unsigned char* streamBuffer, unsigned int streamBufferBytes, unsigned int streamBufferOffset);
    static HRESULT registerIOPlugin(void* pInstance, IOPLUGIN* ioPlugin, bool b_opening, bool b_direct_io, void* fh);
    static HRESULT dispose(void* pInstance);
	static HRESULT getIOBufferFullness(void* pInstance, int* fullness, int* pFlag);

    // functions for SI engine outgoing interface
    static int insertFilterList(unsigned int navHandle, unsigned int count, SI_FILTER_DESC* filterDescArray);
    static int removeFilterList(unsigned int navHandle, unsigned int count, unsigned int* pidArray, SI_FILTER_TYPE* typeArray);
    static int removeAllFilters(unsigned int navHandle);
    static int setActiveComponent(unsigned int navHandle, SI_FILTER_TYPE type, int pid);
    static int postEvent(unsigned int navHandle, unsigned int eventId, unsigned char* eventData, unsigned int eventSize);
    static int setSiState(unsigned int navHandle, unsigned char* pStateData, unsigned int bytes);
	static int resetPosition(unsigned int navHandle);
	static SI_SCRAMBLE_CHECK_RESULT checkScramble(unsigned int navHandle, int pid, int* pIsScramble);
	
    // for state feedback from GeneralIFO
    static void stateFeedback(void* instance, CGeneralIFO::STATE_TYPE type, void* state, int bytes);

	static void feedbackPrivateInfo(unsigned int navHandle, SI_PRIVATE_INFO_ID id, long infoLen, unsigned char* pInfo);
protected:

    // methods exposed via the input plugin functions
    HRESULT Load(char* path, bool* pbContinue);
    HRESULT Unload();
    HRESULT Read(NAVBUF* pBuffer, unsigned int prohibitedChMask);
    HRESULT UpdatePosition(PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    HRESULT PropertySetGet(NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
	HRESULT Read_TunerMode(NAVBUF* pBuffer);
	HRESULT Load_TunerMode(char* path, bool* pbContinue);
		
    // methods exposed via the SI engine outgoing interface functions
    int InsertFilterList(unsigned int count, SI_FILTER_DESC* filterDescArray);
    int RemoveFilterList(unsigned int count, unsigned int* pidArray, SI_FILTER_TYPE* typeArray);
    int RemoveAllFilters();
    int SetActiveComponent(SI_FILTER_TYPE type, int pid);
	SI_SCRAMBLE_CHECK_RESULT CheckScramble(int pid, int* pIsScramble);
	void FeedbackPrivateInfo(SI_PRIVATE_INFO_ID id, long infoLen, unsigned char* pInfo);
	
    // attach SI engine
    void InitSiEngine(unsigned int siHandle);

    // set recording mode (on/off, with/without preview)
    HRESULT SetRecordMode(NAVRECORDMODE* pRecordMode, bool bLocked);

    // handle events if any of the event flags is set (return true to return to nav-filter)
    bool HandleEvents(NAVBUF* pBuffer);

    // handle things when flush is requested
    void OnFlush();

    // handle things when read-error or invalid TS stream is encountered
    void OnReadError(NAVBUF* pBuffer, int status);

    // handle things when read-position catches record-position
    void OnCatchRecording(NAVBUF* pBuffer);

    // handle things related to locating the next scanning step
    void GotoNextScanningStep();

    // helper who helps packing private info for delivery
    inline void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer)
    {
    	pBuffer->type              = NAVBUF_DATA;
    	pBuffer->data.channelIndex = 0;
    	pBuffer->data.startAddress = 0xFFFFFFFF;
    	pBuffer->data.payloadSize  = 0;
    	pBuffer->data.infoId       = infoId;
    	pBuffer->data.infoData     = (unsigned char*)infoData;
    	pBuffer->data.infoSize     = infoSize;
	}

    // helper who helps packing event info for delivery
    inline void PackEvent(unsigned int eventId, void* eventData, unsigned int eventSize, NAVBUF* pBuffer)
    {
    	pBuffer->type            = NAVBUF_EVENT;
    	pBuffer->event.eventId   = eventId;
    	pBuffer->event.eventData = (unsigned char*)eventData;
    	pBuffer->event.eventSize = eventSize;
	}


    // update TS demux target after SI engine updates the pid filter list, or preview/scanning mode-change
    void UpdateTsDemuxTarget(NAVBUF* pBuffer);

    // update PS demux target after entering a PS segment, or scanning mode-change
    void UpdatePsDemuxTarget(NAVBUF* pBuffer);

    // helper who gets target index from SI filter type
    int GetTargetIdxFromType(SI_FILTER_TYPE type);

    // playback status enquiry
    HRESULT GetPlaybackStatus(NAVPLAYBACKSTATUS* status);

    // re-sync bit-stream (return offset in bytes where sync-word is found; -1 if failed)
    int ResyncStream(unsigned char* pData, unsigned int bytes);

#if defined(DVB_T)
    bool ParseVideoInfoFromPMT(unsigned char* pPMT, SI_CHANNEL_VIDEO_INFO* pInfo);
    bool ParseAudioInfoFromPMT(unsigned char* pPMT, SI_CHANNEL_AUDIO_INFO* pInfo, SI_LPCM_INFO* pLpcmInfo);
    bool ParseTSonLoad(NAVIOINFO* pInfo);
#endif
    void ReleaseTsBuffer();
    void ReleasePreParsingBuf();

	HRESULT PlayAtTime(int time);
	
	HRESULT DeliverPrivateInfoAccordingType(int target, SI_CODEC_TYPE codecType);
			
    // estimate TS file bit-rate
    unsigned int EstimateBitRateForHttp(NAVIOINFO* pIOInfo, char byWhat, bool* pbContinue);
    unsigned int EstimateBitRate(NAVIOINFO* pIOInfo, char byWhat, bool* pbContinue);
    HRESULT GetPCR(unsigned char* pBuffer, int bufferSize, long long* pPCRPos, long long* pPCR, int* pPCRPID, bool* pbLinear, bool* pbContinue, bool isForward = true);
    HRESULT GetPTS(unsigned char* pBuffer, int bufferSize, long long* pPTSPos, long long* pPTS, int* pPCRPID, bool* pbContinue);
    HRESULT GetMiddleRate(long long* pPCR, long long* pPCRPos, int entrySize, unsigned int* pBitRate, bool* pbLinear, bool* pbContinue, bool bCareLinear);

	void StoreFilterPrivateData(int searchIdx, SI_FILTER_DESC* filterDesc);

	
	NAV_AVSYNC_MODE GetAVSyncMode();
	
protected:

    osal_mutex_t   		m_siMutex;
    osal_mutex_t*  		m_pNavMutex;
    bool           		m_bUpdateDemuxTarget;
    bool           		m_bLoaded;
    bool           		m_bSiEngineAttached;
    bool				m_bStopping;
    bool           		m_bRunning;
    bool				m_bStepping;	//workaround for bug 10110
    bool           		m_bRepeat;
    bool           		m_bDirectIO;
    bool           		m_bWriting;
    long           		m_fatalErrCode;
    long				m_unsupportedFormat;
    long           		m_rate;
    unsigned int		m_status;
    unsigned int   		m_events;
    unsigned int   		m_tsPacketSize;
    unsigned char* 		m_pTsBuffer;
    unsigned char*		m_pBufForPreParsing;	// for HTTP and HLS, this buffer keeps the first 188*1024 bytes of bitstream for parse PAT/PMT.
    unsigned int		m_sizeOfBufForPreParsing;
    bool				m_bIsTsParsedOnLoad;	// true means PAT/PMT has been parsed on Load().
    int				m_bIOHttpOrHLS; // 1: TS only (from Http or Udp or Rtp), 2: HLS
    bool				m_bHLSNotSeekAtLoad;
    bool				m_bIsSeekSuccessBetweenLoadAndRead;
    IOPLUGIN			m_ioParsePCR;
    void*				m_ioHandleParsePCR;
    long long			m_1stPCRPos;
    long long			m_1stPCRTime;
    long long			m_lastPCRPos;
    long long			m_lastPCRTime;
    //osal_thread_t		m_get_last_PCR_thred_id;
    IFlowManager*  		m_pFlowManager;
    NAV_RECORD_MODE		m_recordMode;
    NAV_PREVIEW_MODE	m_previewMode;
    int             	m_waitCount;

	CDtvFileEditor*		m_pFileEditor;
	
    // members for file-mode
    CDtvFileReader* m_pFileReader;
    IOPLUGIN*       m_pIOPlugin;      // IO plugin (if non-NULL, we're in file-mode)
    void*           m_fileHandler;    // represents the opened file by IO plugin
    bool			m_bIOSeekable;
    bool			m_bIOTimeSeekable;
	int             m_maxReadSize;    // max number of bytes to read from IO plugin each time
    unsigned int    m_residueBytes;   // number of bytes from file buffer remaining due to TS packet sync
    unsigned char*  m_residueStart;   // points to the start of residue bytes
    unsigned int    m_residueAdjust;  // residue adjustment amount after reading every m_maxReadSize bytes
    NAVBUF          m_reservedNavBuf; // sometimes we need to reserve a buffer for delivery later
    int             m_scanStepTicks;  // number of ticks (90KHz) to wait between scanning steps
    int             m_scanTicksLeft;  // number of ticks (90KHz) to wait before current scanning step ends
    int64_t         m_scanRefClock;   // keep track of reference clock value during scanning
    int             m_readErrorCount; // number of consecutive read errors
    int   			m_demuxErrorCount;// number of consecutive no video or audio data demuxed.
    ENUM_MEDIA_TYPE m_mediaType;      // current media type (for demuxer)
	int				m_maxResyncTime;
	int64_t			m_firstPTS;
	int64_t			m_elapsedPTS;
	
	unsigned int 	m_thumbnailIdx;
	NAVMULTITHUMBNAILPARA m_thumbnailPara;
	//int				m_tempAudioType[2]; 	//for bug 12439
	SI_CODEC_TYPE	m_currVideoType;	//for bug 12711
	
	
    // members for tuner-mode
    CDtvFileWriter* m_pFileWriter;
    unsigned char*  m_pRecordPointer;
    unsigned char*  m_pTSBufferLimit;
    unsigned char*  m_pReleaseData; // the position will be released next time, which should be at 94k boundary
    unsigned long   m_releaseSize;
    unsigned long   m_dataThreshold;
    unsigned long   m_recordingIdleCount;
    unsigned char   m_tunerHandle;
    unsigned char*  m_pCurrSiState;
    unsigned int    m_currSiStateSize;
	bool			m_bRecordPause;
	
    // members for SI engine
    SI_ENGINE_IN m_siEngineIn; // SI engine incoming interface
    unsigned int m_siHandle;   // SI engine handle

    // PID filter list
    SI_FILTER_DESC m_pidFilterList[SI_MAX_FILTER_NUMBER];
    unsigned int   m_pidFilterCount;

    // active targets of audio/video/subtitle/teletext
    struct {
        int  pid;
        bool bNewTarget;
		int  IsScramble;		/* -1 means scramble control info is not ready.
								 * 0 means packets are not scramble
								 * 1 means packets are scramble */
    } m_activeTarget[NUM_OF_TARGETS];

    // demux targets
    DEMUXTARGET_TS_ENTRY m_demuxTargetEntries[SI_MAX_FILTER_NUMBER];
    DEMUXTARGET_TS       m_demuxTarget;
    DEMUXTARGET          m_demuxTargetPsAudio;
    DEMUXTARGET          m_demuxTargetTsAudio;	/*  id : codecType
    												subId : sourceType ( MEDIATYPE_FILE or MEDIATYPE_STB)
    												subId2 : audio preference (only for those audio streams carrying packets with diff types)
    											 */

    // other private info
    VIDEODECODECOMMAND  m_updatedVideoDecCmd;
    VIDEOMIXERBLEND     m_updatedBlendCmd;
    AUDIODECODECOMMAND  m_updatedAudioDecCmd;
    SPUCONFIG           m_updatedSPUConfig;
    VIDEODVDVOBUINFO    m_updatedVOBUInfo;
    VIDEODECODETOBUFFER m_updatedDecodeThumbCmd;
    VIDEOVC1SEQINFO 	m_updateVC1SeqInfo;
	VIDEODIVX4 			m_MP4VideoFormat;
    unsigned char*		m_pThumbDecDone;
    unsigned long       m_context;
    long				m_errConcealmentLevel[2];	//[0] : for normal speed, [1] : for scanning mode
    unsigned int		m_thumbnailTimePos;
    
#ifndef DISABLESIGNATURE
	uint32_t 			m_mediaSignature[4];

	HRESULT ReadMediaSignature(int64_t totalBytes);
#endif    

	long				m_eioLevel;
	
	NAV_AVSYNC_MODE		m_avSyncMode;
    uint64_t 			m_audioCapability;

#if defined(ENABLE_DTV_3D)
	CDtvSubPath*   		m_pSubpath;
#endif	

	VIDEODVDDCICCI		m_dcicci;
	unsigned int		m_aps;
	
	int64_t 			m_lastSystemPTS;
	int64_t				m_tick;
	char 				m_scandone;
	ENUM_MEDIA_TYPE     m_ioType;
#if defined(DVB_T)
    SI_CHANNEL_VIDEO_INFO m_videoInfo;
    SI_CHANNEL_AUDIO_INFO m_audioInfo;
    SI_LPCM_INFO m_lpcmInfo;
    LPCM_SETTING m_filter_lpcm_data;
#endif
};

#endif //#define __C_DTV_INPUT_PLUGIN_H__
