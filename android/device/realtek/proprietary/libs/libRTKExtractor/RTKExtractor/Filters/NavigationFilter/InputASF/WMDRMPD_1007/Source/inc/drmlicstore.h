/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRM_LICSTORE_H__
#define __DRM_LICSTORE_H__

#include "drmhds.h"

#ifdef __cplusplus
extern "C" {
#endif


/* License store context size */
#define MAX_PRILIC_LIST     10


/*
** License store context stucture. This contains the HDS namespace context it is using and
** the local stack info for internal use
*/
typedef struct _tag_LicContext
{    
    DRM_HDS_SLOT_CONTEXT oSlotContext;
    DRM_HDS_NAMESPACE_CONTEXT oNamespaceContext;
    DRM_HDS_CONTEXT *pDeviceStoreContext;
    DRM_BOOL fInited;
    /* Extra 2 bytes of alignment here */
} _LicContext;

/*
**
*/
typedef enum _tag_EnumMode
{
    eLicEnumNone=0,       /* no license found */
    eLicEnumPrioritized=1, /* a local list of license's KID and LID is loaded for query */
    eLicEnumFiltered,     /* enum for licenses matching given KID */
    eLicEnumNatural,          /* enum all licenses */
    eLicEnumPostPrioritized   /* enum for licenses matching given KID after eLicEnumPrioritized exhausted */
} _EnumMode;

/*
**
*/
typedef struct _tag_PrioritizedLicense
{
    DRM_DWORD dwPriority;   /* the higher the value, the higher the priority */
    DRM_LID oLID;
    DRM_BOOL fDelete;
    DRM_HDS_SLOT_HINT slotHint;
} _PrioritizedLicense;

typedef struct _tag_PrioritizedList 
{
    DRM_KID oKID;
    DRM_DWORD nNextLicIndex;
    DRM_DWORD nNumLicenses; /* # of license slot used in the array */
    _PrioritizedLicense pPrioritizedList[MAX_PRILIC_LIST+1]; /* one extra for working buffer */
} _PrioritizedList;

typedef struct _tag_NonPriorityList
{
    DRM_BOOL fCurrIsValid;
    DRM_KID oCurrKID;
    DRM_LID oCurrLID;
} _NonPriorityList;

/*
**
*/
typedef struct _tag_LicEnumContext
{
    _LicContext *pLS;
    DRM_HDS_ENUM_CONTEXT oHdsEnumContext;
    DRM_HDS_SLOT_CONTEXT oSlotContext;
    DRM_BOOL fInited;
    
    _EnumMode eMode;
    struct
    {
        _PrioritizedList oPrioritized;
        _NonPriorityList oNonPrioritized;
    } List;
} _LicEnumContext;

typedef _LicContext     DRM_LICSTORE_CONTEXT;
typedef _LicEnumContext DRM_LICSTOREENUM_CONTEXT;


/* 
** API Functions 
*/

/**********************************************************************
** Function:    DRM_LST_Open
** Synopsis:    Initialize the license store at szDataStoreName
** Arguments:   [pbLicContext] -- user given licstore context buffer
**              [pcbLicContext] -- pointer to the size of pbLicContext
** Returns:     
** Notes:
***********************************************************************/
DRM_RESULT DRM_API DRM_LST_Open( 
    OUT DRM_LICSTORE_CONTEXT *pbLicContext,
    IN  DRM_HDS_CONTEXT      *poHdsContext ); 


/**********************************************************************
** Function:    DRM_LST_Close
** Synopsis:    Close the license store.  This will also Commit before close
** Arguments:   [pbLicContext] -- user given licstore context buffer
** Returns:     
** Notes:
***********************************************************************/
DRM_RESULT DRM_API DRM_LST_Close( 
    IN DRM_LICSTORE_CONTEXT *pbLicContext);


/**********************************************************************
** Function:    DRM_LST_AddLicense
** Synopsis:    Add a license to the store 
** Arguments:   [pbLicContext] -- user given licstore context buffer
**              [cbLicense] -- size of license xml string in bytes.
**              [pbLicense] -- byte pointer of given license xml string
** Returns:     
** Notes:
***********************************************************************/
DRM_RESULT DRM_API DRM_LST_AddLicense(
    IN DRM_LICSTORE_CONTEXT *pbLicContext,
    IN DRM_DWORD             cbLicense,
    IN DRM_BYTE             *pbLicense,
    IN DRM_KID              *pkid,
    IN DRM_LID              *plid,
    IN DRM_DWORD             dwPriority );


/**********************************************************************
** Function:    DRM_LST_GetLicense
** Synopsis:    Get a license from the store using KID and LID
** Arguments:   [pbLicContext] -- user given licstore context buffer
**              [bKID] -- given license's KID
**              [bLID] -- given license's LID
**              [pbLicense] -- user given buffer to receive license
**              [pcbLicense] -- size of pbLicense in bytes
** Returns:     
** Notes:
***********************************************************************/
DRM_RESULT DRM_API DRM_LST_GetLicense( 
    IN     DRM_LICSTORE_CONTEXT *pbLicContext,
    IN     DRM_KID              *pKID,      /* key to use for the query */
    IN     DRM_LID              *pLID, 
    IN     DRM_HDS_SLOT_HINT    *pSlotHint,
       OUT DRM_BYTE             *pbLicense, 
    IN OUT DRM_DWORD            *pcbLicense);

DRM_RESULT DRM_API DRM_LST_DeleteLicense(
    DRM_LICSTORE_CONTEXT *pcontextLS,
    DRM_KID              *pkid,
    DRM_LID              *plid, 
    DRM_HDS_SLOT_HINT    *pslotHint );

/**********************************************************************
** Function:    DRM_LST_InitEnum
** Synopsis:    un-indexed enumeration of the license store
** Arguments:   [pbLicContext] -- user given licstore context buffer
**              [pKID] -- KID filter to be used. if not specified, all license will be enum'ed
**              [fPrioritized] -- Ignored if pKID is NULL. TRUE to prioritize returned licenses 
**                           with highest priority first. FALSE to enum in nature order.
**              [pLicEnumContext] -- user given Enumeration context
** Returns:     
** Notes:
***********************************************************************/
DRM_RESULT DRM_API DRM_LST_InitEnum(
    IN  DRM_LICSTORE_CONTEXT     *pbLicContext, 
    IN  DRM_KID                  *pKID,           /* if NULL, all licenses will be enum'ed */
    IN  DRM_BOOL                  fPrioritized,   /* ignored if pKID is NULL */
    OUT DRM_LICSTOREENUM_CONTEXT *pLicEnumContext); 

DRM_RESULT DRM_API DRM_LST_EnumNext( 
    IN DRM_LICSTOREENUM_CONTEXT *pLicEnumContext,    /* setup by DRM_LST_InitEnum() */
    OUT DRM_KID                 *pKID,               /* if priortized or pKID is given, this is optional */
    OUT DRM_LID                 *pLID,
    OUT DRM_HDS_SLOT_HINT       *pSlotHint,
    OUT DRM_DWORD               *pcbLicense);        /* License size */

/* delete current enumerated license */
DRM_RESULT DRM_API DRM_LST_EnumDelete( 
    IN DRM_LICSTOREENUM_CONTEXT *pLicEnumContext);

/*--------------------------------------------------------------------
** 
** Function :   DRM_LST_LicCount
** 
** Synopsis :   Counts the number of licenses in the store
** 
** Arguments :  [pLicEnumCtxt]: context setup by DRM_LST_InitEnum
**              [pcLicenses]: pointer to DWORD recieving number of
**                  licenses in store
** 
** Returns :    
** 
** Notes :      BUGBUG: Investigate whether it can be more efficient
**              by doing the enumeration within the HDS layer
** 
--------------------------------------------------------------------*/
DRM_RESULT DRM_API DRM_LST_LicCount( 
    IN  DRM_LICSTOREENUM_CONTEXT *pLicEnumCtxt, 
    OUT DRM_DWORD                *pcLicenses );

/* Callback function to report progress during License store cleanup */
typedef DRM_RESULT ( DRM_API *pfnStoreCleanupProgress )( 
    IN  const DRM_VOID    *pvCallerData, 
    IN        DRM_DWORD   cLicensesProcessed,
    IN        DRM_DWORD   cTotalLicenses  /*  # of licenses */
    );

/*--------------------------------------------------------------------
** 
** Function :   DRM_LST_Clean
** 
** Synopsis :   Cleanup the license store at szDataStoreName. It 
**              cleans up the license store by removing expired or 
**              unusable licenses, and defragmenting the license data 
**              store
** 
** Arguments :  [pbLicEvalCtxt] - Preinitialized License eval context
**              [pbLicStoreCtxt] - Preinitialized LicStore context
**              [pbBuffer] - Buffer supplied by user
**              [cbBuffer] - Size of pbBuffer
**              [pvCallerData] -- Opaque context data to be funnelled
**                  back to caller during callback. Could be NULL.
**              [dwCallbackInterval] -- % completion after which 
**                  Callback (if any) should be invoked. 
**                  e.g. if dwCallbackInterval = 5, callback will be 
**                  invoked after 5%, 10%, 15%...completion
**              [pfnCallback] -- optional caller supplied callback 
**                  function - can be NULL.

** Returns :    DRM_RESULT
** 
** Notes :      
** 
--------------------------------------------------------------------*/
DRM_RESULT DRM_API DRM_LST_Clean (
    IN       DRM_BYTE                *pbLicEvalCtxt,
    IN       DRM_BYTE                *pbLicStoreCtxt,
    IN       DRM_BYTE                *pbBuffer,
    IN       DRM_DWORD                cbBuffer,
    IN const DRM_VOID                *pvCallerData,
    IN       DRM_DWORD                dwCallbackInterval,
    IN       pfnStoreCleanupProgress  pfnCallback,
    IN       DRM_HDS_CONTEXT         *poHdsContext);



#ifdef __cplusplus
}
#endif

#endif  /* __DRM_LICSTORE_H__ */


