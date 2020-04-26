/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTASF_INTERNAL_H__
#define __NDTASF_INTERNAL_H__

#include <drmcommon.h>
#include <drmcontextsizes.h>
#include <oemcipher.h>
#include <drmmanager.h>
#include <oemaes.h>
#include <ndtcontextsizes.h>
#include <asfsample.h>

ENTER_PK_NAMESPACE;

/* File i/o buffer size (reading input file) */
#define FILE_BUFFER_SIZE                        65536
#define MIN_OBJ_SIZE                            24
#define DATA_OBJECT_SIZE                        50
#define ASF_TRANSCRYPT_WMDRMND_OBJECT_SIZE      MIN_OBJ_SIZE + 50

/* How many packets to transcrypt at a time */
#define ASF_PACKET_TRANSCRYPTS_PER_CALL         10

/* mux constants */
#define INVALID_SAMPLE_TIME                  DRM_UI64(-1)
#define WAVE_FORMAT_PCM                      1
#define MFASF_MULTIPLEXER_AUTOADJUST_BITRATE 0x1
#define ASF_STATUSFLAGS_INCOMPLETE           ((DRM_DWORD) 0x01)
#define STANDARD_REPLICATED_DATA_SIZE        8
#define PAYLOAD_OVERHEAD                     27
#define REASONABLE_LOOKAHEAD_FOR_PAYLOAD     20
#define MAX_PACKET_PAYLOADS                  63
#define MFASF_MAX_STREAM_NUMBER              127

/* parsing/writing asf data */
#define LoadBYTEOffset( b, p, i )                                                    \
{                                                                                    \
    b = GET_BYTE(p, i);                                                              \
    i += SIZEOF_U8;                                                                  \
}

#define LoadWORDOffset( w, p, i )                                                    \
{                                                                                    \
    DRM_BYT_CopyBytes( &(w), 0, (DRM_BYTE *) (p), (i), SIZEOF( DRM_WORD ) );         \
    FIX_ENDIAN_WORD((w));                                                            \
    (i) += SIZEOF( DRM_WORD );                                                       \
}

#define LoadDWORDOffset( dw, p, i )                                                  \
{                                                                                    \
    DRM_BYT_CopyBytes( &(dw), 0, (DRM_BYTE *) (p), (i), SIZEOF( DRM_DWORD ) );       \
    FIX_ENDIAN_DWORD((dw));                                                          \
    (i) += SIZEOF( DRM_DWORD );                                                      \
}

#define LoadQWORDOffset( qw, p, i )                                                  \
{                                                                                    \
    DRM_BYT_CopyBytes( &(qw), 0, (DRM_BYTE *) (p), (i), SIZEOF( DRM_UINT64 ) );      \
    FIX_ENDIAN_QWORD((qw));                                                          \
    (i) += SIZEOF( DRM_UINT64 );                                                     \
}

#define LoadGUIDOffset( g, p, i )                                                    \
{                                                                                    \
    LoadDWORDOffset( (g).Data1, p, i );                                              \
    LoadWORDOffset( (g).Data2, p, i );                                               \
    LoadWORDOffset( (g).Data3, p, i );                                               \
    PUT_BYTE((g).Data4, 0, GET_BYTE(p, i));                                          \
    PUT_BYTE((g).Data4, 1, GET_BYTE(p, (i + 1)));                                    \
    PUT_BYTE((g).Data4, 2, GET_BYTE(p, (i + 2)));                                    \
    PUT_BYTE((g).Data4, 3, GET_BYTE(p, (i + 3)));                                    \
    PUT_BYTE((g).Data4, 4, GET_BYTE(p, (i + 4)));                                    \
    PUT_BYTE((g).Data4, 5, GET_BYTE(p, (i + 5)));                                    \
    PUT_BYTE((g).Data4, 6, GET_BYTE(p, (i + 6)));                                    \
    PUT_BYTE((g).Data4, 7, GET_BYTE(p, (i + 7)));                                    \
    i += 8;                                                                          \
}

#define SaveBYTEOffset( byteptr, byte, i )                                           \
{                                                                                    \
    PUT_BYTE(byteptr, i, byte);                                                      \
    i += SIZEOF_U8;                                                                  \
}

#define SaveWORDOffset( byteptr, word, i )                                           \
{                                                                                    \
    FIX_ENDIAN_WORD((word));                                                         \
    DRM_BYT_CopyBytes((byteptr),(i),(DRM_BYTE*)&(word),0,SIZEOF(DRM_WORD));          \
    FIX_ENDIAN_WORD((word));                                                         \
    (i) += SIZEOF( DRM_WORD );                                                       \
}

#define SaveDWORDOffset( byteptr, dword, i )                                         \
{                                                                                    \
    FIX_ENDIAN_DWORD((dword));                                                       \
    DRM_BYT_CopyBytes((byteptr),(i),(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));        \
    FIX_ENDIAN_DWORD((dword));                                                       \
    (i) += SIZEOF( DRM_DWORD );                                                      \
}

#define SaveQWORDOffset( byteptr, qword, i )                                         \
{                                                                                    \
    FIX_ENDIAN_QWORD((qword));                                                       \
    DRM_BYT_CopyBytes((byteptr),(i),(DRM_BYTE*)&(qword),0,SIZEOF(DRM_UINT64));       \
    FIX_ENDIAN_QWORD((qword));                                                       \
    (i) += SIZEOF( DRM_UINT64 );                                                     \
}

#define SaveGUIDOffset( p, g, i )                                                    \
{                                                                                    \
    SaveDWORDOffset( p, (g).Data1, i );                                              \
    SaveWORDOffset( p, (g).Data2, i );                                               \
    SaveWORDOffset( p, (g).Data3, i );                                               \
    PUT_BYTE(p, i, GET_BYTE((g).Data4, 0));                                          \
    PUT_BYTE(p, (i) + 1, GET_BYTE((g).Data4, 1));                                    \
    PUT_BYTE(p, (i) + 2, GET_BYTE((g).Data4, 2));                                    \
    PUT_BYTE(p, (i) + 3, GET_BYTE((g).Data4, 3));                                    \
    PUT_BYTE(p, (i) + 4, GET_BYTE((g).Data4, 4));                                    \
    PUT_BYTE(p, (i) + 5, GET_BYTE((g).Data4, 5));                                    \
    PUT_BYTE(p, (i) + 6, GET_BYTE((g).Data4, 6));                                    \
    PUT_BYTE(p, (i) + 7, GET_BYTE((g).Data4, 7));                                    \
    i += 8;                                                                          \
}


#ifdef DEBUG_MUX
#define MUX_TRACE(stuff) TRACE(stuff)
#else
#define MUX_TRACE(stuff)
#endif


DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_InitializeTranscryption( 
    __out                       NDT_ASF_CONTEXT * const pAsfContext,
    __in                        DRM_VOID        * const pNDTManagerContext,
    __in const                  DRM_ID                  idSession,
    __in_bcount(cbLicenseKeyID) DRM_BYTE        * const pbLicenseKeyID,
    __in const                  DRM_DWORD               cbLicenseKeyID
);

DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_Initialize( 
    __inout    NDT_ASF_CONTEXT * const pAsfContext,
    __in       OEM_FILEHDL             hFile
);

DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_GetFileInfo( 
    __inout NDT_ASF_CONTEXT * const pAsfContext,
    __inout ASF_FILE_INFO   * const pFileInfo
);

DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_ClearFileInfo( 
    __out ASF_FILE_INFO * const pFileInfo
);

DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_SetTimePosition(
    __in_opt   DRM_VOID        *f_pOEMContext,
    __inout    NDT_ASF_CONTEXT *pAsfContext,
    __in       DRM_DWORD        dwTimePosition
);

DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_Uninitialize( 
    __inout  NDT_ASF_CONTEXT * const pAsfContext
);

DRM_RESULT DRM_CALL NDTMGR_ASF_Internal_GetHeader(
    __in                     NDT_ASF_CONTEXT  * const pAsfContext,
    __out_bcount(*pcbHeader) DRM_BYTE         * const pbHeader,
    __inout                  DRM_DWORD        * const pcbHeader
);
DRM_RESULT NDTMGR_ASF_Internal_ReadData(
    __in_opt                 DRM_VOID        *f_pOEMContext,
    __inout                  NDT_ASF_CONTEXT *pAsfContext,
    __out_bcount(*f_pcbData) DRM_BYTE        *f_pbData,
    __inout                  DRM_DWORD       *f_pcbData
);
DRM_RESULT NDTMGR_ASF_Internal_ReadSample(
    __inout    NDT_ASF_CONTEXT  * const pAsfContext,
    __out      NDT_SAMPLEINFO   * const pSampleInfo,
    __out      ASF_SAMPLE             **ppSample,
    __in const DRM_DWORD        * const pcbSampleData
);

EXIT_PK_NAMESPACE;

#endif /* __NDTASF_INTERNAL_H__ */
