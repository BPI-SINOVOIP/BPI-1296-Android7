/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
**  drmaescommon.h
**
**  Contains common definitions used by OEM AES implementations as well as
**  AES API functions.
**  
*/


#ifndef __DRMAESCOMMON_H__
#define __DRMAESCOMMON_H__

#include <drmcommon.h>

ENTER_PK_NAMESPACE;

/*
** AES Key Sizes
*/
#define DRM_AES_KEYSIZE_128 ( 16 ) /* Size ( in bytes ) of a 128 bit key */
#define DRM_AES_KEYSIZE_192 ( 24 ) /* Size ( in bytes ) of a 192 bit key */
#define DRM_AES_KEYSIZE_256 ( 32 ) /* Size ( in bytes ) of a 256 bit key */
#define DRM_AES_MAXKEYSIZE  DRM_AES_KEYSIZE_256

/*
** Round counts corresponding to key sizes
*/
#define DRM_AES_ROUNDS_128  ( 10 ) 
#define DRM_AES_ROUNDS_192  ( 12 )
#define DRM_AES_ROUNDS_256  ( 14 )
#define DRM_AES_MAXROUNDS   DRM_AES_ROUNDS_256

#define DRM_AES_BLOCKLEN    DRM_AES_KEYSIZE_128 /* Size ( in bytes ) of a single encrypted/decrypted block */
#define DRM_AES_ROUNDS      DRM_AES_ROUNDS_128 

EXIT_PK_NAMESPACE;

#endif /* __DRMAESCOMMON_H__ */

