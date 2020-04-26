#ifndef __SysPrecisionClock_h__
#define __SysPrecisionClock_h__

#include "BasePrecisionClock.h"

class SysPrecisionClock : public BasePrecisionClock 
{
public:
	SysPrecisionClock();
	virtual ~SysPrecisionClock();	

protected:
	virtual TimeValue	GetMasterTime();
	int					mCountPerSec;
	int64_t				mInitialSysOffset;
};


#endif
