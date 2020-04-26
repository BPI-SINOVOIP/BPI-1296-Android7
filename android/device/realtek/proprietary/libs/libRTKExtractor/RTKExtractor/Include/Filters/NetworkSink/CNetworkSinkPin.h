#ifndef __C_PIN_NW_SINK_H__
#define __C_PIN_NW_SINK_H__

#include <CStream.h>
//#include <Filters/PrivateInfo.h>
//#include <Filters/SPUDecoder/SPU_RPC.h>
//#include <Filters/SPUDecoder/CSpuAgent.h>

//#define USE_RING_BUF

typedef struct{
    BYTE* pBuffer;
    int len;
}SINK_IQUEUE_ELEMENT;

typedef ulCircularQueue<SINK_IQUEUE_ELEMENT>		SINK_DATA_INFO_QUEUE;
//typedef ulDArray<SINK_IQUEUE_ELEMENT>		SINK_DATA_INFO_QUEUE;

class CInPinNetworkSink: public CBaseInputPin{
public:
    							CInPinNetworkSink(void);
    							~CInPinNetworkSink(void) ;
    HRESULT                     Receive(BYTE* pWritePointer, long WriteBufferSize);

    HRESULT                     ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
//    HRESULT                     UpdateConnectionMediaType(MEDIA_TYPE* pmt);
    HRESULT                     Disconnect(void);
//    HRESULT                     PrivateInfo(int infoId, BYTE* pInfo, int length);                                
//	HRESULT                     Flush(void);
#ifndef USE_RING_BUF
	HRESULT						Flush(void);
#endif
private:	
	
	void						xInit(void);
	
#ifdef USE_RING_BUF	
public:

	IMemRingAllocator*			m_pAllocator;	// ring buffer allocator
	
		
protected:
	//IMemRingAllocator*			m_pAllocator;	// ring buffer allocator
	IReadPointerHandle*   		m_pRBHandle;	// ring buffer handle	
	RINGBUFFER_HEADER*          m_pRBHeader; // ring buffer header
	BYTE*						m_pRBLower;
	BYTE* 						m_pRBUpper;	
#else

protected:
	bool						m_bIsRestData;
	int							m_nOffset;
#endif	
	//long 						m_pRBSize;	

	//IMemRingAllocator* 			m_pICQAllocator;	// inband command buffer allocator	
//	IMemRingAllocator* 			m_pBSAllocator;		// BS buffer allocator	
	
	//IReadPointerHandle*   		m_pICQRPHandle;		// inband command buffer read pointer handler
	//RINGBUFFER_HEADER*          m_pICQBufferHeader; // inband command buffer header
	//BYTE*						m_pICQBufferLower;
	//BYTE* 						m_pICQBufferUpper;		
//    osal_mutex_t				m_mutexpin;    
//    HRESULT MyMemcpy(BYTE* pCurr, BYTE* Data, long Size);
//    HRESULT  DeliverInBandCmd(BYTE* Data, long Size);
//	CLNT_STRUCT					m_rpcclnt;
    //HRESULT 	ByteSwap(BYTE* Data, long Size);
	
};
#endif
