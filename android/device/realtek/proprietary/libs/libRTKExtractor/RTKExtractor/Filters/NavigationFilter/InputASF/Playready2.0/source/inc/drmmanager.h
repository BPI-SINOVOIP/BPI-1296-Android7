/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMANAGER_H__
#define __DRMMANAGER_H__

#include <drmcommon.h>
#include <drmrevocation.h>
#include <drmcontextsizes.h>
#include <drmviewrights.h>
#include <drmenvelope.h>
#include <drmcallbacks.h>
#include <drmdomainapi.h>
#include <drmlicacqv3.h>
#include <drmmanagertypes.h>

ENTER_PK_NAMESPACE;

/* Minimum OpaqueBuffer size. */
#define MINIMUM_APPCONTEXT_OPAQUE_BUFFER_SIZE                  ( DRM_MAX_LICENSE_CHAIN_DEPTH * ( DWORD_ALIGNED_SIZE( DRM_STANDARD_PLAYREADYOBJSIZE ) + DWORD_ALIGNED_SIZE( DRM_MAX_LICENSESIZE ) ) )

DRM_API DRM_RESULT DRM_CALL Drm_Initialize(
    __in                                    DRM_APP_CONTEXT     *f_poAppContext,
    __in_opt                                DRM_VOID            *f_pOEMContext,
    __in_bcount( f_cbOpaqueBuffer )         DRM_BYTE            *f_pbOpaqueBuffer,
    __in                                    DRM_DWORD            f_cbOpaqueBuffer,
    __in                              const DRM_CONST_STRING    *f_pdstrDeviceStoreName );

DRM_API DRM_RESULT DRM_CALL Drm_ResizeOpaqueBuffer(
    __in                                    DRM_APP_CONTEXT     *f_poAppContext,
    __in_bcount( f_cbOpaqueBuffer )         DRM_BYTE            *f_pbOpaqueBuffer,
    __in                                    DRM_DWORD            f_cbOpaqueBuffer );

DRM_API DRM_RESULT DRM_CALL Drm_GetOpaqueBuffer(
    __in                                    DRM_APP_CONTEXT     *f_poAppContext,
    __out                                   DRM_BYTE           **f_ppbOpaqueBuffer );

DRM_API DRM_VOID DRM_CALL Drm_Uninitialize(
    IN DRM_APP_CONTEXT *f_poAppContext );

DRM_API DRM_RESULT DRM_CALL Drm_Reinitialize(
    IN DRM_APP_CONTEXT *f_poAppContext );

DRM_API DRM_VOID DRM_CALL Drm_ClearAllCaches( DRM_VOID );

DRM_API DRM_RESULT DRM_CALL Drm_Content_SetProperty(
    IN                                        DRM_APP_CONTEXT          *f_poAppContext,
    IN                                        DRM_CONTENT_SET_PROPERTY  f_eProperty,
    __in_bcount_opt( f_cbPropertyData ) const DRM_BYTE                 *f_pbPropertyData,
    IN                                        DRM_DWORD                 f_cbPropertyData );

DRM_API DRM_RESULT DRM_CALL Drm_Content_GetProperty(
    __inout                                DRM_APP_CONTEXT          *f_poAppContext,
    __in                                   DRM_CONTENT_GET_PROPERTY  f_eProperty,
    __out_bcount_opt( *f_pcbPropertyData ) DRM_BYTE                 *f_pbPropertyData,
    __inout                                DRM_DWORD                *f_pcbPropertyData );

DRM_API DRM_RESULT DRM_CALL Drm_Content_UpdateEmbeddedStore(
    __in    DRM_APP_CONTEXT     *f_poAppContext );

DRM_API DRM_RESULT DRM_CALL Drm_Content_UpdateEmbeddedStore_Commit(
    __in    DRM_APP_CONTEXT     *f_poAppContext );

DRM_API DRM_RESULT DRM_CALL Drm_Device_GetProperty(
    IN                                  DRM_APP_CONTEXT         *f_poAppContext,
    IN                                  DRM_DEVICE_GET_PROPERTY  f_eProperty,
     __out_bcount_opt( *f_pcbProperty ) DRM_BYTE                *f_pbProperty,
    IN OUT                              DRM_DWORD               *f_pcbProperty );

DRM_API DRM_RESULT DRM_CALL Drm_SyncListItem_InitEnum(
    IN DRM_APP_CONTEXT *f_poAppContext );

DRM_API DRM_RESULT DRM_CALL Drm_SyncListItem_EnumNext(
    IN  DRM_APP_CONTEXT *f_poAppContext,
    IN  DRM_DWORD        f_cMaxCount,
    IN  DRM_DWORD        f_cMaxHours,
    OUT DRM_KID         *f_pKID );

/*
** ---------------------------------------
** License acquisition API functions
** ---------------------------------------
*/
DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_GenerateChallenge(
    IN                                            DRM_APP_CONTEXT  *f_poAppContext,
    __in_ecount( f_cRights )                const DRM_CONST_STRING *f_rgpdstrRights[],
    IN                                            DRM_DWORD         f_cRights,
    IN                                      const DRM_DOMAIN_ID    *f_poDomainID,
    __in_ecount_opt( f_cchCustomData )      const DRM_CHAR         *f_pchCustomData,
    IN                                            DRM_DWORD         f_cchCustomData,
    __out_ecount_opt( *f_pcchSilentURL )          DRM_CHAR         *f_pchSilentURL,
    IN OUT                                        DRM_DWORD        *f_pcchSilentURL,
    __out_ecount_opt( *f_pcchNonSilentURL )       DRM_CHAR         *f_pchNonSilentURL,
    IN OUT                                        DRM_DWORD        *f_pcchNonSilentURL,
    __out_bcount_opt( *f_pcbChallenge )           DRM_BYTE         *f_pbChallenge,
    IN OUT                                        DRM_DWORD        *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_ProcessResponse(
    IN                          DRM_APP_CONTEXT                *f_poAppContext,
    IN                          DRM_PROCESS_LIC_RESPONSE_FLAG   f_dwFlags,
    IN                          pfnLicenseResponseCallback      f_pfnCallback,
    IN                          DRM_VOID                       *f_pvCallbackContext,
    __in_bcount( f_cbResponse ) DRM_BYTE                       *f_pbResponse,
    IN                          DRM_DWORD                       f_cbResponse,
    OUT                         DRM_LICENSE_RESPONSE           *f_poLicenseResponse );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_GenerateAck(
    __in                                DRM_APP_CONTEXT      *f_poAppContext,
    __in                                DRM_LICENSE_RESPONSE *f_poLicenseResponse,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE             *f_pbChallenge,
    __inout                             DRM_DWORD            *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_ProcessAckResponse(
    __in                        DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount( f_cbResponse ) DRM_BYTE        *f_pbResponse,
    __in                        DRM_DWORD        f_cbResponse,
    __out_opt                   DRM_RESULT      *f_pResult );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_GetContentHeader(
    IN                                       DRM_APP_CONTEXT *f_poAppContext,
    IN                                       DRM_KID         *f_pkid,
    __out_ecount_opt( *f_pcchContentHeader ) DRM_WCHAR       *f_pwszContentHeader,
    IN OUT                                   DRM_DWORD       *f_pcchContentHeader );

DRM_API DRM_RESULT DRM_CALL Drm_Reader_Bind(
    IN                             DRM_APP_CONTEXT      *f_poAppContext,
    __in_ecount( f_cRights ) const DRM_CONST_STRING     *f_rgpdstrRights[],
    IN                             DRM_DWORD             f_cRights,
    IN                             DRMPFNPOLICYCALLBACK  f_pfnPolicyCallback,
    IN const                       DRM_VOID             *f_pv,
    OUT                            DRM_DECRYPT_CONTEXT  *f_pcontextDCRY );

DRM_API DRM_RESULT DRM_CALL Drm_Reader_Commit(
    IN       DRM_APP_CONTEXT      *f_poAppContext,
    IN       DRMPFNPOLICYCALLBACK  f_pfnPolicyCallback,
    IN const DRM_VOID             *f_pvCallbackData );

DRM_API DRM_RESULT DRM_CALL Drm_Reader_InitDecrypt(
    __inout_ecount( 1 )                                                DRM_DECRYPT_CONTEXT *f_pDecryptContext,
    __in_bcount_opt( min(DRM_CPHR_CB_FOR_INITDECRYPT,f_cbData) ) const DRM_BYTE            *f_pbLast15,
    __in                                                               DRM_DWORD            f_cbData );

DRM_API DRM_RESULT DRM_CALL Drm_Reader_Decrypt(
    __in_ecount( 1 )           DRM_DECRYPT_CONTEXT          *f_pDecryptContext,
    __inout_opt                DRM_AES_COUNTER_MODE_CONTEXT *f_pCtrContext,
    __inout_bcount( f_cbData ) DRM_BYTE                     *f_pbData,
    __in                       DRM_DWORD                     f_cbData );

DRM_API DRM_RESULT DRM_CALL Drm_License_GetProperty(
    IN                                          DRM_APP_CONTEXT           *f_poAppContext,
    IN                                          DRM_LICENSE_GET_PROPERTY   f_eProperty,
    __inout_bcount_opt( *f_pcbExtraData ) const DRM_BYTE                  *f_pbExtraData,
    IN                                    const DRM_DWORD                 *f_pcbExtraData,
    OUT                                         DRM_DWORD                 *f_pdwOutputData );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseQuery_GetState(
    IN                                    DRM_APP_CONTEXT        *f_poAppContext,
    __in_ecount( f_cRightsQueried ) const DRM_CONST_STRING       *f_rgpdstrRights[],
    IN                                    DRM_DWORD               f_cRightsQueried,
    __out_ecount( f_cRightsQueried )      DRM_LICENSE_STATE_DATA  f_rgStateData[],
    IN                                    DRMPFNPOLICYCALLBACK    f_pfnPolicyCallback,
    IN const                              DRM_VOID               *f_pv );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseQuery_IsAllowed(
    IN       DRM_APP_CONTEXT            *f_poAppContext,
    IN const DRM_CONST_STRING           *f_pdstrQuery,
    IN       DRMPFNPOLICYCALLBACK        f_pfnPolicyCallback,
    IN const DRM_VOID                   *f_pvCallbackData,
    OUT      DRM_LICENSE_STATE_CATEGORY *f_pCategory );


/*
** ---------------------------------------
** Sync list API functions
** ---------------------------------------
*/
#define DRM_SYNC_IGNORE_THRESHOLD_PARAMETER    0xFFFFFFFF

DRM_API DRM_RESULT DRM_CALL Drm_Sync_GenerateChallenge(
    IN                             DRM_APP_CONTEXT *f_poAppContext,
    IN                             DRM_DWORD        f_cMaxCount,
    IN                             DRM_DWORD        f_cMaxHours,
    IN                             DRM_DWORD        f_iKIDStart,
    IN                             DRM_DWORD        f_cKIDs,
    OUT                            DRM_DWORD       *f_piKIDNext,
    OUT                            DRM_DWORD       *f_pcKIDs,
    __out_bcount_opt( *f_pcbData ) DRM_BYTE        *f_pbData,
    IN OUT                         DRM_DWORD       *f_pcbData );

/*
** ---------------------------------------
** Secure Clock API functions
** ---------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL Drm_SecureClock_ProcessResponse(
    IN                          DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount( f_cbResponse ) DRM_BYTE        *f_pbResponse,
    IN                          DRM_DWORD        f_cbResponse,
    OUT                         DRM_RESULT      *f_pResult );

DRM_API DRM_RESULT DRM_CALL Drm_SecureClock_GenerateChallenge(
    IN                                  DRM_APP_CONTEXT *f_poAppContext,
    __out_ecount_opt( *f_pcchURL )      DRM_WCHAR       *f_pwszURL,
    IN OUT                              DRM_DWORD       *f_pcchURL,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE        *f_pbChallenge,
    IN OUT                              DRM_DWORD       *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL Drm_SecureClock_GetValue(
    IN                                            DRM_APP_CONTEXT *f_poAppContext,
    __in_ecount_opt( *f_pcchSecureTime )          DRM_WCHAR       *f_pwszSecureTime,
    IN OUT                                        DRM_DWORD       *f_pcchSecureTime,
    IN                                            DRM_DWORD       *f_pdwFlags,
    __in_bcount_opt( *f_pcbSecureTimeWithStatus ) DRM_BYTE        *f_pbSecureTimeWithStatus,
    IN OUT                                        DRM_DWORD       *f_pcbSecureTimeWithStatus );

DRM_API DRM_RESULT DRM_CALL Drm_StoreMgmt_CleanupStore(
    IN       DRM_APP_CONTEXT        *f_poAppContext,
    IN       DRM_DWORD               f_dwFlags,
    IN const DRM_VOID               *f_pvCallerData,
    IN       DRM_DWORD               f_dwCallbackInterval,
    IN       pfnStoreCleanupProgress f_pfnCallback );

DRM_API DRM_RESULT DRM_CALL Drm_StoreMgmt_DeleteLicenses(
    IN  DRM_APP_CONTEXT  *f_poAppContext,
    IN  DRM_CONST_STRING *f_pdcstrKID,
    OUT DRM_DWORD        *f_pcLicDeleted );

DRM_API DRM_RESULT DRM_CALL Drm_ProcessCommand(
    IN                                   DRM_APP_CONTEXT *f_poAppContext,
    IN                                   DRM_DWORD        f_dwOperationCode,
    IN                                   DRM_DWORD        f_dwRequestArgument1,
    IN                                   DRM_DWORD        f_dwRequestArgument2,
    IN                                   DRM_DWORD        f_dwRequestArgument3,
    IN                                   DRM_DWORD        f_dwRequestArgument4,
    __in_bcount( f_dwRequestDataLength ) DRM_BYTE        *f_pbRequestData,
    IN                                   DRM_DWORD        f_dwRequestDataLength,
    OUT                                  DRM_DWORD       *f_pdwResponseResult1,
    OUT                                  DRM_DWORD       *f_pdwResponseResult2,
    OUT                                  DRM_DWORD       *f_pdwResponseResult3,
    OUT                                  DRM_DWORD       *f_pdwResponseResult4 );

DRM_API DRM_RESULT DRM_CALL Drm_ProcessRequest(
    IN                                       DRM_APP_CONTEXT *f_poAppContext,
    IN                                       DRM_DWORD        f_dwOperationCode,
    IN                                       DRM_DWORD        f_dwRequestArgument1,
    IN                                       DRM_DWORD        f_dwRequestArgument2,
    IN                                       DRM_DWORD        f_dwRequestArgument3,
    IN                                       DRM_DWORD        f_dwRequestArgument4,
    OUT                                      DRM_DWORD       *f_pdwResponseResult1,
    OUT                                      DRM_DWORD       *f_pdwResponseResult2,
    OUT                                      DRM_DWORD       *f_pdwResponseResult3,
    OUT                                      DRM_DWORD       *f_pdwResponseResult4,
    __out_bcount( *f_pdwResponseDataLength ) DRM_BYTE        *f_pbResponseData,
    OUT                                      DRM_DWORD       *f_pdwResponseDataLength );

/*
** ---------------------------------------
** Envelope API functions
** ---------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_Open(
    IN           DRM_APP_CONTEXT            *f_poAppContext,
    IN           DRM_VOID                   *f_pOEMContext,
    __in_z const DRM_WCHAR                  *f_pwszFilename,
    OUT          DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_DuplicateFileContext(
    IN           DRM_VOID                   *f_pOEMContext,
    __in_z const DRM_WCHAR                  *f_pwszFilename,
    IN           DRM_ENVELOPED_FILE_CONTEXT *f_pSourceHandle,
    OUT          DRM_ENVELOPED_FILE_CONTEXT *f_pNewHandle );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_InitializeRead(
    IN DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile,
    IN DRM_DECRYPT_CONTEXT        *f_pDecrypt );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_Close(
    IN DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_GetSize(
    IN  DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile,
    OUT DRM_DWORD                  *f_pcbFileSize );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_Read(
    IN                                                DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile,
     __out_ecount_part( f_cbToRead, *f_pcbBytesRead ) DRM_BYTE                   *f_pbBuffer,
    IN                                                DRM_DWORD                   f_cbToRead,
    OUT                                               DRM_DWORD                  *f_pcbBytesRead );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_WritePlayReadyObject(
    __in         DRM_APP_CONTEXT            *f_poAppContext,
    __inout_opt  DRM_VOID                   *f_pOEMContext,
    __in_z const DRM_WCHAR                  *f_pwszFilename,
    __inout      DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_Seek(
    IN  DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile,
    IN  DRM_LONG                    f_lDistanceToMove,
    IN  DRM_DWORD                   f_dwMoveMethod,
    OUT DRM_DWORD                  *f_pdwNewFilePointer );

DRM_API DRM_RESULT DRM_CALL Drm_Envelope_GetOriginalFilename(
    IN                                          DRM_ENVELOPED_FILE_CONTEXT *f_pEnvFile,
    __out_ecount_opt( *f_pcchOriginalFilename ) DRM_WCHAR                  *f_pwszOriginalFilename,
    IN OUT                                      DRM_DWORD                  *f_pcchOriginalFilename );

/*
** ---------------------------------------
** Revocation API functions
** ---------------------------------------
*/

/* Recommeded revocation buffer size. */
#define REVOCATION_BUFFER_SIZE                  30 * 1024

DRM_API DRM_RESULT DRM_CALL Drm_Revocation_SetBuffer(
    __inout                                 DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount_opt( f_cbRevocationBuffer ) DRM_BYTE        *f_pbRevocationBuffer,
    __in                                    DRM_DWORD        f_cbRevocationBuffer );

DRM_API DRM_RESULT DRM_CALL Drm_Revocation_GetBuffer(
    __inout                                      DRM_APP_CONTEXT  *f_poAppContext,
    __deref_out_bcount( *f_pcbRevocationBuffer ) DRM_BYTE        **f_ppbRevocationBuffer,
    __out                                        DRM_DWORD        *f_pcbRevocationBuffer );

DRM_API DRM_RESULT DRM_CALL Drm_Revocation_StoreRevListArray(
    __inout                           DRM_APP_CONTEXT *f_poAppContext,
    __in                              DRM_DWORD        f_cRevocationLists,
    __in_ecount( f_cRevocationLists ) DRM_RVK_LIST    *f_pRevocationLists );

DRM_API DRM_RESULT DRM_CALL Drm_Revocation_GetList(
    __inout                          DRM_APP_CONTEXT          *f_poAppContext,
    __in                             DRM_REVOCATION_TYPE_ENUM  f_eRevType,
    __out_bcount_opt( *f_pcbBuffer ) DRM_BYTE                 *f_pbBuffer,
    __inout_opt                      DRM_DWORD                *f_pcbBuffer,
    __out_opt                        DRM_DWORD                *f_pdwVersion );

DRM_API DRM_RESULT DRM_CALL Drm_Revocation_StorePackage(
    __inout                           DRM_APP_CONTEXT *f_poAppContext,
    __in_ecount( f_cchPackage ) const DRM_CHAR        *f_pchPackage,
    __in                              DRM_DWORD        f_cchPackage );

DRM_API DRM_RESULT Drm_Platform_Initialize( DRM_VOID );

DRM_API DRM_RESULT Drm_Platform_Uninitialize( DRM_VOID );

/*
** ---------------------------------------
** Activation API functions
** ---------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL Drm_Activation_ProcessResponseGenerateChallenge(
    __inout_bcount( ACTIVATION_CONTEXT_SIZE )  DRM_BYTE        *f_pbActivationCtx,
    __in_bcount_opt( f_cbResponse )            DRM_BYTE        *f_pbResponse,
    __in_opt                                   DRM_DWORD        f_cbResponse,
    __out_bcount_opt( *f_pcbChallenge )        DRM_BYTE        *f_pbChallenge,
    __inout                                    DRM_DWORD       *f_pcbChallenge,
    __out_ecount_opt( *f_pcchUrl )             DRM_CHAR        *f_pszUrl,
    __inout                                    DRM_DWORD       *f_pcchUrl,
    __out                                      DRM_RESULT      *f_pResult );

DRM_API DRM_RESULT DRM_CALL Drm_GetContextSizes(
    __out DRM_DWORD *f_pdwAppContextSize, 
    __out DRM_DWORD *f_pdwOpaqueBufferSize, 
    __out DRM_DWORD *f_pdwDecryptContextSize);


DRM_API DRM_RESULT DRM_CALL Drm_Reader_CloneDecryptContext(
    __in_ecount(1)  DRM_DECRYPT_CONTEXT *f_pDecryptContextIn,
    __out_ecount(1) DRM_DECRYPT_CONTEXT *f_pDecryptContextOut);

EXIT_PK_NAMESPACE;

#endif /* __DRMMANAGER_H__ */

