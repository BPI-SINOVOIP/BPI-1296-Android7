/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMANAGER_H__
#define __DRMMANAGER_H__

#include <drmrevocation.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __tagDRM_MANAGER_DECRYPT_CONTEXT
{
    DRM_BYTE rgbBuffer[ __CB_DECL( SIZEOF( DRM_CIPHER_CONTEXT) ) ];
}DRM_MANAGER_DECRYPT_CONTEXT;

typedef struct __tagDRM_MANAGER_ENCRYPT_CONTEXT
{
    DRM_BYTE rgbBuffer[ __CB_DECL( SIZEOF( DRM_CIPHER_CONTEXT) )];
}DRM_MANAGER_ENCRYPT_CONTEXT;


#if !DRM_LICENSE_STATE_TYPES_DEFINED
#define DRM_LICENSE_STATE_TYPES_DEFINED 1
/* Enum and structure for license properties queries */
typedef enum DRM_LICENSE_STATE_CATEGORY
{
    WM_DRM_LICENSE_STATE_NORIGHT = 0,
    WM_DRM_LICENSE_STATE_UNLIM,
    WM_DRM_LICENSE_STATE_COUNT,
    WM_DRM_LICENSE_STATE_FROM,
    WM_DRM_LICENSE_STATE_UNTIL,
    WM_DRM_LICENSE_STATE_FROM_UNTIL,
    WM_DRM_LICENSE_STATE_COUNT_FROM,
    WM_DRM_LICENSE_STATE_COUNT_UNTIL,
    WM_DRM_LICENSE_STATE_COUNT_FROM_UNTIL,
    WM_DRM_LICENSE_STATE_EXPIRATION_AFTER_FIRSTUSE,
    WM_DRM_LICENSE_STATE_FORCE_SYNC
} DRM_LICENSE_STATE_CATEGORY;

typedef struct _DRM_LICENSE_STATE_DATA
{
    DRM_DWORD dwStreamId;                       /* 0 -> All streams, != 0 -> A particular stream. */
    DRM_DWORD dwCategory;                       /* Indicates the category of string to be displayed. */
    DRM_DWORD dwNumCounts;                      /* Number of items supplied in dwCount. */
    DRM_DWORD dwCount[4];                       /* Up to 4 counts. */
    DRM_DWORD dwNumDates;                       /* Number of items supplied in dwDate. */
    DRMFILETIME datetime[4];                    /* Up to 4 dates. */
    DRM_DWORD dwVague;                          /* 0 -> certain, 1 -> atleast.  (There could be more */
                                                /*               licenses. Aggregation not possible.) */
} DRM_LICENSE_STATE_DATA;

#endif

typedef enum DRM_DEVICE_PROPERTY
{
    WM_DRM_CLIENTINFO = 0,
    WM_DRM_DRMVERSION,
    WM_DRM_SECURITYVERSION,
    WM_DRM_V2_HEADER_KID
} DRM_DEVICE_PROPERTY;

#define JANUS_MAJOR_VERSION                 10
#define JANUS_MINOR_VERSION                 1

/* Strings to be used from DRMManager */
extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_PLAYBACK;

#include <drmviewprops.h>

typedef struct __tagDRM_MANAGER_CONTEXT
{
    /* This data is Opaque.  Do not set any value in it. */
    DRM_BYTE rgbOpaqueBuffer[ __CB_DECL( DRM_DRMMANAGER_CONTEXT_BUFFER_SIZE ) ];
} DRM_MANAGER_CONTEXT;

#define DRM_METER_RESPONSE_PARTIAL 0x00000001

/**********************************************************************
**
** Function:    DRM_MGR_Initialize
**
** Synopsis:    Initializes DRM.  The context that is initialized after this call can be used in later calls
**              to most DRM functions to perform a varety of DRM related tasks.
**
** Arguments:   [pDrmContext]          -- Opaque DRM Manager context
**              [pdstrDeviceStoreName] -- Path DRM should use as the current store.
**
** Returns:     DRM_SUCCESS on success or failed with,
**              DRM_E_NEEDDEVCERTINDIV if the device needs to be individualized
**              DRM_E_INVALIDARG if either parameter is incorrect or 
**              other errors found in drmresults.h
**
** Notes:       The pDrmContext should be uninitialized with a call to DRM_MGR_Uninitialize
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_Initialize( 
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN const DRM_CONST_STRING     *pdstrDeviceStoreName );

/**********************************************************************
**
** Function:    DRM_MGR_Uninitialize
**
** Synopsis:    Allows DRM to shutdown any allocated resources.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**
** Returns:     None.
**
**********************************************************************/
DRM_VOID DRM_API DRM_MGR_Uninitialize( 
    IN DRM_MANAGER_CONTEXT *pDrmContext );


/**********************************************************************
**
** Function:    DRM_MGR_Reinitialize
**
** Synopsis:    Cleans up the header information stored in the DRM context.
**              Useful when loading a mix of content with v1 and v2 headers. Call this
**              function in between calls to SetV1Header or SetV2Header.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the argument is invalid.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_Reinitialize( 
    IN       DRM_MANAGER_CONTEXT  *pDrmContext );


/**********************************************************************
**
** Function:    DRM_MGR_SetRevocationBuffer
**
** Synopsis:    Passes in a temporary buffer to be used by revocation code
**
** Arguments:   [pDrmContext]          -- Opaque DRM Manager context
**              [pbRevocationBuffer]   -- Pointer to a buffer
**              [cbRevocationBuffer]   -- Size of the buffer in bytes
**
** Returns:     DRM_SUCCESS on success or failed with,
**              DRM_E_INVALIDARG if a parameter is incorrect or 
**              other errors found in drmresults.h
**
** Notes:       
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_SetRevocationBuffer( 
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN       DRM_BYTE             *pbRevocationBuffer,
    IN       DRM_DWORD             cbRevocationBuffer );

/* Recommeded revocation buffer size */
#define REVOCATION_BUFFER_SIZE 30*1024


/**********************************************************************
**
** Function:    DRM_MGR_GetRevocationBuffer
**
** Synopsis:    Gets the temporary buffer to be used by revocation code
**
** Arguments:   [pDrmContext]          -- Opaque DRM Manager context
**              [ppbRevocationBuffer]   -- Pointer to a pointer that will point to the revocation buffer
**              [pcbRevocationBuffer]   -- Pointer to a DWORD to hold the size of the revocation buffer in bytes
**
** Returns:     DRM_SUCCESS on success or failed with,
**              DRM_E_INVALIDARG if a parameter is incorrect or 
**              other errors found in drmresults.h
**
** Notes:       
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_GetRevocationBuffer( 
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    OUT      DRM_BYTE            **ppbRevocationBuffer,
    OUT      DRM_DWORD            *pcbRevocationBuffer );


/**********************************************************************
**
** Function:    DRM_MGR_UpdateRevocationVersionsCache
**
** Synopsis:    Loads revocation versions from the HDS and caches them for use in DRM_MGR_Bind
**
** Arguments:   [pDrmContext]          -- Opaque DRM Manager context
**              [pfUpdated]            -- Optional pointer to hold a boolean indication of whether any revocation 
**                                        data in the cache was updated
**
** Returns:     DRM_SUCCESS on success or failed with,
**              DRM_E_INVALIDARG if a parameter is incorrect or 
**              other errors found in drmresults.h
**
** Notes:       
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_UpdateRevocationVersionsCache( 
 IN OUT DRM_MANAGER_CONTEXT          *pDrmContext,
    OUT DRM_BOOL                     *pfUpdated);


/**********************************************************************
**
** Function:    DRM_MGR_StoreRevocationLists
**
** Synopsis:    Stores new revocation lists and rev info blobs
**
** Arguments:   [f_pContext]           -- Opaque DRM Manager context
**              [f_cRevocationLists]   -- Number of revocation lists and rev info blobs
**              [f_pRevocationLists]   -- One or more revocation lists and rev info blobs
**
**
** Returns:     DRM_SUCCESS on success or failed with,
**              DRM_E_INVALIDARG if a parameter is incorrect or 
**              other errors found in drmresults.h
**
** Notes:       
**
**********************************************************************/

DRM_RESULT DRM_MGR_StoreRevocationLists(
    IN DRM_MANAGER_CONTEXT *f_pContext,
    IN DRM_DWORD f_cRevocationLists,
    IN DRM_RVK_LIST *f_pRevocationLists );


/**********************************************************************
**
** Function:    DRM_MGR_SetV2Header
**
** Synopsis:    Sets the DRM XML header blob.  This allows the DRM runtime to have enough information to attempt to
**              perform DRM tasks that related to specific peices of content, such as Bind and GetLicenseStateData.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pbHeader]    -- Pointer to the DRM header.  For ASF content this is found in the ContentEncryptionObjEx ASF header.
**              [cbHeader]    -- Count of bytes in pbHeader
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_SetV2Header(
    IN       DRM_MANAGER_CONTEXT *pDrmContext,
    IN const DRM_BYTE            *pbHeader,
    IN       DRM_DWORD            cbHeader );

/**********************************************************************
**
** Function:    DRM_MGR_SetV1Header
**
** Synopsis:    Set the DRMv1 content header information that may have been found in the ASF header.
**
** Arguments:   [pDrmContext]  -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pbKeyID]      -- Array of bytes that contains the KeyID from the V1 ASF header object.
**              [cbKeyID]      -- Count of bytes in pbKeyID
**              [pbSecretData] -- Array of bytes that contains the SecretData from the V1 ASF header
**              [cbSecretData] -- COunt of bytes in pbSecretData
**              [pbURL]        -- Array of bytes that contains the license acquisition URL from the V1 ASF header
**              [cbURL]        -- Count of bytes in pbURL
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
** Notes:       WM Device DRM doesn't understand the DRMv1 header data enough to acquire licenses and use those licenses.
**              It is possible however that DRMv1 content was transferred to a device with a V2 license and thus we must
**              understand enough about it's header to use it.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_SetV1Header(
    IN       DRM_MANAGER_CONTEXT *pDrmContext,
    IN const DRM_BYTE            *pbKeyID,
    IN       DRM_DWORD            cbKeyID,
    IN const DRM_BYTE            *pbSecretData,
    IN       DRM_DWORD            cbSecretData,
    IN const DRM_BYTE            *pbURL,
    IN       DRM_DWORD            cbURL);

#if DRM_SUPPORT_DLA
/**********************************************************************
**
** Function:    DRM_MGR_GenerateLicenseChallenge
**
** Synopsis:    Using the current DRM header set by SetV2Header DRM will generate a liecense challenge that can be sent
**              to a license server.  The license server generally issues a response that should be passed to 
**              DRM_MGR_ProcessLicenseResponse.
**
** Arguments:   [pDrmContext]   -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [ppdstrRights]  -- An array of DSTR*s representing rights that the caller would like to access the content with.
**              [cRights]       -- Count of the pointers in the rgpdstrRights array.
**              [pwszUrl]       -- user allocated buffer to get URL in WCHAR. Null Terminated.
**              [pcchUrl]       -- character count of pwszUrl. Can be NULL if URL is not required.
**              [pszChallenge]  -- The license challenge will be copied into this string buffer
**              [pcchChallenge] -- Length in characters of the pcchChallenge buffer.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_V1_NOT_SUPPORTED if a V1 content header was only set,
**              DRM_E_HEADER_NOT_SET if no content header was set,
**              DRM_E_BUFFERTOOSMALL if the input pszURL or pszChallenge buffers were too small,
**              or other error codes as defined in drmresults.h
**
** Notes:       DRM_MGR_SetV2Header must be called prior to this function.  On return from this function the character
**              count parameters will be updated with the number of characters copied to the buffer if successful
**              or the required length of the buffer if the current buffer size is too small.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GenerateLicenseChallenge(
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN const DRM_CONST_STRING    **ppdstrRights, /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD             cRights,
       OUT   DRM_WCHAR            *pwszUrl,
    IN OUT   DRM_DWORD            *pcchUrl,
       OUT   DRM_CHAR             *pszChallenge,
    IN OUT   DRM_DWORD            *pcchChallenge );


/**********************************************************************
**
** Function:    DRM_MGR_GetDeviceProperty
**
** Synopsis:    Retrieves clients properties.  Available properties are defined in the enum DRM_DEVICE_PROPERTY.
**
** Arguments:   [pDrmContext]   -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [eProperty]     -- The property to retrieve.  Must be a value from the DRM_DEVICE_PROPERTY enum
**              [pbProperty]    -- Buffer to receive the property value.  The format of the data depends upon the property.
**              [pcbProperty]   -- Count of bytes in pbProperty
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_HEADER_NOT_SET if no content header was set,
**              DRM_E_BUFFERTOOSMALL if the input pbProperty buffer is too small,
**              or other error codes as defined in drmresults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetDeviceProperty(
    IN       DRM_MANAGER_CONTEXT  *pDrmContext,
    IN       DRM_DEVICE_PROPERTY   eProperty, 
       OUT   _XMBContext          *pbProperty,
    IN OUT   DRM_DWORD            *pcbProperty );
#endif

/**********************************************************************
**
** Function:    DRM_MGR_ProcessLicenseResponse
**
** Synopsis:    Process a response from a license server.  Usually this is a series of licenses that will ultimately be
**              stored in the device license store.
**
** Arguments:   [f_pDrmContext]       -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [f_pfnCallback]       -- Pointer to a callback function which is called for each license in response. Can be NULL.
**              [f_pvCallbackContext] -- User context returned in callback function. Can be NULL
**              [f_pbResponse]        -- Pointer to a response blob from a license server
**              [f_cbResponse]        -- Count of bytes in pbResponse
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if the arguments are invalid
**              or any other value as defined in drmresults.h
**
** Notes:       pbResponse is not declared as const.  This buffer will be modified in place to conserve device memory.
**              If caller needs a copy unmodified they should copy it elsewhere first.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_ProcessLicenseResponse(
    IN DRM_MANAGER_CONTEXT     *f_pDrmContext,
    IN pfnStoreLicenseCallback  f_pfnCallback,
    IN DRM_VOID                *f_pvCallbackContext,
    IN DRM_BYTE                *f_pbResponse,
    IN DRM_DWORD                f_cbResponse );

/**********************************************************************
**
** Function:    DRM_MGR_Bind
**
** Synopsis:    Ask DRM to a complete search for a license that can be used to render the content matching the header
**              set in a call to DRM_MGR_SetV2Header.
**
** Arguments:   [pDrmContext]             -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [rgpdstrRights]           -- An array of strings representing rights that the caller would like to access the content with.
**              [cRights]                 -- Count of the pointers in the rgpdstrRights array.
**              [pfnOutputLevelsCallback] -- A pointer to a callback function that provides information to the caller about output restrictions in licenses.
**              [pv]                      -- A void pointer that will be passed through to the callback function.
**              [pDecryptContext]         -- A context that may be used to decrypt data.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if and of the arguments are invalid
**              DRM_E_HEADER_NOT_SET if DRM_MGR_SetV1Header or DRM_MGR_SetV2Header haven't been called
**              DRM_E_LICENSENOTFOUND if a usuable license could not be found 
**              any other return code as defined in drmresults.h
**
** Notes:       If DRM_MGR_Bind fails with DRM_E_LICENSENOTFOUND the caller should call DRM_MGR_GenerateLicenseChallenge
**              and attempt to acquire a license if that functionality is available to the device.
**              If success the caller should call DRM_MGR_Commit to get a decryption context.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_Bind(
    IN       DRM_MANAGER_CONTEXT         *pDrmContext,
    IN const DRM_CONST_STRING            *rgpdstrRights[], /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD                    cRights,
    IN       DRMPFNOUTPUTLEVELSCALLBACK   pfnOutputLevelsCallback,
    IN const DRM_VOID                    *pv,
       OUT   DRM_MANAGER_DECRYPT_CONTEXT *pDecryptContext );


/*****************************************************************************
** Function: DRM_MGR_GetSourceID
**
** Synopsis: This function should be called after DRM_MGR_Bind returns successfully. 
**           It returns the SourceID in the DRM_MANAGER_CONTEXT that was set 
**           in DRM_MGR_Bind.
** Arguments:
**           IN  *f_pDrmContext    -- initialized DRM_MANAGER context 
**           OUT *f_pdwSourceID    -- pointer to return the source ID
** Returns:
**           DRM_SUCCESS           -- On success
**           DRM_E_INVALIDARG      -- if any argument is invalid
** Notes  :  Call this API only after DRM_MGR_Bind() returns successfully
*****************************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetSourceID ( 
    IN     DRM_MANAGER_CONTEXT  *f_pDrmContext,
       OUT DRM_DWORD            *f_pdwSourceID );

#if DRM_SUPPORT_REVOCATION
/*****************************************************************************
** Function: DRM_MGR_GetLicenseRevInfoVersion
**
** Synopsis: This function should be called after DRM_MGR_Bind returns successfully. 
**           It returns the rev info version in the DRM_MANAGER_CONTEXT that was set in 
**           DRM_MGR_Bind.
** Arguments:
**           IN  *f_pDrmContext    -- initialized DRM_MANAGER context 
**           OUT *f_pdwRevInfo
** Returns:
**           DRM_SUCCESS           -- On success
**           DRM_E_INVALIDARG      -- if any argument is invalid
*****************************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetLicenseRevInfoVersion ( 
    IN     DRM_MANAGER_CONTEXT  *f_pDrmContext,
       OUT DRM_DWORD            *f_pdwRevInfo );
#endif

/**********************************************************************
**
** Function:    DRM_MGR_Commit
**
** Synopsis:    Commits all metering and secure store transactions to the store file.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context that has been bound to a license by DRM_MGR_Bind.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if the arguement is invalid
**              any other return code as defined in drmresults.h
**
** Notes:       If this function fails the app ( which may already be decrypting and playing back content ) should stop
**              all content rendering.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_Commit( 
    IN     DRM_MANAGER_CONTEXT         *pDrmContext );

/******************************************************************************
**
** Function :   DRM_MGR_InitDecrypt
** 
** Synopsis :   Initializes state for phased decryption
** 
** Arguments :  [f_pDecryptContext] -- decrypt context 
**              [f_pbLast15]        -- Last 15 bytes of ciphertext
**              [f_cbData]          -- # of bytes in ENTIRE ciphertext              
** 
******************************************************************************/
DRM_RESULT DRM_API DRM_MGR_InitDecrypt(
    IN DRM_MANAGER_DECRYPT_CONTEXT  *f_pDecryptContext,
    IN DRM_BYTE                     *f_pbLast15,
    IN DRM_DWORD                     f_cbData );

/******************************************************************************
** 
** Function :   DRM_MGR_Decrypt
** 
** Synopsis :   Decrypts part of the ciphertext. 
** 
** Arguments :  [pDecryptContext] -- Decrypt context setup in DRM_MGR_Bind
**              [pbData]          -- # of bytes of data to be decrypted
**              [cbData]          -- Pointer to encrypted buffer
** 
** Returns :    
** 
** Notes :      IMPORTANT: Phased decrypt should be called on segments of 
**              encrypted data sequentially, i.e. if encrypted data is divided 
**              four segments, the order of decryption should be segment 1,
**              followed by 2, 3, 4.
**              Content is decrypted in place.  Clear output is the same length 
**              as the encrypted input.
**              To remove complexity we do not handle the case where
**              a packet is split somewhere in the middle of the last 15 bytes.
**              The caller is responsible to ensure that the last 15 bytes never
**              straddle multiple calls to Decrypt.
**
******************************************************************************/
DRM_RESULT DRM_API DRM_MGR_Decrypt(
    IN     DRM_MANAGER_DECRYPT_CONTEXT *pDecryptContext,
    IN OUT DRM_BYTE                    *pbData,
    IN     DRM_DWORD                    cbData );


/**********************************************************************
**
** Function:    DRM_MGR_GetLicenseData
**
** Synopsis:    Retrieves data about the usability of content.  This is data that is aggregated across all licenses that match
**              the header speficied by DRM_MGR_SetV1Header or DRM_MGR_SetV2Header
**
** Arguments:    
**              [pDrmContext]    -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [rgpdstrRights]  -- An array of strings representing rights that the caller would like license state information about.
**              [rgStateData]    -- Array of DRM_LICENSE_STATE_DATA structures that will hold the results of the queires.
**              [cRightsQueried] -- Number of elements in the rgpdstrRights array and the rgStateData array.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any of the arguments are invalid or
**              DRM_E_HEADER_NOT_SET if a header has not been set or
**              any of the return codes as defined in drmresults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetLicenseData(
    IN       DRM_MANAGER_CONTEXT    *pDrmContext,
    IN const DRM_CONST_STRING       *rgpdstrRights[], /* Array of DRM_CONST_STRING pointers */
       OUT   DRM_LICENSE_STATE_DATA  rgStateData[], /* array of DRM_LICENSE_STATE_DATAs */
    IN       DRM_DWORD               cRightsQueried );


#if DRM_SUPPORT_METERING
/**********************************************************************
**
** Function:    DRM_MGR_GenerateMeterChallenge
**
** Synopsis:    Generates a challenge that includes all the metering information for a specific MAS identified by the
**              metering certificate.
**
** Arguments:   [pDrmContext]    -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pdstrMeterCert] -- Metering certificate that identifies the metering ID that wants to collect data.  This is an XML string.
**              [pwszUrl]        -- user allocated buffer to get URL in WCHAR.
**              [pcchUrl]        -- character count of pwszUrl. Can be NULL if URL is not required.
**              [pbChallenge]    -- Buffer to hold the metering challenge
**              [pcbChallenge]   -- Size in bytes of pbData on input.  Number of bytes copied to pbData on return or length required if the buffer was too small.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any of the parameters are invalid or
**              any of the return codes as defined in drmresults.h
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_GenerateMeterChallenge(
    IN        DRM_MANAGER_CONTEXT *pDrmContext,
    IN  const DRM_CONST_STRING    *pdstrMeterCert,
       OUT    DRM_WCHAR           *pwszUrl,
    IN OUT    DRM_DWORD           *pcchUrl,
       OUT    _XMBContext         *pbChallenge,
    IN OUT    DRM_DWORD           *pcbChallenge);

/**********************************************************************
**
** Function:    DRM_MGR_ProcessMeterResponse
**
** Synopsis:    A meter repsonse is sent to the device after a sever process the challenge.  This response is handled
**              by DRM here to possible reset the metering data just reported.
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pbData]      -- Input buffer containing the response to process.
**              [cbData]      -- Count of bytes in pbData
**              [pfFlagsOut]  -- Status output flags.  Either 0 or DRM_METER_RESPONSE_PARTIAL
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if the argument is invalid or
**              any return code as defined in drmresults.h
**
** Notes:       If DRM_METER_RESPONSE_PARTIAL is returned in pfFlagsOut it indicates that the challenge/response
**              round trip was successful but only a partial amout of the entire metering data was sent.  The application
**              should perform the entire DRM_MGR_GenerateMeterChallenge/DRM_MGR_ProcessMeterReponse cycle again.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_ProcessMeterResponse(
    IN      DRM_MANAGER_CONTEXT  *pDrmContext,
    IN      DRM_BYTE             *pbData,
    IN      DRM_DWORD             cbData,
       OUT  DRM_DWORD            *pfFlagsOut);
#endif   /* DRM_SUPPORT_METERING  */


#define DRM_SYNC_IGNORE_THRESHOLD_PARAMETER    0xFFFFFFFF

#if DRM_SUPPORT_LICENSE_SYNC
/**********************************************************************
**
** Function:    DRM_MGR_GenerateSyncChallenge
**
** Synopsis:    Creates a license synchronization challenge in multiple session.
**
** Arguments:   [pDrmContext]  -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [cMaxCount]    -- maximum remaining playcount before license expiration to use evaluating licenses with
**                                count-based expirations.  Use DRM_SYNC_IGNORE_THRESHOLD_PARAMETER (0xFFFFFFFF) to
**                                ignore this parameter and always sync count-based licenses
**              [cMaxHours]    -- maximum number of hours before license expiration to use evaluating licenses with
**                                date-based expirations.  Use DRM_SYNC_IGNORE_THRESHOLD_PARAMETER (0xFFFFFFFF) to
**                                ignore this parameter and always sync date-based licenses
**              [iStart]       -- starting index for challenge
**              [cItemsToProcess]   -- total number items to be processed 
**              [piNextStart]       -- Starting index for next challenge 
**              [pcProcessed]       -- total number of item processed

**              [pbData]       -- user-allocated buffer to be used as output buffer
**              [pcbData]      -- size of pbChallenge
**
** Returns:     DRM_SUCCESS on success. On failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**
** Notes:       To get the minimum size required, first call with pbChallenge = NULL and
**              *pcbChallenge =0.  This will return DRM_E_BUFFERTOOSMALL and the pcbChallenge 
**              will point to the required buffer size.
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_GenerateSyncChallenge(
    IN     DRM_MANAGER_CONTEXT *pDrmContext,
    IN     DRM_DWORD            cMaxCount,
    IN     DRM_DWORD            cMaxHours,
    IN     DRM_DWORD            iStart,
    IN     DRM_DWORD            cItemsToProcess,
       OUT DRM_DWORD           *piNextStart,
       OUT DRM_DWORD           *pcProcessed,
       OUT _XMBContext         *pbData,
    IN OUT DRM_DWORD           *pcbData);


#endif /* DRM_SUPPORT_LICENSE_SYNC */

#if DRM_SUPPORT_SECURE_CLOCK
/**********************************************************************
**
** Function:    DRM_MGR_ClkProcessResponse
**
** Synopsis:    Process Secure clockrequest response received from server
**
** Arguments:   [pDrmContext] -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pbResponse]  -- Response string received from server
**              [cbResponse]  -- size of pbResponse in bytes.
**              [pResult]     -- pointer to DRM_RESULT to get error from server included in response
**
** Returns:     DRM_SUCCESS on success, or failed with 
**              DRM_E_INVALIDARG,
**              DRM_E_CLK_INVALID_RESPONSE
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_ClkProcessResponse(
    IN     DRM_MANAGER_CONTEXT *pDrmContext,
    IN     DRM_BYTE            *pbResponse,
    IN     DRM_DWORD            cbResponse,
       OUT DRM_RESULT          *pResult);

/**********************************************************************
**
** Function:    DRM_MGR_ClkGenerateChallenge
**
** Synopsis:    Creates a Secure Clock challenge.
**
** Arguments:   [pDrmContext]  -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pwszUrl]      -- user allocated buffer to get URL in WCHAR. Null Terminated.
**              [pcchUrl]      -- character count of pwszUrl. Can be NULL if URL is not required.
**              [pbChallenge]  -- user allocated buffer to be used as output buffer
**              [pcbChallenge] -- size of pbChallenge
**
** Returns:     DRM_SUCCESS on success. On failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**
** Notes:       To get the minimum size required, first call with pbChallenge=NULL,
**              *pcbChallenge =0.  
**              This will return DRM_E_BUFFERTOOSMALL and pcchUrl and pcbChallenge 
**              will point to a value of minimum context size.
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_ClkGenerateChallenge(
    IN     DRM_MANAGER_CONTEXT  *pDrmContext,
       OUT DRM_WCHAR            *pwszUrl,
    IN OUT DRM_DWORD            *pcchUrl,
       OUT DRM_BYTE             *pbChallenge,
    IN OUT DRM_DWORD            *pcbChallenge );

/**********************************************************************
**
** Function:    DRM_MGR_GetSecureClock
**
** Synopsis:    Get Secure clock state
**
** Arguments:   [pDrmContext]             -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pwszSecureTime]          -- pointer to WCHAR buffer to get Secure time. Optional 
**              [pcchSecureTime]          -- size of buffer in WCHAR. Optioanl
**              [pdwFlags]                -- Pointer to DWORD. Optional
**              [pbSecureTimeWithStatus]  -- pointer to BYTE buffer to get Secure time and flag status in XML format. Optional 
**              [pcbSecureTimeWithStatus] -- size of buffer. Optioanl
**
** Returns:     DRM_SUCCESS on success. 
**
** Notes:       There are 2 forms in which the secure clock data is being returned. Either in
**              poSecureTime and pdwFlags or in pbSecureTimeWithStatus. The second one in in XML format
**              which contains both information as in poSecureTime and pdwFlags. You can choose either format.
**              To get minimum buffer size, this API should be called with *pcchSecureTime =0 (or *pcbSecureTimeWithStatus =0)
**              This will return DRM_E_BUFFERTOOSMALL and pcchSecureTime and pcbSecureTimeWithStatus 
**              will point to a value of minimum context size.
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetSecureClock(
    IN     DRM_MANAGER_CONTEXT  *pDrmContext,
    IN     DRM_WCHAR            *pwszSecureTime,
    IN OUT DRM_DWORD            *pcchSecureTime, 
    IN     DRM_DWORD            *pdwFlags,
    IN     DRM_BYTE             *pbSecureTimeWithStatus,
    IN OUT  DRM_DWORD           *pcbSecureTimeWithStatus  );

#endif /* DRM_SUPPORT_SECURE_CLOCK */



#if DRM_SUPPORT_ENCRYPTION
/**********************************************************************
**
** Function:    DRM_MGR_CreateEncryptContext
**
** Synopsis:    Creates an encryption context that can be used to encrypt clear text data with the DRM content encryption
**              algorithm.  A device that wants to create DRM protected content would use this to do so.
**
** Arguments:   [rgbKey]          -- The content key the caller wants the content encrypted with.
**              [pEncryptContext] -- A context that will be initialzied internally with the content key.
**
** Returns:     DRM_SUCCESS on success or 
**              DRM_E_INVALIDARG if any of the arguments are invalid or
**              any return code as defined in drmresults.h
**
** Notes:       This is very light wieght.  Content can be encrypted without call DRM_MGR_Initialize
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_CreateEncryptContext(
    IN const DRM_BYTE                     rgbKey[__CB_DECL(DRMCIPHERKEYLEN)],
       OUT   DRM_MANAGER_ENCRYPT_CONTEXT *pEncryptContext );

/**********************************************************************
**
** Function:    DRM_MGR_Encrypt
**
** Synopsis:    Encrypts cleartext content using the DRM content encryption algorithm and the given encryption context.
**
** Arguments:   [pEncryptContext] -- Encryption context previously initialized by DRM_MGR_CreateEncryptContext.
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
DRM_RESULT DRM_API DRM_MGR_Encrypt(
    IN     DRM_MANAGER_ENCRYPT_CONTEXT *pEncryptContext,
    IN OUT DRM_BYTE                    *pbData,
    IN     DRM_DWORD                    cbData );
#endif  /* DRM_SUPPORT_ENCRYPTION */


/**********************************************************************
**
** Function:    DRM_MGR_CleanupLicenseStore
**
** Synopsis:    Instructs DRM to delete all unusable licenses from the license store and perform maintaince functions 
**              on the data store file.
**
** Arguments:   [pDrmContext]        -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pvCallerData]       -- Opaque context data to be funnelled back to caller during callback. Could be NULL.
**              [dwCallbackInterval] -- % completion after which Callback (if any) should be invoked. 
**                                      e.g. if dwCallbackInterval = 5, callback will be invoked after 5%, 10%, 15%...completion
**              [pfnCallback]        -- optional caller supplied callback function - can be NULL.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              any return code as defined in drmrestults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_CleanupLicenseStore(
    IN       DRM_MANAGER_CONTEXT     *pDrmContext,
    IN const DRM_VOID                *pvCallerData,
    IN       DRM_DWORD                dwCallbackInterval,
    IN       pfnStoreCleanupProgress  pfnCallback );


/**********************************************************************
**
** Function:    DRM_MGR_GetLicenseState
**
** Synopsis:    Gets License state for the KID included in pdstrQuery. 
**              Currently it tells whether license is usable for Play action.
**
** Arguments:   [pDrmContext]        -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [pdstrQuery]         -- Query containing KID, Action etc. 
**              [pCategory]          -- returns DRM_LICENSE_STATE_CATEGORY. 
**              [pdwReserved1]       -- Reserved for future use. Must be NULL. 
**              [pdwReserved2]       -- Reserved for future use. Must be NULL. 
**              [pdwReserved3]       -- Reserved for future use. Must be NULL. 
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              any return code as defined in drmrestults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetLicenseState(
    IN        DRM_MANAGER_CONTEXT        *pDrmContext,
    IN  const DRM_CONST_STRING           *pdstrQuery,
        OUT   DRM_LICENSE_STATE_CATEGORY *pCategory,
        OUT   DRM_DWORD                  *pdwReserved1,
        OUT   DRM_DWORD                  *pdwReserved2,
        OUT   DRM_DWORD                  *pdwReserved3);

/*****************************************************************************
** Function:    DRM_MGR_GetRevocationList
**
** Synopsis:    open the secure store and get the version number and buffer containing the 
**              revocation list currently stored based upon the type of revocation list
**              specified
**
** Arguments:   [f_pDrmContext]        -- Opaque DRM Manager context initialized by a call to 
**                                        DRM_MGR_Initialize.
**              [f_eRevType]           -- Type of Revocation List and Version to retrieve.
**              [f_pbBuffer]           --
**              [f_pcbBuffer]          -- size of f_pbBuffer, in bytes, returns size of  
**                                        revocation list
**              [f_pdwVersion]         -- Returns version of revocation list.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              DRM_E_PRIVKEYREADERROR if BB context not initialized or
**              DRM_E_FILENOTFOUND if revocation list doesn't exist in secure store or
**              DRM_E_BUFFERTOOSMALL if *f_pcbBuffer smaller than required buffer size to return 
**                                     revocation list or
**              any return code as defined in drmresults.h
*****************************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetRevocationList(
    IN        DRM_MANAGER_CONTEXT        *f_pDrmContext,
    IN        DRM_REVOCATION_TYPE_ENUM    f_eRevType,
    OUT       DRM_BYTE                   *f_pbBuffer,
    IN OUT    DRM_DWORD                  *f_pcbBuffer,
    OUT       DRM_DWORD                  *f_pdwVersion);

/*****************************************************************************
** Function:    DRM_MGR_GetRevocationListVersion
**
** Synopsis:    open the secure store and get the version number for the 
**              revocation list currently stored based upon the type of revocation list
**              specified
**
** Arguments:   [f_pDrmContext]        -- Opaque DRM Manager context initialized by a call to 
**                                        DRM_MGR_Initialize.
**              [f_eRevType]           -- Type of Revocation List and Version to retrieve.
**              [f_pdwVersion]         -- Returns version of revocation list.
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              DRM_E_PRIVKEYREADERROR if BB context not initialized or
**              DRM_E_FILENOTFOUND if revocation list doesn't exist in secure store or
**              any return code as defined in drmresults.h
*****************************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetRevocationListVersion(
    IN        DRM_MANAGER_CONTEXT        *f_pDrmContext,
    IN        DRM_REVOCATION_TYPE_ENUM    f_eRevType,
    OUT       DRM_DWORD                  *f_pdwVersion);

/*****************************************************************************
** Function:    DRM_MGR_GetRevInfo
**
** Synopsis:    open the secure store and get the revocation information structu
re for
**              revocation lists currently stored.
**
** Arguments:   [f_pDrmContext]        -- Opaque DRM Manager context initialized
 by a call to
**                                        DRM_MGR_Initialize.
**              [f_pbRevInfo]          -- Pointer to Revocation Info buffer
**              [f_cbRevInfo]          -- Size of Revocation info buffer
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              DRM_E_PRIVKEYREADERROR if BB context not initialized or
**              DRM_E_FILENOTFOUND if revocation info doesn't exist in secure st
ore or
**              DRM_E_BUFFERTOOSMALL if cbRevinfo isn't large enough to hold rev
 info or
#               DRM_E_NOTIMPL if DRM_SUPPORT_REVOCATION is not defined or
**              any return code as defined in drmresults.h
*****************************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetRevInfo(
    IN        DRM_MANAGER_CONTEXT   *f_pDrmContext,
    IN        DRM_BYTE              *f_pbRevInfo,
    IN  OUT   DRM_DWORD             *f_pcbRevInfo);

/**********************************************************************
**
** Function:    DRM_MGR_GetWMDRMNETMinAppSec
**
** Synopsis:    Gets License minimum application security level.  Must be called
**              after a successful bind via DRM_MGR_Bind and prior to DRM_MGR_Commit.
**
** Arguments:   [f_pDrmContext]        -- Opaque DRM Manager context initialized by a call to DRM_MGR_Initialize
**              [f_pdwMinAppSec]          -- Returns License Chains minimum app security level. 
**
** Returns:     DRM_SUCCESS on success or
**              DRM_E_INVALIDARG if any parameter is invalid or
**              DRM_E_LICENSENOTBOUND if license hasn't been bound by calling DRM_MGR_Bind successfully
**              any return code as defined in drmrestults.h
**
**********************************************************************/
DRM_RESULT DRM_API DRM_MGR_GetWMDRMNETMinAppSec(
    IN        DRM_MANAGER_CONTEXT        *f_pDrmContext,
    OUT       DRM_DWORD                  *f_pdwMinAppSec);

/**********************************************************************
**
** Function:    DRM_MGR_ProcessCommand
**
**
**********************************************************************/


DRM_RESULT DRM_API DRM_MGR_ProcessCommand(
    IN       DRM_MANAGER_CONTEXT   *f_pcontextMGR,
    IN       DRM_DWORD              f_dwOperationCode,
    IN       DRM_DWORD              f_dwRequestArgument1,
    IN       DRM_DWORD              f_dwRequestArgument2,
    IN       DRM_DWORD              f_dwRequestArgument3,
    IN       DRM_DWORD              f_dwRequestArgument4,
    IN       DRM_BYTE              *f_pbRequestData,
    IN       DRM_DWORD              f_dwRequestDataLength,
        OUT  DRM_DWORD             *f_pdwResponseResult1,
        OUT  DRM_DWORD             *f_pdwResponseResult2,
        OUT  DRM_DWORD             *f_pdwResponseResult3,
        OUT  DRM_DWORD             *f_pdwResponseResult4);

/**********************************************************************
**
** Function:    DRM_MGR_ProcessRequest
**
**
**********************************************************************/

DRM_RESULT DRM_API DRM_MGR_ProcessRequest( 
    IN       DRM_MANAGER_CONTEXT   *f_pcontextMGR,
    IN       DRM_DWORD              f_dwOperationCode,
    IN       DRM_DWORD              f_dwRequestArgument1,
    IN       DRM_DWORD              f_dwRequestArgument2,
    IN       DRM_DWORD              f_dwRequestArgument3,
    IN       DRM_DWORD              f_dwRequestArgument4,
        OUT  DRM_DWORD             *f_pdwResponseResult1,
        OUT  DRM_DWORD             *f_pdwResponseResult2,
        OUT  DRM_DWORD             *f_pdwResponseResult3,
        OUT  DRM_DWORD             *f_pdwResponseResult4,
        OUT  DRM_BYTE              *f_pbResponseData,
        OUT  DRM_DWORD             *f_pdwResponseDataLength);




#ifdef __cplusplus      
}
#endif

#endif /* __DRMMANAGER_H__ */

