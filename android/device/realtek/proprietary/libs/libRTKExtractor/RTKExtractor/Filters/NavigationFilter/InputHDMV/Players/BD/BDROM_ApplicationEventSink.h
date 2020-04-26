//-----------------------------------------------------------------------------
// BDROM_ApplicationEventSink.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_APPLICATIONEVENTSINK_H__
#define __BDROM_APPLICATIONEVENTSINK_H__

#include "BDROM_NavigatorTypes.h"

/// @class	BDROMAppEventSink
/// @brief	Interface that must be implemented by the BDROM Player platform specific wrapper in order to receive events from the core BDROM Navigator
///			Applcation must also register the object that implements this interface with the BDROM Navigator (by calling RegisterApplicationEventSink())
///
class BDROMAppEventSink
{
public:
	BDROMAppEventSink()					{ }
	virtual ~BDROMAppEventSink()		{ }

	virtual void NotifyEvent(BDROM_Event event) = 0;
};

#endif // __BDROM_APPLICATIONEVENTSINK_H__

