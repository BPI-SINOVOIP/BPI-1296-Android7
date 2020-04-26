#ifndef __USB_AUDIO_IN_H_
#define __USB_AUDIO_IN_H_

#include <OSAL.h>
#include <CStream.h>
#include <Platform_Lib/USBAudioControl/USBAudioControl.h>
#include <Filters/AudioFilter_c/CAudioFilter.h>
#include <Filters/AudioFilter_c/CMultiAllocator.h>

#define USB_AUDIO_IN_CH_NUM		AUDIO_IN_OUTPIN
//#define USB_AUDIO_IN_CH_NUM		AUDIO_DEC_OUTPIN

class USBAudioInOutputPin: public CAudioMultiOutputPin
{
public :
	USBAudioInOutputPin(long pinID)
		: CAudioMultiOutputPin() { m_pinID = pinID; };
			
	~USBAudioInOutputPin(){};
	
	HRESULT Connect(IPin* pReceivePin, const MEDIA_TYPE* pmt);
};


class USBAudioInFilter: public CBaseFilter
{
	friend class USBAudioInOutputPin;
public:
	USBAudioInFilter();
	~USBAudioInFilter();
	
	HRESULT Run(void);
	HRESULT Stop(void);
	HRESULT Pause(FILTER_STATE State);
	
	HRESULT StartEncode();
	HRESULT StopEncode();
	
protected:
	HRESULT SetupAgentConnection(IPin* pConnectedPin);
	
private:
	HRESULT CreateAudioAgent();
	HRESULT DestroyAudioAgent();
		
private:
	USBAudioInOutputPin		*m_pOutPin;
	CBaseOutputPin			*m_pCHPinArr[USB_AUDIO_IN_CH_NUM];
	CMemRingAllocator		*m_pInAllocatorArr[USB_AUDIO_IN_CH_NUM];
    USBAudioControl			*m_pUsbControl;
	
protected:	
	long					m_AudioID;
	bool					m_Init;
	
};

#endif	// __USB_AUDIO_IN_H_
