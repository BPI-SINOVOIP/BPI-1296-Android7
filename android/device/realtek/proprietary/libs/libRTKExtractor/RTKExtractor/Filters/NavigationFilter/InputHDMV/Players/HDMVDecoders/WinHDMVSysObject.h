//-----------------------------------------------------------------------------
// WinHDMVDecoder_DecodedDataBuffer.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINHDMV_SYS_OBJECT_H__
#define __WINHDMV_SYS_OBJECT_H__

#include "HDMVDecoder_DecodedDataBuffer.h"

//__________________________________________________________________________________________________________________________________________________
// WinHDMVSysObject
// Container for Object data description, Decoded object data, its Spite and its Surface
//
class WinHDMVSysObject : public HDMVSysObject
{
public:
	WinHDMVSysObject()			{}
	~WinHDMVSysObject()			{}

public:
	virtual bool IsValid(void)														{ return (m_pSurface.get() != 0); }

	SonicError CacheHDMVObject(UInt32 uiObjectId);

	void SetPalette(UInt16 uiObjId, UInt8 uiObjVersion, UInt8 uiPaletteId, UInt8 uiPaletteVersion, SInt32 siPaletteIndex, ref_ptr<SonicSurface::ColorPalette> pPalette);

	void SetSrcRect(UInt32 left, UInt32 top, UInt32 right, UInt32 bottom)	{ m_srcRect = PiRect(left, top, right, bottom); }

protected:
	virtual void RendererPrepare()							{ }
	virtual void RendererCommit()							{ }
};


#endif // __WINHDMV_SYS_OBJECT_H__
