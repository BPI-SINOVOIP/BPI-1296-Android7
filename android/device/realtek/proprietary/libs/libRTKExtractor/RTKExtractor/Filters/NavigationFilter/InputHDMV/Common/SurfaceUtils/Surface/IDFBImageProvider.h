#ifndef IDFBIMAGEPROVIDER_H
#define IDFBIMAGEPROVIDER_H

#include "IDFBBase.h"
#include "IDFBSurface.h"

class IDFBImageProvider : public IDFBBase<IDirectFBImageProvider>
{
public:
	IDFBImageProvider(IDirectFBImageProvider* dfbif = 0) : IDFBBase <IDirectFBImageProvider> (dfbif) {}

	DFBResult	GetSurfaceDescription(DFBSurfaceDescription &dscr);
	DFBResult	GetImageDescription(DFBImageDescription &dscr);
	DFBResult	RenderTo(IDFBSurface *dest, const DFBRectangle &destRect);
	DFBResult	SetRenderCallback(DIRenderCallback callback, void *callbackData);
};

#endif
