/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMIMPL_H__
#define __OEMIMPL_H__

#include <drmsha1.h>
#include <drmpkcrypto.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_UNIQUEID_LEN 256
#define MAX_URL_LEN 256

/* Device information functions */

/**********************************************************************
** Function:    OEM_GetDeviceCert
** Synopsis:    Retreives the local device certificate.
** Arguments:   [pbDevCert] -- buffer to hold the device certificate
**              [pcbDevCert] -- pointer to DWORD that is the length of 
**              pbDevCert in bytes.  The length of the certicficate is 
**              returned here on function exit.
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**              DRM_E_FILEREADERROR
** Notes:       To get the minimum size required, first call with 
**              pbDevCert=NULL and pcbData points to value 0. This 
**              will return DRM_E_BUFFERTOOSMALL 
**              and pcbData will point to a value of minimum pbDevCert
***********************************************************************/
DRM_RESULT DRM_API OEM_GetDeviceCert(
    OUT    DRM_BYTE  *pbDevCert,
    IN OUT DRM_DWORD *pcbDevCert);

/**********************************************************************
** Function:    OEM_GetDeviceCertTemplate
** Synopsis:    Retreives the local device certificate.
** Arguments:   [pbDevCert] -- buffer to hold the device certificate
**              [pcbDevCert] -- pointer to DWORD that is the length of 
**              pbDevCert in bytes.  The length of the certicficate is 
**              returned here on function exit.
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_BUFFERTOOSMALL
**              DRM_E_FILEREADERROR
** Notes:       To get the minimum size required, first call with 
**              pbDevCert=NULL and pcbData points to value 0. This 
**              will return DRM_E_BUFFERTOOSMALL 
**              and pcbData will point to a value of minimum pbDevCert
***********************************************************************/
DRM_RESULT DRM_API OEM_GetDeviceCertTemplate(
    OUT    DRM_BYTE  *pbDevCert,
    IN OUT DRM_DWORD *pcbDevCert);



/**********************************************************************
** Function:    OEM_SetDeviceCert
** Synopsis:    Stores the local device certificate.
** Arguments:   [pbDevCert] -- buffer holding the device certificate
**              [cbDevCert] -- Length of pbDevCert in bytes.  
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
**              DRM_E_FILEWRITEERROR
***********************************************************************/

DRM_RESULT DRM_API OEM_SetDeviceCert(
    IN const DRM_BYTE *pbDevCert,
    IN DRM_DWORD       cbDevCert);


/**********************************************************************
** Function:    OEM_GetSecureStoreGlobalPasswordSeed
** Synopsis:    Retreives the local device global secure store password.
** Arguments:   [rgbSecStorePassword] -- buffer to hold the secure store
**              global password
** Returns:     DRM_SUCCESS on success. On Failure, it returns:
**              DRM_E_INVALIDARG
***********************************************************************/
DRM_RESULT DRM_API OEM_GetSecureStoreGlobalPasswordSeed( 
    OUT DRM_BYTE rgbSecStorePassword [__CB_DECL(SHA_DIGEST_LEN)] );

/**********************************************************************
** Function:    OEM_GetGroupCertPrivateKey
** Synopsis:    Get the GroupCert private key
** Arguments:   [pPrivKey] -- Buffer to hold the device private key
***********************************************************************/
DRM_RESULT DRM_API OEM_GetGroupCertPrivateKey( 
    OUT PRIVKEY *pPrivKey );

/**********************************************************************
** Function:    OEM_GetFallbackPrivateKey
** Synopsis:    Get the Fallback private key
** Arguments:   [pPrivKey] -- Buffer to hold the device private key
***********************************************************************/
DRM_RESULT DRM_API OEM_GetFallbackSigningKey( 
    OUT PRIVKEY *pPrivKey);

/**********************************************************************
** Function:    OEM_GetUniqueID
** Synopsis:    Get the unique id of the device in WCHAR * format. 
**              It is compared in OEM_CompareMachineId.
** Arguments:   [wchUniqueId] -- Buffer to hold the device unique id.
**              Max len is MAX_UNIQUEID_LEN
**              [pLen] -- size of unique id in WCHAR
***********************************************************************/
DRM_RESULT DRM_API OEM_GetUniqueID( 
    IN DRM_WCHAR      wchUniqueId[MAX_UNIQUEID_LEN], 
    IN OUT DRM_DWORD *pLen );

/**********************************************************************
** Function:    OEM_GenRandomBytes
** Synopsis:    Generate random bytes using an OEM provided random 
**              number generator.
** Arguments:   [pbData] -- Buffer to hold the random bytes
**              [cbData] -- Count of bytes to generate and fill in pbData
***********************************************************************/
DRM_RESULT DRM_API OEM_GenRandomBytes( 
    OUT DRM_BYTE  *pbData,
    IN  DRM_DWORD  cbData );

/**********************************************************************
** Function:    OEM_GetClockResetState
** Synopsis:    Get the Secure Clock Reset state.
** Arguments:   [pfReset] -- Bool to get the state
***********************************************************************/
DRM_RESULT DRM_API OEM_GetClockResetState( 
    DRM_BOOL *pfReset );

/**********************************************************************
** Function:    OEM_SetClockResetState
** Synopsis:    Set the Secure Clock Reset state.
** Arguments:   [fReset] -- Bool to set the state
***********************************************************************/
DRM_RESULT DRM_API OEM_SetClockResetState( 
    DRM_BOOL fReset );


DRM_RESULT OEM_StringCchCopyN( 
    DRM_WCHAR       *pwszDest,
    DRM_DWORD        cchDest,
    const DRM_WCHAR *pwszSrc,
    DRM_DWORD        cchSrc );

/* File IO functions
** =========================================================== 
*/

/* File Handle */
typedef DRM_VOID *OEM_FILEHDL;
#define OEM_INVALID_HANDLE_VALUE ((OEM_FILEHDL)-1)

#ifdef UNDER_CE
#define OEM_CRITICAL_SECTION_SIZE     20
#elif defined(_WIN64)
#define OEM_CRITICAL_SECTION_SIZE     40
#else
#define OEM_CRITICAL_SECTION_SIZE     24
#endif
typedef struct __tagOEM_CRITICAL_SECTION
{
    DRM_BYTE rgb [__CB_DECL(OEM_CRITICAL_SECTION_SIZE)];
} OEM_CRITICAL_SECTION;

/* OEM_OpenFile Access modes */

#define OEM_GENERIC_READ       (0x80000000L)
#define OEM_GENERIC_WRITE      (0x40000000L)
#define OEM_GENERIC_EXECUTE    (0x20000000L)
#define OEM_GENERIC_ALL        (0x10000000L)

/* OEM_OpenFile Share modes */

#define OEM_FILE_SHARE_NONE     0x00000000  
#define OEM_FILE_SHARE_READ     0x00000001  
#define OEM_FILE_SHARE_WRITE    0x00000002  

/* OEM_OpenFile Creation dispositions */

#define OEM_CREATE_NEW          1
#define OEM_CREATE_ALWAYS       2
#define OEM_OPEN_EXISTING       3
#define OEM_OPEN_ALWAYS         4
#define OEM_TRUNCATE_EXISTING   5

#define OEM_ATTRIBUTE_HIDDEN    0x00000002
#define OEM_ATTRIBUTE_SYSTEM    0x00000004
#define OEM_ATTRIBUTE_NORMAL    0x00000080

/* SetFilePointer move methods */

#define OEM_FILE_BEGIN          0
#define OEM_FILE_CURRENT        1
#define OEM_FILE_END            2

/* For information on these functions and parameters see MSDN 
   For OEM_OpenFile see CreateFile in MSDN -- not all flags are supported.*/

/**********************************************************************
** Function:    OEM_OpenFile
** Synopsis:    Creates, opens, reopens a file
** Arguments:   [pwFileName]--Pointer to DRM_WCHAR buffer holding File
**              name.
**              [dwAccessMode]--Type of access to the object. 
**              OEM_GENERIC_READ, OEM_GENERIC_WRITE,
**              OEM_GENERIC_EXECUTE and OEM_GENERIC_ALL
**              [dwShareMode]--Sharing mode of the object
**              OEM_FILE_SHARE_NONE, OEM_FILE_SHARE_READ
**              and OEM_FILE_SHARE_WRITE
**              [dwCreationDisposition]--Action to take on files 
**              that exist, and on files that do not exist.
**              OEM_CREATE_NEW, OEM_CREATE_ALWAYS, OEM_OPEN_EXISTING
**              OEM_OPEN_ALWAYS and OEM_TRUNCATE_EXISTING
**              [dwAttributes]--File attributes and flags.
**              OEM_ATTRIBUTE_HIDDEN, OEM_ATTRIBUTE_SYSTEM and
**              OEM_ATTRIBUTE_NORMAL
** Returns:     Valid OEM FILE HANDLE. If fails, 
**              returns OEM_INVALID_HANDLE_VALUE 
***********************************************************************/

OEM_FILEHDL DRM_API OEM_OpenFile(
    const DRM_WCHAR *pwFileName,
    DRM_DWORD        dwAccessMode,
    DRM_DWORD        dwShareMode,
    DRM_DWORD        dwCreationDisposition,
    DRM_DWORD        dwAttributes);

/**********************************************************************
** Function:    OEM_CloseFile
** Synopsis:    Closes an open handle opened by OEM_OpenFile.
** Arguments:   [hFile]--File Handle
** Returns:     Non zero value if succeeds, zero if failed.
***********************************************************************/
DRM_BOOL DRM_API OEM_CloseFile(
    OEM_FILEHDL hFile);

/**********************************************************************
** Function:    OEM_ReadFile
** Synopsis:    Reads data from a file.
** Arguments:   [hFile]--File Handle
**              [pvBuffer]--Pointer to the buffer that receives the 
**              data read from the file. 
**              [nNumberOfBytesToRead]--Number of bytes to read.
**              [pNumberOfBytesRead]--Total number of bytes read. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_BOOL DRM_API OEM_ReadFile(
    OEM_FILEHDL hFile,
    DRM_VOID   *pvBuffer,
    DRM_DWORD   nNumberOfBytesToRead,
    DRM_DWORD  *pNumberOfBytesRead);

/**********************************************************************
** Function:    OEM_WriteFile
** Synopsis:    Writes data to a file.
** Arguments:   [hFile]--File Handle
**              [pvBuffer]--Pointer to the buffer holding the 
**              data read to the file. 
**              [nNumberOfBytesToWrite]--Number of bytes to write.
**              [pNumberOfBytesWritten]--Total number of bytes written. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_WriteFile(
    OEM_FILEHDL hFile,
    DRM_VOID   *pvBuffer,
    DRM_DWORD   nNumberOfBytesToWrite,
    DRM_DWORD  *pNumberOfBytesWritten);

/**********************************************************************
** Function:    OEM_SetFilePointer
** Synopsis:    Sets File pointer.
** Arguments:   [lDistanceToMove]--Number of bytes to move
**              [dwMoveMethod]--Starting point for the file pointer move 
**              OEM_FILE_BEGIN, OEM_FILE_CURRENT and  OEM_FILE_END
**              [pdwNewFilePointer]--New File pointer.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_SetFilePointer(
    OEM_FILEHDL hFile,
    DRM_LONG    lDistanceToMove,
    DRM_DWORD   dwMoveMethod,
    DRM_DWORD  *pdwNewFilePointer);

/**********************************************************************
** Function:    OEM_LockFile
** Synopsis:    Immidiately locks the portion of specified file. 
** Arguments:   [hFile]--File Handle
**              [fExclusive]-- If TRUE, locks file for exclusive access  
**              by the calling process.
**              [dwFileOffset]--Offset from begining of file.
**              [nNumberOfBytesToLock]--Number of bytes to lock. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_LockFile(
    OEM_FILEHDL hFile,
    DRM_BOOL    fExclusive,
    DRM_DWORD   dwFileOffset,
    DRM_DWORD   nNumberOfBytesToLock,
    DRM_BOOL    fWait);

/**********************************************************************
** Function:    OEM_UnlockFile
** Synopsis:    Unlocks the locked portion of specified file. 
** Arguments:   [hFile]--File Handle
**              [dwFileOffset]--Offset from begining of file.
**              [nNumberOfBytesToLock]--Number of bytes to lock. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_UnlockFile(
    OEM_FILEHDL hFile,
    DRM_DWORD   dwFileOffset,
    DRM_DWORD   nNumberOfBytesToUnlock);

/**********************************************************************
** Function:    OEM_SetEndOfFile
** Synopsis:    Moves the end-of-file (EOF) position for the 
**              specified file to the current position of the file 
**              pointer. 
** Arguments:   [hFile]--File Handle
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_SetEndOfFile(
    OEM_FILEHDL hFile);

/**********************************************************************
** Function:    OEM_GetFileSize
** Synopsis:    Gets size of the file. 
** Arguments:   [hFile]--File Handle
**              [pdwFileSize]--Pointer to DRM_DWORD to get the size.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_GetFileSize(
    OEM_FILEHDL hFile,
    DRM_DWORD  *pdwFileSize);

/**********************************************************************
** Function:    OEM_FlushFileBuffers
** Synopsis:    Flushes the buffers of the specified file and causes 
**              all buffered data to be written to the file. 
** Arguments:   [hFile]--File Handle
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_FlushFileBuffers(
    OEM_FILEHDL hFile);


/* Memory allocation functions */

/**********************************************************************
** Function:    OEM_malloc
** Synopsis:    Allocates memory from heap. 
** Arguments:   [size]--Number of bytes to allocate.
** Returns:     Valid pointer if succeeds, NULL if failed. 
***********************************************************************/

DRM_VOID* DRM_API OEM_malloc(
    DRM_DWORD size);

/**********************************************************************
** Function:    OEM_free
** Synopsis:    Frees allocated memory. 
** Arguments:   [pv]--Pointer to memory buffer.
***********************************************************************/

DRM_VOID  DRM_API OEM_free( 
    DRM_VOID* pv );

#define SAFE_OEM_FREE(p) \
    if (p != NULL) \
    { \
        OEM_free((DRM_VOID *) p); \
        p = NULL; \
    }



/**********************************************************************
** Function:    OEM_realloc
** Synopsis:    Re-allocates memory from heap. 
** Arguments:   [pv]--Pointer to previous allocated memory buffer.
**              [size]--Number of bytes to allocate.
** Returns:     Valid pointer if succeeds, NULL if failed. 
***********************************************************************/

DRM_VOID* DRM_API OEM_realloc(
    DRM_VOID*pv, DRM_DWORD size);


/* Time functions -- See MSDN for more information */

/**********************************************************************
** Function:    OEM_SystemTimeToFileTime
** Synopsis:    Converts System Time format to File time format. 
** Arguments:   [lpSystemTime]--Pointer to DRMSYSTEMTIME structure 
**              containing system time
**              [lpFileTime]--Pointer to DRMFILETIME structure to get
**              the time.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_SystemTimeToFileTime(
    IN const DRMSYSTEMTIME *lpSystemTime,
    OUT      DRMFILETIME   *lpFileTime
    );

/**********************************************************************
** Function:    OEM_FileTimeToSystemTime
** Synopsis:    Converts File time format to System Time format. 
** Arguments:   [lpFileTime]--Pointer to DRMFILETIME structure 
**              containing file time
**              [lpSystemTime]--Pointer to DRMSYSTEMTIME structure to 
**              get the time.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/

DRM_BOOL DRM_API OEM_FileTimeToSystemTime(
    IN const DRMFILETIME   *lpFileTime,
    OUT      DRMSYSTEMTIME *lpSystemTime
    );

/**********************************************************************
** Function:    OEM_GetDeviceDateTime
** Synopsis:    Gets current System time. It is expressed in UTC. 
** Arguments:   [lpSystemTime]--Pointer to DRMSYSTEMTIME structure 
**              to get the time.
***********************************************************************/

DRM_VOID DRM_API OEM_GetDeviceDateTime(
    OUT DRMSYSTEMTIME* lpSystemTime
    );

/**********************************************************************
** Function:    OEM_GetDeviceTime
** Synopsis:    Gets current System time. It is expressed in UTC. 
** Arguments:   [pfiletime]--Pointer to DRMFILETIME structure 
**              to get the time.
***********************************************************************/

DRM_VOID DRM_API OEM_GetDeviceTime(
    OUT DRMFILETIME *pfiletime);

/**********************************************************************
** Function:    OEM_SetSystemTime
** Synopsis:    Sets the System time. It is expressed in UTC. 
** Arguments:   [lpSystemTime]--Pointer to DRMSYSTEMTIME structure 
**              holding the time.
***********************************************************************/

DRM_VOID DRM_API OEM_SetSystemTime(
    IN DRMSYSTEMTIME* lpSystemTime
    );

/**********************************************************************
** Function:    OEM_InitializeCriticalSection
** Synopsis:    Initializes critical section. 
** Arguments:   [pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              initialized.
***********************************************************************/

DRM_VOID DRM_API OEM_InitializeCriticalSection(
    IN OUT OEM_CRITICAL_SECTION *pCS);

/**********************************************************************
** Function:    OEM_DeleteCriticalSection
** Synopsis:    Deletes existing critical section. 
** Arguments:   [pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              deleted.
***********************************************************************/

DRM_VOID DRM_API OEM_DeleteCriticalSection(
    IN OUT OEM_CRITICAL_SECTION *pCS);

/**********************************************************************
** Function:    OEM_EnterCriticalSection
** Synopsis:    Enters critical section. 
** Arguments:   [pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              entered.
***********************************************************************/

DRM_VOID DRM_API OEM_EnterCriticalSection(
    IN OUT OEM_CRITICAL_SECTION *pCS);

/**********************************************************************
** Function:    OEM_LeaveCriticalSection
** Synopsis:    Leaves critical section. 
** Arguments:   [pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              left.
***********************************************************************/

DRM_VOID DRM_API OEM_LeaveCriticalSection(
    IN OUT OEM_CRITICAL_SECTION *pCS);


/**********************************************************************
** Function:    OEM_GetSetDataStoreRedundancy
** Synopsis:    Leaves critical section. 
** Arguments:   
***********************************************************************/

DRM_RESULT DRM_API OEM_GetSetDataStoreRedundancy(
    IN     DRM_VOID  *f_pvOpaqueData,
    IN OUT DRM_BYTE  *f_pbKey,
    IN     DRM_DWORD  f_cbKey,
    IN     DRM_BOOL   f_fGet);


#if DRM_SUPPORT_PROFILING
/**********************************************************************
** Function:    OEM_GetProfileLogPath
** Synopsis:    Get the path to the profiling log
** Arguments:   
***********************************************************************/

DRM_RESULT DRM_API OEM_GetProfileLogPath(
       OUT DRM_WCHAR *f_rgwchPath, 
    IN     DRM_DWORD *f_pcchPath);

#endif



/*************************  RSA SUPPORT *******************************/

#define MAX_RSA_PUB_KEY_BIT_LEN  4096
#define MAX_RSA_PUB_KEY_BYTE_LEN (MAX_RSA_PUB_KEY_BIT_LEN / 8 )

#define CB_SIGNATURE_RSA 128
#define CCH_SIGNATURE_RSA CCH_BASE64_EQUIV(CB_SIGNATURE_RSA)
#define CB_BSAFE_KEY_MIN 20 /* BSAFE header is five DWORDs */

/*
 * RSA public key
 *
 * The actual contents of the structure is opaque.
 *
 * The opaque data is represented as a byte array below.
 * Don't access the array or assume the size of this structure.
 */
typedef struct
{
    DRM_BYTE opaque[__CB_DECL(CB_BSAFE_KEY_MIN)];
} DRM_RSA_PUBLIC_KEY;

/*
 * RSA private key
 *
 * The actual contents of the structure is opaque.
 *
 * The opaque data is represented as a byte array below.
 * Don't access the array or assume the size of this structure.
 */
typedef struct
{
    DRM_BYTE opaque[__CB_DECL(CB_BSAFE_KEY_MIN)];
} DRM_RSA_PRIVATE_KEY;

enum DRM_RSA_KEY_TYPE
{
    DRM_RSA_KEY_TYPE_UNKNOWN,
    DRM_RSA_KEY_TYPE_PUBLIC,
    DRM_RSA_KEY_TYPE_PRIVATE
};

typedef struct
{
    DRM_RSA_PUBLIC_KEY  *pkeyRSAPublic;
    DRM_RSA_PRIVATE_KEY *pkeyRSAPrivate;
} DRM_KEYPAIR_RSA;

typedef struct
{
    PUBKEY  keyECCPublic;
    PRIVKEY keyECCPrivate;
} DRM_KEYPAIR_ECC;

/*
** The OEM_DrmRsa_IdentifyKey identifies a pointer as a public key, private key, or not a key
**
** Parameters:
**
**      f_pvKey - pointer to either form of RSA key
*/

enum DRM_RSA_KEY_TYPE OEM_DrmRsaIdentifyKey(
    IN const DRM_VOID *f_pvKey);

/*
 * The DrmRsaSetPublicKey function allocates an internal form of an RSA public key
 *
 * Parameters:
 *
 *      dwPubKeyExp - Specifies the exponent of the RSA public key
 *
 *      pbPubKeyMod - Specifies the modulus of the RSA public key
 *
 *      cbPubKeyMod - Speicifies the size (in bytes) of the modulus of the RSA public key
 *
 *      pPublicKey - Returns a pointer to the internal form of an RSA public key
 *          Buffer must be freed using OEM_free()
 *
 * Return Value:
 *  DRM_SUCCESS - Operation completed successfully
 *  DRM_E_OUTOFMEMORY - There is no enough memory to complete the operation
 */

DRM_RESULT DRM_API OEM_DrmRsaSetPublicKey(
    IN       DRM_DWORD dwPubKeyExp,
    IN       DRM_BYTE *pbPubKeyMod,
    IN       DRM_DWORD cbPubKeyMod,
       OUT   DRM_RSA_PUBLIC_KEY **pPublicKey );

/*
 * The DrmRsaSetPrivateKey function allocates an internal form of an RSA private key
 *
 * Parameters:
 *
 *      pbPrivateKey - Specifies the RSA private key.
 *          This buffer is in an OEM specific format.
 *          The format of this buffer is not necessarily the same as the one returned.
 *
 *      cbPrivateKey - Specifies the size (in bytes) of the RSA private key
 *
 *      ppPrivateKey - Returns the RSA private key.
 *          Buffer must be freed using OEM_free()
 *
 * Return Value:
 *  DRM_SUCCESS - Operation completed successfully
 *  DRM_E_OUTOFMEMORY - There is no enough memory to complete the operation
 */

DRM_RESULT DRM_API OEM_DrmRsaSetPrivateKey(
    IN       DRM_BYTE *pbPrivateKey,
    IN       DRM_DWORD cbPrivateKey,
       OUT   DRM_RSA_PRIVATE_KEY **ppPrivateKey );

/*
 * The DrmRsaKeySize routine determines the key size for the specific key.
 *
 * The key size is the size of the buffer used during encryption or decryption with the key.
 * A buffer of this size should be passed into the DrmRsaEncPublic and DrmRsaDecPrivate routines.
 *
 * Notice that this key size shouldn't be used for any other purpose.  Specifically,
 * note that this size may be larger then DrmRsaModulusSize.
 *
 * Parameters:
 *
 *      pKey - Specifies the key to determine the size of
 *          This key must be either of type DRM_RSA_PUBLIC_KEY or DRM_RSA_PRIVATE_KEY.
 *          NULL indicates that the private key baked into the device should be used
 *
 * Return Value:
 *      Size of the key in bytes
 *      0 indicates that the key isn't properly formatted
 */

DRM_DWORD DRM_API OEM_DrmRsaKeySize( const DRM_VOID *pKey );

/*
 * The OEM_DrmRsaBufferSize routine determines the size for the specific key.
 *
 * Parameters:
 *
 *      f_pvKey - Specifies the key to determine the size of
 *          This key must be either of type DRM_RSA_PUBLIC_KEY or DRM_RSA_PRIVATE_KEY.
 *
 *      f_pcbKey- on success this is filled in with the buffer size for the key
 *
 * Notes: while public keys are intrinsically self-describing, RSA private keys
 *        only reveal their total size by the implicit relationship between the
 *        size of the public key modulus and the buffer size
 */

DRM_RESULT DRM_API OEM_DrmRsaGetBufferSize (IN  const DRM_VOID  *f_pvKey,
                                            OUT       DRM_DWORD *f_pcbKey);

/*
 * The DrmRsaModulusSize routine determines the modulus size for the specific key.
 *
 * The modulus size represents the publicly visible strength of the key.
 *
 * Parameters:
 *
 *      pKey - Specifies the key to determine the modulus size of
 *          This key must be either of type DRM_RSA_PUBLIC_KEY or DRM_RSA_PRIVATE_KEY.
 *          NULL indicates that the private key baked into the device should be used
 *
 * Return Value:
 *      Modulus size of the key in bytes
 *      0 indicates that the key isn't properly formatted
 */

DRM_DWORD DRM_API OEM_DrmRsaModulusSize( DRM_VOID *pKey );



/*
 * The DrmRsaEncPublic routine RSA encrypts a fixed size buffer with an RSA public key
 *
 * Parameters:
 *
 *      pKey - Specifies the public key
 *
 *      pbDataIn - Specifies the input data
 *          The array must be DrmRsaKeySize bytes long.
 *          Only the first DrmRsaModulusSize bytes are encrypted.
 *          Any unused bytes in the array should be zeroed.
 *
 *      pbDataOut - Specifies the output data
 *          The array must be DrmRsaKeySize() bytes long.
 *          Only the first DrmRsaModulusSize bytes represent the encrypted value.
 *
 * Return Value:
 *      TRUE - encryption succeeded.
 *      FALSE - encryption failed.
 */

DRM_BOOL DRM_API OEM_DrmRsaEncPublic(
    IN       DRM_RSA_PUBLIC_KEY *pKey,
    IN       DRM_BYTE           *pbDataIn,
       OUT   DRM_BYTE           *pbDataOut );

/*
 * The DrmRsaDecPrivate routine RSA decrypts a fixed size buffer with an RSA private key
 *
 * Parameters:
 *
 *      pKey - Specifies the private key
 *          NULL indicates that the private key baked into the device should be used
 *
 *      pbDataIn - Specifies the input data
 *          The array must be DrmRsaKeySize bytes long.
 *          Only the first DrmRsaModulusSize bytes are encrypted.
 *          Any unused bytes in the array should be zeroed.
 *
 *      pbDataOut - Specifies the output data
 *          The array must be DrmRsaKeySize() bytes long.
 *
 * Return Value:
 *      TRUE - decryption succeeded.
 *      FALSE - decryption failed.
 */

DRM_BOOL DRM_API OEM_DrmRsaDecPrivate(
    IN       DRM_RSA_PRIVATE_KEY *pKey,
    IN       DRM_BYTE            *pbDataIn,
       OUT   DRM_BYTE            *pbDataOut );

DRM_RESULT OEM_DrmRsaGenKeys(
    IN        DRM_DWORD             f_cBits,
    IN        DRM_DWORD             f_exponentPublic,
    OUT       DRM_RSA_PUBLIC_KEY  **f_ppkeyRSAPublic,
    OUT       DRM_DWORD            *f_pcbPublic,
    OUT       DRM_RSA_PRIVATE_KEY **f_ppkeyRSAPrivate,
    OUT       DRM_DWORD            *f_pcbPrivate);

DRM_RESULT DRM_API OEM_DrmRsaParsePublicKey(
    IN          DRM_RSA_PUBLIC_KEY *f_pkeyRSAPublic,
    OUT         DRM_DWORD          *f_pdwExponent,
    OUT         DRM_BYTE          **f_ppbModulus,
    OUT         DRM_DWORD          *f_pcbModulus);

DRM_RESULT DRM_API OEM_DrmRsaParsePrivateKey(
    IN    DRM_RSA_PRIVATE_KEY *f_pkeyRSAPrivate,
    OUT   DRM_DWORD           *f_pdwExponent,
    OUT   DRM_BYTE           **f_ppbModulus,
    OUT   DRM_DWORD           *f_pcbModulus,
    OUT   DRM_BYTE           **f_ppbPrivate,
    OUT   DRM_DWORD           *f_pcbPrivate);

/*
 * The DrmRsaReleaseKey routines free keys created with OEM_DrmRsaSetPublicKey and OEM_DrmRsaSetPrivateKey
 *
 * Parameters:
 *
 *      Specifies the key
 *
 * Return Value:
 *      DRM_E_INVALIDARG - the given pointer does not point to a valid RSA key struct
 */

DRM_RESULT DRM_API OEM_DrmRsaReleaseKey(
       IN DRM_VOID *f_pPrivateKey);


/* TBD: write a real function for this that cannot be optimized away */

#define OEM_SecureZeroMemory(pb,cb) ZEROMEM(pb,cb)



#define SAFE_SECURE_OEM_FREE(p, c) \
    if (p != NULL) \
    { \
        OEM_SecureZeroMemory(p,c);  \
        OEM_free((DRM_VOID *) p); \
        p = NULL; \
    }

    

#ifdef __cplusplus
}
#endif

#endif /* __OEMIMPL_H__ */
