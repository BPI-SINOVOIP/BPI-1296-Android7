//-----------------------------------------------------------------------------
// BasePrecisionClock.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
/*
//	Base class for high-precision clock objects.
//
//	In standard use, the Precision Clock contains one master clock time. This is only 
//	updated to the system when SynchronizeClocks() is called. All owned 
//	clocks are adjusted by their relative offsets to the master time. SynchronizeClocks()
//	is called on every call to GetClockTime(). This can result in two clocks drifting out
//	of sync with each other if you iteratively call to get all clocks one-by-one. As such,
//	if you need multiple clocks locked to the same master time you should call
//	GetClockTimes() and that will return a list of all clocks with times synchronized to the
//	same system time slice.
//
//	Individual clocks can be Paused and Resumed via the described API. When resumed, they
//	will be resynchronized to the current master clock time and therefore report times continuously
//	to clients without any sudden "jump" in reported time.
//
//	Error Handling: Throws exceptions for all unexpected errors.5
*/

#ifndef __BasePrecisionClock__
#define __BasePrecisionClock__

//#include "RendererTypes.h"
#include "SystemHeaders.h"
#include "CommonTypes.h"
#include "ref_ptr.h"
#include "TimeValue.h"
#include <list>
#include <map>
#include "HDMVDebugUtil.h"

typedef		UInt32		PrecisionClockID;
typedef std::map< PrecisionClockID, TimeValue > MapClockTimes;

class RENDERER_EXPORT BasePrecisionClock 
{
public:
	//____ PrecisionClockInfo __________________________________________________________________________
	//
	// Container class for an arbitrary clock based on the master clock. Information about
	// synchronization, pause states, id and the like can be found here for any client-created
	// clock.
	//
	class PrecisionClockInfo
	{
		public:
			enum PrecisionClockMode
			{
				eMode_Automatic = 1,			//clock updates automatically based on Synchronize
				eMode_Manual = 2				//clock never updates on Synch(), must be manually advanced
			};

		public:
			explicit PrecisionClockInfo(PrecisionClockID id) :
			mClockID(id), mbPaused(false), mClockTime(0), mClockOffset(0), mMode(eMode_Automatic) 
			{
				HDMV_DETECT_MEM_LEAK("[%s %d] create  PrecisionClockInfo ClockID = %ld ++++++++++++++++++++++++++++\n", __FILE__, __LINE__, id);
			};
			~PrecisionClockInfo()
			{
				HDMV_DETECT_MEM_LEAK("[%s %d] delete  PrecisionClockInfo ClockID = %ld ----------------------------\n", __FILE__, __LINE__, mClockID);
			};

		public:
			PrecisionClockID	mClockID;
			bool				mbPaused;		//whether updating per Synch
			TimeValue			mClockTime;		//last value per Synch
			TimeValue			mClockOffset;	//offset to running master clock
			PrecisionClockMode	mMode;			//what mode is the clock running in?
	};	
	

public:
	BasePrecisionClock();
	virtual ~BasePrecisionClock();

	void ResetClocks();
	void GetClockTimes(MapClockTimes& timeMap);							//Get synchronized times of all clocks	

	//convenience function - gets a particular clock from a previously sync'd call to GetClockTimes.
	static bool GetSyncClockTime(MapClockTimes& timesIn, PrecisionClockID clock, TimeValue& timeOut);

	//per-clock operations
	bool GetClockTime(PrecisionClockID clock, TimeValue& timeOut);			//Get time of a specific clock, will synchronize	
	bool ClockExists(PrecisionClockID clock);
	bool AddClock(PrecisionClockID clock);									//all clocks are added active, you must pause if you don't want it to run
	bool DeleteClock(PrecisionClockID clock);
	bool PauseClock(PrecisionClockID clock);								//time on a paused clock will not advance
	bool IsClockPaused(PrecisionClockID clock, bool& bIsPausedOut);
	bool ResumeClock(PrecisionClockID clock);								//start time running again on a previously paused clock
	bool ResetClock(PrecisionClockID clock);								//reset a given clock to time 0
	bool SetClock(PrecisionClockID clock, TimeValue timeIn);				//set a clock to a new time 
	bool AdvanceClock(PrecisionClockID clock, TimeValue timeDelta);			//change a clock by a particular amount of time 
	bool SetClockMode(PrecisionClockID clock, PrecisionClockInfo::PrecisionClockMode mode);		//change a clock to operate in a different mode

protected:
	//this must be called prior to calling GetClockTime() - it will update all clocks
	void SynchronizeClocks();

	virtual TimeValue GetMasterTime() = 0;					//master system time - cannot be modified. System-specific version must
															//be created that can get high-precision clock info when called

#ifdef COMRENDERER

public:
	BasePrecisionClock(void *);

protected:
	void * mPtr;	// opaque pointer to COM interface

#else	// COMRENDERER

protected:
	TimeValue												mSyncTime;		//time set on last Synchronize
	typedef std::list< ref_ptr<PrecisionClockInfo> >		ClockList;
	ref_ptr< ClockList >									mpClockList;

#endif	// COMRENDERER
};


#endif
