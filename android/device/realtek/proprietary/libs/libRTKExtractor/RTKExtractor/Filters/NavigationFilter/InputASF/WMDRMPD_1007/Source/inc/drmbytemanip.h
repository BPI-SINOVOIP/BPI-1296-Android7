/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_BYTEMANIP_H__ 
#define __DRM_BYTEMANIP_H__ 

#ifdef __cplusplus
extern "C" {
#endif

DRM_VOID DRM_API DRM_BYT_ReverseBytes(
    IN OUT DRM_BYTE *pbData,
    IN     DRM_DWORD cbData );

#if SIXTEEN_BIT_ADDRESSING

/* the distinction between BYTE and NATIVE_BYTE is an artificial one solely for
   testing under x86 and other environments capable of addressing 8-bit quantities.
   Redefining a BYTE to be a WORD allows simulation of 16-bit addressing 
   
   For practical reasons it serves as a reminder that what looks like a BYTE isn't
   necessarily the familiar 8 bits */

typedef DRM_BYTE DRM_NATIVE_BYTE;
typedef DRM_CHAR DRM_NATIVE_CHAR;

DRM_VOID DRM_API DRM_BYT_CopyBytes( 
       OUT   DRM_VOID *f_pbTo, 
    IN       DRM_DWORD f_ibTo, 
    IN const DRM_VOID *f_pbFrom,
    IN       DRM_DWORD f_ibFrom,
    IN       DRM_DWORD f_cb );

DRM_VOID DRM_API DRM_BYT_MoveBytes( 
       OUT   DRM_VOID *f_pbTo, 
    IN       DRM_DWORD f_ibTo, 
    IN const DRM_VOID *f_pbFrom,
    IN       DRM_DWORD f_ibFrom,
    IN       DRM_DWORD f_cb );

DRM_VOID DRM_API DRM_BYT_SetBytes(
       OUT DRM_VOID *f_pb,
    IN     DRM_DWORD f_ib,
    IN     DRM_DWORD f_cb,
    IN     DRM_BYTE  f_b );

DRM_INT DRM_API DRM_BYT_CompareBytes(
    IN const DRM_VOID *f_pbA,
    IN       DRM_DWORD f_ibA,
    IN const DRM_VOID *f_pbB,
    IN       DRM_DWORD f_ibB,
    IN       DRM_DWORD f_cb );



DRM_NATIVE_BYTE DRM_API DRM_16B_Read8BitByte(
    IN  const DRM_VOID *f_pv,
    IN        DRM_DWORD f_ib);

DRM_NATIVE_CHAR DRM_API DRM_16B_Read8BitChar(
    IN  DRM_VOID *f_pv,
    IN  DRM_DWORD f_ich);

DRM_VOID DRM_API DRM_16B_Write8BitByte(
    OUT DRM_VOID        *f_pv,
    IN  DRM_DWORD        f_ib,
    IN  DRM_NATIVE_BYTE  f_b);

DRM_VOID DRM_API DRM_16B_Write8BitChar(
    OUT DRM_VOID        *f_pv,
    IN  DRM_DWORD        f_ich,
    IN  DRM_NATIVE_CHAR  f_ch);

DRM_VOID DRM_API DRM_16B_Read8BitBytes(
    IN  DRM_VOID        *f_pv,
    IN  DRM_DWORD        f_cb,
    OUT DRM_NATIVE_BYTE *f_pb);

DRM_VOID DRM_API DRM_16B_Write8BitBytes(
    OUT DRM_VOID        *f_pv,
    IN  DRM_DWORD        f_cb,
    IN  DRM_NATIVE_BYTE *f_pb);

DRM_VOID DRM_API DRM_16B_Read8BitString(
    IN  DRM_VOID        *f_pv,
    IN  DRM_DWORD        f_cch,
    OUT DRM_NATIVE_CHAR *f_pch);

DRM_VOID DRM_API DRM_16B_Write8BitString(
    OUT DRM_VOID        *f_pv,
    IN  DRM_DWORD        f_cch,
    IN  DRM_NATIVE_CHAR *f_pch);

#define DRM_PACK_BYTES_IN_PLACE 0x00000001

DRM_VOID *DRM_API DRM_16B_Pack8BitBytes(
    DRM_NATIVE_BYTE *f_pbMachine,
    DRM_DWORD        f_cbMachine,
    DRM_VOID        *f_pvPacked,
    DRM_DWORD        f_fOptions);

DRM_VOID *DRM_API DRM_16B_IncrementPackedPointer(
    IN OUT DRM_NATIVE_BYTE *f_pbMachine,
    IN     DRM_DWORD        f_cbMachine,
       OUT DRM_NATIVE_BYTE *f_pbHead);

DRM_VOID *DRM_API DRM_16B_DecrementPackedPointer(
    IN OUT DRM_NATIVE_BYTE  *f_pbMachine,
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


#define DRM_BYT_CopyBytes(to,tooffset,from,fromoffset,count) memcpy(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))
#define DRM_BYT_MoveBytes(to,tooffset,from,fromoffset,count) memmove(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))
#define DRM_BYT_SetBytes(pb,ib,cb,b) memset(&((pb)[(ib)]),b,cb)
#define DRM_BYT_CompareBytes(pbA,ibA,pbB,ibB,cb) memcmp(&((pbA)[(ibA)]),&((pbB)[(ibB)]),(cb))

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

#ifdef __cplusplus
}
#endif

#endif /* __DRM_BYTEMANIP_H__ */
