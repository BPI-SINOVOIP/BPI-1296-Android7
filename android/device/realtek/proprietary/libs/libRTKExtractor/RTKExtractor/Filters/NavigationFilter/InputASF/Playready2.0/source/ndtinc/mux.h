/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef NDTASFMUX_H
#define NDTASFMUX_H

#include <muxstream.h>
#include <sendwindow.h>

ENTER_PK_NAMESPACE;

#define MFASF_MAX_STREAM_NUMBER 127

#define INVALID_SAMPLE_TIME DRM_UI64(-1)
#define MFASF_MULTIPLEXER_AUTOADJUST_BITRATE 0x1

typedef struct tagASF_MUX_STATISTICS
{
    DRM_DWORD cFramesWritten;
    DRM_DWORD cFramesDropped;

} ASF_MUX_STATISTICS;

typedef struct __tagASF_MULTIPLEXER_CONTEXT
{
    DRM_DWORD                      m_dwFlags;

    DRM_DWORD                      m_cbFixedPacketSize;
    DRM_UINT64                     m_msPreroll;
    DRM_UINT64                     m_msSyncTolerance;
    DRM_DWORD                      m_dwMaxBitratePlusOverhead;

    ASF_MUX_STREAM_CONTEXT        *m_rgpMuxStreams[ MFASF_MAX_STREAM_NUMBER + 1 ];
    DRM_WORD                      *m_StreamNumbers;
    DRM_WORD                       m_cStreamNumbers;
    ASF_MUX_STATISTICS             m_rgMuxStats[ MFASF_MAX_STREAM_NUMBER + 1 ];

    ASF_MUX_SEND_WINDOW_CONTEXT   *m_pMuxSendWindow;
    PTR_LIST                       m_WaitingPackets;

    ASF_MUX_QUEUE_OBJECT_CONTEXT **m_ppMuxQueueObjects;
    DRM_DWORD                      m_cMuxQueueObjects; /* for generating a payload from a sample/sample fragment */

    DRM_UINT64                     m_cPackets;
    DRM_UINT64                     m_hnsLastPacketSendTime;
    DRM_UINT64                     m_hnsLastPacketDuration;

} ASF_MULTIPLEXER_CONTEXT;


DRM_RESULT ASFMux_Initialize( 
    __inout    ASF_MULTIPLEXER_CONTEXT * const pAsfMuxContext,
    __in const PTR_LIST                * const pStreamInfoList,
    __in const DRM_DWORD                       dwFixedPacketSize,
    __in const DRM_UINT64                      qwPreroll,
    __in const DRM_DWORD                       dwMaxBitrate
);

DRM_VOID ASFMux_Uninitialize( 
    __in ASF_MULTIPLEXER_CONTEXT * const pAsfMuxContext
);

DRM_RESULT ASFMux_ProcessSample( 
    __inout    ASF_MULTIPLEXER_CONTEXT * const pAsfMuxContext,
    __in const DRM_WORD                        wStreamNumber,
    __in       ASF_SAMPLE              * const pISample,
    __in const DRM_UINT64                      hnsTimestampAdjust
);

DRM_RESULT ASFMux_GetNextPacket(
    __in  ASF_MULTIPLEXER_CONTEXT  * const pAsfMuxContext,
    __out ASF_SAMPLE                     **ppIPacket
);

DRM_RESULT ASFMux_Flush( 
    __in  ASF_MULTIPLEXER_CONTEXT  * const pAsfMuxContext
);

DRM_RESULT ASFMux_OnCompletedPacket(
    __inout    ASF_MULTIPLEXER_CONTEXT * const pAsfMuxContext,
    __inout    ASF_SAMPLE              * const pIPacket,
    __in const DRM_UINT64                      hnsSampleTime,
    __in const DRM_UINT64                      hnsSampleDuration
);

DRM_RESULT ASFMux_GetMinStreamSendtimeCutoff( 
    __in const ASF_MULTIPLEXER_CONTEXT * const pAsfMuxContext,
    __out      DRM_UINT64              * const phnsSendTimeCutoff 
);

DRM_RESULT ASFMux_AdjustSendTimeForOffset(    
    __in const ASF_MULTIPLEXER_CONTEXT      * const pAsfMuxContext,
    __in const DRM_DWORD                            cbOriginalObjectSize,
    __inout    ASF_MUX_QUEUE_OBJECT_CONTEXT * const pQueueObject
);

EXIT_PK_NAMESPACE;

#endif /* NDTASFMUX_H */

