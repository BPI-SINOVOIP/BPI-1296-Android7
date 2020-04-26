/*
// $Id: DrmAdpKeyStore.h 1250 2006-06-06 22:35:28Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef ADPKEYSTORE_H
#define ADPKEYSTORE_H

#include "DivXInt.h"
#include "AdpLib/DrmAdpTypes.h"

// Errors.
#define ERROR_KEYSTORE_NONE					     0
#define ERROR_KEYSTORE_INVALID_BASE_KEY			50
#define ERROR_KEYSTORE_DECRYPTION_FAILED		51
#define ERROR_KEYSTORE_MEMORY					52
#define ERROR_KEYSTORE_KEY_TOO_BIG			    54
#define ERROR_KEYSTORE_ID_NOT_FOUND             55

// Sizes.
#define ADP_BASE_KEY_SIZE_BYTES			32
#define ADP_BASE_KEY_SIZE_BITS			256
#define ADP_BASE_KEY_ID_SIZE_BYTES		44
#define ADP_MESSAGE_KEY_SIZE_BYTES		20
#define HEX_OCTET_PER_BYTE               2

#ifdef __cplusplus
extern "C" {
#endif

uint8_t getAdpBaseKeyIdFromIndex(uint8_t index, uint8_t *baseKeyId);
uint8_t getAdpBaseKeyFromId(const uint8_t *baseKeyId, uint8_t *adpBaseKey);
uint8_t getAdpBaseHexKeyFromId(const uint8_t *baseKeyId, char *adpBaseHexKey);
uint8_t getProtectionFormat(uint8_t *protectionFormatFlag);
uint8_t getMessageKey(uint8_t protectionFormatFlag, uint8_t *messageKey);
uint8_t getPublicKeyFromId(const uint32_t id,
						   char nHex[ADP_RSA_HEX_KEY_SIZE_BYTES],
						   char eHex[ADP_RSA_HEX_E_EXP_SIZE_BYTES]);
#ifdef __cplusplus
};
#endif


#endif
//ADPKEYSTORE_H
