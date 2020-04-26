/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CHAIN_H__
#define __DRM_CHAIN_H__

#include <drmchaintypes.h>
#include <drmcallbacks.h>
#include <drmliceval.h>

ENTER_PK_NAMESPACE;

/* This function is recursive!!!
   INPUTS -- stuff in DRM_FFLICENSE struct
   OUTPUT -- array of binding info structures to pass to CreateDecryptor
             Array of open secure store contexts
             Array of KID/LID pairs that were used in the chain
  Caller should call CreateDecryptor when ready
  Will have to read the license in again.
  Caller should then read each license in and do the DoneWithOperation for each license
  Caller should then update metering data for each license */

DRM_API DRM_RESULT DRM_CALL DRM_LIC_CompleteLicenseChain(
    IN OUT   DRM_FFLICENSE               *f_pFFLicense,
    IN const DRM_CONST_STRING            *f_rgpdstrRights[], /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD                    f_cRights,
       OUT   DRM_DWORD                   *f_pdwChainDepth,
    IN       DRM_DST                     *f_pDatastore,
    IN       DRMPFNPOLICYCALLBACK         f_pfnPolicyCallback,
    IN const DRM_VOID                    *f_pv );

DRM_API DRM_RESULT DRM_CALL DRM_LIC_ReportActions(
    IN OUT   DRM_FFLICENSE                *f_pFFLicense,
    IN const DRM_CONST_STRING             *f_rgpdstrRights[], /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD                     f_cRights,
    IN       DRM_DWORD                     f_dwChainDepth,
    IN       DRM_DST                      *f_pDatastore,
    IN       DRMPFNPOLICYCALLBACK          f_pfnPolicyCallback,
    IN const DRM_VOID                     *f_pv );

DRM_API DRM_RESULT DRM_CALL DRM_LIC_CheckClockRollback(
    IN DRM_LICEVAL_CONTEXT                      *f_pLicEval,
    IN DRM_LICSTORE_CONTEXT                     *f_pLicStoreXML,
    IN DRM_LICSTORE_CONTEXT                     *f_pLicStoreXMR,
    IN DRM_LICSTOREENUM_CONTEXT                 *f_pLicEnum,
    IN DRM_BYTE                                 *f_pbBuffer,
    IN DRM_DWORD                                 f_cbBuffer,
    IN DRM_DST                                  *f_pDatastore,
    __in_bcount( f_cbStackBuffer ) DRM_BYTE     *f_pbStackBuffer,
    IN DRM_DWORD                                 f_cbStackBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_LICOPL_ProcessPlayOutputLevelDataXMR( 
    IN const DRM_XMR_PLAYBACK_RIGHTS      *f_pdstrRestrictions,
    IN OUT   DRM_PLAY_OPL_EX2             *f_pPlayOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

EXIT_PK_NAMESPACE;

#endif  /* __DRM_CHAIN_H__ */
