/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_UTILITIES_H__
#define __DRM_UTILITIES_H__

#include <drmcontextsizes.h>

ENTER_PK_NAMESPACE;

typedef enum DRM_ROOTPUBKEY_CATEGORY
{
    WM_DRM_ROOTPUBKEY_LICENSES = 0,
    WM_DRM_ROOTPUBKEY_LRB,   
    WM_DRM_ROOTPUBKEY_CLK,
    WM_DRM_ROOTPUBKEY_MTR   
} DRM_ROOTPUBKEY_CATEGORY;

DRM_API DRM_BOOL DRM_CALL DRM_UTL_IsCertDateOK(
    __in_opt    DRM_VOID        *pOEMContext,
    __in_bcount( 4 ) const DRM_BYTE  *f_pbDate);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckCertNoCache(
    IN const CERT               *f_pcert, 
    IN const PUBKEY             *f_ppubkey, 
    IN       DRM_BOOL            f_fCheckDate, 
    IN       DRM_CRYPTO_CONTEXT *f_pcontextCRYP,
    IN       DRM_VOID            *f_pOEMContext );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckCertificate(
    IN  const   CERT                    *f_pcert, 
    IN  const   PUBKEY                  *f_ppubkey, 
    IN          DRM_BOOL                 f_fCheckCertDate,   
    IN          DRM_LICEVAL_CONTEXT     *f_pcontextLEVL,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckCert(
    IN const CERT                *f_pcert, 
    IN const PUBKEY              *f_ppubkey, 
    IN       DRM_BOOL             f_fCheckDate, 
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckDACCert(
    IN const CERT                *f_pcert, 
    IN const PUBKEY              *f_ppubkey, 
    IN       DRM_BOOL             f_fCheckDate, 
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL );

#if DRM_SUPPORT_CERTIFICATE_CACHING
DRM_API DRM_RESULT DRM_CALL  DRM_UTL_AddCachedCertificate (
    IN       DRM_BB_CONTEXT       *f_pcontextBBX,
    IN       DRM_SECSTORE_CONTEXT *f_pcontextSSTRevocation,
    IN       DRM_DST              *f_pDatastore,
    IN const DRM_BYTE             *f_pbData,
    IN       DRM_DWORD             f_cbData );

DRM_API DRM_BOOL DRM_CALL DRM_UTL_VerifyCachedCertificate (
    IN       DRM_BB_CONTEXT       *f_pcontextBBX,
    IN       DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN       DRM_DST              *f_pDatastore,
    IN const DRM_BYTE             *f_pbData,
    IN       DRM_DWORD             f_cbData );
#endif

DRM_API DRM_RESULT DRM_CALL DRM_PK_EncryptLarge( 
    IN       DRM_VOID            *f_pOEMContext,
    IN const PUBKEY              *f_ppubkey,
    __inout_bcount( f_cbClear ) DRM_BYTE *f_pbClear, 
    IN       DRM_DWORD            f_cbClear, 
    __out_bcount( f_cbClear + DRM_ECC160_CIPHERTEXT_LEN ) DRM_BYTE *f_pbCipher, 
    IN       DRM_DWORD            f_cbKeySymmetric, 
    IN       DRM_CRYPTO_CONTEXT  *f_pcontextCRYP);

DRM_API DRM_RESULT DRM_CALL DRM_PK_DecryptLarge( 
    IN const PRIVKEY            *f_ppubkey,
    __inout_bcount( f_cbCipher) DRM_BYTE *f_pbCipher, 
    IN       DRM_DWORD           f_cbCipher, 
    __out_bcount( f_cbCipher - DRM_ECC160_CIPHERTEXT_LEN ) DRM_BYTE *f_pbClear, 
    IN       DRM_CRYPTO_CONTEXT *f_pcontextCRYP);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_GetVersionAsString( 
    __in_bcount( VERSION_LEN ) DRM_BYTE  rgbVersion[__CB_DECL(VERSION_LEN)],
    __out_ecount( VER_STRING_MAX_LEN ) DRM_WCHAR wszVerString [VER_STRING_MAX_LEN]);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_GetVersionFromString(
   __in_ecount( cchVersion ) const DRM_WCHAR  *pwszVersion,
   IN       DRM_DWORD   cchVersion,
   __out_ecount( VERSION_LEN ) DRM_WORD rgwVersion[VERSION_LEN] );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_StringToGuid( 
    IN const DRM_CONST_STRING *pdstrString,
    OUT      DRM_GUID         *pGuid );

#define DRM_GUID_STRING_LEN 38
DRM_API DRM_RESULT DRM_CALL DRM_UTL_GuidToString
( 
    IN      const                           DRM_GUID  *pGuid, 
        __out_ecount(DRM_GUID_STRING_LEN)   DRM_WCHAR  rgwszString[DRM_GUID_STRING_LEN]
);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_StringInsertSubString(
    IN OUT   DRM_STRING       *pdstrString,
    IN       DRM_DWORD         dwInsertPoint,
    IN const DRM_CONST_STRING *pdstrSubString);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_StringRemoveSubString(
    IN OUT   DRM_STRING       *pdstrString,
    IN const DRM_CONST_STRING *pdstrSubString);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_StringInsertBlankSubString(
    IN OUT DRM_STRING *f_pdstrTarget,
    IN     DRM_DWORD   f_ichInsertion,
    IN     DRM_DWORD   f_cch);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_StringInsertBlankSubStringA(
    IN OUT DRM_ANSI_STRING *f_pdastrTarget,
    IN DRM_DWORD f_ichInsertion,
    IN DRM_DWORD f_cch );
    
DRM_API DRM_BOOL DRM_CALL DRM_UTL_DSTRStringsEqual(
    const DRM_CONST_STRING *pdstr1,
    const DRM_CONST_STRING *pdstr2);

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DASTRStringsEqual(
    const DRM_ANSI_CONST_STRING *f_pdastr1,
    const DRM_ANSI_CONST_STRING *f_pdastr2);

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DASSTRStringsEqual(
    __in_ecount( f_pdasstr->m_ich + f_pdasstr->m_cch ) const DRM_CHAR *f_pszBase,
    const DRM_SUBSTRING         *f_pdasstr,
    const DRM_ANSI_CONST_STRING *f_pdastr);

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DASSTRStrings_CI_Equal(
    __in_ecount( f_pdasstr->m_ich + f_pdasstr->m_cch ) const DRM_CHAR      *f_pszBase,
    const DRM_SUBSTRING *f_pdasstr,
    const DRM_ANSI_CONST_STRING *f_pdastr );

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DASSSTRStringsEqual(
    __in_ecount( f_pdasstr0->m_ich + f_pdasstr0->m_cch ) const DRM_CHAR *f_pszBase0,
    const DRM_SUBSTRING *f_pdasstr0,
    __in_ecount( f_pdasstr1->m_ich + f_pdasstr1->m_cch ) const DRM_CHAR *f_pszBase1,
    const DRM_SUBSTRING *f_pdasstr1);

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DSTRSearch( 
    IN const DRM_CONST_STRING *f_pdstrString,
    IN const DRM_CONST_STRING *f_pdstrSubString,
       OUT   DRM_CONST_STRING *f_pdstrFoundString );

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DSTRSearchReverse( 
    IN const DRM_CONST_STRING *f_pdstrString,
    IN const DRM_CONST_STRING *f_pdstrSubString,
       OUT   DRM_CONST_STRING *f_pdstrFoundString );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_EnsureDataAlignment(
    __in_bcount( f_cbOriginal ) const DRM_BYTE   *f_pbOriginal,
    IN  const DRM_DWORD   f_cbOriginal,
    __deref_out_bcount( *f_pcbAligned ) DRM_BYTE  **f_ppbAligned,
    OUT       DRM_DWORD  *f_pcbAligned,
    IN  const DRM_DWORD   f_cbDataType,
    OUT       DRM_DWORD  *f_pcbAdjustment);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_VerifyXMLSignature(
    IN  const   DRM_CONST_STRING        *f_pdstrSignedData,
    IN  const   DRM_CONST_STRING        *f_pdstrSignatureXML,
    IN  const   DRM_CONST_STRING        *f_pdstrCertChainXML,
    IN          DRM_BOOL                 f_fCheckExpiry,
    IN          DRM_BOOL                 f_fCheckCertChain,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey,
    IN  OUT     DRM_LICEVAL_CONTEXT     *f_pcontextLEVL);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_VerifyXMLSignatureEx(
    IN  const   DRM_CONST_STRING        *f_pdstrSignedData,
    IN  const   DRM_CONST_STRING        *f_pdstrSignatureValue,
    IN  const   DRM_CONST_STRING        *f_pdstrCertChainXML,
    IN          DRM_BOOL                 f_fCheckExpiry,
    IN          DRM_BOOL                 f_fCheckCertChain,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey,
    IN  OUT     DRM_LICEVAL_CONTEXT     *f_pcontextLEVL);

DRM_API DRM_BOOL DRM_CALL DRM_UTL_DateLessThan(
    IN const DRMFILETIME* f_pFileTime1, 
    IN const DRMFILETIME* f_pFileTime2);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_DecodeKID( 
    IN const DRM_CONST_STRING *f_pdstrKID, 
       OUT   DRM_KID          *f_pkid );
        
DRM_API DRM_VOID DRM_CALL DRM_UTL_PromoteASCIItoUNICODE(
    __in_ecount( f_pdassstrIn->m_ich + f_pdassstrIn->m_cch ) const DRM_CHAR *f_pszBase,
    const DRM_SUBSTRING *f_pdassstrIn, 
          DRM_STRING    *f_pdstrOut);    

DRM_API DRM_VOID DRM_CALL DRM_UTL_DemoteUNICODEtoASCII( 
    __in_ecount(cchMax) const DRM_WCHAR *pwszFileName, 
    __out_ecount(cchMax) DRM_CHAR  *pszFileName, 
                         DRM_DWORD  cchMax );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_XMLDecode(
    __in_ecount( f_cchEncoded ) const DRM_WCHAR *f_pwszEncoded,
    IN                                  DRM_DWORD    f_cchEncoded,
    __out_ecount_opt(*f_pcchDecoded) DRM_WCHAR *f_pwszDecoded,
    IN  OUT                             DRM_DWORD   *f_pcchDecoded );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_XMLDecodeUA(
    __in_ecount( f_cchEncoded ) const DRM_WCHAR *f_pwchEncoded,
    IN DRM_DWORD f_cchEncoded,
    __out_ecount_opt(*f_pcchDecoded) DRM_CHAR *f_pchDecoded,
    IN OUT DRM_DWORD *f_pcchDecoded );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_XMLDecodeA(
    __in_ecount( f_cchEncoded ) const DRM_CHAR *f_pchEncoded,
    IN DRM_DWORD f_cchEncoded,
    __out_ecount_opt(*f_pcchDecoded) DRM_CHAR *f_pchDecoded,
    IN OUT DRM_DWORD *f_pcchDecoded );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_XMLEncodeA(
    __in_ecount( f_cchDecoded ) const DRM_CHAR *f_pchDecoded,
    IN DRM_DWORD f_cchDecoded,
    __out_ecount_opt(*f_pcchEncoded) DRM_CHAR *f_pchEncoded,
    IN OUT DRM_DWORD *f_pcchEncoded );

DRM_API DRM_VOID DRM_CALL DRM_XOR( 
    __inout_bcount(cb)   DRM_BYTE *pbLHS, 
    __in_bcount(cb) const DRM_BYTE *pbRHS, 
    IN                   DRM_DWORD cb );

/************************************************
*  Reads GUID from specified offset in byte array
*  pointed by pbData.
*
************************************************/
#define DRM_GUID_LEN 16
DRM_API DRM_RESULT DRM_CALL DRM_UTL_ReadGUID(
   __in_bcount( cbData ) const DRM_BYTE *pbData, 
   IN  const    DRM_DWORD cbData,
   IN           DRM_DWORD ibGuidOffset,
   OUT          DRM_GUID *pDrmGuid );

/************************************************
*  Compares 2 GUID, returns 
*  pointed by pbData.
*
************************************************/

DRM_API DRM_BOOL DRM_CALL DRM_UTL_AreGUIDEqual( 
    const DRM_GUID *pDrmGuid1, 
    const DRM_GUID *pDrmGuid2 );


DRM_API DRM_BOOL DRM_CALL DRM_UTL_IsZeros(
    __in_bcount( cbBuff ) const DRM_BYTE *pbBuff,
    IN DRM_DWORD cbBuff );


DRM_API DRM_RESULT DRM_CALL DRM_UTL_LShift( 
    __in_bcount( f_cb ) const DRM_BYTE *f_pbIn, 
    __out_ecount(f_cb) DRM_BYTE *f_pbOut, 
    IN                 DRM_DWORD f_cb );


DRM_API DRM_RESULT DRM_CALL DRM_UTL_RShift( 
    __in_bcount( f_cb ) const DRM_BYTE *f_pbIn, 
    __out_ecount(f_cb) DRM_BYTE *f_pbOut, 
    IN                 DRM_DWORD f_cb );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_MultiplyDWORDs(
    IN      DRM_DWORD   f_dwFirst,
    IN      DRM_DWORD   f_dwSecond,
        OUT DRM_DWORD  *f_pdwResult );

EXIT_PK_NAMESPACE;

#endif /* __DRM_UTILITIES_H__ */
