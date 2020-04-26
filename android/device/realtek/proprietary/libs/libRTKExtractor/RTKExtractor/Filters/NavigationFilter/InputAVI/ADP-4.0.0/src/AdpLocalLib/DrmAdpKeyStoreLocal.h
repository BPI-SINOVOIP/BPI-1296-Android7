/*
// $Id: DrmAdpKeyStoreLocal.h 1375 2006-06-12 17:24:57Z mkiefer $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef DRMADPKEYSTORELOCAL_H_INCLUDE
#define DRMADPKEYSTORELOCAL_H_INCLUDE

#include "DivXInt.h"
#include "AdpLib/DrmAdpTypes.h"

// Sizes.
#define ADP_HARDWARE_SECRET_BYTES	32
#define ADP_HARDWARE_KEY_BYTES      16
#define ADP_HARDWARE_KEY_BITS       128

// Returns.
#define ADP_NO_LOCAL_HARDWARE_KEY    60
#define ADP_NO_LOCAL_PUBLIC_KEY      61

#ifdef __cplusplus
extern "C" {
#endif

void localGetHardwareSecret(uint8_t *hardwareKey);
uint8_t localGetPublicKeyFromId(const uint32_t id, char nHex[ADP_RSA_HEX_KEY_SIZE_BYTES], char eHex[ADP_RSA_HEX_E_EXP_SIZE_BYTES]);

#ifdef __cplusplus
};
#endif

#endif
// DRMADPKEYSTORELOCAL_H_INCLUDE
