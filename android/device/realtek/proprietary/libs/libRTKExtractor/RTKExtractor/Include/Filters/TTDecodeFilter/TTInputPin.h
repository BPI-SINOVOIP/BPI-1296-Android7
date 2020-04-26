#ifndef TT_INPUT_PIN_H
#define TT_INPUT_PIN_H


#include <OSAL.h>
#include <CStream.h>


class TTInputPin: public CBaseInputPin
{
	friend class 	TTDecodeFilter;
	
public:
	TTInputPin(void);
	virtual ~TTInputPin();
	    
    	
    //derived from CBasePin 
	HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Flush(void);
		
			

		
protected:

};


#endif
