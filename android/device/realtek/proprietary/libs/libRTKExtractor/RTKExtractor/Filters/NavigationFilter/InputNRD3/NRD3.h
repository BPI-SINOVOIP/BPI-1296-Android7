#ifndef __C_INPUT_PLUGIN_NRD3_H__
#define __C_INPUT_PLUGIN_NRD3_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <OSAL.h>
#include <Filters/PrivateInfo.h>
#include <Util.h>
#include <Platform_Lib/SocketAVData/SocketAVData.h>

class CInputNRD3
{
	enum {
		EVENT_FLUSH				= 1 << 0,
		EVENT_NEW_SEGMENT		= 1 << 1,
		EVENT_VIDEO_BLEND   	= 1 << 2,
		EVENT_VIDEO_DECODE  	= 1 << 3,							
		EVENT_AUDIO_DECODE  	= 1 << 4,
		EVENT_AUDIO_FORMAT  	= 1 << 5,
		EVENT_NEW_VIDEO_TYPE	= 1 << 6,
		EVENT_EOS			 	= 1 << 7,
		EVENT_FLUSH_AUDIO	 	= 1 << 8,
		EVENT_FLUSH_SPIC	 	= 1 << 9,
		EVENT_DCICCI		 	= 1 << 10,
		EVENT_WAIT_PRES_END     = 1 << 11,
    };
    
public:
	CInputNRD3(osal_mutex_t* mutex) 
	: m_navMutex(mutex)
	, m_pIOBuffer(NULL) {}
	
	~CInputNRD3() {}
	
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
	HRESULT ParseMediaAttribute(unsigned char* pMediaAttr);
		
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
	VIDEODVDDCICCI	   m_dcicci;
	
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

	int64_t m_elapsedPTS;
	int64_t m_firstPTS;
	
	int		m_waitCount;
};

#endif //__C_INPUT_PLUGIN_NRD3_H__
