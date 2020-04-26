#ifndef __C_PRINT_OUT_FILTER_H__
#define __C_PRINT_OUT_FILTER_H__

#include <CStream.h>
//#include <assert.h>
//#include "CFilter.h"
#include <StreamClass/CMemAllocator.h>

#include "io/dvr_ifo_types.h"
#include "io/dvr_ifo_access.h"
#include <Util.h>
#include <Filters/CDVDProgMux.h>
#include <Filters/FileNavFilter/CDubbingPin.h>
//#include <Filters/FileNavFilter/CFileNavFilter.h>

typedef ulCircularQueue<PRINTOUT_IQUEUE_ELEMENT> VRO_PKT_QUEUE;

class CDubbingFilter: public CBaseFilter{
friend class	CDubbingInPin;
public:
                CDubbingFilter(void);
				~CDubbingFilter(void);
    HRESULT     Stop(void);
    HRESULT     Pause(FILTER_STATE State);
    HRESULT     Run(void);
	void		init(void);
	HRESULT		Store_VOBU(PROG_STREAM_VOBU_INFO *VOBUInfo);
	void		SetTVSystem(uint8_t i8TVSystem);

    
protected:
#ifdef RING_BUF 
    bool		InitRingData(void);
#endif      
	size_t		VROWriteBytes(int fhIdx, void *data, size_t byte_size);
	int			FileOpen(void);
    static void DubbingThreadEntry(void* pThis);
    bool		DubbingThread(void);
protected:
    CDubbingInPin*		m_pInPin; 
    CDubbingInfoInputPin* m_pInfoInputPin;
#ifdef RING_BUF      
    IMemRingAllocator*	m_pAllocator; 
    IReadPointerHandle* m_pRPHandle;
    VRO_PKT_QUEUE		m_VROQueue;     
#else  
    IMemAllocator*		m_pAllocator;
    VoidArrSem			m_VROQueue;
#endif    
    CThread*            m_pThread;    
    int					m_fhVRO;
    bool				m_bEndOfStream;
    bool				m_bIsSendEndEvent;
    bool				m_bIsEndOfCell;

	PROG_STREAM_VOBU_INFO	*m_VOBUBuf;
	int					m_nVOBUBufStart;
	int					m_nVOBUBufEnd;    
	uint8_t				m_i8TVSystem;
};

#endif
