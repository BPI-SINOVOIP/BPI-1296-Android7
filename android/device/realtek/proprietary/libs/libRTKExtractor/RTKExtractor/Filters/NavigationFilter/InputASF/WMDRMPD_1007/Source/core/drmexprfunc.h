/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_EXPR_FUNCTION_H__
#define __DRM_EXPR_FUNCTION_H__

#ifdef __cplusplus
extern "C" {
#endif

/* The public entry point */
DRM_RESULT DRM_API FunctionGetValue(
    IN  TOKEN             *pOperator, 
    IN  DRM_LONG           cArgs, 
    IN  TOKEN              rgArgs[], 
    OUT TOKEN             *pOperandResult, 
    IN  EXPR_EVAL_CONTEXT *pEEContext );

DRM_LONG DRM_API Arity(DRM_EXPR_FUNCTION_TYPE fnType);

DRM_BOOL DRM_API IsValidFunction(
    IN const DRM_CONST_STRING       *pdstrToken, 
    IN       DRM_DWORD              *pfnType);


#ifdef __cplusplus
}
#endif

#endif /*__DRM_EXPR_FUNCTION_H__ */
