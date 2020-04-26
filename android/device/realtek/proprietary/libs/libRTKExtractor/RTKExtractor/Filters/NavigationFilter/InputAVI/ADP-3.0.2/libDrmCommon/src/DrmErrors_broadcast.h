/*
// $Header: /Mux_Demux_SDK60/libDrmCommon/src/DrmErrors_broadcast.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivXNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
These constants define the error message returned from the DrmAdpApi.
*/

#ifndef DRMERRORS_H_INCLUDE
#define DRMERRORS_H_INCLUDE

#define DRM_ERROR_NONE						0

#define DRM_ERROR_INVALID_MASTER_KEY		10  /* from: drmInitPlaybackContext */
#define DRM_ERROR_DECRYPTION_FAILED			11  /* from: drmDecryptFrame */
#define DRM_ERROR_READING_MEMORY			12	/* from: DrmLocal (reserved as local read 
												  	     memory error */
#define DRM_ERROR_WRITING_MEMORY			13  /* from: DrmLocal (reserved s local write
														 memory error) */
#define DRM_ERROR_UNRECOGNIZED_DRM_MODE		14  /* from: drmInitPlaybackContext */
#define DRM_ERROR_NOT_AUTH_USER             15  /* from: drmQueryRentalStatus, drmCommitPlayback */
#define DRM_ERROR_RENTAL_EXPIRED            16  /* from: drmQueryRentalStatus, drmCommitPlayback */
#define DRM_ERROR_NEED_MORE_RANDOM_SAMPLE	17  /* from: drmGetRegistrationCodeString, 
														 drmCommitPlayback */
#define DRM_ERROR_NOT_INITIALIZED			18  /* from: drmQueryRentalStatus */
#define DRM_ERROR_NOT_LIMITED_RENTAL_TYPE	19  /* from: drmQueryRentalStatus */ 
#define DRM_ERROR_NOT_COMMITTED				20  /* from: drmDecryptFrame */
#define DRM_ERROR_NOT_RENTAL_QUERIED		21  /* from: drmCommitPlayback */
#define DRM_ERROR_BAD_SLOT_NUMBER           22  /* from: drmQueryRentalStatus */
#define DRM_ERROR_NULL_GUARD_SET_SIGNAL		23	/* from: drmCommitPlayback (for certification
														 testing support). */
#define DRM_ERROR_INVALID_ALIGNMENT			24	/* from: drmInitPlaybackContext */
#define DRM_NO_LOCAL_HARDWARE_KEY           25  /* from: localGetHardwareKey */

// Encrypt related only errors.
#define DRM_ERROR_BAD_ENCRYPT_CONTEXT		101
#define DRM_ERROR_FAILED_TO_ENCRYPT			102

/* NOTE */
/* if you want to build your application with 60 keys, comment following two defines  */
/* if you want to build your application with insecure key 59, comment the BROADCAST_FLAG define*/
/* if you want to build your application with broadcast key 58, comment the INSECURE_FLAG define*/
#define BROADCAST_FLAG 1
//#define INSECURE_FLAG 1

#ifdef INSECURE_FLAG 
#define USE_ONE_MASTER_KEY 1
#endif

#ifdef  BROADCAST_FLAG
#define USE_ONE_MASTER_KEY 1
#endif

#endif DRMERRORS_H_INCLUDE
