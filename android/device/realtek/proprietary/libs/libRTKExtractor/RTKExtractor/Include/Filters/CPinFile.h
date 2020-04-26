
#ifndef __C_PIN_FILE_H__
#define __C_PIN_FILE_H__

#include <StreamClass/CPin.h>

class COutPinFileRead: public CBaseOutputPin{
public:
    virtual                     ~COutPinFileRead(void) {};
    HRESULT                     Connect(
                                    IPin*                   pReceivePin,
                                    const MEDIA_TYPE*       pmt
                                );
	HRESULT                     QueryAccept(
		                    const MEDIA_TYPE*       pmt
				);
	HRESULT                     EndOfStream(unsigned int eventID);
protected:
};

class CInPinFileWrite: public CBaseInputPin{
public:
    virtual                     ~CInPinFileWrite(void) {};

    HRESULT                     Receive(
                                    IMediaSample*           pSample
                                );
    HRESULT                     ReceiveConnection(
                                    IPin*                   pConnector,
                                    const MEDIA_TYPE*       pmt
                                );
	HRESULT                     QueryAccept(
		                    const MEDIA_TYPE*       pmt
				);
};

#endif
