//-----------------------------------------------------------------------------
// PiRect.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
/*
	File:		PiRect.h

	Contains:	Cross platform rectangle class

	Written by:	Uri Rabin

	Change History (most recent first):

			7/6/2000		uhr		Initial file creation.
			12/12/2000		uhr		now inherit from gt::Rect<SInt32>.
*/

#ifndef __PiRect_h__
#define __PiRect_h__

#include "GeometryTemplates.h"

#include "CommonTypes.h"
#include "PiPoint.h"
#include "PiSize.h"

typedef enum {
	eHandleNone		= 0,
	eTopLeft		= 1,
	eTop			= 2,
	eTopRight		= 3,
	eRight			= 4,
	eBottomRight	= 5,
	eBottom			= 6,
	eBottomLeft		= 7,
	eLeft			= 8,
	eCenter			= 9
} eResizeHandles;

class 	PiPoint;
class	PiSize;

namespace Gdiplus {
	class Rect;
	class RectF;
}


class PiRect : public gt::Rect<SInt32>
{
private:
	typedef		gt::Rect<SInt32> INHERITED;

public:
				PiRect() 					: INHERITED()								{}
				PiRect(SInt32 left, SInt32 top, SInt32 right, SInt32 bottom) 
											: INHERITED(left, top, right, bottom)		{}

				PiRect(const PiRect& rhs) : INHERITED(rhs)							{}
				PiRect(const PiPoint& tl, const PiSize& size) : INHERITED(tl, size)	{}
											  							 
				PiRect(const PiPoint& tl, const PiPoint& br)	: INHERITED(tl, br)	{}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiRect(const gt::Rect<SInt32>& rhs)			: INHERITED(rhs)		{}
#if defined(__MACTYPES__)
				PiRect(const ::Rect& rhs)	: INHERITED(rhs.left, rhs.top, rhs.right, rhs.bottom)	{}
#endif

#if defined(_WIN32)
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiRect(const RECT& rhs)	: INHERITED(rhs.left, rhs.top, rhs.right, rhs.bottom)	{}
#endif
				~PiRect() {}

				PiPoint		Position() const	{ return PiPoint(TL()); }

#if defined __MACTYPES__
				::Rect		AsRect();
#endif
#if	defined(_WIN32)
				void				AsRECT(RECT &rOut);
				Gdiplus::Rect &		AsRect(Gdiplus::Rect &) const;
				Gdiplus::RectF &	AsRectF(Gdiplus::RectF &) const;
#endif

				void		Set(SInt32 left, SInt32 top, SInt32 right, SInt32 bottom) 
												{ SetLeft(left); SetTop(top); SetRight(right); SetBottom(bottom); }
				void		Resize(const PiPoint& inDelta, eResizeHandles inHandle);
				void		ConstAspectRatioResize(const PiPoint& inDelta, eResizeHandles inHandle);
				PiPoint		GetHandlePosition(eResizeHandles inHandle) const;

};

extern const PiRect kPiRect_0000;

class PiFRect : public gt::Rect<float>
{
private:
	typedef		gt::Rect<float> INHERITED;

public:
				PiFRect() 					: INHERITED()								{}
				PiFRect(SInt32 left, SInt32 top, SInt32 right, SInt32 bottom) 
											: INHERITED(static_cast<float>(left), static_cast<float>(top),
														 static_cast<float>(right), static_cast<float>(bottom))		{}
				PiFRect(float left, float top, float right, float bottom) 
											: INHERITED(left, top, right, bottom)		{}

// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiFRect(const PiRect& rhs) : INHERITED(static_cast<float>(rhs.Left()), static_cast<float>(rhs.Top()),
														  static_cast<float>(rhs.Right()), static_cast<float>(rhs.Bottom())) {}
				PiFRect(const PiFRect& rhs) : INHERITED(rhs)						{}
															  							 
				PiFRect(const PiFPoint& tl, const PiFPoint& br)	: INHERITED(tl, br)	{}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiFRect(const gt::Rect<float>& rhs)			: INHERITED(rhs)		{}
#if defined(_WIN32)
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiFRect(const Gdiplus::Rect& rhs);
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
				PiFRect(const Gdiplus::RectF& rhs);
#endif
				~PiFRect() {}

				PiFPoint	Position() const	{ return PiFPoint(TL()); }


#if	defined(_WIN32)
				Gdiplus::RectF &	AsRectF(Gdiplus::RectF &) const;
#endif

				void		Set(SInt32 left, SInt32 top, SInt32 right, SInt32 bottom) 
							{ 
								SetLeft(static_cast<float>(left)); 
								SetTop(static_cast<float>(top)); 
								SetRight(static_cast<float>(right)); 
								SetBottom(static_cast<float>(bottom)); 
							}
				void		Set(float left, float top, float right, float bottom) 
							{ 
								SetLeft(left); 
								SetTop(top); 
								SetRight(right); 
								SetBottom(bottom); 
							}
};

#endif

