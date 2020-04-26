/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __LEAKYBUCKET_H
#define __LEAKYBUCKET_H

#define LEAKYBUCKETFLAGS_AUTOADJUST_BITRATE ((DRM_DWORD) 0x1)

ENTER_PK_NAMESPACE;

typedef struct tagASF_LEAKY_BUCKET_CONTEXT
{
    DRM_UINT64      m_hnsBucketSize;
    DRM_DWORD       m_dwBitrate;

    DRM_DWORD       m_dwFlags;

    DRM_UINT64      m_hnsNextLeakTime;

    DRM_UINT64      m_hnsLastLeakTime;
    DRM_DWORD       m_cbLastLeakSize;

    DRM_DWORD       m_dwTotalBitrateAdjust;
    DRM_UINT64      m_hnsLastAutoAdjustInterval;
    DRM_DWORD       m_cbLastAutoAdjustSize;

} ASF_LEAKY_BUCKET_CONTEXT;

DRM_RESULT ASFLeakyBucket_Initialize(
    __out      ASF_LEAKY_BUCKET_CONTEXT * const pAsfLeakyBucketContext,
    __in const DRM_DWORD                        dwBitrate,
    __in const DRM_UINT64                       hnsBucketSize
);

DRM_RESULT ASFLeakyBucket_AddToBucket(
    __inout    ASF_LEAKY_BUCKET_CONTEXT * const pAsfLeakyBucketContext,
    __in const DRM_DWORD                        cbSize,
    __in const DRM_UINT64                       hnsTimestamp,
    __in const DRM_UINT64                       hnsLeakTimeFloor,
    __out      DRM_UINT64               * const phnsLeakTime 
);

EXIT_PK_NAMESPACE;

#endif /* __LEAKYBUCKET_H */
