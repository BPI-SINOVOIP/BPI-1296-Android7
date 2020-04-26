/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_SOAP_XML_UTILITY_H
#define __DRM_SOAP_XML_UTILITY_H

#include <drmxmlbuildera.h>
#include <drmxmlsig.h>
#include <drmmanager.h>
ENTER_PK_NAMESPACE;

/* Whether to generate SOAP envelope. */
#define DRM_USE_SOAP        1

/*
** Type of data element to be retrieved from the server response.
** GARD stands for Get Additional Response Data.
*/
enum
{
    /* Data type code for custom data. */
    DRM_GARD_CUSTOM_DATA  = 1,

    /* Data type code for redirected URL. */
    DRM_GARD_REDIRECT_URL = 2,

    /* Data type code for service ID. */
    DRM_GARD_SERVICE_ID   = 3,

    /* Data type code for account ID. */
    DRM_GARD_ACCOUNT_ID   = 4
};

/*
** ---------------------------------------
** SOAP XML utility API functions
** ---------------------------------------
*/
DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_EncodeData(
    __in_ecount( f_cchData )         const DRM_CHAR  *f_pchData,
    IN                                     DRM_DWORD  f_cchData,
    __out_ecount_opt( *f_pcchEncodedData ) DRM_CHAR  *f_pchEncodedData,
    OUT                                    DRM_DWORD *f_pcchEncodedData );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_DecodeData(
    __in_ecount( f_cchData )               DRM_CHAR  *f_pchData,
    IN                                     DRM_DWORD  f_cchData,
    __out_ecount_opt( *f_pcchDecodedData ) DRM_CHAR  *f_pchDecodedData,
    OUT                                    DRM_DWORD *f_pcchDecodedData );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_PadANSIString(
    IN OUT DRM_ANSI_STRING *f_pdastrIn,
    IN     DRM_DWORD        f_cchPad );

DRM_API DRM_BOOL DRM_CALL DRM_SOAPXML_IsBlankGUID(
    IN DRM_GUID *f_poGUID );

DRM_API DRM_DWORD DRM_CALL DRM_SOAPXML_CalcDWORDCharCount(
    IN DRM_DWORD f_dwNumber );

#if DRM_USE_SOAP

DRM_API DRM_DWORD DRM_CALL DRM_SOAPXML_CalcSOAPHeaderCharCount( DRM_VOID );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_SkipSOAPHeaders(
    __in_ecount( f_pdasstrResponse->m_ich + f_pdasstrResponse->m_cch ) DRM_CHAR      *f_pszResponse,
    IN OUT                                                             DRM_SUBSTRING *f_pdasstrResponse );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_PrependXMLRoot(
    IN OUT DRM_ANSI_STRING *f_pdastrXML );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_BuildSOAPHeaderXML(
    IN OUT _XMBContextA *f_poXMLContext,
    IN     DRM_DWORD     f_cbXMLContext );

#endif

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_InitXMLKey(
    IN     DRM_VOID           *f_poOEMContext,
    OUT    DRM_XML_KEY        *f_poXMLKey,
    IN OUT DRM_CRYPTO_CONTEXT *f_pCryptoCtx );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_EncryptDataWithXMLKey(
    IN     DRM_VOID              *f_poOEMContext,
    IN OUT DRM_XML_KEY           *f_poXMLKey,
    IN OUT DRM_ANSI_CONST_STRING *f_pdastrData,
    IN OUT DRM_CRYPTO_CONTEXT    *f_pCrypto );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_GetDeviceCert(
    __in_opt                              DRM_VOID  *f_poOEMContext,
    IN                                    DRM_BOOL   f_fB64Encoded,
    __out_ecount_opt( *f_pcchDeviceCert ) DRM_CHAR  *f_pchDeviceCert,
    IN OUT                                DRM_DWORD *f_pcchDeviceCert );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_PrepareXMLContext(
    __in_bcount( f_cbXMLContext )          DRM_BYTE      *f_pbXMLContext,
    IN                                     DRM_DWORD      f_cbXMLContext,
    __deref_out_bcount( *f_pcbXMLContext ) _XMBContextA **f_ppoXMLContext,
    OUT                                    DRM_DWORD     *f_pcbXMLContext );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_ParseCustomDataForProtocol(
    __in_bcount( f_cbResponse )     const DRM_BYTE  *f_pbResponse,
    __in                                  DRM_DWORD  f_cbResponse,
    __in                                  DRM_DWORD  f_dwProtocolType,
    __out_ecount_opt( *f_pcchCustomData ) DRM_CHAR  *f_pchCustomData,
    __inout_opt                           DRM_DWORD *f_pcchCustomData );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_ValidateProtocolSignature(
    __in_bcount( f_cbResponse )   const DRM_BYTE       *f_pbResponse,
    __in                                DRM_DWORD       f_cbResponse,
    __inout_bcount( f_cbScratchBCertBuffer )
                                        DRM_BYTE       *f_pbScratchBCertBuffer,
    __in                          const DRM_DWORD       f_cbScratchBCertBuffer,
    __in                                DRM_BB_CONTEXT *f_poBBXContext,
    __inout                             DRM_DWORD      *f_pdwProtocolType );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_ParseStatusCodeAndChooseBest(
    __in_bcount( f_cbResponse ) const DRM_BYTE   *f_pbResponse,
    IN                                DRM_DWORD   f_cbResponse,
    IN                                DRM_RESULT  f_drOriginal,
    OUT                               DRM_RESULT *f_pResult );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_ParseStatusCode(
    __in_bcount( f_cbResponse ) const DRM_BYTE   *f_pbResponse,
    __in                              DRM_DWORD   f_cbResponse,
    __out                             DRM_RESULT *f_pResult );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_GetAdditionalResponseData(
    __in_bcount( f_cbResponse )     const DRM_BYTE  *f_pbResponse,
    IN                                    DRM_DWORD  f_cbResponse,
    IN                                    DRM_DWORD  f_dwDataType,
    __out_ecount_opt( *f_pcchDataString ) DRM_CHAR  *f_pchDataString,
    IN OUT                                DRM_DWORD *f_pcchDataString );

DRM_API DRM_RESULT DRM_CALL Drm_GetAdditionalResponseData(
    IN                                    DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount( f_cbResponse )     const DRM_BYTE        *f_pbResponse,
    IN                                    DRM_DWORD        f_cbResponse,
    IN                                    DRM_DWORD        f_dwDataType,
    __out_ecount_opt( *f_pcchDataString ) DRM_CHAR        *f_pchDataString,
    IN OUT                                DRM_DWORD       *f_pcchDataString );

DRM_API DRM_RESULT DRM_CALL DRM_SOAPXML_EnsureTagValueNativeByteAligned(
    __inout _XMBContextA *f_poXMLContext,
    __in const DRM_ANSI_CONST_STRING *f_pdastrTagName );

enum DRM_SOAPXML_PROTOCOL_TYPE
{
    DRM_SOAPXML_PROTOCOL_UNKNOWN                   = 0x00,
    DRM_SOAPXML_PROTOCOL_EXCEPTION                 = 0x01,
    DRM_SOAPXML_PROTOCOL_DOMAIN_JOIN_RESPONSE      = 0x02,
    DRM_SOAPXML_PROTOCOL_DOMAIN_LEAVE_RESPONSE     = 0x03,
    DRM_SOAPXML_PROTOCOL_METERING_CERT_RESPONSE    = 0x04,
    DRM_SOAPXML_PROTOCOL_METERING_DATA_RESPONSE    = 0x05,
    DRM_SOAPXML_PROTOCOL_LIC_ACQ_RESPONSE          = 0x06,
    DRM_SOAPXML_PROTOCOL_LIC_ACQ_ACK_RESPONSE      = 0x07,
    DRM_SOAPXML_PROTOCOL_MAXIMUM_DEFINED           = 0x07
};

typedef struct tagDRM_SOAPXML_PROTOCOL_INTERNAL_DEFINE
{
    const DRM_ANSI_CONST_STRING *pdastrCustomDataPath;
    const DRM_ANSI_CONST_STRING *pdastrDataPath;
    const DRM_ANSI_CONST_STRING *pdastrSignaturePath;
    DRM_BOOL                     fSignatureRequired;
} DRM_SOAPXML_PROTOCOL_INTERNAL_DEFINE;

#define EMPTY_DRM_SOAPXML_PROTOCOL_INTERNAL_DEFINE { NULL, NULL, NULL, FALSE }

EXIT_PK_NAMESPACE;

#endif /* __DRM_SOAP_XML_UTILITY_H */
