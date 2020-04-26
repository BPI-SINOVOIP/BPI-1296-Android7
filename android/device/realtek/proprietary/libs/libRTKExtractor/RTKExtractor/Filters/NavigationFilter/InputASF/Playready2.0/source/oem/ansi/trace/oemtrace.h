/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMTRACE_H__
#define __OEMTRACE_H__

#if DRM_SUPPORT_TRACING

#include <oemtraceimpl.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_TRACE_GLOBAL_CONTEXT * DRM_CALL Oem_Trace_GetGlobalContext( DRM_VOID );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_Info(
    __in DRM_CHAR *f_pszFormat,
    ... );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_ID(
    __in DRM_DWORD f_dwFormatID,
    __in DRM_RESULT f_dr,
    __in DRM_DWORD f_cParam,
    __in DRM_DWORD f_dwParam1,
    __in DRM_DWORD f_dwParam2,
    __in DRM_DWORD f_dwParam3 );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_Enter_Scope(
    __in DRM_DWORD f_dwScopeID );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_Leave_Scope(
    __in DRM_DWORD f_dwScopeID,
    __in DRM_RESULT f_dr );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_Status_Code(
    __in DRM_DWORD f_dwEventID,
    __in DRM_RESULT f_dr );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_Start(
    __in_opt DRM_VOID *f_poOEMContext,
    __in_opt DRM_CONST_STRING *f_pdstrTraceLog );

DRM_API DRM_RESULT DRM_CALL Oem_Trace_Stop( DRM_VOID );

/* See drmtrace.h for detailed descriptions of each macro. */
#define INVALID_TRACE_ID                0xFFFFFFFF
#define DRM_TRACE_ID( dwTraceID )       INVALID_TRACE_ID

#define DRM_TRACE( szFormat )                           do { _DRM_TRACE_IF_NOT_FILTERED { Oem_Trace_Info szFormat } } while( FALSE )
#define DRM_TRACE_NO_SCOPE( szFormat )                  do { Oem_Trace_Info szFormat; } while( FALSE )
#define DRM_TRACE_ID0( eRobust, dwID, dr )              do { _DRM_TRACE_IF_NOT_FILTERED { Oem_Trace_ID( dwID, dr, 0, 0, 0 , 0 ); } } while( FALSE )
#define DRM_TRACE_ID1( eRobust, dwID, dr, dwParam1 )  \
                                                        do { _DRM_TRACE_IF_NOT_FILTERED { Oem_Trace_ID( dwID, dr, 1, dwParam1, 0 , 0 ); } } while( FALSE )
#define DRM_TRACE_ID2( eRobust, dwID, dr, dwParam1, dwParam2 )  \
                                                        do { _DRM_TRACE_IF_NOT_FILTERED { Oem_Trace_ID( dwID, dr, 2, dwParam1, dwParam2 , 0 ); } } } while( FALSE )
#define DRM_TRACE_ID3( eRobust, dwID, dr, dwParam1, dwParam2, dwParam3 )    \
                                                        do { _DRM_TRACE_IF_NOT_FILTERED { Oem_Trace_ID( dwID, dr, 3, dwParam1, dwParam2 , dwParam3 ); } } } while( FALSE )
#define DRM_TRACE_ENTER_SCOPE( eRobust, dwScopeID )     do { Oem_Trace_Enter_Scope( dwScopeID ); } while( FALSE )
#define DRM_TRACE_LEAVE_SCOPE( eRobust, dwScopeID, dr ) do { Oem_Trace_Leave_Scope( dwScopeID, dr ); } while( FALSE )
#define DRM_TRACE_STATUS_CODE( eRobust, dwEventID, dr ) do { _DRM_TRACE_IF_NOT_FILTERED { Oem_Trace_Status_Code( dwEventID, dr ); } } while( FALSE )

#define DRM_TRACE_START( pOEMContext, pdstrFilename )   do { Oem_Trace_Start( pOEMContext, pdstrFilename ); } while( FALSE )
#define DRM_TRACE_STOP()                                do { Oem_Trace_Stop(); } while( FALSE )

EXIT_PK_NAMESPACE;

#endif  /* DRM_SUPPORT_TRACING */

#endif  /* __OEMTRACE_H__ */
