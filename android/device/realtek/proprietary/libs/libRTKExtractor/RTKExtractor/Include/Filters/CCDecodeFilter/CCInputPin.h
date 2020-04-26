#ifndef CC_INPUT_PIN_H
#define CC_INPUT_PIN_H


#include <OSAL.h>
#include <CStream.h>


class CCInputPin: public CBaseInputPin
{
	friend class 	CCDecodeFilter;
	
public:
	CCInputPin(void);
	virtual ~CCInputPin();
	    
    	
    //derived from CBasePin 
	HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Flush(void);
		
		
protected:

	
	RINGBUFFER_HEADER*      m_pCCBufferHeader;
};


#endif
