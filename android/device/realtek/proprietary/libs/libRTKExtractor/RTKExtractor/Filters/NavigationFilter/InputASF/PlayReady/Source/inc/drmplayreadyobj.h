/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMPLAYREADYOBJ_H__
#define __DRMPLAYREADYOBJ_H__

ENTER_PK_NAMESPACE;

#include <drmcipher.h>
#include <drmembeddedstore_impl.h>

/*
** PlayReady Data Header Format
**
** Field         | Type
** -----------------------------
** Data Size     | DWORD
** Record Count  | WORD
*/
#define SIZE_OF_DRM_PLAYREADY_DATA_HEADER   ( SIZEOF( DRM_DWORD ) + SIZEOF( DRM_WORD ) )

/*
** PlayReady Record Header Format
**
** Field         | Type
** -----------------------------
** Record Type   | WORD
** Record Size   | WORD
*/
#define SIZE_OF_DRM_PLAYREADY_RECORD_HEADER ( 2 * SIZEOF( DRM_WORD ) )
#define SIZE_OF_PLAYREADY_EMBEDDED_LICENSE_SPACE (1024*10)

/*
** Contains space for the PlayReady Object header, two record headers,
** WMDRM header data, and the embedded license store.
*/
#define DRM_MAX_PLAYREADYOBJSIZE            ( SIZE_OF_DRM_PLAYREADY_DATA_HEADER + \
                                              2 * SIZE_OF_DRM_PLAYREADY_RECORD_HEADER + \
                                              DRM_MAX_HEADERDATASIZE + \
                                              MAX_EMBEDDED_STORE_LEN  )

typedef enum 
{
    PLAYREADY_V4_XML_HEADER          = 0x0001,
    PLAYREADY_V4_SIGNATURE           = 0x0002,
    PLAYREADY_EMBEDDED_LICENSE_STORE = 0x0003
} eDRMPlayReady_RECORD_TYPES;

typedef enum
{
    RM_HEADER_VERSION_UNKNOWN = 0,
    RM_HEADER_VERSION_2       = 2,
    RM_HEADER_VERSION_4       = 4,
} eRMHeaderVersion;

DRM_API DRM_RESULT DRM_CALL DRM_PRO_GetRMHeaderVersion(
    __in_bcount( f_cbHeader ) const DRM_BYTE        *f_pbHeader,
    __in                      const DRM_DWORD        f_cbHeader,
    __out_ecount( 1 )               eRMHeaderVersion *f_peVersion );

DRM_API DRM_RESULT DRM_CALL DRM_PRO_IsValidObject( 
    __in_bcount( f_cbBuffer ) const DRM_BYTE *f_pbBuffer, 
    __in                      const DRM_DWORD f_cbBuffer ); 

DRM_API DRM_RESULT DRM_CALL DRM_PRO_GetRecord( 
    __in_bcount( f_cbBuffer )   const DRM_BYTE   *f_pbBuffer,
    __in                        const DRM_DWORD   f_cbBuffer,
    __in                              DRM_WORD    f_wRecordType,
    __out                             DRM_BYTE  **f_ppbEmbeddedRecord,
    __out                             DRM_DWORD  *f_pcbEmbeddedRecord );

DRM_API DRM_RESULT DRM_CALL DRM_PRO_Create
(
    __in    DRM_CONST_STRING           *f_pdstrPlayReadyHeader,
    __out_bcount_opt( *f_pcbPlayReadyObject )
            DRM_BYTE                   *f_pbPlayReadyObject,
    __inout DRM_DWORD                  *f_pcbPlayReadyObject
);

DRM_API DRM_RESULT DRM_CALL DRM_PRO_GetCipherTypeFromPlayReadyHeader(
    __in_bcount( f_cbPlayReadyHeader )
            DRM_BYTE  *f_pbPlayReadyHeader,
    __in    DRM_DWORD f_cbPlayReadyHeader,
    __out   DRM_SUPPORTED_CIPHERS *f_peCipherType );

DRM_API DRM_RESULT DRM_CALL DRM_PRO_GetCipherType(
    __in_bcount( f_cbPlayReadyObject )
            DRM_BYTE  *f_pbPlayReadyObject,
    __in    DRM_DWORD f_cbPlayReadyObject,
    __out   DRM_SUPPORTED_CIPHERS *f_peCipherType );

DRM_API DRM_RESULT DRM_CALL DRM_PRO_ConvertHeaderFromWmdrmToPlayReady( 
    __in_bcount( f_cbWmdrmHeader )      const DRM_BYTE  *f_pbWmdrmHeader,
    __in                                const DRM_DWORD  f_cbWmdrmHeader,
    __in_ecount_nz_opt( f_cchPlayReadySilentURL )
                                        const DRM_WCHAR *f_pwchPlayReadySilentURL,
    __in                                const DRM_DWORD  f_cchPlayReadySilentURL,
    __in_ecount_nz_opt( f_cchPlayReadyNonSilentURL )
                                        const DRM_WCHAR *f_pwchPlayReadyNonSilentURL,
    __in                                const DRM_DWORD  f_cchPlayReadyNonSilentURL,
    __in_ecount_nz_opt( f_cchServiceID )
                                        const DRM_WCHAR *f_pwchServiceID,
    __in                                const DRM_DWORD  f_cchServiceID,
    __inout_bcount_opt( *f_pcbPlayReadyHeader )      
                                              DRM_BYTE  *f_pbPlayReadyHeader,
    __in                                      DRM_DWORD *f_pcbPlayReadyHeader );

DRM_API DRM_RESULT DRM_CALL Drm_PlayReadyObject_ConvertFromWmdrmHeader( 
    __in_bcount( f_cbWmdrmHeader )  const DRM_BYTE  *f_pbWmdrmHeader,
    __in                            const DRM_DWORD  f_cbWmdrmHeader,
    __in_ecount_nz_opt( f_cchPlayReadySilentURL )
                                    const DRM_WCHAR *f_pwchPlayReadySilentURL,
    __in                            const DRM_DWORD  f_cchPlayReadySilentURL,
    __in_ecount_nz_opt( f_cchPlayReadyNonSilentURL )
                                    const DRM_WCHAR *f_pwchPlayReadyNonSilentURL,
    __in                            const DRM_DWORD  f_cchPlayReadyNonSilentURL,
    __in_ecount_nz_opt( f_cchServiceID )
                                    const DRM_WCHAR *f_pwchServiceID,
    __in                            const DRM_DWORD  f_cchServiceID,
    __inout_bcount_opt( *f_pcbPlayReadyObject )
                                          DRM_BYTE  *f_pbPlayReadyObject,
    __in                                  DRM_DWORD *f_pcbPlayReadyObject );


EXIT_PK_NAMESPACE;

#endif /* __DRMPLAYREADYOBJ_H__ */

