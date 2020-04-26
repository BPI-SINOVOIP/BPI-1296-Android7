/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTASFTYPES_H__
#define __NDTASFTYPES_H__

#include <drmcommon.h>
#include <drmmanager.h>
#include <asfsample.h>

#define MAXSTREAMS 128

ENTER_PK_NAMESPACE;

typedef struct __tagASF_PACKET_PARSER_INFO
{
    DRM_BOOL      fParityPacket;
    DRM_BOOL      fEccPresent;
    DRM_BOOL      fMultiPayloads;
    DRM_DWORD     dwParseOffset;
    DRM_BYTE      bECLen;
    DRM_BYTE      bPacketLenType;
    DRM_BYTE      bPadLenType;
    DRM_BYTE      bSequenceLenType;
    DRM_DWORD     dwPacketLenTypeOffset;
    DRM_BYTE      bOffsetLenType;
    DRM_BYTE      bPayLenType;
    DRM_BYTE      bPayBytes;
    DRM_DWORD     dwPacketLenOffset;
    DRM_DWORD     dwExplicitPacketLength;
    DRM_DWORD     dwSequenceOffset;
    DRM_DWORD     dwSequenceNum;
    DRM_DWORD     dwPadLenOffset;
    DRM_DWORD     dwPadding;
    DRM_DWORD     dwSCR;
    DRM_DWORD     dwPayLenTypeOffset;
    DRM_DWORD     dwPayloads;
    DRM_WORD      wDuration;
    DRM_BYTE      bStreamIDLenType;
    DRM_BYTE      bObjectIDLenType;
    DRM_BYTE      bReplicatedDataLenType;
    DRM_BYTE      bReplicatedDataLenBytes;
    DRM_DWORD     cbPacketLength;
} ASF_PACKET_PARSER_INFO;

typedef struct __tagASF_PAYLOAD_PARSER_INFO
{
    DRM_WORD      wPacketOffset;
    DRM_WORD      wTotalSize;

    DRM_BYTE      bStreamId;
    DRM_BYTE      bObjectId;
    DRM_BYTE      bRepData;
    DRM_BYTE      bJunk;

    DRM_DWORD     dwObjectOffset;
    DRM_DWORD     dwObjectSize;
    DRM_DWORD     dwObjectPres;

    DRM_BOOL      fIsKeyFrame;
    DRM_BOOL      fIsCompressedPayload;
    DRM_WORD      wPayloadSize;
    DRM_WORD      wTotalDataBytes;
    DRM_DWORD     dwDeltaPresTime;

    DRM_WORD      wBytesRead;
    DRM_BYTE      bSubPayloadState;
    DRM_BYTE      bNextSubPayloadSize;
    DRM_WORD      wSubpayloadLeft;
    DRM_WORD      wSubCount;

    DRM_DWORD     dwPayloadDataStartBuffer;
    DRM_DWORD     cPayloadDataBuffers;

    DRM_BYTE      rgbRepData[256]; /* NOTE: dynamic allocation could save space here */

} ASF_PAYLOAD_PARSER_INFO;

typedef enum __tagASF_PARSE_STATE
{
    P_NOT_VALID = 0,
    P_NEW_PACKET,
    P_PACKET_HEADER,
    P_PAYLOAD_HEADER,
    P_PAYLOAD,
    P_UNCOMPRESSEDPAYLOAD,
    P_COMPRESSEDPAYLOAD,
    P_PAYLOAD_END,
    P_END
} ASF_PARSE_STATE;

typedef struct __tagASF_CONTENT_ENCRYPTION_OBJECT
{
    DRM_BYTE      *pbSecretData;
    DRM_DWORD      cbSecretData;

    DRM_BYTE      *pbProtectionType;
    DRM_DWORD      cbProtectionType;

    DRM_BYTE      *pbKeyID;
    DRM_DWORD      cbKeyID;

    DRM_BYTE      *pbLicenseURL;
    DRM_DWORD      cbLicenseURL;
} ASF_CONTENT_ENCRYPTION_OBJECT;

typedef struct __tagASF_CONTENT_ENCRYPTION_OBJECT_EXT
{
    DRM_BYTE      *pbData;
    DRM_DWORD      cbData;
} ASF_CONTENT_ENCRYPTION_OBJECT_EXT;

typedef struct
{
    DRM_BYTE      *pbData;
    DRM_DWORD      cbData;
} ASF_PLAYREADY_OBJECT;

typedef struct __tagASF_PAYLOAD_INFO
{
    DRM_WORD  wStreamNumber;
    DRM_BOOL  fIsKeyFrame;
    DRM_BYTE  bObjectID;
    DRM_DWORD msObjectPresTime;
    DRM_DWORD cbObjectOffset;
    DRM_DWORD cbObjectSize;
    DRM_DWORD cbPayloadDataSize;
} ASF_PAYLOAD_INFO;

typedef struct __tagASF_WAVEFORMATEX
{
    DRM_WORD  wFormatTag;
    DRM_WORD  nChannels;
    DRM_DWORD nSamplesPerSec;
    DRM_DWORD nAvgBytesPerSec;
    DRM_WORD  nBlockAlign;
    DRM_WORD  wBitsPerSample;
    DRM_WORD  cbSize;
} ASF_WAVEFORMATEX;

typedef struct __tagASF_BITMAPINFOHEADER
{
    DRM_DWORD  biSize;
    DRM_LONG   biWidth;
    DRM_LONG   biHeight;
    DRM_WORD   biPlanes;
    DRM_WORD   biBitCount;
    DRM_DWORD  biCompression;
    DRM_DWORD  biSizeImage;
    DRM_LONG   biXPelsPerMeter;
    DRM_LONG   biYPelsPerMeter;
    DRM_DWORD  biClrUsed;
    DRM_DWORD  biClrImportant;
} ASF_BITMAPINFOHEADER;

typedef struct __tagASF_STREAM_VIDEOINFOHEADER
{
    DRM_DWORD dwEncodedImageWidth;
    DRM_DWORD dwEncodedImageHeight;
    DRM_BYTE  bReserved;
    DRM_WORD  cbFormatDataSize;
    ASF_BITMAPINFOHEADER BMIH;
} ASF_STREAM_VIDEOINFOHEADER;

typedef struct __tagASF_STREAM_INFO
{
    DRM_WORD   wStreamNumber;
    DRM_GUID   guidStreamType;
    DRM_DWORD  dwAverageBitrate;

    ASF_WAVEFORMATEX   WFX;
    ASF_STREAM_VIDEOINFOHEADER VIH;
    DRM_UINT64     hnsAvgTimePerFrame;
    DRM_DWORD      dwFlags;

    DRM_DWORD   dwMaxBitrate;
    DRM_DWORD   msMaxBufferWindow;

    DRM_DWORD  dwCurrentObjectTotalSize;
    DRM_DWORD  dwCurrentObjectSize;
    DRM_UINT64 qwCurrentSampleOffset;
    DRM_UINT64 qwCurrentSampleID;

    DRM_BYTE *pbStreamPropertiesObj;

} ASF_STREAM_INFO;

typedef struct __tagASF_PARSER_STATE
{
    DRM_INT64                          iCurrentFileOffset;
    DRM_INT64                          iFirstPacketOffset;
    DRM_INT64                          iLastPacketOffset;
    DRM_INT64                          iPreviousPacketOffset;
    DRM_INT64                          iCurrentPacketOffset;
    DRM_INT64                          iNextPacketOffset;

    ASF_PACKET_PARSER_INFO            *pPacketInfo;
    ASF_PAYLOAD_PARSER_INFO           *pPayloadInfo;
    DRM_DWORD                          dwPayload;
    DRM_DWORD                          dwPayloadOffset;
    DRM_WORD                           wPayStart;
    DRM_BOOL                           fHeaderDone;
    DRM_BOOL                           fBigEndian;
    DRM_BOOL                           fSeekPositionSet;
    DRM_UINT64                         qwSeekPosition;
    DRM_DWORD                          dwTimePosition;
    DRM_UINT64                         qwSeekPacket;
    ASF_PARSE_STATE                    tParseState;
    DRM_DWORD                          dwPacketCount;
    DRM_DWORD                          dwPacketSize;
    DRM_DWORD                          dwMaxBitrate;
    DRM_DWORD                          dwTotalPackets;
    DRM_DWORD                          dwPacketsProcessed;
    ASF_CONTENT_ENCRYPTION_OBJECT     *pContentEncObj;
    ASF_CONTENT_ENCRYPTION_OBJECT_EXT *pContentEncObjExt;
    ASF_PLAYREADY_OBJECT              *pPlayReadyObj;
    DRM_UINT64                         qwHeaderSize;
    DRM_DWORD                          dwNoOfHeaders;
    DRM_DWORD                          dwNoOfStreams;
    DRM_BOOL                           fEncrypted;
    DRM_BOOL    fHeaderParsed;

    DRM_DWORD   cbOriginalHeaderSize;
    DRM_DWORD   cbContentEncryptionObject;
    DRM_DWORD   cbContentEncryptionObjectEx;
    DRM_DWORD   cbPlayReadyObject;
    /* TRUE if the PlayReady object is contained in the extended header object, FALSE
    ** if it's contained in the regular header. Only valid when there is a PlayReady
    ** object present (ie cbPlayReadyObject non-zero) */
    DRM_BOOL    fPlayReadyObjectInsideEHO;

    DRM_BOOL    fFoundStreamPropertiesObjectEx[MAXSTREAMS];
    DRM_BOOL    fFoundStream [MAXSTREAMS];
} ASF_PARSER_STATE;

typedef enum __eASF_TRANSCRYPTION_STATE
{
    ASF_TRANSCRYPTION_STATE_UNINITIALIZED = 0,
    ASF_TRANSCRYPTION_STATE_INITIALIZED,
    ASF_TRANSCRYPTION_STATE_HEADER_TRANSCRYPTED
} ASF_TRANSCRYPTION_STATE;

typedef struct __tagASF_TRANSCRYPT_HEADER_CONTEXT
{
    DRM_BYTE    *pbHeader;
    DRM_DWORD    cbHeaderSize;
    DRM_DWORD    dwHeaderOffset;

    /* Stores the offset to the file size QWORD in the File Properties Object */
    DRM_DWORD    dwFileSizeOffset;
    /* Stores the buffer offset for undelivered data
     * (data that will be sent on the next transcrypt call
     */
    DRM_DWORD    dwSentDataOffset;

    DRM_DWORD   cbHeaderExtensionObjectData;
    DRM_UINT64         qwFileSize;


    /* Stores the offset for the header extension object data size */
    DRM_DWORD   dwHeaderExtensionDataSize;
    DRM_DWORD   cbHeaderExtensionObjectExtra;
} ASF_TRANSCRYPT_HEADER_CONTEXT;

typedef struct __tagASF_DATA_CONTEXT
{
    DRM_BYTE    *pbDataBuffer;
    DRM_DWORD    cbDataBufferSize;
    DRM_DWORD    dwDataBufferOffset;

    /* Stores the buffer offset for undelivered data
     * (data that will be sent on the next transcrypt call
     */
    DRM_DWORD    dwSentDataOffset;
} ASF_DATA_CONTEXT;

typedef struct __tagASF_TRANSCRYPT_DATA_CONTEXT
{
    ASF_DATA_CONTEXT DataContext;
    DRM_UINT64   qwCurrentSampleID;
} ASF_TRANSCRYPT_DATA_CONTEXT;

typedef struct __tagASF_TRANSCRYPT_CONTEXT
{
    ASF_TRANSCRYPTION_STATE          State;

    DRM_GUID                         guidFileID;

    ASF_TRANSCRYPT_HEADER_CONTEXT    Header;
    ASF_TRANSCRYPT_DATA_CONTEXT      Data;

    DRM_DECRYPT_CONTEXT              decryptContext;


    DRM_VOID                        *pNDTManagerContext;
    DRM_ID                           idSession;

    DRM_BOOL                         fAllPacketsTranscrypted;

    DRM_BYTE                        *pbLicenseKeyID;
    DRM_DWORD                        cbLicenseKeyID;

    DRM_BOOL                         fIsVBRContent;

    DRM_VOID                        *pMux;
    ASF_SAMPLE                      *pOutputPacket;
} ASF_TRANSCRYPT_CONTEXT;

typedef struct __tagASF_FILE_INFO
{
    /* Content Encryption Object information */
    ASF_CONTENT_ENCRYPTION_OBJECT       *pContentEncObj;

    /* Extended Content Encryption Object information */
    ASF_CONTENT_ENCRYPTION_OBJECT_EXT   *pContentEncObjExt;

    /* PlayReady Object */
    ASF_PLAYREADY_OBJECT                *pPlayReadyObj;

    DRM_BOOL                             fAudioOnly;
    DRM_DWORD                            dwStreamCount;
} ASF_FILE_INFO;

EXIT_PK_NAMESPACE;

#endif /* __NDTASFTYPES_H__ */
