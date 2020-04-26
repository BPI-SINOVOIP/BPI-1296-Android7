/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_UTILITIESLITE_H__
#define __DRM_UTILITIESLITE_H__

#include <drmcommon.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_UTL_GetVersionAsString(
    __in_bcount( VERSION_LEN )         const DRM_BYTE  f_rgbVersion   [__CB_DECL(VERSION_LEN)],
    __out_ecount( VER_STRING_MAX_LEN )       DRM_WCHAR f_wszVerString [VER_STRING_MAX_LEN] );

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

DRM_API DRM_RESULT DRM_CALL DRM_UTL_StringToNumber(
    __in      DRM_CONST_STRING *pdstrString,
    __out     DRM_LONG         *plValue,
    __out_opt DRM_DWORD        *pcchValue );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_NumberToString(
    __in                          DRM_DWORD  f_dwValue,
    __out_ecount_z( f_cchBuffer ) DRM_WCHAR *f_pwszBuffer,
    __in                          DRM_DWORD  f_cchBuffer,
    __in                          DRM_DWORD  f_dwPadZeros,
    __in                          DRM_DWORD  f_dwRadix,
    __out_opt                     DRM_DWORD *f_pcchRLength );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_UTL_NumberToStringA(
    __in                          DRM_DWORD  f_dwValue,
    __out_ecount_z( f_cchBuffer ) DRM_CHAR  *f_pszBuffer,
    __in                          DRM_DWORD  f_cchBuffer,
    __in                          DRM_DWORD  f_dwPadZeros,
    __in                          DRM_DWORD  f_dwRadix,
    __out_opt                     DRM_DWORD *f_pcchRLength );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_GetVersionFromStringAsDWORD(
   __in_ecount( f_cchVersion ) const DRM_WCHAR  *f_pwszVersion,
   __in                              DRM_DWORD   f_cchVersion,
   __inout                           DRM_DWORD  *f_pdwVersion );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_GetVersionFromStringAsDWORDA(
   __in_ecount( f_cchVersion ) const DRM_CHAR   *f_pszVersion,
   __in                              DRM_DWORD   f_cchVersion,
   __inout                           DRM_DWORD  *f_pdwVersion );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_GetZuluTimeFromStringAsUI64(
    __inout DRM_CONST_STRING *f_pdstrZuluTime,
    __out   DRM_UINT64       *f_pui64DateTime );

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
    __in DRM_DWORD f_cchDecoded,
    __in DRM_BOOL f_fAttributeText,
    __out_ecount_opt(*f_pcchEncoded) DRM_CHAR *f_pchEncoded,
    __inout DRM_DWORD *f_pcchEncoded );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_XMLEncode(
    __in  const   DRM_CONST_STRING    *f_pdstrToEncode,
    __in          DRM_BOOL             f_fAttributeText,
    __out         DRM_STRING          *f_pdstrEncoded );

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

#endif /* __DRM_UTILITIESLITE_H__ */

