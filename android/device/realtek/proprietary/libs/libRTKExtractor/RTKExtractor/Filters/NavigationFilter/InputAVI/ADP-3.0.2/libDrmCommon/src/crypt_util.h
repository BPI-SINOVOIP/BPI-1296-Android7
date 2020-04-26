// $Header: /Mux_Demux_SDK60/libDrmCommon/src/crypt_util.h 1     2/14/05 11:21a Sbramwell $
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef CRYPT_UTIL_H
#define CRYPT_UTIL_H

#include "rijndael-api-fst.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "portab.h"

int crypt_util_make_key(unsigned char *keydata, int direction, keyInstance *key, int keyLength);
void crypt_util_encrypt(cipherInstance * cipher, keyInstance *key, unsigned char *clearBuffer, 
						int clearBufferSize, unsigned char *encryptedBuffer, int * encryptedBufferSize );

void crypt_util_decrypt(cipherInstance * cipher, keyInstance *key, unsigned char *encryptedBuffer, 
						int encryptedBufferSize, unsigned char *clearBuffer, int * clearBufferSize );


#ifdef __cplusplus
};
#endif

#endif // CRYPT_UTIL_H
