// $Header: /Mux_Demux_SDK60/libDrmCommon/src/sha1.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef SHA_1_H
#define SHA_1_H

#include <malloc.h>
#include <stdlib.h>
#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// sha1GetHash
//
// Gets the SHA1 hash string for
// the given buffer (should be at 
// least a few hundreds bytes long)
//
//

char *sha1GetHash(unsigned char *buffer, 
				  unsigned int length, 
				  unsigned int *outlength);

#ifdef __cplusplus
};
#endif


#endif // SHA_1_H
