//-----------------------------------------------------------------------------
// BDROM_Timer.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BDROM_Timer_h
#define __BDROM_Timer_h

#include "SysPrecisionClock.h"
#include "TimeValue.h"

class BDROM_Timer
{
public:
	BDROM_Timer(PrecisionClockID clockID, TimeValue interval, bool isLoop);

	virtual ~BDROM_Timer();

	bool Accept(PrecisionClockID clockID)		{ return m_clockID == clockID; }
	virtual void Start(TimeValue currentTime)	{ m_lastFiredTime = currentTime; }

	virtual bool ProcessClock(TimeValue currentTime);

	bool IsLoop()								{ return m_isLoop; }
	void MarkDelete()							{ m_isDeleting = true; }
	bool IsDeleting()							{ return m_isDeleting; }

	PrecisionClockID ClockId(void)				{ return m_clockID; }

protected:
	virtual void Fire(TimeValue currentTime) = 0;
	void SetLooping(bool bLooping)				{ m_isLoop = bLooping; }

private:
	PrecisionClockID m_clockID;
	bool m_isLoop;
	bool m_isDeleting;

protected:
	TimeValue m_interval;
	TimeValue m_lastFiredTime;
};
#endif
