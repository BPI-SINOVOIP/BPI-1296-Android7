#ifndef TT_OUTPUT_PIN_H
#define TT_OUTPUT_PIN_H


#include <OSAL.h>
#include <CStream.h>


class TTOutputPin: public CBaseOutputPin
{
	
	friend class 	TTDecodeFilter;
public:
	TTOutputPin(void);
	virtual ~TTOutputPin();
	    
    	
    //derived from CBasePin 
	HRESULT	Connect(IPin* pReceivePin, const MEDIA_TYPE* pmt);
	
	CMemRingAllocator* m_pAllocator ;	
protected:

	
};


#endif

