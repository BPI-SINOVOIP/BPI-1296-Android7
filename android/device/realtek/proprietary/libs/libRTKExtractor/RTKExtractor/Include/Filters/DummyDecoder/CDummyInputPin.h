#ifndef __C_Dummy_INPUT_PIN_H__
#define __C_Dummy_INPUT_PIN_H__

#include <OSAL.h>
#include <CStream.h>

#include <AudioInbandAPI.h>

#ifdef TARGET_BOARD
#define MyMemcpy(des, src, size) pli_IPCCopyMemory(src, des, size)
#else
#define MyMemcpy(des, src, size) memcpy(des, src, size)
#endif

//#define _READ_BUFFER_SIZE   8192

enum { VIDEO_PIN = 0,
       AUDIO_PIN,
       SPIC_PIN,
       TELETEXT_PIN,
       NUMBER_OF_PINS };

class CDummyInputPin: public CBaseInputPin
{

	friend class CDummyDecoder;
public:
    	CDummyInputPin(long streamBufSize, long cmdBufSize, int setPinID);
    	~CDummyInputPin(void) ;


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

            HRESULT GetPkt(void *ptr);
            HRESULT GetPinPktCnt(void *ptr);
            HRESULT GetAudioFormatPkt(void *ptr);
            		
protected:	
                inline HRESULT  DeliverInBandCommand(BYTE* Data, long Size);	
		HRESULT InitRingBuf();

protected:
	
	IReadPointerHandle*   		m_pICQRPHandle;		// inband command buffer read pointer handler
	RINGBUFFER_HEADER*          m_pICQBufferHeader; // inband command buffer header
	IMemRingAllocator* 			m_pICQAllocator;	// inband command buffer allocator
	BYTE*						m_pICQBufferLower;
	BYTE* 						m_pICQBufferUpper;
	BYTE*						m_pICQBufferPrev;
	unsigned int					ICQPTSPktCnt;
	BYTE*						m_pBSBufferLower;
	BYTE* 						m_pBSBufferUpper;
	
	
	IReadPointerHandle*   		m_pBSRPHandle;		// bit stream buffer read pointer handler
	RINGBUFFER_HEADER*          m_pBSBufferHeader; // bit stream buffer header
    
	bool						m_bAuxPin;
    CCritSec		m_Lock;
    bool isEOS;
    bool EOSDone;
    int PinID;	//VIDEO_PIN or AUDIO_PIN
    AUDIO_DEC_NEW_FORMAT	mAudioFormatICQ;
    int64_t lastPTS;
    int mOffset;    /* For remove inband command used */
};


#endif
