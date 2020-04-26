

#ifndef __C_VIDEO_OUT_FILTER_H__
#define __C_VIDEO_OUT_FILTER_H__

#include "CStream.h"
#include <Filters/NavigationFilter/NavDef.h>

#include <Filters/VideoDecoder/CVideoFilter.h>
#include <Filters/VideoDecoder/CVideoOutInputPin.h>

typedef struct {
	unsigned char*	lumaAddr ;
	long 			lumaPitch;
	unsigned char*	chromaAddr;
	long			chromaPitch;
	long 			width;
	long 			height;
	
} STILL_PIC;
	
typedef enum{
	FILTER_TYPE_VO,
	FILTER_TYPE_THUMBNAIL,
#ifdef ENABLE_FP10
	FILTER_TYPE_FLASH_FRAME,
#endif
}VO_FILTER_TYPE;
	
class CVideoOutFilter : public IFilterSeeking, public CVideoFilter
{
	friend class CVideoOutInputPin;

public:
	CVideoOutFilter(VO_FILTER_TYPE type = FILTER_TYPE_VO);
	virtual	~CVideoOutFilter();
	HRESULT                     FilterType(FILTER_TYPE*            pFilterType){
	  *pFilterType = FILTER_RENDER_VIDEO;
	  return S_OK;
	}	
	
	HRESULT    SetSeekingInfo(long Speed, long Skip);
	HRESULT    GetSeekingInfo(long* pSpeed, long* pSkip);
	HRESULT    CheckCapabilities(FILTER_SEEKING_CAP* pSeekingCap);
	
	//derived from CBaseFilter
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);
	HRESULT	   Step(void);

    HRESULT 	SetVideoOutLayer(VO_VIDEO_PLANE layer, unsigned char realTimeSrc = 0, unsigned char zeroBuffer = 1);
    HRESULT 	SetRescaleMode(VO_RESCALE_MODE RESMode);

    //HRESULT		Hide();
    //HRESULT 	DisplayBorderColor();
	HRESULT 	ShowStillPicture(STILL_PIC* pPic);
	HRESULT 	FillVideoBorder(long border, long numPixels, VO_COLOR*	 pColor);
	HRESULT		FastDisplay(unsigned char enable, unsigned char count);
		
	// the followings are only for thumbnail decoding
	HRESULT 	SetOutFormat(NAVTHUMBNAILPARA* pFormat, unsigned long thumbAckAddr);
	HRESULT 	SetThumbnailThreshold(long threshold);
	HRESULT 	SetThumbnailStartPic(long startPicNum, long endPicNum);
	
	static HRESULT SetSystemRescaleMode(VO_RESCALE_MODE RESMode);	
	
#ifdef ENABLE_FP10
	HRESULT		GetFlashInfo(char** pHeaderAddr, char** pBaseAddr, unsigned long *pOffset);

	RINGBUFFER_HEADER  *m_pOutBufferHeader;
	CMemRingAllocator  *m_OutRing;
#endif
	
		
protected:
	CVideoOutInputPin*  m_pInputPin[2];
	CBaseInputPin*		m_pTTOutputPin;
	
	VO_FILTER_TYPE		m_type;
	VO_VIDEO_PLANE	 	m_VOLayer;
	VO_RESCALE_MODE	 	m_RESModeFromPrivateInfo;
	bool 				m_bUseRESModeFromPrivateInfo;

	long 				m_Speed;
    long 				m_Skip;
    
    static VO_RESCALE_MODE	 	m_gRESMode;

};






#endif

