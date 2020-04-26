#ifndef __LIVE_IO_PLUGIN_H__
#define __LIVE_IO_PLUGIN_H__


#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT openIOPlugin_live(IOPLUGIN* pIOPlugin);
//HRESULT openIOPlugin_Transcode(IOPLUGIN* pIOPlugin);

#ifdef __cplusplus
}
#endif

#endif /*__LIVE_IO_PLUGIN_H__*/

