/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#include <drmcommon.h>
#include <drmrevocation.h>
#include <wmdrmndresults.h>
#include <ndtcontextsizes.h>
#include <drmndtmanager.h>
#include <oemndt.h>

#define CERTIFICATE_INFO_INITIALIZER { {0},  \
                                       {0},  \
                                       0,    \
                                       0,    \
                                       0,    \
                                       NULL  \
                                     }

ENTER_PK_NAMESPACE;

WMDRMND_RESULT DRM_NDT_DRToWMDRMNDError(
    __in const DRM_RESULT f_dr
);

DRM_RESULT DRM_CALL GetWMDRMNETRevocationEntries(
    __in_bcount(f_cbCRL) DRM_BYTE             *f_pbCRL,
    __in const           DRM_DWORD             f_cbCRL,
    __out                DRM_DWORD            *f_pcEntries,
    __out                WMDRMNET_CRL_ENTRY  **f_ppEntries
);

DRM_RESULT GetWMDRMNDRevocationList(
    __in  NDTMGR_CONTEXT_INTERNAL                 *f_pContext,
    __deref_out_bcount(*f_pcbCRLBuffer) DRM_BYTE **f_ppbCRLBuffer,
    __out DRM_DWORD                               *f_pcbCRLBuffer,
    __out DRM_DWORD                               *f_pdwCRLVersion
);

DRM_VOID LicenseFreeKey(
    __in     NDT_LICENSE_CONTEXT  *f_pLicContext
);

DRM_RESULT NDTMGR_Internal_FindSession(
    __in        NDTMGR_CONTEXT                      *f_pNdtMgrContext,
    __in const  DRM_ID                               f_idSession,
    __in const  NDTMGR_SESSION_TYPE                  f_SessionType,
    __out       NDTMGR_INTERNAL_SESSION_CONTEXT    **f_ppSessionContext
);

DRM_RESULT NDTMGR_Internal_CreateSession(
    __in_opt    DRM_VOID                            *f_pOEMContext,
    __in        NDTMGR_CONTEXT                      *f_pNdtMgrContext,
    __in const  NDTMGR_SESSION_TYPE                  f_sessionType,
    __in_opt    DRM_APP_CONTEXT                     *f_pAppContext,
    __out       NDTMGR_INTERNAL_SESSION_CONTEXT    **f_ppSessionContext
);

DRM_RESULT NDTMGR_Internal_RemoveSession(
    __in        NDTMGR_CONTEXT                      *f_pNdtMgrContext,
    __in        NDTMGR_INTERNAL_SESSION_CONTEXT     *f_pSessionContext
);

DRM_RESULT DRM_NDT_Proximity_VerifyEncryptedChallenge(
    __in_opt   DRM_VOID                 *f_pOEMContext,
    __in       NDTMGR_CONTEXT           *f_pNdtMgrContext,
    __in const DRM_ID                    f_idRegistrationSession,
    __in       DRM_ID                    f_EncryptedChallenge
);

DRM_RESULT DRM_NDT_Proximity_GetChallenge(
    __in_opt   DRM_VOID                 *f_pOEMContext,
    __in       NDTMGR_CONTEXT           *f_pNdtMgrContext,
    __in const DRM_ID                    f_idRegistrationSession,
    __out      DRM_ID                   *f_pChallenge
);

EXIT_PK_NAMESPACE;

