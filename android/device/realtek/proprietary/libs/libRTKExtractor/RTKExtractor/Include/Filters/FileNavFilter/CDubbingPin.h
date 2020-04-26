#ifndef __C_PIN_PRINT_OUT_H__
#define __C_PIN_PRINT_OUT_H__

#include <CStream.h>
#include <Filters/FileNavFilter/CFileNavFilter.h>
//#include <Filters/PrivateInfo.h>
/*
#include "CObject.h"
#include "CPin.h"
*/
typedef enum{
    QUEUE_TYPE_VRO_PKT =0,   
    QUEUE_TYPE_END_OF_STREAM = 1
}PRINTOUT_IQUEUE_TYPE;

typedef struct{
    PRINTOUT_IQUEUE_TYPE  Qtype;
    void*       addr;
}PRINTOUT_IQUEUE_ELEMENT;

typedef struct _tagVROPacketInfo{
    BYTE* pBuffer;
    int len;
} VROPacketInfo;

class CDubbingInPin: public CBaseInputPin{
public:

#ifdef RING_BUF 
    HRESULT		Receive(BYTE* pWritePointer, long WriteBufferSize);
#else    
	HRESULT		Receive(IMediaSample* pSample);
#endif    
    HRESULT		ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
//	HRESULT		QueryAccept(const MEDIA_TYPE* pmt);
	HRESULT     Disconnect(void);
    //HRESULT     PrivateInfo(int infoId, BYTE* pInfo, int length);     
    HRESULT     EndOfStream(unsigned int eventID);
/*  
private:
	uint16_t	i16MVOBUNo;
*/	
};

class CDubbingInfoInputPin: public CBaseInputPin{
    HRESULT     PrivateInfo(BYTE *pInfo, int nLength);     
};
#endif
