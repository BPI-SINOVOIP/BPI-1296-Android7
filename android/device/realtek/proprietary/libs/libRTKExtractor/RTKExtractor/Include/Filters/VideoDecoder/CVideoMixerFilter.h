

#ifndef __C_VIDEO_MIXER_FILTER_H__
#define __C_VIDEO_MIXER_FILTER_H__

#include "CStream.h"
//#include <Filters/VideoDecoder/VideoType.h>
#include <Filters/VideoDecoder/CVideoFilter.h>
#include <Filters/VideoDecoder/CMixerInputPin.h>


/*typedef enum {
	VO_LAYER_1 = 0,
	VO_LAYER_2 = 1
} VIDEO_OUT_LAYER;*/

class CVideoMixerFilter : /*public IFilterSeeking, */public CVideoFilter
{
	friend class CMixerInputPin;

public:
	CVideoMixerFilter(bool bUsedForSeamlessPlayback = true);
	virtual ~CVideoMixerFilter();
	
	/*HRESULT    SetSeekingInfo(long Speed, long Skip);
	HRESULT    GetSeekingInfo(long* pSpeed, long* pSkip);
	HRESULT    CheckCapabilities(FILTER_SEEKING_CAP* pSeekingCap);*/
	
	//derived from CBaseFilter
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);
	
    
    //====================================
	//	used for video conference
	//====================================
#if (IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)) && defined(ENABLE_CONFERENCE)	
    HRESULT Configure(VIDEO_RPC_MIXER_FILTER_CONFIGURE config);
    HRESULT ConfigWindow(MIXER_WIN* pWindow, long count);
    HRESULT SetMasterWindow(long windowID);
#endif	
	
protected:
	CBaseOutputPin*	  	   	m_pOutputPin;
	CMixerInputPin*     	m_pInputPin[2];
	CMemRingAllocator* 	   	m_pICQAllocator;	// inband command buffer allocator

#if defined(ENABLE_CONFERENCE) && defined(SINGLE_MIXER)	
	static long m_mixer;
	static int m_mixerStartPin;
	
public:
	static long GetMixerID() {return m_mixer;}
#endif
	
};






#endif

