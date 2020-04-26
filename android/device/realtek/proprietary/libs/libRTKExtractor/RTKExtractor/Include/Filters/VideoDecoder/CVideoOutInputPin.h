#ifndef __C_VIDEOOUT_INPUT_PIN_H__
#define __C_VIDEOOUT_INPUT_PIN_H__

#include <OSAL.h>
#include <CStream.h>


class CVideoOutInputPin: public CBaseInputPin
{

	friend class CVideoOutFilter;
public:
    	
    	CVideoOutInputPin(void){};
    	~CVideoOutInputPin(void){};
    	
    	
    	
    	
    	//derived from CBaseInputPin. receive data from demux 
    	//HRESULT	Receive(BYTE* pWritePointer, long WriteBufferSize);
    	HRESULT PrivateInfo(int infoId, BYTE* pInfo, int length);
    	HRESULT Flush(void);
    	
    	//derived from CBasePin 
		HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
		
		
		

};


#endif
