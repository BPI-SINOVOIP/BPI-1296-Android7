#ifndef __C_DTV_SUBPATH_READER_H__
#define __C_DTV_SUBPATH_READER_H__
#include <Util.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/PrivateInfo.h>
#include <Filters/NavigationFilter/SiEngine.h>


class CDtvSubPath
{
	friend class CInputDTV;
	
	enum { TARGET_VIDEO,
           TARGET_SUBTITLE,
           
           NUM_OF_TARGETS,           
    };
    
	enum { 						
           EVENT_SUBPATH_NEW_VIDEO_TYPE			= 1 << 0,	
           EVENT_SUBPATH_UPDATE_DEMUX_TARGET    = 1 << 1,
           EVENT_SUBPATH_NEW_SEGMENT			= 1 << 2,
           EVENT_SUBPATH_PACKET_SIZE			= 1 << 3,
           EVENT_SUBPATH_EOS					= 1 << 4,
           EVENT_SUBPATH_VOBU					= 1 << 5,
           
    };
    
    enum { 						
           STATUS_MAINPATH_READ_COMPLETE		= 1 << 0,
           STATUS_SUBPATH_READ_COMPLETE			= 1 << 1,	
           STATUS_RE_SYNC						= 1 << 2,
           STATUS_RESYNC_FAIL					= 1 << 3,
           STATUS_SUBPATH_USE_DIRECTIO			= 1 << 4,
    };
public : 
	CDtvSubPath(IOPLUGIN* pIOPlugin, char* path, osal_mutex_t* pNavMutex, int packetSize, int exp) ;
	~CDtvSubPath() ;
	
	int InsertVideoPid(SI_FILTER_DESC* filterDescArray);
	
	bool HandleEvents(NAVBUF* pBuffer);
	bool UpdateTsDemuxTarget(NAVBUF* pBuffer);
		
	bool OnFlush();
	bool Seek(char* path, int readPos);
	
	bool SetSpeed(int speed, int scanningSecStride);
	inline void UpdateScanLimit(int scanLimit) 
	{
		m_scanLimit = scanLimit;
		SET_FLAG(m_events, EVENT_SUBPATH_VOBU);
	}
	inline bool IsAtScanLimit() 
	{
	#ifdef SINGLE_RP_FOR_MVC_TS 
		return (m_speed != 256);
	#else	
		return (m_speed != 256 && m_readPos >= m_scanLimit);
	#endif	
	}
	
	bool ReadSubPath(NAVBUF* pBuffer, bool bMainPathFull, int& mainPathReadCount);
	bool Read(NAVBUF* pBuffer);
	
	inline void MainPathOnReadEnd() {SET_FLAG(m_status, STATUS_MAINPATH_READ_COMPLETE);}
	inline bool HasSubPathEvents() { return (m_3DOutput && m_events != 0);}
	inline void OnStop() { SET_FLAG(m_events, EVENT_SUBPATH_EOS); }
	inline bool GetOutputMode() { return m_3DOutput;}
	inline void SetOutputMode(bool bEnable) { m_3DOutput = bEnable;}
	
	inline void RemoveAllFilters()
	{
    	for(int i = 0; i < NUM_OF_TARGETS; i++)
    	{
        	m_activeTarget[i].pid = -1;
        	m_activeTarget[i].bNewTarget = true;
        	
    	}
    #ifndef SINGLE_RP_FOR_MVC_TS 	
    	SET_FLAG(m_events, EVENT_SUBPATH_UPDATE_DEMUX_TARGET);
    #endif	
    }
    
	inline void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer)
	{
    	pBuffer->type              = NAVBUF_DATA;
    	pBuffer->data.channelIndex = 1;
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
    	return;
	}

protected:	
	int ResyncStream(unsigned char* pData, unsigned int bytes);

protected:
	bool		m_3DOutput;
	long		m_events;
	long		m_status;
	int			m_videoCodec;
	int			m_tsPacketSize;
	
	int 		m_speed;
	int 		m_scanningSecStride;
	
	/* reading */
	unsigned char* m_pTsBuffer;
	void*          m_pFileHandle;
	IOPLUGIN*      m_pIOPlugin;
	osal_mutex_t*  m_pNavMutex;
	
	int			m_readUnitSizeExp;
	int			m_readPos;
	int			m_scanLimit;
	long   		m_fatalErrCode;
	int			m_readCount;
	int			m_residueBytes;
	int    		m_residueAdjust; 
	unsigned char*  	m_residueStart;
	
	
	struct {
		int  pid;
		int  codecType;
		bool bNewTarget;
	} m_activeTarget[NUM_OF_TARGETS];
	
	DEMUXTARGET_TS_ENTRY m_demuxTargetEntries[SI_MAX_FILTER_NUMBER];
    DEMUXTARGET_TS       m_demuxTarget;
    VIDEODVDVOBUINFO 	 m_vobuInfo;
};

#endif //__C_DTV_SUBPATH_READER_H__
