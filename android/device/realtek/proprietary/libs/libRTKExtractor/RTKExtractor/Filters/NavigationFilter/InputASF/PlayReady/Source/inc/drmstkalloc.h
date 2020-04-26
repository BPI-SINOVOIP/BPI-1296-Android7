/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_STACK_ALLOCATOR__
#define __DRM_STACK_ALLOCATOR__

ENTER_PK_NAMESPACE;

#define DRM_STK_REQUIRED(cbData) \
    (   (cbData) \
    +   SIZEOF(DRM_DWORD) \
    +   ( ((cbData) % SIZEOF(DRM_DWORD_PTR) == 0) ? (0) : (SIZEOF(DRM_DWORD_PTR) - ((cbData) % SIZEOF(DRM_DWORD_PTR))) ) )  

typedef struct __tagDRM_STACK_ALLOCATOR_CONTEXT
{
    DRM_DWORD nStackTop;    /* position of stack top */
    DRM_DWORD cbStack;      /* size of stack */
    DRM_BYTE *pbStack;      /* stack buffer */
    DRM_BOOL  fWasPreAlloc; /* used to signal that we PreAlloc, and Alloc shouldn't fill mem with 0xaa */
} DRM_STACK_ALLOCATOR_CONTEXT;

/*********************************************************************
**
**  Function:  DRM_STK_Free
**
**  Synopsis:  Free a buffer that was allocated using DRM_STK_Alloc.
**
**  Arguments:  
**     [pContext] -- Stack allcator context that was used to allocate the pbBuffer parameter
**     [pbBuffer] -- Pointer returned from a call to DRM_STK_Alloc
**
**  Notes:  Pointers must be freed in LIFO order ( just like a stack ).
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_STK_Free(
    IN DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    IN DRM_VOID                    *pbBuffer);

#define SAFE_STK_FREE(pdsac,pv)    \
    if (pv != NULL)                \
    {                              \
        DRM_STK_Free (pdsac, pv);  \
        pv = NULL;                 \
    }

DRM_API DRM_RESULT DRM_CALL DRM_STK_PreAlloc(
    IN  DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    OUT DRM_DWORD                   *pcbSize,
    __deref_out_bcount( *pcbSize ) DRM_VOID **ppbBuffer);

/*********************************************************************
**
**  Function:  DRM_STK_Alloc
**
**  Synopsis:  Allocates a buffer from a give stack context
**
**  Arguments:  
**     [pContext] -- Stack allocator context to allocate from
**     [cbSize] -- Size of the buffer needed
**     [ppbBuffer] -- Pointer to pointer to hold the new memory offset.
**
**  Notes:  Pointers must be freed in LIFO order ( just like a stack ).
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_STK_Alloc(
    IN  DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    IN  DRM_DWORD                    cbSize,
    __deref_out_bcount( cbSize ) DRM_VOID **ppbBuffer);

DRM_API DRM_RESULT DRM_CALL DRM_STK_Alloc_Aligned(
    IN  DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    IN  DRM_DWORD                    cbSize,
    IN  DRM_DWORD                    cbAligned,
    __out_opt DRM_VOID             **ppbBufferRaw,
    __deref_out_bcount( cbSize ) DRM_VOID  **ppbBufferAligned );

/*********************************************************************
**
**  Function:  DRM_STK_Init
**
**  Synopsis:  Initializes stack with supplied buffer and size.
**             It is kind of trivial, but frees person from
               knoledge about stack structure. 
**  Arguments:  
**     [pContext] -- Stack allocator context to allocate from
**     [pbBuffer] -- Pointer for the buffer for the stack
**     [cbSize] --  Size of the buffer supplied
**
**  Notes:  Pointers must be freed in LIFO order ( just like a stack ).
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_STK_Init(
    IN  DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    __in_bcount( cbSize) DRM_BYTE *pbBuffer,
    IN  DRM_DWORD                    cbSize   );

EXIT_PK_NAMESPACE;

#endif /* __DRM_STACK_ALLOCATOR__ */

