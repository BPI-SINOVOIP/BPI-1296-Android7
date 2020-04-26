/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMANAGER_H__
#define __DRMMANAGER_H__

#include <drmcommon.h>
#if DRM_SUPPORT_REVOCATION
#include <drmrevocation.h>
#endif /* DRM_SUPPORT_REVOCATION */
#include <drmcontextsizes.h>
#include <drmviewprops.h>
#include <drmenvelope.h>
#include <drmcallbacks.h>

ENTER_PK_NAMESPACE;

typedef struct
{
    DRM_BYTE rgbBuffer[ __CB_DECL( SIZEOF( DRM_CIPHER_CONTEXT ) ) ];
} DRM_DECRYPT_CONTEXT;

#if !DRM_LICENSE_STATE_TYPES_DEFINED
#define DRM_LICENSE_STATE_TYPES_DEFINED 1
#endif /* DRM_LICENSE_STATE_TYPES_DEFINED */

typedef enum
{
    DRM_DGP_DEVICE_CERT_MD    = 1,
    DRM_DGP_DEVICE_CERT_PD    = 2,
    DRM_DGP_CLIENT_INFO       = 3,
    DRM_DGP_PLAYREADY_VERSION = 4,
    DRM_DGP_SECURITY_VERSION  = 5,
    DRM_DGP_WMDRMPD_VERSION   = 6
} DRM_DEVICE_GET_PROPERTY;

/*
** pbKeyID:      Array of bytes that contains the KeyID from the V1 ASF header object.
** cbKeyID:      Count of bytes in pbKeyID.
** pbSecretData: Array of bytes that contains the SecretData from the V1 ASF header.
** cbSecretData: Count of bytes in pbSecretData.
** pbURL:        Array of bytes that contains the license acquisition URL from the V1 ASF header.
** cbURL:        Count of bytes in pbURL.
*/
typedef struct
{
    const DRM_BYTE *pbKeyID;

    DRM_DWORD       cbKeyID;

    const DRM_BYTE *pbSecretData;

    DRM_DWORD       cbSecretData;

    const DRM_BYTE *pbURL;

    DRM_DWORD       cbURL;

} DRM_CSP_V1_HEADER_DATA;

typedef enum
{
    DRM_LGP_MIN_APP_SEC = 1,
    DRM_LGP_SOURCE_ID,
    DRM_LGP_REVINFO_VERSION,
    DRM_LGP_MID,
    DRM_LGP_HAS_INCLUSION_GUID
} DRM_LICENSE_GET_PROPERTY;

typedef struct
{
    DRM_CHAR szMID[ DRM_MAX_LICENSE_CHAIN_DEPTH ][ CCH_BASE64_EQUIV( SIZEOF( DRM_MID ) ) ] ;
} DRM_LGP_MID_EXTRA_DATA;

typedef struct
{
    DRM_GUID guid;
} DRM_LGP_HAS_INCLUSION_GUID_EXTRA_DATA;

/* Strings to be used from DRMManager */
extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_PLAYBACK;

EXIT_PK_NAMESPACE;

#include <drmviewprops.h>

ENTER_PK_NAMESPACE;

typedef struct
{
    /* This data is Opaque.  Do not set any value in it. */
    DRM_BYTE rgbOpaqueBuffer[ __CB_DECL( DRM_APP_CONTEXT_BUFFER_SIZE ) ];
} DRM_APP_CONTEXT;

#define DRM_METER_RESPONSE_PARTIAL              0x00000001

EXIT_PK_NAMESPACE;

#if DRM_SUPPORT_DOMAINS

#include <drmdomainapi.h>

#endif /* DRM_SUPPORT_DOMAINS */

#if DRM_SUPPORT_METERING

#include <drmmeterapi.h>

#endif /* DRM_SUPPORT_METERING */

#include <drmlicacqv3.h>

#include <drmsoapxmlutility.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL _SetupLicEvalObjectToShare(
    IN DRM_APP_CONTEXT_INTERNAL *f_poAppContextInternal );

DRM_API DRM_RESULT DRM_CALL Drm_Initialize(
    IN       DRM_APP_CONTEXT  *f_poAppContext,
    IN       DRM_VOID         *f_pOEMContext,
    IN const DRM_CONST_STRING *f_pdstrDeviceStoreName );

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

#if DRM_SUPPORT_DLA

#if DRM_SUPPORT_LICENSE_SYNC

DRM_API DRM_RESULT DRM_CALL Drm_SyncListItem_InitEnum(
    IN DRM_APP_CONTEXT *f_poAppContext );

DRM_API DRM_RESULT DRM_CALL Drm_SyncListItem_EnumNext(
    IN  DRM_APP_CONTEXT *f_poAppContext,
    IN  DRM_DWORD        f_cMaxCount,
    IN  DRM_DWORD        f_cMaxHours,
    OUT DRM_KID         *f_pKID );

#endif  /* DRM_SUPPORT_LICENSE_SYNC */

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_GenerateChallenge(
    IN                                      DRM_APP_CONTEXT  *f_poAppContext,
    __in_ecount( f_cRights ) const          DRM_CONST_STRING *f_rgpdstrRights[],
    IN                                      DRM_DWORD         f_cRights,
    IN const                                DRM_DOMAIN_ID    *f_poDomainID,
    __in_ecount_opt( f_cchCustomData )      DRM_CHAR         *f_pchCustomData,
    IN                                      DRM_DWORD         f_cchCustomData,
    __out_ecount_opt( *f_pcchSilentURL )    DRM_CHAR         *f_pchSilentURL,
    IN OUT                                  DRM_DWORD        *f_pcchSilentURL,
    __out_ecount_opt( *f_pcchNonSilentURL ) DRM_CHAR         *f_pchNonSilentURL,
    IN OUT                                  DRM_DWORD        *f_pcchNonSilentURL,
    __out_bcount_opt( *f_pcbChallenge )     DRM_BYTE         *f_pbChallenge,
    IN OUT                                  DRM_DWORD        *f_pcbChallenge );

#endif  /* DRM_SUPPORT_DLA */

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_ProcessResponse(
    IN                          DRM_APP_CONTEXT           *f_poAppContext,
    IN                          pfnLicenseResponseCallback f_pfnCallback,
    IN                          DRM_VOID                  *f_pvCallbackContext,
    __in_bcount( f_cbResponse ) DRM_BYTE                  *f_pbResponse,
    IN                          DRM_DWORD                  f_cbResponse,
    OUT                         DRM_LICENSE_RESPONSE      *f_poLicenseResponse );

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
    __inout_ecount( 1 )                               DRM_DECRYPT_CONTEXT  *f_pDecryptContext,
    __in_bcount_opt( min(DRM_CPHR_CB_FOR_INITDECRYPT,f_cbData) ) DRM_BYTE  *f_pbLast15,
    __in                                                        DRM_DWORD   f_cbData );

DRM_API DRM_RESULT DRM_CALL Drm_Reader_Decrypt(
    __in_ecount( 1 )           DRM_DECRYPT_CONTEXT          *f_pDecryptContext,
    __inout_opt                DRM_AES_COUNTER_MODE_CONTEXT *f_pCtrContext,
    __inout_bcount( f_cbData ) DRM_BYTE                     *f_pbData,
    __in                       DRM_DWORD                     f_cbData );

DRM_API DRM_RESULT DRM_CALL Drm_License_GetProperty(
    IN                                    DRM_APP_CONTEXT           *f_poAppContext,
    IN                                    DRM_LICENSE_GET_PROPERTY   f_eProperty,
    __inout_bcount_opt( *f_pcbExtraData ) DRM_BYTE                  *f_pbExtraData,
    IN                                    DRM_DWORD                 *f_pcbExtraData,
    OUT                                   DRM_DWORD                 *f_pdwOutputData );

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

#define DRM_SYNC_IGNORE_THRESHOLD_PARAMETER    0xFFFFFFFF

#if DRM_SUPPORT_LICENSE_SYNC

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_GetContentHeader(
    IN                                       DRM_APP_CONTEXT *f_poAppContext,
    IN                                       DRM_KID         *f_pkid,
    __out_ecount_opt( *f_pcchContentHeader ) DRM_WCHAR       *f_pwszContentHeader,
    IN OUT                                   DRM_DWORD       *f_pcchContentHeader );

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

#endif /* DRM_SUPPORT_LICENSE_SYNC */

#if DRM_SUPPORT_SECURE_CLOCK

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

#endif /* DRM_SUPPORT_SECURE_CLOCK */

DRM_API DRM_RESULT DRM_CALL Drm_StoreMgmt_CleanupStore(
    IN       DRM_APP_CONTEXT        *f_poAppContext,
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

DRM_RESULT DRM_API DRM_CALL Drm_Envelope_WritePlayReadyObject(
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

DRM_API DRM_RESULT DRM_CALL Drm_SecureStore_GetExtendedTokenValue(
    IN       DRM_SECSTORE_CONTEXT *f_pContextSST,
    IN const DRM_WORD              f_wRightID,
    IN const DRM_WORD              f_wRestrictionID,
    IN const DRM_CONST_STRING     *f_pdstrAttribute,
    OUT      TOKEN                *f_pToken );

DRM_API DRM_RESULT DRM_CALL Drm_SecureStore_SetExtendedTokenValue(
    IN       DRM_SECSTORE_CONTEXT *f_pContextSST,
    IN const DRM_WORD              f_wRightID,
    IN const DRM_WORD              f_wRestrictionID,
    IN const DRM_CONST_STRING     *f_pdstrAttribute,
    IN const TOKEN                *f_pToken,
    IN       DRM_DWORD             f_dwFlags );

#if DRM_SUPPORT_REVOCATION

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

#endif /* DRM_SUPPORT_REVOCATION */

/**********************************************************************
**
** Function:    Drm_Writer_Encrypt
**
** Synopsis:    Encrypts cleartext content using the DRM content encryption algorithm and the given encryption context.
**
** Arguments:   [pEncryptContext] -- Encryption context previously initialized.
**              [pCtrContext]     -- If encrypting with AES counter mode, pointer to a counter mode context
**              [pbData]          -- Input data that is encrypted in place
**              [cbData]          -- Count of bytes in pbData
**
** Returns:     DRM_SUCECSS on succes or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              any return code as defined in drmresults.h
**
** Notes:       Content is encrypted in place.  The input buffer will be modified.
**
**********************************************************************/
DRM_RESULT DRM_API Drm_Writer_Encrypt(
    IN                      DRM_CIPHER_CONTEXT           *pEncryptContext,
    __inout_opt             DRM_AES_COUNTER_MODE_CONTEXT *pCtrContext,
    __inout_bcount(cbData)  DRM_BYTE                     *pbData,
    IN                      DRM_DWORD                     cbData );

EXIT_PK_NAMESPACE;

#endif /* __DRMMANAGER_H__ */

