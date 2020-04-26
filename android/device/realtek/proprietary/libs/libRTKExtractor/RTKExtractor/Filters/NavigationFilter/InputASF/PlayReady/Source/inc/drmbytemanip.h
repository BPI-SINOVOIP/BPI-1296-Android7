/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_BYTEMANIP_H__ 
#define __DRM_BYTEMANIP_H__ 

#include "drmcrt.h"

ENTER_PK_NAMESPACE;

DRM_API DRM_VOID DRM_CALL DRM_BYT_ReverseBytes(
    __inout_bcount(cbData) DRM_BYTE  *pbData,
    IN                     DRM_DWORD  cbData );

#if SIXTEEN_BIT_ADDRESSING

/* the distinction between BYTE and NATIVE_BYTE is an artificial one solely for
   testing under x86 and other environments capable of addressing 8-bit quantities.
   Redefining a BYTE to be a WORD allows simulation of 16-bit addressing 
   
   For practical reasons it serves as a reminder that what looks like a BYTE isn't
   necessarily the familiar 8 bits */

typedef DRM_BYTE DRM_NATIVE_BYTE;
typedef DRM_CHAR DRM_NATIVE_CHAR;

DRM_API DRM_VOID DRM_CALL DRM_BYT_CopyBytes( 
    __out_ecount( f_ibTo + f_cb ) DRM_VOID *f_pbTo, 
    IN       DRM_DWORD f_ibTo, 
    __in_ecount( f_ibFrom + f_cb ) const DRM_VOID *f_pbFrom,
    IN       DRM_DWORD f_ibFrom,
    IN       DRM_DWORD f_cb );

DRM_API DRM_VOID DRM_CALL DRM_BYT_MoveBytes( 
    __out_ecount( f_ibTo + f_cb ) DRM_VOID *f_pbTo, 
    IN       DRM_DWORD f_ibTo, 
    __in_ecount( f_ibFrom + f_cb ) const DRM_VOID *f_pbFrom,
    IN       DRM_DWORD f_ibFrom,
    IN       DRM_DWORD f_cb );

DRM_API DRM_VOID DRM_CALL DRM_BYT_SetBytes(
    __out_ecount( f_ib + f_cb ) DRM_VOID *f_pb,
    IN     DRM_DWORD f_ib,
    IN     DRM_DWORD f_cb,
    IN     DRM_BYTE  f_b );

DRM_API DRM_INT DRM_CALL DRM_BYT_CompareBytes(
    __in_ecount( f_ibA + f_cb ) const DRM_VOID *f_pbA,
    IN       DRM_DWORD f_ibA,
    __in_ecount( f_ibB + f_cb ) const DRM_VOID *f_pbB,
    IN       DRM_DWORD f_ibB,
    IN       DRM_DWORD f_cb );



DRM_API DRM_NATIVE_BYTE DRM_CALL DRM_16B_Read8BitByte(
    __in_ecount( f_ib / CB_NATIVE_BYTE ) const DRM_VOID *f_pv,
    IN        DRM_DWORD f_ib);

DRM_API DRM_NATIVE_CHAR DRM_CALL DRM_16B_Read8BitChar(
    __in_ecount( f_ib / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD f_ich);

DRM_API DRM_VOID DRM_CALL DRM_16B_Write8BitByte(
    __out_ecount( f_ib / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD        f_ib,
    IN  DRM_NATIVE_BYTE  f_b);

DRM_API DRM_VOID DRM_CALL DRM_16B_Write8BitChar(
    __out_ecount( f_ib / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD        f_ich,
    IN  DRM_NATIVE_CHAR  f_ch);

DRM_API DRM_VOID DRM_CALL DRM_16B_Read8BitBytes(
    __in_ecount( f_cb / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD        f_cb,
    __out_ecount( f_cb ) DRM_NATIVE_BYTE *f_pb);

DRM_API DRM_VOID DRM_CALL DRM_16B_Write8BitBytes(
    __out_ecount( f_cb / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD        f_cb,
    __in_ecount( f_cb ) DRM_NATIVE_BYTE *f_pb);

DRM_API DRM_VOID DRM_CALL DRM_16B_Read8BitString(
    __in_ecount( f_cch / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD        f_cch,
    __out_ecount( f_cch ) DRM_NATIVE_CHAR *f_pch);

DRM_API DRM_VOID DRM_CALL DRM_16B_Write8BitString(
    __out_ecount( f_cch / CB_NATIVE_BYTE ) DRM_VOID *f_pv,
    IN  DRM_DWORD        f_cch,
    __in_ecount( f_cch ) DRM_NATIVE_CHAR *f_pch);

#define DRM_PACK_BYTES_IN_PLACE 0x00000001

DRM_API DRM_VOID *DRM_CALL DRM_16B_Pack8BitBytes(
    __inout_ecount( f_cbMachine ) DRM_NATIVE_BYTE *f_pbMachine,
    DRM_DWORD        f_cbMachine,
    __out_ecount_opt( f_cbMachine / CB_NATIVE_BYTE ) DRM_VOID *f_pvPacked,
    DRM_DWORD        f_fOptions);

DRM_API DRM_VOID *DRM_CALL DRM_16B_IncrementPackedPointer(
    __inout_ecount( f_cbMachine / / CB_NATIVE_BYTE ) DRM_NATIVE_BYTE *f_pbMachine,
    IN     DRM_DWORD        f_cbMachine,
       OUT DRM_NATIVE_BYTE *f_pbHead);

DRM_API DRM_VOID *DRM_CALL DRM_16B_DecrementPackedPointer(
    __inout_ecount( f_cbMachine / / CB_NATIVE_BYTE ) DRM_NATIVE_BYTE  *f_pbMachine,
    IN     DRM_DWORD         f_cbMachine,
    IN     DRM_NATIVE_BYTE   f_bHead);

#define GET_BYTE(pb,ib)              DRM_16B_Read8BitByte((pb),(ib))
#define PUT_BYTE(pb,ib,b)            DRM_16B_Write8BitByte((pb),(ib),(b))
#define GET_CHAR(pch,ich)            DRM_16B_Read8BitByte((pch),(ich))
#define PUT_CHAR(pch,ich,ch)         DRM_16B_Write8BitByte((pch),(ich),(ch))

#define MAKE_MACHINE_BYTE(b0,b1)   ((DRM_NATIVE_BYTE)(((b0)<<8)|((b1)&0x00FF)))

#define TWO_BYTES(b0,b1)             MAKE_MACHINE_BYTE(b0,b1)
#define ONE_WCHAR(ch0,ch1)           TWO_BYTES(ch0,ch1)

#else /* 8-bit addressing */


#define DRM_BYT_CopyBytes(to,tooffset,from,fromoffset,count) DRMCRT_memcpy(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))
#define DRM_BYT_MoveBytes(to,tooffset,from,fromoffset,count) DRMCRT_memmove(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))
#define DRM_BYT_SetBytes(pb,ib,cb,b) DRMCRT_memset(&((pb)[(ib)]),b,cb)
#define DRM_BYT_CompareBytes(pbA,ibA,pbB,ibB,cb) DRMCRT_memcmp(&((pbA)[(ibA)]),&((pbB)[(ibB)]),(cb))

#define GET_BYTE(pb,ib)             (pb)[(ib)]
#define PUT_BYTE(pb,ib,b)           (pb)[(ib)]=(b)
#define GET_CHAR(pch,ich)           (pch)[(ich)]
#define PUT_CHAR(pch,ich,ch)        (pch)[(ich)]=(ch)

#if TARGET_LITTLE_ENDIAN
#define MAKE_MACHINE_WCHAR(ch0,ch1)  ( ((ch1)<<8) | ((ch0) & 0x00FF) )
#else
#define MAKE_MACHINE_WCHAR(ch0,ch1)  ( ((ch0)<<8) | ((ch1) & 0x00FF) )
#endif

#define TWO_BYTES(b0,b1)             b0,b1
#define ONE_WCHAR(ch0,ch1)           MAKE_MACHINE_WCHAR(ch0,ch1)

#endif /* 16/8 bit addressing */

#define MEMSET(pb,ch,cb) DRM_BYT_SetBytes((pb),0,(cb),(ch))
#define ZEROMEM(pb,cb)   DRM_BYT_SetBytes((pb),0,(cb),0)
#define MEMCPY(pbTo,pbFrom,cb)  DRM_BYT_CopyBytes(   (pbTo),0,(pbFrom),0,(cb))
#define MEMMOVE(pbTo,pbFrom,cb) DRM_BYT_MoveBytes(   (pbTo),0,(pbFrom),0,(cb))
#define MEMCMP(pbA,pbB,cb)      DRM_BYT_CompareBytes((pbA) ,0,(pbB),   0,(cb))

EXIT_PK_NAMESPACE;

#endif /* __DRM_BYTEMANIP_H__ */
