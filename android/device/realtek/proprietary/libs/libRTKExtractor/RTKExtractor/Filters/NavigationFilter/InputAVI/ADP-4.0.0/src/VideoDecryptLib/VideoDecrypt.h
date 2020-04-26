/*
// $Id: VideoDecrypt.h 783 2006-03-29 19:13:54Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef VIDEODECRYPT_H_INCLUDE
#define VIDEODECRYPT_H_INCLUDE

#include "VideoDecryptTypes.h"

#define VIDEO_DECRYPT_ERROR_NONE			0
#define VIDDEC_ERROR_CRYPTO_FAILURE			100

#ifdef __cplusplus
extern "C" {
#endif

uint8_t decryptVideo(VideoFrameKeys *frameKeys, uint8_t *frame, uint32_t frameSize, uint8_t *drmFrameInfo);
void copyToVideoCryptoInfo(uint8_t *drmFrameInfo, VideoCryptoInfo *cryptoInfo);

#ifdef __cplusplus
};
#endif

#endif
// VIDEODECRYPT_H_INCLUDE
