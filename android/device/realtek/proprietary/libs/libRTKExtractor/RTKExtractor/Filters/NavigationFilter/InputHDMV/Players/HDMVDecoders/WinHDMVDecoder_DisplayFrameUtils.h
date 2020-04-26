//-----------------------------------------------------------------------------
// WinHDMVDecoder_DisplayFrameUtils.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINHDMVDECODER_DISPLAYFRAMEUTILS_H__
#define __WINHDMVDECODER_DISPLAYFRAMEUTILS_H__

#include "HDMVDecoder_Types_Internal.h"
#include "DisplayLayer.h"
#include "WinDisplayFrame.h"

namespace BDROM_HDMVDecoder
{

// GCFrameType
// type of display frame (or rather type of usage display frame is being put to)
//
typedef enum tagGCFrameType
{
	kGCFrameType_Normal,			// Normal display frame, i.e., no special usage attributes attached to this
	kGCFrameType_PageEffect			// Display frame contains an IG Page effect
} GCFrameType;

// GCFrameInfo
// Info about IG Frames
//
struct GCFrameInfo
{
public:
	GCFrameInfo()	{ Reset(); }
	void Reset(void)		
		{
			m_pFrame = 0;
			m_FrameStartTime = 0;
			m_FrameDuration = 0;
			m_FrameType = kGCFrameType_Normal;
		}

	void Set(WinDisplayFrame * pFrame, TimeValue startTime, TimeValue duration, GCFrameType frameType = kGCFrameType_Normal)
		{
			m_pFrame = pFrame;
			m_FrameStartTime = startTime;
			m_FrameDuration = duration;
			m_FrameType = frameType;
		}

	// returns true if frame display is based on PTS
	bool HasStartTime(void)			{ return ((m_pFrame) && (m_FrameStartTime != kFrameTime_Now)); }

	// returns true if frame duration is not INFINITE
	bool HasDuration(void)			{ return ((m_pFrame) && (m_FrameStartTime != kFrameDur_Infinite)); }

	bool IsNormalType(void)			{ return m_FrameType == kGCFrameType_Normal; }
	bool IsPageEffectType(void)		{ return m_FrameType == kGCFrameType_PageEffect; }

public:
	WinDisplayFrame	*	m_pFrame;			// last frame commited for display
	TimeValue			m_FrameStartTime;
	TimeValue			m_FrameDuration;
	GCFrameType			m_FrameType;
};


void ClearDisplayFrame(DisplayLayer * pDisplayLayer, GCFrameInfo * pFrameInfo);					// removes frame from display. Active frame is undefined after this.
void ClearDisplayLayer(DisplayLayer * pDisplayLayer);				// clears display layer. Will remove all frames from display
};

#endif // __WINHDMVDECODER_DISPLAYFRAMEUTILS_H__
