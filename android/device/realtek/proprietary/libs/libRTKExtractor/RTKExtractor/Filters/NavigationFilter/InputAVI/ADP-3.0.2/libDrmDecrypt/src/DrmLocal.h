/*
// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/DrmLocal.h 2     2/18/05 11:06a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
These function are the actual implementation to read and write from
the persistent memory.

Production players need to implement these functions according to 
how the memory is stored in the product.
*/

#ifndef DRMLOCAL_H_INCLUDE
#define DRMLOCAL_H_INCLUDE

#include "portab.h" /* uint8_t, uint32_t */

#include "DrmMemory.h"

/** @brief @b DEFAULT_STORAGE_FILE is used only for PC based reference code only

*/ 
#define DEFAULT_STORAGE_FILE "/usr/local/etc/dvdplayer/dmem.bin"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief @b localLoadDrmMemory is used to retreive data from the secure memory on the playback device

The DRM memory must be stored in secure memory that is persistent between power on and power
down.

@param[in] memory - pointer to the ::DrmMemory structure

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_READING_MEMORY

@b Note: The DRM memory must be stored in secure memory that is persistent between power on and power. This function must be modified 
with the code that will access the non-volatile memory of the product.
*/
uint8_t localLoadDrmMemory(DrmMemory *memory);


/** @brief @b localSaveDrmMemory is used to store data to the secure memory on the playback device

The DRM memory must be stored in secure memory that is persistent between power on and power
down.

@param[in] memory - pointer to the ::DrmMemory structure

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_WRITING_MEMORY

@b Note: The DRM memory must be stored in secure memory that is persistent between power on and power. This function must be modified 
with the code that will access the non-volatile memory of the product.
*/
uint8_t localSaveDrmMemory(DrmMemory *memory);

/** @brief @b localGetRapidUpdater is called to generate random numbers

@return uint8_t 
@retval updated value

@b Note: Sample values are taken from a rapidly
changing variable. The variable must cycle from 0 to 256 uniformly every 50 milliseconds or
faster. The routine provided uses the clock function. If the target hardware does not support a
clock function, a similar mechanism using a system counter must be implemented.
*/
uint8_t localGetRapidUpdater();

/** @brief @b localWaitSecond is used to create a delay between successive attempts to activate a product.


@return void

@b Note: This routine is used to create a delay between successive attempts to activate a product. The
reference implementation is based on the clock() call from the standard library time.h. Check to see
if this works correctly for your platform. If not, you must provide a new function that delays
between .5 and 2.5 seconds when called.
*/
void localWaitSecond();

/** @brief @b localGetHardwareKey is used to get the local hardware key for the product.

@param[in,out] hardwareKey - pointer to memory allocated by caller to contain the hardware key

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_NO_LOCAL_HARDWARE_KEY

@b Note: Replace with platform specific implementation
*/
uint8_t localGetHardwareKey(uint8_t *hardwareKey);

/** @brief @b localLoadAsFile is a file manipulation routine included in DrmLocal.c for the PC based reference code only

@param[in,out] memoryPacked - pointer to memory allocated by caller to contain the DRM information

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_READING_MEMORY

@b Note: All file manipulation routines included in DrmLocal.c are for the PC based reference code only.
*/
uint8_t localLoadAsFile(DrmPackedMemory *memoryPacked);


/** @brief @b localSaveAsFile is a file manipulation routine included in DrmLocal.c for the PC based reference code only

@param[in] memoryPacked - memory block containing the DRM information

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_WRITING_MEMORY

@b Note: All file manipulation routines included in DrmLocal.c are for the PC based reference code only.
*/
uint8_t localSaveAsFile(DrmPackedMemory memoryPacked);


/** @brief @b setDrmLocalFileName is used to set the file name used to store the DRM memory and is included in DrmLocal.c for the PC based reference code only

@param[in] value - null terminated string containing filename

@return void 

@b Note: All file manipulation routines included in DrmLocal.c are for the PC based reference code only.
*/
void setDrmLocalFileName(char *value);

#ifdef __cplusplus
};
#endif

#endif 
/*DRMLOCAL_H_INCLUDE*/
