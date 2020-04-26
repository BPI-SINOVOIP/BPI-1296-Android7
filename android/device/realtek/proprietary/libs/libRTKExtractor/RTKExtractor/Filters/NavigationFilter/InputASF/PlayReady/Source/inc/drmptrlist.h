/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef DRMPTRLIST_H
#define DRMPTRLIST_H

ENTER_PK_NAMESPACE;

typedef struct tagPTR_LIST_ENTRY
{
    DRM_VOID                 *pbData;
    struct tagPTR_LIST_ENTRY *pNext;
} PTR_LIST_ENTRY;

typedef struct tagPTR_LIST
{
    DRM_DWORD       dwMaxSize;
    DRM_DWORD       dwCurrentSize;
    PTR_LIST_ENTRY *pHead;
} PTR_LIST;


DRM_API DRM_RESULT DRM_CALL DRM_PtrList_Initialize(
    __out       PTR_LIST  * const f_pPtrList,
    __in const  DRM_DWORD         f_dwSize
);

DRM_API DRM_VOID DRM_CALL DRM_PtrList_Uninitialize(
    __out PTR_LIST * const pPtrList
);

DRM_API DRM_RESULT DRM_CALL DRM_PtrList_GetFront(
    __in const PTR_LIST  * const f_pPtrList,
    __out      DRM_VOID        **f_ppbData
);

DRM_API DRM_RESULT DRM_CALL DRM_PtrList_RemoveHead(
    __in    PTR_LIST  * const f_pPtrList,
    __out   DRM_VOID        **f_ppbData
);

DRM_API DRM_RESULT DRM_CALL DRM_PtrList_AddTail(
    __in  PTR_LIST * const f_pPtrList,
    __in  DRM_VOID * const f_pbData
);

DRM_API DRM_RESULT DRM_CALL DRM_PtrList_GetTail(
    __in const   PTR_LIST * const f_pPtrList,
    __out        DRM_VOID       **f_ppbData
);

DRM_API DRM_RESULT DRM_CALL DRM_PtrList_GetByIndex(
    __in const PTR_LIST * const   f_pPtrList,
    __in const DRM_DWORD          f_dwIndex,
    __out      DRM_VOID         **f_ppbData
);

DRM_API DRM_RESULT DRM_CALL DRM_PtrList_RemoveByIndex(
    __in       PTR_LIST * const   f_pPtrList,
    __in const DRM_DWORD          f_dwIndex,
    __out      DRM_VOID         **f_ppbData
);

EXIT_PK_NAMESPACE;

#endif /* DRMPTRLIST_H */
