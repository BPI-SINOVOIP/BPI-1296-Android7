/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_EXPR_EVAL_H__
#define __DRM_EXPR_EVAL_H__

#include <drmlicstore.h>
#include <drmtoken.h>

#ifdef __cplusplus
extern "C" {
#endif


/* In the following Extract*Token functions if cchExpression == 0 it is assumed that the string is NULL termintated. */
/* Note: These functions modify the pointer they are given. */

DRM_RESULT DRM_API ExtractStringToken( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_RESULT DRM_API ExtractLongToken  ( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_RESULT DRM_API ExtractSymbolToken( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_RESULT DRM_API ExtractDateToken  ( DRM_CONST_STRING* pdstrExpression, TOKEN *pToken );


typedef DRM_RESULT (DRM_API *pfnGetVariable)(const DRM_CONST_STRING*, TOKEN*, DRM_VOID*);
typedef DRM_RESULT (DRM_API *pfnSetVariable)(const DRM_CONST_STRING*, const TOKEN*, TOKEN*, DRM_VOID*);

#define DRM_EXPR_EVAL_MAX_TOKEN_STACK_SIZE 15

typedef struct tagEXPR_EVAL_INTERNAL_DATA
{
    TOKEN rgOperandTokenStack[DRM_EXPR_EVAL_MAX_TOKEN_STACK_SIZE];
    TOKEN rgOperatorTokenStack[DRM_EXPR_EVAL_MAX_TOKEN_STACK_SIZE];
    DRM_DWORD dwOperandStack;
    DRM_DWORD dwOperatorStack;
    DRM_BOOL fShortCircuit;
} EXPR_EVAL_INTERNAL_DATA;

typedef struct tagEXPR_EVAL_CONTEXT
{
    /* Data the caller must set  */
    pfnGetVariable GetVariable; /* Function pointers so variables can be fetched/set */
    pfnSetVariable SetVariable;
    DRM_VOID* pvOpaqueData;         /* Pointer to be passed into GetVariable() and SetVariable() */
    DRM_KID KID;
    DRM_LID LID;
    DRM_LICSTOREENUM_CONTEXT *pLicStoreEnumContext;    /* ExprEval needs an open license store enum context in case the license calls delete license */

    /* Internal memory sandbox for ExprEval to use.  Caller doesn't set this memory */
    EXPR_EVAL_INTERNAL_DATA ExprEvalData;
} EXPR_EVAL_CONTEXT;


DRM_RESULT DRM_API DRM_EXPR_EvaluateExpression(
    IN const DRM_CONST_STRING  *pdstrExpression, 
    IN OUT   EXPR_EVAL_CONTEXT *pEEContext, 
       OUT   TOKEN             *ptResult );


#ifdef __cplusplus
}
#endif

#endif /* __DRM_EXPR_EVAL_H__ */
