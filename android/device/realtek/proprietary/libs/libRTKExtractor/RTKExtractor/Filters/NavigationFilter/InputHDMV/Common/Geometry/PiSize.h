//-----------------------------------------------------------------------------
// PiSize.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
/*
	File:		PiSize.h

	Contains:	Cross platform size class

	Written by:	Uri Rabin

	Change History (most recent first):

			7/10/2000		uhr		Initial file creation.
			12/12/2000		uhr		now inherit from gt::Size<SInt32>.
*/

#ifndef __PiSize_h__
#define __PiSize_h__

#include "GeometryTemplates.h"

#include "CommonTypes.h"

class PiPoint;

class PiSize :public gt::Size<SInt32>
{
private:
	typedef gt::Size<SInt32> INHERITED;

public:

				PiSize() 							: INHERITED()					{}
				PiSize(SInt32 inX, SInt32 inY) 	: INHERITED(inX, inY)			{}
				PiSize(const PiSize& rhs) 		: INHERITED(rhs)				{}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiSize(const PiPoint& rhs);
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiSize(const gt::Size<SInt32>& rhs):	INHERITED(rhs)			{}

#if	defined(_WIN32)
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiSize(const SIZE& rhs)			: INHERITED(rhs.cx, rhs.cy)	{}
#endif
				~PiSize() {}

#if	defined(_WIN32)
				SIZE		AsSIZE();
				POINT		AsPOINT();
#endif
				void	Set(SInt32 inX, SInt32 inY) 	{ SetX(inX); SetY(inY); }
};

extern const PiSize kPiSize_00;

#endif

