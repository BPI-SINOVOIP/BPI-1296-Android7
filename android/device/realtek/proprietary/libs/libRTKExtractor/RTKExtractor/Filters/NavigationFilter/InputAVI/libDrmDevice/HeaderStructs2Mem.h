/*
   // $Id: $
   // Copyright (c) 2006 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
 */
#ifndef __STRUCTS2MEM_H
#define __STRUCTS2MEM_H

#include "DivXAesDecrypt.h"
#include "DivXAesEncrypt.h"
#include "DrmTypes.h"

#if defined __cplusplus
extern "C"
{
#endif
                                        
int32_t frameKeys2Mem( const drmFrameKeys_t* frameKeys,
                       uint8_t** mem,
                       aesKeyHandle keyHandle );
int32_t targetHeader2Mem( const drmTargetHeader_t* targetHeader,
                          uint8_t** mem,
                          aesKeyHandle keyHandle );
int32_t drmMemory2Mem( drmMemory_t * drmMemory,
                       uint8_t * * mem,
                       aesKeyHandle keyHandle );

#if defined __cplusplus
}
#endif

#endif

