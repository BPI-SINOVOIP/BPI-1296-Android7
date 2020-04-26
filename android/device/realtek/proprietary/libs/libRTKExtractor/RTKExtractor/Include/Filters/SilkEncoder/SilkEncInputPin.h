#ifndef SILK_ENC_INPUT_PIN_H
#define SILK_ENC_INPUT_PIN_H

#include <OSAL.h>
#include <CStream.h>

class SilkEncInputPin: public CBaseInputPin
{
	friend class 	SilkEncoder;
	
public:
	SilkEncInputPin(void);
	virtual ~SilkEncInputPin();
	    
    //derived from CBasePin 
	HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Receive(BYTE *pWritePointer, long WriteBufferSize);

protected:
	IReadPointerHandle* m_pRPHandle;

};


#endif	// SILK_ENC_INPUT_PIN_H

