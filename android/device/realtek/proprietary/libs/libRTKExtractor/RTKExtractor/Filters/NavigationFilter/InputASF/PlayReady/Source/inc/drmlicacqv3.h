/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_LICACQ_V3_H
#define __DRM_LICACQ_V3_H

#include <drmxmlsig.h>

ENTER_PK_NAMESPACE;

/*
** Define the maximum number of rights
** that can be requested at one time.
*/
#define DRM_MAX_RIGHTS_REQUESTED    1


#if DRM_SUPPORT_REVOCATION
/*
** There are 4 types of revocation data:
** PlayReadyApp, PlayReadyRuntime, WMDRMNET, and REV_INFO v2.
*/
#define MAX_REVLIST_INFO    4
#endif

#if DRM_SUPPORT_REVOCATION
/*
** Structure that stores the version information of
** a particular revocation list.
*/
typedef struct
{
    const DRM_GUID *m_poGUID;

    DRM_DWORD m_dwVersion;
} _REVLIST_INFO;
#endif

/*
** Structure that encapsulates the license state
** information of a uplink KID to be sent to the
** server during license acquisition.
*/
typedef struct
{
    /* KID of the uplink license. */
    DRM_CONST_STRING m_dstrKID;

    /* Raw license state data of the KID above. */
    DRM_LICENSE_STATE_DATA m_oStateData[ DRM_MAX_RIGHTS_REQUESTED ];
} _UPLINK_INFO;

/*
** Structure that encapsulates the all the information
** needed for the license acquisition challenge XML.
** When the structure is first created, it should be
** zeroed to have all of its member fields initialized.
*/
typedef struct
{
    /* The content header. */
    IN DRM_CONST_STRING m_dstrContentHeader;

    /* Total number of rights ( actions ). */
    IN DRM_DWORD m_cRights;

    /*
    ** An array of DRM string pointer that
    ** defines a set of rights ( actions ).
    */
    IN const DRM_CONST_STRING **m_ppdstrRights;

    /* Total number of uplink KIDs. */
    IN DRM_DWORD m_cUplinks;

    /* An array of license state information of uplink KIDs. */
    IN _UPLINK_INFO m_rgoUplinks[ DRM_MAX_LICENSE_CHAIN_DEPTH + 1 ];

    /* A string to contain a B64-encoded uplink KID */
    IN DRM_WCHAR m_rgwchUplinkKID[ CCH_BASE64_EQUIV( SIZEOF( DRM_KID ) ) ];

#if DRM_SUPPORT_REVOCATION
    /* An array of revocation version information. */
    IN _REVLIST_INFO m_rgoRevListInfo[ MAX_REVLIST_INFO ];
#endif

    /* Size ( number of characters ) of a device certificate. */
    OUT DRM_DWORD m_cchDeviceCert;

    /*
    ** Size ( number of characters ) of a collection of domain
    ** certificates to be passed to the server. Please note
    ** it includes the intermediate <CertificateChain>
    ** tags that separate neighboring domain certificates.
    */
    OUT DRM_DWORD m_cchDomainCerts;

    /* The custom data string to be sent to the server. */
    IN DRM_ANSI_CONST_STRING m_dastrCustomData;

    /*
    ** Service ID extracted from the content header. Only those
    ** domain certificates that match the service ID will
    ** be incorporated into the challenge. This field is used
    ** only for domain bound content.
    */
    IN DRM_GUID m_oServiceID;

    /*
    ** Stack allocator used for getting temporary buffers.
    ** User should initialize with a buffer that is big enough.
    */
    IN DRM_STACK_ALLOCATOR_CONTEXT m_oStackAllocatorContext;

    /*
    ** Key information used for XML encryption/signature.
    ** User should call DRM_SOAPXML_InitXMLKey() to initialize.
    */
    IN DRM_XML_KEY m_oXMLKey;

    /* Pointer to a domain store context. */
    IN DRM_DOMAINSTORE_CONTEXT *m_poDomainStoreContext;

    /* Pointer to a black box context. */
    IN DRM_BB_CONTEXT *m_poBBXContext;

    /* Pointer to an OEM specific context. */
    IN DRM_VOID *m_poOEMContext;

    /* Nonce associated with the challenge. */
    DRM_ID m_oNonce;

} DRM_LICCHLG_CONTEXT_V3;

typedef struct
{
    /* Pointer to a license evaluation context. */
    IN DRM_LICEVAL_CONTEXT *m_poLicEvalContext;

    /* Pointer to a license store context. */
    IN DRM_LICSTORE_CONTEXT *m_poLicStoreContext;

    /* Pointer to a sync context. */
    IN DRM_SYNC_CONTEXT *m_poSyncContext;

    /* Pointer to a view rights context. */
    IN DRM_VIEW_RIGHTS_CONTEXT *m_poViewRightsContext;

    /* Pointer to a data store context. */
    IN DRM_DST  *m_poDatastore;

    /* Pointer to a license buffer. */
    IN DRM_BYTE *m_pbLicenseBuffer;

#if DRM_SUPPORT_REVOCATION

    /* Pointer to a black box context. */
    IN DRM_BB_CONTEXT *m_poBBXContext;

    /* Pointer to a revocation store context. */
    IN DRM_REVOCATIONSTORE_CONTEXT *m_poRevStoreContext;

#endif

    /* Pointer to a nonce store context. */
    DRM_NONCESTORE_CONTEXT *m_poNonceStoreContext;

} DRM_LICRESP_CONTEXT_V3;

/* Define the maximum size in bytes of a transaction ID. */
#define DRM_MAX_TRANSACTION_ID  100

/*
** Defines the maximum number of licenses can be contained
** in a single license response.
*/
#define DRM_MAX_LICENSE_ACK     20

/*
** Structure that is used to store the result of
** license processing and storage.
*/
typedef struct
{
    /* KID of the license. */
    DRM_KID m_oKID;

    /* LID of the license. */
    DRM_LID m_oLID;

    /* DRM result code of the processing and storage. */
    DRM_RESULT m_dwResult;

    /* Flag specifying post-processing behavior. */
    DRM_DWORD m_dwFlags;

} DRM_LICENSE_ACK;

/* Values for m_dwFlags */
#define DRM_LICENSE_ACK_FLAGS_EMBED 1


/* Enumeration of license protocols. */
typedef enum
{
    /* Unrecognized protocol. */
    eUnknownProtocol = 0,

    /* V2 protocol. */
    eV2Protocol,

    /* V3 protocol. */
    eV3Protocol

} DRM_LICENSE_PROTOCOL_TYPE;

/* Structure that encapsulates the license processing result. */
typedef struct
{
    /* Type of license protocol detected. */
    DRM_LICENSE_PROTOCOL_TYPE m_eType;

    /* Buffer to store the transaction ID. */
    DRM_BYTE m_rgbTransactionID[ DRM_MAX_TRANSACTION_ID ];
    
    /*
    ** Actual size in bytes of the transaction ID.
    ** Should <= DRM_MAX_TRANSACTION_ID.
    */
    DRM_DWORD m_cbTransactionID;

    /* Array that contains license processing storage results. */
    DRM_LICENSE_ACK m_rgoAcks[ DRM_MAX_LICENSE_ACK ];

    /*
    ** Actual number of element in the array mentioned above.
    ** Should <= DRM_MAX_LICENSE_ACK.
    */
    DRM_DWORD m_cAcks;

    /* Status code parsed from the server response. */
    DRM_RESULT m_dwResult;

} DRM_LICENSE_RESPONSE;

/*
** Internal functions but are called from other files.
*/
DRM_API DRM_RESULT DRM_CALL DRM_LA_ParseServiceID(
    IN  const DRM_CONST_STRING *f_pdstrContentHeader,
    OUT       DRM_GUID         *f_pServiceID );

DRM_API DRM_RESULT DRM_CALL DRM_LA_ParseLicenseAcquisitionURL(
    IN     const DRM_CONST_STRING *f_pdstrContentHeader,
    IN           DRM_BOOL          f_fSilent,
    __out_ecount_opt( *f_pcchURL ) DRM_CHAR *f_pchURL,
    IN OUT       DRM_DWORD        *f_pcchURL );

DRM_API DRM_RESULT DRM_CALL DRM_LA_ProcessRevocationPackage(
    IN DRM_LICRESP_CONTEXT_V3 *f_poLicRespContext,
    IN DRM_ANSI_CONST_STRING *f_pdastrResponse,
    OUT DRM_DWORD *f_pcProcessed );

DRM_API DRM_RESULT DRM_CALL DRM_LA_GenerateChallengeV3(
    IN DRM_LICCHLG_CONTEXT_V3 *f_poLicChlgContext,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE *f_pbChallenge,
    IN OUT DRM_DWORD *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL DRM_LA_CheckForCustomData(
    __in_bcount( f_cbResponse ) const DRM_BYTE *f_pbResponse,
    IN DRM_DWORD f_cbResponse );

DRM_API DRM_RESULT DRM_CALL DRM_LA_ProcessResponseV3(
    IN DRM_LICRESP_CONTEXT_V3 *f_poLicRespContext,
    IN DRM_ANSI_CONST_STRING *f_pdastrResponse,
    __out_bcount( *f_pcbTransactionID ) DRM_BYTE *f_pbTransactionID,
    IN OUT DRM_DWORD *f_pcbTransactionID,
    __out_ecount_part( DRM_MAX_LICENSE_ACK, *f_pcLicAcks ) DRM_LICENSE_ACK *f_poLicAcks,
    OUT DRM_DWORD *f_pcLicAcks );

DRM_API DRM_RESULT DRM_CALL DRM_LA_ProcessLicenseV3(
    IN DRM_LICRESP_CONTEXT_V3 *f_poLicRespContext,
    __in_ecount( f_pdasstrLicense->m_ich + f_pdasstrLicense->m_cch ) DRM_CHAR *f_pszResponse,
    IN DRM_SUBSTRING *f_pdasstrLicense,
    IN DRM_SLKDATA *f_pSLKData,
    OUT DRM_LICENSE_ACK *f_poLicAck );

DRM_API DRM_RESULT DRM_CALL DRM_LA_GenerateAckV3(
    __in_opt DRM_VOID *f_poOEMContext,
    IN DRM_BB_CONTEXT *f_poBBXContext,
    IN DRM_STACK_ALLOCATOR_CONTEXT *f_poStackAllocatorContext,
    IN DRM_LICENSE_RESPONSE *f_poLicenseResponse,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE *f_pbChallenge,
    IN OUT DRM_DWORD *f_pcbChallenge );

/*
** ---------------------------------------
** License acquisition API functions
** ---------------------------------------
*/
DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_GenerateAck(
    IN DRM_APP_CONTEXT *f_poAppContext,
    IN DRM_LICENSE_RESPONSE *f_poLicenseResponse,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE *f_pbChallenge,
    IN OUT DRM_DWORD *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL Drm_LicenseAcq_ProcessAckResponse(
    IN DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount( f_cbResponse ) DRM_BYTE *f_pbResponse,
    IN DRM_DWORD f_cbResponse,
    OUT DRM_RESULT *f_pResult );

EXIT_PK_NAMESPACE;

#endif /* __DRM_LICACQ_V3_H */
