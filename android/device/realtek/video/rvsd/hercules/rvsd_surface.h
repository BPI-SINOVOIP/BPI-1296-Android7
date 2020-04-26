#ifndef _RVSD_SURFACE_H_
#define _RVSD_SURFACE_H_

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#ifdef Android_JB
#include <gui/ISurfaceTexture>
#else
#include <gui/IGraphicBufferProducer.h>
#endif
#include <utils/RefBase.h>
using namespace android;

sp<SurfaceControl> getSurfaceControl(sp<SurfaceComposerClient>& composerClient);
sp<Surface> getSurface(sp<SurfaceControl>& surfaceControl);
#ifdef Android_JB
sp<ISurfaceTexture>& getSurfaceTexture(sp<Surface>& surface);
#else
sp<IGraphicBufferProducer>& getSurfaceTexture(sp<Surface>& surface);
#endif

#endif // _RVSD_SURFACE_H_
