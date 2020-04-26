/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef NDTASFSENDWINDOW_H
#define NDTASFSENDWINDOW_H

#include <drmptrlist.h>
#include <drmorderedlist.h>
#include <packetwriter.h>

ENTER_PK_NAMESPACE;

#define ASF_MUX_QUEUE_TYPE_UNKNOWN        0
#define ASF_MUX_QUEUE_TYPE_FRAGMENTABLE   1
#define ASF_MUX_QUEUE_TYPE_UNFRAGMENTABLE 2

typedef struct tagASF_MUX_QUEUE_OBJECT_CONTEXT
{
    DRM_DWORD           m_dwType;
    DRM_DWORD           m_cRef;
    DRM_UINT64          m_msPresTime;
    DRM_UINT64          m_hnsSendTime;
    DRM_WORD            m_wStreamNumber;
    DRM_BYTE            m_bObjectID;
    DRM_DWORD           m_dwFlags;
    DRM_BOOL            m_fFragmentable;

    DRM_DWORD           m_cbSize;
    DRM_DWORD           m_cbTotalObjectSize;

    ASF_SAMPLE         *m_pMFSample;
    DRM_DWORD           m_cbAlreadyPacketized;
    DRM_DWORD           m_cbTargetSize;
    DRM_DWORD           m_cbOffsetIntoFirstSample;
    ASF_SAMPLE         *m_pInternalSample;

    PTR_LIST            m_SamplesToBeFinished;

} ASF_MUX_QUEUE_OBJECT_CONTEXT;


DRM_RESULT ASFMuxQueueObject_Initialize(
    __out      ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext,
    __in const DRM_DWORD                            dwType,
    __in       ASF_SAMPLE                   * const pMFSample,
    __in const DRM_DWORD                            cbOffsetIntoFirstSample,
    __in const DRM_DWORD                            cbTargetSize,
    __in const DRM_UINT64                           msPresTime,
    __in const DRM_UINT64                           hnsSendTime,
    __in const DRM_WORD                             wStreamNumber,
    __in const DRM_BYTE                             bObjectID,
    __in const DRM_DWORD                            dwFlags
);

DRM_VOID ASFMuxQueueObject_Uninitialize(
    __inout ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext
);

DRM_RESULT ASFMuxQueueObject_GeneratePayload( 
    __inout ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext,
    __inout ASF_PACKET_WRITER_CONTEXT    * const pASFPacketWriter
);

DRM_RESULT ASFMuxQueueObject_FillInASFPayloadInfo(
    __in const ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext,
    __out      ASF_PAYLOAD_PARSER_INFO      * const pASFPayloadInfo 
);

DRM_RESULT ASFMuxQueueObject_PopUsedSample( 
    __inout ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext,
    __out   ASF_SAMPLE                         **ppUsedSample
);

DRM_RESULT ASFMuxQueueObject_AddSegment(
    __inout    ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext,
    __in       ASF_SAMPLE                   * const pMFSample,
    __in const DRM_DWORD                            cbOffsetIntoSample,
    __out      DRM_DWORD                    * const pcbBytesUsed 
);

DRM_RESULT ASFMuxQueueObject_AddRef(
    __out ASF_MUX_QUEUE_OBJECT_CONTEXT * const pAsfMuxQueueObjectContext
);

DRM_VOID ASFMuxQueueObject_Release(
    __out ASF_MUX_QUEUE_OBJECT_CONTEXT *pAsfMuxQueueObjectContext
);

typedef struct tagASF_MUX_SEND_WINDOW_CONTEXT
{
    DRM_DWORD                                        m_msWindowSize;
    DRM_DWORD                                        m_cbFixedPacketSize;
    DRM_DWORD                                        m_msFirstPresTime;

    struct __tagASF_MULTIPLEXER_CONTEXT             *m_pMux;  /* Backpointer */

    ORDERED_LIST                                     m_Samples;
    DRM_UINT64                                       m_hnsLatestSendTimeQueued;

    /* Properties of in-progress write packet */
    ASF_PACKET_WRITER_CONTEXT                        m_ASFPacketWriterContext;
    DRM_UINT64                                       m_hnsWritePacketSendTime;

    /* Running stats */
    DRM_UINT64                                       m_cbSent;
    DRM_DWORD                                        m_cPacketsSent;
    DRM_UINT64                                       m_msLastSendTimeSent;
    DRM_UINT64                                       m_msLastPresTimeSent;
    DRM_UINT64                                       m_msLastSendDurationSent;
} ASF_MUX_SEND_WINDOW_CONTEXT;


DRM_RESULT ASFMuxSendWindow_Initialize(
    __out      ASF_MUX_SEND_WINDOW_CONTEXT         * const pAsfMuxSendWindowContext,
    __in const DRM_DWORD                                   msWindowSize,
    __in const DRM_DWORD                                   cbFixedPacketSize,
    __in const DRM_DWORD                                   msFirstPresTime,
    __in       struct __tagASF_MULTIPLEXER_CONTEXT * const pMux 
);

DRM_VOID ASFMuxSendWindow_Uninitialize(
    __inout ASF_MUX_SEND_WINDOW_CONTEXT * const pAsfMuxSendWindowContext
);

DRM_RESULT ASFMuxSendWindow_AddObjectsAndSendPackets(
    __inout               ASF_MUX_SEND_WINDOW_CONTEXT  * const pAsfMuxSendWindowContext,
    __in_ecount(cObjects) ASF_MUX_QUEUE_OBJECT_CONTEXT       **ppMuxQueueObjects,
    __in const            DRM_DWORD                            cObjects 
);

DRM_RESULT ASFMuxSendWindow_Flush(
    __inout ASF_MUX_SEND_WINDOW_CONTEXT * const pAsfMuxSendWindowContext
);

EXIT_PK_NAMESPACE;

#endif /* NDTASFSENDWINDOW_H */
