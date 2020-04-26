/*
// $Id: AudioDecryptTypes.h 783 2006-03-29 19:13:54Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef AUDIODECRYPTTYPES_H_INCLUDE
#define AUDIODECRYPTTYPES_H_INCLUDE

#include "DivXInt.h"

#define AUDIO_KEY_SIZE_BYTES				16
#define AUDIO_CRYPTO_BLOCK_SIZE_BYTES		16
#define AUDIO_KEY_SIZE_BITS					128
#define AUDIO_MAX_ENCRYPT_BYTES             128
#define AUDIO_USE_VIDEO_KEY_INDEX           0

typedef struct AudioCryptoInfoStruct
{
    uint8_t key[AUDIO_KEY_SIZE_BYTES];
	uint16_t offset;
	uint16_t size;
} AudioCryptoInfo;

#endif
// AUDIODECRYPTTYPES_H_INCLUDE
