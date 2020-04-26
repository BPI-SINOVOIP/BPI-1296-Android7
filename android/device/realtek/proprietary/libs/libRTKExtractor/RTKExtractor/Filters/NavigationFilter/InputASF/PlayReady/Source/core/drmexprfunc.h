/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_EXPR_FUNCTION_H__
#define __DRM_EXPR_FUNCTION_H__

ENTER_PK_NAMESPACE;

/* The public entry point */
DRM_API DRM_RESULT DRM_CALL FunctionGetValue(
    IN  TOKEN             *pOperator, 
    IN  DRM_LONG           cArgs, 
    IN  TOKEN              rgArgs[], 
    OUT TOKEN             *pOperandResult, 
    IN  EXPR_EVAL_CONTEXT *pEEContext );

DRM_API DRM_LONG DRM_CALL Arity(DRM_EXPR_FUNCTION_TYPE fnType);

DRM_API DRM_BOOL DRM_CALL IsValidFunction(
    IN const DRM_CONST_STRING       *pdstrToken, 
    IN       DRM_DWORD              *pfnType);


EXIT_PK_NAMESPACE;

#endif /*__DRM_EXPR_FUNCTION_H__ */
