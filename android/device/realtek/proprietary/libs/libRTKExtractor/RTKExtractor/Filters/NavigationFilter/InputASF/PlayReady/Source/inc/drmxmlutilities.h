/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMXMLUTILS_H__
#define __DRMXMLUTILS_H__

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_XMU_MatchNodeFromTemplate(
    IN  const   DRM_CONST_STRING    *f_pdstrXmlNodeList,
    IN  const   DRM_CONST_STRING    *f_pdstrXmlTemplate,
    IN          DRM_BOOL            f_fAttributeMatch,
        OUT     DRM_BOOL            *f_pfMatch);

EXIT_PK_NAMESPACE;

#endif      /* __DRMXMLUTILS_H__ */
