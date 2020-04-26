/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
** Header file for the Error contract mechanism.
*/

#ifndef __DRMCONTRACT_H__
#define __DRMCONTRACT_H__

ENTER_PK_NAMESPACE;

#if DRM_ERROR_CONTRACT_TEST_MODE

/*
** The following are the API IDs for each top-level API with an Error Code Contract (ECC).
** The order here is alphabetical by name to match the ordering in the .chm help file:
**     PlayReady.chm --> Programming Reference --> DRM Functions
**
** IF YOU ADD/DELETE ANY APIs HERE, YOU MUST ALSO:
** 1) ENSURE THAT THE CHM FILE IS UP-TO-DATE
** 2) UPDATE THE g_prgdrErrorCodelist STRUCTURE IN 'drmcontract.c' TO MATCH THIS ORDERING
** 3) POSSIBLY UPDATE THE TEST FILE ...\Test\Utils\ErrorContractTest.pc.pk.ce.txt (HAS HARDCODED API IDs)
*/
enum ECC_DRM_API_List
{
    ECC_DRM_GENERIC_ERROR_LIST = 0,
    
    ECC_Drm_Content_UpdateEmbeddedStore,
    ECC_Drm_Content_SetProperty,
    ECC_Drm_Device_GetProperty,
    ECC_Drm_DomainCert_EnumNext,
    ECC_Drm_DomainCert_Find,
    ECC_Drm_DomainCert_InitEnum,
    ECC_Drm_Envelope_Close,
    ECC_Drm_Envelope_DuplicateFileContext,
    ECC_Drm_Envelope_GetSize,
    ECC_Drm_Envelope_GetOriginalFileName,
    ECC_Drm_Envelope_InitializeRead,
    ECC_Drm_Envelope_Open,
    ECC_Drm_Envelope_Read,
    ECC_Drm_Envelope_Seek,
    ECC_Drm_GetAdditionalResponseData,
    ECC_Drm_Initialize,
    ECC_Drm_JoinDomain_GenerateChallenge,
    ECC_Drm_JoinDomain_ProcessResponse,
    ECC_Drm_LeaveDomain_GenerateChallenge,
    ECC_Drm_LeaveDomain_ProcessResponse,
    ECC_Drm_License_GetProperty,
    ECC_Drm_LicenseAcq_GenerateChallenge,
    ECC_Drm_LicenseAcq_GenerateAck,
    ECC_Drm_LicenseAcq_GetContentHeader,
    ECC_Drm_LicenseAcq_ProcessAckResponse,
    ECC_Drm_LicenseAcq_ProcessResponse,
    ECC_Drm_LicenseQuery_GetState,
    ECC_Drm_LicenseQuery_IsAllowed,
    ECC_Drm_MeterCert_Delete,
    ECC_Drm_MeterCert_EnumNext,
    ECC_Drm_MeterCert_GenerateChallenge,
    ECC_Drm_MeterCert_InitEnum,
    ECC_Drm_MeterCert_Invalidate,
    ECC_Drm_MeterCert_ProcessResponse,
    ECC_Drm_MeterCert_Update,
    ECC_Drm_Metering_GenerateChallenge,
    ECC_Drm_Metering_ProcessResponse,
    ECC_Drm_ProcessCommand,
    ECC_Drm_ProcessRequest,
    ECC_Drm_Reader_Bind,
    ECC_Drm_Reader_Commit,
    ECC_Drm_Reader_Decrypt,
    ECC_Drm_Reader_InitDecrypt,
    ECC_Drm_Reinitialize,
    ECC_Drm_SecureClock_GenerateChallenge,
    ECC_Drm_SecureClock_GetValue,
    ECC_Drm_SecureClock_ProcessResponse,
    ECC_Drm_SecureStore_GetExtendedTokenValue,
    ECC_Drm_SecureStore_SetExtendedTokenValue,
    ECC_Drm_StoreMgmt_CleanupStore,
    ECC_Drm_StoreMgmt_DeleteLicenses,
    ECC_Drm_Sync_GenerateChallenge,         
    ECC_Drm_SyncListItem_EnumNext,
    ECC_Drm_SyncListItem_InitEnum,
    ECC_Drm_Uninitialize,
    ECC_Drm_Content_GetProperty,
    ECC_Drm_Envelope_WritePlayReadyObject,
    ECC_Drm_Content_UpdateEmbeddedStore_Commit,
    ECC_Drm_PlayReadyObject_ConvertFromWmdrmHeader
};

/*
** Macro to interface with the DRM API.
** The argument 'apivalue' should be one of the values defined above
*/
#define ChkECC( apivalue, dr )                                                                     \
     do                                                                                            \
     {                                                                                             \
         if ( DRM_FAILED( dr ) && ( !DRM_ECC_ExpectedErrorCode( apivalue, dr ) ) )                \
         {                                                                                         \
              TRACE( ( "Error code 0x%X doesn't match expected error codes for "#apivalue, dr ) ); \
              DRMASSERT( FALSE ) ;                                                                 \
         }                                                                                         \
     } while ( FALSE )

/*
** Declaring the API arrays containing the error codes 
*/ 
extern DRM_RESULT g_rgdrGenericErrorList[];

extern DRM_RESULT g_rgdrDrm_Content_UpdateEmbeddedStore[];
extern DRM_RESULT g_rgdrDrm_Content_SetProperty[];
extern DRM_RESULT g_rgdrDrm_Device_GetProperty[];
extern DRM_RESULT g_rgdrDrm_DomainCert_EnumNext[];
extern DRM_RESULT g_rgdrDrm_DomainCert_Find[];
extern DRM_RESULT g_rgdrDrm_DomainCert_InitEnum[];
extern DRM_RESULT g_rgdrDrm_Envelope_Close[];
extern DRM_RESULT g_rgdrDrm_Envelope_DuplicateFileContext[];
extern DRM_RESULT g_rgdrDrm_Envelope_GetSize[];
extern DRM_RESULT g_rgdrDrm_Envelope_GetOriginalFileName[];
extern DRM_RESULT g_rgdrDrm_Envelope_InitializeRead[];
extern DRM_RESULT g_rgdrDrm_Envelope_Open[];
extern DRM_RESULT g_rgdrDrm_Envelope_Read[];
extern DRM_RESULT g_rgdrDrm_Envelope_Seek[];
extern DRM_RESULT g_rgdrDrm_GetAdditionalResponseData[];
extern DRM_RESULT g_rgdrDrm_Initialize[];
extern DRM_RESULT g_rgdrDrm_JoinDomain_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_JoinDomain_ProcessResponse[];
extern DRM_RESULT g_rgdrDrm_LeaveDomain_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_LeaveDomain_ProcessResponse[];
extern DRM_RESULT g_rgdrDrm_License_GetProperty[];
extern DRM_RESULT g_rgdrDrm_LicenseAcq_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_LicenseAcq_GenerateAck[];
extern DRM_RESULT g_rgdrDrm_LicenseAcq_GetContentHeader[];
extern DRM_RESULT g_rgdrDrm_LicenseAcq_ProcessAckResponse[];
extern DRM_RESULT g_rgdrDrm_LicenseAcq_ProcessResponse[];
extern DRM_RESULT g_rgdrDrm_LicenseQuery_GetState[];
extern DRM_RESULT g_rgdrDrm_LicenseQuery_IsAllowed[];
extern DRM_RESULT g_rgdrDrm_MeterCert_Delete[];
extern DRM_RESULT g_rgdrDrm_MeterCert_EnumNext[];
extern DRM_RESULT g_rgdrDrm_MeterCert_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_MeterCert_InitEnum[];
extern DRM_RESULT g_rgdrDrm_MeterCert_Invalidate[];
extern DRM_RESULT g_rgdrDrm_MeterCert_ProcessResponse[];
extern DRM_RESULT g_rgdrDrm_MeterCert_Update[];
extern DRM_RESULT g_rgdrDrm_Metering_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_Metering_ProcessResponse[];
extern DRM_RESULT g_rgdrDrm_ProcessCommand[];
extern DRM_RESULT g_rgdrDrm_ProcessRequest[];
extern DRM_RESULT g_rgdrDrm_Reader_Bind[];
extern DRM_RESULT g_rgdrDrm_Reader_Commit[];
extern DRM_RESULT g_rgdrDrm_Reader_Decrypt[];
extern DRM_RESULT g_rgdrDrm_Reader_InitDecrypt[];
extern DRM_RESULT g_rgdrDrm_Reinitialize[];
extern DRM_RESULT g_rgdrDrm_SecureClock_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_SecureClock_GetValue[];
extern DRM_RESULT g_rgdrDrm_SecureClock_ProcessResponse[];
extern DRM_RESULT g_rgdrDrm_SecureStore_GetExtendedTokenValue[];
extern DRM_RESULT g_rgdrDrm_SecureStore_SetExtendedTokenValue[];
extern DRM_RESULT g_rgdrDrm_StoreMgmt_CleanupStore[];
extern DRM_RESULT g_rgdrDrm_StoreMgmt_DeleteLicenses[];
extern DRM_RESULT g_rgdrDrm_Sync_GenerateChallenge[];
extern DRM_RESULT g_rgdrDrm_SyncListItem_EnumNext[];
extern DRM_RESULT g_rgdrDrm_SyncListItem_InitEnum[];
extern DRM_RESULT g_rgdrDrm_Uninitialize[];
extern DRM_RESULT g_rgdrDrm_Content_GetProperty[];
extern DRM_RESULT g_rgdrDrm_Envelope_WritePlayReadyObject[];
extern DRM_RESULT g_rgdrDrm_Content_UpdateEmbeddedStore_Commit[];
extern DRM_RESULT g_rgdrDrm_PlayReadyObject_ConvertFromWmdrmHeader[];

DRM_API DRM_BOOL DRM_CALL DRM_ECC_ExpectedErrorCode(
    __in const DRM_UINT   f_uiAPIid, 
    __in const DRM_RESULT f_dr );
    
#else      
#define ChkECC( apivalue, dr )     
#endif   /* DRM_ERROR_CONTRACT_TEST_MODE */

EXIT_PK_NAMESPACE;

#endif   /* __DRMCONTRACT_H__ */
