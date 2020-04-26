//-----------------------------------------------------------------------------
// BDROM_SecondaryVideoPositionTimer.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BDROM_SecondaryVideoPositionTimer_h
#define __BDROM_SecondaryVideoPositionTimer_h

#include "BDROM_Timer.h"

class BaseRenderer;
namespace BDROM
{
	struct pip_metadata_entry;
	struct STN_table;
}

class BDROM_SecondaryVideoPositionTimer: public BDROM_Timer
{
public:
	BDROM_SecondaryVideoPositionTimer(PrecisionClockID clockID,
									  BDROM::STN_table* pSTNTable,
									  std::vector<BDROM::pip_metadata_entry>& pip_metadata_entries,
									  unsigned secVideo,
									  ref_ptr<BaseRenderer> pRenderer);
	~BDROM_SecondaryVideoPositionTimer();

	virtual void Start(TimeValue startTime);

private:
	virtual void Fire(TimeValue currentTime);

	void ChangePosition();

	std::vector<BDROM::pip_metadata_entry>&	mpip_metadata_entries;
	size_t									m_currentEntry;
	BDROM::STN_table					*	m_pSTNTable;
	unsigned								m_secVideo;
	ref_ptr<BaseRenderer>					m_pRenderer;

	BDROM_SecondaryVideoPositionTimer(const BDROM_SecondaryVideoPositionTimer&);
	const BDROM_SecondaryVideoPositionTimer& operator=(const BDROM_SecondaryVideoPositionTimer&);
};
#endif
