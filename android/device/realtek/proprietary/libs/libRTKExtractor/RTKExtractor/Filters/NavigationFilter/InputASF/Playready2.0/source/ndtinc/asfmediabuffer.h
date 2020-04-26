/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef NDTASFMEDIABUFFER_H
#define NDTASFMEDIABUFFER_H

ENTER_PK_NAMESPACE;

typedef struct __tagASF_MEDIA_BUFFER
{
    DRM_DWORD                     cRef;
    DRM_BYTEBLOB                  blobBuffer;
    DRM_DWORD                     cbCurrentOffset;
    DRM_DWORD                     cbCurrentLength;
    DRM_DWORD                     dwFlags;

    struct __tagASF_MEDIA_BUFFER *pOriginalBuffer;

} ASF_MEDIA_BUFFER;

DRM_RESULT ASFMediaBuffer_Initialize(
    __inout    ASF_MEDIA_BUFFER * const pAsfMediaBuffer,
    __in const DRM_BYTEBLOB             blobBuffer,
    __in const DRM_DWORD                cbOffset
);

DRM_RESULT ASFMediaBuffer_InitializeWrapper(
    __inout    ASF_MEDIA_BUFFER * const pAsfMediaBufferWrapper,
    __in       ASF_MEDIA_BUFFER * const pOriginalAsfMediaBuffer,
    __in const DRM_DWORD                cbOffset,
    __in const DRM_DWORD                cbCurrentLength
);

DRM_VOID ASFMediaBuffer_Uninitialize(
    __inout    ASF_MEDIA_BUFFER * const pAsfMediaBuffer
);

DRM_RESULT ASFMediaBuffer_AddRef(
    __inout    ASF_MEDIA_BUFFER * const pAsfMediaBuffer
);

DRM_VOID ASFMediaBuffer_Release(
    __inout    ASF_MEDIA_BUFFER        *pAsfMediaBuffer
);

EXIT_PK_NAMESPACE;

#endif /* NDTASFMEDIABUFFER_H */
