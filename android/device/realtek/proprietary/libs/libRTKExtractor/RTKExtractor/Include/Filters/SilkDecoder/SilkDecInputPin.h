#ifndef SILK_DEC_INPUT_PIN_H
#define SILK_DEC_INPUT_PIN_H

#include <OSAL.h>
#include <CStream.h>

class SilkDecInputPin: public CBaseInputPin
{
	friend class 	SilkDecoder;
	
public:
	SilkDecInputPin(void);
	virtual ~SilkDecInputPin();
	    
    //derived from CBasePin 
	HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Receive(BYTE *pWritePointer, long WriteBufferSize);

};

#endif	// SILK_DEC_INPUT_PIN_H
