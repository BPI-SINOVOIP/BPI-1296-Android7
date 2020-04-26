
#include <linux/kernel.h>
#include <linux/random.h>
#include <hdcp2_hal.h>
#include <crypto.h>

/**
 * Standard include files
 */

/**
 * This one gets htonl() and ntohl(), etc.
 */

/**
 * For openssl SHA-256 and AES
 */
#include <rcp_api.h>
#include <sha2.h>

#define CRYPTO_AES_KEY_LEN (16)

/**
 * @warn Make sure LOCALDBG is set to DISABLE for production
 */

#ifdef DEBUG_CRYPTO_SPU
#define LOCALDBG ENABLE
#else	/*  */
#define LOCALDBG DISABLE
#endif	/*  */

#define TRACE() H2DBGLOG((LOCALDBG, "File: %s, Line %d\n", __FILE__, __LINE__))

#if 1
#define INFO ENABLE
#else	/*  */
#define INFO DISABLE
#endif	/*  */

unsigned int crypto_random32()
{
	unsigned int rand;
	get_random_bytes(&rand, sizeof(unsigned int));
	return rand;
}

H2bool crypto_sha256Init(H2Sha256Ctx_t *pCtx)
{
	SHA256_Init(&pCtx->c);
	return TRUE;
}

/**
 * Calculate the SHA-256 hash of the supplied memory buffer. This
 * function can be called multiple times to extend the hash
 * calculation accross multiple buffers. To get the hash, the final
 * function must be called to indicate no more buffers will be supplied.
 *
 * @param[in]  pCtx  Pointer to SHA-256 context. The contents to the
 *                   context are defined in hdcp2_hal.h
 * @param[in]  pBuf  Pointer to memory buffer to calculate hash on.
 * @param[in]  len   Length in bytes of the memory buffer.
 *
 * @retval  True if successful, False if an error occurred.
 *
 */
H2bool crypto_sha256Update(H2Sha256Ctx_t *pCtx, H2uint8 const *pBuf, H2uint32 len)
{
	SHA256_Update(&pCtx->c, pBuf, len);
	return TRUE;
}

/**
 * Finalize the SHA-256 hash calculation and retrieve the hash. This
 * function also frees any resources associated with the supplied
 * context. The context must be initialized again before reusing it.
 *
 * @param[in]  pCtx  Pointer to SHA-256 context. The contents to the
 *                   context are defined in hdcp2_hal.h
 * @param[out] pHash Pointer to buffer to copy hash into. The buffer must
 *                   be SHA256_DIGEST_SIZE bytes long.
 *
 * @retval  True if successful, False if an error occurred.
 *
 */
H2bool crypto_sha256Final(H2Sha256Ctx_t *pCtx, H2uint8 *pHash)
{
	SHA256_Final(pHash, &pCtx->c);
	return TRUE;
}

H2bool crypto_sha1Init(SHA1_CTX *context)
{
	SHA1Init(context);
	return TRUE;
}

H2bool crypto_sha1Update(SHA1_CTX *context, const void *data, u32 len)
{
	SHA1Update(context, data, len);
	return TRUE;
}

H2bool crypto_sha1Final(unsigned char digest[20], SHA1_CTX *context)
{
	SHA1Final(digest, context);
	return TRUE;
}

/**
 * Encrypt/Decrypt a memory buffer using AES-CTR-128.
 *
 * @param[in]  gaIn  Pointer to cipher text.
 * @param[in]  len   Length in bytes of cipher text. This
 *                   must be a multiple of 16 bytes.
 * @param[in]  pSK   Pointer to 16 byte AES Key.
 * @param[in]  pIV   Pointer to 16 byte AES IV.
 * @param[out] pOut  Pointer to plain text buffer. This must be
 *                   the same length as pIn.
 *
 * @return H2_OK or H2_ERROR.
 *
 */
void dump_array(unsigned char *d, unsigned long len)
{
	unsigned long i;
	for (i = 0; i < len; i++) {
		if ((i & 0xF) == 0)
			pr_debug("\n");
		pr_debug("%02x ", d[i]);
	}
}

long _memcmp(unsigned char *in, unsigned char *in2, unsigned long len)
{
	while (len-- && (*(in++) == *(in2++)))
		;
	return len;
}

