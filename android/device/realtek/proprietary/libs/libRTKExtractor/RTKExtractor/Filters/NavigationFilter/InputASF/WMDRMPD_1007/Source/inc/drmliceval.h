/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMLICEVAL_H__
#define __DRMLICEVAL_H__

#include <drmsecurestore.h>
#include <drmexpreval.h>
#include <drmblackbox.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Flags for use with SetLicense method */
enum
{    
    LICEVAL_VERIFY_CERT_WITH_EXPIRYCHECK    = 1,
    LICEVAL_VERIFY_CERT_WITHOUT_EXPIRYCHECK = 2,
    LICEVAL_VERIFY_SIGNATURE                = 4,
    LICEVAL_VERIFY_IGNORE_VERIFICATION      = 8
};


#define LICEVAL_STORE_CHECK_SETTINGS             (LICEVAL_VERIFY_CERT_WITH_EXPIRYCHECK|LICEVAL_VERIFY_SIGNATURE) 
#define LICEVAL_OPEN_CHECK_SETTINGS              (LICEVAL_VERIFY_CERT_WITHOUT_EXPIRYCHECK|LICEVAL_VERIFY_SIGNATURE)

typedef struct __tagDRM_APPCERT_INFO
{
    DRM_DWORD appSec;
    DRM_DWORD appcd_subject;
} DRM_APPCERT_INFO;

typedef enum
{
    LICEVAL_NOREFERENECE                    = 0,
    LICEVAL_MACHINEDATETIMEREFERENCED       = 1,
    LICEVAL_GRACEPERIODREFERENCED           = 2
}eDRM_LICEVAL_TIMEBASED_STATE;

typedef enum
{    
    eDRM_LIC_CACHED_ATTRIB_KID = 0,
    eDRM_LIC_CACHED_ATTRIB_LID,
    eDRM_LIC_CACHED_ATTRIB_CONTENTPUBKEY,
    eDRM_LIC_CACHED_ATTRIB_METERING_ID,
    eDRM_LIC_CACHED_ATTRIB_CHAINEDKID,
    eDRM_LIC_CACHED_ATTRIB_LAST
} eDRM_LIC_CACHED_ATTRIBS;

typedef struct tagDRM_CACHED_EVENT
{
    DRM_CONST_STRING    dstrType;
    DRM_CONST_STRING    dstrAction;
    DRM_CONST_STRING    dstrCondition;
    DRM_CONST_STRING    dstrRestriction;
} DRM_CACHED_EVENT;

#if DRM_SUPPORT_REVOCATION
/*
**  Current revocation list values should be placed in this struct and passed to CompleteChain so licenses can be compared against these
**  values.
*/
typedef struct __tagDRM_REVLIST_ID_VALUES
{
    DRM_DWORD app;
    DRM_DWORD device;
    DRM_DWORD wmdrmnet;
    DRM_DWORD riv;
}DRM_REVLIST_ID_VALUES;
#endif

typedef struct __tagDRM_LICEVAL_CONTEXT
{
    DRM_BB_CONTEXT              *pcontextBBX;
    APPCERT                      appcert;
    EXPR_EVAL_CONTEXT            contextEXPR;
    DRM_CONST_STRING             dstrContentHeader;
    DRM_CONST_STRING             dstrContentLicense;
    DRM_CONST_STRING             dstrBBVer;
    DRM_CONST_STRING             dstrDRMVer;
    DRM_CONST_STRING             dstrDRMKVer;    
    DRM_SECSTORE_CONTEXT        *pcontextSSTLicense;
    DRM_SECSTORE_CONTEXT        *pcontextSSTGlobal;
    DRM_SECSTORE_CONTEXT        *pcontextSSTRevocation;
    DRM_LICSTOREENUM_CONTEXT    *pLicStoreEnumContext;
    DRM_APPCERT_INFO             certinfoSDK;
    DRM_APPCERT_INFO             certinfoApp;
    DRM_BOOL                     fAppInfoValid;
    DRM_DWORD                    dwChainDepth;
    DRM_BOOL                     fDeviceRegistered;      /* TRUE if the device is registered */
    DRM_LID                      LID;
    DRM_KID                      KID;
    DRM_BOOL                     fGlobalSecStoreWritable;
    DRM_LONG                     lReasonForFail;
    const DRM_CONST_STRING      *pwszPMLicVersion;       /* IN:  Set to a string that represents the PM license version */
    DRM_LONG                     lPMAppSec;              /* IN:  Set to the app sec of the PM */
    DRM_LONG                     lPMRights;              /* This is an OUT param.  If CREATE_PM_LICENSE was queries this will contain the rights bits of the license to create */
    DRMSYSTEMTIME                PMExpiryDate;           /* This is an OUT param.  If CREATE_PM_LICENSE was queries this will contain the expiry date */
    DRM_CONST_STRING             wszDRMKParam;           /* This is an OUT string.  After a license is evaluated this will point to a version string if SAP is required */
    DRM_DWORD                    dwFlags;                /* Default as this LICEVAL_OPEN_CHECK_SETTINGS */
    DRM_BYTE                    *pbRevocationBuffer;     /* User gives a point to work in.  This is actually going to be used for revocation from the secure store */
    DRM_DWORD                    cbRevocationBuffer;     /* Size of the user supplied pointer */    
    DRM_BOOL                     fReserved;              /* Must be initialized to 0 */
    DRM_BOOL                     fSecureTimeDataValid;   /* Is secure time data valid.Must be initialized to 0 */
    DRM_BOOL                     fInGracePeriod;         /* Is in Grace period */
    DRM_UINT64                   u64GPStartTime;         /*Grace period duration*/
    DRM_DWORD                    cPlaylistBurnIncrement; /* OUT:  If playlist burn is supported this will set (if the license also sets it) */
    DRM_HDS_CONTEXT             *pcontextHDS;
    DRM_BOOL                     fIgnoreTimeBoundLicense;
    eDRM_LICEVAL_TIMEBASED_STATE eTimeBasedState;
#if DRM_SUPPORT_REVOCATION
    DRM_REVLIST_ID_VALUES        idCRLsCurrent;
    DRM_BOOL                     fUpdatedRevocationList;
#endif
    DRM_BOOL                     fDeleteLicense;
    
    DRM_BOOL                     fUseCachedAttribs;
    DRM_CONST_STRING rgdstrCachedAttribs[ eDRM_LIC_CACHED_ATTRIB_LAST ]; 
    DRM_CACHED_EVENT rgCachedEvents[ DRM_MAX_ACTIONS ];
    DRM_DWORD        cCachedEvents;
} DRM_LICEVAL_CONTEXT;

typedef enum
{
    DRM_LICENSE_EVAL_SELECT = 1,
    DRM_LICENSE_EVAL_DELETE = 2,
    DRM_LICENSE_EVAL_STORE = 3,
    DRM_LICENSE_EVAL_REPORTCLOCK_ROLLBACK = 4,  /* Only use DRM_LICENSE_EVAL_DONE_WITH_OPERATION for clock rollback. */
    DRM_LICENSE_EVAL_ACTION = 5                 /* With ACTION you must provide an action string */
} eDRM_LICEVAL_OPERATIONS;

typedef enum
{
    DRM_LICENSE_EVAL_CAN_DO_OPERATION = 1,
    DRM_LICENSE_EVAL_DONE_WITH_OPERATION = 2
} eDRM_LICEVAL_OPERATION_STATE;


DRM_RESULT DRM_API DRM_LEVL_PerformOperations(
    IN  DRM_LICEVAL_CONTEXT          *pcontextLicenseEval,
    IN  eDRM_LICEVAL_OPERATIONS       eOperation,
    IN  eDRM_LICEVAL_OPERATION_STATE  eOperationState,
    IN  const DRM_CONST_STRING       *pdstrAction,     /* Only required if DRM_LICENSE_EVAL_ACTION is passed as eOperation */
    OUT DRM_BOOL                     *pfPerform,
    OUT DRM_BOOL                     *pfActionExisted,
    IN  DRM_HDS_CONTEXT              *pcontextHDS);

DRM_RESULT DRM_API DRM_LEVL_EvaluateExpression(
    IN       DRM_LICEVAL_CONTEXT *pcontextLicenseEval,
    IN const DRM_CONST_STRING    *pdstrExpression,
    IN       DRM_BOOL            *pfValue);

DRM_RESULT DRM_API DRM_LEVL_IsLicenseReadyForDeletion( 
    IN  DRM_LICEVAL_CONTEXT *pContext,
    OUT DRM_BOOL            *pfDelete);

DRM_RESULT DRM_LEVL_GetLicenseReasonForUnusable(
    IN       DRM_LICEVAL_CONTEXT    *f_pcontextLicEval,
       OUT   DRM_LONG               *f_plReasonForUnusable );

#ifdef __cplusplus
}
#endif

#endif /* __DRMLICEVAL_H__ */
