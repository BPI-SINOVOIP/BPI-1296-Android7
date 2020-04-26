#ifndef __C_PIN_SPU_H__
#define __C_PIN_SPU_H__

#include <CStream.h>
//#include <Filters/PrivateInfo.h>
//#include <Filters/SPUDecoder/SPU_RPC.h>
//#include <Filters/SPUDecoder/CSpuAgent.h>

typedef struct _tagSPUPacketInfo{
    BYTE* pBuffer;
    int len;
    int64_t pts;
} SPUPacketInfo;

class CInPinSpu: public CBaseInputPin{
public:
    							CInPinSpu(void);
    							~CInPinSpu(void) ;
    HRESULT                     Receive(BYTE* pWritePointer, long WriteBufferSize);

    HRESULT                     ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
    HRESULT                     UpdateConnectionMediaType(MEDIA_TYPE* pmt);
    HRESULT                     Disconnect(void);
    HRESULT                     PrivateInfo(int infoId, BYTE* pInfo, int length);                                
	HRESULT                     Flush(void);
	void						init(void);
	HRESULT						ReceiveData(BYTE* pData, long DataSize);

	
protected:

	IMemRingAllocator*			m_pAllocator;	// BS buffer allocator
	IReadPointerHandle*   		m_pBSRPHandle;		// bit stream buffer read pointer handler
	RINGBUFFER_HEADER*          m_pBSBufferHeader; // bit stream buffer header
	BYTE*						m_pBSBufferLower;
	BYTE* 						m_pBSBufferUpper;		
	
	IMemRingAllocator* 			m_pICQAllocator;	// inband command buffer allocator	
//	IMemRingAllocator* 			m_pBSAllocator;		// BS buffer allocator	
	
	IReadPointerHandle*   		m_pICQRPHandle;		// inband command buffer read pointer handler
	RINGBUFFER_HEADER*          m_pICQBufferHeader; // inband command buffer header
	BYTE*						m_pICQBufferLower;
	BYTE* 						m_pICQBufferUpper;		
//    osal_mutex_t				m_mutexpin;    
    HRESULT MyMemcpy(BYTE* pCurr, BYTE* Data, long Size);
    HRESULT  DeliverInBandCmd(BYTE* Data, long Size);
	CLNT_STRUCT					m_rpcclnt;
    //HRESULT 	ByteSwap(BYTE* Data, long Size);
	
};
#endif
