/*
// $Id: AudioDecrypt.h 783 2006-03-29 19:13:54Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef AUDIODECRYPT_H_INCLUDE
#define AUDIODECRYPT_H_INCLUDE

#include "AudioDecryptTypes.h"
#include "DivXInt.h"

#define AUDIO_DECRYPT_ERROR_NONE			0
#define AUDDEC_ERROR_CRYPTO_FAILURE			130

#ifdef __cplusplus
extern "C" {
#endif

uint8_t decryptAudio(AudioCryptoInfo *audioSettings, uint8_t *frame, uint32_t frameSize);

#ifdef __cplusplus
};
#endif

#endif
// AUDIODECRYPT_H_INCLUDE
