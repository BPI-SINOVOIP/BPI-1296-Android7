/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCONTEXTSIZES_H__
#define __DRMCONTEXTSIZES_H__

#ifndef DRM_MAX_LICENSE_CHAIN_DEPTH
#define DRM_MAX_LICENSE_CHAIN_DEPTH   2                                /* Total number of licenses in a chain support by this DRM implementation */
#endif
#define MAX_DEVICE_CERT_SIZE          (10*1024)                        /* Device cert and licenses will be staged to share the same buffer       */
#define MAX_DEVICE_CERT_TEMPLATE_SIZE ((MAX_DEVICE_CERT_SIZE*6)/10)    /* 60% of the max devcert                                                 */
#define DRM_MAX_HEADERDATASIZE        (5*1024)                         /* 5K of header data                                                      */
#define DRM_MAX_LICENSESIZE           MAX_DEVICE_CERT_SIZE             /* 10K max license size                                                   */
#define MAX_ACTIONS_LENGTH            150
#define MAX_REVOCATION_LIST_SIZE      (30*1024)                        /* Revocation not supported in the Porting kit */

#include <drmcommon.h>
#include <drmlicense.h>
#include <drmsha1.h>
#include <drmhmac.h>
#include <drmrc4.h>
#include <drmcipher.h>
#include <drmstkalloc.h>
#include <drmdevcert.h>
#include <drmhds_impl.h>
#include <drmhds.h>
#include <drmsecurestore.h>
#include <drmexpreval.h>
#include <drmliceval.h>
#include <drmutilities.h>
#include <drmmetering.h>
#include <drmblackbox.h>
#include <drmlicstore.h>
#include <drmlicacq.h>
#include <drmoutputleveltypes.h>
#include <drmoutputlevels.h>
#include <drmchain.h>


typedef enum __EHeaderInContext
{
    eHeaderNotSet=0,
    eHeaderIsV1,
    eHeaderIsV2,
    eKidOnly
} EHeaderInContext;

typedef struct __tagDRM_MANAGER_CONTEXT_INTERNAL
{
    DRM_DWORD cbHeaderData;
    DRM_DWORD cbKID;

    DRM_BOOL fLicStoreOpen;
    DRM_BOOL fSecStoreGlobalContextOpen;
    EHeaderInContext eHeaderInContext;  

    DRM_KID KID;
    DRM_WCHAR wszBBVerString[VER_STRING_MAX_LEN];

    DRM_BB_CONTEXT oBlackBoxContext;
    DRM_BINDING_INFO oBindInfo[DRM_MAX_LICENSE_CHAIN_DEPTH];

    DRM_LICSTORE_CONTEXT oLicStoreContext;
    DRM_LICSTOREENUM_CONTEXT oLicEnum[DRM_MAX_LICENSE_CHAIN_DEPTH];

    DRM_VIEW_RIGHTS_CONTEXT oViewRightsContext;
    DRM_LICEVAL_CONTEXT     oLicEvalContext;    
    DRM_SECSTORE_CONTEXT    oSecStoreGlobalContext;
    
    DRM_DWORD            dwChainDepth;
    DRM_SECSTORE_CONTEXT rgcontextSST   [DRM_MAX_LICENSE_CHAIN_DEPTH];
#if DRM_SUPPORT_CERTIFICATE_CACHING
    DRM_SECSTORE_CONTEXT contextSSTCertCache;
#endif

#if DRM_SUPPORT_LICENSE_SYNC
    DRM_SYNC_CONTEXT contextSync;
#endif

#if DRM_SUPPORT_REVOCATION
    DRM_REVLIST_ID_VALUES        idCRLsCurrent;
#endif
    DRM_BYTE            *pbRevocationBuffer;
    DRM_DWORD            cbRevocationBuffer;

    DRM_FFLICENSE        oFFLicense;
#if DRM_SUPPORT_DLA
    DRM_LICENSE_CHALLENGE licensechallenge;
#endif
    DRM_HDS_CONTEXT oHdsContext;
    DRM_BOOL        fStoreOpened;
    DRM_DWORD       cchDeviceStoreName;
    /* These next 4 buffers can be used independently or together in pieces.  Keep them flush so they
       have maximum raw bytes usuable.  See ProcessResponse */
    DRM_BYTE        rgbDRMHeaderData[__CB_DECL(DRM_MAX_HEADERDATASIZE)];
    DRM_BYTE        rgbDRMLicense   [__CB_DECL(DRM_MAX_LICENSESIZE)];
    DRM_BOOL        fClockSet;
    DRM_BOOL        fBindThenCommit;
    DRM_DWORD       dwSourceid;
#if DRM_SUPPORT_REVOCATION
    DRM_DWORD       dwLicRevInfoVer;
#endif
} DRM_MANAGER_CONTEXT_INTERNAL;

#define DRM_DRMMANAGER_CONTEXT_BUFFER_SIZE SIZEOF(DRM_MANAGER_CONTEXT_INTERNAL)
#define DRM_BINDCONDITIONS_BUFFER_SIZE     SIZEOF(DRM_BIND_CONDITIONS_INTERNAL)      

#endif /* __DRMCONTEXTSIZES_H__ */
