//-----------------------------------------------------------------------------
// DirectFBText.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef DirectFBText_H
#define DirectFBText_H

#if defined (_DIRECTFB)

#include "BaseText.h"
//#include "SysSurfaceCommon.h"
//#include "IDFBFont.h"
//#include "IDFB.h"

#define DEFAULT_DFB_FONT_SIZE 12

//#include <iostream>
//#include <map>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <algorithm>

//#include "ref_ptr.h"
//#include "UnicodeUtilities.h"


namespace SonicSurface
{
//Map

/*FontMap::iterator		m_Pos;*/
#ifndef REALTEK_MODIFY
class DirectFBFontImplementation : public PsImplementation {
	public:
		explicit DirectFBFontImplementation(PiFont *pFont);
		explicit DirectFBFontImplementation(IDFBFont *pFont);
		virtual ~DirectFBFontImplementation();
		virtual PsImplementation* Clone() const;

		IDFBFont	*CloneFont() const;

	private:
		DirectFBFontImplementation& operator=(const DirectFBFontImplementation& /*rRhs*/);
		DirectFBFontImplementation(DirectFBFontImplementation const &/*rhs*/);

		safe_ptr<IDFBFont> mpFont;
};



void GetTextScaling(SurfaceAspectRatio aspectRatio, PiFPoint& textScaling);
#endif

class DirectFBTextData : public BaseTextData
{
public:
	DirectFBTextData();
	virtual ~DirectFBTextData();

	DirectFBTextData& operator=(const DirectFBTextData& rRhs) {
						if (&rRhs != this) {
							BaseTextData::operator=(rRhs);	// do the base class' assignment
							bEditing = rRhs.bEditing;		// and then ours
						}
						return(*this);
					}
	bool	operator==( DirectFBTextData const & rRhs ) const;

	DirectFBTextData(DirectFBTextData const &/*rhs*/);
	bool		IsEditing() const {return bEditing;}

private:
	bool		bEditing;	// If true, sets compositing quality to high speed rather than high quality
};

#ifndef REALTEK_MODIFY
class DirectFBTextRenderer : public BaseTextRenderer
{
public:
	DirectFBTextRenderer();
	virtual ~DirectFBTextRenderer();

	virtual void DrawText(SysSurface& dstSurface, SysTextData& textData, PiPoint *pOffset = 0);

protected:


private:
//	void DrawTextNoShadows(SysSurface& dstSurface, const SysTextData& textData);

	// assign & copy constructor. Be sure to properly use!
	DirectFBTextRenderer(DirectFBTextRenderer const &/*rhs*/);
	DirectFBTextRenderer& operator=(DirectFBTextRenderer const & /*rhs*/);

};

class DirectFBFontCollection : public BaseFontCollection
{
	friend class DirectFBFontImplementation;

private:
	ref_ptr<IDFB> mDFB;
protected:
	DirectFBFontCollection(ref_ptr<IDFB> DFB);
public:
	virtual ~DirectFBFontCollection();

	static DirectFBFontCollection* CreateFontCollection(ref_ptr<IDFB> DFB);
	static DirectFBFontCollection* GetFontCollection() { return dynamic_cast<DirectFBFontCollection*>(BaseFontCollection::GetFontCollection()); }

	ref_ptr<IDFB> GetDFB() const { return mDFB; }
};
#endif

}

#endif	//(_DIRECTFB)

#endif	//DirectFBText_H
