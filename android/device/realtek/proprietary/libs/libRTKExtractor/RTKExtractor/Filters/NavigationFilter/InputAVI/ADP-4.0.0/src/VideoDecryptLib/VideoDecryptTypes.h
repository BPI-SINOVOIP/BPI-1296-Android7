/*
// $Id: VideoDecryptTypes.h 783 2006-03-29 19:13:54Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef VIDEODECRYPTTYPES_H_INCLUDE
#define VIDEODECRYPTTYPES_H_INCLUDE

#include "DivXInt.h"

#define VIDEO_KEY_SIZE_BYTES				16
#define VIDEO_CRYPTO_BLOCK_SIZE_BYTES		16
#define VIDEO_KEY_SIZE_BITS					128
#define VIDEO_KEY_COUNT_MAX					128
#define VIDEO_MAX_ENCRYPT_BYTES             512
#define VIDEO_FRAME_PROTECTION_INFO_BYTES    10

#define KEY_INDEX_SIZE 2
#define OFFSET_SIZE    4
#define CRYPTO_SIZE    4

typedef struct VideoCryptoInfoStruct
{
	uint16_t keyIndex;
	uint32_t offset;
	uint32_t size;
} VideoCryptoInfo;

typedef struct VideoFrameKeysStruct
{
	uint8_t keys[VIDEO_KEY_COUNT_MAX][VIDEO_KEY_SIZE_BYTES];
} VideoFrameKeys;

#endif
// VIDEODECRYPTTYPES_H_INCLUDE
