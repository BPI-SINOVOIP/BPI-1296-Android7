#ifndef IDFBSURFACE_H
#define IDFBSURFACE_H

#include "IDFBBase.h"
#include "IDFBPalette.h"
#include "IDFBFont.h"

#include "ref_ptr.h"
#include "SonicCriticalSection.h"

class IDFBSurface : public IDFBBase <IDirectFBSurface>
{
	friend class IDFBDisplayLayer;
	friend class IDFBImageProvider;
	friend class IDFBVideoProvider;
	friend class IDFBWindow;

public:
	IDFBSurface(IDirectFBSurface *dfbif = 0);
	virtual ~IDFBSurface() {if (polyPoints) free(polyPoints); }

	DFBResult	GetCapabilities(DFBSurfaceCapabilities &caps);
	DFBResult	GetClip(DFBRegion &clip);
	DFBResult	GetSize(int &width, int &height);
	DFBResult	GetVisibleRectangle(DFBRectangle &rect);
	DFBResult	GetPixelFormat(DFBSurfacePixelFormat &pixelFormat);
	DFBResult	GetAccelerationMask (IDFBSurface &source, DFBAccelerationMask &mask);
	DFBResult	GetPalette(IDFBPalette *&palette);
	DFBResult	SetPalette(IDFBPalette &palette);
	DFBResult	SetAlphaRamp(UInt8 a0, UInt8 a1, UInt8 a2, UInt8 a3);
	DFBResult	Lock(DFBSurfaceLockFlags flags, void **ptr, int *pitch);
	DFBResult	Unlock();
	DFBResult	Flip(const DFBRegion &region, DFBSurfaceFlipFlags flags);
	DFBResult	Flip(const DFBRegion *region, DFBSurfaceFlipFlags flags);
	DFBResult	SetField(int field);
	DFBResult	Clear(UInt8 r, UInt8 g, UInt8 b, UInt8 a);
	DFBResult	SetClip(const DFBRegion &clip);
	DFBResult	SetColor(UInt8 r,UInt8 g, UInt8 b, UInt8 a);
	DFBResult	SetColorIndex(unsigned int index);
	DFBResult	SetSrcBlendFunction(DFBSurfaceBlendFunction function);
	DFBResult	SetDstBlendFunction(DFBSurfaceBlendFunction function);
	DFBResult	SetPorterDuff(DFBSurfacePorterDuffRule rule);
	DFBResult	SetSrcColorKey(UInt8 r, UInt8 g, UInt8 b);
	DFBResult	SetSrcColorKeyIndex(unsigned int index);
	DFBResult	SetDstColorKey(UInt8 r, UInt8 g, UInt8 b);
	DFBResult	SetDstColorKeyIndex (unsigned int index);
	DFBResult	SetBlittingFlags(DFBSurfaceBlittingFlags flags);
	DFBResult	Blit(IDFBSurface &source, const DFBRectangle &sourceRect, int x, int y);
	DFBResult	TileBlit(IDFBSurface &source, const DFBRectangle &sourceRect, int x, int y);
	DFBResult	StretchBlit(IDFBSurface &source, DFBRectangle &sourceRect, DFBRectangle &destinationRect);
	DFBResult	StretchBlit(IDFBSurface &source, DFBRectangle *sourceRect, DFBRectangle *destinationRect);
	DFBResult	TextureTriangles(IDFBSurface &source, const DFBVertex &vertices, const int *indices, int num, DFBTriangleFormation formation);
	DFBResult	SetDrawingFlags(DFBSurfaceDrawingFlags flags);
	DFBResult	FillRectangle(int x, int y, int width, int height);
	DFBResult	DrawRectangle(int x, int y, int width, int height);
	DFBResult	DrawLine(int x1, int y1, int x2, int y2);
	DFBResult	DrawLines(const DFBRegion &lines, unsigned int numLines);
	DFBResult	FillRectangles(const DFBRectangle &rects, unsigned int numRects);
	DFBResult	FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	DFBResult	SetFont(const IDFBFont &font);
	DFBResult	GetFont(IDFBFont *&font);
	DFBResult	DrawString(const char *text, int bytes, int x, int y, DFBSurfaceTextFlags flags);
	DFBResult	DrawGlyph(unsigned int index, int x, int y, DFBSurfaceTextFlags flags);
	DFBResult	GetSubSurface(const DFBRectangle &rect, IDFBSurface *&subSurface);
	DFBResult	GetSubSurface(int x, int y,	int width, int height, IDFBSurface *&subSurface);
	DFBResult	Dump(const char *directory, const char *prefix);
	DFBResult	SetColor(const DFBColor &color);
	DFBResult	SetColor(const DFBColor *color);
	DFBResult	FillRectangle(const DFBRectangle &rect);
	DFBResult	DrawRectangle(const DFBRectangle &rect);
	DFBResult	DrawLine(const DFBRegion &line);
	DFBResult	ReleaseSource();

	int			GetNumPointsAllocated() { return polyAllocated; }
	int *		GetPolyPoints(int n);
	int			FreePolyPoints(int /*n*/) { if (polyPoints) free(polyPoints); return 0; }
	void		SetPenWidth(int width) { penWidth = width; }
	int			GetPenWidth() { return penWidth; }

    bool        isSrcColorKey() const { return m_bSrcColorKey; }

private:
	SonicCriticalSection mLock;
	int		polyAllocated;
	int*	polyPoints;
	int		penWidth;
    bool    m_bSrcColorKey;
	DFBRegion	clipRegion;
};

#endif	//IDFBSURFACE_H
