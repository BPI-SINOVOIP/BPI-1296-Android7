/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
** Module Name: aesbox.h
**
** Abstract:
**    This module contains the read-only table state for AES.
**
*/

#ifndef __AESBOX_H__
#define __AESBOX_H__

#include <drmcommon.h>

ENTER_PK_NAMESPACE;

/* 
** NOTE: 
** T and U tables will be accessed at DWORD boundaries, and
** therefore must be DWORD aligned.
*/

extern const DRM_BYTE S[__CB_DECL( 256 )];

extern const DRM_DWORD_ALIGN DRM_BYTE T1[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T2[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T3[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T4[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T5[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T6[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T7[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE T8[256][__CB_DECL( 4 )];

extern const DRM_BYTE S5[__CB_DECL( 256 )];

extern const DRM_DWORD_ALIGN DRM_BYTE U1[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE U2[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE U3[256][__CB_DECL( 4 )];
extern const DRM_DWORD_ALIGN DRM_BYTE U4[256][__CB_DECL( 4 )];

extern const DRM_DWORD rcon[30];

EXIT_PK_NAMESPACE;

#endif /* __AESBOX_H__ */

