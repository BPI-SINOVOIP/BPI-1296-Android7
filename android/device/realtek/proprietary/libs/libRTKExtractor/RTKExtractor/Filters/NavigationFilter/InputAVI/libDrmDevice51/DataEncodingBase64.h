/*
   // $Id: base64.h 450 2006-01-13 02:31:16Z egrab $
   // Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
 */

#ifndef BASE_64_H_INCLUDE
#define BASE_64_H_INCLUDE

#include "DivXInt.h"

/*
 * Base-64 encode & decode
 */
#ifdef __cplusplus
extern "C" {
#endif

int32_t encodingBase64Encode( int8_t *buffer,
                              uint32_t length,
                              int8_t *outbuffer,
                              uint32_t *outlength );

int32_t encodingBase64Decode( int8_t *str,
                              uint32_t length,
                              int8_t *outbuffer,
                              uint32_t *outlength );

int32_t encodingValidBase64Data( int8_t * str,
                                 uint32_t length );
int32_t encodingValidBase32Data( const int8_t * str,
                                 uint32_t length );

/*
 *    Base-32 encode & decode
 */
void encodingBase32Encode40Bits( const uint8_t * input,
                                 int8_t * output );
void encodingBase32Decode40Bits( const int8_t * input,
                                 uint8_t * output );

void encodingBase32Encode50Bits( const uint8_t * input,
                                 int8_t * output );
void encodingBase32Decode50Bits( const int8_t * input,
                                 uint8_t * output );
#ifdef __cplusplus
};
#endif

#endif
//BASE_64_H_INCLUDE
