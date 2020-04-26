//-----------------------------------------------------------------------------
// BaseDisplayFrame.h
// Copyright (c)  Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BASE_DISPLAY_FRAME_H_
#define _BASE_DISPLAY_FRAME_H_


#include "TimeValue.h"
//#include "ref_ptr.h"
#include <vector>
#include "PiRect.h"

typedef enum tagDisplayFrameState
{
    FRAME_STATE_NEED_TO_INIT = 0,
    FRAME_STATE_AVAILABLE,
    FRAME_STATE_IN_USE,
    FRAME_STATE_COMMITTED
} DisplayFrameState;

// Frame Dirty Flags
#define FRAME_DIRTYFLAG_NONE			0x0				// No Dirty Flags
#define FRAME_DIRTYFLAG_SRC_RECT		0x00000001		// Src Rect changed
#define FRAME_DIRTYFLAG_DEST_RECT		0x00000002		// Dest Rect changed

class BaseDisplayFrame;
typedef std::vector<BaseDisplayFrame *> DisplayFrameArray;

class BaseDisplayFrame
{
    public:
    
    BaseDisplayFrame(UInt32 width, UInt32 height);
    virtual ~BaseDisplayFrame();

    // Functions with these names need to be created in derived classes.
    // Since these are very platform specific they are just provided here
    // for example.  Most likely they will take parameters like a color
    // or a hardware device which can create and clear video memory.
    virtual void Destroy(void) = 0;
    virtual void Clear(UInt8 alpha = 0,
                       UInt8 red = 0,
                       UInt8 green = 0,
                       UInt8 blue = 0) = 0;

    void SetStartTime(Time27M time);
    Time27M GetStartTime() const;

    void SetDuration(Time27M duration);
    Time27M GetDuration() const;
    
    void SetState(DisplayFrameState state);
    DisplayFrameState GetState() const;

    UInt32 GetWidth() const;
    UInt32 GetHeight() const;

	// Controls whether frame will be kept on display even after duration has elapsed if there is no next frame to display
	// this is to avoid flickering (just in case client does not produce the next frame fast enough)
	void ForceDisplayTillNextFrame(bool bForce);
	bool IsForceDisplayTillNextFrame(void) const;

	// Sets new source rect.
	// Can be set only when frame is FRAME_IN_USE. Returns true if source rect is set.
	bool SetSourceRect(PiRect & srcRect);
	PiRect GetSourceRect(void) const;

	bool SetDestRect(const PiRect & srcRect);
	PiRect GetDestRect(void) const;
    
	void SetDirtyFlag(UInt32 df);
	void ClearDirtyFlag(UInt32 df);
	bool IsDirty(void) const;
	bool IsDirtyFlagSet(UInt32 df) const;

    protected:

    DisplayFrameState   m_state;
    UInt32              m_width;
    UInt32              m_height;
	Time27M             m_timeStart;
    Time27M             m_timeDuration;
	PiRect				m_srcRect;
	PiRect				m_destRect;
	UInt32				m_dirtyFlags;
	bool				m_bKeepTillNextFrame;
};

#endif
