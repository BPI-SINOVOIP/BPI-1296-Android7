//-----------------------------------------------------------------------------
// BaseText.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef _BaseText_H
#define _BaseText_H

#include "CommonTypes.h"
#include "PiRect.h"
#include "ref_ptr.h"
//#include "SysPrimitives.h"
//#include "SysEffects.h"
//#include "SysTextForward.h"
//#include "SysSurfaceForward.h"
#include "UnicodeUtilities.h"
#include "TextDefs.h"

#include "PiRGBColor.h"
namespace SonicSurface 
{

//_____________________________________________________________________________
//
// Utils
//
//_____________________________________________________________________________



//_____________________________________________________________________________
//
// Classes
//
//_____________________________________________________________________________

class PiFont //: public PiPrimitive 
{
	public:
		PiFont(unicode::string8 const &rFaceName, SInt32 fontSize, eTextStyle fontStyle, UInt32 scriptID)
			: //PiPrimitive(),
				mScript(scriptID), mName(rFaceName), mSize(fontSize), mHorzSizeRatio(1.0), mStyle(fontStyle), mHeight(0)
		{}
		// OK, need this for old-style calls that don't provide a unicode::string8... -sds 2/7/02
		PiFont(std::string const &rFaceName, SInt32 fontSize, eTextStyle fontStyle, UInt32 scriptID)
			: //PiPrimitive(),
				mScript(scriptID), mName(unicode::to_string8(rFaceName)), mSize(fontSize), mHorzSizeRatio(1.0), mStyle(fontStyle), mHeight(0)
		{}
		PiFont(PiFont const &rOther)
			: //PiPrimitive(rOther),
				mScript(rOther.mScript), mName(rOther.mName), mSize(rOther.mSize), mHorzSizeRatio(rOther.mHorzSizeRatio), mStyle(rOther.mStyle), mHeight(rOther.mHeight)
		{}
		virtual ~PiFont() {}

	// accessors
		unicode::string8 const &GetFace() const
							{ return(mName); }
		UInt32		GetSize() const
						{ return(mSize); }
		UInt32		GetHorzSize() const
						{ return (SInt32)(mSize*mHorzSizeRatio); }
		UInt32		GetStyle() const
						{ return(eTextStyle(mStyle)); }
		UInt32		GetScript() const
						{ return(mScript); }

	// mutators
		void	SetFace(unicode::string8 const &rName)
						{ mName = rName; /*InvalidateImplementation(); */}
		void	SetSize(UInt32 size)
						{ mSize = size; /*InvalidateImplementation(); */}
		void	SetHorzSizeRatio(double ratio)
						{ mHorzSizeRatio = ratio; /*InvalidateImplementation(); */}
		void	SetStyle(UInt32 style)
						{ mStyle = style; /*InvalidateImplementation(); */}
		void	SetScript(UInt32 script)
						{ mScript = script; /*InvalidateImplementation(); */}
	// Utility functions
		bool	GetFontMetrics(int& rAscent, int& rMaxAscent, int& rDescent, int& rMaxDescent, int& rLeading, int& rHeight);
		void	MeasureText(unicode::string8 const &rText, PiRect &rOutBounds, double xScale = 1.0, double yScale = 1.0, const PiRect *pInTestBounds = 0);

	// operator overloads
		PiFont const &operator=(PiFont const &rOther)
						{	if (this == &rOther) return(*this);	// avoid self-assignment
							//PiPrimitive::operator=(rOther);
							mScript = rOther.mScript;
							mName = rOther.mName;
							mSize = rOther.mSize;
							mHorzSizeRatio = rOther.mHorzSizeRatio;
							mStyle = rOther.mStyle;
							mHeight = rOther.mHeight;
							/*InvalidateImplementation();*/
							return(*this);
						}

		bool operator==( PiFont const& rRhs ) const;

	protected:
		UInt32				mScript;	// script ID of the font
		unicode::string8	mName;		// name of the font family, ie; "Ariel"
		UInt32				mSize;		// size, in points
		double				mHorzSizeRatio; // ratio of horz font size to primary font size
		UInt32				mStyle;		// style, from the enum above, or bitwise 'OR' of the above styles

		float				mHeight;	// "height" of font.
};

class BaseTextData
// **CodeWizzard** - Violation:  Effective C++ item 20  - Avoid data members in the public interfac
// **CodeWizzard** - Violation:  Universal Coding Standards item 16  - Do not specify public data member
{
public:
	BaseTextData();
	virtual ~BaseTextData();

	BaseTextData& operator=(BaseTextData const &rRhs);
	BaseTextData(BaseTextData const &rhs);
	bool operator==(BaseTextData const&rhs) const;

//private:
public:
	// ***** Appearance *****
	unicode::string8			mText;

	PiFont						mFont;	
	PiRGBColor					mColor;

	//ref_ptr<PiSolidBrush>		mpHilightBrush;		// optional hilight brush
	SInt32						mnStartHilight;		// hilighting start
	SInt32						mnEndHilight;		// hilighting end;
	bool						mbHilight;			// do we hilight the given range.

	eTextAlignment				mTextAlignment;
    eTextVerticalAlignment      mTextVerticalAlignment;
	eTextDirection				mTextDirection;
	double						mVerticalLineSpacing;	// multiplier of font point size, 1.05 is default

	PiFPoint					mTextScale;

//	SysShadowParams				mShadowParams;
	bool						mbIsShadowEnabled;

    float                       mOutlineWidth;
    PiRGBColor                  mOutlineColor;

    bool                        mbAntialiased;
	bool						mbKerned;

	bool						mbAppearanceDirty;	// appearance information has changed
    // *****

	// ***** Position *****
	PiRect						mTextBounds;		// original text bounds
	mutable PiRect				mUpdateBounds;		// full bounds -- including shadowing area if applicable

	bool						mbPositionDirty;	// position information has changed
	// *****

};
#ifndef REALTEK_MODIFY
class BaseTextRenderer
{
public:
	BaseTextRenderer();
	virtual ~BaseTextRenderer();

	virtual void DrawText(SysSurface& dstSurface, SysTextData& textData, PiPoint *pOffset = 0) = 0;

protected:


private:
	// assign & copy constructor. Be sure to properly use!
	BaseTextRenderer(BaseTextRenderer const &/*rhs*/);
	BaseTextRenderer& operator=(BaseTextRenderer const & /*rhs*/);

};

class BaseFontCollection
{
private:
	static BaseFontCollection* mBaseFontCollection;
protected:
	BaseFontCollection();
public:
	virtual ~BaseFontCollection();

protected:
	static BaseFontCollection* CreateFontCollection();
	static BaseFontCollection* GetFontCollection() { return mBaseFontCollection; }

public:
	typedef enum
	{
		eMatchedNone = 0,
		eMatchedFamily = 0x01,
		eMatchedStyle = 0x02,
		eMatchedSize = 0x04,
		eMatchedAll = eMatchedFamily | eMatchedStyle | eMatchedSize,
	} eLookupResult;

protected:
	std::vector<unicode::string8> mFonts;
	UInt32 mCurrentMemoryFontTotalSize;
	UInt32 mMaxMemoryFontTotalSize;
	static bool GetFontMemory(const unicode::string8& inFontBuffer, UInt8** outBuffer, UInt32* outBufferSize);
public:
	virtual bool AddFont(const unicode::string8& inFileName, const unicode::string8& inFamilyName, UInt32 inStyle = (UInt32) -1, int inMinSize = -1, int inMaxSize = -1);
	virtual bool AddFont(const void* inBuffer, UInt32 inBufferSize, const unicode::string8& inFamilyName, UInt32 inStyle = (UInt32) -1, int inMinSize = -1, int inMaxSize = -1);
	virtual void ClearCollection(const unicode::string8* inFamilyName = 0);
	virtual bool DoesFamilyExist(const unicode::string8& inFamilyName) const;
	virtual bool GetFontFamilyName(UInt32 inIndex, unicode::string8& outName) const;
	virtual UInt32 GetFontCount() const;
	virtual bool GetFontInfo(UInt32 inIndex, unicode::string8& outFileName, unicode::string8& outName, UInt32& outStyle, int& outMinSize, int& outMaxSize) const;
	virtual eLookupResult LookupFontFileName(const unicode::string8& inFamilyName, UInt32 inStyle, int inSize, unicode::string8& outFileName) const;
	virtual UInt32 SetMemoryFontMaxTotalSize(UInt32 inMaxSize);
};
#endif
}

#endif	//_BaseText_H
