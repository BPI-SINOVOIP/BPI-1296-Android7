#ifndef __C_MPEG2_INPUT_PIN_H__
#define __C_MPEG2_INPUT_PIN_H__

#include <OSAL.h>
#include <CStream.h>

#ifdef TARGET_BOARD
#define MyMemcpy(des, src, size) pli_IPCCopyMemory(src, des, size)
#else
#define MyMemcpy(des, src, size) memcpy(des, src, size)
#endif

//#define _READ_BUFFER_SIZE   8192


class CMPEG2InputPin: public CBaseInputPin
{

	friend class CMPEG2Decoder;
public:
    	CMPEG2InputPin(long streamBufSize, long cmdBufSize);
    	~CMPEG2InputPin(void) ;


    	//derived from CBaseInputPin. receive data from demux 
    	HRESULT	Receive(BYTE* pWritePointer, long WriteBufferSize);
    	HRESULT PrivateInfo(int infoId, BYTE* pInfo, int length);
    	HRESULT Flush(void);
    	
    	//derived from CBasePin 
		HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
		HRESULT NewSegment(void);
		HRESULT UpdateConnectionMediaType(MEDIA_TYPE* pmt);

	
		//derived from IPin
		HRESULT EndOfStream(unsigned int eventID);
		
		ENUM_MEDIA_TYPE GetVideoMajorType()
		{
			return m_mt.GetMajorType();
		}
		
protected:	
                inline HRESULT  DeliverInBandCommand(BYTE* Data, long Size);	
		HRESULT InitRingBuf();

protected:
	
	IReadPointerHandle*   		m_pICQRPHandle;		// inband command buffer read pointer handler
	RINGBUFFER_HEADER*          m_pICQBufferHeader; // inband command buffer header
	IMemRingAllocator* 			m_pICQAllocator;	// inband command buffer allocator
	BYTE*						m_pICQBufferLower;
	BYTE* 						m_pICQBufferUpper;
	
	
	IReadPointerHandle*   		m_pBSRPHandle;		// bit stream buffer read pointer handler
	RINGBUFFER_HEADER*          m_pBSBufferHeader; // bit stream buffer header
    
    bool						m_bAuxPin;
    
       
};


#endif
