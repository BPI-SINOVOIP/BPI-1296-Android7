/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTCONTEXTSIZES_H__
#define __NDTCONTEXTSIZES_H__

#include <ndttypes.h>
#include <drmcrt.h>
#include <oemaes.h>
#include <drmoutputleveltypes.h>
#include <drmcommon.h>
#include <oemndt.h>
#include <oemcontextsizes.h>
#include <ndtasftypes.h>
#include <drmptrlist.h>
#include <oemrsaoaeppss.h>

ENTER_PK_NAMESPACE;

struct _tagNDTMGR_INTERNAL_SESSION_CONTEXT;

typedef struct _tagNDTMGR_INTERNAL_SESSION_CONTEXT
{
    DRM_ID                                      idSession;
    NDTMGR_SESSION_TYPE                         sessionType;
    struct _tagNDTMGR_INTERNAL_SESSION_CONTEXT *pNextContext;
    struct _tagNDTMGR_INTERNAL_SESSION_CONTEXT *pPrevContext;
} NDTMGR_INTERNAL_SESSION_CONTEXT;

typedef enum __tagNDT_REGISTRATION_STATE
{
    REG_STATE_INIT = 0,
    REG_STATE_PROCESSED_REGISTRATION_REQUEST,
    REG_STATE_PROXIMITY_CHALLENGE_SENT,
    REG_STATE_PROXIMITY_VERIFIED,
    REG_STATE_DONE
} NDT_REGISTRATION_STATE;

typedef enum __tagNDT_MEDIA_SESSION_STATE
{
    MEDIA_STATE_INITED = 0,
    MEDIA_STATE_SENDING_DATA,
    MEDIA_STATE_LICENSE_READY_FOR_LEAF,
    MEDIA_STATE_LICENSE_READY,
    MEDIA_STATE_CLOSE,
} NDT_MEDIA_SESSION_STATE;

typedef struct __tagNDT_CERTIFICATE_INFO
{
    DRM_BYTE            rgbReceiverIdentityHash[__CB_DECL(DRM_SHA1_DIGEST_LEN)];
    DRM_BYTE            rgbDigestValue[__CB_DECL(DRM_SHA1_DIGEST_LEN)];
    DRM_DWORD           dwSecurityLevel;
    DRM_DWORD           dwWMDRMNDFeatures;
    DRM_DWORD           dwPubKeyModulusSize;
    DRM_RSA_PUBLIC_KEY* pDevicePublicKey;
} NDT_CERTIFICATE_INFO;

typedef struct __tagNDT_ASF_CONTEXT
{
    /* Flag to determine if we are doing protedcted content play back */
    DRM_BOOL                     fProtectedContentPlayback;

    /* DRM status of the file, 1 if it is protected */
    DRM_DWORD                    dwDRMStatus;

    /* Encryption Type */
    DRM_SUPPORTED_CIPHERS        eCipherType;

    /* How many additional bytes will be added to the Payload Extension
    ** (a.k.a Replicated Data) section of each payload, which is dependent
    ** upon how the ASF file is encrypted.
    */
    DRM_DWORD                    dwAesPayloadExtensionAdditionalBytes;
    DRM_DWORD                    rgdwSampleIDRepDataOffset[MAXSTREAMS];

    /* License status of the file, 1 if we have license to play it */
    DRM_DWORD                    dwLicenseStatus;

    /* Are we performing clear content encryption */
    DRM_BOOL                     fEncryptClear;

    /* Delivery mode, sample or muxed */
    NDT_ASFDELIVERY_MODE         ASFDeliveryMode;

    /* TRUE if the license has been committed with DRM_MGR_Commit.
       This happens once the first data block is read, and does things such
       as decrementing playcounts and deleting licenses */
    DRM_BOOL                     fCommitted;

    /* Input file handle */
    OEM_FILEHDL                  hInFile;

    /* File buffer */
    DRM_BYTE                    *pbFileBuffer;
    ASF_PARSER_STATE             ParserState;

    DRM_UINT64                   qwSendDuration;
    DRM_UINT64                   qwDataPacketsCount;
    DRM_UINT64                   qwDuration;
    DRM_UINT64                   qwPreroll;
    DRM_DWORD                    dwFlags;

    DRM_DECRYPT_CONTEXT          decryptContext;
    DRM_BOOL                     fOPLValid;
    DRM_PLAY_OPL                 playOPLs;

    DRM_DWORD                    dwNoOfStreams;
    DRM_BOOL                     fAudioOnly;
    PTR_LIST                     StreamInfoList;
    PTR_LIST                     Samples;
    DRM_UINT64                   qwNextSampleID;
    DRM_DWORD                    dwOutputPacketSize;
    DRM_DWORD                    dwMaximumObjectSize;

    ASF_TRANSCRYPT_CONTEXT       ASFTranscryptContext;

    DRM_BOOL                     fInclusionListValid;
    DRM_GUIDLIST                 InclusionList;
    ASF_FILE_INFO                ASFFileInfo;

    DRM_BOOL                     fIsPlayReady;
} NDT_ASF_CONTEXT;

typedef struct __tagNDT_LICENSE_CONTEXT
{
    /* Used in XMR license */
    DRM_KID         idKID;
    DRM_ID          idRightsID;

    DRM_DWORD       dwClientCRLVersion;
    DRM_BYTE        rgbKeyMaterial[ __CB_DECL(2*DRM_AES_KEYSIZE_128) ];
    DRM_BYTE       *pbKeyContentEncryption;
    DRM_BYTE       *pbKeyContentIntegrity;
    DRM_AES_KEY    *pAESKeyContentEncryption;
    DRM_BOOL        fProtectedContentPlayback;
    DRM_WORD        wMinimumSecurityLevel;
} NDT_LICENSE_CONTEXT;

typedef NDT_LICENSE_CONTEXT NDT_SIMPLE_LICENSE_CONTEXT;
typedef struct tagNDT_CHAINED_LICENSE_CONTEXT
{
    NDT_LICENSE_CONTEXT rootLicenseContext;
    NDT_LICENSE_CONTEXT leafLicenseContext;
} NDT_CHAINED_LICENSE_CONTEXT;

typedef struct __tagNDTMGR_INTERNAL_CONTENTSESSION_CONTEXT
{
    NDTMGR_INTERNAL_SESSION_CONTEXT  sessionContext;
    DRM_APP_CONTEXT                 *pAppContext;
    NDT_MEDIA_SESSION_STATE          mediaSessionState;

    NDT_LICENSE_MODE                 licenseMode;
    DRM_VOID                        *pLicenseContext;
    NDT_ASF_CONTEXT                  asfContext;

    OEM_DEVICE_HANDLE                hDevice;
    DRM_BYTE                         rgbReceiverIdentityHash[__CB_DECL(DRM_SHA1_DIGEST_LEN)];

    DRM_DWORD                        dwReceiverSecurityLevel;
    DRM_DWORD                        dwReceiverWMDRMNDFeatures;
} NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT;

typedef struct __tagNDTMGR_INTERNAL_REGISTRATIONSESSION_CONTEXT
{
    NDTMGR_INTERNAL_SESSION_CONTEXT sessionContext;
    NDT_REGISTRATION_STATE          state;

    DRM_BYTEBLOB                    DeviceID;
    OEM_DEVICE_HANDLE               hDevice;

    DRM_ID                          idSerial;
    DRM_ID                          idSeed;

    DRMFILETIME                     ftRegistrationStartTime;
    DRM_DWORD                       dwProximityStartTime;

    DRM_ID                          idNonce;
    DRM_BYTE                        bSequenceNumber;

    DRM_WORD                        wResult;

    DRM_BYTE                       *pbProximityDetectionAddress;
    DRM_WORD                        cbProximityDetectionAddress;
} NDTMGR_INTERNAL_REGISTRATIONSESSION_CONTEXT;


typedef struct __tagNDTMGR_CONTEXT_INTERNAL
{
    /*
     * Content transfer
     */
    DRM_APP_CONTEXT                 *pAppContext;

    OEM_DEVICESTORE_CONTEXT          deviceStoreContext;
    OEM_DEVICESTORE_CONTEXT          upperLayerDeviceStoreContext;
    DRM_DWORD                        dwDeviceStoreRegistrationCount;

    NDTMGR_INTERNAL_SESSION_CONTEXT *pFirstSession;

    DRM_DWORD                        dwOpenedCount;
    NDT_CERTIFICATE_INFO             certificateInfo;
    DRM_DWORD                        dwCRLVersion;
} NDTMGR_CONTEXT_INTERNAL;

#define NDTMGR_CONTEXT_BUFFER_SIZE SIZEOF(NDTMGR_CONTEXT_INTERNAL)

#define SIZEOF_U8 1

typedef enum tagNDT_ENCRYPTION_MODE
{
    NDT_ENCRYPTION_MODE_ASF = 0,
    NDT_ENCRYPTION_MODE_MPEG = 1,
    NDT_ENCRYPTION_MODE_LINK = 2
} NDT_ENCRYPTION_MODE;

typedef struct tagNDT_CRYPTO_DATA_ASF
{
    DRM_BYTEBLOB    SampleData;
    DRM_UINT64      qwSampleID;
    DRM_UINT64      qwOffset;
} NDT_CRYPTO_DATA_ASF;

typedef struct tagNDT_CRYPTO_DATA_MPEG
{
    DRM_BYTEBLOB    Data;
    DRM_UINT64      qwBaseCounter;
    DRM_UINT64      qwMinorCounter;
} NDT_CRYPTO_DATA_MPEG;

typedef struct tagNDT_CRYPTO_DATA_LINK
{
    DRM_BYTEBLOB    Data;
    DRM_UINT64      qwDataSegmentID;
    DRM_UINT64      qwOffset;
    DRM_UINT64      qwBlockID;
    DRM_BOOL        fNoEncrypt;
    DRM_BOOL        fNoBlockID;
    DRM_BYTEBLOB*   pDataSegmentDescriptor;
} NDT_CRYPTO_DATA_LINK;

EXIT_PK_NAMESPACE;

#endif  /* __NDTCONTEXTSIZES_H__ */

