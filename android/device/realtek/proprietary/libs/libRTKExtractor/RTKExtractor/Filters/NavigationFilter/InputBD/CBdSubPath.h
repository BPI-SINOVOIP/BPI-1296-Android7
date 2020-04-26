#ifndef __C_BD_SUBPATH_READER_H__
#define __C_BD_SUBPATH_READER_H__
#include <Util.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/PrivateInfo.h>
#include <Filters/NavigationFilter/SiEngine.h>
#include "CBdParser.h"
#include "EPMap.h"

class CBdSubPath
{
	friend class CBdReader;
	
	enum { TARGET_VIDEO,
           TARGET_SUBTITLE,
           
           NUM_OF_TARGETS,           
    };
    
	enum { 						
           EVENT_SUBPATH_NEW_VIDEO_TYPE			= 1 << 0,	
           EVENT_SUBPATH_UPDATE_DEMUX_TARGET    = 1 << 1,
           EVENT_SUBPATH_NEW_SEGMENT			= 1 << 2,
           //EVENT_SUBPATH_VIDEO_DECODE			= 1 << 3,
           EVENT_SUBPATH_PACKET_SIZE			= 1 << 3,
           EVENT_SUBPATH_EOS					= 1 << 4,
           EVENT_SUBPATH_VOBU					= 1 << 5,
           
    };
    
    enum { 						
           STATUS_SUBPATH_READ_COMPLETE			= 1 << 0,	
           STATUS_RE_SYNC						= 1 << 1,
           STATUS_RESYNC_FAIL					= 1 << 2,
    };
public : 
	CBdSubPath(CBdParser* pBdParser) ;
	~CBdSubPath() ;
	
	
	int SelectMVCSubPath(bdt_pl_t* pPLInfo);
	void UnloadSubPlayItem();
	bool LoadSubPlayItem(bdt_pl_t* pPLInfo, int subPathNo, int itemNo);
	void DetermineEPMap();
	
	bool HandleEvents(NAVBUF* pBuffer);
	bool UpdateTsDemuxTarget(NAVBUF* pBuffer);
		
	bool OnFlush();
	bool NewPosition();
	bool GetSPNByPTS(int64_t pts, int* pSPN);
	bool GotoTimePosition(int64_t pts);
	bool UpdateScanLimit();
	bool LocateNextIFrame(int64_t targetPTS, int64_t* pPTS, int* pSPN);
	bool Read(NAVBUF* pBuffer);
	bool SetSpeed(int speed, int scanningSecStride);
	
	inline bool UpdatePresentSPN(int64_t pts) {return GetSPNByPTS(pts, &m_presentSPN);}
	inline bool OnStop() {SET_FLAG(m_events, EVENT_SUBPATH_EOS); return true;}
	inline bool IsLoad() { return (m_fd != -1);}
	inline void RemoveAllFilters()
	{
    
    	for(int i = 0; i < NUM_OF_TARGETS; i++)
    	{
        	m_activeTarget[i].pid = -1;
        	m_activeTarget[i].bNewTarget = true;
        	
    	}
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
	int			m_subPathNo;
	int			m_progNo;
	int			m_streamNo;
	int			m_videoCodec;
	int			m_tsPacketSize;
	bdt_cl_t	m_clpiInfo;
	CBdParser*  m_pBdParser;
	
	int 		m_speed;
	int 		m_scanningSecStride;
	
	/* reading */
	unsigned char* m_pTsBuffer;
	int			m_fd;

	int			m_presentSPN;
	int			m_readSPN;
	int			m_scanLimitSPN;
	long   		m_fatalErrCode;
	int			m_readPacketCount;
	int			m_residueBytes;
	int    		m_residueAdjust; 
	unsigned char*  	m_residueStart;
	EPMap 		m_epMap;
	
	
	struct {
		int  pid;
		int  codecType;
		bool bNewTarget;
	} m_activeTarget[NUM_OF_TARGETS];
	
	DEMUXTARGET_TS_ENTRY m_demuxTargetEntries[SI_MAX_FILTER_NUMBER];
    DEMUXTARGET_TS       m_demuxTarget;
    VIDEODVDVOBUINFO 	 m_vobuInfo;
};

#endif //__C_BD_SUBPATH_READER_H__
