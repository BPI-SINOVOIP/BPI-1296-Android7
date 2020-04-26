/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCFDTYPES_H__
#define __DRMCFDTYPES_H__

#include <drmtypes.h>

ENTER_PK_NAMESPACE;

typedef struct __tagDRM_SYMDATA
{
    DRM_SLK    SLK;
    DRM_BYTE   rgbEncryptedSLK[ __CB_DECL( DRM_ECC160_CIPHERTEXT_LEN ) ];
    DRM_ID     idSLK;
} DRM_SYMDATA;



typedef enum
{
    DRM_SIMPLE_LICENSE = 1,
    DRM_ROOT_LICENSE,
    DRM_LEAF_LICENSE
} DRM_LICENSE_TYPE;


EXIT_PK_NAMESPACE;

#endif /* __DRMCFDTYPES_H__ */

