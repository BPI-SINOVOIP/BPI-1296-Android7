
#ifndef __C_USB_AUDIO_OUT_FILTER_H__
#define __C_USB_AUDIO_OUT_FILTER_H__


#include <OSAL.h>
#include <CStream.h>
#include <Filters/UsbAudioOut/CUsbAOutInputPin.h>
#include <Platform_Lib/USBAudioControl/USBAudioControl.h>

class CUsbAOutFilter: public CBaseFilter
{
	friend class CUsbAOutInputPin;
	
public:
	CUsbAOutFilter(void);		
	virtual ~CUsbAOutFilter();
	
	
    HRESULT	Run(void);												
	HRESULT	Stop(void);												
	HRESULT	Pause(FILTER_STATE State);		
	
	HRESULT Flush();
	
	/**
	* \fn  HRESULT ReopenDevice()
	*
	* \brief call ReopenDevice() if usb headset is plugged.
	*	It has no effect if usb device is already opened.
	*
	* \return HRESULT:
	*    S_OK if usb device is opened.
 	*
	* \param  none
	*/
	HRESULT ReopenDevice();
	
	/**
	* \fn  HRESULT RemoveDevice()
	*
	* \brief call RemoveDevice() if usb headset is unplugged.
	*	It has no effect if usb device is not opened.
	*
	* \return HRESULT:
	*    S_OK if device handler is closed.
 	*
	* \param  none
	*/
	HRESULT RemoveDevice();
	
private:
	void DeliverAudioData(CMemRingAllocator* pAllocator, int canReadSize, BYTE* pBufLower, long bufSize);
	HRESULT GetBufferHeader();
	
	
	/////////////////////////////////////////////////////////////////////////
	// the following APIs are used to control usb audio out device
	/////////////////////////////////////////////////////////////////////////
	HRESULT ConfigUsbStatus_SampleRate();
	HRESULT OpenDevice();
	HRESULT CloseDevice();
	HRESULT OutputToDevice(unsigned char* pData, int len);
	HRESULT SetRateToDevice(int rate);
	HRESULT SetChNumToDevice(int num);
	HRESULT SetFMT(int fmt);
		
    static void ThreadEntry(void* pInstance);
    
    
private:
	
	CThread* 	         	m_pThread;
	CUsbAOutInputPin*		m_pInputPinContainer;
    CBaseInputPin*			m_pInputPin;
    USBAudioControl* 		m_pUsbAudioControl;
    
    //CMultiRingAllocator* 	m_pAllocatorContainer;
    //CMemRingAllocator*		m_pAllocator;
    int					m_chIndex;
    int 				m_pChannelCount;
	RINGBUFFER_HEADER*	m_pBufferHeader_Channel[USB_AOUT_CHANNEL_NUM];		
	CMemRingAllocator*  m_pAllocator_Channel[USB_AOUT_CHANNEL_NUM];		
	
	long 				m_agentInstanceID;
	bool 				m_bDeviceOpened;    
	CCritSec            m_Mutex;
	
	
	//used to buffer a pair of sample of left and right channel	
	//sampling rate = 44100=> 4096 / 4 = 1024 samples => 1024/44100 = 0.02 sec
	//if size of devBuf = 4bytes, audio will be jitter in DTV live.
	//So, we allocate 4k for devBuf and user should not be aware of delay of 0.02sec
	int m_devBufCount;
	unsigned char m_devBuf[4096];

protected:
	
	bool m_bEOS;	
};

#endif

