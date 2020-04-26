/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_EXPR_EVAL_H__
#define __DRM_EXPR_EVAL_H__

#include <drmlicstore.h>
#include <drmtoken.h>

ENTER_PK_NAMESPACE;


/* In the following Extract*Token functions if cchExpression == 0 it is assumed that the string is NULL termintated. */
/* Note: These functions modify the pointer they are given. */

DRM_API DRM_RESULT DRM_CALL ExtractStringToken( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_API DRM_RESULT DRM_CALL ExtractLongToken  ( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_API DRM_RESULT DRM_CALL ExtractSymbolToken( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_API DRM_RESULT DRM_CALL ExtractDateToken  ( DRM_CONST_STRING* pdstrExpression, TOKEN *pToken );


typedef DRM_RESULT (DRM_CALL *pfnGetVariable)(const DRM_CONST_STRING*, TOKEN*, DRM_VOID*);
typedef DRM_RESULT (DRM_CALL *pfnSetVariable)(const DRM_CONST_STRING*, const TOKEN*, TOKEN*, DRM_VOID*);

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

    /* Internal memory sandbox for ExprEval to use.  Caller doesn't set this memory */
    EXPR_EVAL_INTERNAL_DATA ExprEvalData;
} EXPR_EVAL_CONTEXT;


DRM_API DRM_RESULT DRM_CALL DRM_EXPR_EvaluateExpression(
    IN const DRM_CONST_STRING  *pdstrExpression, 
    IN OUT   EXPR_EVAL_CONTEXT *pEEContext, 
       OUT   TOKEN             *ptResult );

/*
** Secure store variable accessor/mutator functions. 
** These will be given to the Expression evaluator via it's context structure.  
** The implementation lives in exprvariable.c 
*/
DRM_API DRM_RESULT DRM_CALL GlobalSetVariable(
    const DRM_CONST_STRING *pStringToken, 
    const TOKEN            *pNewValue, 
          TOKEN            *pResult, 
          DRM_VOID         *pvOpaqueData );

DRM_API DRM_RESULT DRM_CALL GlobalGetVariable(
    const DRM_CONST_STRING *pStringToken,
          TOKEN            *pResult,
          DRM_VOID         *pvOpaqueData );

EXIT_PK_NAMESPACE;

#endif /* __DRM_EXPR_EVAL_H__ */
