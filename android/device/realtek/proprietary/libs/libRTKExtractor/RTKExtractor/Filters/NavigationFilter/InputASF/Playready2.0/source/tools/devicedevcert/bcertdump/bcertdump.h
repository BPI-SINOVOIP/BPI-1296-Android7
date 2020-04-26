/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __BCERTDUMP_H__
#define __BCERTDUMP_H__

typedef int (__cdecl*PRINTFUNC)(__in_z __format_string const char * _Format, ...);

#ifdef __cplusplus
extern "C" {
#endif

DRM_API DRM_RESULT DRM_CALL DRM_BCDM_PrintCertChain (
    __in_bcount(f_cbBinaryCertDataLength) const DRM_BYTE  *f_pbBinaryCertData,
    __in                                        DRM_DWORD  f_cbBinaryCertDataLength,
    __in                                        DRM_BOOL   f_fVerboseOutput,
    __in                                        DRM_BOOL   f_fUseB64,
    __in                                        PRINTFUNC  f_funcPrint );

#ifdef __cplusplus
}
#endif

#endif /* __BCERTDUMP_H__ */

