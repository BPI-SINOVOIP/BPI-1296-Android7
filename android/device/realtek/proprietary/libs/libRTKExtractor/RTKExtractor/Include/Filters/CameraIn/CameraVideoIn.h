#ifndef CAMERA_VIDEO_IN_H
#define CAMERA_VIDEO_IN_H

#include <OSAL.h>
#include <CStream.h>
#include "camera_uvc.h"


#define CAMERA_VI_VIDEO_WIDTH	320
#define CAMERA_VI_VIDEO_HEIGHT	240
#define CAMERA_VI_VIDEO_FPS	15

class CameraVideoInOutputPin: public CBaseOutputPin
{
public:
	CameraVideoInOutputPin(long streamBufSize, long cmdBufSize);
	~CameraVideoInOutputPin();
	
	HRESULT Connect(IPin* pReceivePin, const MEDIA_TYPE* pmt);
	CMemRingAllocator*  InitBuffer(IPin* pReceivePin, long instanceID, RINGBUFFER_TYPE bufferType, int bufferSize, unsigned char** ppBufLower, unsigned char** ppBufUpper, bool bPassToVideo);
		
public:
	unsigned char* m_pBSBufferLower;
	unsigned char* m_pBSBufferUpper;
	CMemRingAllocator* m_pVEBSAllocator;

	unsigned char* m_pCMDBufferLower;
	unsigned char* m_pCMDBufferUpper;
	CMemRingAllocator* m_pCMDAllocator;	
	
private:
	long m_streamBufSize;
	long m_cmdBufSize;
};

/////////////////////////////////////////////////////////////////////
class CameraVideoInFilter: public CBaseFilter
{
	friend class CameraVideoInOutputPin;
public:
	CameraVideoInFilter();
	~CameraVideoInFilter();
	
	HRESULT Run(void);
	HRESULT Stop(void);
	HRESULT Pause(FILTER_STATE State);
	
	HRESULT UpdateCameraVideoFormat(CAMERA_VFMT fmt, long width = CAMERA_VI_VIDEO_WIDTH , long height = CAMERA_VI_VIDEO_HEIGHT);
	CAMERA_VIDEO_FORMAT GetCurrVideoFormat();
	
	HRESULT StartEncode(bool bNeedThread = false);
	HRESULT StopEncode();
	
	HRESULT Read();
		
private:
	static void 	ThreadEntry(void* pInstance);
	HRESULT 		WriteDataToRingBuf(CMemRingAllocator* pAllocator, BYTE* pData, unsigned long phyAddr, long dataSize, bool bIsCMD);
		
private:
	Camera*  				m_pCamera;
	CThread*				m_pThread;	
	CameraVideoInOutputPin* m_pOutPin;
	
	bool					m_bEnableEcho;
	
	//address of ring buffer which used to deliver jpeg frame
	unsigned long			m_pFrameBufferPhyAddr;
	BYTE* 					m_pFrameBufferNonCachedVirAddr;
	BYTE* 					m_pFrameBufferCachedVirAddr;
	
	uint64_t 				m_latestDDRCopyHandle;
	
protected:
	CAMERA_VIDEO_FORMAT		m_cameraVideoFormat;
	bool					m_bConnectToVE;
};



#endif
