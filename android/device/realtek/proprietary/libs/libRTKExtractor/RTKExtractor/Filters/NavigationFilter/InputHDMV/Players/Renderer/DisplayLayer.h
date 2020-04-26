//-----------------------------------------------------------------------------
// DisplayLayer.h
// Copyright (c)  Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _DISPLAY_LAYER_H_
#define _DISPLAY_LAYER_H_

//#include "RendererTypes.h"
#include "BaseDisplayFrame.h"
#include "BaseDisplayLayer.h"
//#include "TimeValue.h"
#include "SonicCriticalSection.h"
//#include <vector>

class DisplayLayer : public BaseDisplayLayer
{
    public:

    DisplayLayer(UInt32 zOrder = 0);
    virtual ~DisplayLayer();

    /////////////////////////////////////////////////////////////////////
    // The following are most likely used by something like The Navigator
    /////////////////////////////////////////////////////////////////////
        
    // Get a display frame from the available list
    BaseDisplayFrame* GetAvailableDisplayFrame();
    
    // Commit a display layer to the committed list.  It will be put back on the
    // available list after its display time is done.
    bool Commit(BaseDisplayFrame *pDisplayFrame, Time27M start, Time27M duration);
    
    // Add a display frame to the managed display frames set
    bool AddDisplayFrame(BaseDisplayFrame *pDisplayFrame);
    
    // Remove a display fram from the managed display frame set
    bool RemoveDisplayFrame(BaseDisplayFrame *pDisplayFrame);

    // Change the duration of a committed display frame
	bool ChangeDisplayFrameDuration(BaseDisplayFrame *pDisplayFrame, Time27M duration);

	// Turn On/Off Loop mode of the Display Layer. In this mode, frame removed from display are added back to the end of the
	// commited list.
	// - By default Loop mode is Off
	// - All frames should have a start time of 'Now' and a valid duration.
	// - A call to Reset() will also turn Off loop mode
	void SetLoopMode(bool bOn);

    ////////////////////////////////////////////////
    // The following are most likely used by the HDR
    ////////////////////////////////////////////////

	// HDR must call DisplayStart() before asking for a frame, and call DisplayEnd() after it has finished using the frame
	// This allow the DisplayLayer to synchronize internal cleanup activities (e.g. Uncommit/Remove frame that is being curently used by HDR).
	void DisplayStart(void);
	void DisplayEnd(void);
    
    // Find a committed display frame that matches this time stamp    
    BaseDisplayFrame* GetCommittedDisplayFrameAt(Time27M time);

    // Put a display frame back in the available list    
    bool UnCommit(BaseDisplayFrame *pDisplayFrame);
    
    // Put all display frames into the available list and set all times and durations to zero.
    bool Reset();
    
    int GetNumberOfDisplayFrames();
    int GetNumberOfAvailableDisplayFrames();
    int GetNumberOfCommittedDisplayFrames();
    UInt32 GetZOrder();
    
    private:

	SonicCriticalSection m_AutoLock;	// Serialize vector access
	SonicCriticalSection m_DisplayLock;	// Serialize with frame display
    
    UInt32              m_nZOrder;
    UInt32              m_nTotalDisplayFrames;

    DisplayFrameArray   m_availableDisplayFrames;
    DisplayFrameArray   m_committedDisplayFrames;

	bool				m_bLoopMode;		// true if Display Layer is in Loop mode.

#ifdef _DEBUG
public:
	void RememberLastFrame(const BaseDisplayFrame* inFrame);
	Time27M GetLastFrameStartTime() const;
	Time27M GetLastFrameDuration() const;
	PiSize GetLastFrameSize() const;
	PiRect GetLastFrameSourceRect() const;
	PiRect GetLastFrameDestRect() const;

private:
	Time27M mLastFrameStartTime;
	Time27M mLastFrameDuration;
	PiSize mLastFrameSize;
	PiRect mLastFrameSourceRect;
	PiRect mLastFrameDestRect;
#endif // _DEBUG
};

enum
{
	DISPLAYLAYER_ZORDER_PG  = 0,
	DISPLAYLAYER_ZORDER_TXT = 0,
	DISPLAYLAYER_ZORDER_IG  = 1,
	DISPLAYLAYER_ZORDER_BDJ = 1,
	DISPLAYLAYER_ZORDER_FAA = 2
};

#endif
