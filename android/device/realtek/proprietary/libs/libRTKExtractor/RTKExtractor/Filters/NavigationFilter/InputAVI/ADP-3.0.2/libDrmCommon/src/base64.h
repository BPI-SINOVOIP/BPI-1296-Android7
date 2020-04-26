// $Header: /Mux_Demux_SDK60/libDrmCommon/src/base64.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef BASE_64_H
#define BASE_64_H

#include <malloc.h>
#include <stdio.h>

//
// Base-64 encode & decode
// 
#ifdef __cplusplus
extern "C" {
#endif

char *base64Encode(char *buffer, 
				   unsigned int length, 
				   unsigned int *outlength);

char *base64Decode(char *str,
				   unsigned int length,
				   unsigned int *outlength);

int validBase64Data(char *str, unsigned int length);
int validBase32Data(char *str, unsigned int length);
//
// Base-32 encode & decode
// 
void base32Encode40Bits(unsigned char *input, char *output);
void base32Decode40Bits(char *input, unsigned char *output);

#ifdef __cplusplus
};
#endif

#endif // BASE_64_H
