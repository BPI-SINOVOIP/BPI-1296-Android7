#ifndef C_WRITER_INPUT_PARSER_PIN_H
#define C_WRITER_INPUT_PARSER_PIN_H


#include <OSAL.h>
#include <CStream.h>


class CWriterInputParserPin: public CBaseInputPin
{
	friend class 	CIndexGenerator;
	
public:
	CWriterInputParserPin(void);
	virtual ~CWriterInputParserPin();
	    
    	
    //derived from CBasePin 
	HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Flush(void);
		
			

		
protected:

	long					m_BufferSize;
	BYTE*                   m_pBufferLower;
    BYTE*                   m_pBufferUpper;
    
    
    RINGBUFFER_HEADER*          m_pMSGBufferHeader;
};


#endif
