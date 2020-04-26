//-----------------------------------------------------------------------------
// TimeValue.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//	Time value type used for HD-DVD and possibly others.

#ifndef __TimeValue__
#define __TimeValue__

#include "Time27M.h"

typedef Time27M		TimeValue;			// common type for all time values

static const TimeValue kOneSecondTimeValue = OneSecond;			// TimeValue of one second

#ifndef REALTEK_MODIFY
static const TimeValue kInfiniteTimeValue = Forever27M;			// TimeValue of "infinity"
static const TimeValue kUnresolvedTimeValue = TIME27M_MIN;		// TimeValue of "unresolved"

inline TimeValue ToTimeValue(Time10M pts)
{
	return (To27M(pts));
}

inline TimeValue ToTimeValue(Time90K pts)
{
	return (To27M(pts));
}

inline Time10M TimeValueTo10M(TimeValue pts)
{
	return (To10M(pts));
}
#endif //REALTEK_MODIFY
#endif
