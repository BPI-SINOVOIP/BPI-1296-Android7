#ifndef __C_INPUT_BD_PLUGIN_H__
#define __C_INPUT_BD_PLUGIN_H__


#include <Util.h>
#include <IStream.h>
#include <InbandAPI.h>
#include <AudioInbandAPI.h>
#include "VideoRPCBaseDS_data.h"
#include <Filters/PrivateInfo.h>

#include "CBdReader.h"

#define ABS(x)  ((x < 0)? -x : x)
#define HAS_VIDEO(_this)  ((_this)->m_activeTarget[TARGET_VIDEO].pid != -1)
#define HAS_AUDIO(_this)  ((_this)->m_activeTarget[TARGET_AUDIO].pid != -1)

typedef struct
{
	int mplsIdx;
	int mplsItemNo;
	int actAudStrm;
	int actPGStrm;
	int elapsedTime;
	
} BD_NAV_STATE;

class CInputBD
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
           EVENT_SPIC_OFFSET_SEQ_ID			= 1 << 23,
           EVENT_MP4_VIDEO_FORMAT           = 1 << 24,
           EVENT_ERROR_CONCEALMENT_LEVEL	= 1 << 25,	
           EVENT_UPDATE_DEMUX_VIDEO_TARGET  = 1 << 26,	   
           PENDING_SPIC_ON_OFF				= 1 << 27,
           EVENT_UPDATE_DEMUX_START_PTS		= 1 << 28,
           EVENT_UPDATE_AVSYNC_MODE			= 1 << 29,
           EVENT_VIDEO_USABILITY_INFO		= 1 << 30,
    };	
    
	enum {
		STATUS_RE_SYNC           			= 1 << 0,
		STATUS_READ_ERROR					= 1 << 1,
		STATUS_BITSTREAM_ERROR				= 1 << 2,
		STATUS_WAIT_SEGMENT_END				= 1 << 3,
		STATUS_WAIT_PRES_END				= 1 << 4,
		STATUS_WAIT_SCAN_STEP				= 1 << 5,
		STATUS_RESYNC_FAIL					= 1 << 10,
		STATUS_WAIT_SUB_PATH_COMPLETE		= 1 << 11,
		STATUS_WAIT_MAIN_PATH_COMPLETE		= 1 << 12,
	};    
	
	enum { TARGET_VIDEO,
           TARGET_AUDIO,
           TARGET_SUBTITLE,
           TARGET_CHECK_SCRAMBLE,
           NUM_OF_TARGETS,           
    };
	
public:

    CInputBD()  { 
    	m_bdReader.SetInputBDPtr(this); 
    	
    	// FIXED_CID_14522
    	m_pNavMutex = NULL;
		m_pFlowManager = NULL;    	
		m_pTsBuffer = NULL;
		m_residueStart = NULL;
		m_pIOPlugin = NULL;
		m_mplsList = NULL;
		memset(&m_demuxTarget, 0, sizeof(DEMUXTARGET_TS));
		memset(&m_pidFilterList, 0, sizeof(SI_FILTER_DESC)*SI_MAX_FILTER_NUMBER);
    }
    ~CInputBD() { Unload(); }	
    
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
    
    HRESULT LoadMoviePlayList(int mplsId, int mplsItemNo);
    
    NAV_AVSYNC_MODE GetAVSyncMode();
            
protected:

    // methods exposed via the input plugin functions
    HRESULT Load(char* path, bool* pbContinue);
    HRESULT Unload();
    HRESULT Identify(char* path);
    HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);    
    HRESULT GetMediaInfo(NAVMEDIAINFO* pMediaInfo);
    HRESULT PropertySetGet(NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    HRESULT Read(NAVBUF* pBuffer, unsigned int prohibitedChannelMask);
    HRESULT UpdatePosition(PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    
    // handle things when flush is requested
    void OnFlush();
    
    // handle events if any of the event flags is set (return true to return to nav-filter)
    bool HandleEvents(NAVBUF* pBuffer);
    
    // helper who helps packing private info for delivery
    void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer);

    // helper who helps packing event info for delivery
    void PackEvent(unsigned int eventId, void* eventData, unsigned int eventSize, NAVBUF* pBuffer);    
    
    // re-sync bit-stream (return offset in bytes where sync-word is found; -1 if failed)
    int ResyncStream(unsigned char* pData, unsigned int bytes);
    
    int InsertFilterList(unsigned int count, SI_FILTER_DESC* filterDescArray);
    int RemoveFilterList(unsigned int count, unsigned int* pidArray, SI_FILTER_TYPE* typeArray);
    int RemoveAllFilters();
    int SetActiveComponent(SI_FILTER_TYPE type, int pid, bool bActive);
    
    // helper who gets target index from SI filter type
    int GetTargetIdxFromType(SI_FILTER_TYPE type);
    
    void StoreFilterPrivateData(int searchIdx, SI_FILTER_DESC* filterDesc);
    
    // update TS demux target after SI engine updates the pid filter list, or preview/scanning mode-change
    void UpdateTsDemuxTarget(NAVBUF* pBuffer);
    
    // playback status enquiry
    HRESULT GetPlaybackStatus(NAVPLAYBACKSTATUS* status);
    
    HRESULT PlayAtPosition(int64_t pos);
    HRESULT PlayAtTime(int64_t second);
    HRESULT PlayAtTitle(int title);
    HRESULT SetAudioStream(int audioStrm);	// 0: disable audio
    HRESULT SetSubpictureStream(int subtitleStrm, NAV_DISPLAY_STATE displayState);
    HRESULT PlayChapter(int titleNum, int chapterNum);
    HRESULT PlaySegment(int titleNum, unsigned int startSeconds, unsigned int startFrameIdx,
            			unsigned int endSeconds, unsigned int endFrameIdx, bool bRepeat);
    
    HRESULT GetDiscStatus(NAVDISCSTATUS* status);
    HRESULT GetTitleStatus(NAVTITLESTATUS* status, int titleNum);
    HRESULT GetAudioStatus(NAVAUDIOSTATUS* status);
    HRESULT GetSpicStatus(NAVSPICSTATUS* status);
    
    // handle things related to locating the next scanning step
    void GotoNextScanningStep();
    
    void OnReadError(NAVBUF* pBuffer, int status);    
   
    HRESULT GotoNextPlayItem();
#if defined(ENABLE_BLURAY_3D)
    HRESULT RemoveUnusedPID();
    bool CheckInsertOrNot(int pid, SI_CODEC_TYPE codecType, bool bActive);
    void SetOffsetSeqID(NAVBUF* pBuffer);
    HRESULT HandleMainPathReadComplete(NAVBUF* pBuffer);
    void    HandleSubPathReadComplete();
#endif    	
protected:
	
	osal_mutex_t*  		m_pNavMutex;
	bool           		m_bLoaded;
	IFlowManager*  		m_pFlowManager;
	CBdReader			m_bdReader;
	unsigned int   		m_events;	
	unsigned int		m_status;
	long           		m_rate;	
	unsigned int   		m_tsPacketSize;
	long				m_unsupportedFormat;
	NAVBUF          	m_reservedNavBuf; 	// sometimes we need to reserve a buffer for delivery later
	unsigned char* 		m_pTsBuffer;
	unsigned int    	m_residueBytes;   	// number of bytes from file buffer remaining due to TS packet sync
    unsigned char*  	m_residueStart;   	// points to the start of residue bytes
    unsigned int    	m_residueAdjust;  	// residue adjustment amount after reading every m_maxReadSize bytes	
    int64_t         	m_scanRefClock;   	// keep track of reference clock value during scanning
	int             	m_maxReadSize;
	long           		m_fatalErrCode;
	bool           		m_bUpdateDemuxTarget;
	bool           		m_bRunning;
	bool				m_bStepping;		//workaround for bug 10110
	NAV_REPEAT_MODE		m_repeatMode;
	unsigned long       m_context;
	bool				m_updatedSpicOnOff;
	int             	m_readErrorCount; 	// number of consecutive read errors
	
	IOPLUGIN*       	m_pIOPlugin;      	// IO plugin (if non-NULL, we're in file-mode)    
	ENUM_MEDIA_TYPE 	m_mediaType;      	// current media type (for demuxer)     
	
	int					m_mplsNum;			// number of movie playlist
	NAVBDTITLELIST*		m_mplsList;		// an array of movie playlist
	int					m_mplsIdx;			// the current playing title index
	
	
	// demux
	DEMUXTARGET_TS_ENTRY m_demuxTargetEntries[SI_MAX_FILTER_NUMBER];
    DEMUXTARGET_TS       m_demuxTarget;
	DEMUXTARGET        	 m_demuxTargetTsAudio;	/*  id : codecType
    												subId : sourceType ( MEDIATYPE_FILE or MEDIATYPE_STB)
    												subId2 : audio preference (only for those audio streams carrying packets with diff types)
    											 */
    											 
	// other private info
    VIDEODECODECOMMAND  m_updatedVideoDecCmd;
    VIDEOMIXERBLEND     m_updatedBlendCmd;
    AUDIODECODECOMMAND  m_updatedAudioDecCmd;    
    SPUCONFIG           m_updatedSPUConfig;		
    long				m_errConcealmentLevel[2];	//[0] : for normal speed, [1] : for scanning mode	
    VIDEOVC1SEQINFO 	m_updateVC1SeqInfo;
    VIDEODVDVOBUINFO    m_updatedVOBUInfo;
    
    uint32_t 			m_mediaSignature[4];
    
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
    
    int             	m_waitCount;
    int64_t				m_firstPTS;
	int64_t				m_elapsedPTS;			
    int             	m_scanStepTicks;  // number of ticks (90KHz) to wait between scanning steps
    int             	m_scanTicksLeft;  // number of ticks (90KHz) to wait before current scanning step ends	
    
#if 1	// fixbug 18043
	unsigned long		m_startPTS;
#endif    

struct { int          title;
         int64_t	  ptsA;
         int64_t	  ptsB;
         bool         bRepeat;  

         bool		  bWaitingJump;
       } m_repeatAB; // AB repeat region
    
#ifndef DONTFIXBUG21352_21355
	int					m_actVidIdxInClip;
	int					m_actAudIdxInClip;
	int					m_actSpicIdxInClip;
#endif    
#ifndef DONTFIXBUG21884 
	uint64_t 			m_audioCapability;
#endif

	NAV_AVSYNC_MODE		m_avSyncMode;
};    

#endif //#define __C_INPUT_BD_PLUGIN_H__
