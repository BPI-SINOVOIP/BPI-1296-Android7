#ifndef CAMERA_AUDIO_IN_H
#define CAMERA_AUDIO_IN_H

#include <OSAL.h>
#include <CStream.h>
#include <Platform_Lib/USBAudioControl/USBAudioControl.h>

class CameraAudioInOutputPin: public CBaseOutputPin
{
public :
	CameraAudioInOutputPin(){};
	~CameraAudioInOutputPin(){};
	
	HRESULT Connect(IPin* pReceivePin, const MEDIA_TYPE* pmt);
};

class CameraAudioInFilter: public CBaseFilter
{
	friend class CameraAudioInOutputPin;
public:
	CameraAudioInFilter();
	~CameraAudioInFilter();
	
	HRESULT Run(void);
	HRESULT Stop(void);
	HRESULT Pause(FILTER_STATE State);
	
	HRESULT StartEncode();
	HRESULT StopEncode();
	
	void	SetMicMute(bool isMute) { if (m_pUsbControl) m_pUsbControl->SetMuteState(READ_FROM_DEVICE, isMute); }
	
private:
	HRESULT CreateAudioAgent();
	HRESULT DestroyAudioAgent();
		
private:
	CameraAudioInOutputPin* m_pOutAEPin;
	USBAudioControl* m_pUsbControl;
	
protected:	
	long			m_AudioID;
};

#endif
