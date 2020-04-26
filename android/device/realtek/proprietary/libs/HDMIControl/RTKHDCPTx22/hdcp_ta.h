/*
 * Copyright (c) 2014, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TA_HDCP_H
#define TA_HDCP_H

/* This UUID is generated with uuidgen
   the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_HDCP_UUID { 0x8baaf200, 0x2450, 0x11e4, \
		{ 0xab, 0xe2, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

/* The TAFs ID implemented in this TA */

#define TA_HELLO_WORLD_CMD_INC_VALUE 0xff

enum WIDEVINE_CMD_FOR_TA
{
	TA_TEE_OpenKeybox 			= 0x1,
	TA_TEE_IsKeyboxValid 		= 0x2,
	TA_TEE_GetKeyData			= 0x3,
	TA_TEE_GetDeviceID 			= 0x4,
	TA_TEE_RandomData 			= 0x5,
	TA_TEE_SetIVBuffer			= 0x6,
	TA_TEE_SetEntitlementKey 	= 0x7,
	TA_TEE_DeriveControlWord 	= 0x8,
	TA_TEE_DecryptAVContent 	= 0x9,
	TA_TEE_HDCPSendAkeInit = 0xa,
	TA_TEE_HDCPCheckLLCSignature = 0xb,
	TA_TEE_HDCPSendAkeNoStoredKm = 0xc,
	TA_TEE_HDCPSendLCInit = 0xd,
	TA_TEE_HDCPComputeL = 0xe,
	TA_TEE_HDCPSendSke = 0xf,
	TA_TEE_HDCPLC128Cipher = 0x10,
	TA_TEE_HDCPComputeH = 0x11,
	TA_TEE_HDCPSaveRxInfo = 0x12,
	TA_TEE_HDCPGetRxInfo = 0x13,
	TA_TEE_HDCPReceiverIDList = 0x14,
	TA_TEE_HDCPComputeM =0x15,
	TA_TEE_HDCPLC128CipherRESET = 0x16,
	TA_TEE_HDCP22_SetParamKey = 0x17,
	TA_TEE_HDCP22_GetParamKey = 0x18
	
};

#define AES_BLOCK_SIZE 16


#endif /*TA_HELLO_WORLD_H*/
