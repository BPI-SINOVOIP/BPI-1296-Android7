/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _NDTNETMSG_H_
#define _NDTNETMSG_H_

#include <drmcommon.h>
#include <drmrevocation.h>
#include <ndtcontextsizes.h>

ENTER_PK_NAMESPACE;

/*
 * Version of the WMDRMNET protocol
 */
#define WMDRMNET_VERSION_2 2

/*
 * Message Types of the WMDRMNET protocol
 */

/* Registration Message */
#define WMDRMNET_REGISTRATION_MESSAGE_TYPE 1
#define WMDRMNET_REGISTRATION_MESSAGE_SIZE ( \
            SIZEOF_U8 +                         /* Protocol Vesion */ \
            SIZEOF_U8 +                         /* Message Type */ \
            SIZEOF(DRM_ID) +                    /* Serial Number */ \
            SIZEOF(DRM_DWORD) )                 /* Certificate Length */ \


/* Registration Response Message */
#define WMDRMNET_REGISTRATION_RESPONSE_MESSAGE_TYPE 2
#define WMDRMNET_REGISTRATION_RESPONSE_MESSAGE_SIZE ( \
            SIZEOF_U8 +                         /* Protocol Vesion */ \
            SIZEOF_U8 +                         /* Message Type */ \
            SIZEOF(DRM_WORD) +                  /* Signature Offset */ \
            SIZEOF(DRM_ID) +                    /* Serial Number */ \
            SIZEOF(DRM_ID) +                    /* Session ID */ \
            SIZEOF(DRM_WORD) +                  /* Address Length */ \
            SIZEOF_U8 +                         /* Seed Encryption Type */  \
            SIZEOF(DRM_WORD) +                  /* Seed Length */  \
            SIZEOF_U8 +                         /* Signature Type */ \
            SIZEOF(DRM_WORD) )                  /* Signature Length */ \

/* Proximity Start Message */
#define WMDRMNET_PROXIMITY_START_MESSAGE_TYPE 3
#define WMDRMNET_PROXIMITY_START_MESSAGE_SIZE ( \
            SIZEOF_U8 +                  /* Protocol Vesion */ \
            SIZEOF_U8 +                  /* Message Type */ \
            SIZEOF(DRM_ID) )                    /* Session ID */ \


/* Proximity Challenge Message */
#define WMDRMNET_PROXIMITY_CHALLENGE_MESSAGE_TYPE 4
#define WMDRMNET_PROXIMITY_CHALLENGE_MESSAGE_SIZE ( \
            SIZEOF_U8 +                  /* Protocol Vesion */ \
            SIZEOF_U8 +                  /* Message Type */ \
            SIZEOF_U8 +                  /* Sequence Number */ \
            SIZEOF(DRM_ID) +                    /* Session ID */ \
            SIZEOF(DRM_ID) )                    /* Challenge */ \

/* Proximity Response Message */
#define WMDRMNET_PROXIMITY_RESPONSE_MESSAGE_TYPE 5
#define WMDRMNET_PROXIMITY_RESPONSE_MESSAGE_SIZE ( \
            SIZEOF_U8 +                  /* Protocol Vesion */ \
            SIZEOF_U8 +                  /* Message Type */ \
            SIZEOF_U8 +                  /* Sequence Number */ \
            SIZEOF(DRM_ID) )                    /* Session ID */ \

/* Proximity Result Message */
#define WMDRMNET_PROXIMITY_RESULT_MESSAGE_TYPE 6
#define WMDRMNET_PROXIMITY_RESULT_MESSAGE_SIZE ( \
            SIZEOF_U8 +                  /* Protocol Vesion */ \
            SIZEOF_U8 +                  /* Message Type */ \
            SIZEOF(DRM_ID) +                    /* Session ID */ \
            SIZEOF(DRM_WORD) )                  /* Result */

/* License Request Message */
#define WMDRMNET_LICENSE_REQUEST_MESSAGE_TYPE 7
#define WMDRMNET_LICENSE_REQUEST_MESSAGE_SIZE ( \
            SIZEOF_U8 +                         /* Protocol Vesion */ \
            SIZEOF_U8 +                         /* Message Type */ \
            SIZEOF(DRM_ID) +                    /* Rights ID */ \
            SIZEOF(DRM_DWORD) +                 /* Reserved */ \
            SIZEOF(DRM_ID) +                    /* Serial Number */ \
            SIZEOF(DRM_DWORD) +                 /* Certificate Length */ \
            SIZEOF(DRM_WORD) )                  /* Action Length */

/* License Response Message */
#define WMDRMNET_LICENSE_RESPONSE_MESSAGE_TYPE 8
#define WMDRMNET_LICENSE_RESPONSE_MESSAGE_SIZE ( \
            SIZEOF_U8 +                        /* Protocol Vesion */ \
            SIZEOF_U8 +                        /* Message Type */ \
            SIZEOF(DRM_DWORD) +                /* Length of Reserved field */ \
            SIZEOF(DRM_DWORD))                 /* Length of XMR License */ \

#define WMDRMNET_REVOCATION_LIST_REQUEST_MESSAGE_TYPE 10
#define WMDRMNET_REVOCATION_LIST_REQUEST_MESSAGE_SIZE ( \
            SIZEOF_U8 +                        /* Protocol Vesion */ \
            SIZEOF_U8 +                        /* Message Type */ \
            SIZEOF_U8 )                        /* Number of entries */ \

#define WMDRMNET_REVOCATION_LIST_RESPONSE_MESSAGE_TYPE 11
#define WMDRMNET_REVOCATION_LIST_RESPONSE_MESSAGE_SIZE ( \
            SIZEOF_U8 +                        /* Protocol Vesion */ \
            SIZEOF_U8 )                        /* Message Type */

/* Define signature types */
#define WMDRMNET_SIGNATURE_AES_OMAC_TYPE    1 /* AES OMAC1 */
#define WMDRMNET_SIGNATURE_SHA_RSAPSS_TYPE  2 /* SHA1 signed by the 1024-bit RSA private key */

#define WMDRMNET_SIGNATURE_AES_OMAC_LENGTH      DRM_AES_BLOCKLEN /* OMAC Signature Size */

/* Define encryption types */
#define WMDRMNET_ENCRYPT_RSA_OAEP 1   /* 1024-bit RSA OAEP */

/* Actions */
#define WMDRMNET_PLAY_ACTION "PLAY"
#define WMDRMNET_PLAY_ACTION_SIZE 4 /* unused */


/*
 *
 * Define a structure for packing and unpacking WmdrmNet messages
 */

typedef struct {

    /* Allocated Buffer */
    DRM_BYTE *Buffer;
    DRM_DWORD BufferSize;

    /* Current Offset into the buffer */
    DRM_DWORD Offset;

} WMDRMNET_MESSAGE_DESCRIPTOR;

/*
 * Low level procedures for packing and unpacking messages
 */

DRM_API DRM_RESULT DRM_CALL WmdrmNetAllocateMessageDescriptor(
    __in_bcount_opt(MessageSize) DRM_BYTE                    * const Message,
    __in const                   DRM_DWORD                           MessageSize,
    __out                        WMDRMNET_MESSAGE_DESCRIPTOR       **MessageDescriptor
);

DRM_API DRM_DWORD DRM_CALL WmdrmNetFixedMessageSize(
    __in const DRM_BYTE f_bMessageType
);

DRM_API DRM_VOID DRM_CALL WmdrmNetPackOctet(
    __out      WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __in const DRM_BYTE                            Octet
);

DRM_API DRM_BOOL DRM_CALL WmdrmNetUnpackOctet(
    __inout WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __out   DRM_BYTE                    * const Octet
);

DRM_API DRM_BOOL DRM_CALL WmdrmNetUnpackOctets(
    __inout                       WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __deref_out_bcount(ValueSize) DRM_BYTE                          **Value,
    __in const                    DRM_DWORD                           ValueSize
);

DRM_API DRM_VOID DRM_CALL WmdrmNetPackWord(
    __out      WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __in const DRM_WORD                            Value
);

DRM_API DRM_BOOL DRM_CALL WmdrmNetUnpackWord(
    __inout                        WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __out_bcount(SIZEOF(DRM_WORD)) DRM_WORD                    * const Value
);

DRM_API DRM_VOID DRM_CALL WmdrmNetPackDword(
    __out      WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __in const DRM_DWORD                           Value
);

DRM_API DRM_VOID DRM_CALL WmdrmNetPackUint64(
    __out      WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __in const DRM_UINT64                          Value
);

DRM_API DRM_BOOL DRM_CALL WmdrmNetUnpackDword(
    __inout                         WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __out_bcount(SIZEOF(DRM_DWORD)) DRM_DWORD                   * const Value
);

DRM_API DRM_BOOL DRM_CALL WmdrmNetUnpackVal128(
    __inout                      WMDRMNET_MESSAGE_DESCRIPTOR * const MessageDescriptor,
    __out_bcount(SIZEOF(DRM_ID)) DRM_ID                      * const Value
);

DRM_API DRM_VOID DRM_CALL WmdrmNetFreeMessageDescriptor(
    __out      WMDRMNET_MESSAGE_DESCRIPTOR *f_MessageDescriptor,
    __in const DRM_BOOL                     f_fDone
);




/*
 * Procedures for packing and unpacking messages
 */

DRM_API DRM_BYTE DRM_CALL WmdrmNetGetMessageType(
    __in_bcount(MessageSize) const DRM_BYTE  * const Message,
    __in                     const DRM_DWORD         MessageSize
);



/*
** ----------------------------------------------------------------------------
**                            REGISTRATION functions
** ----------------------------------------------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackRegistrationRequestMessage(
    __in_bcount(f_cbMessage)                    DRM_BYTE  * const f_pbMessage,
    __in const                                  DRM_DWORD         f_cbMessage,
    __out                                       DRM_ID    * const f_pSerialNumber,
    __deref_out_bcount(*f_pcbDeviceCertificate) DRM_BYTE        **f_ppbDeviceCertificate,
    __out                                       DRM_DWORD        *f_pcbDeviceCertificate
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackRegistrationResponseMessage(
    __in const                     DRM_ID             * const f_pSerialNumber,
    __in const                     DRM_RSA_PUBLIC_KEY * const f_pDevicePublicKey,
    __in const                     DRM_DWORD                  f_cbPubKeyModulus,
    __in const                     DRM_ID             * const f_pSessionId,
    __in const                     DRM_ID             * const f_pSeed,
    __in_bcount(f_cbAddress) const DRM_BYTE           * const f_pAddress,
    __in const                     DRM_WORD                   f_cbAddress,
    __out_bcount(*f_pcbMessage)    DRM_BYTE           * const f_pbMessage,
    __inout                        DRM_DWORD          * const f_pcbMessage
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackRegistrationMessage(
    __in const                               DRM_ID   * const SerialNumber,
    __in_bcount(DeviceCertificateSize) const DRM_BYTE * const DeviceCertificate,
    __in const                               DRM_DWORD        DeviceCertificateSize,
    __deref_out_bcount_full(*MessageSize)    DRM_BYTE       **Message,
    __out DRM_DWORD                                   * const MessageSize
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackRegistrationResponseMessage(
    __in const                        DRM_RSA_PRIVATE_KEY * const f_pPrivateKey,
    __in_bcount(f_cbMessage)          DRM_BYTE            * const f_pbMessage,
    __in const                        DRM_DWORD                   f_cbMessage,
    __out                             DRM_ID              * const f_pidSeed,
    __out                             DRM_ID              * const f_pidSerialNumber,
    __out                             DRM_ID              * const f_pidSession,
    __deref_out_bcount(*f_pcbAddress) DRM_BYTE                  **f_ppbAddress,
    __out                             DRM_WORD            * const f_pcbAddress,
    __in                              DRM_CRYPTO_CONTEXT  *       f_pCryptoCtx
);



/*
** ----------------------------------------------------------------------------
**                            PROXIMITY functions
** ----------------------------------------------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackProximityStartMessage(
    __in const                       DRM_ID     * const SessionId,
    __deref_out_bcount(*MessageSize) DRM_BYTE         **Message,
    __out                            DRM_DWORD  * const MessageSize
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackProximityStartMessage(
    __in_bcount(MessageSize) DRM_BYTE * const Message,
    __in const               DRM_DWORD        MessageSize,
    __out                    DRM_ID   * const SessionID
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackProximityChallengeMessage(
    __in const                       DRM_BYTE          bSequenceNumber,
    __in const                       DRM_ID    * const idSession,
    __in const                       DRM_ID    * const idChallenge,
    __deref_out_bcount(*MessageSize) DRM_BYTE        **Message,
    __out                            DRM_DWORD * const MessageSize
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackProximityChallengeMessage(
    __in_bcount(MessageSize) DRM_BYTE  * const Message,
    __in const               DRM_DWORD         MessageSize,
    __out                    DRM_BYTE  * const Sequence,
    __out                    DRM_ID    * const SessionId,
    __out                    DRM_ID    * const Challenge
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackProximityResponseMessage(
    __in const                       DRM_AES_KEY * const pAesKey,
    __in const                       DRM_BYTE            Sequence,
    __in const                       DRM_ID      * const SessionId,
    __in const                       DRM_ID      * const Challenge,
    __deref_out_bcount(*MessageSize) DRM_BYTE          **Message,
    __out                            DRM_DWORD   * const MessageSize
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackProximityResponseMessage(
    __in_bcount(MessageSize) DRM_BYTE  * const Message,
    __in const               DRM_DWORD         MessageSize,
    __out                    DRM_BYTE  * const Sequence,
    __out                    DRM_ID    * const SessionID,
    __out                    DRM_ID    * const Nonce
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackProximityResultMessage(
    __in const                       DRM_ID    * const SessionID,
    __in const                       DRM_WORD          Result,
    __deref_out_bcount(*MessageSize) DRM_BYTE        **Message,
    __out                            DRM_DWORD * const MessageSize
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackProximityResultMessage(
    __in_bcount(MessageSize) DRM_BYTE  * const Message,
    __in const               DRM_DWORD         MessageSize,
    __out                    DRM_ID    * const SessionId,
    __out                    DRM_WORD  * const Result
);




/*
** ----------------------------------------------------------------------------
**                            LICENSE functions
** ----------------------------------------------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackLicenseRequestMessage(
    __in_bcount(f_cbMessage)         DRM_BYTE  * const f_pbMessage,
    __in const                       DRM_DWORD         f_cbMessage,
    __out                            DRM_ID    * const f_pRightsId,
    __out                            DRM_ID    * const f_pSerialNumber,
    __deref_out_bcount(*f_pcbDeviceCertificate) DRM_BYTE **f_ppbDeviceCertificate,
    __out                            DRM_DWORD * const f_pcbDeviceCertificate,
    __deref_out_bcount(*f_pcbAction) DRM_BYTE        **f_ppbAction,
    __out                            DRM_WORD  * const f_pcbAction,
    __out                            DRM_DWORD * const f_pdwCRLVersion
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackLicenseResponseMessage(
    __in_bcount(f_cbLicense)   const  DRM_BYTE  * const f_pbLicense,
    __in                       const  DRM_DWORD         f_cbLicense,
    __in_bcount(f_cbCRLBuffer) const  DRM_BYTE  * const f_pbCRLBuffer,
    __in const                        DRM_DWORD         f_cbCRLBuffer,
    __deref_out_bcount(*f_pcbMessage) DRM_BYTE        **f_ppbMessage,
    __out                             DRM_DWORD * const f_pcbMessage
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackLicenseRequestMessage(
    __in const                               DRM_ID    * const RightsId,
    __in const                               DRM_ID    * const SerialNumber,
    __in_bcount(DeviceCertificateSize) const DRM_BYTE  * const DeviceCertificate,
    __in const                               DRM_DWORD         DeviceCertificateSize,
    __in_bcount(ActionSize) const            DRM_BYTE  * const Action,
    __in const                               DRM_WORD          ActionSize,
    __deref_out_bcount(*MessageSize)         DRM_BYTE        **Message,
    __out                                    DRM_DWORD * const MessageSize
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackLicenseResponseMessage(
    __in_bcount(MessageSize)       DRM_BYTE  * const Message,
    __in const                     DRM_DWORD         MessageSize,
    __deref_out_bcount(*cbLicense) DRM_BYTE        **ppbLicense,
    __out                          DRM_DWORD * const cbLicense
);




/*
** ----------------------------------------------------------------------------
**                            REVOCATION functions
** ----------------------------------------------------------------------------
*/

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackRevocationListRequestMessage(
    __in_bcount(f_cbMessage)                      DRM_BYTE  * const f_pbMessage,
    __in const                                    DRM_DWORD         f_cbMessage,
    __out                                         DRM_DWORD * const f_pcguidsRevocationLists,
    __deref_out_ecount(*f_pcguidsRevocationLists) DRM_GUID        **f_ppguidsRevocationLists
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackRevocationListResponseMessage(
    __in const                            DRM_DWORD            f_cRevocationLists,
    __in_ecount(f_cRevocationLists) const DRM_RVK_LIST * const f_pRevocationLists,
    __inout_bcount(*f_pcbMessage)         DRM_BYTE     * const f_pbMessage,
    __inout                               DRM_DWORD    * const f_pcbMessage
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetPackRevocationListRequestMessage(
    __in const                                 DRM_DWORD         f_cguidsRevocationLists,
    __in_ecount(f_cguidsRevocationLists) const DRM_GUID  * const f_pguidsRevocationLists,
    __inout_bcount_opt(*f_pcbMessage)          DRM_BYTE  * const f_pbMessage,
    __inout                                    DRM_DWORD * const f_pcbMessage
);

DRM_API DRM_RESULT DRM_CALL WmdrmNetUnpackRevocationListResponseMessage(
    __in_bcount(f_cbMessage)                 DRM_BYTE     * const f_pbMessage,
    __in const                               DRM_DWORD            f_cbMessage,
    __inout                                  DRM_DWORD    * const f_pcRevocationLists,
    __inout_ecount_opt(*f_pcRevocationLists) DRM_RVK_LIST * const f_pRevocationLists
);

EXIT_PK_NAMESPACE;

#endif /* _NDTNETMSG_H_ */
