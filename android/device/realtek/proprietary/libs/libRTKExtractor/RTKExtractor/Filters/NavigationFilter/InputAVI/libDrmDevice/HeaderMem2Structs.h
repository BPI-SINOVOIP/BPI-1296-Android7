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
#ifndef __MEM2STRUCTS_H
#define __MEM2STRUCTS_H

#include "DrmTypes.h"
#include "DivXAesDecrypt.h"

#if defined __cplusplus
extern "C"
{
#endif

int32_t mem2FrameKeys( uint8_t** mem,
                       drmFrameKeys_t* frameKeys,
                       aesKeyHandle keyHandle );
int32_t mem2TargetHeader( uint8_t** mem,
                          drmTargetHeader_t* targetHeader,
                          aesKeyHandle keyHandle );
int32_t mem2TransactionHeader( uint8_t** mem,
                               drmTransactionInfoHeader_t* transactionHeader,
                               aesKeyHandle keyHandle );
int32_t mem2VideoDDChunk( uint8_t * * mem,
                          drmVideoDDChunk_t * videoDDChunk,
                          aesKeyHandle keyHandle );
int32_t mem2DrmMemory( uint8_t * * mem,
                       drmMemory_t * drmMemory,
                       aesKeyHandle keyHandle );


#if defined __cplusplus
}
#endif

#endif

