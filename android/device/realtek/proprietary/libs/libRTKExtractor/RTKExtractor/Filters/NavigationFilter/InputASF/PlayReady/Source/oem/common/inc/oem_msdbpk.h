/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
**  File oem_msdbpk.h - declarations for OEM functions required for MSDB PK.
**  These OEM functions are called by MSDB PK internally and should be implemented 
**  in order to link the project.
**  There is an example implementation of these functions in file oemcfdimpl.c
*/

#ifndef __OEM_MSDB_PK_H__
#define __OEM_MSDB_PK_H__

#include <drmkeygenerator.h>



/* The size of a Content Key Seed in raw (30 bytes), not B64 encoded (40 bytes), form. */
#define CKSEED_NOT_B64_ENCODED_LEN  30

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL OEM_GetCRL(
   IN     DRM_GUID   f_guidCRL,      
   OUT    DRM_BYTE  *f_pbCRLData,    
   IN OUT DRM_DWORD *f_pcbCRLData    
);

DRM_API DRM_RESULT DRM_CALL OEM_SetCRL
(
   IN       DRM_GUID        f_guidCRL,  
   IN const DRM_BYTE       *f_pbCRLData,
   IN       DRM_DWORD       f_cbCRLData
);

DRM_API DRM_RESULT DRM_CALL OEM_GetContentKeySeed
(
      OUT  DRM_WCHAR        f_wszKeySeed[DRM_KEYSEED_BASE64_LEN]
);

DRM_API DRM_RESULT DRM_CALL OEM_GetLicenseServerPrivateKey
(
    OUT PRIVKEY   *f_pLSPrivateKey
);

DRM_API DRM_RESULT DRM_CALL OEM_GetLicenseCertificates
(
       OUT DRM_BYTE  *f_pbCertificate2,
    IN     DRM_DWORD  f_cbC2Size,
       OUT DRM_BYTE  *f_pbCertificate1,
    IN     DRM_DWORD  f_cbC1Size
);

EXIT_PK_NAMESPACE;

#endif /* __OEM_MSDB_PK_H__ */
