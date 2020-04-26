/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef NDTASFSAMPLE_H
#define NDTASFSAMPLE_H

#include <asfmediabuffer.h>
#include <drmptrlist.h>

ENTER_PK_NAMESPACE;

#define ASF_SAMPLE_READY             ((DRM_DWORD) 0x00000020)
#define MF_SAMPLE_FLAG_CLEANPOINT    ((DRM_DWORD) 0x00000004)
#define MF_SAMPLE_FLAG_DISCONTINUITY ((DRM_DWORD) 0x00000002)

typedef struct __tagASF_SAMPLE
{
    DRM_DWORD               dwFlags;
    DRM_DWORD               cRef;
                           
    DRM_DWORD               dwStreamNumber;
                           
    DRM_UINT64              hnsSampleTime;
    DRM_UINT64              hnsSampleDuration;
    DRM_DWORD               cbTotalLength;
    PTR_LIST                MediaBuffers;

    DRM_BYTE                bRepData;
    DRM_BYTE                rgbRepData[256]; /* NOTE: dynamic allocation could save space here */

} ASF_SAMPLE;


DRM_RESULT ASFSample_Initialize(
    __out ASF_SAMPLE * const f_pAsfSample
);

DRM_RESULT ASFSample_GetBufferCount(
    __in const  ASF_SAMPLE * const f_pAsfSample,
    __out       DRM_DWORD  * const f_pdwBufferCount
);

DRM_RESULT ASFSample_GetBufferByIndex(
    __in const  ASF_SAMPLE  * const f_pAsfSample,
    __in const  DRM_DWORD           f_dwIndex,
    __out       ASF_MEDIA_BUFFER  **f_ppBuffer
);

DRM_RESULT ASFSample_AddBuffer(
    __inout  ASF_SAMPLE        * const f_pAsfSample,
    __in     ASF_MEDIA_BUFFER  * const f_pBuffer
);

DRM_VOID ASFSample_RemoveBufferByIndex(
    __in        ASF_SAMPLE * const f_pAsfSample,
    __in const  DRM_DWORD          f_dwIndex
);

DRM_RESULT ASFSample_GetTotalLength(
    __in const ASF_SAMPLE * const f_pAsfSample,
    __out      DRM_DWORD  * const f_pcbTotalLength
);

DRM_RESULT ASFSample_CopyToBuffer(
    __in const                 ASF_SAMPLE * const f_pAsfSample,
    __out_bcount(*f_pcbBuffer) DRM_BYTE   * const f_pbBuffer,
    __inout                    DRM_DWORD  * const f_pcbBuffer 
);

DRM_RESULT ASFSample_AddRef(
    __in     ASF_SAMPLE * const f_pAsfSample
);

DRM_VOID ASFSample_Release(
    __in ASF_SAMPLE *pAsfSample
);

EXIT_PK_NAMESPACE;

#endif /* NDTASFSAMPLE_H */
