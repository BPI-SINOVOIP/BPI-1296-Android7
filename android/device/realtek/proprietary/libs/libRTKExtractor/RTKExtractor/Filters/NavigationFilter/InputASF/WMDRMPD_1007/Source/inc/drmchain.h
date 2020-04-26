/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CHAIN_H__
#define __DRM_CHAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DRM_MAX_COPY_OPL_DATA_SIZE (1024*4)
#define DRM_MAX_PLAY_OPL_DATA_SIZE (128)

typedef DRM_RESULT (DRM_API *pfnCheckDeviceRevocation)(DRM_LICEVAL_CONTEXT* pLicEvalContext, DRM_VOID *pv );

#define DRM_POLICY_FLAG_ONLINE_DISABLED              0x00000001
#define DRM_POLICY_FLAG_METERING_DISABLED_ENTERPRISE 0x00000002

#if DRM_SUPPORT_COPY_OPL
typedef struct __tagCOPY_OPL_CHAIN
{
    DRM_BYTE     rgbCopyOPLBuffer[__CB_DECL(DRM_MAX_COPY_OPL_DATA_SIZE)];
    DRM_COPY_OPL oplCopy;
} COPY_OPL_CHAIN;
#endif

typedef struct __tagPLAY_OPL_CHAIN
{
    DRM_BYTE     rgbPlayOPLBuffer[__CB_DECL(DRM_MAX_PLAY_OPL_DATA_SIZE)];
    DRM_PLAY_OPL oplPlay;
} PLAY_OPL_CHAIN;


typedef struct _FFLICENSE
{
    IN     DRM_LICSTORE_CONTEXT     *pLicStore; /* Context to an Open license store */
           DRM_LICSTOREENUM_CONTEXT *pLicStoreEnum; /* Enumerators for all depths, [0] should be initialized */
    IN OUT DRM_SECSTORE_CONTEXT     *pSecStoreLicense;
    IN     DRM_LICEVAL_CONTEXT      *pLicEval;
    IN     DRM_V1Header             *pV1Header;
       OUT DRM_BINDING_INFO         *pBindingInfo; /* This should point to an array of DRM_MAX_LICENSE_CHAIN_DEPTH binding infos. */
    IN     DRM_BYTE                 *pbBuffer;     /* Working buffer */
    IN     DRM_DWORD                 cbBuffer;
    IN     DRM_DWORD                 dwChainDepth; /* The current depth of the chain.  If we are looking for the first license this is 0 */
    IN     DRM_DWORD                 dwPolicyFlags; /* bitmask of policy flags */
    IN     DRM_BYTE                  rgbRevocationPassword [__CB_DECL(SHA_DIGEST_LEN)];
           DRM_BYTE                  rgbLicensePassword    [__CB_DECL(SHA_DIGEST_LEN)];    
       OUT DRM_BOOL                  fSecStoreOpen         [DRM_MAX_LICENSE_CHAIN_DEPTH]; /* On function exit the caller should close the stores if they are open */
       OUT DRM_BOOL                  rgfDeleteLicense      [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_KID                   rgkid                 [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_LID                   rglid                 [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_HDS_SLOT_HINT         rgslotHint            [DRM_MAX_LICENSE_CHAIN_DEPTH];
#if DRM_SUPPORT_METERING
       OUT DRM_MID                   rgmid                 [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_BOOL                  rgfHasMetering        [DRM_MAX_LICENSE_CHAIN_DEPTH]; /* On function exit the caller should close the stores if they are open */
#endif

#if DRM_SUPPORT_COPY_OPL
       OUT COPY_OPL_CHAIN            rgCopyOpl             [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_BOOL                  rgfCopyOplValid       [DRM_MAX_LICENSE_CHAIN_DEPTH];
#endif
       OUT PLAY_OPL_CHAIN            rgPlayOpl             [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_BOOL                  rgfPlayOplValid       [DRM_MAX_LICENSE_CHAIN_DEPTH];
       OUT DRM_INCLUSION_LIST_CALLBACK_STRUCT inclusionListCallback;

#if DRM_SUPPORT_WMDRMNET       
       OUT DRM_DWORD                 dwMinimumRequiredAppSec;
#endif
#if DRM_SUPPORT_REVOCATION
    IN      DRM_REVLIST_ID_VALUES    idCRLsCurrent;   
        OUT DRM_DWORD                fUpdatedRevocationList;
#endif
    IN      DRM_BOOL                 fCanBind;
        OUT DRM_DWORD                dwSourceid;
#if DRM_SUPPORT_REVOCATION
        OUT DRM_DWORD                dwLicRevInfoVer;
#endif
} DRM_FFLICENSE;

/* This function is recursive!!! 
   INPUTS -- stuff in DRM_FFLICENSE struct
   OUTPUT -- array of binding info structures to pass to CreateDecryptor
             Array of open secure store contexts 
             Array of KID/LID pairs that were used in the chain
  Caller should call CreateDecryptor when ready 
  Will have to read the license in again. 
  Caller should then read each license in and do the DoneWithOperation for each license
  Caller should then update metering data for each license */

DRM_RESULT DRM_API DRM_LIC_CompleteLicenseChain(
    IN OUT   DRM_FFLICENSE              *pFFLicense,
    IN const DRM_CONST_STRING           *rgpdstrRights[], /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD                   cRights,
       OUT   DRM_DWORD                  *pdwChainDepth,
    IN       DRM_HDS_CONTEXT            *pcontextHDS,
    IN       DRMPFNOUTPUTLEVELSCALLBACK  pfnOutputRestrictionsCallback,
    IN const DRM_VOID                   *pv );

DRM_RESULT DRM_API DRM_LIC_ReportActions(
    IN OUT   DRM_FFLICENSE         *pFFLicense,
    IN const DRM_CONST_STRING      *rgpdstrRights[], /* Array of DRM_CONST_STRING pointers */
    IN       DRM_DWORD              cRights,
    IN       DRM_DWORD              dwChainDepth,
    IN       DRM_HDS_CONTEXT       *pcontextHDS);

#if DRM_SUPPORT_ANTIROLLBACK_CLOCK

DRM_RESULT DRM_API DRM_LIC_CheckClockRollback(  
    IN DRM_LICEVAL_CONTEXT      *pLicEval,
    IN DRM_LICSTORE_CONTEXT     *pLicStore,
    IN DRM_LICSTOREENUM_CONTEXT *pLicEnum,
    IN DRM_BYTE                 *pbBuffer,
    IN DRM_DWORD                 cbBuffer,
    IN DRM_HDS_CONTEXT          *pcontextHDS);

#endif /* DRM_SUPPORT_ANTIROLLBACK_CLOCK  */

#ifdef __cplusplus
}
#endif

#endif  /* __DRM_CHAIN_H__ */
