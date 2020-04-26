//-----------------------------------------------------------------------------
// PiRGBColor.h
// Copyright (c) 2001 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef __PiRGBColor_h__
#define __PiRGBColor_h__

#include "CommonTypes.h"
#include "SysPrimitivesCommon.h"
//#include "ByteSwapper.h"

namespace SonicSurface {

class PiYUVColor;

class PiRGBColor {
public:
	PiRGBColor() : mBlue(0), mGreen(0), mRed(0), mAlpha(0xff)   
		{}
	PiRGBColor(UInt8 inRed, UInt8 inGreen, UInt8 inBlue, UInt8 inAlpha = 0xff)
		: mBlue(inBlue), mGreen(inGreen), mRed(inRed), mAlpha(inAlpha)
		{}
	explicit PiRGBColor(SonicSurface::HexColor hexColor, UInt8 alpha = 0xff)
		: mPixel(hexColor)
		{ mAlpha = alpha; }
	explicit PiRGBColor(UInt32 const *pixel) 
		{ mPixel = *pixel; }

	~PiRGBColor() {}

	// accessors
	SonicSurface::HexColor	GetHexColor() const
					{ return static_cast<SonicSurface::HexColor>(mPixel & 0xFFFFFF); }
	UInt8		GetRed() const
					{ return(mRed); }
	UInt8		GetGreen() const
					{ return(mGreen); }
	UInt8		GetBlue() const
					{ return(mBlue); }
	UInt8		GetAlpha() const
					{ return(mAlpha); }
	void		GetRGB(UInt8 &rRed, UInt8 &rGreen, UInt8 &rBlue) const
					{ rRed = mRed; rGreen = mGreen; rBlue = mBlue; }
	void		GetRGBA(UInt8 &rRed, UInt8 &rGreen, UInt8 &rBlue, UInt8 &rAlpha) const
					{ GetRGB(rRed, rGreen, rBlue); rAlpha = mAlpha; }
	// mutators
	void		SetRed(UInt8 value)
					{ mRed = value; }
	void		SetGreen(UInt8 value)
					{ mGreen = value; }
	void		SetBlue(UInt8 value)
					{ mBlue = value; }
	void		SetAlpha(UInt8 value)
					{ mAlpha = value; }
	void		SetRGB(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha = 0xff)
					{ mRed = red; mGreen = green; mBlue = blue; mAlpha = alpha; }
	void		SetRGB(SonicSurface::HexColor hexColor, UInt8 alpha = 0xff)
					{ mPixel = hexColor; mAlpha = alpha; }


	//PiYUVColor &AsYUV(PiYUVColor &yuv) const;

	// operator overloads
	PiRGBColor const &operator=(PiRGBColor const &rOther)
					{	if (this == &rOther) return(*this);	// avoid self-assignment
						mPixel = rOther.mPixel;
						return(*this);
					}
	// allow assignment to a hexadecimal color (ie; PiRGBColor color = 0x7fff00; // yellow-green)
	PiRGBColor const &operator=(SonicSurface::HexColor const &hexColor)
					{	mPixel = hexColor;
						mAlpha = 0xff;
						return(*this);
					}

 	bool operator==(const PiRGBColor& rOther) const
 	{
 		return (mPixel == rOther.mPixel);
 	}
 
 	bool operator!=(const PiRGBColor& rOther) const
 	{
 		return !(rOther == *this);
 	}

	PiRGBColor& operator*=(UInt8 inMult)
	{
		mRed = static_cast<UInt8>(mRed * inMult);
		mGreen = static_cast<UInt8>(mGreen * inMult);
		mBlue = static_cast<UInt8>(mBlue * inMult);
		return *this;
	}
	PiRGBColor& operator/=(UInt8 inDiv)
	{
		mRed = static_cast<UInt8>(mRed / inDiv);
		mGreen = static_cast<UInt8>(mGreen / inDiv);
		mBlue = static_cast<UInt8>(mBlue / inDiv);
		return *this;
	}

	UInt32 AsUInt32() const { return mPixel; }

private:

#if defined(_WIN32)
	#pragma warning(push)
	#pragma warning(disable:4201)	// nameless struct/union
#endif

	union {
		UInt32	mPixel;		// the composite data

		struct {			// the component data...
			#if defined (Bigend__)		// channel order should match processor endianness
				UInt8	mAlpha;
				UInt8	mRed;
				UInt8	mGreen;
				UInt8	mBlue;
			#else
				UInt8	mBlue;
				UInt8	mGreen;
				UInt8	mRed;
				UInt8	mAlpha;
			#endif
		};
	};
	
#if defined(_WIN32)
	#pragma warning(pop) 
#endif
	
};

}; // namespace

#endif //__PiRGBColor_h__
