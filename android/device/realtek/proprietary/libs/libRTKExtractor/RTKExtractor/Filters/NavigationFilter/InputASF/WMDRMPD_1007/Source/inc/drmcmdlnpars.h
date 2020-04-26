/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __CMDLNPARS_H__
#define __CMDLNPARS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
**
**  Function:  DRM_CMD_ParseCmdLine
**
**  Synopsis:  Parse a string for command line arguments.
**
**  Arguments:  
**     [pwszArgument] -- Source string
**     [pwchOptionChar] -- A substring within pwszString
**     [pwszParam] -- The parameter value associated with the option character
**     [pwszExtra] -- Extra data 
**
**  Returns:  
**
**  Notes:  Argument syntax is -c[:param[,extra]]
**
*********************************************************************/

DRM_BOOL DRM_API DRM_CMD_ParseCmdLine(
    IN  const DRM_WCHAR        *pwszArgument,
    OUT       DRM_WCHAR        *pwchOptionChar,
    OUT       DRM_CONST_STRING *pdstrParam,
    OUT       DRM_CONST_STRING *pdstrExtra);

#ifdef __cplusplus
}
#endif

#endif /* __CMDLNPARS_H__ */
