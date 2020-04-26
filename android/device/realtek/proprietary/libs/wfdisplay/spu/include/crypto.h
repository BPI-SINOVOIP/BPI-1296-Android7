/**
 * Crypto interface for HDCP2 XTASK.
 *
 * This file contains function prototypes used for HDCP2.
 *
 * @author Buddy Smith <bsmith@vtilt.com>
 */

#define SHA256_DIGEST_SIZE (256/8)

H2status Decrypt_EKpubKm_kPrivRx( const H2uint8* KprivRx, H2uint8* km, const H2uint8 *EKpubKm);
H2status Decrypt_EKhKm (const H2uint8 *Kh, const H2uint8* eKhKm, int peKhKmSize, H2uint8* Km, const H2uint8 *m);
H2status Compute_EKhKm (const H2uint8 *Kh,const H2uint8* Km,const H2uint8 *m, H2uint8* eKhKm);

H2status Compute_Kd( const H2uint8 *masterKey, const H2uint8 *rn, const H2uint8 *rtx, H2uint8 *Kd);
H2status Compute_dKey(const H2uint8 *Km, const H2uint8 *rn, const H2uint8 *rtx, const H2uint8 * Ctr, H2uint8* dKey, int dKeySize);

H2status Compute_Hprime( const H2uint8 *Kd, const H2uint8 *rtx, const H2uint8 Repeater, H2uint8 *hPrime);
// new added after HDCP 2.2
H2status Compute_Hprime_22(
		const H2uint8 *Kd,
		const H2uint8 *rtx,		// 8
		const H2uint8 Repeater,
		const H2uint8 ReceiverVersion,
		const H2uint8 *ReceiverMask,		// we believe that it is treated as big endian data in previous stage
		const H2uint8 TransmitterVersion,
		const H2uint8 *TransmitterMask,		// we believe that it is treated as big endian data in previous stage
		H2uint8 * Hprime);

H2status Compute_Kh (const H2uint8 *Kpriv_rx, int Kprivrx_len, H2uint8* Kh);
H2status Compute_Eks(const H2uint8* Ks, const H2uint8* dKey3, const H2uint8* rRx, H2uint8* Edkey_Ks);
H2status Compute_Lprime(const H2uint8 * Kd, const H2uint8 *rRx, const H2uint8 *rn, H2uint8 * L, int bUseRnRn);

/** Helper functions */
H2status crypto_swapBytes( H2uint8 *ptr, int length );
H2status xor( const H2uint8 *s1, const H2uint8 *s2, H2uint8 *d, int len );

/** 
 * Implementation specific functions 
 *
 **/

/** struct H2Sha256Ctx is defined in hdcp2_hal.h **/
typedef struct H2Sha256Ctx H2Sha256Ctx_t; 

H2bool crypto_sha256Init( H2Sha256Ctx_t* pCtx );
H2bool crypto_sha256Update( H2Sha256Ctx_t* pCtx, H2uint8 const * pBuf, H2uint32 len );
H2bool crypto_sha256Final( H2Sha256Ctx_t* pCtx, H2uint8* pHash );

H2uint32 crypto_random32( void );

H2status crypto_aesCtr128(H2uint8 const *pIn, int len, H2uint8 const * pSK, H2uint8 const * pIV, H2uint8* pOut );

void crypto_DCacheFlush( void* pAddr, int size );


