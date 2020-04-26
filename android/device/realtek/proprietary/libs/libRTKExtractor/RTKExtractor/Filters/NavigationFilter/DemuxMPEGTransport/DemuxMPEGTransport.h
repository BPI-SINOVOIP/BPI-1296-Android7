#ifndef __DEMUX_MPEG_TRANSPORT_H__
#define __DEMUX_MPEG_TRANSPORT_H__

#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NAV_USE_DLL_PLUGIN__
    HRESULT FUNCNAME_OPEN_DEMUX_PLUGIN
#else
    HRESULT openDemuxPluginMPEGTransport
#endif
    (
        ENUM_MEDIA_TYPE type,
        int             channelIndex,
        INPUTPLUGIN*    pInputPlugin,
        NAVDEMUXCONFIG* pDemuxConfig,
        DEMUXPLUGIN*    pDemuxPlugin
    );

#ifdef __cplusplus
}
#endif

#endif /*__DEMUX_MPEG_TRANSPORT_H__*/
