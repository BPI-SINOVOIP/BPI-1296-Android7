/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMCOMMON_H__
#define __OEMCOMMON_H__

#include <drmcommon.h>
#include <drmhash.h>
#include <drmpkcrypto.h>
#include <drmbase64.h>

ENTER_PK_NAMESPACE;

extern DRM_EXPORT_VAR DRM_CONST_STRING g_dstrDrmPath;

extern const DRM_CONST_STRING g_dstrPrivKey;
extern const DRM_CONST_STRING g_dstrZGPrivKey;
extern const DRM_CONST_STRING g_dstrZPrivKeySign;
extern const DRM_CONST_STRING g_dstrZPrivKeyEncrypt;
extern const DRM_CONST_STRING g_dstrNDRPrivKey;

extern const DRM_CONST_STRING g_dstrDevcert;
extern const DRM_CONST_STRING g_dstrDevcerttemplate;

extern const DRM_CONST_STRING g_dstrZDevcert;
extern const DRM_CONST_STRING g_dstrZGroupCert;

extern const DRM_CONST_STRING g_dstrReceiverCert;
extern const DRM_CONST_STRING g_dstrReceiverTemplate;

extern const DRM_CONST_STRING g_dstrDevcert;
extern const DRM_CONST_STRING g_dstrDevcerttemplate;

extern const DRM_CONST_STRING g_dstrReceiverCert;
extern const DRM_CONST_STRING g_dstrReceiverTemplate;

typedef enum DRMFILESPATH
{
    CERTPATH = 0,
    CERTTEMPLATEPATH,
    HDSPATH,
} DRMFILESPATH;

typedef enum
{
    DRM_DCT_WMDRMPD = 1,
    DRM_DCT_WMDRMPD_TEMPLATE,
    DRM_DCT_PLAYREADY,
    DRM_DCT_PLAYREADY_TEMPLATE,
    DRM_DCT_NETRECEIVER,
    DRM_DCT_NETRECEIVER_TEMPLATE,
} DRM_DEVICE_CERT_TYPE;

typedef enum
{
    DRM_DKT_WMDRMPD_GROUP = 1,
    DRM_DKT_WMDRMPD_FALLBACK,
    DRM_DKT_PLAYREADY_DEVICE_SIGN,
    DRM_DKT_PLAYREADY_DEVICE_ENCRYPT,
    DRM_DKT_PLAYREADY_MODEL,
    DRM_DKT_PLAYREADY_NDR,
    DRM_DKT_PLAYREADY_NDR_MODEL
} DRM_DEVICE_KEY_TYPE;

typedef enum
{
    DRM_DIT_WMDRMPD_ID = 1,
    DRM_DIT_PLAYREADY_ID,
} DRM_DEVICE_ID_TYPE;

typedef DRM_VOID * OEM_DEVICE_HANDLE;

/*
** Internal functions that should be implemented by each OEM
** implementation (oeminfo.c).
*/
DRM_API DRM_RESULT DRM_CALL _GetDRMFullPathName( 
    __out DRM_WCHAR **f_ppwszFullPath, 
    __in_opt const DRM_CONST_STRING *f_pdstrFilename,
    __in DRMFILESPATH f_eFilesPath );
    
DRM_RESULT _GetPrivateKey(
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_BOOL f_fGCprivkey,
    __out_bcount( f_cbPrivKey ) DRM_BYTE *f_pbPrivkey,
    __in DRM_DWORD f_cbPrivKey );

DRM_RESULT _GetHWID(
    __out_bcount(DRM_SHA1_DIGEST_LEN) DRM_BYTE *shaOut );

/* OEM device information functions. (oeminfo.c) */
DRM_API DRM_RESULT DRM_CALL Oem_Device_GetCert(
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_DEVICE_CERT_TYPE f_eCertType,
    __out_bcount_opt( *f_pcbDevCert ) DRM_BYTE *f_pbDevCert,
    __inout DRM_DWORD *f_pcbDevCert );

DRM_API DRM_RESULT DRM_CALL Oem_Device_SetCert(
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_DEVICE_CERT_TYPE f_eCertType,
    __in_bcount(f_cbDevCert) DRM_BYTE *f_pbDevCert,
    __in const DRM_DWORD f_cbDevCert );

DRM_API DRM_RESULT DRM_CALL Oem_Device_GetSecureStoreGlobalPasswordSeed( 
    __out_bcount(DRM_SHA1_DIGEST_LEN) DRM_BYTE f_rgbSecStorePassword[ __CB_DECL( DRM_SHA1_DIGEST_LEN ) ] );

DRM_API DRM_RESULT DRM_CALL Oem_Device_GetKey( 
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_DEVICE_KEY_TYPE f_eKeyType,
    __out_bcount_opt( *f_pcbKey ) DRM_BYTE *f_pbKey,
    __inout DRM_DWORD *f_pcbKey );

DRM_API DRM_RESULT DRM_CALL Oem_Device_SetKey( 
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_DEVICE_KEY_TYPE f_eKeyType,
    __in_bcount(f_cbKey) DRM_BYTE *f_pbKey,
    __in const DRM_DWORD f_cbKey );

DRM_API DRM_RESULT DRM_CALL Oem_Device_GetUniqueId( 
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_DEVICE_ID_TYPE f_eIDType,
    __out_bcount_opt(*f_pcbLength) DRM_BYTE *f_pbUniqueId, 
    __inout DRM_DWORD *f_pcbLength );

DRM_API DRM_RESULT DRM_CALL Oem_Clock_GetResetState( 
    __in_opt DRM_VOID *f_pOEMContext, 
    __out DRM_BOOL *f_pfReset );

DRM_API DRM_RESULT DRM_CALL Oem_Clock_SetResetState( 
    __in_opt DRM_VOID *f_pOEMContext, 
    __in DRM_BOOL f_fReset );


/*****************************************************************************
** Function:    Oem_Clock_GetTickCount
**
** Synopsis:    Returns the number of milliseconds since the system was started.
**
** Arguments:   [f_pOEMContext] : A pointer to the OEM context; may be NULL
**
** Returns:     DRM_DWORD - the number of milliseconds since the system was started
**
** Notes:       None.
**
******************************************************************************/
DRM_API DRM_DWORD DRM_CALL Oem_Clock_GetTickCount(
__in_opt    DRM_VOID      *f_pOEMContext
);


/* OEM file IO functions (oemfileio.c). */

/* File Handle */
typedef DRM_VOID *OEM_FILEHDL;
#define OEM_INVALID_HANDLE_VALUE ((OEM_FILEHDL)-1)

/* Oem_File_Open Access modes */
#define OEM_GENERIC_READ       (0x80000000L)
#define OEM_GENERIC_WRITE      (0x40000000L)
#define OEM_GENERIC_EXECUTE    (0x20000000L)
#define OEM_GENERIC_ALL        (0x10000000L)

/* Oem_File_Open Share modes */
#define OEM_FILE_SHARE_NONE     0x00000000  
#define OEM_FILE_SHARE_READ     0x00000001  
#define OEM_FILE_SHARE_WRITE    0x00000002  

/* Oem_File_Open Creation dispositions */
#define OEM_CREATE_NEW          1
#define OEM_CREATE_ALWAYS       2
#define OEM_OPEN_EXISTING       3
#define OEM_OPEN_ALWAYS         4
#define OEM_TRUNCATE_EXISTING   5

#define OEM_ATTRIBUTE_HIDDEN    0x00000002
#define OEM_ATTRIBUTE_SYSTEM    0x00000004
#define OEM_ATTRIBUTE_NORMAL    0x00000080

#define OEM_FILE_FLAG_WRITE_THROUGH 0x80000000

/* SetFilePointer move methods */
#define OEM_FILE_BEGIN          0
#define OEM_FILE_CURRENT        1
#define OEM_FILE_END            2

/*
** For information on these functions and parameters see MSDN 
** For Oem_File_Open see CreateFile in MSDN -- not all flags are supported.
*/

/**********************************************************************
** Function:    Oem_File_Open
** Synopsis:    Creates, opens, reopens a file
** Arguments:   [f_pOEMContext]--Optional pointer to OEM specific context data
**              [f_pwszFileName]--Pointer to DRM_WCHAR buffer holding File
**              name.
**              [f_dwAccessMode]--Type of access to the object. 
**              OEM_GENERIC_READ, OEM_GENERIC_WRITE,
**              OEM_GENERIC_EXECUTE and OEM_GENERIC_ALL
**              [f_dwShareMode]--Sharing mode of the object
**              OEM_FILE_SHARE_NONE, OEM_FILE_SHARE_READ
**              and OEM_FILE_SHARE_WRITE
**              [f_dwCreationDisposition]--Action to take on files 
**              that exist, and on files that do not exist.
**              OEM_CREATE_NEW, OEM_CREATE_ALWAYS, OEM_OPEN_EXISTING
**              OEM_OPEN_ALWAYS and OEM_TRUNCATE_EXISTING
**              [f_dwAttributes]--File attributes and flags.
**              OEM_ATTRIBUTE_HIDDEN, OEM_ATTRIBUTE_SYSTEM and
**              OEM_ATTRIBUTE_NORMAL
** Returns:     Valid OEM FILE HANDLE. If fails, 
**              returns OEM_INVALID_HANDLE_VALUE 
***********************************************************************/
DRM_API OEM_FILEHDL DRM_CALL Oem_File_Open(
    __in_opt DRM_VOID *f_pOEMContext,
    __in_z const DRM_WCHAR *f_pwszFileName,
    __in DRM_DWORD f_dwAccessMode,
    __in DRM_DWORD f_dwShareMode,
    __in DRM_DWORD f_dwCreationDisposition,
    __in DRM_DWORD f_dwAttributes );

/**********************************************************************
** Function:    Oem_File_Close
** Synopsis:    Closes an open handle opened by Oem_File_Open.
** Arguments:   [f_hFile]--File Handle
** Returns:     Non zero value if succeeds, zero if failed.
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_Close(
    __in OEM_FILEHDL f_hFile );

/**********************************************************************
** Function:    Oem_File_Read
** Synopsis:    Reads data from a file.
** Arguments:   [f_hFile]--File Handle
**              [f_pvBuffer]--Pointer to the buffer that receives the 
**              data read from the file. 
**              [f_nNumberOfBytesToRead]--Number of bytes to read.
**              [f_pNumberOfBytesRead]--Total number of bytes read. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_Read(
    __in OEM_FILEHDL f_hFile,
    __out_ecount_part( f_nNumberOfBytesToRead, *f_pNumberOfBytesRead ) DRM_VOID *f_pvBuffer,
    __in DRM_DWORD f_nNumberOfBytesToRead,
    __out DRM_DWORD *f_pNumberOfBytesRead );

/**********************************************************************
** Function:    Oem_File_Write
** Synopsis:    Writes data to a file.
** Arguments:   [f_hFile]--File Handle
**              [f_pvBuffer]--Pointer to the buffer holding the 
**              data read to the file. 
**              [f_nNumberOfBytesToWrite]--Number of bytes to write.
**              [f_pNumberOfBytesWritten]--Total number of bytes written. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_Write(
    __in OEM_FILEHDL f_hFile,
    __in_ecount( f_nNumberOfBytesToWrite ) DRM_VOID *f_pvBuffer,
    __in DRM_DWORD f_nNumberOfBytesToWrite,
    __out DRM_DWORD *f_pNumberOfBytesWritten );

/**********************************************************************
** Function:    Oem_File_SetFilePointer
** Synopsis:    Sets File pointer.
** Arguments:   [f_lDistanceToMove]--Number of bytes to move
**              [f_dwMoveMethod]--Starting point for the file pointer move 
**              OEM_FILE_BEGIN, OEM_FILE_CURRENT and  OEM_FILE_END
**              [f_pdwNewFilePointer]--New File pointer.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_SetFilePointer(
    __in OEM_FILEHDL f_hFile,
    __in DRM_LONG f_lDistanceToMove,
    __in DRM_DWORD f_dwMoveMethod,
    __out_opt DRM_DWORD *f_pdwNewFilePointer );

/**********************************************************************
** Function:    Oem_File_Lock
** Synopsis:    Immidiately locks the portion of specified file. 
** Arguments:   [f_hFile]--File Handle
**              [f_fExclusive]-- If TRUE, locks file for exclusive access  
**              by the calling process.
**              [f_dwFileOffset]--Offset from begining of file.
**              [f_nNumberOfBytesToLock]--Number of bytes to lock. 
**              [f_fWait]--Whether to wait for the lock to complete.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_Lock(
    __in OEM_FILEHDL f_hFile,
    __in DRM_BOOL f_fExclusive,
    __in DRM_DWORD f_dwFileOffset,
    __in DRM_DWORD f_nNumberOfBytesToLock,
    __in DRM_BOOL f_fWait );

/**********************************************************************
** Function:    Oem_File_Unlock
** Synopsis:    Unlocks the locked portion of specified file. 
** Arguments:   [f_hFile]--File Handle
**              [f_dwFileOffset]--Offset from begining of file.
**              [f_nNumberOfBytesToLock]--Number of bytes to lock. 
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_Unlock(
    __in OEM_FILEHDL f_hFile,
    __in DRM_DWORD f_dwFileOffset,
    __in DRM_DWORD f_nNumberOfBytesToUnlock );

/**********************************************************************
** Function:    Oem_File_SetEndOfFile
** Synopsis:    Moves the end-of-file (EOF) position for the 
**              specified file to the current position of the file 
**              pointer. 
** Arguments:   [f_hFile]--File Handle
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_SetEndOfFile(
    __in OEM_FILEHDL f_hFile);

/**********************************************************************
** Function:    Oem_File_GetSize
** Synopsis:    Gets size of the file. 
** Arguments:   [f_hFile]--File Handle
**              [f_pdwFileSize]--Pointer to DRM_DWORD to get the size.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_GetSize(
    __in OEM_FILEHDL f_hFile,
    __out DRM_DWORD  *f_pdwFileSize );

/**********************************************************************
** Function:    Oem_File_FlushBuffers
** Synopsis:    Flushes the buffers of the specified file and causes 
**              all buffered data to be written to the file. 
** Arguments:   [f_hFile]--File Handle
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_File_FlushBuffers(
    __in OEM_FILEHDL f_hFile );

/* OEM time functions (oemtime.c). */

/**********************************************************************
** Function:    Oem_Clock_SystemTimeToFileTime
** Synopsis:    Converts System Time format to File time format. 
** Arguments:   [f_lpSystemTime]--Pointer to DRMSYSTEMTIME structure 
**              containing system time
**              [f_lpFileTime]--Pointer to DRMFILETIME structure to get
**              the time.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_Clock_SystemTimeToFileTime(
    __in const DRMSYSTEMTIME *f_lpSystemTime,
    __out DRMFILETIME *f_lpFileTime );

/**********************************************************************
** Function:    Oem_Clock_FileTimeToSystemTime
** Synopsis:    Converts File time format to System Time format. 
** Arguments:   [f_lpFileTime]--Pointer to DRMFILETIME structure 
**              containing file time
**              [f_lpSystemTime]--Pointer to DRMSYSTEMTIME structure to 
**              get the time.
** Returns:     Non zero value if succeeds, zero if failed. 
***********************************************************************/
DRM_API DRM_BOOL DRM_CALL Oem_Clock_FileTimeToSystemTime(
    __in const DRMFILETIME *f_lpFileTime,
    __out DRMSYSTEMTIME *f_lpSystemTime );

/**********************************************************************
** Function:    Oem_Clock_GetSystemTime
** Synopsis:    Gets current System time. It is expressed in UTC. 
** Arguments:   [f_pOEMContext]--OEM specific data
**              [f_lpSystemTime]--Pointer to DRMSYSTEMTIME structure 
**              to get the time.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_Clock_GetSystemTime(
    __in_opt    DRM_VOID      *f_pOEMContext,
    __out       DRMSYSTEMTIME *f_psystime );

/**********************************************************************
** Function:    Oem_Clock_GetSystemTimeAsFileTime
** Synopsis:    Gets current System time. It is expressed in UTC. 
** Arguments:   [f_pOEMContext]--OEM specific data
**              [f_pfiletime]--Pointer to DRMFILETIME structure 
**              to get the time.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_Clock_GetSystemTimeAsFileTime(
    __in_opt    DRM_VOID    *f_pOEMContext,
    __out       DRMFILETIME *f_pfiletime );

/**********************************************************************
** Function:    Oem_Clock_SetSystemTime
** Synopsis:    Sets the System time. It is expressed in UTC. 
** Arguments:   [f_pOEMContext]--OEM specific data
**              [f_lpSystemTime]--Pointer to DRMSYSTEMTIME structure 
**              holding the time.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_Clock_SetSystemTime(
    __in_opt    DRM_VOID      *f_pOEMContext,
    __in        DRMSYSTEMTIME *f_lpSystemTime );

/* OEM memory allocation functions (oemimpl.c). */

DRM_API DRM_VOID * DRM_CALL Oem_MemAlloc(
    __in DRM_DWORD f_cbSize );

DRM_API DRM_VOID DRM_CALL Oem_MemFree(
    __in DRM_VOID *f_pv );

#define SAFE_OEM_FREE( p )            \
    if ( p != NULL )                  \
    {                                 \
        Oem_MemFree( ( DRM_VOID * )p );  \
        p = NULL;                     \
    }

DRM_API DRM_VOID * DRM_CALL Oem_MemRealloc( 
    __inout DRM_VOID  *f_pv, 
    __in DRM_DWORD f_cbSize );
    
/* OEM special implementation functions (oemimpl.c). */
#ifdef _MSC_VER

__forceinline DRM_VOID * DRM_CALL DRMCRT_ScrubMemory(
    __in_ecount( f_cbCount ) DRM_VOID  *f_ptr,
    IN DRM_DWORD  f_cbCount  )
{
    /*
    ** Casting the pointer to volatile makes the MS compiler act
    ** as if another thread can see and access the buffer. This 
    ** prevents the compiler from reordering or optimizing away 
    ** the writes. 
    */

    volatile char *vptr = (volatile char *)f_ptr;

    while (f_cbCount) 
    {
        *vptr = 0;
        vptr++;
        f_cbCount--;
    }

    return f_ptr;
}

#define OEM_SECURE_ZERO_MEMORY DRMCRT_ScrubMemory

#else

#include <stdio.h>
#include <assert.h>

DRMINLINE DRM_VOID * DRM_CALL DRMCRT_ScrubMemory(
    __in_ecount( f_cbCount ) DRM_VOID  *f_ptr,
    IN DRM_DWORD  f_cbCount  )
{
    /*
    ** Casting the pointer to volatile makes the MS compiler act
    ** as if another thread can see and access the buffer. This 
    ** prevents the compiler from reordering or optimizing away 
    ** the writes. 
    */
    //printf("Not implemented for DRMCRT_ScrubMemory!!!!!!!!!!!!!!!\n");
    volatile char *vptr = (volatile char *)f_ptr;

    while (f_cbCount) 
    {
        *vptr = 0;
        vptr++;
        f_cbCount--;
    }

    //assert(0);

    return f_ptr;
}

#define OEM_SECURE_ZERO_MEMORY DRMCRT_ScrubMemory

#endif /* _MSC_VER */


#define SAFE_SECURE_OEM_FREE( p, c )                                    \
    if (p != NULL)                                                      \
    {                                                                   \
        OEM_SECURE_ZERO_MEMORY( p, c );                                 \
        Oem_MemFree( ( DRM_VOID * ) p );                                \
        p = NULL;                                                       \
    }

DRM_API DRM_VOID DRM_CALL Oem_Random_SetSeed(
    __in_opt DRM_VOID *f_pOEMContext,
    __in const DRM_UINT64 f_qwNewSeed );

DRM_API DRM_RESULT DRM_CALL Oem_Random_GetBytes( 
    __in_opt DRM_VOID *f_pOEMContext,
    __out_bcount(f_cbData) DRM_BYTE *f_pbData, 
    __in DRM_DWORD f_cbData );

DRM_API DRM_RESULT DRM_CALL Oem_Hds_GetSetReplayCheck(
    __in DRM_VOID *f_pvOpaqueData,
    __inout_bcount( f_cbKey ) DRM_BYTE *f_pbKey,
    __in DRM_DWORD f_cbKey,
    __in DRM_BOOL f_fGet );

#if DRM_SUPPORT_BLOCK_HEADER_CACHING

EXIT_PK_NAMESPACE;

#include <drmhds.h>

ENTER_PK_NAMESPACE;

/* Number of entries in the block header cache. Each entry will occupy
** SIZEOF( BLOCK_HEADER_CACHE_ENTRY ) bytes of memory. A larger cache
** will mean a lower chance of needing to load a block header from the disk
*/
#define MAX_CACHED_BLOCK_HEADERS 500

typedef struct
{
    DRM_DWORD dwBlockNumber;
    DRM_DWORD dwCacheHits;
    DRM_DWORD dwBlockHeaderMetadata;
    DRM_BYTE  rgbBlockHeader[CALC_MAXBLOCKHEADERLEN];
    DRM_DWORD cbBytesAvailable;

} BLOCK_HEADER_CACHE_ENTRY;

DRM_API DRM_VOID DRM_CALL Oem_Hds_ClearBlockHeaderCache( DRM_VOID );

DRM_API DRM_RESULT DRM_CALL Oem_Hds_CheckBlockHeaderCache(
    __in      DRM_DWORD   f_dwBlockNumber,
    __in_opt  OEM_FILEHDL f_hHDSFileHandle,
    __out_opt DRM_DWORD  *f_pdwBlockHeaderMetadata,
    __out_bcount_opt( f_cbBytesToRead ) DRM_BYTE   *f_pbBlock,
    __in      DRM_DWORD   f_cbBytesToRead );

DRM_API DRM_RESULT DRM_CALL Oem_Hds_UpdateBlockHeaderCache(
    __in     DRM_DWORD   f_dwBlockNumber,
    __in_opt OEM_FILEHDL f_hHDSFileHandle,
    __in     DRM_DWORD   f_dwBlockHeaderMetadata,
    __in_bcount_opt( f_cbBytesToWrite ) DRM_BYTE   *f_pbBlock,
    __in     DRM_DWORD   f_cbBytesToWrite );

#endif /* DRM_SUPPORT_BLOCK_HEADER_CACHING */

#if DRM_SUPPORT_SST_REDUNANCY

EXIT_PK_NAMESPACE;

#include <drmtoken.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL VariableSSTRedundancyGetorSet(
   __in const DRM_CONST_STRING *f_pdstrToken, 
   __in TOKEN *f_pNewValue, 
   __out TOKEN *f_pResult, 
   __inout DRM_VOID *f_pvOpaqueData, 
   __in DRM_BOOL f_fSet );
   
#endif /* DRM_SUPPORT_SST_REDUNANCY */

#if TEST_USE_OFFSET_CLOCK

typedef struct
{
    DRM_BOOL    fFreezeTickCount;
    DRM_DWORD   dwTickCount;
} DRM_TEST_OEM_CONTEXT;

#endif

EXIT_PK_NAMESPACE;

#endif /* __OEMCOMMON_H__ */
