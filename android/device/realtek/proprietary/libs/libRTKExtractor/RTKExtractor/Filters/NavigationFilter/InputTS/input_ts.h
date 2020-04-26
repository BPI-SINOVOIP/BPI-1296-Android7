#ifndef __INPUT_TS_H__
#define __INPUT_TS_H__

#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#ifdef __NAV_USE_DLL_PLUGIN__
HRESULT
FUNCNAME_OPEN_INPUT_PLUGIN(
    ENUM_MEDIA_TYPE             majorType,
    ENUM_MEDIA_TYPE             subType,
    osal_mutex_t*               mutex,
    INPUTPLUGIN*                pInputPlugin);
#else
HRESULT
openInputPluginTS(
    ENUM_MEDIA_TYPE             majorType,
    ENUM_MEDIA_TYPE             subType,
    osal_mutex_t*               mutex,
    INPUTPLUGIN*                pInputPlugin);
#endif


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INPUT_TS_H__
