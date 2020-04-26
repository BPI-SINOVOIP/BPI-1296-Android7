/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __CMDLNPARS_H__
#define __CMDLNPARS_H__

#define DRM_CMD_MAX_ARGUMENTS 15

#ifdef __cplusplus
extern "C" {
#endif

DRM_LONG DRM_CALL DRM_Main( 
    IN                  DRM_LONG argc, 
    __in_ecount( argc ) DRM_WCHAR** argv );

#ifdef __cplusplus
}
#endif

ENTER_PK_NAMESPACE;

DRM_LONG DRM_CALL DRM_CMD_ConvertToArgvArgc( 
    IN OUT __nullterminated  const         DRM_WCHAR *lpCmdLine, 
    __out_ecount( DRM_CMD_MAX_ARGUMENTS )  DRM_WCHAR *argv[DRM_CMD_MAX_ARGUMENTS]);

DRM_BOOL DRM_CALL DRM_CMD_ParseCmdLine(
    IN  const        DRM_WCHAR        *pwszArgument,
    __out_ecount(1)  DRM_WCHAR        *pwchOptionChar,
    OUT              DRM_CONST_STRING *pdstrParam,
    OUT              DRM_CONST_STRING *pdstrExtra);

DRM_BOOL DRM_CALL DRM_CMD_ParseCmdLineMultiChar(
    IN  const DRM_WCHAR        *pwszArgument,
    OUT       DRM_CONST_STRING *pstrOptionStr,
    OUT       DRM_CONST_STRING *pdstrParam,
    OUT       DRM_CONST_STRING *pdstrExtra);

DRM_RESULT DRM_CALL DRM_CMD_TryProcessDefaultOption(
    __in const DRM_WCHAR  *f_wszArgument );

EXIT_PK_NAMESPACE;

#endif /* __CMDLNPARS_H__ */
