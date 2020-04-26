// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/DrmAdpHelper.h 1     2/14/05 11:22a Sbramwell $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivXNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef DRM_ADP_HELPER_H
#define DRM_ADP_HELPER_H

#include "../../common/portab.h" /* uint8_t, uint16_t */
#include "../../libDrmCommon/src/DrmTypes.h"
#define DIVX_BIG_ENDIAN      1
#define DIVX_LITTLE_ENDIAN   0

#ifdef __cplusplus
extern "C" {
#endif

uint8_t getRandomByte();
void drmHelperSetRandomSample();
uint32_t drmHelperGetRandomSampleCounter();

uint8_t getSessionKey(DrmAdpTargetHeader fileInfo, uint8_t *sessionKey);  /* Increments use counter if appropriate. */
uint8_t drmHdrCpy(DrmHeader* drmHeader, uint8_t* from);
uint8_t drmHdr2mem(uint8_t* mem, DrmHeader* drmHeader);

uint8_t mem2drmAdpTargetHeader( DrmAdpTargetHeader* drmAdpTH, uint8_t* mem);
uint8_t mem2drmRentalRecord( DrmRentalRecord* rentalRecord, uint8_t* mem);
uint8_t mem2drmActivateRecord( DrmActivateRecord* activateRecord, uint8_t* mem);

uint8_t drmAdpTargetHeader2mem( uint8_t* mem, DrmAdpTargetHeader* drmAdpTH);
uint8_t drmRentalRecord2mem(uint8_t* mem,DrmRentalRecord* rentalRecord);
uint8_t drmActivateRecord2mem(uint8_t* mem, DrmActivateRecord* activateRecord);

#ifdef __cplusplus
};
#endif

#endif
/*DRM_ADP_HELPER_H*/
