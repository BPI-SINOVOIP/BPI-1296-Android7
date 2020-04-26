#ifndef __INPUT_BD_H__
#define __INPUT_BD_H__

#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NAV_USE_DLL_PLUGIN__
    HRESULT FUNCNAME_OPEN_INPUT_PLUGIN
#else
    HRESULT openInputPluginBD
#endif
    (
        ENUM_MEDIA_TYPE majorType,
        ENUM_MEDIA_TYPE subType,
        osal_mutex_t*   mutex,
        INPUTPLUGIN*    pInputPlugin
    );

#ifdef __cplusplus
}
#endif

#endif /*__INPUT_BD_H__*/
