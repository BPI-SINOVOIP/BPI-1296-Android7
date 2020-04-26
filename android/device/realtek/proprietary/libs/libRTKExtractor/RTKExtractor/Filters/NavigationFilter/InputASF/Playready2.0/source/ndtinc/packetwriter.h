/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __PACKETWRITER_H
#define __PACKETWRITER_H

#include <asfsample.h>

ENTER_PK_NAMESPACE;

typedef struct tagPACKET_SNAPSHOT
{
    ASF_PACKET_PARSER_INFO   PacketParseInfo;
    DRM_DWORD                cbFinalPayloadTotalSize;
    DRM_WORD                 cFinalCompressedSubPayloads;
    DRM_DWORD                cbFinalCompressedPayloadTotal;
    DRM_BYTE                 bFinalCompressedPresTimeDelta;    
} PACKET_SNAPSHOT;


typedef struct tagASF_PACKET_WRITER_CONTEXT
{
    ASF_PACKET_PARSER_INFO           m_ASFPacketInfo;
    PTR_LIST                         m_ASFPayloadInfos;
    PTR_LIST                         m_PayloadDataBuffers;
    DRM_DWORD                        m_cbFixedPacketSize;
} ASF_PACKET_WRITER_CONTEXT;


DRM_RESULT ASFPacketWriter_Initialize(
    __out      ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext,
    __in const DRM_DWORD                         dwFixedPacketSize
);

DRM_VOID ASFPacketWriter_Uninitialize(
    __out ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext
);

DRM_RESULT ASFPacketWriter_SetPacketInfo(
    __inout    ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext,
    __in const ASF_PACKET_PARSER_INFO * const pPacketInfo
);

DRM_RESULT ASFPacketWriter_GetPacketInfo(
    __in const ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext,
    __out      ASF_PACKET_PARSER_INFO    * const pPacketInfo 
);

DRM_RESULT ASFPacketWriter_GetDataSpaceAvailable(
    __inout    ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext,
    __in const DRM_WORD                          wStreamNumber,
    __in const DRM_BYTE                          bObjectID,
    __in const ASF_SAMPLE                * const pMediaObjectWithAttributes,
    __out      DRM_DWORD                 * const pcbSpaceAvailable 
);

DRM_RESULT ASFPacketWriter_AddPayload(
    __inout    ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext,
    __in const ASF_PAYLOAD_PARSER_INFO   * const pPayloadInfo,
    __in const ASF_SAMPLE                * const pMFSample,
    __in const DRM_BOOL                          fSampleIsCompleteMediaObject 
);

DRM_RESULT ASFPacketWriter_WritePacket(
    __in const ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext,
    __out      ASF_SAMPLE                      **ppASFPacket
);

DRM_RESULT ASFPacketWriter_Clear(
    __out ASF_PACKET_WRITER_CONTEXT * const pAsfPacketWriterContext
);

EXIT_PK_NAMESPACE;

#endif /* __PACKETWRITER_H */
