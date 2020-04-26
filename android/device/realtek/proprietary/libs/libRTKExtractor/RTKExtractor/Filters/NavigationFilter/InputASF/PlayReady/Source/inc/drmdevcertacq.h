/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMDEVCERTACQ_H__
#define __DRMDEVCERTACQ_H__

ENTER_PK_NAMESPACE;

/**********************************************************************
** Function:    DRM_DevCert_ProcessResponse
** Synopsis:    Process devcert request response received from server
** Arguments:   [pbResponse] -- Response string received from server
**              [cbResponse] -- size of pbResponse in bytes.
**              [pResult] --pointer to DRM_RESULT to get error from server included in response
**              [pcontextCrypto] --pointer to the DRM_CRYPTO_CONTEXT
** Returns:     DRM_SUCCESS on success, or failed with 
**              DRM_E_INVALIDARG,
**              DRM_E_INVALIDDEVICECERTIFICATE.
**
***********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_DCA_ProcessResponse(
    IN  DRM_BYTE           *pbResponse,
    IN  DRM_DWORD           cbResponse,
    OUT DRM_RESULT         *pResult,
    IN  DRM_CRYPTO_CONTEXT *pcontextCrypto );

/**********************************************************************
** Function:    DRM_DevCert_CreateChallenge
** Synopsis:    Creates a devcert challenge.
** Arguments:   
**              [pdstrDevCertTemplate] -- pointer to Signed DevCert Template drm string
**              [poSignKey] -- pointer to a Bootstrap private key to sign challenge.
**              [pcontextCrypto] -- pointer to DRM_CRYPTO_CONTEXT
**              [pwszUrl] -- user allocated buffer to get URL in WCHAR
**              [pcchUrl] -- pointer to get size of URL. Can be NULL if URL is not required.
**              [pbData] -- user allocated buffer to be used as output buffer
**              [pcbData] -- size of pbData
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
** Notes:       To get the minimum size required, first call with pbData=NULL and
**              pcbData points to value 0. This will return DRM_E_BUFFERTOOSMALL 
**              and pcbData will point to a value of minimum context size.
***********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_DCA_CreateChallenge(
    IN const DRM_CONST_STRING   *pdstrDevCertTemplate,
    IN const PRIVKEY            *poSignKey,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto, 
       OUT   DRM_WCHAR          *pwszUrl,
    IN OUT   DRM_DWORD          *pcchUrl,
       OUT   DRM_BYTE           *pbData,
    IN OUT   DRM_DWORD          *pcbData );

EXIT_PK_NAMESPACE;

#endif /* __DRMDEVCERTACQ_H__ */
