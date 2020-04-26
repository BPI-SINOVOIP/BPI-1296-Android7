//-----------------------------------------------------------------------------
// Time27M.h
// Copyright (c) 1998 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//SR FS: Reviewed [DDT 20040924] 
//SR FS: Reviewed [JSB 20040927] 

#ifndef TIME27M
#define TIME27M

#ifdef REALTEK_MODIFY
#include "CommonTypes.h"

typedef SInt64		Time27M;
typedef UInt32		Time90K;

static const Time27M OneSecond				= 27000000;
static const Time90K OneSecond90K			= 90000;

inline Time90K To90KFrom45K(UInt32 time45K)
{
	//To prevent overflow, saturate result to original value.
	return (time45K & 0x80000000) ? time45K : (time45K << 1);
}

#else

#include <climits>
#include "CommonTypes.h"

// type for presentation and decoding times

typedef SInt64		Time1323M;
typedef SInt64		Time27M;
typedef SInt64		Time10M;	// Clock used for VideoEditing Core and DirectShow

typedef UInt32		Time90K;
typedef double		Time1;

static const Time27M PALFieldDuration		= 540000;
static const Time27M NTSCFieldDuration		= 450450;
static const Time27M AC3FieldDuration		= 864000;
static const Time27M AC3BlockDuration		= 864000/6;
static const Time27M MPAFieldDuration		= 648000;
static const Time27M DTSFieldDuration		= 288000;
static const Time27M VPCMFieldDuration		= 45000;
static const Time27M APCMFieldDuration		= 22500;
static const Time27M One90K					= 300;
static const Time27M NTSC3000FieldDuration	= 450000;
static const Time27M NTSC2400FieldDuration	= 562500;


static const Time27M OneSecond				= 27000000;
static const Time90K OneSecond90K			= 90000;
static const Time10M OneSecond10M			= 10000000;
static const Time27M OneSecond1323			= 1323000000;

static const Time27M PAL1323Duration		= 26460000;
static const Time27M NTSC1323Duration		= 22072050;
static const Time27M AC31323Duration		= 42336000;
static const Time27M MPA1323Duration		= 31752000;
static const Time27M MPA441k1323Duration	= 34560000;
static const Time27M DTS1323Duration		= 14112000;
static const Time27M VPCM48K1323Duration	= 2205000;
static const Time27M VPCM441K1323Duration	= 2400000;
static const Time27M APCM48K1323Duration	= 1102500;
static const Time27M APCM441K1323Duration	= 1200000;
static const Time27M One90K1323				= 14700;
static const Time27M One27M1323				= 49;

#define TIME27M_MAX 0x7fffffffffffffffLL
#define TIME27M_MIN 0x8000000000000000LL

// Define this as somewhat less than TIME27M_MAX so we can add to it and not wrap.
static const Time27M Forever27M = 0x0fffffffffffffffLL;


static const Time10M kInfiniteDuration10M = static_cast<Time10M>(static_cast<double>(OneSecond10M) * -1.0);
static const Time10M UnknownTime10M = 0x8000000000000000LL;


inline Time90K To90K(Time27M pts)
{
	return static_cast<Time90K>(pts / One90K);
}

inline Time90K Time10MTo90K(Time10M pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time90K>(static_cast<double>(pts) / (static_cast<double>(OneSecond10M) / static_cast<double>(OneSecond90K)) + round);
}

inline Time90K To90K(Time1 pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time90K>(pts * static_cast<Time1>(OneSecond90K) + round);
}

inline Time27M To27M(Time90K pts)
{
	return static_cast<Time27M> (pts * One90K);
}

inline Time27M To27M(Time1 pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time27M>(pts * static_cast<Time1>(OneSecond) + round);
}

inline Time27M To27M(Time10M pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time27M>(static_cast<double>(pts) * (static_cast<double>(OneSecond) / static_cast<double>(OneSecond10M)) + round);
}

inline Time1323M To1323M(Time27M pts)
{
	return pts * One27M1323;
}

inline Time1323M Time90KTo1323M(Time90K pts)
{
	return pts * One90K1323;
}

inline Time1323M To1323M(Time1 pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time1323M>(pts * static_cast<Time1>(OneSecond1323) + round);
}

inline Time1 To1(Time27M pts)
{
	return static_cast<Time1>(pts) / static_cast<Time1>(OneSecond);
}

inline Time1 To1(Time90K pts)
{
	return static_cast<Time1>(pts) / static_cast<Time1>(OneSecond90K);
}

inline Time1 Time10MTo1(Time10M pts)
{
	return static_cast<Time1>(pts) / static_cast<Time1>(OneSecond10M);
}

inline Time1 Time1323MTo1(Time10M pts)
{
	return static_cast<Time1>(pts) / static_cast<Time1>(OneSecond1323);
}

inline Time10M To10M(Time90K pts)
{
	return static_cast<Time10M>(static_cast<double>(pts) * (static_cast<double>(OneSecond10M) / static_cast<double>(OneSecond90K)) + 0.5);
}

inline Time10M To10M(Time27M pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time10M>(static_cast<double>(pts) / (static_cast<double>(OneSecond) / static_cast<double>(OneSecond10M)) + round);
}

inline Time10M To10M(Time1 pts)
{
	const double round = (pts < 0 ? -0.5 : 0.5);
	return static_cast<Time10M>(pts * static_cast<Time1>(OneSecond10M) + round);
}


inline Time27M TruncTo27M(Time1323M pts)
{
	return pts / One27M1323;
}

inline Time27M TruncTo(Time27M pts, Time27M grid)
{
	Time27M temp = (pts/grid)*grid;
	while (temp > pts) temp -= grid;
	return temp;
}

inline Time27M TruncTo27M(Time1 pts, Time27M grid)
{
	return TruncTo(To27M(pts), grid);
}

inline Time1323M TruncTo1323M(Time1 pts, Time1323M grid)
{
	return TruncTo(To1323M(pts), grid);
}

inline Time27M RoundTo(Time27M pts, Time27M grid)
{
	return TruncTo((pts + (grid >> 1)), grid);
}

inline Time27M RoundTo27M(Time1 pts, Time27M grid)
{
	return TruncTo((To27M(pts) + (grid >> 1)), grid);
}

inline Time1323M RoundTo1323M(Time1 pts, Time1323M grid)
{
	return TruncTo((To1323M(pts) + (grid >> 1)), grid);
}
#endif //REALTEK_MODIFY
#endif
