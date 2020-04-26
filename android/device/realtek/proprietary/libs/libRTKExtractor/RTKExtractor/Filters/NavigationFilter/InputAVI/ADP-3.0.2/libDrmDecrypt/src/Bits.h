/*
// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/Bits.h 1     2/14/05 11:22a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
These functions help with bit manipulation and conversions of data.
*/

#ifndef BITS_H_INCLUDE
#define BITS_H_INCLUDE

#include "../../common/portab.h" /* uint8_t, uint16_t */

#define A_BECOMES_B 1
#define B_BECOMES_A 2

#ifdef __cplusplus
extern "C" {
#endif

uint16_t pack(uint8_t low, uint8_t hi);
void unpack(uint16_t whole, uint8_t *hi, uint8_t *low);
void assign(uint8_t *a, uint8_t *b, uint8_t direction);
uint8_t getByteLoopSizeFromBits(uint8_t sizeInBits);

void quarterByteSwap(uint8_t *a, uint8_t *b, uint8_t position);
void halfByteSwap(uint8_t *a, uint8_t *b, uint8_t position);
void maskSwap(uint8_t *a, uint8_t *b, uint8_t mask);

#ifdef __cplusplus
};
#endif

#endif
/*BITS_H_INCLUDE*/
