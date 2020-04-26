/*
// $Id: CryptoRandom.h 783 2006-03-29 19:13:54Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef CRYPTORANDOM_H_INCLUDE
#define CRYPTORANDOM_H_INCLUDE

#include "DivXInt.h"

// Return codes.
#define CRYPTO_RANDOM_ERROR_NONE		0
#define CRYPTO_RANDOM_INIT_FAILURE      350
#define CRYPTO_RANDOM_GET_FAILURE       351

#ifdef __cplusplus
extern "C" {
#endif

int32_t getRandomBytes(uint8_t *buffer, const uint32_t bufferSize);

// Random session will be useful when lots of random data is needed.  Performance will be better.
/*
int32_t getRandomSession();
int32_t getRandomBytesViaSession();
void closeRandomSession();
*/

#ifdef __cplusplus
};
#endif

#endif
// CRYPTORANDOM_H_INCLUDE
