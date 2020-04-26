#ifndef IDFBFONT_H
#define IDFBFONT_H

#include "IDFBBase.h"

class IDFBFont : public IDFBBase<IDirectFBFont>
{
	friend class IDFBSurface;

public:
	IDFBFont(IDirectFBFont* dfbif = 0) : IDFBBase <IDirectFBFont> (dfbif) {}

	DFBResult	GetAscender(int &ascender);
	DFBResult	GetDescender(int &descender);
	DFBResult	GetHeight(int &height);
	DFBResult	GetMaxAdvance(int &maxAdvance);
	DFBResult	GetKerning(unsigned int prevIndex, unsigned int curIndex, int &kernX, int &kernY);
	DFBResult	GetStringWidth(const char *text, int bytes, int &width);
	DFBResult	GetStringExtents(const char *text, int bytes, DFBRectangle &logRect, DFBRectangle &inkRect);
	DFBResult	GetGlyphExtents(unsigned int index, DFBRectangle &rect, int &advance);
};

#endif	//IDFBFONT_H
