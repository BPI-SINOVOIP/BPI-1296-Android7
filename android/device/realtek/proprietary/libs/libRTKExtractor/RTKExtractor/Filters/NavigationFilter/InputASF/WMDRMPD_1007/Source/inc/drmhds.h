/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRM_HDS_H__
#define __DRM_HDS_H__

#include <drmhds_impl.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __tagDRM_HDS_NAMESPACE_BLOB
{
    DRM_BYTE rgb[__CB_DECL(DRM_HDS_NSNAME_SIZE)];
} DRM_HDS_NAMESPACE;

typedef struct __tagDRM_HDS_CONTEXT
{
    DRM_BYTE rgbOpaqueData[__CB_DECL( DRM_HDS_CONTEXT_LEN )];
} DRM_HDS_CONTEXT;

typedef struct __tagDRM_HDS_NAMESPACE_CONTEXT
{
    DRM_BYTE rgbOpaqueData[ __CB_DECL(DRM_HDS_NAMESPACE_CONTEXT_LEN) ];
} DRM_HDS_NAMESPACE_CONTEXT;

typedef struct __tagDRM_HDS_SLOT_CONTEXT
{
    DRM_BYTE rgbOpaqueData[ __CB_DECL(DRM_HDS_SLOT_CONTEXT_LEN) ];
} DRM_HDS_SLOT_CONTEXT;

typedef struct __tagDRM_HDS_ENUM_CONTEXT
{
    DRM_BYTE rgbOpaqueData[ __CB_DECL(DRM_HDS_ENUM_CONTEXT_LEN) ];
} DRM_HDS_ENUM_CONTEXT;

typedef struct __tagDRM_HDS_BLOCKSCAN_CONTEXT
{
    DRM_BYTE rgbOpaqueData[__CB_DECL(DRM_HDS_BLOCKSCAN_CONTEXT_LEN)];
} DRM_HDS_BLOCKSCAN_CONTEXT;

typedef struct __tagDRM_HDS_SLOT_HINT
{
    DRM_BYTE rgbOpaqueData[__CB_DECL(DRM_HDS_SLOT_HINT_LEN)];
} DRM_HDS_SLOT_HINT;

/* 
** API Functions 
*/


/*
** ----------------------------------------------------------------------------
** Methods applied to whole hashed file store 
** ----------------------------------------------------------------------------
*/

/**********************************************************************
** Function:    DRM_HDS_Init
** Synopsis:    Init HDS Context
** Arguments:   [pcontextHDS] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       The function MUST be called in the main thread BEFORE
**              any of the child thread(s) starting sharing the same 
**              context. If this function is called after the child
**              thread(s) started, the result is undefined.
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_Init( 
    IN OUT DRM_HDS_CONTEXT *pcontextHDS);


/**********************************************************************
** Function:    DRM_HDS_Uninit
** Synopsis:    Uninit HDS Context
** Arguments:   [pcontextHDS] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       This function MUST be called in the main thread AFTER
**              any of the child thread(s) sharing the same context 
**              stopped. If this function is called before the child
**              thread(s) stopped, the result is undefined.
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_Uninit( 
    IN OUT DRM_HDS_CONTEXT *pcontextHDS);


/**********************************************************************
** Function:    DRM_HDS_CreateStore
** Synopsis:    Create DRM data store file
** Arguments:   [] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_CreateStore( 
    IN const DRM_WCHAR       *wszHdsName, 
    IN const DRM_DWORD        dwBlockSize,       /* block size is limited up to 64K */
    IN const DRM_HDSBLKNUM    eBlkNumType,
    IN OUT   DRM_HDS_CONTEXT *pcontextHDS,
    IN       DRM_BOOL         fTruncateExist);

/**********************************************************************
** Function:    DRM_HDS_CreateStore
** Synopsis:    Create DRM data store file
** Arguments:   [] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_CreateStore2( 
    IN const DRM_WCHAR       *wszHdsName, 
    IN const DRM_DWORD        dwBlockSize,       /* block size is limited up to 64K */
    IN const DRM_HDSBLKNUM    eBlkNumType,
    IN OUT   DRM_HDS_CONTEXT *pcontextHDS,
    IN       DRM_BOOL         fTruncateExist,
    IN const DRM_DWORD        nInitSizeinKB);


/**********************************************************************
** Function:    DRM_HDS_OpenStore
** Synopsis:    Initialize the data store at szDataStoreName
** Arguments:   [wszHdsName] -- 
**              [pcontextHDS] --
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_OpenStore( 
    IN const DRM_WCHAR       *wszHdsName, 
    OUT      DRM_HDS_CONTEXT *pcontextHDS );      /* user given context buffer, NULL to get size */    

/**********************************************************************
** Function:    DRM_HDS_OpenStore
** Synopsis:    Initialize the data store at szDataStoreName
** Arguments:   [wszHdsName] -- 
**              [pcontextHDS] --
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_OpenStore2( 
    IN const DRM_WCHAR       *wszHdsName, 
       OUT   DRM_HDS_CONTEXT *pcontextHDS,       /* user given context buffer, NULL to get size */    
    IN const DRM_DWORD        nGrowbySizeinKB);

/**********************************************************************
** Function:    DRM_HDS_CloseStore
** Synopsis:    Close the license store.  This will also Commit before close
** Arguments:   [pcontextHDS] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_CloseStore( 
    IN DRM_HDS_CONTEXT *pcontextHDS );

/**********************************************************************
** Function:    DRM_HDS_CommitStore
** Synopsis:    flush unwritten data to disk
** Arguments:   [pcontextHDS] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_CommitStore(
    IN DRM_HDS_CONTEXT *pcontextHDS );


/**********************************************************************
** Function:    DRM_HDS_CleanupStore
** Synopsis:    clean up empty blocks and hidden slot in store 
** Arguments:   [pcontextHDS] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_CleanupStore(
    IN DRM_HDS_CONTEXT *pcontextHDS,
    IN DRM_BOOL         fWait);

DRM_RESULT DRM_API DRM_HDS_OpenNamespace(
    IN       DRM_HDS_CONTEXT           *pcontextHDS,    
    IN const DRM_HDS_NAMESPACE         *pNamespace,
    IN       DRM_HDS_OPENMODE           eOpenMode,
    IN       DRM_WORD                   wMaxNumChildren,
    OUT      DRM_HDS_NAMESPACE_CONTEXT *pcontextNS);

/* remove the entire namespace tree and its nodes in the data store */
DRM_RESULT DRM_API DRM_HDS_DeleteNamespace( 
    IN       DRM_HDS_CONTEXT   *pcontextHDS,
    IN const DRM_HDS_NAMESPACE *pNamespace,
    IN       DRM_BOOL           fWait);

/* start using the specified namespace, any subsequent calls will be based on this namespace */
DRM_RESULT DRM_API DRM_HDS_CloseNamespace(
    IN DRM_HDS_NAMESPACE_CONTEXT *pcontextNS);  /* Namespace context returned from DRM_HDS_OpenNamespace */


DRM_RESULT DRM_API DRM_HDS_CommitNamespace(
    IN DRM_HDS_NAMESPACE_CONTEXT *pcontextNS);

/*
** ----------------------------------------------------------------------------
** Methods to navigate, create, delete, open and close HDS dataslots  
** ----------------------------------------------------------------------------
*/

/* Initialize a slot hint based on the slot context passed */
DRM_RESULT DRM_API DRM_HDS_MakeSlotHint(
    IN     DRM_HDS_SLOT_CONTEXT      *pcontextSlot,
    IN OUT DRM_HDS_SLOT_HINT         *pslotHint);

/* Create or open dataslot using given key1 and key2 */
DRM_RESULT DRM_API DRM_HDS_OpenSlot( 
    IN       DRM_HDS_NAMESPACE_CONTEXT *pcontextNS,   /* Namespace context returned from DRM_HDS_OpenNamespace */
    IN       DRM_DWORD                  dwMode,       /* combo of DRM_HDS_OPENMODE and DRM_HDSSLOT_LOCKMODE */
    IN const DRM_HDS_HASHKEY           *pHashKey, 
    IN const DRM_HDS_UNIQUEKEY         *pUniqueKey,
    IN       DRM_HDS_SLOT_HINT         *pSlotHint,
    IN OUT   DRM_DWORD                 *pcbSlotSize,  /* current dataslot size */
       OUT   DRM_HDS_SLOT_CONTEXT      *pcontextSlot);/* user given Dataslot context buffer */

/* Close an opened dataslot */
DRM_RESULT DRM_API DRM_HDS_CloseSlot( 
    IN DRM_HDS_SLOT_CONTEXT *pcontextSlot);

/* delete the slot immediately */
DRM_RESULT DRM_API DRM_HDS_DeleteSlot( 
    IN       DRM_HDS_NAMESPACE_CONTEXT *pcontextNS,    /* Namespace context returned from DRM_HDS_OpenNamespace */
    IN const DRM_HDS_HASHKEY           *pHashKey, 
    IN const DRM_HDS_UNIQUEKEY         *pUniqueKey,
    IN       DRM_HDS_SLOT_HINT         *pSlotHint,
    IN       DRM_BOOL                   fWait);


DRM_RESULT DRM_API DRM_HDS_Checkpoint( 
    IN OUT   DRM_HDS_CONTEXT *pcontextHDS,
    IN       DRM_VOID        *pvOpaqueData,
    IN       DRM_BOOL         fCreate);                /* TRUE to create, FALSE to verify */


/**********************************************************************
** Function:    DRM_HDS_InitSlotEnum
** Synopsis:    
** Arguments:   [pcontextNS]   -- Namespace context init'd by DRM_HDS_OpenNamespace()
**              [pHashKey]     -- Hashkey, if given to query, if not, enum all will be performed
**              [eLockMode]    -- Flag specifing how to lock the slot
**              [pcontextEnum] -- returned Enum context
** Returns:     DRM_SUCCESS on success
** Notes:       This is 2 functions in one. If pHashKey is given, only the slots
**              with the same hashkey will be enum'ed. if pHashKey is NULL, all
**              slots within the namespace will be enum'ed.
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_InitSlotEnum(
    IN       DRM_HDS_NAMESPACE_CONTEXT *pcontextNS,     /* Namespace context returned from DRM_HDS_OpenNamespace */
    IN const DRM_HDS_HASHKEY           *pHashKey,       /* if NULL, all slots will be enum'ed */
    IN       DRM_HDS_LOCKMODE           eLockMode,
       OUT   DRM_HDS_ENUM_CONTEXT      *pcontextEnum);


/**********************************************************************
** Function:    DRM_HDS_SlotEnumNext
** Synopsis:    Get the next slot in the enumeration
** Arguments:   [pcontextEnum] -- Enum Context init'd by DRM_HDS_InitSlotEnum()
**              [pcontextSlot] -- Slot Context of the current slot
**              [pHashKey] -- Hashkey of the current slot
**              [pUniqueKey] -- Uniquekey of the current slot
**              [pcbSlot] -- size of the current slot
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_SlotEnumNext( 
    IN     DRM_HDS_ENUM_CONTEXT *pcontextEnum,       /* setup by DRM_HDS_InitSlotEnum() */
       OUT DRM_HDS_SLOT_CONTEXT *pcontextSlot,       /* user given Dataslot context buffer */
       OUT DRM_HDS_HASHKEY      *pHashKey,           /* optional parameter */
       OUT DRM_HDS_UNIQUEKEY    *pUniqueKey,         /* optional parameter */
       OUT DRM_DWORD            *pcbSize);           /* current dataslot size, optional parameter */

/**********************************************************************
** Function:    DRM_HDS_SlotEnumReloadCurrent
** Synopsis:    Load the current slot in the enumeration
** Arguments:   [pcontextEnum] -- Enum Context init'd by DRM_HDS_InitSlotEnum()
**              [pcontextSlot] -- Slot Context of the current slot
**              [pHashKey] -- Hashkey of the current slot
**              [pUniqueKey] -- Uniquekey of the current slot
**              [pcbSlot] -- size of the current slot
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_SlotEnumReloadCurrent( 
    IN     DRM_HDS_ENUM_CONTEXT *pcontextEnum,       /* setup by DRM_HDS_InitSlotEnum() */
       OUT DRM_HDS_SLOT_CONTEXT *pcontextSlot,       /* user given Dataslot context buffer */
       OUT DRM_HDS_HASHKEY      *pHashKey,           /* optional parameter */
       OUT DRM_HDS_UNIQUEKEY    *pUniqueKey,         /* optional parameter */
       OUT DRM_DWORD            *pcbSize);           /* current dataslot size, optional parameter */

/**********************************************************************
** Function:    DRM_HDS_SlotEnumDeleteCurrent
** Synopsis:    Delete the current enumerated slot
** Arguments:   [pcontextEnum] -- Enum Context init'd by DRM_HDS_InitSlotEnum()
**              [pcontextSlot] -- Slot Context of the current slot
** Returns:     DRM_SUCCESS on success
** Notes:       The enumeration must be init'ed with EXCLUSIVE right.
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_SlotEnumDeleteCurrent( 
    IN  DRM_HDS_ENUM_CONTEXT *pcontextEnum,
    IN  DRM_HDS_SLOT_CONTEXT *pcontextSlot);

/* Block Scan API: rapid deletion of multiple slots */

DRM_RESULT DRM_API DRM_HDS_BlockScanDelete( 
    IN       DRM_HDS_NAMESPACE_CONTEXT *f_pcontextNS,
    IN const DRM_HDS_HASHKEY           *f_pkeyHash, 
    IN const DRM_HDS_UNIQUEKEY          f_rgkeyUnique [],
    IN const DRM_DWORD                  f_cKeysUnique,
    IN const DRM_BOOL                   f_fWait,
       OUT   DRM_HDS_BLOCKSCAN_CONTEXT *f_pcontextBlockScan);

/*
** ----------------------------------------------------------------------------
** Methods to read, write and seek within a dataslot
** ----------------------------------------------------------------------------
*/

/**********************************************************************
** Function:    DRM_HDS_SlotResize
** Synopsis:    resize an opened dataslot
** Arguments:   [pcontextSlot] -- An open slot context
**              [cbSize]       -- New size of the slot
** Returns:     DRM_SUCCESS on success
** Notes:       
** Possible cases:
** 
***********************************************************************/
DRM_RESULT DRM_API DRM_HDS_SlotResize( 
    IN DRM_HDS_SLOT_CONTEXT *pcontextSlot, 
    IN DRM_DWORD             cbSize);

DRM_RESULT DRM_API DRM_HDS_SlotSeek( 
    IN       DRM_HDS_SLOT_CONTEXT *pcontextSlot,
    IN const DRM_LONG              lOffset,
    IN const DRM_HDS_SEEKMODE      eOrigin,         /* as defined above */
       OUT   DRM_DWORD            *pwSeekPointer);  /* optional parameter, can be NULL */

DRM_RESULT DRM_API DRM_HDS_SlotRead( 
    IN     DRM_HDS_SLOT_CONTEXT *pcontextSlot,
    IN     DRM_DWORD             cbData,
       OUT DRM_BYTE             *pbData,
       OUT DRM_DWORD            *pcbRead);

DRM_RESULT DRM_API DRM_HDS_SlotWrite( 
    IN       DRM_HDS_SLOT_CONTEXT *pcontextSlot,
    IN       DRM_DWORD             cbData,
    IN const DRM_BYTE             *pbData,
       OUT   DRM_DWORD            *pcbWritten);


#ifdef __cplusplus
}
#endif

#endif  /* __DRM_HDS_H__ */
