//-----------------------------------------------------------------------------
// SonicPBCE_EventSink.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_EVENTSINK_H__
#define __SONICPBCE_EVENTSINK_H__

#include "SonicPBCE_Registers.h"
#include "SonicPBCE_CommonTypes.h"
#include "BDROM_StreamControl.h"
#include "BDROM_NavigatorTypes.h"


// BDROM_SonicPBCE_EventSink
// Internal Event Sink for use by all the PBCE components. This is meant to be implemented by the PlayList Manager
//
class BDROM_SonicPBCE_EventSink
{
public:
	//------------------------------
	// events meant to be sent by PBCE_Clips
	//

	// EndOfClip: clip data fully read.
	virtual void EndOfClip(PBCE_PathType pathType, bool m_bMainPath)			= 0;

	// PreRollDataRead: enough data read for PreRoll. Sent only by MainPath Clip
	virtual void PreRollDataRead(void)														= 0;

	//------------------------------
	// Events meant to be sent by PBCE_Paths
	//

	// EndOfItem: PlayItem / SubPlayItem end reached
	virtual void EndOfItem(PBCE_PathType pathType)											= 0;

	//------------------------------
	// Regular BDROM events
	//

	// SendEvent: forwards events up the chain
	virtual void SendEvent(BDROM_Event & evt)												= 0;
};

#endif
