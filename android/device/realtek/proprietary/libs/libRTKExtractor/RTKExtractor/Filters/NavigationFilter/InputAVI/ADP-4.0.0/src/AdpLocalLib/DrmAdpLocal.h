/*
// $Id: DrmAdpLocal.h 783 2006-03-29 19:13:54Z egrab $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*
These function are the actual implementation to read and write from
the persistent memory.

Production players need to implement these functions according to 
how the memory is stored in the product.
*/

#ifndef DRMADPLOCAL_H_INCLUDE
#define DRMADPLOCAL_H_INCLUDE

#include "AdpLib/DrmAdpTypes.h"

// Returns.
#define ADP_LOCAL_SUCCESS                   0
#define DRM_ERROR_READING_MEMORY			12			
#define DRM_ERROR_WRITING_MEMORY			13

#define DEFAULT_STORAGE_FILE "/usr/local/etc/dvdplayer/dmem.bin"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t localClock();
uint32_t localClockTicksPerSec();
void localSetModelId(uint8_t *modelId);
uint8_t localLoadDrmMemory(uint8_t *memory);
uint8_t localSaveDrmMemory(uint8_t *memory);

#ifdef ADP_HARDWARE_SIMULATED
void setDrmLocalFileName(char *value);
#endif

#ifdef __cplusplus
};
#endif

#endif
// DRMADPLOCAL_H_INCLUDE
