/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_UTILITIES_H__
#define __DRM_UTILITIES_H__

#include <drmcontextsizes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DRM_ROOTPUBKEY_CATEGORY
{
    WM_DRM_ROOTPUBKEY_LICENSES = 0,
    WM_DRM_ROOTPUBKEY_LRB,   
    WM_DRM_ROOTPUBKEY_CLK,
    WM_DRM_ROOTPUBKEY_MTR   
} DRM_ROOTPUBKEY_CATEGORY;

DRM_BOOL DRM_API DRM_UTL_IsCertDateOK(const DRM_BYTE *f_pbDate);

DRM_RESULT DRM_API DRM_UTL_CheckCertNoCache(
    IN const CERT               *f_pcert, 
    IN const PUBKEY             *f_ppubkey, 
    IN       DRM_BOOL            f_fCheckDate, 
    IN       DRM_CRYPTO_CONTEXT *f_pcontextCRYP);

DRM_RESULT DRM_API DRM_UTL_CheckCertificate(
    IN  const   CERT                    *f_pcert, 
    IN  const   PUBKEY                  *f_ppubkey, 
    IN          DRM_BOOL                 f_fCheckCertDate,   
    IN          DRM_LICEVAL_CONTEXT     *f_pcontextLEVL,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey);

DRM_RESULT DRM_API DRM_UTL_CheckCert(
    IN const CERT                *f_pcert, 
    IN const PUBKEY              *f_ppubkey, 
    IN       DRM_BOOL             f_fCheckCertDate, 
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL);

DRM_RESULT DRM_API DRM_UTL_CheckDACCert(
    IN const CERT                *f_pcert, 
    IN const PUBKEY              *f_ppubkey, 
    IN       DRM_BOOL             f_fCheckCertDate, 
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL);

DRM_RESULT DRM_API DRM_PK_EncryptLarge( 
    IN const PUBKEY              *f_ppubkey,
    IN OUT   DRM_BYTE            *f_pbClear, 
    IN       DRM_DWORD            f_cbClear, 
    OUT      DRM_BYTE            *f_pbCipher, 
    IN       DRM_DWORD            f_cbKeySymmetric, 
    IN       DRM_CRYPTO_CONTEXT  *f_pcontextCRYP);

DRM_RESULT DRM_API DRM_PK_DecryptLarge( 
    IN const PRIVKEY            *f_ppubkey,
    IN OUT   DRM_BYTE           *f_pbCipher, 
    IN       DRM_DWORD           f_cbCipher, 
       OUT   DRM_BYTE           *f_pbClear, 
    IN       DRM_CRYPTO_CONTEXT *f_pcontextCRYP);

DRM_RESULT DRM_API DRM_PK_SymmetricCrypt(
    IN const DRM_BYTE  *f_pbKey,
    IN       DRM_DWORD  f_cbKey,
    IN       DRM_DWORD  f_cbCipher,
    IN       DRM_BYTE  *f_pbCipher,
    IN       DRM_DWORD  f_cbClear,
       OUT   DRM_BYTE  *f_pbClear );

DRM_RESULT DRM_API DRM_PK_SymmetricSign(
    IN const DRM_BYTE  *f_pbKey,
    IN       DRM_DWORD  f_cbKey,
    IN const DRM_BYTE *f_pbData,
    IN       DRM_DWORD f_cbData,
       OUT   DRM_BYTE  f_rgbSymSig[__CB_DECL( SHA_DIGEST_LEN )] );

#if DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS
DRM_RESULT DRM_API DRM_PK_SymmetricVerify(
    IN const DRM_BYTE  *f_pbKey,
    IN       DRM_DWORD  f_cbKey,
    IN const DRM_BYTE *f_pbData,
    IN       DRM_DWORD f_cbData,
    IN const DRM_BYTE  f_rgbSymSig[__CB_DECL( SHA_DIGEST_LEN )] );
#endif

DRM_RESULT DRM_API DRM_UTL_GetVersionAsString( 
    IN  DRM_BYTE  rgbVersion[__CB_DECL(VERSION_LEN)],
    OUT DRM_WCHAR wszVerString [VER_STRING_MAX_LEN]);

DRM_RESULT DRM_API DRM_UTL_GetVersionFromString(
   IN const DRM_WCHAR  *pwszVersion,
   IN       DRM_DWORD   cchVersion,
      OUT   DRM_WORD    rgwVersion[VERSION_LEN] );

DRM_RESULT DRM_API DRM_UTL_StringToGuid( 
    IN const DRM_CONST_STRING *pdstrString,
    OUT      DRM_GUID         *pGuid );

DRM_DWORD DRM_API DRM_UTL_NumberToString (
    IN  DRM_DWORD  dwValue, 
    OUT DRM_WCHAR *pszBuffer, 
    IN  DRM_DWORD  cchBuffer);

DRM_RESULT DRM_API DRM_UTL_StringInsertSubString(
    IN OUT   DRM_STRING       *pdstrString,
    IN       DRM_DWORD         dwInsertPoint,
    IN const DRM_CONST_STRING *pdstrSubString);

DRM_RESULT DRM_API DRM_UTL_StringRemoveSubString(
    IN OUT   DRM_STRING       *pdstrString,
    IN const DRM_CONST_STRING *pdstrSubString);

DRM_RESULT DRM_API DRM_UTL_StringInsertBlankSubString(
    IN OUT DRM_STRING *f_pdstrTarget,
    IN     DRM_DWORD   f_ichInsertion,
    IN     DRM_DWORD   f_cch);

DRM_BOOL DRM_API DRM_UTL_DSTRStringsEqual(
    const DRM_CONST_STRING *pdstr1,
    const DRM_CONST_STRING *pdstr2);

DRM_BOOL DRM_API DRM_UTL_DASTRStringsEqual(
    const DRM_ANSI_CONST_STRING *f_pdastr1,
    const DRM_ANSI_CONST_STRING *f_pdastr2);

DRM_BOOL DRM_API DRM_UTL_DASSTRStringsEqual(
    const DRM_CHAR              *f_pszBase,
    const DRM_SUBSTRING         *f_pdasstr,
    const DRM_ANSI_CONST_STRING *f_pdastr);

DRM_BOOL DRM_API DRM_UTL_DASSSTRStringsEqual(
    const DRM_CHAR      *f_pszBase0,
    const DRM_SUBSTRING *f_pdasstr0,
    const DRM_CHAR      *f_pszBase1,
    const DRM_SUBSTRING *f_pdasstr1);

DRM_BOOL DRM_API DRM_UTL_DSTRSearch( 
    IN const DRM_CONST_STRING *f_pdstrString,
    IN const DRM_CONST_STRING *f_pdstrSubString,
       OUT   DRM_CONST_STRING *f_pdstrFoundString );

DRM_BOOL DRM_API DRM_UTL_DSTRSearchReverse( 
    IN const DRM_CONST_STRING *f_pdstrString,
    IN const DRM_CONST_STRING *f_pdstrSubString,
       OUT   DRM_CONST_STRING *f_pdstrFoundString );

DRM_RESULT DRM_API DRM_UTL_EnsureDataAlignment(
    IN  DRM_BYTE   *pbOriginal,
    IN  DRM_DWORD   cbOriginal,
    OUT DRM_BYTE  **ppbAligned,
    OUT DRM_DWORD  *pcbAligned,
    IN  DRM_DWORD   cbDataType,
    OUT DRM_DWORD  *pcbAdjustment);

DRM_RESULT DRM_API DRM_UTL_VerifyXMLSignature(
    IN  const   DRM_CONST_STRING        *f_pdstrSignedData,
    IN  const   DRM_CONST_STRING        *f_pdstrSignatureXML,
    IN  const   DRM_CONST_STRING        *f_pdstrCertChainXML,
    IN          DRM_BOOL                 f_fCheckExpiry,
    IN          DRM_BOOL                 f_fCheckCertChain,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey,
    IN  OUT     DRM_LICEVAL_CONTEXT     *f_pcontextLEVL);

DRM_RESULT DRM_API DRM_UTL_VerifyXMLSignatureEx(
    IN  const   DRM_CONST_STRING        *f_pdstrSignedData,
    IN  const   DRM_CONST_STRING        *f_pdstrSignatureValue,
    IN  const   DRM_CONST_STRING        *f_pdstrCertChainXML,
    IN          DRM_BOOL                 f_fCheckExpiry,
    IN          DRM_BOOL                 f_fCheckCertChain,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey,
    IN  OUT     DRM_LICEVAL_CONTEXT     *f_pcontextLEVL);

DRM_BOOL DRM_API DRM_UTL_DateLessThan(
    IN const DRMFILETIME* f_pFileTime1, 
    IN const DRMFILETIME* f_pFileTime2);

DRM_RESULT DRM_API DRM_UTL_DecodeKID( 
    IN const DRM_CONST_STRING *f_pdstrKID, 
       OUT   DRM_KID          *f_pkid );
        
DRM_VOID DRM_API DRM_UTL_PromoteANSItoUNICODE(
    const DRM_CHAR      *f_pszBase,
    const DRM_SUBSTRING *f_pdassstrIn, 
          DRM_STRING    *f_pdstrOut);    

DRM_VOID DRM_API DRM_UTL_DemoteUNICODEtoANSI( 
    const DRM_WCHAR *pwszFileName, 
          DRM_CHAR  *pszFileName, 
          DRM_DWORD  cchMax );

DRM_RESULT DRM_UTL_XMLDecode(
    IN  const   DRM_WCHAR   *f_pwszEncoded,
    IN          DRM_DWORD    f_cchEncoded,
        OUT     DRM_WCHAR   *f_pwszDecoded,
    IN  OUT     DRM_DWORD   *f_pcchDecoded );

DRM_VOID DRM_API DRM_XOR( 
    IN OUT   DRM_BYTE *pbLHS, 
    IN const DRM_BYTE *pbRHS, 
    IN       DRM_DWORD cb );

#ifdef __cplusplus
}
#endif

#endif /* __DRM_UTILITIES_H__ */
