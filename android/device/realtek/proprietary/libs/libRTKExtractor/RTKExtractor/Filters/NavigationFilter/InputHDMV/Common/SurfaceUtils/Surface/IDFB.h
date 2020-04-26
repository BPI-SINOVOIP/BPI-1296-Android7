#ifndef IDFB_H
#define IDFB_H

#include "IDFBBase.h"
#include "IDFBSurface.h"
#include "IDFBPalette.h"
#include "IDFBScreen.h"
#include "IDFBDisplayLayer.h"
#include "IDFBInputDevice.h"
#include "IDFBEventBuffer.h"
#include "IDFBDataBuffer.h"
#include "IDFBImageProvider.h"
#include "IDFBVideoProvider.h"
#include "IDFBFont.h"


class IDFB : public IDFBBase <IDirectFB>
{
public:
	IDFB(IDirectFB *dfbif = 0) : IDFBBase <IDirectFB> (dfbif) {}
	virtual ~IDFB() {}

	DFBResult	CreateSurface(const DFBSurfaceDescription &desc, IDFBSurface *&surface);
	DFBResult	CreatePalette(const DFBPaletteDescription &desc, IDFBPalette *&palette);
	DFBResult	CreateEventBuffer(IDFBEventBuffer *&eventBuffer);
	DFBResult	CreateInputEventBuffer(DFBInputDeviceCapabilities caps, DFBBoolean global, IDFBEventBuffer *&eventBuffer);
	DFBResult	CreateDataBuffer(const DFBDataBufferDescription *desc, IDFBDataBuffer *&dataBuffer);
	DFBResult	CreateImageProvider(const char *filename, IDFBImageProvider *&imageProvider);
	DFBResult	CreateVideoProvider(const char *filename, IDFBVideoProvider *&videoProvider);
	DFBResult	CreateFont(const char *filename, const DFBFontDescription &desc, IDFBFont *&font);
#ifdef CN_ADIRONDACKS
	// "Adirondacks" added an extension to create fonts from memory in addition to the ability to load them from a file.
	DFBResult	CreateMemoryFont(const void *data, unsigned int length, const DFBFontDescription &desc, IDFBFont *&font);
#endif // CN_ADIRONDACKS

	DFBResult	GetScreen(DFBScreenID screenID, IDFBScreen *&screen);
	DFBResult	GetDisplayLayer(DFBDisplayLayerID layerID, IDFBDisplayLayer *&displayLayer);
	DFBResult	GetInputDevice(DFBInputDeviceID deviceID, IDFBInputDevice *&inputDevice);

	DFBResult	SetCooperativeLevel(DFBCooperativeLevel level);
	DFBResult	SetVideoMode(int width, int height, int bpp);
	DFBResult	GetCardCapabilities(
// See http://www.directfb.org/index.php/viewcvs.cgi/DirectFB/include/directfb.h.diff?r1=1.266&r2=1.267
#if (DIRECTFB_MAJOR_VERSION == 0) && (DIRECTFB_MINOR_VERSION == 9) && (DIRECTFB_MICRO_VERSION < 23)
		DFBCardCapabilities 
#else
		DFBGraphicsDeviceDescription
#endif
		&caps);
	DFBResult	EnumVideoModes(DFBVideoModeCallback callback, void *callbackData);
	DFBResult	EnumScreens(DFBScreenCallback callback, void *callbackData);
	DFBResult	EnumDisplayLayers(DFBDisplayLayerCallback callback, void *callbackData);
	DFBResult	EnumInputDevices(DFBInputDeviceCallback callback, void *callbackData);
	DFBResult	SetClipboardData(const char *mime, const void *data, unsigned int size, struct timeval &timestamp);
	DFBResult	GetClipboardData(char **mime, void **data, unsigned int *size);
	DFBResult	GetClipboardTimeStamp(struct timeval &timestamp);
	DFBResult	Suspend();
	DFBResult	Resume();
	DFBResult	WaitIdle();
	DFBResult	WaitForSync();
	DFBResult	GetInterface(const char *type, const char *implementation, void *arg, void *idfb);
};

extern DFBResult	DFBInit(int *argc = 0, char *(*argv[]) = 0);
extern DFBResult	IDirectFBCreate(IDirectFB *&iDirectFB);
extern DFBResult	IDFBCreate(IDFB &idfb);

#endif
