/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/******************************************************************************
**    File drmcfdalloc.h - Interface for memory allocation 
**    Two implementation are provided:
**    1. Legacy implementation that allocates memory from global stack
**    2. Dynamic memory allocation (using Oem_MemAlloc)
**
**    Use macro DRM_MSDB_USE_DYNAMIC_MEM_ALLOCATION (values are 0 or 1) 
**    to specify required implementation
**    
**    Notes about stack memory allocation:
**    1. Before using any allocation functions user should call DRM_CFD_InitStackMemory
**    2. Allocation with DRM_CFD_Alloc and freeing with DRM_CFD_free should be symmetrical.
**       It means if memory buffers are allocated in order 1,2,3, they should be freed 
**       in opposite order 3,2,1.
******************************************************************************/

#ifndef __DRMCFDALLOC_H__
#define __DRMCFDALLOC_H__

#include <drmnamespace.h>   /* defining proper namespace (if used) */

ENTER_PK_NAMESPACE;

/*********************************************************************
**
**  Function:  DRM_CFD_InitStackMemory
**
**  Synopsis:  Stack-based memory allocation: provides OEM buffer 
**             Dynamic memory allocation: does nothing
**
**  Arguments:  
**     [f_pbBuffer] -- Pointer to an OEM allocated buffer..
**     [f_cbBufferSize] -- Size of the buffer.
** 
** 
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CFD_InitStackMemory
(
    __in_bcount_opt(f_cbBufferSize) DRM_VOID *f_pbBuffer, 
    __in                            DRM_DWORD f_cbBufferSize
);

/*********************************************************************
**
**  Function:  DRM_CFD_FreeStackMemory
**
**  Synopsis:  Stack-based memory allocation: Free an OEM stack buffer 
**             Dynamic memory allocation: does nothing 
** 
**  Arguments:  
**     [f_ppbBuffer] -- Pointer to pointer to hold original memory block  
**                   used in DRM_CFD_InitStackMemory.
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CFD_FreeStackMemory( __out_opt DRM_VOID **f_ppbBuffer );

/*********************************************************************
**
**  Function:  DRM_CFD_GetAllocatedMemory
**
**  Synopsis:  Stack-based memory allocation: 
**                 returns amount of memory currenly allocated by DRM_CFD_Alloc
**             Dynamic memory allocation: returns 0 (zero) 
** 
**  Arguments:  
**     [f_pdwAllocatedMem] -- Pointer to DWORD variable filled with amount of 
**                            allocated memory.
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CFD_GetAllocatedMemory( __out DRM_DWORD *f_pdwAllocatedMem );

/*********************************************************************
**
**  Function:  DRM_CFD_Alloc
**
**  Synopsis:  Stack-based memory allocation: allocates a buffer from a static stack context
**             Dynamic memory allocation: allocates a buffer of specified size
**
**  Arguments:  
**     [f_cbSize] -- Size of the buffer needed
**      
**  Return value.
**     Returns pointer to buffer is successful, NULL otherwise.
**     The only indication of error - returned NULL pointer.
**     Signature of this function is the same as malloc or Oem_MemAlloc.
**
**  Notes:  If stack allocation is used then pointers must be freed in LIFO order.
**
*********************************************************************/
DRM_API DRM_VOID* DRM_CALL DRM_CFD_Alloc( __in DRM_DWORD f_cbSize );

/*********************************************************************
**
**  Function:  DRM_CFD_free
**
**  Synopsis:  Stack-based memory allocation: 
**                 free a buffer that was allocated using DRM_CFD_Alloc.
**             Dynamic memory allocation: free a memory buffer
**
**  Arguments:  
**     [f_pbBuffer] -- Pointer returned from a call to DRM_CFD_Alloc
**
**  Notes:  If stack allocation is used then pointers must be freed in LIFO order.
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_CFD_Free( __in DRM_VOID *f_pbBuffer );


EXIT_PK_NAMESPACE;

#endif /* __DRMCFDALLOC_H__ */ 
