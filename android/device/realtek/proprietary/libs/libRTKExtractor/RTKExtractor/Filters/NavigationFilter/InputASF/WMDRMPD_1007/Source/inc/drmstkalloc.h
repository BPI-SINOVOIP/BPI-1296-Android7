/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_STACK_ALLOCATOR__
#define __DRM_STACK_ALLOCATOR__

#ifdef __cplusplus
extern "C" {
#endif

#define DRM_STK_REQUIRED(cbData) \
    (   (cbData) \
    +   SIZEOF(DRM_DWORD) \
    +   ( ((cbData) % SIZEOF(DRM_DWORD_PTR) == 0) ? (0) : (SIZEOF(DRM_DWORD_PTR) - ((cbData) % SIZEOF(DRM_DWORD_PTR))) ) )  

typedef struct __tagDRM_STACK_ALLOCATOR_CONTEXT
{
    DRM_DWORD nStackTop;    /* position of stack top */
    DRM_DWORD cbStack;      /* size of stack */
    DRM_BYTE *pbStack;      /* stack buffer */
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
DRM_RESULT DRM_API DRM_STK_Free(
    IN DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    IN DRM_VOID                    *pbBuffer);

#define SAFE_STK_FREE(pdsac,pv)    \
    if (pv != NULL)                \
    {                              \
        DRM_STK_Free (pdsac, pv);  \
        pv = NULL;                 \
    }

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
DRM_RESULT DRM_API DRM_STK_Alloc(
    IN  DRM_STACK_ALLOCATOR_CONTEXT *pContext,
    IN  DRM_DWORD                    cbSize,
    OUT DRM_VOID                   **ppbBuffer);


#ifdef __cplusplus
}
#endif

#endif /* __DRM_STACK_ALLOCATOR__ */

