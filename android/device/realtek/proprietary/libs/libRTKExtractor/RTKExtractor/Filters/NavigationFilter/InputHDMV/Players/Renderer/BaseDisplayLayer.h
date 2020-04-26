//-----------------------------------------------------------------------------
// BaseDisplayLayer.h
// Copyright (c)  Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BASE_DISPLAY_LAYER_H_
#define _BASE_DISPLAY_LAYER_H_

//#include "RendererTypes.h"
#include "BaseDisplayFrame.h"
//#include "TimeValue.h"
#include "SonicCriticalSection.h"
//#include <vector>

//class BaseDisplayLayer;
//typedef std::vector<BaseDisplayLayer *> BaseDisplayLayerArray;

const UInt32	kFrameTime_Now						= 0;
const UInt32	kFrameDur_None						= 0;
const UInt32	kFrameDur_Infinite					= 0xFFFFFFFF;


class BaseDisplayLayer
{
    public:

		BaseDisplayLayer(UInt32 zOrder = 0) : mScaleToPrimaryVideo(true), mCanvas(0) { /*zOrder; */}
	virtual ~BaseDisplayLayer() { }

	// HDR must call DisplayStart() before asking for a frame, and call DisplayEnd() after it has finished using the frame
	// This allow the DisplayLayer to synchronize internal cleanup activities (e.g. Uncommit/Remove frame that is being curently used by HDR).
	virtual void DisplayStart(void) = 0;
	virtual void DisplayEnd(void) = 0;
    
    // Find a committed display frame that matches this time stamp    
    virtual BaseDisplayFrame* GetCommittedDisplayFrameAt(Time27M time) = 0;

    virtual UInt32 GetZOrder() = 0;

	bool GetScaleToPrimaryVideo() const { return mScaleToPrimaryVideo; }
	void SetScaleToPrimaryVideo(bool inScale) { mScaleToPrimaryVideo = inScale; }

	UInt32 GetCanvas() const { return mCanvas; }
	void SetCanvas(UInt32 inCanvas) { mCanvas = inCanvas; }

protected:
	bool mScaleToPrimaryVideo;
	UInt32 mCanvas;
};

#endif
