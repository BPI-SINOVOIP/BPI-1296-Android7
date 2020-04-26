/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __PFDDRMCONTEXTSIZES_H__
#define __PFDDRMCONTEXTSIZES_H__

#include <drmcommon.h>
#include <ndttypes.h>
#include <drmndtmanager.h>
#include <ndtcontextsizes.h>
#include <drmdevstore.h>
#include <drmptrlist.h>

ENTER_PK_NAMESPACE;

typedef struct __tagPFDMGR_CONTENT_SESSION_CONTEXT_INTERNAL
{
    DRM_DWORD       dwSessionID;
    DRM_ID          idContentSession;

    DRM_BYTE       *pbLicenseResponseMsg;
    DRM_DWORD       cbLicenseResponseMsg;

    OEM_FILEHDL     hRawFile;

    /*
    ** This is a per-session (file) context. One is needed for each file we
    ** might be streaming.
    */
    DRM_APP_CONTEXT AppContext;
    DRM_BYTE        rgbDrmBuffer[MINIMUM_APPCONTEXT_OPAQUE_BUFFER_SIZE];

} PFDMGR_CONTENT_SESSION_CONTEXT_INTERNAL;

typedef struct __tagPFDMGR_CONTEXT_INTERNAL
{
    /*
    ** This is a global context used for revocation and other housekeeping. It
    ** should share the same HDS as the per-session contexts.
    */
    DRM_APP_CONTEXT AppContext;
    DRM_BYTE        rgbDrmBuffer[MINIMUM_APPCONTEXT_OPAQUE_BUFFER_SIZE];

    NDTMGR_CONTEXT  ndtMgrContext;

    DRM_DWORD       dwRegistrationTransactionID;
    DRM_ID          idRegistrationSession;
    DRM_BYTE       *pbRegistrationResponseMsg;
    DRM_DWORD       cbRegistrationResponseMsg;

    PTR_LIST        ContentSessions;

    DRM_BYTE       *pbCurrentRegistrationRequestMsg;
    DRM_DWORD       cbCurrentRegistrationRequestMsg;
} PFDMGR_CONTEXT_INTERNAL;

#define PFDMGR_CONTEXT_BUFFER_SIZE SIZEOF(PFDMGR_CONTEXT_INTERNAL)

EXIT_PK_NAMESPACE;

#endif  /* __PFDDRMCONTEXTSIZES_H__ */
