/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef ECAPI_H
#define ECAPI_H 1

#include "bigdefs.h"

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

//  -------------- SHA-1 message digest ------------------


#define A_SHA_DIGEST_LEN 20
        /* Number of bytes output by SHA-1 */



typedef struct {
        DWORD magic_sha1;    // Magic value for A_SHA_CTX
        DWORD awaiting_data[16];
                             // Data awaiting full 512-bit block.
                             // Length (nbit_total[0] % 512) bits.
                             // Unused part of buffer (at end) is zero
        DWORD partial_hash[5];
                             // Hash through last full block
        DWORD nbit_total[2];
                             // Total length of message so far
                             // (bits, mod 2^64)
} A_SHA_CTX;

extern BOOL DRM_API A_SHAInit(A_SHA_CTX *ctx);
extern BOOL DRM_API A_SHAUpdate(A_SHA_CTX*, const BYTE*, DWORDREGC);
extern BOOL DRM_API A_SHAFinal(A_SHA_CTX*, BYTE digest[A_SHA_DIGEST_LEN]);
// Next is new
extern BOOL DRM_API A_SHA(const BYTE *msg, DWORDREGC msglen,
                         BYTE digest[A_SHA_DIGEST_LEN]);

//  -------------- MD5 message digest --------------------

#define MD5DIGESTLEN 16
#define MD5_LEN MD5DIGESTLEN
        /* Two names for number of bytes output by MD5 */

typedef struct {
        DWORD magic_md5;     // Magic value for MD5_CTX
        DWORD awaiting_data[16];
                             // Data awaiting full 512-bit block.
                             // Length (nbit_total[0] % 512) bits.
                             // Unused part of buffer (at end) is zero.
        DWORD partial_hash[4];
                             // Hash through last full block
        DWORD nbit_total[2];
                             // Total length of message so far
                             // (bits, mod 2^64)
        BYTE digest[MD5DIGESTLEN];
                             // Actual digest after MD5Final completes
} MD5_CTX;

typedef const MD5_CTX MD5_CTXc;

// N.B.  Following were void in RSA32API.
//       How were errors reported?
extern BOOL DRM_API MD5Init(MD5_CTX*);
extern BOOL DRM_API MD5Update(MD5_CTX*, const BYTE*, DWORDREGC);
extern BOOL DRM_API MD5Final(MD5_CTX*);

// Next is new
extern BOOL DRM_API MD5(const BYTE *msg, DWORDREGC msglen,
                       BYTE digest[MD5DIGESTLEN]);

// ------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif // ECAPI_H
