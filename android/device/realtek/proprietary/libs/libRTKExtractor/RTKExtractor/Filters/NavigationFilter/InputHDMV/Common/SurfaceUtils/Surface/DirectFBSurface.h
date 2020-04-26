//-----------------------------------------------------------------------------
// DirectFBSurface.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef DirectFB_SURFACE_H
#define DirectFB_SURFACE_H

#if defined (_DIRECTFB)
#include "IDFB.h"
#include "BaseSurface.h"
#include "SysSurfaceCommon.h"
#include "IDFBSurface.h"
#include "ref_ptr.h"

struct DirectFBImageStruct;

namespace SonicSurface 
{

//_____________________________________________________________________________

class DirectFBSurface : public BaseSurface
{
	typedef BaseSurface INHERITED;

public:
	DirectFBSurface();
	DirectFBSurface(SInt32 width, SInt32 height, SInt32 colorDepth, 
						void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New);
	DirectFBSurface(SInt32 width, SInt32 height, SurfaceFormat surfFormat, 
						void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New);

	explicit DirectFBSurface(ref_ptr<IDFBSurface> pSurface);

	virtual ~DirectFBSurface();

	virtual void	Initialize(SInt32 width, SInt32 height, SInt32 colorDepth, void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New)
		{INHERITED::Initialize(width, abs(height), colorDepth, pBits, abs(stride), use); }
	virtual void	Initialize(SInt32 width, SInt32 height, SurfaceFormat surfFormat, void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New)
		{INHERITED::Initialize(width, abs(height), surfFormat, pBits, abs(stride), use); }
#if 0
	//assign & copy constructor. Be sure to properly use!
	DirectFBSurface(DirectFBSurface const &rhs);
	DirectFBSurface& operator=(DirectFBSurface const &rhs);
#endif
	//	From BaseSurface
	//virtual bool	WriteToFile(FileSpec const &fs) const;
	virtual void	Clear(const PiRGBColor &color);
	virtual void	Clear() {INHERITED::Clear();}
	virtual void	Clear(UInt8 idx) {INHERITED::Clear(idx);}
	virtual void	SetClippingRect(const PiRect& r);
	virtual void	ResetClippingRect();

	ref_ptr<IDFBSurface>	GetSurfacePtr() const;

    // palettee
    void	SetPalette(const ColorPalette& palette, bool remapPixels);

	// queries
	virtual bool	IsAlphaPremultiplied() const;

	// get at surface data
	virtual void  LockPixels() const;
    virtual void  UnlockPixels() const;
	virtual bool  IsLockable() const { return true; }

	bool WriteToFile(char* pPrefixStr = 0) const;
		
protected:
	virtual void	ResetSurface();

	void InitBaseFromDFBSurface(ref_ptr<IDFBSurface>& pSurface);

	virtual void	SetSurfaceClipRect(const PiRect &r);

private:
	ref_ptr<IDFBSurface> m_pSurface;
	SurfaceUse sfUse;
};

}


#endif	//(_DIRECTFB)

#endif	//DirectFB_SURFACE_H

