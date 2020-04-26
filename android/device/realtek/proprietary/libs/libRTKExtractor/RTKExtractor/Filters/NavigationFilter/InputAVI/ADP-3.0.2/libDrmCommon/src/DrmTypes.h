// $Header: /Mux_Demux_SDK60/libDrmCommon/src/DrmTypes.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivXNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef DRM_DEFINES_H
#define DRM_DEFINES_H

#include "DrmSizes.h"

// Key Source Modes
#define KEY_SOURCE_MODE_BASE_FLAGS				0x0001
#define KEY_SOURCE_MODE_CONNECTED_FLAGS			0x0002
#define KEY_SOURCE_MODE_DISCONNECTED_FLAGS		0x0004

// DRM Modes
#define DRM_TYPE_BASE						0xF0F0
#define DRM_TYPE_ACTIVATION_PURCHASE		0xC3C3
#define DRM_TYPE_ACTIVATION_RENTAL			0x3C3C
#define DRM_TYPE_PURCHASE					0x5555
#define DRM_TYPE_RENTAL						0xAAAA

// Use Limits.
#define USE_LIMIT_ID_UNLIMITED		0x0707
#define USE_LIMIT_ID_ONE			0x1001		
#define USE_LIMIT_ID_THREE			0x3003
#define USE_LIMIT_ID_FIVE			0x5005
#define USE_LIMIT_ID_SEVEN			0x7007
#define USE_LIMIT_ID_TEN			0xAAAA
#define USE_LIMIT_ID_TWENTY			0x5555
#define USE_LIMIT_ID_THIRTY			0xF0F0

typedef struct DrmUseLimitIdInfoStruct
{
    uint16_t id;
    uint8_t uses;
} DrmUseLimitIdInfo;

static DrmUseLimitIdInfo useLimitIdInfo[USE_LIMITS] =
{
	{USE_LIMIT_ID_UNLIMITED, 0},
	{USE_LIMIT_ID_ONE, 1},
	{USE_LIMIT_ID_THREE, 3},
	{USE_LIMIT_ID_FIVE, 5},
	{USE_LIMIT_ID_SEVEN, 7},
	{USE_LIMIT_ID_TEN, 10},
	{USE_LIMIT_ID_TWENTY, 20},
	{USE_LIMIT_ID_THIRTY, 30}
};

static uint8_t nullGuard[OWNER_GUARD_BYTES] = {0,0,0};

typedef struct DrmActivateRecordStruct
{
	uint8_t memoryGuard[OWNER_GUARD_BYTES];
	uint8_t modelId[MODEL_ID_BYTES];
	uint8_t userKey[KEY_SIZE_BYTES];
	uint8_t reserved[3];
} DrmActivateRecord;

typedef struct DrmRentalRecordStruct
{
	uint16_t useLimitId;
	uint8_t serialNumber[SLOT_SERIAL_NUMBER_BYTES];
	uint8_t slotNumber;
	uint8_t reserved[3];
} DrmRentalRecord;

typedef struct DrmAdpTargetHeaderStruct
{
	uint16_t drmMode;
	uint8_t userId[OWNER_USER_ID_BYTES];
	uint8_t reservedAlign;
    DrmRentalRecord rentalRecord;
	uint8_t sessionKey[KEY_SIZE_BYTES];
	DrmActivateRecord activateRecord;
	uint8_t reserved[DRM_ADP_RESERVED];
} DrmAdpTargetHeader;

typedef struct DrmTransactionInfoHeaderStruct
{
	uint8_t transactionId[TRANSACTION_ID_BYTES];
	uint16_t transactionAuthorityId;
	uint32_t contentId;
	uint8_t reserved[DRM_OTHER_RESERVED];
} DrmTransactionInfoHeader;

typedef struct DrmHeaderStruct
{
	uint32_t keySourceMode;
	uint8_t masterKeyId[DRM_PORTABLE_KEY_LENGTH];
	DrmAdpTargetHeader adpTarget;
	DrmTransactionInfoHeader transaction;
	uint8_t frameKeys[DRM_FRAME_KEY_COUNT][KEY_SIZE_BYTES];
} DrmHeader;

// DRM Playback Context States
#define DRM_STATE_INITIALIZED				1
#define DRM_STATE_RENTAL_QUERIED			2
#define DRM_STATE_COMMITTED					3

typedef struct DrmPlaybackContextStruct
{
	DrmHeader clearHeader;
	uint8_t state;
} DrmPlaybackContext;

#endif
/*DRM_DEFINES_H*/
