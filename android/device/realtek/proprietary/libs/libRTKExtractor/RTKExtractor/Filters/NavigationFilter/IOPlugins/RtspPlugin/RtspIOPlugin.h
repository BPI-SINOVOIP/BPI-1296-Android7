#ifndef __RTSP_IO_PLUGIN_H__
#define __RTSP_IO_PLUGIN_H__


#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT openIOPlugin_Rtsp(IOPLUGIN* pIOPlugin);
//HRESULT openIOPlugin_Transcode(IOPLUGIN* pIOPlugin);

#ifdef __cplusplus
}
#endif

#endif /*__RTSP_IO_PLUGIN_H__*/

