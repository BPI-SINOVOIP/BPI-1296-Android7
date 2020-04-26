/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMSECURECLOCK_H__
#define __DRMSECURECLOCK_H__

ENTER_PK_NAMESPACE;

typedef struct __tagDRM_SECSTORE_CLOCK_DATA
{
    DRM_TID        tid;
    DRM_DWORD      fInGracePeriod; /*Used as a DRM_BOOL. DWORD for compatibility*/
    DRM_UINT64     LastKnownGoodSecureClock;
    DRM_UINT64     LastKnownRealtimeSecureClock;
    DRM_UINT64     LastKnownGracePeriodStartTime;
    DRM_UINT64     RefreshDate;
    DRM_UINT64     ChallengeTime;
    DRM_DWORD      flag; /*One bit for each member. Starting from TID. For data validity*/
} DRM_SECSTORE_CLOCK_DATA;

#define CLK_TID_VALID           0x00000001
#define CLK_IN_GP_VALID         0x00000002
#define CLK_LK_GSC_VALID        0x00000004
#define CLK_LK_RSC_VALID        0x00000008
#define CLK_LK_GP_VALID         0x00000010
#define CLK_REFRESHDATE_VALID   0x00000020
#define CLK_CHALLENGETIME_VALID 0x00000040
#define CLK_EVER_SET            0x00000080

DRM_VOID _PrepareSecureClockDataForWriting( DRM_SECSTORE_CLOCK_DATA *pData );
#define _PrepareSecureClockDataForReading _PrepareSecureClockDataForWriting

/* pseudo-hashes to read/write Data in the secure store */

extern DRM_EXPORT_VAR const DRM_ID g_idSData;
extern DRM_EXPORT_VAR const DRM_ID g_idSDataPrev;


/**********************************************************************
** Function:    DRM_CLK_CreateChallenge
** Synopsis:    Creates a Secure Clock challenge.
** Arguments:   
**              [pdstrURL] -- pointer to DevCert struct
**              [rgbSecStoreInPswd] -- Secure store password
**              [pContext] -- pointer to DRM_CRYPTO_CONTEXT
**              [poDatastore] -- pointer to DST_CONTEXT
**              [poSecureStoreContext] -- pointer to Secure Store context
**              [pOEMContext] -- OEM implemented context
**              [pwszUrl] -- user allocated buffer to get URL in WCHAR
**              [pcchUrl] -- character count of pwszUrl. Can be NULL if URL is not required.
**              [pbData] -- user allocated buffer to be used as output buffer
**              [pcbData] -- size of pbData
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
** Notes:       To get the minimum size required, first call with pbData=NULL and
**              pcbData points to value 0. This will return DRM_E_BUFFERTOOSMALL 
**              and pcbData will point to a value of minimum context size.
***********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CLK_CreateChallenge(
    IN const DRM_CONST_STRING        *pdstrURL,
    IN const DRM_BYTE                 rgbSecStoreInPswd [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN       DRM_CRYPTO_CONTEXT      *pContext, 
    IN       DRM_DST                 *poDatastore,
    IN       DRM_SECSTORE_CONTEXT    *poSecureStoreContext,
    IN       DRM_VOID                *pOEMContext,
    OUT      DRM_WCHAR               *pwszUrl,
    IN OUT   DRM_DWORD               *pcchUrl,
    IN       DRM_BYTE                *pbData,
    IN OUT   DRM_DWORD               *pcbData );


/**********************************************************************
** Function:    DRM_CLK_ProcessResponse
** Synopsis:    Process Secure clock request response received from server
** Arguments:   [pDevCert] -- pointer to DevCert struct
**              [pbResponse] -- Response string received from server
**              [cbResponse] -- size of pbResponse in bytes.
**              [rgbSecStoreInPswd] -- Secure store password
**              [pResult] --pointer to DRM_RESULT to get error from server included in response
**              [pContext] --pointer to the DRM_CRYPTO_CONTEXT
**              [poDatastore] -- pointer to DST_CONTEXT
**              [poSecureStoreContext] -- pointer to Secure Store context
**              [poLicEvalContext] -- pointer to LICEVAL_CONTEXT
**              [pOEMContext] -- OEM defined context
** Returns:     DRM_SUCCESS on success, or failed with 
**              DRM_E_INVALIDARG,
**              CLK_INVALID_RESPONSE, and
**              error codes from Licesne store.
**
***********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CLK_ProcessResponse(
    IN const PUBKEY                *ppubkeySecureClockServer,
    IN       DRM_BYTE              *pbResponse,
    IN       DRM_DWORD              cbResponse,
    IN const DRM_BYTE               rgbSecStoreInPswd [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    OUT      DRM_RESULT            *pResult,
    IN       DRM_CRYPTO_CONTEXT    *pContext,
    IN       DRM_DST               *poDatastore,
    IN       DRM_SECSTORE_CONTEXT  *poSecureStoreContext,
    IN       DRM_LICEVAL_CONTEXT   *poLicEvalContext,
    IN       DRM_VOID              *pOEMContext );

/**********************************************************************
** Function:    DRM_CLK_GetSecureClock
** Synopsis:    Gets Secure clock.
** Arguments:   
**              [poSecureTime] -- pointer to WCHAR buffer to get Secure time 
**              [pcchSecureTime] -- size of buffer in WCHAR
**              [pdwFlags] -- pointer to DWORD. 0 not set, 1 set, 2 need refresh
**              [pbSecureTimeWithStatus] -- pointer to BYTE buffer to get Secure time and flag status in XML format. Optional 
**              [pcbSecureTimeWithStatus] -- size of buffer. Optioanl
**              [rgbSecStoreInPswd] -- Secure store password
**              [pContext] -- pointer to DRM_CRYPTO_CONTEXT
**              [poDatastore] -- pointer to DST_CONTEXT
**              [poSecureStoreContext] -- pointer to Secure Store context
**              [pOEMContext] -- OEM implmented context
** Returns:     DRM_SUCCESS on success. 
***********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CLK_GetSecureClock(
    OUT      DRM_WCHAR            *pwszSecureTime,
    IN OUT   DRM_DWORD            *pcchSecureTime, 
    OUT      DRM_DWORD            *pdwFlags,
    IN       DRM_BYTE             *pbSecureTimeWithStatus,
    IN OUT   DRM_DWORD            *pcbSecureTimeWithStatus, 
    IN const DRM_BYTE              rgbSecStoreInPswd [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN       DRM_CRYPTO_CONTEXT   *pContext,
    IN       DRM_DST              *poDatastore,
    IN       DRM_SECSTORE_CONTEXT *poSecureStoreContext,
    IN       DRM_VOID             *pOEMContext );


/**********************************************************************
** Function:    DRM_CLK_InitCheckSecureClock
** Synopsis:    Initializes and checks Secure clock. Called by DRM_Manager_Init
** Arguments:   
**              [rgbSecStoreInPswd] -- Secure store password
**              [pContext] -- pointer to DRM_CRYPTO_CONTEXT
**              [poDatastore] -- pointer to DST_CONTEXT
**              [pfClockset] -- pointer to DRM_BOOL to know whether clock is set.
**              [poSecureStoreContext] -- pointer to Secure Store context
**              [poLicEvalContext] -- pointer to LICEVAL_CONTEXT
**              [pOEMContext] -- OEM implmented context
** Returns:     DRM_SUCCESS on success. 
***********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CLK_InitCheckSecureClock(
    IN const DRM_BYTE              rgbSecStoreInPswd [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN       DRM_CRYPTO_CONTEXT   *pContext,
    IN       DRM_DST              *poDatastore,
    IN OUT   DRM_BOOL             *pfClockset,
    IN       DRM_SECSTORE_CONTEXT *poSecureStoreContext,
    IN       DRM_LICEVAL_CONTEXT  *poLicEvalContext,
    IN       DRM_VOID             *pOEMContext );

EXIT_PK_NAMESPACE;

#endif /*__DRMSECURECLOCK_H__ */
