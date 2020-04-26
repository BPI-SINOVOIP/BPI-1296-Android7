/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _ASFCABSTRACTION_H_
#define _ASFCABSTRACTION_H_

#include <drmtypes.h>

#ifdef __cplusplus
extern "C" {
#endif/* __cplusplus */

typedef DRM_RESULT ( *PFNPAYLOADCALLBACK ) ( DRM_BYTE* f_pbPayloadData, DRM_DWORD f_cbPayloaddata, DRM_VOID* f_pvContext );
typedef void ASFContext;

/********************************************************************** 
** Structure for ASF Encryption Data.
** This structure represents the data that allow author to protect the content.
**
** 1. Length of KeyID
** 2. KeyID
** 3. License Acquisition URL Length
** 4. License Acquisition URL
** 5. Secret Data length
** 6. Secret Data
** 7. DRM V2 Header size
** 8. DRM V2 Header 
***********************************************************************/
typedef struct _ASFEncryptionData
{
    /* DRM V1 Header */
    DRM_DWORD        dwKeyIDLen;
    DRM_CHAR*        pKeyID; 
    DRM_DWORD        dwAcqLicURLLen;
    DRM_CHAR*        pAcqLicURL; 
    DRM_DWORD        dwSecretDataLength;
    DRM_BYTE*        pSecretData;

    /* DRM V2 Header */
    DRM_DWORD        dwDRMV2HeaderSize;
    DRM_WCHAR*       pDRMV2Header;
} ASFEncryptionData;

/******************************************************************************
** 
** Function :   ASF_CopyEncryptionData
** 
** Synopsis :   Makes full copy of Encryption data. Allocates memory for v1 and v2 data and copy it.
** 
** Arguments :  f_pSrcEncryptData    [in]         - Source Encryption data to copy from.
**              f_pTargetEncryptData [out]        - Target encryption data to copy to.
**                        
**
** Returns :    S_OK  on Success. 
**              There might be 2 errors - invalid arguments or DRM_E_OUTOFMEMORY.
**              
******************************************************************************/

DRM_RESULT ASF_CopyEncryptionData

( 
    IN  const ASFEncryptionData *f_pSrcEncrypData,
    OUT       ASFEncryptionData *f_pTargetEncrypData
);

DRM_RESULT ASF_FreeEncryptionData

( 
    __inout_ecount(1) ASFEncryptionData *f_pEncryptData
);


/*
** C APIs for ASF Parsing
*/

DRM_RESULT ASF_Initialize
(
 __in_ecount_z(f_dwInputFile+1)    const    DRM_WCHAR              *f_pwszInputFile, 
                                            DRM_DWORD              f_dwInputFile,
 __in_ecount_z(f_dwOutputFile+1)   const    DRM_WCHAR              *f_pwszOutputFile,
                                            DRM_DWORD              f_dwOutputFile,
                                            DRM_BOOL               f_fEncrypt,
                                            PFNPAYLOADCALLBACK     f_pPayLoadFn,
 __in_opt                                   DRM_VOID               *f_pvDRMContext,
 __deref_out_ecount( 1 )                    ASFContext             **f_ppASFContext
);

DRM_VOID ASF_Uninitialize
(
 __in_ecount(1)     ASFContext      *f_pASFContext
);

DRM_RESULT ASF_GetEncryptionData
(
IN     ASFContext           *f_pASFContext, 
   OUT ASFEncryptionData    *f_pEncryptionData
);

DRM_RESULT ASF_SetEncryptionData
(
 __inout_ecount(1)     ASFContext          *f_pASFContext, 
 __in_ecount(1)        ASFEncryptionData   *f_pEncryptionData
);

DRM_RESULT ASF_SetDRMContext
(
    __inout_ecount(1)       ASFContext           *f_pASFContext, 
    __in                    DRM_VOID             *f_pvDRMCtx
);

DRM_RESULT ASF_ProcessHeader
(
 __inout_ecount(1)     ASFContext          *f_pASFContext
);

DRM_RESULT ASF_ProcessPackets
(
 __inout_ecount(1)      ASFContext          *f_pASFContext,
                        DRM_DWORD            f_dwTimeSlice,
 __out_ecount(1)        DRM_BOOL            *f_pfIsDone
);

#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif  /*_ASFCABSTRACTION_H_*/
