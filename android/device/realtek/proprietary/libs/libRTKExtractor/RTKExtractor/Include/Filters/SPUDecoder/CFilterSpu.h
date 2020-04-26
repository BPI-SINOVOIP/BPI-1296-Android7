#ifndef __C_FILTER_SPU_H__
#define __C_FILTER_SPU_H__

#include <CStream.h>
//#include <Filters/SPUDecoder/spu.h>
//#include <Filters/SPUDecoder/CPinSpu.h>
//#include <Filters/SPUDecoder/SpuDecoderType.h>
#include <Filters/SPUDecoder/SPU_RPC.h>
#include <Util.h>
//#include <Filters/PrivateInfo.h>

//typedef ulCCInPinSpuircularQueue<UOP_Data_T>             DVD_UOP_QUEUE;


class CInPinSpu;


//class CFilterSpu: public CBaseFilter,
//                  public CReferenceClock{
class CFilterSpu: public CBaseFilter,
                  public IFilterSeeking{

friend class                    CInPinSpu;

public:
                                CFilterSpu(void);
                                ~CFilterSpu(void);

    HRESULT				FilterType(FILTER_TYPE* pFilterType); // ??

    HRESULT				Stop(void);
    HRESULT				Pause(FILTER_STATE State);
    HRESULT				Run(void);
    void				init(void);

    HRESULT				CheckCapabilities(FILTER_SEEKING_CAP* pSeekingCap);
	HRESULT				SetSeekingInfo(long Speed, long Skip);
	HRESULT				GetSeekingInfo(long* pSpeed, long* pSkip);
	HRESULT				GetAgentInstanceID(long* ID);                                                        
	HRESULT				CreateAgent(VIDEO_VF_TYPE type, long instanceID);
//	bool				SetReadPointer(BYTE *pNewReadPointer);
//	HRESULT				DoGetRefClock(void);
//	HRESULT				GetLocalPTS(int64_t *PTS);
/*
#ifdef USE_SPU_DECODER_SIMULATOR
	bool				DoFlushPostComplete(void);
#endif
*/
//	HRESULT				GetRingSZ(void);
	HRESULT				GetRPCClnt(CLNT_STRUCT* clnt);
	HRESULT				EnableSubPicture(void);

protected:
    // Ring buffer allocator parts
//    bool				InitRingData(void);

protected:
    CInPinSpu*			m_pInPin;
//#ifndef USE_SPU_DECODER_SIMULATOR    
    CBaseOutputPin*    	m_pOutPin;
//#endif    
//    IMemAllocator*		m_pAllocator;
//    IReadPointerHandle*	m_pRPHandle;
    long				m_agentInstanceID;
//    IReferenceClock*	m_pRefClock;
	CLNT_STRUCT			m_rpcclnt;
    long 				m_Speed;
    long 				m_Skip;	
//	CCritSec			m_mutexf;
};

#endif
