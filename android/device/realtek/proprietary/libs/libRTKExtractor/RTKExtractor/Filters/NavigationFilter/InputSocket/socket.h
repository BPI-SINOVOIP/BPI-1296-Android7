#ifndef __C_SOCKET_INPUT_PLUGIN_H__
#define __C_SOCKET_INPUT_PLUGIN_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <OSAL.h>
#include <Filters/PrivateInfo.h>
#include <Util.h>
#include <Platform_Lib/SocketAVData/SocketAVData.h>
#include <Filters/NavigationFilter/SiEngine.h>
#include <Platform_Lib/TSDescrambler/TSDescrambler.h>
#include "../CommonUtility/MiniSIEngine/MiniSIEngineInterface.h"
#include "../CommonUtility/MiniSIEngine/MiniSIEngine.h"
#include <Filters/NavigationFilter/NavPrintfMacro.h>

class CInputSocket
{
	enum {
		EVENT_FLUSH					= 1 << 0,
		EVENT_NEW_SEGMENT			= 1 << 1,
		EVENT_VIDEO_BLEND			= 1 << 2,
		EVENT_VIDEO_DECODE			= 1 << 3,							
		EVENT_AUDIO_DECODE			= 1 << 4,
		EVENT_AUDIO_FORMAT			= 1 << 5,
		EVENT_NEW_VIDEO_TYPE		= 1 << 6,
		EVENT_EOS					= 1 << 7,
		EVENT_FLUSH_AUDIO			= 1 << 8,
		EVENT_FLUSH_SPIC			= 1 << 9,
		EVENT_UPDATE_DEMUX_TARGET	= 1 << 10,
		EVENT_TS_PACKET_SIZE		= 1 << 11,
		EVENT_AUDIO_DEMUX_TARGET	= 1 << 12,
		EVENT_IV					= 1 << 13,
		EVENT_KEY					= 1 << 14,
		EVENT_DCICCI				= 1 << 15,
		EVENT_WAIT_PRES_END         = 1 << 16,
		EVENT_NEW_DEMUX_TYPE        = 1 << 17,
		EVENT_FJPEG_PIC_INFO	    = 1 << 18,
		EVENT_VIDEO_DPBBYPASS	    = 1 << 19,
		EVENT_VIDEO_FORCE_BUFFER_RESOLUTION = 1 <<20,
		EVENT_ERROR_CONCEALMENT_LEVEL	= 1 << 21,
		EVENT_DECODE_IGNORE_RATIO	= 1 << 22,
		EVENT_VC1_SEQ_INFO          = 1 << 23,
		EVENT_MPEG4_INFO            = 1 << 24,
		EVENT_MJPG_INFO             = 1 << 25,
		EVENT_OFFSET_TO_PHY_ADDR    = 1 << 26,
    };
    
    enum {
		STATUS_SPU_ON_OFF           = 1 << 0,
    };
    
    enum { TARGET_VIDEO = 0,
           TARGET_AUDIO,
           TARGET_PG,
           TARGET_PAT,
           TARGET_PMT,
           NUM_OF_TARGETS,
    };
public:
	CInputSocket(osal_mutex_t* mutex) 
	: m_navMutex(mutex)
	, m_pIOBuffer(NULL) {}
	
	~CInputSocket() {}
	
private:
	
	HRESULT Load(char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
	HRESULT UnLoad();	
	HRESULT GetMediaInfo(NAVMEDIAINFO* pMediaInfo);
	HRESULT Read(NAVBUF* pBuffer);
	HRESULT UpdatePlaybackPosition(PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
	HRESULT PrivateDataFeedback(unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
	HRESULT PropertySetGet(NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
	HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);

	bool    HandleEvents(NAVBUF* pBuffer);
	void	OnFlush();
	HRESULT ParseMediaAttribute(unsigned char* pMediaAttr, int size);
	HRESULT ParseVideoAttribute(unsigned char* attr, int size);
	HRESULT ParseAudioAttribute(unsigned char* attr, int size);
	HRESULT ParseContainerAttribute(unsigned char* attr, int size);
	HRESULT ParseSpuAttribute(unsigned char* attr, int size);
	
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
	
	inline void PackEvent(unsigned int eventId, void* eventData, unsigned int eventSize, NAVBUF* pBuffer)
	{
    	pBuffer->type            = NAVBUF_EVENT;
    	pBuffer->event.eventId   = eventId;
    	pBuffer->event.eventData = (unsigned char*)eventData;
    	pBuffer->event.eventSize = eventSize;
	}
	
	inline bool IsToBeDrop(long auType, NAVBUF* pBuffer)
	{
		if(m_flush.bOn && 
		  (m_flush.dataType == 0 || m_flush.dataType == auType))
		{
			pBuffer->type = NAVBUF_WAIT;
        	pBuffer->wait.waitTime = 16;
        	return true;
		}	
		return false;
	}
	
#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
	void ClearReadCountVariables();
	void CheckReadPtsDiffBigCount(int readBytes);
	void InitDebugVariables();
	void SavePacketPos(unsigned int rtp_seq, unsigned int offset, packet_pos* pPos, char showForVideoFrameEnd);
	HRESULT CheckPCRPacket(unsigned int bufSize, unsigned char* buf);
#endif
	
	void UpdateTsDemuxTarget(NAVBUF* pBuffer);
	void RemoveAllPID(bool bExceptPAT);
	void UpdateTsAudioDemuxTarget(NAVBUF* pBuffer);
	void InitTsTarget();
	
	HRESULT HandleSocketInfo(SOCKET_INFO_TYPE flag, long infoSize, const unsigned char* infoData, NAVBUF* pBuffer);
	HRESULT GetPrivateData(int target, SI_CODEC_TYPE siType, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
	
	int SiEventHandler(MINI_SI_EVENT_T* pEvent);
	
	void RemoveRtpHeader(unsigned char** ppPayload, int* pPayloadSize);
public:
	//_________________________________________________________________________
	// input plugin export functions
	//_________________________________________________________________________
	
    static HRESULT loadMedia(void* pInstance, char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
    static HRESULT unloadMedia(void* pInstance);
    static HRESULT getMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
    static HRESULT read(void* pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    static HRESULT updatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    static HRESULT privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    static HRESULT execUserCmd(void* pInstance, NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    static HRESULT propertySetGet(void* pInstance, NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    static HRESULT dispose(void* pInstance);	
	static HRESULT getIOBufferFullness(void* pInstance, int* fullness, int* pFlag);
    static HRESULT registerIO(void* pInstance, IOPLUGIN* ioPlugin, bool b_opening, bool b_direct_io, void* fh);

 	static int siEventHandler(long instanceID, MINI_SI_EVENT_T* pEvent);
 	
private:
	osal_mutex_t*      m_navMutex;
	IOPLUGIN* 		   m_ioPlugin;
	void*			   m_fileHandler;
	unsigned char*     m_pIOBuffer;
	NAVIOINFO          m_ioInfo;
	
	VIDEODECODECOMMAND m_videoDecCmd;
	AUDIODECODECOMMAND m_audioDecCmd;
	VIDEOMIXERBLEND    m_videoBlendCmd;
	AUDIOFORMAT        m_audioFormat;
	SOCKET_MEDIA_TYPE  m_videoType;
	SOCKET_MEDIA_TYPE  m_audioType;
	SOCKET_MEDIA_TYPE  m_spuType;
	ENUM_MEDIA_TYPE    m_mediaType;
	VIDEODVDDCICCI	   m_dcicci;
	VIDEODIVX4         m_divx4;
	VIDEOVC1SEQINFO    m_vc1Info;
	
	
	long           	   m_fatalErrCode;
	long               m_events;
	long               m_status;
	bool               m_bStop;
	
	
	struct {
		bool bOn;
		long dataType;	//0 : all data, 1 : video data, 2 : audio data, 3 : spic data
		long stamp;
		
		void reset() {bOn = false;}
		void start(long type, long num) {bOn = true; dataType = type; stamp = num;}
	} m_flush;
	
	///////////////////// for ts container ///////////////////////

	struct {
		long            pid;
		bool            bNewTarget;
		ENUM_MEDIA_TYPE audioPreference;
		int             privateDataSize;
		void*           privateData;
		
	} m_tsTarget[NUM_OF_TARGETS];
	
	long                 m_tsPacketSize;
	DEMUXTARGET_TS		 m_demuxTarget;
	DEMUXTARGET_TS_ENTRY m_demuxTargetEntries[SI_MAX_FILTER_NUMBER];
	DEMUXTARGET          m_demuxTargetTsAudio;	/*  id : codecType
    												subId : sourceType ( MEDIATYPE_FILE or MEDIATYPE_STB)
    												subId2 : audio preference (only for those audio streams carrying packets with diff types)
    											 */
	MiniSIEngine*       m_pMiniSI;
	bool                m_bMonitorPAT;
	bool                m_bUpdateDemuxTarget;
	
	bool				m_bValidIV;
	bool				m_bValidKey;
	unsigned char 		m_IV[16];
	NAVDEMUXKEY			m_key;
	
    											 
	int64_t 			m_elapsedPTS;
	int64_t 			m_firstPTS;
	
	int					m_waitCount;
	
	VIDEOFJPEGPICINFO	m_picInfo;
	VIDEOFORMAT			m_resolution;
	int			m_context;
	SOCKET_APP_TYPE		m_appType;
	SOCKET_CONTAINER_TYPE m_containerType;
	
	DESCRAMBLE_CONFIG   m_desConfig;
	NAVDEMUXCWOFFSET	m_ivOffset;
	NAVDEMUXCWOFFSET	m_keysOffset;
	long m_errConcealmentLevel;
	
	bool                m_spicOnOff;
	unsigned int        m_spicLangCode;
	EventSpicFrame      m_spicFrame;
        
	NAVIOBUFMAPINFO     m_inputBufInfo;

	unsigned short		m_pcrPid;
};

#endif //__C_SOCKET_INPUT_PLUGIN_H__
