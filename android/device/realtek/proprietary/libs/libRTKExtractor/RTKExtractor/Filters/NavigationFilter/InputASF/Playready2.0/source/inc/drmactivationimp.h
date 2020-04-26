/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMACTIVATIONIMP_H__
#define __DRMACTIVATIONIMP_H__

#include <drmtypes.h>

ENTER_PK_NAMESPACE;

/**********************************************************************
**
** Function:    DRM_ACT_GenerateChallengeInternal
**
** Synopsis:    This method generates an XML activation challenge
**              with the given challenge parameters.
**
** Arguments:
**              f_dwClientSdkVersion  : Client SDK version in the
**                                      form A.B.C.D. For a PK client, the
**                                      SDK version will be the build version.
**              f_dwClientSdkType     : Client SDK type. Supported
**                                      values are currently 0 = PlayReady SDK,
**                                      1 = Silverlight Loader, 2 = Activation
**                                      library.
**              f_dwPlatform          : Client platform. Supported
**                                      values are currently 0 = WindowsOnX86,
**                                      1 = WindowsOnX64, 2 = WindowsOnARM,
**                                      3 = OSXOnX86, 4 = OSXOnX64,
**                                      5 = XBOXOnPPC, 6 = iOSOnARM
**              f_dwTransformVersion  : Transform version DWORD specifying the
**                                      underlying protocol and transforms executed
**                                      on the activation blob.
**              f_pbPrimaryPayload: buffer containing the activation blob in
**                                  the client lib.
**              f_cbPrimaryPayload: Length of the activation blob in bytes.
**              f_pbAuxPayload    : Optional buffer containing the current
**                                  keyfile for re-activation.
**              f_cbAuxPayload    : Length of the keyfile blob in bytes.
**              f_pbChallenge         : Buffer that contains the returned XML
**                                      challenge. The caller is responsible
**                                      for allocating this buffer. This can be
**                                      set to NULL when querying for buffer size.
**              f_pcbChallenge        : Size of the returned challenge in bytes.
**
** Returns:     DRM_E_BUFFERTOOSMALL  : The passed in buffer was too small for
**                                      generating the challenge. f_pcbChallenge
**                                      contains the required buffer size.
**              DRM_SUCCESS           : The challenge was generated successfully.
**              DRM_E_INVALIDARG      : If the argument is invalid or
**              any return code as defined in drmresults.h.
**
** Notes:       Does not use a caller passed in stack for allocating scratch space
**              but instead accounts for scratch buffer space in the requested
**              challenge buffer size so caller can predictably allocate required
**              space.
**
**********************************************************************/
DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ACT_GenerateChallengeInternal(
    __in                                      DRM_DWORD   f_dwClientSdkVersion,
    __in                                      DRM_DWORD   f_dwClientSdkType,
    __in                                      DRM_DWORD   f_dwPlatform,
    __in                                      DRM_DWORD   f_dwTransformVersion,
    __in_bcount( f_cbPrimaryPayload )   const DRM_BYTE   *f_pbPrimaryPayload,
    __in                                      DRM_DWORD   f_cbPrimaryPayload,
    __in_ecount_opt( f_cbAuxPayload )   const DRM_BYTE   *f_pbAuxPayload,
    __in                                      DRM_DWORD   f_cbAuxPayload,
    __inout_bcount_opt( *f_pcbChallenge )     DRM_BYTE   *f_pbChallenge,
    __inout                                   DRM_DWORD  *f_pcbChallenge );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ACT_VerifyAppData(
    __in_ecount(f_cbAppId) const DRM_BYTE  *f_pbAppId,
    __in                         DRM_DWORD  f_cbAppId,
    __in                         DRM_DWORD  f_dwAppVersion );

/**********************************************************************
**
** Function:    DRM_ACT_ProcessResponseGenerateChallenge
**
** Synopsis:    This method processes an XML activation response
**              and returns the next challenge to be used by the application OR
**              processes the activation response if this is the final
**              protocol response to be processed.
**
** Arguments:
**              f_pbActivationCtx     : Activation protocol state buffer.
**              f_pbResponse          : Buffer that contains the XML
**                                      response to be processed.
**              f_cbResponse          : Size of the response in bytes.
**              f_pbChallenge         : Buffer that contains the next returned XML
**                                      challenge if any.
**              f_pcbChallenge        : Size of the next returned challenge in bytes.
**              f_pszUrl              : The next Url to be used by the application.
**              f_pcchUrl             : Size of the next Url in bytes.
**              f_pResult             : Optional pointer to a variable that receives the
**                                      status code.
**
** Returns:     DRM_E_BUFFERTOOSMALL  : The passed in challenge buffer was too small
**                                      to generate the next challenge. The required size
**                                      is returned in f_pcbChallenge.
**              DRM_SUCCESS           : The response was processed successfully.
**              DRM_E_INVALIDARG      : If the argument is invalid.
**              or any return code as defined in drmresults.h.
**
** Notes:
**
**********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_ACT_ProcessResponseGenerateChallenge(
    __inout_bcount( ACTIVATION_CONTEXT_SIZE )  DRM_BYTE        *f_pbActivationCtx,
    __in_bcount_opt( f_cbResponse )            DRM_BYTE        *f_pbResponse,
    __in_opt                                   DRM_DWORD        f_cbResponse,
    __out_bcount_opt( *f_pcbChallenge )        DRM_BYTE        *f_pbChallenge,
    __inout                                    DRM_DWORD       *f_pcbChallenge,
    __out_ecount_opt( *f_pcchUrl )             DRM_CHAR        *f_pszUrl,
    __inout                                    DRM_DWORD       *f_pcchUrl,
    __out                                      DRM_RESULT      *f_pResult );

/**********************************************************************
**
** Function:    DRM_ACT_ProcessResponseInternal
**
** Synopsis:    This method processes an XML activation response
**              and returns the primary and optionally the auxillary
**              payload Url.
**
** Arguments:   f_pdastrResponse  : buffer containing the response XML.
**              f_pbPrimaryPayload: Buffer containing the activation keyfile response.
**              f_cbPrimaryPayload: Length of the activation keyfile response in bytes.
**              f_pdastrAuxPayloadUrl: returned string containing the optional auxillary
**                                     payload Url. An EMPTY_DRM_STRING is returned if no
**                                     auxillary response Url is present. The caller should
**                                     pass in an EMPTY_DRM_STRING for this parameter.
**
** Returns:     DRM_E_BUFFERTOOSMALL  : The passed in buffer was too small for
**                                      returning the primary payload. f_pcbPrimaryPayload
**                                      contains the required buffer size.
**              DRM_SUCCESS           : The challenge was generated successfully.
**              DRM_E_INVALIDARG      : If the argument is invalid.
**              DRM_E_PROTOCOL_VERSION_NOT_SUPPORTED : The response namespace version is
**                                                     not supported.
**              or any return code as defined in drmresults.h.
**
** Notes:       Space needed for the auxillary payload Url is accounted for
**              in the requested primary payload buffer size.
**
**********************************************************************/
DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ACT_ProcessResponseInternal(
    __in                                     DRM_ANSI_CONST_STRING *f_pdastrResponse,
    __inout_bcount( *f_pcbPrimaryPayload )   DRM_BYTE              *f_pbPrimaryPayload,
    __inout                                  DRM_DWORD             *f_pcbPrimaryPayload,
    __out                                    DRM_ANSI_STRING       *f_pdastrAuxPayloadUrl );

/**********************************************************************
**
** Function:    DRM_ACT_ProcessWSDLResponseInternal
**
** Synopsis:    This method processes an XML activation petition response
**              (the activation service WSDL)
**
** Arguments:   f_pdastrResponse      : buffer containing the response XML.
**              f_pdastrActivationUrl : returned string containing the activation Url.
**
** Returns:     DRM_E_BUFFERTOOSMALL : The passed in DRM_ANSI_STRING was too small for
**                                      returning the Url. cchString
**                                      contains the required buffer size.
**              DRM_SUCCESS          : The response was processed successfully.
**              DRM_E_INVALIDARG     : If the argument is invalid.
**              or any return code as defined in drmresults.h.
**
** Notes:
**
**********************************************************************/
DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ACT_ProcessWSDLResponseInternal(
    __in                                    DRM_ANSI_CONST_STRING  *f_pdastrResponse,
    __out                                   DRM_ANSI_STRING        *f_pdastrActivationUrl );

EXIT_PK_NAMESPACE;

#endif /* __DRMACTIVATIONIMP_H__ */

