#ifndef IDFBDISPLAYLAYER_H
#define IDFBDISPLAYLAYER_H

#include "IDFBBase.h"
#include "IDFBSurface.h"
#include "IDFBScreen.h"
#include "IDFBWindow.h"

class IDFBDisplayLayer : public IDFBBase<IDirectFBDisplayLayer>
{
public:
	IDFBDisplayLayer(IDirectFBDisplayLayer* dfbif = 0) : IDFBBase <IDirectFBDisplayLayer> (dfbif) {}

	DFBResult	GetID(DFBDisplayLayerID &layerID);
	DFBResult	GetDescription(DFBDisplayLayerDescription &descr);
	DFBResult	GetSurface(IDFBSurface *&surface);
	DFBResult	GetScreen(IDFBScreen *&screen);
	DFBResult	SetCooperativeLevel(DFBDisplayLayerCooperativeLevel level);
	DFBResult	SetOpacity(UInt8 opacity);
	DFBResult	SetSourceRectangle(int x, int y, int width, int height);
	DFBResult	SetScreenLocation(float x, float y, float width, float height);
	DFBResult	SetScreenPosition(int x, int y);
	DFBResult	SetScreenRectangle(int x, int y, int width, int height);
	DFBResult	SetSrcColorKey(UInt8 r, UInt8 g, UInt8 b);
	DFBResult	SetDstColorKey(UInt8 r, UInt8 g, UInt8 b);
	DFBResult	GetLevel(int &level);
	DFBResult	SetLevel(int level);
	DFBResult	GetCurrentOutputField(int &field);
	DFBResult	SetFieldParity(int field);
	DFBResult	WaitForSync();
	DFBResult	GetConfiguration(DFBDisplayLayerConfig &config);
	DFBResult	TestConfiguration(const DFBDisplayLayerConfig &config, DFBDisplayLayerConfigFlags &flags);
	DFBResult	SetConfiguration(DFBDisplayLayerConfig &config);
	DFBResult	SetBackgroundMode(DFBDisplayLayerBackgroundMode mode);
	DFBResult	SetBackgroundImage(IDFBSurface &surface);
	DFBResult	SetBackgroundColor(UInt8 r,UInt8 g, UInt8 b, UInt8 a);
	DFBResult	GetColorAdjustment(DFBColorAdjustment &adj);
	DFBResult	SetColorAdjustment(const DFBColorAdjustment &adj);
	DFBResult	CreateWindow(const DFBWindowDescription &desc, IDFBWindow *&window);
	DFBResult	GetWindow(DFBWindowID windowID, IDFBWindow *&window);
	DFBResult	EnableCursor(bool enable);
	DFBResult	GetCursorPosition(int &x, int &y);
	DFBResult	WarpCursor(int x, int y);
	DFBResult	SetCursorAcceleration(int numerator, int denominator, int threshold);
	DFBResult	SetCursorShape(IDFBSurface &shape, int hotX, int hotY);
	DFBResult	SetCursorOpacity(UInt8 opacity);
};

#endif
