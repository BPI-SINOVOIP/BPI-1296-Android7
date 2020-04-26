/***************************************************************************
                          InputDVDVR.h  -  description
                             -------------------
    begin                : 1/4/05
    copyright            : (C) 2005 by Tony Huang
    email                : tonyhuang@realtek.com.tw
 ***************************************************************************/

#ifndef __INPUT_DVD_VR_H__
#define __INPUT_DVD_VR_H__

#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __NAV_USE_DLL_PLUGIN__
    HRESULT FUNCNAME_OPEN_INPUT_PLUGIN
#else
    HRESULT openInputPluginDVDVR
#endif
    (
        ENUM_MEDIA_TYPE majorType,
        ENUM_MEDIA_TYPE subType,
        osal_mutex_t* mutex,
        INPUTPLUGIN* pInputPlugin
    );

#endif //__INPUT_DVD_VR_H__
