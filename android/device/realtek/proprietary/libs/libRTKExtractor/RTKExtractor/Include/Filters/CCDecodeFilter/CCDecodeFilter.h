
#ifndef __CC_DECODE_FILTER_H__
#define __CC_DECODE_FILTER_H__


#include <OSAL.h>
#include <CStream.h>

#include <Filters/CCDecodeFilter/CCDecoderAPI.h>
#include <Filters/CCDecodeFilter/CCInputPin.h>
#include <rpc_common.h>
#include <RPCBaseDS_data.h>
#include <VideoRPCBaseDS.h>
#include <VideoRPC_System.h>
#include <Filters/VideoDecoder/VP_RPC.h>

class CCDecodeFilter: public CBaseFilter
{
	friend class CCInputPin;
public:
	CCInputPin* 	m_pInputPin;


	CCDecodeFilter(void);
	~CCDecodeFilter();

    HRESULT		Run(void);
	HRESULT		Stop(void);
	HRESULT		Pause(FILTER_STATE State);
	HRESULT		GetCaptionNo(CC_ACTIVE_NO *servNo);

	HRESULT		CCShow(char bEnable);
	HRESULT		CCSetVideoRect(CC_RECT* pOriRect, CC_RECT* pOutputRect);
	HRESULT		CCSetSource();
	HRESULT		CCEnable(char bEnable);
    HRESULT     CCSetHandlerID(void);
	HRESULT		CCChangeCaptionType(char capType);
	HRESULT		CCChangeServiceLayer(char servLayer);
	HRESULT 	CCSetDecodeMode(VIDEO_DECODER_CC_BYPASS_MODE mode);
	HRESULT 	CCSetUIHandle(SURFACEHANDLE hSurface1, SURFACEHANDLE hSurface2);
	HRESULT		CCSetInterHandle(SURFACEHANDLE hSmallSurf1, SURFACEHANDLE hSmallSurf2,
	                             SURFACEHANDLE hSmallSurf3, SURFACEHANDLE hSmallSurf4);
	HRESULT		CCSetCharSize(CC_FONT_SIZE size);
	HRESULT		CCSetCharColor(CC_FONT_COLOR color);
	HRESULT		CCSetBgAlpha(CC_BG_ALPHA alpha);
	HRESULT		CCGetCharAttr(CC_FONT_SIZE *size, CC_FONT_COLOR *color);

private:
	CC_RECT 			m_oriRect;
	CC_RECT				m_outputRect;
	char				m_bEnableCC;
	char				m_bShowCC;

	static void ThreadEntry(void* pData);

protected:
	long				m_id;
	long				m_VideoDecoderID;

};

#endif
