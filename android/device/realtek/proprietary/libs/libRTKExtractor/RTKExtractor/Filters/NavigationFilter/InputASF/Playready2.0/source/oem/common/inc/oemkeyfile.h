/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __OEMKEYFILE_H__
#define __OEMKEYFILE_H__

#include <drmcommon.h>
#include <oemaeskey.h>
#include <oemaescommon.h>
#include <drmkeyfile.h>

ENTER_PK_NAMESPACE;

extern DRM_EXPORT_VAR DRM_DWORD g_dwWMRobustnessVersion;
extern DRM_EXPORT_VAR DRM_DWORD g_dwPRRobustnessVersion;

extern DRM_EXPORT_VAR DRM_DWORD g_dwPlatformID;


/*
** Data presentation of one entry in AES key history list
*/
typedef struct {
    /*
    ** Key ID
    */
    const DRM_GUID guidKeyFileAESKeyID;

    /*
    ** Key value
    */
    const DRM_BYTE rgbKeyFileAESKey[__CB_DECL( DRM_AES_KEYSIZE_128 )];
    
} DRM_KEYHISTORY_ENTRY;


/*
** Sample keyfile key for test purposes
*/
extern const DRM_EXPORT_VAR DRM_KEYHISTORY_ENTRY g_oTestKeyfileKey;

/*
** Point to an array of key history entries
** The pointer is here to enable mocking the key history entries in tests. 
*/
extern DRM_EXPORT_VAR DRM_KEYHISTORY_ENTRY **g_ppKeyHistoryList;
/*
** Number of key history entries in the list
*/
extern DRM_EXPORT_VAR DRM_DWORD            g_dwKeyHistoryCount;

#define KF_MAX_FILE_PATH    256

/*
** Path of the key file.
*/
extern DRM_EXPORT_VAR DRM_WCHAR g_rgwchKeyFileName[ KF_MAX_FILE_PATH ];

/*
** Flag indicating whether the default key file should be used.
*/
extern DRM_EXPORT_VAR DRM_BOOL g_fUseDefaultKeyFile;

#define KF_DECRYPTION_TYPE_CLEAR       0x0000
#define KF_DECRYPTION_TYPE_AES_CBC     0x0001
#define KF_DECRYPTION_TYPE_MAX_VALUE   0x0004

#define KF_MAX_ENCRYPTION_BUFFER 1024 / BITS_PER_BYTE  /* Big Enough to hold RSA1024 Priv Key */
/*
** Not supported in PK code
*/
#define KF_DECRYPTION_TYPE_AES_CTR_WM7       0x0002
#define KF_DECRYPTION_TYPE_AES_CTR_WM7_PLAIN 0x0003
#define KF_DECRYPTION_TYPE_AES_CTR_WM7_SST   0x0004

DRM_API OEM_FILEHDL Oem_Keyfile_Open(
    __in_opt DRM_VOID   *f_pOEMContext,
    __in     DRM_DWORD   f_dwAccessMode,
    __in     DRM_DWORD   f_dwShareMode,
    __in     DRM_DWORD   f_dwCreationDisposition,
    __in     DRM_DWORD   f_dwAttributes );

DRM_API DRM_RESULT Oem_Keyfile_Delete( );

DRM_API DRM_VOID Oem_Keyfile_Close(
    __in OEM_FILEHDL f_fHandle );


DRM_API DRM_RESULT Oem_Keyfile_GetRobustnessVersion(
    DRM_VOID    *f_pOEMContext,
    DRM_KEYFILE_CERT_TYPE f_eCertType,
    DRM_DWORD   *f_pdwRobustnessVersion,
    DRM_DWORD   *f_pdwPlatform );

DRM_API DRM_RESULT Oem_Keyfile_LoadFromFile(
     __inout DRM_KEYFILE_CONTEXT *f_pKeyFile );

DRM_API DRM_RESULT DRM_CALL Oem_Keyfile_StoreToFile(
     __inout DRM_KEYFILE_CONTEXT *f_pKeyFile );

DRM_API DRM_RESULT Oem_Keyfile_GetKFAesKey(
    __in                const DRM_GUID *f_pguidAesKeyID,
    __inout_ecount( 1 ) DRM_AES_KEY    *f_pAesKey );

DRM_API DRM_RESULT Oem_Keyfile_GetCurrentKFAesKeyId(
    __out DRM_GUID *f_pguidAesKeyID );

EXIT_PK_NAMESPACE;

#endif /*__OEMKEYFILE_H__ */

