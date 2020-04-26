#ifndef __RTP_IO_PLUGIN_H__
#define __RTP_IO_PLUGIN_H__


#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT openIOPlugin_Rtp(IOPLUGIN* pIOPlugin);
//HRESULT openIOPlugin_Transcode(IOPLUGIN* pIOPlugin);

#ifdef __cplusplus
}
#endif

#endif /*__RTP_IO_PLUGIN_H__*/

