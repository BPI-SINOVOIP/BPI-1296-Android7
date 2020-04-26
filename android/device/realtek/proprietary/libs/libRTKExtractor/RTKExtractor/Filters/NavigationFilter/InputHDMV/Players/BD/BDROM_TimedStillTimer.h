//-----------------------------------------------------------------------------
// BDROM_TimedStillTimer.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BDROM_TimedStillTimer_h
#define __BDROM_TimedStillTimer_h

#include "BDROM_Timer.h"

class NotificationEventManager;

class BDROM_TimedStillTimer: public BDROM_Timer
{
public:
	BDROM_TimedStillTimer(ref_ptr<NotificationEventManager> pNEManager, TimeValue interval);
	~BDROM_TimedStillTimer();

protected:
	virtual void Fire(TimeValue currentTime);

private:
	ref_ptr<NotificationEventManager>	m_pNEManager;
};
#endif
