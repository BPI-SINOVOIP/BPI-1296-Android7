#ifndef __C_MIXER_INPUT_PIN_H__
#define __C_MIXER_INPUT_PIN_H__

#include <OSAL.h>
#include <CStream.h>

#ifdef TARGET_BOARD
#define MyMemcpy(des, src, size) pli_IPCCopyMemory(src, des, size)
#else
#define MyMemcpy(des, src, size) memcpy(des, src, size)
#endif

class CMixerInputPin: public CBaseInputPin
{

	friend class CVideoMixerFilter;
public:
    	
    	CMixerInputPin(int pinID){m_PinID = pinID;};
    	~CMixerInputPin(void){};
    	
    	int				m_PinID;
    	BYTE*			m_pICQBufferLower;
		BYTE* 			m_pICQBufferUpper;
		
    	
    	//derived from CBaseInputPin. receive data from demux 
    	HRESULT	Receive(BYTE* pWritePointer, long WriteBufferSize);
    	HRESULT PrivateInfo(int infoId, BYTE* pInfo, int length);
    	HRESULT Flush(void);
    	
    	//derived from CBasePin 
		HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
		
protected:
	

    //inline HRESULT 	ByteSwap(BYTE* Data, long Size);
    inline HRESULT  DeliverInBandCommand(BYTE* Data, long Size);
};


#endif
