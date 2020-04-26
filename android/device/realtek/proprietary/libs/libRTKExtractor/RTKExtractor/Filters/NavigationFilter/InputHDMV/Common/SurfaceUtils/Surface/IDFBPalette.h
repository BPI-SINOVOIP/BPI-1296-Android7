#ifndef IDFBPALETTE_H
#define IDFBPALETTE_H

#include "CommonTypes.h"
#include "IDFBBase.h"

class IDFBPalette : public IDFBBase<IDirectFBPalette> 
{
	friend class IDFBSurface;

public:
	IDFBPalette(IDirectFBPalette* dfbif = 0) : IDFBBase <IDirectFBPalette> (dfbif) {}

	DFBResult	GetCapabilities(DFBPaletteCapabilities &caps);
	DFBResult	GetSize(unsigned int &size);
    DFBResult	SetEntries(const DFBColor &entries, unsigned int numEntries, unsigned int offset);
	DFBResult	GetEntries(DFBColor &entries, unsigned int numEntries, unsigned int offset);
	DFBResult	FindBestMatch(UInt8 r, UInt8 g, UInt8 b, UInt8 a, unsigned int &index);
	DFBResult	CreateCopy(IDFBPalette *&palette);
};

#endif
