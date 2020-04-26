/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCONTEXTSIZES_H__
#define __DRMCONTEXTSIZES_H__

/*
** Device cert and licenses will be staged to share the same buffer       
*/
#define MAX_DEVICE_CERT_SIZE          ( 10 * 1024 )                        
#define MAX_DEVICE_CERT_TEMPLATE_SIZE ( ( MAX_DEVICE_CERT_SIZE * 6 ) / 10 )/* 60% of max devcert */
#define DRM_MAX_HEADERDATASIZE        ( 5 * 1024 )                         /* 5K of header data  */
#define DRM_MAX_LICENSESIZE           MAX_DEVICE_CERT_SIZE               /* 10K max license size */
#define MAX_ACTIONS_LENGTH            150
#define MAX_REVOCATION_LIST_SIZE      ( 30 * 1024 )

/* The buffer should be big enough to store a single XMR license. */
#define MAX_NST_BUFFER_SIZE           1024

#include <drmcommon.h>
#include <drmlicense.h>
#include <drmsha1.h>
#include <drmhmac.h>
#include <drmrc4.h>
#include <drmcipher.h>
#include <drmstkalloc.h>
#include <drmdevcert.h>
#include <drmdatastore.h>
#include <drmhds.h>
#include <drmsecurestore.h>
#include <drmexpreval.h>
#include <drmliceval.h>
#include <drmutilities.h>
#include <drmmeterimp.h>
#include <drmblackbox.h>
#include <drmlicstore.h>
#include <drmlicacq.h>
#include <drmoutputleveltypes.h>
#include <drmoutputlevels.h>
#include <drmchain.h>
#include <drmplayreadyobj.h>

#if DRM_SUPPORT_DOMAINS
#include <drmdomainstore.h>
#endif /* DRM_SUPPORT_DOMAINS */

#include <drmnoncestore.h>

typedef enum _DRM_CONTENT_SET_PROPERTY
{
    DRM_CSP_HEADER_NOT_SET    = 0,
    DRM_CSP_V1_HEADER         = 1,
    DRM_CSP_V2_HEADER         = 2,
    DRM_CSP_KID               = 3,
    DRM_CSP_V2_4_HEADER       = 5,
    DRM_CSP_V4_HEADER         = 6,
    DRM_CSP_AUTODETECT_HEADER = 7,
    DRM_CSP_PLAYREADY_OBJ     = 8

} DRM_CONTENT_SET_PROPERTY;

typedef enum _DRM_CONTENT_GET_PROPERTY
{
    DRM_CGP_HEADER_KID        = 0,
    DRM_CGP_HEADER_TYPE       = 1,
    DRM_CGP_HEADER            = 2,
    DRM_CGP_PLAYREADY_OBJ     = 3,
    DRM_CGP_CIPHER_TYPE       = 4

} DRM_CONTENT_GET_PROPERTY;

typedef struct __tagDRM_APP_CONTEXT_INTERNAL
{
    DRM_DWORD                   cbKID;

    DRM_BOOL                    fLicStoreOpenXML;
    DRM_BOOL                    fLicStoreOpenXMR;
    DRM_BOOL                    fSecStoreGlobalContextOpen;
    DRM_CONTENT_SET_PROPERTY    eHeaderInContext;  
    DRM_BOOL                    fCanBind;

    DRM_BOOL                    fResponseSignatureChecked;
    DRM_DWORD                   dwLastResponseProtocol;

    DRM_VOID                   *pOEMContext;

    DRM_KID                     KID;
    DRM_WCHAR                   wszBBVerString[VER_STRING_MAX_LEN];

    DRM_BB_CONTEXT              oBlackBoxContext;
    DRM_BINDING_INFO            oBindInfo[DRM_MAX_LICENSE_CHAIN_DEPTH];    
    DRM_XMR_LICENSE             licenseXMR[DRM_MAX_LICENSE_CHAIN_DEPTH];

    DRM_LICSTORE_CONTEXT        oLicStoreContextXML;
    DRM_LICSTORE_CONTEXT        oLicStoreContextXMR;
    DRM_LICSTORE_CONTEXT        oLicStoreContextEmbedded;
    DRM_LICSTOREENUM_CONTEXT    oLicEnum[DRM_MAX_LICENSE_CHAIN_DEPTH];
    DRM_LICSTOREENUM_MULTIPLE_CONTEXT oLicEnumMultiple[DRM_MAX_LICENSE_CHAIN_DEPTH];

    DRM_VIEW_RIGHTS_CONTEXT     oViewRightsContext;
    DRM_LICEVAL_CONTEXT         oLicEvalContext;    
    DRM_SECSTORE_CONTEXT        oSecStoreGlobalContext;
    
    DRM_DWORD                   dwChainDepth;
    DRM_SECSTORE_CONTEXT        rgcontextSST[DRM_MAX_LICENSE_CHAIN_DEPTH];
#if DRM_SUPPORT_CERTIFICATE_CACHING || DRM_SUPPORT_REVOCATION
    DRM_SECSTORE_CONTEXT        contextSSTCertCache;
#endif

#if DRM_SUPPORT_LICENSE_SYNC
    DRM_SYNC_CONTEXT            contextSync;
#if DRM_SUPPORT_DLA
    DRM_SYNC_ENUM_CONTEXT       contextSyncEnum;
#endif
#endif

#if DRM_SUPPORT_REVOCATION
    DRM_REVLIST_ID_VALUES        idCRLsCurrent;
    DRM_REVOCATIONSTORE_CONTEXT  oRevContext;
#endif
    DRM_BYTE                    *pbRevocationBuffer;
    DRM_DWORD                    cbRevocationBuffer;

    DRM_FFLICENSE                oFFLicense;
#if DRM_SUPPORT_DLA
    DRM_LICENSE_CHALLENGE        licensechallenge;
#endif
#if DRM_SUPPORT_METERING
    DRM_METERCERT_CONTEXT        oMeterCertContext;
#endif
    DRM_DST                      oDatastoreHDS;
    DRM_HDS_CONTEXT              oContextHDS;
    DRM_DST                      oDatastoreEST;
    DRM_EST_CONTEXT              oContextEST;
    DRM_BOOL                     fStoreOpened;
    DRM_DWORD                    cchDeviceStoreName;
    
    union
    {
        /*
        ** These next 2 buffers can be used independently or together in pieces.  
        ** Keep them flush so they have maximum raw bytes usuable.  
        ** See ProcessResponse 
        */
        struct
        {
            DRM_BYTE             rgbDRMHeaderData[__CB_DECL( DWORD_ALIGNED_SIZE( DRM_MAX_PLAYREADYOBJSIZE ) )];
            DRM_BYTE             rgbDRMLicense[__CB_DECL( DWORD_ALIGNED_SIZE( DRM_MAX_LICENSESIZE ) )];
        } oBuffers;
        
        DRM_BYTE                 rgbDRMBuffer[__CB_DECL( DWORD_ALIGNED_SIZE( DRM_MAX_LICENSESIZE ) + DWORD_ALIGNED_SIZE( DRM_MAX_PLAYREADYOBJSIZE ) )];
    } oDRMBuffer;
    
    DRM_WCHAR                   *pwszHeaderData;  /* Used with rgbDRMHeaderData: Points into the PRO at the V4 header */
    DRM_DWORD                    cbHeaderData;
    DRM_BYTE                    *pbEmbeddedStore; /* Used with rgbDRMHeaderData: Points into the PRO at the embedded store */
    DRM_DWORD                    cbEmbeddedStore;
    DRM_DWORD                    cbPROData;       /* Used with rgbDRMHeaderData: Specifies the length of the entire PRO if it is set  */
    DRM_DWORD                    cbTotalHeaderData; /* Used with rgbDRMHeaderData: Specifies the total length of the buffer filled with header data or PRO data */

    DRM_BOOL                     fClockSet;
    DRM_BOOL                     fBindThenCommit;
    DRM_DWORD                    dwSourceid;
#if DRM_SUPPORT_REVOCATION
    DRM_DWORD                    dwLicRevInfoVer;
#endif
#if DRM_SUPPORT_DOMAINS
    DRM_DOMAINSTORE_CONTEXT      oDomainStoreContext;
#endif /* DRM_SUPPORT_DOMAINS */

    DRM_LICSTORE_CONTEXT         oLicStoreContextNST;
    DRM_NONCESTORE_CONTEXT       oNonceStoreContext;
    
    DRM_BYTE                     rgbNSTBuffer[ MAX_NST_BUFFER_SIZE ];
} DRM_APP_CONTEXT_INTERNAL;

#define DRM_APP_CONTEXT_BUFFER_SIZE        SIZEOF( DRM_APP_CONTEXT_INTERNAL )
#define DRM_BINDCONDITIONS_BUFFER_SIZE     SIZEOF( DRM_BIND_CONDITIONS_INTERNAL )      

#endif /* __DRMCONTEXTSIZES_H__ */
