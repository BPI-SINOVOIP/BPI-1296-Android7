/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCRL_H__
#define __DRMBCRL_H__

#include <drmcommon.h>
#include <drmtypes.h>
#include <drmlicense.h>
#include <drmeccp256.h>
#include <drmcontextsizes.h>

ENTER_PK_NAMESPACE;

#define PLAYREADY_DRM_BCRL_SIGNATURE_TYPE 1

typedef DRM_BYTE DRM_RevocationEntry[32]; 
typedef DRM_BYTE DRM_CrlIdentifier[16];

typedef struct __tagDRM_BCRL
{    
    DRM_CrlIdentifier Identifier; 
    DRM_DWORD dwVersion; 

    DRM_DWORD cRevocationEntries;
    DRM_RevocationEntry* Entries;

} DRM_BCRL;

/*
** The following byte value should be equal to the largest signature length supported
*/
#define DRM_BCRL_MAX_SIGNATURE_DATA_LENGTH   SIZEOF( SIGNATURE_P256 )

typedef struct _tagDrmBCrlSignatureData
{   
    DRM_BYTE    type;
    DRM_WORD    cb;   
    DRM_BYTE    rgb[ __CB_DECL( DRM_BCRL_MAX_SIGNATURE_DATA_LENGTH ) ];
} DRM_BCRL_SIGNATURE_DATA;

typedef struct __tagDRM_BCRL_Signed
{    
    DRM_BCRL Crl;
    DRM_BCRL_SIGNATURE_DATA Signature;
    DRM_BYTE* pbCertificateChain;
    DRM_DWORD cbCertificateChain;
} DRM_BCRL_Signed;

/*********************************************************************
**
**  Used to validate the signature on instances of a PlayReady 
**  Binary CRL or RevInfoV2, both of which use the same type of
**  certificate chain and signature structure.
**
*********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_BCrl_VerifySignature (
                 __in_bcount(f_cbSignedBytes) const DRM_BYTE               *f_pbSignedBytes,
                 __in                               DRM_DWORD               f_cbSignedBytes,
                 __in_bcount(f_cbSignature)         DRM_BYTE               *f_pbSignature,
                 __in                               DRM_DWORD               f_cbSignature,
                 __in_bcount(f_cbCertificateChain)  DRM_BYTE               *f_pbCertificateChain,
                 __in                               DRM_DWORD               f_cbCertificateChain,
                 __in_ecount(1)               const PUBKEY_P256            *f_pRootPubkey,
                 __in                               DRM_CRYPTO_CONTEXT     *f_pCryptoCtx,
                 __in_opt                           DRM_BB_CONTEXT         *f_pcontextBBX,
                 __in_opt                           DRM_SECSTORE_CONTEXT   *f_pcontextSST,
                 __in_opt                           DRM_DST                *f_pDatastore );

/*********************************************************************
**
**  Parses Playready/Silverlight CRL from binary to data structure.
**  Verifies signature and certificate chain.
**
**  NOTE: The parser does not make copies of the DRM_RevocationEntry
**        data, it just points to them in the f_pbCrlData buffer so
**        you cannot free the f_pbCrlData and still have a valid f_poCrl
**        data structure.
**
*********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_BCrl_ParseCrl(
 __in_bcount(f_cbCrlData) const DRM_BYTE *f_pbCrlData,
 __in                     const DRM_DWORD f_cbCrlData,
 __out                    DRM_BCRL_Signed *f_poCrl,
 __in_ecount(1)           const PUBKEY_P256  *f_pRootPubkey,
 __inout                  DRM_CRYPTO_CONTEXT *f_pCryptoCtx,
 __in_opt                 DRM_BB_CONTEXT     *f_pcontextBBX,
 __in_opt                 DRM_SECSTORE_CONTEXT *f_pcontextSST,
 __in_opt                 DRM_DST            *f_pDatastore
);

/*********************************************************************
**
**  Checks to see if the given certificate is in the given CRL file.
**  Internally the API uses the DRM_BCrl_ParseCrl method to parse
**  and verify the CRL.  Then it walks through the entries in the CRL
**  to see if the given certificate is revoked.  The API returns
**  DRM_E_CERTIFICATE_REVOKED if any certificate in the chain is revoked.
**
*********************************************************************/


DRM_API DRM_RESULT DRM_CALL DRM_BCrl_CheckIfPlayReadyCertRevoked
(
    __in_bcount(f_cbPlayReadyCert)const DRM_BYTE            *f_pbPlayReadyCert,
    __in                                DRM_DWORD           f_cbPlayReadyCert,
    __in_bcount(f_cbPlayReadyCRL) const DRM_BYTE           *f_pbPlayReadyCRL,
    __in                                DRM_DWORD           f_cbPlayReadyCRL,
    __out_opt                           DRM_SHA256_Digest  *f_pRevokedCertDigest,
    __out_opt                           DRM_DWORD          *f_pdwPlayReadyCRLVer,
    __inout                             DRM_CRYPTO_CONTEXT *f_pCryptoCtx,
    __in_opt                            DRM_BB_CONTEXT     *f_pcontextBBX,
    __in_opt                            DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    __in_opt                            DRM_DST            *f_pDatastore
);

  
EXIT_PK_NAMESPACE;

#endif /* __DRMBCRL_H__ */


