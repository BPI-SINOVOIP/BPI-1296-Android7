/*
// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/DrmMessage.h 1     2/14/05 11:22a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
These functions and structures allow representation and manipulation
of the challenge response messages used by the ADP DRM.
*/

#ifndef DRMMESSAGE_H_INCLUDE
#define DRMMESSAGE_H_INCLUDE

#include "../../common/portab.h" /* uint8_t, uint16_t */
#include "../../libDrmCommon/src/DrmSizes.h"

typedef struct DrmMessagePackedStruct
{
    uint8_t message[MAX_MESSAGE_SIZE_BYTES];
    uint8_t sizeInBits;
} DrmMessagePacked;

typedef struct RegistrationRequestStruct
{
    uint8_t userIdGuard[OWNER_GUARD_BYTES];
    uint8_t modelId[MODEL_ID_BYTES];
} RegistrationRequest;


#ifdef __cplusplus
extern "C" {
#endif

void packRegistrationRequest(RegistrationRequest request, DrmMessagePacked *messagePacked);

void packRegistrationLogic(RegistrationRequest *request, DrmMessagePacked *messagePacked, uint8_t direction);
void registrationSecuritySwap(DrmMessagePacked *messagePacked);
uint8_t setHash(uint8_t keyId, uint8_t *keyHash);
void wrapSecureMessage(DrmMessagePacked *transformMessage);
void unwrapSecureMessage(DrmMessagePacked *transformMessage);

#ifdef __cplusplus
};
#endif

#endif
/*DRMMESSAGE_H_INCLUDE*/
