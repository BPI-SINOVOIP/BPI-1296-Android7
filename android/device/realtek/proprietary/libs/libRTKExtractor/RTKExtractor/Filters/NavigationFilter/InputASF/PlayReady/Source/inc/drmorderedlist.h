/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef DRMORDEREDLIST_H
#define DRMORDEREDLIST_H

ENTER_PK_NAMESPACE;

typedef struct tagORDERED_LIST_ENTRY
{
    DRM_UINT64                    qwValue;
    DRM_VOID                     *pbData; 
    struct tagORDERED_LIST_ENTRY *pNext;
} ORDERED_LIST_ENTRY;

typedef struct tagORDERED_LIST
{
    DRM_DWORD                     dwCurrentSize;
    DRM_DWORD                     dwMaxSize;
    ORDERED_LIST_ENTRY           *pHead;
} ORDERED_LIST;


DRM_RESULT DRM_CALL DRM_OrderedList_Initialize(
    __out      ORDERED_LIST            *pOrderedList,
    __in const DRM_DWORD                dwSize
);

DRM_VOID DRM_CALL DRM_OrderedList_Uninitialize(
    __out ORDERED_LIST * const pOrderedList
);

DRM_RESULT DRM_CALL DRM_OrderedList_GetCount(
    __in const ORDERED_LIST * const pOrderedList,
    __out      DRM_DWORD    * const pdwCount
);

DRM_RESULT DRM_CALL DRM_OrderedList_AddEntry(
    __inout    ORDERED_LIST * const pOrderedList,
    __in const DRM_UINT64           qwValue,
    __in       DRM_VOID     * const pbData
);

DRM_RESULT DRM_CALL DRM_OrderedList_RemoveEntry(
    __inout    ORDERED_LIST  * const pOrderedList,
    __in const DRM_DWORD             dwFirstEntryToConsider,
    __out      DRM_UINT64    * const pqwValue,
    __out      DRM_VOID            **ppbData
);

DRM_RESULT DRM_CALL DRM_OrderedList_AddLeadingEntry(
    __inout    ORDERED_LIST  * const pOrderedList,
    __in const DRM_UINT64            qwValue,
    __in       DRM_VOID      * const pbData
);

DRM_RESULT DRM_CALL DRM_OrderedList_GetEntry(
    __in const ORDERED_LIST  * const pOrderedList,
    __in const DRM_DWORD             dwFirstEntryToConsider,
    __out      DRM_UINT64    * const pqwValue,
    __out      DRM_VOID            **ppbData
);

EXIT_PK_NAMESPACE;

#endif /* DRMORDEREDLIST_H */
