/**
 * Includes
 */
#include <hdcp2_hal.h>
#include <hdcp2_messages.h>
#include <hdcp2_interface.h>

/**
 * This file is shared between the XTASK and the userspace application. Both
 * must be compiled with the same version.
 */

/**
 * Bigdigits library. See included copyright notice.
 */
#include <bigdigits.h>

/**
 * VTILT crypto API. Provides prototypes for most functions in this file.
 */
#include <crypto.h>

/**
 * Definition of the HMAC-SHA256 function.
 */
#include <hmac.h>
#include <rcp_api.h>

/**
 * Standard include files
 */


/**
 * This one gets htonl() and ntohl(), etc.
 */

/**
 *
 * Mask Generation Function for RSA-OAEP-SHA256.
 *
 * @see PKCS#1v2.1 for an explanation of the MGF.
 * @param[inout] mask Pointer to mask data
 * @param[in] length Size of mask data
 * @param[in] seed Seed for SHA256
 * @param[in] seedLen Size of seed data.
 * @scope static.
 * @return H2_OK.
 */
H2status MGF1_SHA256(H2uint8 *mask, H2uint32 length, const H2uint8 *seed, int seedLen)
{
	H2uint32 ii = 0, outBytes = 0;
	H2uint8  cnt[4];
	H2Sha256Ctx_t ctx;

	static H2uint8 digest[SHA256_DIGEST_SIZE];

	crypto_sha256Init(&ctx);

	for (ii = 0; outBytes < length; ii++) {
      /** Copy current byte offset to the counter */
      /** Since the copy is done this way, byte order is irrelevant */
		cnt[0] = (H2uint8) ((ii >> 24) & 0xFF);
		cnt[1] = (H2uint8) ((ii >> 16) & 0xFF);
		cnt[2] = (H2uint8) ((ii >> 8) & 0xFF);
		cnt[3] = (H2uint8) ((ii) & 0xFF);
      /** Clear the context */
		memset(&ctx, 0, sizeof(ctx));
		crypto_sha256Init(&ctx);
      /** Add the seed */
		crypto_sha256Update(&ctx, seed, seedLen);
      /** Add the counter */
		crypto_sha256Update(&ctx, cnt, 4);
		if (outBytes + SHA256_DIGEST_SIZE <= length) {
    /** If at least SHA256_DIGEST_SIZE output bytes are needed, copy to output and continue */
			crypto_sha256Final(&ctx, digest);

			memcpy(mask + outBytes, digest, SHA256_DIGEST_SIZE);
			outBytes += SHA256_DIGEST_SIZE;
		} else {
    /** Otherwise, only copy as many bytes as are still needed and terminate */
			crypto_sha256Final(&ctx, digest);

			memcpy(mask+outBytes, digest, length-outBytes);
			outBytes = length;
		}
	}
	return H2_OK;
}

/**
 * d = s1^r2 over len bytes.
 *
 * This implementation is not very efficient.
 *
 * @param[in] s1 Input to xor
 * @param[in] s2 Input to xor
 * @param[out] d Output = s1 ^ s2
 * @param[in] len Length of data to xor
 * @return H2_OK or H2_ERROR if s1, s2, or d are NULL.
 *
 */
H2status crypt_xor(const uint8_t *s1, const uint8_t *s2, uint8_t *d, int len)
{
	int ii;

	if (!s1 || !s2 || !d) {
		return H2_ERROR;
	}

   /** @todo: Optimize by using 32 bit-xors if needed. */
	for (ii = 0 ; ii < len ; ii++) {
		d[ii] = s1[ii] ^ s2[ii];
	}

	return H2_OK;
}

#ifdef TEST_HDCP2_2_RX_DRIVER

typedef struct {
	DIGIT_T *p;
	DIGIT_T *q;
	DIGIT_T *dP;
	DIGIT_T *dQ;
	DIGIT_T *Qinv;
} crypto_RSAKey;


/**
 * @warn Make sure LOCALDBG is set to DISABLE for production
 */

/* to enable or disable the debug messages of this source file, put 1 or 0 below*/
#ifdef DEBUG_CRYPTO
#define LOCALDBG ENABLE

/** @todo Detect prod / release or real/facsimile keys and set debug automatically */
#else
#define LOCALDBG DISABLE
#endif

#define TRACE() H2DBGLOG((LOCALDBG, "File: %s, Line %d\n", __FILE__, __LINE__))

#if 0
#define INFO ENABLE
#else
#define INFO DISABLE
#endif

/**
 * size of the key components for RSA decryption.
 * This should equal key size / 32.
 */
#define MOD_SIZE (1024/32)

/** Prototypes not included in crypto.h */
static int crypto_rsaPrivateDecryptOaepSha256(uint8_t const *pIn, uint8_t *pOut, const crypto_RSAKey *key);

/**
 * Swap byte order of 'length' bytes. This function is intended
 * to convert a SHA-256 digest into the correct byte order expected
 * as input to other functions. Length should be even.
 *
 * @param[inout] ptr Data to be byte-swapped
 * @param[inout] length Size of data to be modified.
 * @return H2_OK
 */
H2status crypto_swapBytes(H2uint8 *ptr, int length)
{
	int ii;

	H2status status = H2_ERROR;

	do {
		if (length <= 2) {
			status = H2_ERROR;
			break;
		}
	for (ii = 0; ii < length / 2; ii++) {
		char c = ptr[ii];
		ptr[ii] = ptr[length - ii - 1];
		ptr[length - ii - 1] = c;
	}

	status = H2_OK;

	} while (0);

	return status;
}



/**
 * Decrypt EKpubKm using AES CTR decrypt with kPrivRx as AES key
 *
 * @param[in] KprivRx Private key used for decryption in p,q,dP,dQ,qInv form
 * @param[out] km Decrypted km. Should be MASTERKEY_SIZE bytes
 * @param[in] EKpubKm Encrypted Km to decrypt. Should be sent plaintext from TX
 * @return H2_OK or H2_ERROR
 */
H2status Decrypt_EKpubKm_kPrivRx(const H2uint8 *KprivRx, H2uint8 *km, const H2uint8 *EKpubKm)
{
	H2status rc = H2_ERROR;

	int kmSize = MASTERKEY_SIZE;

	do {
		crypto_RSAKey key;


      /**
       * BIGDITS buffers for private key.
       * MOD_SIZE = 32. DIGIT_T = 32 bit, 32*32 = 1024 bit total.
       *
       */
		DIGIT_T keyP[MOD_SIZE];
		DIGIT_T keyQ[MOD_SIZE];
		DIGIT_T keydP[MOD_SIZE];
		DIGIT_T keydQ[MOD_SIZE];
		DIGIT_T keyQinv[MOD_SIZE];
		key.p = keyP;
		key.q = keyQ;
		key.dP = keydP;
		key.dQ = keydQ;
		key.Qinv = keyQinv;

		if (KprivRx == NULL || km == NULL || EKpubKm == NULL) {
			H2DBGLOG((WARN, "ERROR! Decrypt_EKpubKm_kPrivRx: Invaid Input parameters"));
			break;
		}

      /**
       * Initialize data for the key - P, Q, dP, dQ and qInv
       */
		mpConvFromOctets(key.p, MOD_SIZE, KprivRx, KPUBRX_P);
		mpConvFromOctets(key.q, MOD_SIZE, KprivRx+KPUBRX_P, KPUBRX_Q);
		mpConvFromOctets(key.dP, MOD_SIZE, KprivRx+KPUBRX_P+KPUBRX_Q, KPUBRX_dP);
		mpConvFromOctets(key.dQ, MOD_SIZE, KprivRx+KPUBRX_P+KPUBRX_Q+KPUBRX_dP, KPUBRX_dQ);
		mpConvFromOctets(key.Qinv, MOD_SIZE, KprivRx+KPUBRX_P+KPUBRX_Q+KPUBRX_dP+KPUBRX_dQ, KPUBRX_qInv);

      /**
       * Perform decrypt. Return value is the size of decrypted data.
       */

		kmSize = crypto_rsaPrivateDecryptOaepSha256(EKpubKm, km, &key);

      /**
       * Check return size!
       */
		if (kmSize != MASTERKEY_SIZE) {
			rc = H2_ERROR;
		} else {
			rc = H2_OK;
		}
	} while (0);
	return rc;
}



/**
 * Compute Kh = HMAC-SHA256( Kpriv_Rx ).
 *
 * @param[in] Kpriv_rx Private key
 * @param[in] Kprivrx_len Size of KprivRx.
 * @param[out] Kh HMAC-SHA256( Kpriv_Rx )
 * @return H2_OK on success.
 *
 */
H2status Compute_Kh (const H2uint8 *Kpriv_rx, int Kprivrx_len, H2uint8 *Kh)
{
	H2status rc = H2_ERROR;

	H2Sha256Ctx_t c;
	unsigned char tmp[128];

	do {
		H2DBGLOG((LOCALDBG, "HDCP-Sink(Rx): Compute_Kh\n"));

		if (Kpriv_rx == NULL || Kh == NULL) {
			H2DBGLOG((WARN, "ERROR! Compute_Kh: Invalid Input parameters"));
			break;
		}

		crypto_sha256Init(&c);
		crypto_sha256Update(&c, Kpriv_rx, KPRIVRX_SIZE);
		crypto_sha256Final(&c, tmp);
		memcpy(Kh, &tmp[16], KH_SIZE);
		spu_SetKH(Kh);
		rc = H2_OK;

	} while (0);

	return rc;
}



/**
 * Compute Lprime = HMAC-SHA256(rn, kd XOR rRx).
 *
 * @param[in] Kd - Input to XOR.
 * @param[in] rRx - Second input to XOR.
 * @param[in] rn - rn value to use.
 * @param[out] L - H_SIZE bytes. HMAC-SHA256(rn, kd XOR rRx ).
 * @return H2_OK on success.
 *
 */

H2status Compute_Lprime_Rx(const H2uint8 *Kd, const H2uint8 *rRx, const H2uint8 *rn, H2uint8 *L)
{
	H2status rc = H2_ERROR;
	static H2uint8 pKey[KD_SIZE];
	static H2uint8 temp[KD_SIZE];

	H2DBGLOG((LOCALDBG, "Compute_Lprime: Lprime = HMAC-SHA256(rn, kd XOR rRx) \n"));
	do {
		if ((Kd == NULL) || (rRx == NULL) || (rn == NULL) || (L == NULL)) {
			H2DBGLOG((WARN, "ERROR! Compute_Lprime: Invalid Input Parameter \n"));
			break;
		}

		memset(pKey, 0, sizeof(pKey));
		memset(temp, 0, sizeof(temp));

	      /** Copy rRx to the end of temp */
		memcpy(temp + KD_SIZE - RRX_SIZE, rRx, RRX_SIZE);

	      /** pKey = Kd | ( 0 || rRx ) */
		crypt_xor(Kd, temp, pKey, KD_SIZE);

		rc = hmacsha256(pKey, KD_SIZE, rn , RN_SIZE, L);

	} while (0);

	return rc;
}


/**
 * RSA private key decrypt a memory buffer. PKCS#1 1.5 padding is used.
 * The key size is 1024 bits so all buffers are to be this length.
 *
 * @param[in]  pIn   Pointer to memory buffer to decrypt.
 * @param[out] pOut  Pointer to clear text buffer.
 *
 * Notes: Only a NULL label is supported. This function is non-reentrant.
 *
 * @retval Size of decrypted buffer. Zero if decryption failed.
 *
 */

static int crypto_rsaPrivateDecryptOaepSha256(H2uint8 const *pIn, H2uint8 *pOut, const crypto_RSAKey *key)
{
	int rc = 0;
	int bad = 0;
   /** The below values are static to move them off of the stack. */
	static DIGIT_T c[MOD_SIZE];
	static DIGIT_T m[MOD_SIZE];
	static DIGIT_T m1[MOD_SIZE];
	static DIGIT_T m2[MOD_SIZE];
	static DIGIT_T h[MOD_SIZE];
	static DIGIT_T hq[2*MOD_SIZE];       /* Multiply requires 2x size of it's inputs */
	static H2uint8 seedMask[SHA256_DIGEST_SIZE];
	static H2uint8 seed[SHA256_DIGEST_SIZE];
	static H2uint8 dbMask[1024/8];
	static H2uint8 db[128-SHA256_DIGEST_SIZE-1];
	static H2uint8 pTemp[1024/8];
	static H2uint8 *ptr;
	static const H2uint8 sha256NullHash[SHA256_DIGEST_SIZE] = { 0xe3, 0xb0, 0xc4, 0x42,
		0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f,
		0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
		0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 };
	H2uint8 *maskedSeed;
	H2uint8 *maskedDB;

   /** Copy */
	mpConvFromOctets(c, MOD_SIZE, pIn, 1024/8);
	H2DBGLOG((LOCALDBG, "C=\r\n"));
	mpPrint(c, MOD_SIZE);
	H2DBGLOG((LOCALDBG, "\r\n"));
   /** TODO: Add Blinding to avoid giving away hints based on decrypt time. */

   /** Decrypt using Chinese Remainder Theorem */
   /** Let m_1 = c^dP mod p. */
	mpModExp(m1, c, key->dP, key->p, MOD_SIZE);

   /** Let m_2 = c^dQ mod q. */
	mpModExp(m2, c, key->dQ, key->q, MOD_SIZE);

	if (mpCompare(m1, m2, MOD_SIZE) < 0) {
		mpAdd(m1, m1, key->p, MOD_SIZE);
	}

	mpSubtract(m1, m1, m2, MOD_SIZE);

	/** Let h - qInv( m_1 - m_2 ) mod p */
	mpModMult(h, key->Qinv, m1, key->p, MOD_SIZE);
	mpMultiply(hq, h, key->q, MOD_SIZE);

   /** Let m = m_2 + hq */
	mpAdd(m, m2, hq, MOD_SIZE);

   /** Copy the output to pTemp */
	mpConvToOctets(m, MOD_SIZE, pTemp, sizeof(pTemp));
	H2DBGLOG((LOCALDBG, "RSA Decrypt: OAEP Start\r\n"));

   /**
    * pTemp still has OAEP-SHA256 padding.
    *
    * pTemp is:
    * Y || maskedSeed || maskedDB
    * Y is one octet, and should be zero.
    * maskedSeed is hLen ( SHA256_DIGEST_SIZE) bytes.
    * maskedDB is k-hLen-1 bytes
    */

   /** split it up */
	maskedSeed = pTemp+1;
	maskedDB = pTemp+1+SHA256_DIGEST_SIZE;

   /** Do not exit on error conditions, or reveal what the errors were,
    * to avoid leaking timing information.
    */

	if (*pTemp != 0) {
		bad = 1;
	}

   /** Reverse OAEP-SHA256 */
   /** seedMask = MGF( maskedDB, hLen ) */
	MGF1_SHA256(seedMask, SHA256_DIGEST_SIZE, maskedDB, 1024 / 8 - SHA256_DIGEST_SIZE - 1);

   /** seed = maskedSeed xor seedMask */
	crypt_xor(maskedSeed, seedMask, seed, SHA256_DIGEST_SIZE);

   /** dbMask = MGF( seed, k-hLen-1 ) */
	MGF1_SHA256(dbMask, 1024/8-SHA256_DIGEST_SIZE-1, seed, SHA256_DIGEST_SIZE);

   /** DB = maskedDB XOR dbMask */
	crypt_xor(dbMask, maskedDB, db, 1024 / 8 - SHA256_DIGEST_SIZE - 1);

   /**
    * DB should contain:
    * lHash' || PS || 0x01 || M
    *
    * Where:
    * lhash' = SHA256(Label), which should be SHA256( NULL, 0 )
    * PS is a (possibly empty) padding string of all zeroes
    * 0x01 immediately precedes the data
    * M is the message
    */


   /* Verify the hash */
	if (memcmp(db, sha256NullHash, SHA256_DIGEST_SIZE) != 0) {
		int ii;
		H2DBGLOG((LOCALDBG, "SHA256 fail.\n"));
		H2DBGLOG((LOCALDBG, "NULL HASH\n"));
		for (ii = 0; ii < SHA256_DIGEST_SIZE; ii++) {
			H2DBGLOG((LOCALDBG, "0x%x ", sha256NullHash[ii]));
		}
		H2DBGLOG((LOCALDBG, "\nCOMPUTED HASH\n"));
		for (ii = 0; ii < SHA256_DIGEST_SIZE; ii++) {
			H2DBGLOG((LOCALDBG, "0x%x ", db[ii]));
		}
		H2DBGLOG((LOCALDBG, "\n"));

		bad = 1;
	}

	ptr = db + SHA256_DIGEST_SIZE;
	do {
      /** Search for the first non-zero octet, and make sure it's 0x01.
       * Since PS is possibly empty, we do not check for zero until AFTER
       * checking for 0x01.
       */
		if (ptr[0] != 0x00) {
			if (ptr[0] != 0x01) {
/** For security purposes, we cannot reveal which part of
* the decryption has failed.
*
* However, if LOCALDBG is set, the keys and ciphertext are
* printed out, so it is not an issue.
*
*/
				H2DBGLOG((LOCALDBG, "Found non-zero that wasn't 0x01 fail.\n"));
				bad = 1;
			}
    /** Found it! First non-zero octet was 0x01 */
			break;
		}
		ptr++;
	} while (ptr < db + sizeof(db));

	if (ptr >= db + sizeof(db)) {
		bad = 1;
	} else {
		H2DBGLOG((LOCALDBG, "SUCCESS! %d - bad=%d\n", ptr - db, bad));
		ptr++;
      /** Copy the results */
		memcpy(pOut, ptr, db + sizeof(db) - (ptr));
		rc = db + sizeof(db) - (ptr);
	}
	if (bad) {
		rc = 0;
	}
	H2DBGLOG((LOCALDBG, "Rc=%d", rc));
	return rc;

}

/**
 *
 * Compute HMAC-SHA256( Kd, rtx ^ Repeater ).
 *
 * @param[in] Kd
 * @param[in] rtx
 * @param[in] Repeater
 * @param[out] Hprime At least H_SIZE bytes for the results.
 *
 * @return H2_OK on success.
 */

H2status Compute_Hprime_Rx(const H2uint8 *Kd, const H2uint8 *rtx, const H2uint8 Repeater, H2uint8 *Hprime)
{
	H2status rc = H2_ERROR;
	H2uint8 pSrc[RTX_SIZE];

	H2DBGLOG((LOCALDBG, "HDCP-Sink(Rx): Compute_Hprime\n"));

	do {
		if (Kd == NULL || rtx == NULL || Hprime == NULL) {
			H2DBGLOG((WARN, "ERROR! Compute_Hprime: Invalid Input Paramter \n"));
			break;
		}

	      /** pSrc = rtx XOR REPEATER */
		memset(pSrc, 0, sizeof(pSrc));
		memcpy(pSrc, rtx, sizeof(pSrc));
		pSrc[RTX_SIZE - 1] = rtx[RTX_SIZE - 1] ^ Repeater;

	      /** Compute H' = HMAC-SHA256( Kd, RTX ^ Repeater ) */
		rc = hmacsha256(Kd, KD_SIZE, pSrc, sizeof(pSrc),  Hprime);
	} while (0);

	return rc;
}

#if 1
H2status Compute_Mprime_22(unsigned char *kd, unsigned char *bstream_id_type,unsigned char *bseq_num_M, unsigned char *Mprime)
{
	H2status rc = H2_ERROR;

	unsigned char psrc[ (Stream_Type_SIZE)+ seq_num_M_SIZE];
	unsigned char sha256_kd[32]={0};

	if (bstream_id_type == NULL || bseq_num_M == NULL ||kd ==NULL )
		return H2_ERROR;
       Sha256(kd,KD_SIZE,sha256_kd);
	memcpy(psrc, bstream_id_type, (Stream_Type_SIZE));
	memcpy(psrc + (Stream_Type_SIZE), bseq_num_M, seq_num_M_SIZE);
	rc = hmacsha256(sha256_kd, KD_SIZE, psrc, (Stream_Type_SIZE)+ seq_num_M_SIZE, Mprime);


	return rc;
}


H2status Compute_Vprime_22(unsigned char *kd, unsigned char *receiver_id, unsigned char *temp_rxinfor, unsigned char *temp_seq_num, unsigned char *Vprime)
{
	H2status rc = H2_ERROR;
	unsigned char rec_ID_LEN;
	unsigned char psrc[ (RECEIVERID_SIZE*MAX_DEVICECOUNT)+ RXinfo_SIZE + Seq_num_V_SIZE];

	if (receiver_id == NULL || temp_rxinfor == NULL || temp_seq_num == NULL)
		return H2_ERROR;

        rec_ID_LEN = ((((temp_rxinfor[0]&0x1)<<4)|((temp_rxinfor[1]&0xF0)>>4))*RECEIVERID_SIZE);

	memcpy(psrc, receiver_id, (rec_ID_LEN));
	memcpy(psrc + (rec_ID_LEN), temp_rxinfor, RXinfo_SIZE);
	memcpy(psrc + (rec_ID_LEN) + RXinfo_SIZE, temp_seq_num, Seq_num_V_SIZE);
	rc = hmacsha256(kd, KD_SIZE, psrc, (rec_ID_LEN+RXinfo_SIZE+Seq_num_V_SIZE), Vprime);


	return rc;
}

#endif
H2status Compute_Hprime_22(unsigned char *kd, unsigned char *rtx, unsigned char *txcaps, unsigned char *rxcaps, unsigned char *Hprime)
{
	H2status rc = H2_ERROR;
	unsigned char psrc[RTX_SIZE + RXCAPS_SIZE + TXCAPS_SIZE];

#if 0
	unsigned char psrc_test[RTX_SIZE+RXCAPS_SIZE+TXCAPS_SIZE] = {0x18, 0xfa, 0xe4, 0x20, 0x6a, 0xfb, 0x51, 0x49, 0x02, 0x00, 0x01, 0x02, 0x00, 0x00};
	unsigned char kd_test[KD_SIZE] = {0x4f, 0x14, 0x8d, 0x11, 0xdd, 0x49, 0x18, 0x10, 0x6f, 0xab, 0x16, 0x6f, 0xf6, 0xfd, 0xa6, 0xed,
		0xb5, 0x02, 0x0c, 0x0d, 0xf2, 0x81, 0xba, 0xdf, 0xe4, 0x19, 0x77, 0xfa, 0xd0, 0xac, 0x61, 0x17};
#endif

	#ifdef HDCP2_2_WIFI
	hdcp2_Rx_GetRxInfo(rxcaps);
	hdcp2_Rx_GetTxInfo(txcaps);
	#else
	/*hdcp2_Rx_GetRxcaps(rxcaps, RXCAPS_SIZE);
	hdcp2_Rx_GetTxcaps(txcaps, TXCAPS_SIZE);*/
	#endif

	if (rtx == NULL || rxcaps == NULL || txcaps == NULL)
		return H2_ERROR;

	#ifdef HDCP2_2_WIFI
	if (hdcp2_Rx_IsProDesp() == FALSE) {
		unsigned char psrc1[RTX_SIZE];
		memcpy(psrc1, rtx, RTX_SIZE);
		psrc1[RTX_SIZE - 1] ^= hdcp2_Rx_IsRepeater();
		rc = hmacsha256(kd, KD_SIZE, psrc1, sizeof(psrc1), Hprime);
	} else {
		unsigned char psrc1[RTX_SIZE + RXCAPS_SIZE + TXCAPS_SIZE];
		memcpy(psrc1, rtx, RTX_SIZE);
		psrc1[RTX_SIZE - 1] ^= hdcp2_Rx_IsRepeater();
		memcpy(psrc1 + RTX_SIZE, rxcaps, RXCAPS_SIZE);
		memcpy(psrc1 + RTX_SIZE+RXCAPS_SIZE, txcaps, TXCAPS_SIZE);
		rc = hmacsha256(kd, KD_SIZE, psrc1, sizeof(psrc1), Hprime);
	}
	/*rc= hmacsha256(kd, KD_SIZE, psrc1, sizeof(psrc1), Hprime);*/

	#else
	memcpy(psrc, rtx, RTX_SIZE);
	memcpy(psrc + RTX_SIZE, rxcaps, RXCAPS_SIZE);
	memcpy(psrc + RTX_SIZE + RXCAPS_SIZE, txcaps, TXCAPS_SIZE);
	#if 0
	memcpy(kd, kd_test, KD_SIZE);
	memcpy(psrc, psrc_test, 14);
	printk("test text:\n");
	spu_print(psrc, 14);
	printk("test kd:\n");
	spu_print(kd, KD_SIZE);
	#endif
	rc = hmacsha256(kd, KD_SIZE, psrc, sizeof(psrc), Hprime);

	#endif

	return rc;
}

H2status Sha256(const H2uint8 *pIn, int length, H2uint8 *pOut)
{
	H2status rc = H2_ERROR;

	H2Sha256Ctx_t c;

	do {
		if (pIn == NULL || pOut == NULL) {
			H2DBGLOG((WARN, "ERROR! Compute_Kh: Invalid Input parameters"));
			break;
		}

		crypto_sha256Init(&c);
		crypto_sha256Update(&c, pIn, length);
		crypto_sha256Final(&c, pOut);

		rc = H2_OK;

	} while (0);

	return rc;
}

H2status Sha1(const H2uint8 *pIn, int length, H2uint8 *pOut)
{
	H2status rc = H2_ERROR;

	struct SHA1Context c;

	do {
		if (pIn == NULL || pOut == NULL) {
			H2DBGLOG((WARN, "ERROR! Compute_Kh: Invalid Input parameters"));
			break;
		}

		crypto_sha1Init(&c);
		crypto_sha1Update(&c, pIn, length);
		crypto_sha1Final(pOut, &c);

		rc = H2_OK;

	} while (0);

	return rc;
}

#endif

