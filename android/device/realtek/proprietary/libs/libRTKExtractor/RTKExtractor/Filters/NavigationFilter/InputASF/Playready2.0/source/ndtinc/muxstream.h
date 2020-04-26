/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __NDTASFMUXSTREAM_H
#define __NDTASFMUXSTREAM_H

#include <sendwindow.h>
#include <leakybucket.h>

ENTER_PK_NAMESPACE;


typedef struct tagASF_MUX_STREAM_CONTEXT
{
    DRM_DWORD                            m_Type;
    DRM_UINT64                           m_msPreroll;
    struct __tagASF_MULTIPLEXER_CONTEXT *m_pMux;
    
    DRM_WORD                             m_wStreamNumber;
    DRM_DWORD                            m_dwMaxBitrate;
    DRM_DWORD                            m_msMaxBufferWindow;
    DRM_BOOL                             m_fAllObjectsAreCleanPoints;
                               
    DRM_DWORD                            m_dwNextObjectID;
    DRM_UINT64                           m_hnsFirstSendTime;
    DRM_UINT64                           m_msFirstPresTimeSeen;
    DRM_UINT64                           m_msLatestPresTimeSeen;
    DRM_UINT64                           m_msLatestDurationSeen;
    DRM_UINT64                           m_cFrames;
    DRM_UINT64                           m_cbFrameData;
    DRM_DWORD                            m_cbLargestObjectSeen;

    ASF_LEAKY_BUCKET_CONTEXT            *m_pLeakyBucket;

    /* AUDIO */
    DRM_BOOL                             m_fIsPayloadSizeFixed;
    DRM_DWORD                            m_cbFixedPayloadSize;

    ASF_WAVEFORMATEX                    *m_pWFX;
    DRM_DWORD                            m_cbWFX;
    ASF_MUX_QUEUE_OBJECT_CONTEXT        *m_pUnfragmentableObjectInProgress;

    /* VIDEO */
    DRM_UINT64                           m_hnsAvgTimePerFrame;
    DRM_BOOL                             m_fSetSampleDurations;
    ASF_MUX_QUEUE_OBJECT_CONTEXT        *m_pLastQueueObjectSeen;

} ASF_MUX_STREAM_CONTEXT;


DRM_RESULT ASFMuxStream_Initialize(
    __out      ASF_MUX_STREAM_CONTEXT              * const pAsfMuxStreamContext,
    __in       ASF_STREAM_INFO                     * const pIStreamConfig,
    __in const DRM_UINT64                                  msPreroll,
    __in const DRM_UINT64                                  hnsFirstSendTime,
    __in       struct __tagASF_MULTIPLEXER_CONTEXT * const pMuxContext
);

DRM_VOID ASFMuxStream_Uninitialize(
    __inout ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext
);

DRM_RESULT ASFMuxStream_HowManyObjectsWillBeReturned( 
    __in const ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext,
    __in const ASF_SAMPLE             * const pSample,
    __out      DRM_DWORD              * const pcObjects
);

DRM_RESULT ASFMuxStream_OnSample(
    __inout    ASF_MUX_STREAM_CONTEXT       * const pAsfMuxStreamContext,
    __in       ASF_SAMPLE                   * const pISample,
    __in const DRM_UINT64                           hnsTimestampAdjust,
    __in const DRM_DWORD                            dwFlags,
    __inout_ecount(*pcQueueObjects) ASF_MUX_QUEUE_OBJECT_CONTEXT  **ppMuxQueueObject,
    __inout    DRM_DWORD                    * const pcQueueObjects 
);

DRM_RESULT ASFMuxStream_GetObjectSizeForBandwidthPolicing(
    __in const ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext,
    __in const DRM_DWORD                      cbObjectSize,
    __out      DRM_DWORD              * const pcbNewObjectSize
);

DRM_RESULT ASFMuxStream_ApproxSampleDuration(
    __in const ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext,
    __in const ASF_SAMPLE             * const pISample,
    __out      DRM_UINT64             * const pmsDuration
);

DRM_RESULT ASFMuxStream_GetHappyCutoffSendtime(
    __in const ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext,
    __out      DRM_UINT64             * const phnsNextSendtime
);

DRM_RESULT ASFMuxStream_AdjustSendTimeForOffset(
    __in const ASF_MUX_STREAM_CONTEXT       * const pAsfMuxStreamContext,
    __in const DRM_DWORD                            cbOriginalObjectSize,
    __out      ASF_MUX_QUEUE_OBJECT_CONTEXT * const pQueueObject 
);

DRM_VOID ASFMuxStream_Cleanup(
    __inout ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext
);

DRM_RESULT ASFMuxStream_ReadMediaType(
    __inout ASF_MUX_STREAM_CONTEXT * const pAsfMuxStreamContext,
    __in    ASF_STREAM_INFO        * const pIStreamConfig 
);

EXIT_PK_NAMESPACE;

#endif /* __NDTASFMUXSTREAM_H */

