/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMDLA_H__
#define __DRMDLA_H__

#include <drmcommon.h>
#include <drmviewrights.h>
#include <drmxmlbuilder.h>

ENTER_PK_NAMESPACE;
DRM_API DRM_BOOL DRM_CALL DRM_DLA_IsDLASupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_DLA_IsDLAUnsupported(DRM_VOID);

#define CB_STACK_LICENSE_CHALLENGE ((((SIZEOF(CLIENTID)*4)/3)+8)*SIZEOF(DRM_WCHAR))

typedef struct __tagDRM_LICENSE_CHALLENGE
{
    /* XML context buffer */

    _XMBContext             *pbXML;
    DRM_DWORD                cbXML;

    /* uplink entries */

    DRM_LICENSE_STATE_DATA    rglicensestatedata  [DRM_MAX_LICENSE_CHAIN_DEPTH+1];
    DRM_CONST_STRING          rgdstrUplinkKID     [DRM_MAX_LICENSE_CHAIN_DEPTH+1];
    DRM_DWORD                 cUplinks;

    /* actions/rights */

    const DRM_CONST_STRING **ppdstrRights;
    DRM_DWORD                cRights;

    /* miscellaneous client data */

    DRM_CONST_STRING         dstrDeviceCert;
    DRM_CONST_STRING         dstrClientIdentity;
    DRM_CONST_STRING         dstrHeader;
    DRM_CONST_STRING         dstrV1Challenge;
    DRM_CONST_STRING         dstrRevInfo;
    DRM_DWORD                levelAppSecurity;
    DRM_DWORD                idSubject;
    DRM_WCHAR               *pwszBBVer;
    CLIENTID                 clientid;

    DRM_BB_CONTEXT          *pcontextBBX;

    /* stack buffer */

    DRM_BYTE   rgbStack [__CB_DECL(CB_STACK_LICENSE_CHALLENGE)];
} DRM_LICENSE_CHALLENGE;

/**********************************************************************
** Function:    DRM_LA_CreateChallenge
** Synopsis:    Creates and initializes a request context.
** Arguments:   [f_plicensechallenge] -- pointer to a DRM_LICENSE_CHALLENGE, whose members are initialized by the
**                                     caller as follows:
**                  cbXML              -- for internal use
**                  pbXML              -- for internal use
**                  dstrHeader         -- WMRM Header extracted from media file
**                  pwszBbVer          -- NULL terminated string containing the blackbox version
**                  pdstrV1Challenge   -- pointer to a pregenerated V1 challenge string.  Can be NULL.
**                  clientid           -- CLIENTID structure created by the blackbox
**                  rgdstrUplinkKID    -- array of DRM_CONST_STRINGs, the KID's that were found in the
**                                        content header (UPLINK kids)
**                  rglicensestatedata -- The current license state data for each KID in rgdstrKID
**                  cUplinks           -- The number of UPLINKS found in the header.  This is the length
**                                        of the rgdstrUplinkKID and rglicensestatedata arrays
**                  ppdstrRights       -- array of DRM_CONST_STRING pointerss naming rights/actions
**                  cRights            -- array size of ppdstrRights
**                  dstrEncryptedCert  -- device certificate filled in by the caller.  This should be
**                                        encrypted and base64 encoded (for privacy).
**                  pdstrClientIdentity-- string representation of the client id (encrypted/encoded old
**                                        style certificate of the client
**                  levelAppSecurity   -- app security number preparsed from the device certificate
**                  idSubject          -- subject ID preparsed from the device certificate
**
**              [f_pszChallenge]       -- user-allocated buffer for challenge output, B64 encoded; NULL to query size
**              [f_cchChallenge]      -- size of pbChallenge
**
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**
** Notes:       To get the minimum size required, first call with pbData and/or pbChallenge set to NULL.
**              This will return DRM_E_BUFFERTOOSMALL and pcbData and pcbChallenge will point to values
**              of minimum context size.
***********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_LA_CreateChallenge (
    IN DRM_LICENSE_CHALLENGE *f_plicensechallenge,
    OUT      DRM_CHAR        *f_pszChallenge,
    IN OUT   DRM_DWORD       *f_pcchChallenge);

/**********************************************************************
** Function:    DRM_LA_GetClientInfo
** Synopsis:    Creates CLIENTINFO
** Arguments:   [f_pclientid] -- pointer to CLIENTID
**              [f_pbStack] -- pointer to stack for internal use.
**              [f_pcbStack] --pointer to size of f_pbStack in bytes
**              [f_pdstrClientVersionData] --pointer to drm string containing Client version info
**              [f_pdstrSecurityVersionData] -- pointer to drm string containing security version info
**              [f_AppSecurityData] -- Application security data.
**              [f_SubjectId1] --Subject ID1
**              [f_SubjectId2] --Subject ID2 (0 for no subject id2)
**              [f_pdstrDRMKVersion] --DRMK version(0 for no drmk version).
**              [f_pdstrEncryptedDevcert] --pointer to drm string containing encrypted devcert. Optional
**              [f_pbXML] --pointer to a buffer to get this client info.
**              [f_pcbXML] --pointer to size of f_pbXML in bytes
**              [f_fCreateNewDoc] --Flag. If TRUE, creates a new document otherwise add nodes to an existing document
**              [f_pdstrRevInfo] -- pointer to a string with the clients current revinfo versions.  Optional
** Returns:     DRM_SUCCESS on success, or failed with
**              DRM_E_INVALIDARG,
**              DRM_E_BUFFERTOOSMALL.
** Sample CLIENTINFO:
**
**
** Add the following node/subnode to the request:
**
** <CLIENTINFO>
**    <CLIENTID>...</CLIENTID>
**    <CLIENTVERSION>...</CLIENTVERSION>
**    <SECURITYVERSION>...</SECURITYVERSION>
**    <APPSECURITY>...</APPSECURITY>
**    <SUBJECTID1>...</SUBJECTID1>
**    <SUBJECTID2>...</SUBJECTID2>
**    <DRMKVERSION>...</DRMKVERSION>
**    <DEVCERT>...</DEVCERT> optional
** </CLIENTINFO>
**
************************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_LA_GetClientInfo (
    IN       CLIENTID               *f_pclientid,
    __in_bcount( *f_pcbStack ) DRM_BYTE *f_pbStack,
    IN OUT   DRM_DWORD              *f_pcbStack,
    IN       DRM_CONST_STRING       *f_pdstrClientVersionData,
    IN       DRM_CONST_STRING       *f_pdstrSecurityVersionData,
    IN       DRM_DWORD               f_AppSecurityData,
    IN       DRM_DWORD               f_SubjectId1,
    IN       DRM_DWORD               f_SubjectId2,
    IN       DRM_CONST_STRING       *f_pdstrDRMKVersion,
    IN       DRM_CONST_STRING       *f_pdstrEncryptedDevcert,
    IN       _XMBContext            *f_pbXML,
    IN OUT   DRM_DWORD              *f_pcbXML,
    IN       DRM_CRYPTO_CONTEXT     *f_pcontextCRYP,
    IN       DRM_VOID               *f_pOEMContext,
    IN       DRM_BOOL                f_fCreateNewDoc,
    IN       DRM_CONST_STRING       *f_pdstrRevInfo );

EXIT_PK_NAMESPACE;

#endif /* __DRMDLA_H__ */

