//-----------------------------------------------------------------------------
// DisplayLayerClientImpl.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------


#ifndef __DISPLAY_LAYER_CLIENT_IMPL_H__
#define __DISPLAY_LAYER_CLIENT_IMPL_H__


#include "SysSurfaceCommon.h"


class DisplayLayer;
class BaseRenderer;


class DisplayLayerClientImpl
{
public:
	DisplayLayerClientImpl(void)
		: _pRenderer(0)
		, m_pDisplayLayer(NULL)
		, _nFrameWidth(1920)
		, _nFrameHeight(1080)
		, _eFrameFormat(SonicSurface::eFormat_32BGRAPixelFormat)
		, _nFramesCount(2)
	{}

	virtual ~DisplayLayerClientImpl(void)
	{ ResetDisplayLayer(); }

protected:
	void InitDisplayLayer(ref_ptr<BaseRenderer> pRender,
							DisplayLayer* pDisplayLayer,
							UInt32 nWidth = 1920,
							UInt32 nHeight = 1080,
							SonicSurface::SurfaceFormat eFormat = SonicSurface::eFormat_32BGRAPixelFormat,
							UInt32 nFramesCount = 2);
	void ResetDisplayLayer(void);

	void CreateFrames(void);
	void DestroyFrames(void);

	inline DisplayLayer* GetDisplayLayer(void)
	{ assert(m_pDisplayLayer); return m_pDisplayLayer; }

protected:
	ref_ptr<BaseRenderer>			_pRenderer;
	DisplayLayer*					m_pDisplayLayer;
	UInt32							_nFrameWidth;
	UInt32							_nFrameHeight;
	SonicSurface::SurfaceFormat		_eFrameFormat;
	UInt32							_nFramesCount;
};


#endif	// __DISPLAY_LAYER_CLIENT_IMPL_H__
