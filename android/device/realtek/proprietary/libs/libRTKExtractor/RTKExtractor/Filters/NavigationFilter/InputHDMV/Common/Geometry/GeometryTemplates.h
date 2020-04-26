//-----------------------------------------------------------------------------
// GeometryTemplates.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef GEOMETRY_TEMPLATES
#define GEOMETRY_TEMPLATES

#include <cmath>
#include <algorithm>

// A workaround to avoid compile errors due to conflicts with std::min/std::max.
#undef min
#undef max

#if defined(_WIN32)
#pragma warning(disable:4068)	// unknown pragma
#endif

namespace gt
{
template <class T>
T minzero(const T& arg)
{
	return (arg >= T(0) ? arg : T(0));
}

#if defined(_WIN32)
#pragma mark -
#pragma mark Class Size
#pragma mark -
#endif

//-----------------------------------------------------------------------------
// class Size
//-----------------------------------------------------------------------------

template <class T, class Scale  = double >
class Size
// **CodeWizzard** - Severe Violation:  Universal Coding Standards item 28  - Do not overload functions within a template clas
{
public:
	typedef gt::Size<T, Scale> SizeType;
	typedef T CoordType;
	typedef Scale ScaleType;

private:
	T x;
	T y;

public:
	Size() : x(T(1)), y(T(1)) {}
	Size(const T& inX, const T& inY) : x(gt::minzero(inX)), y(gt::minzero(inY)) {}
	template <class U, class UScale>
	Size(const gt::Size<U, UScale>& arg) : x(static_cast<T>(arg.X())), y(static_cast<T>(arg.Y())) {}
// **CodeWizzard** - Violation:  More Effective C++ item 24  - Understand the costs of virtual functions ...
	virtual ~Size() {}

	SizeType& operator=(const SizeType& arg)
	{
		if (&arg == this) return *this;
		x = arg.x;
		y = arg.y;
		return *this;
	}

	const T& X() const { return x; }
	const T& Y() const { return y; }
	void SetX(const T& inX) { x = inX; }
	void SetY(const T& inY) { y = inY; }

	bool operator!() const
	{
		return (x == T(0)) || (y == T(0));
	}

	bool IsZero() const
	{
		return(!x && !y);
	}

	// Binary operations with Size

 	bool operator==(const SizeType& arg) const
 	{
 		if (arg.x != x) return false;
 		if (arg.y != y) return false;
 		return true;
 	}
 
 	bool operator!=(const SizeType& arg) const
 	{
 		return !(arg == *this);
 	}
 
 	const SizeType& operator*=(const SizeType& arg)
 	{
 		x *= arg.x;
 		y *= arg.y;
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const SizeType operator*(const SizeType& arg) const
 	{
 		SizeType temp(*this);
 		temp *= arg;
 		return temp;
 	}
 
 	const SizeType& operator/=(const SizeType& arg)
 	{
 		x /= arg.x;
 		y /= arg.y;
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const SizeType operator/(const SizeType& arg) const
 	{
 		SizeType temp(*this);
 		temp /= arg;
 		return temp;
 	}
 
 	// Binary operations with Scale
 
 	const SizeType& operator*=(Scale factor)
 	{
 		factor = minzero(factor);
		
		// fixes GCC 4.0 error about double being assinged to SInt32.
 		x = static_cast<T>(x * factor);
 		y = static_cast<T>(y * factor);
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	SizeType operator*(Scale factor) const
 	{
 		SizeType temp(*this);
 		temp *= factor;
 		return temp;
 	}
 
 	const SizeType& operator/=(Scale factor)
 	{
 		factor = minzero(factor);
 		x = static_cast<T>(x / factor);
 		y = static_cast<T>(y / factor);
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	SizeType operator/(Scale factor) const
 	{
 		SizeType temp(*this);
 		temp /= factor;
 		return temp;
 	}
 
};

template <class T, class Scale>
Size<T, Scale> operator*(Scale factor, const Size<T, Scale>& sz)
{
	return sz*factor;
}

#if defined(_WIN32)
#pragma mark -
#pragma mark Class Point
#pragma mark -
#endif

//-----------------------------------------------------------------------------
// class Point
//-----------------------------------------------------------------------------

template <class T, class Scale  = double >
class Point
// **CodeWizzard** - Severe Violation:  Universal Coding Standards item 28  - Do not overload functions within a template clas
{
public:
	typedef gt::Size<T, Scale> SizeType;
	typedef gt::Point<T, Scale> PointType;
	typedef T CoordType;
	typedef Scale ScaleType;

private:
	T x;
	T y;

public:
	Point() : x(T(0)), y(T(0)) {}
	Point(const T& inX, const T& inY) : x(inX), y(inY) {}
	template <class U, class UScale>
	Point(const gt::Point<U, UScale>& arg) : x(static_cast<T>(arg.X())), y(static_cast<T>(arg.Y())) {}
	explicit Point(const SizeType& arg) : x(arg.X()), y(arg.Y()) {}
// **CodeWizzard** - Violation:  More Effective C++ item 24  - Understand the costs of virtual functions ...
	virtual ~Point() {}

	PointType& operator=(const PointType& arg)
	{
		if (&arg == this) return *this;
		x = arg.x;
		y = arg.y;
		return *this;
	}

	// Accessors

	const T& X() const { return x; }
	void SetX(const T& inX) { x = inX; }

	const T& Y() const { return y; }
	void SetY(const T& inY) { y = inY; }

	bool IsZero() const
	{
		return(!x && !y);
	}

	// Binary operations with points

 	bool operator==(const PointType& arg) const
 	{
 		if (arg.x != x) return false;
 		if (arg.y != y) return false;
 		return true;
 	}
 
 	bool operator!=(const PointType& arg) const
 	{
 		return !(arg == *this);
 	}
 
 	const PointType& operator+=(const PointType& arg)
 	{
 		x = static_cast<T>(x+arg.x);
 		y = static_cast<T>(y+arg.y);
 		return *this;
 	}
 
 	const PointType& operator+=(const SizeType& arg)
 	{
 		x = static_cast<T>(x+arg.X());
 		y = static_cast<T>(y+arg.Y());
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator+(const PointType& arg) const
 	{
 		PointType temp(*this);
 		temp += arg;
 		return temp;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator+(const SizeType& arg) const
 	{
 		PointType temp(*this);
 		temp += arg;
 		return temp;
 	}
 
 	const PointType& operator-=(const PointType& arg)
 	{
 		x = static_cast<T>(x-arg.x);
 		y = static_cast<T>(y-arg.y);
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator-(const PointType& arg) const
 	{
 		PointType temp(*this);
 		temp -= arg;
 		return temp;
 	}
 
 	// Binary operations with Scale
 
 	const PointType& operator*=(Scale factor)
 	{
 		x = static_cast<T>(x*factor);
 		y = static_cast<T>(y*factor);
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator*(Scale factor) const
 	{
 		PointType temp(*this);
 		temp *= factor;
 		return temp;
 	}
 
 	const PointType& operator/=(Scale factor)
 	{
 		x = static_cast<T>(x/factor);
 		y = static_cast<T>(y/factor);
 		return *this;
 	}
 
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator/(Scale factor) const
 	{
 		PointType temp(*this);
 		temp /= factor;
 		return temp;
 	}
 
 	// Binary operations with Size
 
 	template <class U>
 	const PointType& operator*=(const gt::Size<U>& arg)
 	{
 		x = static_cast<T>(x*arg.X());
 		y = static_cast<T>(y*arg.Y());
 		return *this;
 	}
 
 	template <class U>
 	const PointType& operator/=(const gt::Size<U>& arg)
 	{
 		x = static_cast<T>(x/arg.X());
 		y = static_cast<T>(y/arg.Y());
 		return *this;
 	}
 
 	template <class U>
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator/(const gt::Size<U>& arg) const
 	{
 		PointType temp(*this);
 		temp /= arg;
 		return temp;
 	}
 
 	template <class U>
// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
 	const PointType operator*(const gt::Size<U>& arg) const
 	{
 		PointType temp(*this);
 		temp *= arg;
 		return temp;
 	}
 };

template <class T, class Scale>
gt::Point<T, Scale> operator*(Scale factor, const Point<T, Scale>& pt)
{
	return pt*factor;
}

template <class T, class Scale, class U, class UScale>
gt::Point<T, Scale> operator*(const Size<U, UScale>& sz, const gt::Point<T, Scale>& pt)
{
	gt::Point<T, Scale> temp(pt);
	temp *= sz;
	return temp;
}

#if defined(_WIN32)
#pragma mark -
#pragma mark Class Rect
#pragma mark -
#endif

template< class T, class Scale = double >
class OriginTopLeft
{
public:
	typedef gt::Size<T, Scale> SizeType;
	typedef gt::Point<T, Scale> PointType;
//	typedef gt::Rect<T, Scale> RectType;
	typedef T CoordType;
	typedef Scale ScaleType;

	static bool IsLeftOf(T c1, T c2)
	{
		return c1 < c2;
	}

	static bool IsLeftOrEqual(T c1, T c2)
	{
		return c1 <= c2;
	}

	static T Leftmost(T c1, T c2)
	{
		return std::min(c1, c2);
	}

	static bool IsRightOf(T c1, T c2)
	{
		return c1 > c2;
	}

	static bool IsRightOrEqual(T c1, T c2)
	{
		return c1 >= c2;
	}

	static T Rightmost(T c1, T c2)
	{
		return std::max(c1, c2);
	}

	static bool IsAbove(T c1, T c2)
	{
		return c1 < c2;
	}

	static bool IsAboveOrEqual(T c1, T c2)
	{
		return c1 <= c2;
	}

	static T Topmost(T c1, T c2)
	{
		return std::min(c1, c2);
	}

	static bool IsBelow(T c1, T c2)
	{
		return c1 > c2;
	}

	static bool IsBelowOrEqual(T c1, T c2)
	{
		return c1 >= c2;
	}

	static T Bottommost(T c1, T c2)
	{
		return std::max(c1, c2);
	}

	static SizeType Size(const PointType& tl, const PointType& br)
	{
		return SizeType(br - tl);
	}
};

template< class T, class Scale = double >
class OriginBottomLeft
{
public:
	typedef gt::Size<T, Scale> SizeType;
	typedef gt::Point<T, Scale> PointType;
//	typedef gt::Rect<T, Scale> RectType;
	typedef T CoordType;
	typedef Scale ScaleType;

	static bool IsLeftOf(T c1, T c2)
	{
		return c1 < c2;
	}

	static bool IsLeftOrEqual(T c1, T c2)
	{
		return c1 <= c2;
	}

	static T Leftmost(T c1, T c2)
	{
		return std::min(c1, c2);
	}

	static bool IsRightOf(T c1, T c2)
	{
		return c1 > c2;
	}

	static bool IsRightOrEqual(T c1, T c2)
	{
		return c1 >= c2;
	}

	static T Rightmost(T c1, T c2)
	{
		return std::max(c1, c2);
	}

	static bool IsAbove(T c1, T c2)
	{
		return c1 > c2;
	}

	static bool IsAboveOrEqual(T c1, T c2)
	{
		return c1 >= c2;
	}

	static T Topmost(T c1, T c2)
	{
		return std::max(c1, c2);
	}

	static bool IsBelow(T c1, T c2)
	{
		return c1 < c2;
	}

	static bool IsBelowOrEqual(T c1, T c2)
	{
		return c1 <= c2;
	}

	static T Bottommost(T c1, T c2)
	{
		return std::min(c1, c2);
	}

	static SizeType Size(const PointType& tl, const PointType& br)
	{
		return SizeType(br.X() - tl.X(), tl.Y() - br.Y());
	}
};

//-----------------------------------------------------------------------------
// class Rect
//-----------------------------------------------------------------------------

template < 	class T, class Scale  = double,
			class coord  = OriginTopLeft< T, Scale > >
class Rect
// **CodeWizzard** - Severe Violation:  Universal Coding Standards item 28  - Do not overload functions within a template clas
{
public:
	typedef gt::Size<T, Scale> SizeType;
	typedef gt::Point<T, Scale> PointType;
	typedef gt::Rect<T, Scale> RectType;
	typedef T CoordType;
	typedef Scale ScaleType;

private:
	Point<T, Scale> tl;
	gt::Size<T, Scale> wh;

public:
	Rect() : wh(0, 0) {}
	template <class U, class UScale>
	Rect(const gt::Rect<U, UScale>& r) : tl(r.TL()), wh(static_cast<T>(r.Width()), static_cast<T>(r.Height())) {}
	Rect(const PointType& inTL, const PointType& inBR) : tl(inTL), wh(inBR.X() - inTL.X(), inBR.Y() - inTL.Y()) {}
	Rect(const PointType& inTL, const SizeType& inSize): tl(inTL), wh(inSize) {}
	Rect(const T& left, const T& top, const T& right, const T& bottom) : tl(left, top), wh(right - left, bottom - top) {}

// **CodeWizzard** - Violation:  More Effective C++ item 24  - Understand the costs of virtual functions ...
	virtual ~Rect() {}

	gt::Rect<T>& operator=(const gt::Rect<T>& arg)
	{
		if (&arg == this) return *this;
		tl = arg.tl;
		wh = arg.wh;
		return *this;
	}
	// Accessors
	void Set(const T& left, const T& top, const T& right, const T& bottom)
				{ tl.SetX(left); tl.SetY(top); wh.SetX(right - left); wh.SetY(bottom - top); }
	T Width() const							{ return (wh.X());					}
	void SetWidth(const T& a)				{ wh.SetX(a);  FixRectSize();		}
	T Height() const						{ return (wh.Y());					}
	void SetHeight(const T& a)				{ wh.SetY(a);  FixRectSize();		}

	T Left() const							{ return tl.X();					}
	void SetLeft(const T& a)				{ CoordType temp = Right(); tl.SetX(a); SetRight(temp); FixRectSize();		}

	T Top() const							{ return tl.Y();					}
	void SetTop(const T& a)					{ CoordType temp = Bottom(); tl.SetY(a); SetBottom(temp); FixRectSize();	}

	T Right() const							{ return CoordType(tl.X() + wh.X());	}
	void SetRight(const T& a)				{ wh.SetX(a - tl.X()); FixRectSize();	}

	T Bottom() const						{ return CoordType(tl.Y() + wh.Y());	}
	void SetBottom(const T& a)				{ wh.SetY(a - tl.Y()); FixRectSize();	}

	const PointType& TL() const				{ return tl;												}
	void SetTL(const PointType& pt)		{ PointType br = BR(); tl = pt; SetBR(br); FixRectSize();	}

	PointType BR() const					{ return PointType(tl.X() + wh.X(), tl.Y() + wh.Y());					}
	void SetBR(const PointType& pt)			{ wh.SetX(pt.X() - tl.X()); wh.SetY(pt.Y() - tl.Y()); FixRectSize();	}

	PointType TR() const					{ return PointType(tl.X() + wh.X(), tl.Y());	}
	void SetTR(const PointType& pt)
	{
		wh.SetX(pt.X() - tl.X());
		tl.SetY(pt.Y());
		FixRectSize();
	}

	PointType BL() const					{ return PointType(tl.X(), tl.Y() + wh.Y());	}
	void SetBL(const PointType& pt)
	{
		tl.SetX(pt.X());
		wh.SetY(pt.Y() - tl.Y());
		FixRectSize();
	}

	PointType Center() const				{ return PointType(tl.X() + (wh.X()/CoordType(2)), tl.Y() + (wh.Y()/CoordType(2)));			}
	void SetCenter(const PointType& pt)
	{
		tl = pt - PointType(Size()/Scale(2));
	}

	void CenterRectInRect(const RectType& arg)
	{
		SetCenter(arg.Center());
	}

	const SizeType &Size() const			{ return wh; }
	void SetSize(const SizeType& sz)
	{
		wh = sz;
		FixRectSize();
	}

	const PointType	&GetPosition() const	{ return tl; }
	void SetPosition(const PointType& pt)
	{
		tl = pt;
	}

	void SetPosition(const T& x, const T& y)
	{
		tl.SetX(x);
		tl.SetY(y);
	}

	SizeType Radius() const					{ return Size()/Scale(2);			}
	void SetRadius(const SizeType& sz)		{ SetSize(sz*Scale(2));				}

	bool IsEmpty() const
	{
		return(wh.X() <= CoordType(0) || wh.Y() <= CoordType(0));
	}

	bool IsZero() const
	{
		return(tl.IsZero() && wh.IsZero());
	}

	// Note; this used to be "FixEmptyRect()", but it didn't do that.
	// ie; a call to IsEmpty() would return true after a call to FixEmptyRect().
	// it does ensure that the Right >= Left, and Bottom >= Top, so I changed
	// the name to "FixRectSize()" instead.
	// -sds 2/6/02
	void FixRectSize()
	{
		if (wh.X() < CoordType(0)) wh.SetX(CoordType(0));
 		if (wh.Y() < CoordType(0)) wh.SetY(CoordType(0));
	}

	bool IsIn(const PointType& p)
	{
		T	temp = p.X() - tl.X();
		if (temp < CoordType(0) || temp >= wh.X()) return false;
		temp = p.Y() - tl.Y();
		if (temp < CoordType(0) || temp >= wh.Y()) return false;
		return true;
	}

	bool IsIn(const RectType& p)
	{
		return IsIn(p.TL()) && IsIn(p.BR());
	}

	void Inset(CoordType dX, CoordType dY)
	{
		tl.SetX(tl.X() + dX);
		tl.SetY(tl.Y() + dY);
		wh.SetX(wh.X() - dX - dX);	// width -= dX * 2
		wh.SetY(wh.Y() - dY - dY);	// height -= dY * 2;
		FixRectSize();
	}

	void Inset(CoordType delta)
	{
		Inset(delta, delta);
	}

	void Inset(const PointType& delta)
	{
		Inset(delta.X(), delta.Y());
	}

	void Inset(const SizeType& delta)
	{
		Inset(delta.X(), delta.Y());
	}

	const RectType& SetMinimumSize(const SizeType& minSz)
	{
		if (wh.X() < minSz.X()) {
			wh.SetX(minSz.X());
		}
		if (wh.Y() < minSz.Y())	{
			wh.SetY(minSz.Y());
		}
		return *this;
	}

	void Offset(CoordType dX, CoordType dY)
	{
		tl.SetX(tl.X() + dX);
		tl.SetY(tl.Y() + dY);
	}

	void Offset(PointType const &rOffset)
	{
		tl += rOffset;
	}

	void NegativeOffset(PointType const &rOffset)
	{
		tl -= rOffset;
	}

	bool Intersects(RectType const &rOther) const
	{
		PointType	otherBR = rOther.BR();			// here 'cause this isn't a native operation
		if (tl.X() >= otherBR.X()) return(false);
		if (tl.Y() >= otherBR.Y()) return(false);
		PointType	br = BR();						// here 'cause this isn't a native operation
		if (br.X() < rOther.TL().X()) return(false);
		if (br.Y() < rOther.TL().Y()) return(false);
		return(true);
	}

	// Binary operations with Rect

	bool operator==(const RectType& arg) const
	{
		if (tl != arg.tl) return false;
		if (wh != arg.wh) return false;
		return true;
	}

	bool operator!=(const RectType& arg) const	{ return !((*this) == arg);		}

/*
	// "and"ing and "or"ing rectangles is alot less
	// clear than Union and Intersect, so I've changed them...
	// -sds 4/8/02
*/

	const RectType& Union(RectType const &rArg)
	{
		// Compute the union of two rects.
		if (rArg.IsEmpty()) {
			// arg is empty.  Union is just *this.
		} else if (this->IsEmpty()) {
			// *this is empty.  Union is just arg.
			*this = rArg;
		} else {
			RectType temp(*this);
			SetLeft(coord::Leftmost(temp.Left(), rArg.Left()));
			SetTop(coord::Topmost(temp.Top(), rArg.Top()));
			SetRight(coord::Rightmost(temp.Right(), rArg.Right()));
			SetBottom(coord::Bottommost(temp.Bottom(), rArg.Bottom()));
		}
		return *this;
	}

	const RectType& Intersect(RectType const &rArg)
	{
		// Compute the intersection of two rects.
		if (rArg.IsEmpty()) {
			// arg is empty.  Intersection is therefore empty.
			*this = rArg;
		} else if (this->IsEmpty()) {
			// *this is empty.  Intersection is therefore *this.
		} else {
			RectType temp(*this);
			SetLeft(coord::Rightmost(temp.Left(), rArg.Left()));
			SetTop(coord::Bottommost(temp.Top(), rArg.Top()));
			SetRight(coord::Leftmost(temp.Right(), rArg.Right()));
			SetBottom(coord::Topmost(temp.Bottom(), rArg.Bottom()));
		}
		FixRectSize();
		return *this;
	}

 /*
	// depreciated in favor of the clearer
	// Offset() methods. -sds 2/7/02

	// Binary operations with Point
*/

	// Binary operations with Size

	const RectType& operator*=(const SizeType& arg)
	{
		tl *= arg;
		wh *= arg;
		return *this;
	}
	
	const RectType& operator*=(const Scale scale)
	{
		tl *= scale;
		wh *= scale;
		return *this;
	}
	
	const RectType& operator/=(const Scale scale)
	{
		tl /= scale;
		wh /= scale;
		return *this;
	}

};

template <class T, class Scale>
gt::Rect<T,Scale> operator+(const gt::Point<T,Scale>& a, const gt::Rect<T,Scale>& b)
{
	Rect<T,Scale> temp(b);
	temp += a;
	return temp;
}

template <class T, class Scale>
gt::Rect<T,Scale> operator-(const gt::Point<T,Scale>& a, const gt::Rect<T,Scale>& b)
{
	Rect<T,Scale> temp(b);
	temp -= a;
	return temp;
}

template <class T, class Scale>
gt::Size<T,Scale> operator-(const gt::Size<T,Scale>& a, const gt::Rect<T,Scale>& b)
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
{
	Rect<T,Scale> temp(b);
	temp += a;
	return temp;
}

template <class T, class Scale>
gt::Size<T,Scale> operator-(const gt::Size<T,Scale>& a, const gt::Size<T,Scale>& b)
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
{
	gt::Size<T,Scale> temp;
	temp.SetX(a.X() - b.X());
	temp.SetY(a.Y() - b.Y());
	return temp;
}

template <class T, class Scale>
gt::Rect<T,Scale> operator*(const gt::Size<T,Scale>& a, const gt::Rect<T,Scale>& b)
{
	gt::Rect<T,Scale> temp(b);
	temp *= a;
	return temp;
}

template <class T, class Scale>
gt::Rect<T,Scale> operator*(const Scale a, const gt::Rect<T,Scale>& b)
{
	gt::Rect<T,Scale> temp(b);
	temp *= a;
	return temp;
}

template <class T, class Scale>
gt::Rect<T,Scale> operator/(const gt::Rect<T,Scale>& a, const Scale b)
{
	gt::Rect<T,Scale> temp(a);
	temp /= b;
	return temp;
}

#if defined(_WIN32)
#pragma mark -
#pragma mark Class Arc
#pragma mark -
#endif

//-----------------------------------------------------------------------------
// class Arc
//-----------------------------------------------------------------------------

template <class T, class Scale  = double >
class Arc
// **CodeWizzard** - Severe Violation:  Universal Coding Standards item 28  - Do not overload functions within a template clas
{
public:
	typedef gt::Size<T, Scale> SizeType;
	typedef gt::Point<T, Scale> PointType;
	typedef gt::Arc<T, Scale> ArcType;
	typedef T CoordType;
	typedef Scale ScaleType;

private:
	Point<T, Scale> xy;
	gt::Size<T, Scale> wh;
	T s;
	T e;

public:
	Arc() : xy(0, 0), wh(0, 0), s(T(0)), e(T(0)) {}
	template <class U, class UScale>
	Arc(const gt::Arc<U, UScale>& r) : xy(r.XY()), wh(static_cast<T>(r.Width()), static_cast<T>(r.Height())), s(static_cast<T>(r.StartAngle())), e(static_cast<T>(r.EndAngle())) {}
	Arc(const PointType& inXY, const SizeType& inSize, const T& inS, const T& inE): xy(inXY), wh(inSize), s(inS), e(inE) {}
	Arc(const T& inX, const T& inY, const T& inW, const T& inH, const T& inS, const T& inE) : xy(inX, inY), wh(inW, inH), s(inS), e(inE) {}

// **CodeWizzard** - Violation:  More Effective C++ item 24  - Understand the costs of virtual functions ...
	virtual ~Arc() {}

	gt::Arc<T>& operator=(const gt::Arc<T>& arg)
	{
		if (&arg == this) return *this;
		xy = arg.xy;
		wh = arg.wh;
		s = arg.s;
		e = arg.e;
		return *this;
	}
	// Accessors
	void Set(const T& inX, const T& inY, const T& inW, const T& inH, const T& inS, const T& inE)
				{ xy.SetX(inX); xy.SetY(inY); wh.SetX(inW); wh.SetY(inH); s = inS; e = inE; }
	T Width() const							{ return (wh.X());					}
	void SetWidth(const T& a)				{ wh.SetX(a);						}
	T Height() const						{ return (wh.Y());					}
	void SetHeight(const T& a)				{ wh.SetY(a);						}

	T X() const								{ return (xy.X());					}
	void SetX(const T& a)					{ CoordType temp = Width(); xy.SetX(a); SetWidth(temp); FixArcSize();	}

	T Y() const								{ return (xy.Y());					}
	void SetY(const T& a)					{ CoordType temp = Height(); xy.SetY(a); SetHeight(temp); FixArcSize();	}

	T StartAngle() const					{ return s;							}
	void SetStartAngle(const T& a)			{ s = a; FixArcSize();				}

	T EndAngle() const						{ return e;							}
	void SetEndAngle(const T& a)			{ e = a; FixArcSize();				}

	const PointType& XY() const				{ return xy;						}
	void SetXY(const PointType& pt)			{ xy = pt;							}

	PointType Center() const				{ return PointType(xy);				}
	void SetCenter(const PointType& pt)		{ SetXY(pt);						}

	void CenterArcInArc(const ArcType& arg)
	{
		SetCenter(arg.Center());
	}

	const SizeType &Size() const			{ return wh;						}
	void SetSize(const SizeType& sz)
	{
		wh = sz;
		FixArcSize();
	}

	const PointType	&GetPosition() const	{ return xy; }
	void SetPosition(const PointType& pt)	{ xy = pt;	}

	void SetPosition(const T& x, const T& y)
	{
		xy.SetX(x);
		xy.SetY(y);
	}

	SizeType Radius() const					{ return Size()/Scale(2);			}
	void SetRadius(const SizeType& sz)		{ SetSize(sz*Scale(2));				}

	bool IsEmpty() const
	{
		return(wh.X() <= CoordType(0) || wh.Y() <= CoordType(0) || (s == e));
	}

	bool IsZero() const
	{
		return(wh.IsZero() || (s == e));
	}

	void FixArcSize()
	{
		if (wh.X() < CoordType(0)) wh.SetX(CoordType(0));
 		if (wh.Y() < CoordType(0)) wh.SetY(CoordType(0));
		if (e < s) e += 360;
	}

/*
dpy--> not sure how to exactly handle angles

	bool IsIn(const PointType& p)
	{
		T	temp = p.X() - xy.X();
		if (temp < CoordType(0) || temp >= wh.X()) return false;
		temp = p.Y() - xy.Y();
		if (temp < CoordType(0) || temp >= wh.Y()) return false;
		return true;
	}

	bool IsIn(const ArcType& p)
	{
		return IsIn(p.XY()) && IsIn(p.BR());
	}

	void Inset(CoordType dX, CoordType dY)
	{
		xy.SetX(xy.X() + dX);
		xy.SetY(xy.Y() + dY);
		wh.SetX(wh.X() - dX - dX);	// width -= dX * 2
		wh.SetY(wh.Y() - dY - dY);	// height -= dY * 2;
		FixArcSize();
	}

	void Inset(CoordType delta)
	{
		Inset(delta, delta);
	}

	void Inset(const PointType& delta)
	{
		Inset(delta.X(), delta.Y());
	}

	void Inset(const SizeType& delta)
	{
		Inset(delta.X(), delta.Y());
	}
*/

	const ArcType& SetMinimumSize(const SizeType& minSz)
	{
		if (wh.X() < minSz.X()) {
			wh.SetX(minSz.X());
		}
		if (wh.Y() < minSz.Y())	{
			wh.SetY(minSz.Y());
		}
		return *this;
	}

	void Offset(CoordType dX, CoordType dY)
	{
		xy.SetX(xy.X() + dX);
		xy.SetY(xy.Y() + dY);
	}

	void Offset(PointType const &rOffset)
	{
		xy += rOffset;
	}

	void NegativeOffset(PointType const &rOffset)
	{
		xy -= rOffset;
	}

/*
	bool Intersects(RectType const &rOther) const
	{
		PointType	otherBR = rOther.BR();			// here 'cause this isn't a native operation
		if (xy.X() >= otherBR.X()) return(false);
		if (xy.Y() >= otherBR.Y()) return(false);
		PointType	br = BR();						// here 'cause this isn't a native operation
		if (br.X() < rOther.XY().X()) return(false);
		if (br.Y() < rOther.XY().Y()) return(false);
		return(true);
	}
*/
	// Binary operations with Rect

	bool operator==(const ArcType& arg) const
	{
		if (xy != arg.xy) return false;
		if (wh != arg.wh) return false;
		if (s != arg.s) return false;
		if (e != arg.e) return false;
		return true;
	}

	bool operator!=(const ArcType& arg) const	{ return !((*this) == arg);		}

/*
	// "and"ing and "or"ing rectangles is alot less
	// clear than Union and Intersect, so I've changed them...
	// -sds 4/8/02
*/
/*
	const ArcType& Union(ArcType const &rArg)
	{
		// Compute the union of two rects.
		if (rArg.IsEmpty()) {
			// arg is empty.  Union is just *this.
		} else if (this->IsEmpty()) {
			// *this is empty.  Union is just arg.
			*this = rArg;
		} else {
			ArcType temp(*this);
			SetLeft(coord::Leftmost(temp.Left(), rArg.Left()));
			SetTop(coord::Topmost(temp.Top(), rArg.Top()));
			SetRight(coord::Rightmost(temp.Right(), rArg.Right()));
			SetBottom(coord::Bottommost(temp.Bottom(), rArg.Bottom()));
		}
		return *this;
	}

	const ArcType& Intersect(ArcType const &rArg)
	{
		// Compute the intersection of two rects.
		if (rArg.IsEmpty()) {
			// arg is empty.  Intersection is therefore empty.
			*this = rArg;
		} else if (this->IsEmpty()) {
			// *this is empty.  Intersection is therefore *this.
		} else {
			ArcType temp(*this);
			SetLeft(coord::Rightmost(temp.Left(), rArg.Left()));
			SetTop(coord::Bottommost(temp.Top(), rArg.Top()));
			SetRight(coord::Leftmost(temp.Right(), rArg.Right()));
			SetBottom(coord::Topmost(temp.Bottom(), rArg.Bottom()));
		}
		FixArcSize();
		return *this;
	}
*/

 /*
	// depreciated in favor of the clearer
	// Offset() methods. -sds 2/7/02

	// Binary operations with Point
*/

	// Binary operations with Size

	const ArcType& operator*=(const SizeType& arg)
	{
		xy *= arg;
		wh *= arg;

		// Not clear on how to handle the angles
		// s *= arg.X();
		// e *= arg.Y();
		return *this;
	}
	
	const ArcType& operator*=(const Scale scale)
	{
		xy *= scale;
		wh *= scale;

		// Not clear on how to handle the angles
		// s *= scale;
		// e *= scale;
		return *this;
	}
	
	const ArcType& operator/=(const Scale scale)
	{
		xy /= scale;
		wh /= scale;

		// Not clear on how to handle the angles
		// s /= scale;
		// e /= scale;
		return *this;
	}

};

template <class T, class Scale>
gt::Arc<T,Scale> operator+(const gt::Point<T,Scale>& a, const gt::Arc<T,Scale>& b)
{
	Arc<T,Scale> temp(b);
	temp += a;
	return temp;
}

template <class T, class Scale>
gt::Arc<T,Scale> operator-(const gt::Point<T,Scale>& a, const gt::Arc<T,Scale>& b)
{
	Arc<T,Scale> temp(b);
	temp -= a;
	return temp;
}

template <class T, class Scale>
gt::Size<T,Scale> operator-(const gt::Size<T,Scale>& a, const gt::Arc<T,Scale>& b)
// **CodeWizzard** - Violation:  More Effective C++ item 2 - Consider using op= instead of stand-alone o
{
	Arc<T,Scale> temp(b);
	temp += a;
	return temp;
}

template <class T, class Scale>
gt::Arc<T,Scale> operator*(const gt::Size<T,Scale>& a, const gt::Arc<T,Scale>& b)
{
	gt::Arc<T,Scale> temp(b);
	temp *= a;
	return temp;
}

template <class T, class Scale>
gt::Arc<T,Scale> operator*(const Scale a, const gt::Arc<T,Scale>& b)
{
	gt::Arc<T,Scale> temp(b);
	temp *= a;
	return temp;
}

template <class T, class Scale>
gt::Arc<T,Scale> operator/(const gt::Arc<T,Scale>& a, const Scale b)
{
	gt::Arc<T,Scale> temp(a);
	temp /= b;
	return temp;
}


}
#endif



