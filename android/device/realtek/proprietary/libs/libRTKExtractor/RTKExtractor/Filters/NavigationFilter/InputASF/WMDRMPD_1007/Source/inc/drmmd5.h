/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMD5_H__
#define __DRMMD5_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define MD5DIGESTLEN 16

typedef struct __tagDRM_MD5_CTX {
    DRM_DWORD awaiting_data[16];
                             /* Data awaiting full 512-bit block.       */
                             /* Length (nbit_total[0] % 512) bits.      */
                             /* Unused part of buffer (at end) is zero. */
    DRM_DWORD partial_hash[4];
                             /* Hash through last full block            */
    DRM_DWORD nbit_total[2];       
                             /* Total length of message so far          */
                             /* (bits, mod 2^64)                        */
    DRM_BYTE digest[__CB_DECL(MD5DIGESTLEN)];
                             /* Actual digest after MD5Final completes  */
} DRM_MD5_CTX;

DRM_VOID DRM_API DRM_MD5_Init( IN OUT DRM_MD5_CTX *f_contextMD5);

DRM_VOID DRM_API DRM_MD5_Update(
    IN OUT   DRM_MD5_CTX *f_contextMD5, 
    IN const DRM_BYTE    *f_pbData, 
    IN       DRM_DWORD    f_cbData);

DRM_VOID DRM_API DRM_MD5_Final( IN OUT DRM_MD5_CTX *f_contextMD5);

#ifdef __cplusplus
}
#endif


#endif /* __DRMMD5_H__ */
