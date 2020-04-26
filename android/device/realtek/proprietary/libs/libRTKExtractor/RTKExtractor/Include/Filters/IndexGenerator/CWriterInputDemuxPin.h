#ifndef C_WRITER_INPUT_DEMUX_PIN_H
#define C_WRITER_INPUT_DEMUX_PIN_H


#include <OSAL.h>
#include <CStream.h>


class CWriterInputDemuxPin: public CBaseInputPin
{
	friend class 	CIndexGenerator;
	
public:

	HRESULT	PrivateInfo(int infoId, BYTE* pInfo, int length) ;   	
	HRESULT Flush(void);  
    
};


#endif
