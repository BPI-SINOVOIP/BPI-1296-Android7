/**
 * HMAC implementation
 *
 * INTEL CONFIDENTIAL
 * Copyright 2010 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its
 * suppliers or licensors.  Title to the Material remains with Intel
 * Corporation or its suppliers and licensors.  The Material contains trade
 * secrets and proprietary and confidential information of Intel or its
 * suppliers and licensors.  The Material is protected by worldwide copyright
 * and trade secret laws and treaty provisions. No part of the Material may
 * be used, copied, reproduced, modified, published, uploaded, posted,
 * transmitted, distributed, or disclosed in any way without Intel's prior
 * express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or
 * delivery of the Materials,  either expressly, by implication, inducement,
 * estoppel or otherwise.  Any license under such intellectual property
 * rights must be express and approved by Intel in writing.
 */

#include <hdcp2_hal.h>
#include <hmac.h>
#include <crypto.h>

#ifdef DEBUG_HMAC
#define LOCALDBG ENABLE
#else
#define LOCALDBG DISABLE
#endif

H2status hmacsha256(const H2uint8 *pKeyIn, unsigned int keylen,
		    const H2uint8 *pSrc, H2uint32 ulSrcSize, H2uint8 *pOut)
{
	H2Sha256Ctx_t c;

	static H2uint8 ipad[64];
	static H2uint8 opad[64];
	static H2uint8 key[64];
	static H2uint8 hash[256 / 8];

	unsigned int ii;
	if (keylen > sizeof(key)) {
		crypto_sha256Init(&c);
		crypto_sha256Update(&c, pKeyIn, keylen);
		crypto_sha256Final(&c, key);
		keylen = SHA256_DIGEST_SIZE;
	} else {
		memcpy(key, pKeyIn, keylen);
	}

	if (keylen < sizeof(key)) {
		memset(key + keylen, 0, sizeof(key) - keylen);
		keylen = sizeof(key);
	}

	for (ii = 0; ii < keylen; ii++) {
		ipad[ii] = 0x36 ^ key[ii];
		opad[ii] = 0x5c ^ key[ii];
	}

	crypto_sha256Init(&c);
	crypto_sha256Update(&c, ipad, sizeof(ipad));
	crypto_sha256Update(&c, pSrc, ulSrcSize);
	crypto_sha256Final(&c, hash);
	crypto_sha256Init(&c);
	crypto_sha256Update(&c, opad, sizeof(opad));
	crypto_sha256Update(&c, hash, sizeof(hash));
	crypto_sha256Final(&c, pOut);
#ifdef DEBUG_HMAC
	H2DBGLOG((LOCALDBG, "hmac-sha256=\r\n"));
	for (ii = 0; ii < SHA256_DIGEST_SIZE; ii++) {
		H2DBGLOG((LOCALDBG, "%x ", pOut[ii]));
	}
	H2DBGLOG((LOCALDBG, "\r\n"));
#endif
	return H2_OK;
}
