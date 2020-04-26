#ifndef __CAMERA_IO_PLUGIN_H__
#define __CAMERA_IO_PLUGIN_H__

//#include "../IOPlugins.h"
#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT openIOPlugin_Camera(IOPLUGIN* pIOPlugin);

#ifdef __cplusplus
}
#endif

#endif /*__CAMERA_IO_PLUGIN_H__*/
