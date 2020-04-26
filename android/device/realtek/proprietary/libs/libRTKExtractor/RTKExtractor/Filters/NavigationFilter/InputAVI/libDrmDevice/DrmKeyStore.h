/*
// $Id: DrmAdpKeyStore.h 1896 2006-08-05 01:40:19Z fchan $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KEYSTORE_H
#define __KEYSTORE_H

#include "DivXInt.h"

// Sizes.
#define SIZEOF_BASE_KEY_SIZE_BYTES			32
#define SIZEOF_BASE_KEY_SIZE_BITS			256
#define SIZEOF_BASE_KEY_ID_SIZE_BYTES		44
#define SIZEOF_MESSAGE_KEY_SIZE_BYTES		20
#define HEX_OCTET_PER_BYTE               2

#ifdef __cplusplus
extern "C" {
#endif

int32_t keyStoreGetMessageKey( uint8_t protectionFormatFlag,
                               uint8_t *messageKey );

int32_t keyStoreGetProtectionFormat( uint8_t *protectionFormatFlag,
                                     uint8_t* message );

#ifdef __cplusplus
};
#endif

#endif
