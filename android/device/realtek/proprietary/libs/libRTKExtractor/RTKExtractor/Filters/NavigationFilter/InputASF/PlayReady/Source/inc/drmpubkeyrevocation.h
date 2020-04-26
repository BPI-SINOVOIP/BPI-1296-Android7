/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_PUBKEY_REVOCATION__
#define __DRM_PUBKEY_REVOCATION__

ENTER_PK_NAMESPACE;

PUBKEY g_pubkeyRevocation = 
{
    TWO_BYTES(0x02, 0x14), TWO_BYTES(0xEF, 0x99), TWO_BYTES(0xD4, 0x80), TWO_BYTES(0x0C, 0x96), 
    TWO_BYTES(0x5E, 0xB4), TWO_BYTES(0x57, 0x5C), TWO_BYTES(0xE7, 0x16), TWO_BYTES(0x67, 0x60), 
    TWO_BYTES(0x40, 0x4E), TWO_BYTES(0x6D, 0x23), TWO_BYTES(0x30, 0x42), TWO_BYTES(0xBB, 0xBA), 
    TWO_BYTES(0x68, 0x35), TWO_BYTES(0x42, 0x30), TWO_BYTES(0xA1, 0xB6), TWO_BYTES(0x10, 0x95), 
    TWO_BYTES(0x4C, 0x40), TWO_BYTES(0xF9, 0xAA), TWO_BYTES(0x10, 0x85), TWO_BYTES(0xE5, 0x70)
};

EXIT_PK_NAMESPACE;

#endif  /* __DRM_PUBKEY_REVOCATION__ */
