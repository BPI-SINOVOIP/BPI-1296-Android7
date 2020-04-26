/*
// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/DrmMemory.h 1     2/14/05 11:22a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
These functions and structures allow representation and manipulation
of the memory used by the ADP DRM.
*/

#ifndef DRMMEMORY_H_INCLUDE
#define DRMMEMORY_H_INCLUDE

#include "portab.h" /* uint8_t, uint16_t */
#include "DrmSizes.h"

typedef struct OwnerSlotStruct
{
    uint8_t guard[OWNER_GUARD_BYTES];
    uint8_t key[KEY_SIZE_BYTES];
    uint8_t userId[OWNER_USER_ID_BYTES];
} OwnerSlot;

typedef struct PlaySlotStruct
{
    uint8_t serialNumber[SLOT_SERIAL_NUMBER_BYTES];
    uint8_t counter;
} PlaySlot;

typedef struct DrmMemoryStruct
{
    OwnerSlot owner;
    PlaySlot slots[TOTAL_PLAY_SLOTS];
} DrmMemory;

typedef struct DrmPackedMemoryStruct
{
    uint8_t packed[PACKED_ALLOCATION_BYTES];
} DrmPackedMemory;

#ifdef __cplusplus
extern "C" {
#endif

uint8_t packDrmMemory(DrmMemory memory, DrmPackedMemory *packedMemory);
uint8_t unpackDrmMemory(DrmPackedMemory packedMemory, DrmMemory *memory);
uint8_t packLogic(DrmMemory *memory, DrmPackedMemory *packedMemory, uint8_t direction);

uint8_t randomizeGuard(DrmMemory *memory);

uint8_t loadDrmMemory(DrmMemory *memory);
uint8_t saveDrmMemory(DrmMemory *memory);

#ifdef __cplusplus
};
#endif

#endif
/*DRMMEMORY_H_INCLUDE*/
