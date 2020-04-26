//-----------------------------------------------------------------------------
// BDROM_ElapsedTitleTimer.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BDROM_ElapsedTitleTimer_h
#define __BDROM_ElapsedTitleTimer_h

#include "BDROM_Timer.h"

class NotificationEventManager;
class BDROM_ElapsedTitleTimer: public BDROM_Timer
{
public:
	BDROM_ElapsedTitleTimer(ref_ptr<NotificationEventManager> pNEManager);
	~BDROM_ElapsedTitleTimer();

	virtual bool ProcessClock(TimeValue currentTime);

private:
	virtual void Fire(TimeValue currentTime);

	ref_ptr<NotificationEventManager>	m_pNEManager;
};

#endif
