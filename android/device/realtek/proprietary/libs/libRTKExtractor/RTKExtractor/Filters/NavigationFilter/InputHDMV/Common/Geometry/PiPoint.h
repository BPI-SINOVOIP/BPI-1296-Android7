//-----------------------------------------------------------------------------
// PiPoint.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
/*
	File:		PiPoint.h

	Contains:	Cross platform point class

	Written by:	Uri Rabin

	Change History (most recent first):

			7/6/2000		uhr		Initial file creation.
			12/12/2000		uhr		now inherit from gt::Point<SInt32>.
*/

#ifndef __PiPoint_h__
#define __PiPoint_h__

#include "CommonTypes.h"
#include "GeometryTemplates.h"

#if	defined(_WIN32)

namespace Gdiplus {
	class Point;
	class PointF;
}

#endif

class PiSize;

class PiPoint : public gt::Point<SInt32>
{
private:
	typedef gt::Point<SInt32> INHERITED;

public:
				PiPoint() 							: INHERITED()				{}
				PiPoint(SInt32 inX, SInt32 inY)	: INHERITED(inX, inY)		{}
				PiPoint(const PiPoint& rhs) 		: INHERITED(rhs)			{}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiPoint(const PiSize& rhs);
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiPoint(const gt::Point<SInt32>& rhs):	INHERITED(rhs)		{}

#if	defined(_WIN32)
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiPoint(const POINT& rhs)			: INHERITED(rhs.x, rhs.y)	{}
#endif
				~PiPoint() {}


#if	defined(_WIN32)
				POINT		AsPOINT() const;
				SIZE		AsSIZE() const;

				Gdiplus::Point &	AsPoint(Gdiplus::Point &) const;
				Gdiplus::PointF &	AsPointF(Gdiplus::PointF &) const;
#endif
				void		Set(SInt32 inX, SInt32 inY) 		{ SetX(inX); SetY(inY); }
};

extern const PiPoint kPiPoint_00;

class PiFPoint : public gt::Point<float>
{
private:
	typedef gt::Point<float> INHERITED;

public:
				PiFPoint() 							: INHERITED()				{}
				PiFPoint(SInt32 inX, SInt32 inY)	: INHERITED(static_cast<float>(inX), static_cast<float>(inY))		{}
				PiFPoint(float inX, float inY)	: INHERITED(inX, inY)		{}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiFPoint(const PiPoint& rhs) 		: INHERITED(static_cast<float>(rhs.X()), static_cast<float>(rhs.Y()))			{}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiFPoint(const gt::Point<float>& rhs):	INHERITED(rhs)		{}

				~PiFPoint() {}
#if	defined(_WIN32)
				Gdiplus::PointF &	AsPointF(Gdiplus::PointF &out) const;
#endif
				void		Set(SInt32 inX, SInt32 inY) 		{ SetX(static_cast<float>(inX)); SetY(static_cast<float>(inY)); }
				void		Set(float inX, float inY) 		{ SetX(inX); SetY(inY); }
};


#endif

