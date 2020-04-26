//-----------------------------------------------------------------------------
// BDROM_NavigationTimer.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BDROM_NavigationTimer_h
#define __BDROM_NavigationTimer_h

#include "BDROM_Timer.h"

class NotificationEventManager;
class BDROM_NavigationTimer: public BDROM_Timer
{
public:
	BDROM_NavigationTimer(ref_ptr<NotificationEventManager> pNEManager, UInt32 mobj_ID, TimeValue interval);
	~BDROM_NavigationTimer();

	virtual void Start(TimeValue currentTime);
	virtual bool ProcessClock(TimeValue currentTime);

protected:
	virtual void Fire(TimeValue currentTime);

private:
	ref_ptr<NotificationEventManager>	m_pNEManager;
	UInt32								m_mobj_ID;
	TimeValue							m_tvTotalDuration;
	TimeValue							m_tvStartTime;
};
#endif
