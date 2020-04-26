/*
// $Header: /Mux_Demux_SDK60/libDrmCommon/src/DrmHash.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

#ifndef DRMHASH_H_INCLUDE
#define DRMHASH_H_INCLUDE

#include "../../common/portab.h" /* uint8_t, uint16_t */

#ifdef __cplusplus
extern "C" {
#endif

void hash32to16(uint8_t *from, uint8_t *to);
void hashTo16(uint8_t *from, uint8_t fromSize, uint8_t *to);

#ifdef __cplusplus
};
#endif

#endif
/*DRMHASH_H_INCLUDE*/
