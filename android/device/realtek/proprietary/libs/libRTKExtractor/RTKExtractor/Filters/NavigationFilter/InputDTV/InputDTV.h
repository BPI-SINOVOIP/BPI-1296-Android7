#ifndef __INPUT_DTV_H__
#define __INPUT_DTV_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#ifndef __DTV_ALOG__
#define __DTV_ALOG__
#include <utils/Log.h> 
//#define printf(fmt, args...) ALOGD("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
//#define DBGP(fmt, args...) ALOGD("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NAV_USE_DLL_PLUGIN__
    HRESULT FUNCNAME_OPEN_INPUT_PLUGIN
#else
    HRESULT openInputPluginDTV
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

#endif /*__INPUT_DTV_H__*/
