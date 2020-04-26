#ifndef C_USB_AUDIO_OUT_INPUT_PIN_H
#define C_USB_AUDIO_OUT_INPUT_PIN_H


#include <OSAL.h>
#include <CStream.h>
#include <Filters/AudioFilter_c/CAudioPin.h>

#define USB_AOUT_CHANNEL_NUM	2

class CUsbAOutInputPin: public CAudioMultiInputPin
{
		
public:
	CUsbAOutInputPin(void);
	virtual ~CUsbAOutInputPin();
	    
    	
   	HRESULT	PrivateInfo(int infoId, BYTE* pInfo, int length);
	HRESULT	ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Flush(void);
	HRESULT EndOfStream(unsigned int eventID);
	
			
};


#endif
