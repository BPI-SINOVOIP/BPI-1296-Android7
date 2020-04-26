/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#pragma once


#define CallXmlBuilder(context, fn)   {\
    while (TRUE)\
    {\
        dr = (fn);\
        if ( dr == DRM_E_BUFFERTOOSMALL )\
        {\
            ChkDR(DRMTOOLS_ReallocXmlContext(&(context)));\
            continue;\
        }\
        ChkDR(dr);\
        break;\
    }\
}

#define MAX_FILENAME 1023
#define XML_INIT_SIZE   1024


