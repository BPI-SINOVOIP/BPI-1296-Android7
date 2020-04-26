/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMXMLUTILS_H__
#define __DRMXMLUTILS_H__


#ifdef __cplusplus
extern "C" {
#endif

DRM_RESULT DRM_API DRM_XMU_MatchNodeFromTemplate(
    IN  const   DRM_CONST_STRING    *f_pdstrXmlNodeList,
    IN  const   DRM_CONST_STRING    *f_pdstrXmlTemplate,
    IN          DRM_BOOL            f_fAttributeMatch,
        OUT     DRM_BOOL            *f_pfMatch);


#ifdef __cplusplus
           }
#endif
           
#endif      /* __DRMXMLUTILS_H__ */
