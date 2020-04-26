#ifndef IDFBVIDEOPROVIDER_H
#define IDFBVIDEOPROVIDER_H

#include "IDFBBase.h"
#include "IDFBSurface.h"

class IDFBVideoProvider : public IDFBBase<IDirectFBVideoProvider>
{
public:
	IDFBVideoProvider(IDirectFBVideoProvider* dfbif = 0) : IDFBBase <IDirectFBVideoProvider> (dfbif) {}

	DFBResult	GetCapabilities(DFBVideoProviderCapabilities &caps);
	DFBResult	GetSurfaceDescription(DFBSurfaceDescription &dscr);
	DFBResult	GetStreamDescription(DFBStreamDescription &dscr);
	DFBResult	PlayTo(IDFBSurface *dest, const DFBRectangle *destRect, DVFrameCallback callback, void *ctx);
	DFBResult	Stop();
	DFBResult	SeekTo(double seconds);
	DFBResult	GetPos(double &position);
	DFBResult	GetLength(double &length);
	DFBResult	GetColorAdjustment(DFBColorAdjustment &adj);
	DFBResult	SetColorAdjustment(DFBColorAdjustment &adj);
};

#endif
