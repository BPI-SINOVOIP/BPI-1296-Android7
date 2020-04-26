/*
// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/DrmAdpApi.h 3     3/17/05 5:49p Bfudge $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivXNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
These functions are the top level interface to the Authorized Disconnected Playback (ADP)
Digital Rights Management (DRM) capabilities.  The names correspond to the  DivX
Authorized Disconnected Playback document and the user interface flow.
*/

/** @defgroup drm libDrmDecrypt API
 *  Description of drm decrypt
 *  @{
 */
#ifndef DRMADPAPI_H_INCLUDE
#define DRMADPAPI_H_INCLUDE

#include "portab.h" /* uint8_t, uint16_t */

#define DRM_CONTEXT_SIZE_BYTES			2226
#define DRM_REGISTRATION_CODE_BYTES		9
#define DRM_HEADER_SIZE_BYTES			2224

#ifdef __cplusplus
extern "C" {
#endif

/* Random number functions. */
/** @brief @b drmSetRandomSample creates the set of random samples used within the DRM routines

@return void 

@b Note: Call this at least 3 times from user input before call to drmGetRegistrationCode or drmInitPlayback.
DrmLocal.c should be modified with the platform specific version of the function ::localGetRapidUpdater if clock() does not update fast enough

*/
void drmSetRandomSample();


/** @brief @b drmGetRandomSampleCounter is used to determine the number of random numbers available

@return uint32_t
@retval Number of random samples available
*/ 
uint32_t drmGetRandomSampleCounter(); 

/* Owner Registration Function. */
/** @brief @b drmGetRegistrationCodeString is used to obtain the product registration code, in an encrypted, human
readable form.

@param[out] registrationCodeString - null terminated string representing the registration code

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval other see DrmErrors.h

@b Note: DrmLocal.c should be modified with the platform specific version of the function ::localLoadDrmMemory
*/
uint8_t drmGetRegistrationCodeString(char *registrationCodeString);


/* Playback functions. */
/** @brief @b drmInitPlaybackContext 

@param[in] drmHeaderFromFile - header info in the AVI file to be played
@param[out] drmContext - pointer to block of memory used for calls to ::drmQueryRentalStatus,,::drmCommitPlayback,
and ::drmDecryptFrame 

@retval DRM_ERROR_NONE - success
@retval other see DrmErrors.h

*/
uint8_t drmInitPlaybackContext(uint8_t *drmHeaderFromFile, uint8_t *drmContext);


/** @brief @b drmQueryRentalStatus is used to determine the rental status of the AVI file used to create the @c drmContext

@param[in] drmContext - pointer to the block of memory returned by ::drmInitPlaybackContext
@param[out] useLimit - returned the number of playbacks allowed for the AVI file
@param[out] useCount - returned the number of time the AVI file has been played back

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_NOT_INITIALIZED
@retval DRM_ERROR_NOT_LIMITED_RENTAL_TYPE
@retval DRM_ERROR_NOT_AUTH_USER
@retval DRM_ERROR_BAD_SLOT_NUMBER
@retval DRM_ERROR_RENTAL_EXPIRED

*/
uint8_t drmQueryRentalStatus(uint8_t *drmContext, uint8_t *useLimit,
							 uint8_t *useCount);  /* useLimit and useCount are out
													 parameters. */
/** @brief @b drmCommitPlayback 

@param[in] drmContext - pointer to the block of memory returned by ::drmInitPlaybackContext

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_NOT_RENTAL_QUERIED
@retval DRM_ERROR_DECRYPTION_FAILED
@retval DRM_ERROR_BAD_ENCRYPT_CONTEXT

*/
uint8_t drmCommitPlayback(uint8_t *drmContext);

/** @brief @b drmDecryptFrame 

@param[in] drmContext - pointer to the block of memory returned by ::drmInitPlaybackContext
@param[in,out] frame - pointer to memory buffer containing the encrypted frame data
@param[in] frameSize - size of the data in the @c frame buffer
@param[in] drmFrameInfo - pointer to the memory buffer containing the DRM info for the frame 

@return uint8_t
@retval DRM_ERROR_NONE - success
@retval DRM_ERROR_NOT_COMMITTED
@retval DRM_ERROR_DECRYPTION_FAILED
@retval DRM_ERROR_BAD_ENCRYPT_CONTEXT

@b Note: function uses malloc and free to create memory buffers
*/
uint8_t drmDecryptFrame(uint8_t *drmContext, uint8_t *frame, uint32_t frameSize, 
						uint8_t *drmFrameInfo);

/**  @}  */

#ifdef __cplusplus
};
#endif

#endif
/*DRMADPAPI_H_INCLUDE*/
