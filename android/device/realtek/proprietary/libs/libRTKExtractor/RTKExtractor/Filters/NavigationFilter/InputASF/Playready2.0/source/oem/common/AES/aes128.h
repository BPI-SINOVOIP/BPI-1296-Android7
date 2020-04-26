/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
** Module Name: aes128.h
**
** Abstract:
**    This module contains the public data structures and API definitions
**  needed to utilize the low-level AES encryption routines. 
**  
**  Routines are implemented in aes128.c
*/


#ifndef __AES128_H__
#define __AES128_H__

#include <drmcommon.h>
#include <oemaescommon.h>
#include <oemaesimpl.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_NO_INLINE DRM_VOID Aes_CreateKey128(
    __out_ecount( 1 )                         DRM_AESTable *f_pKeyTable,
    __in_bcount( DRM_AES_KEYSIZE_128 )  const DRM_BYTE      f_rgbKey[__CB_DECL( DRM_AES_KEYSIZE_128 )] );

DRM_API DRM_NO_INLINE DRM_VOID Aes_Encrypt128( 
    __out_bcount( DRM_AES_KEYSIZE_128 )       DRM_BYTE   f_rgbEncrypted[__CB_DECL( DRM_AES_KEYSIZE_128 )],
    __in_bcount( DRM_AES_KEYSIZE_128 )  const DRM_BYTE   f_rgbClear[__CB_DECL( DRM_AES_KEYSIZE_128 )],
    __in_ecount( DRM_AES_ROUNDS_128+1 ) const DRM_DWORD  f_rgbKeys[DRM_AES_ROUNDS_128+1][4] );

DRM_API DRM_NO_INLINE DRM_VOID Aes_Decrypt128( 
    __out_bcount( DRM_AES_KEYSIZE_128 )       DRM_BYTE   f_rgbClear[__CB_DECL( DRM_AES_KEYSIZE_128 )],
    __in_bcount( DRM_AES_KEYSIZE_128 )  const DRM_BYTE   f_rgbEncrypted[__CB_DECL( DRM_AES_KEYSIZE_128 )],
    __in_ecount( DRM_AES_ROUNDS_128+1 ) const DRM_DWORD  f_rgbKeys[DRM_AES_ROUNDS_128+1][4] ) DRM_NO_INLINE_ATTRIBUTE;


EXIT_PK_NAMESPACE;

#endif /* __AES128_H__ */

