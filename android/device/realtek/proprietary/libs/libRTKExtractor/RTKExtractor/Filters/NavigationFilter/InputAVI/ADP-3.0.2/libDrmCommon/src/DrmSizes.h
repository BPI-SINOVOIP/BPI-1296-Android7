/*
// $Header: /Mux_Demux_SDK60/libDrmCommon/src/DrmSizes.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivXNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
This file defines various sizes used by the ADP DRM.
*/

#ifndef DRM_SIZES_H_INCLUDE
#define DRM_SIZES_H_INCLUDE

/* Memory sizes. */
#define TOTAL_OWNERS                    1
#define TOTAL_PLAY_SLOTS                8
#define OWNER_GUARD_BYTES               3
#define OWNER_USER_ID_BYTES             5
#define SLOT_USE_DATA_BYTES				1          
#define SLOT_SERIAL_NUMBER_BYTES		2
#define PACKED_ALLOCATION_BYTES			48

/* Misc sizes. */
#define DRM_TYPES						5
#define USE_LIMITS						8
#define MAX_MESSAGE_SIZE_BYTES          12
#define PROTECTION_FORMAT_BITS          4
#define TRANSACTION_ID_BYTES            46
#define MODEL_ID_BYTES                  2
#define MODEL_ID_BITS					12
#define KEY_SIZE_BYTES					16
#define PORTABLE_SESSION_KEY_SIZE		24
#define MASTER_KEY_SIZE_BYTES			32
#define RANDOM_BYTE_BUCKET_SIZE			6
#define MIN_REGISTRATION_RANDOM_SAMPLES	3
#define DRM_HT_BOUNDED_BYTES			128
#define DRM_BLOCK_ALIGN					16
#define DRM_PORTABLE_KEY_LENGTH			44
#define DRM_FRAME_KEY_COUNT				128
#define DRM_FRAME_DRM_INFO_SIZE			10
#define DRM_ADP_RESERVED                8
#define DRM_OTHER_RESERVED				12
#define DRM_HEADER_DRM_INFO_SIZE        2224

/* Message sizes. */
#define REGISTRATION_CODE_BITS			36
#define REGISTRATION_CHALLENGE_BITS     40
#define REGISTRATION_CODE_STRING_BYTES	9

#endif
/*DRM_SIZES_H_INCLUDE*/
