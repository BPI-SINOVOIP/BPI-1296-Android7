/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRM_LA_H__
#define __DRM_LA_H__

#include <drmlicstore.h>
#include <drmliceval.h>
#include <drmviewprops.h>
#include <drmsyncstore.h>
#include <drmxmlbuilder.h>

#ifdef __cplusplus
extern "C" {
#endif

#if DRM_SUPPORT_DLA
#define CB_STACK_LICENSE_CHALLENGE ((((SIZEOF(CLIENTID)*4)/3)+8)*SIZEOF(DRM_WCHAR))

typedef struct __tagDRM_LICENSE_CHALLENGE
{
    /* XML context buffer */

    _XMBContext             *pbXML;    
    DRM_DWORD                cbXML;
    
    /* uplink entries */
    
    DRM_LICENSE_STATE_DATA   rglicensestatedata  [DRM_MAX_LICENSE_CHAIN_DEPTH+1];
    DRM_CONST_STRING         rgdstrUplinkKID     [DRM_MAX_LICENSE_CHAIN_DEPTH+1];
    DRM_DWORD                cUplinks;
    
    /* actions/rights */
    
    const DRM_CONST_STRING **ppdstrRights;
    DRM_DWORD                cRights;
    
    /* miscellaneous client data */
    
    DRM_CONST_STRING         dstrDeviceCert,
                             dstrClientIdentity,
                             dstrHeader,
                             dstrV1Challenge;
    DRM_DWORD                levelAppSecurity, 
                             idSubject;
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

DRM_RESULT DRM_API DRM_LA_CreateChallenge (
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

DRM_RESULT DRM_API DRM_LA_GetClientInfo (
    IN       CLIENTID               *f_pclientid,
    IN       DRM_BYTE               *f_pbStack,
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
    IN       DRM_BOOL                f_fCreateNewDoc );

#endif

typedef enum
{
    DRM_STORE_LICENSE_NOTIFY_KID       = 1,  /* Callbacks with this reason will have pvData as a DRM_CONST_STRING */
    DRM_STORE_LICENSE_STORE_V1_LICENSE = 2   /* Callbacks with this reason will have pvData as a DRM_ANSI_CONST_STRING */

#if DRM_SUPPORT_PLAYLIST_BURN
    , DRM_STORE_LICENSE_NOTIFY_PLAYLIST_BURN_UPDATE = 3 /* Callbacks with this reason will have pvData as a DRM_PLAYLIST_NOTIFICATION_STRUCT */
#endif

} DRM_STORE_LICENSE_CALLBACK_ENUM;

#if DRM_SUPPORT_PLAYLIST_BURN
typedef struct 
{
    const DRM_CONST_STRING *pdstrKID;
    DRM_DWORD               cPlaylistBurnIncrement;
} DRM_PLAYLIST_NOTIFICATION_STRUCT;
#endif

typedef DRM_RESULT (DRM_API *pfnStoreLicenseCallback)( 
    IN DRM_VOID                        *pvData, 
    IN DRM_STORE_LICENSE_CALLBACK_ENUM  eReason, 
    IN DRM_VOID                        *pvContext );

/**********************************************************************
** Function:    DRM_LA_ProcessResponse
** Synopsis:    Process license request response received from license server
** Arguments:   [f_pbResponse] -- Response string received from license server
**              [f_cbResponse] -- size of pbResponse in bytes.
**              [f_pLicEvalContext] --pointer to the context of license eval object
**              [f_pbLicenseStoreContext] --pointer to the context of license store object
**              [f_pfnCallback] -- pointer to a callback function if the caller wants notification of successfull license stores
**              [f_pvCallbackContext] -- pointer to a context that will be passed to the callback function.
**              [f_poHdsContext] --pointer to the current HDS context that should be used
**              [f_pcontextViewRights] --pointer to a view rights context.  This is only used if Sync Support is enabled.
**              [f_rgbLicenseBuffer] -- Pointer to a buffer of size DRM_MAX_LICENSESIZE bytes.  This will be used as a temporary working buffer.
**              [f_plReason] -- to receive the reason code
** Returns:     DRM_SUCCESS on success, or failed with 
**              DRM_E_INVALIDARG,
**              DRM_E_INVALIDLICENSE, and
**              error codes from Licesne store.
** Notes:       This function process each license(s) contained in the response 
**              received from the license server. It tries to evaluate each license to see
**              if it can store in the license store.
** Sample Response:
**
** <LICENSERESPONSE>
**     <LICENSE version="2.0.0.0">
**         {base64-encoded DRMv7 License}
**     </LICENSE>
**     ...
**     <LICENSE version="2.0.0.0">
**         {base64-encoded DRMv7 License}
**     </LICENSE>
** </LICENSERESPONSE>
***********************************************************************
*/

DRM_RESULT DRM_API DRM_LA_ProcessResponse(
    IN     DRM_BYTE                *f_pbResponse,
    IN     DRM_DWORD                f_cbResponse,
    IN     DRM_LICEVAL_CONTEXT     *f_pLicEvalContext,
    IN     DRM_LICSTORE_CONTEXT    *f_pbLicenseStoreContext,
    IN     pfnStoreLicenseCallback  f_pfnCallback,
    IN     DRM_VOID                *f_pvCallbackContext,
    IN     DRM_HDS_CONTEXT         *f_poHdsContext,
    IN     DRM_VIEW_RIGHTS_CONTEXT *f_pcontextViewRights,
    IN     DRM_BYTE                 f_rgbLicenseBuffer[__CB_DECL(DRM_MAX_LICENSESIZE)],
    IN     DRM_SYNC_CONTEXT        *f_pcontextSync,
       OUT DRM_LONG                *f_plReason);

#ifdef __cplusplus
}
#endif

#endif  /* __DRM_LA_H__ */
