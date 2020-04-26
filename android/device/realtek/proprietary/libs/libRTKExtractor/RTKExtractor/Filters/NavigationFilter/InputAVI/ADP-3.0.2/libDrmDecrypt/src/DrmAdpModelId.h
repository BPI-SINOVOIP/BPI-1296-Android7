/*
// $Header: /Mux_Demux_SDK60/libDrmDecrypt/src/DrmAdpModelId.h 1     2/14/05 11:22a Sbramwell $
// Copyright (c) DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.
*/

/*
This model number is assigned to each product by DivXNetworks.
Value 0x31, 0x30 is a TEST value only.  The 12 bits of your
model number will be part of your local implementation of the DRM.
*/

#ifndef DRMADPMODELID_H_INCLUDE
#define _H_INCLUDE

#define MODEL_ID_LOW_BYTE 0x31
#define MODEL_ID_TOP_BYTE 0x30 & 0xF0 /* Only half of the byte counts. */

#endif
/*DRMADPMODELID_H_INCLUDE*/
