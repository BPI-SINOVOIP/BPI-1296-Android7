/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CALLBACKS_H__
#define __DRM_CALLBACKS_H__

#include <drmnamespace.h>   /* defining proper namespace (if used) */
#include <drmxmr.h>

ENTER_PK_NAMESPACE;

typedef enum
{
    DRM_STORE_LICENSE_NOTIFY_KID       = 1,  /* Callbacks with this reason will have pvData as a DRM_CONST_STRING */
    DRM_STORE_LICENSE_STORE_V1_LICENSE = 2   /* Callbacks with this reason will have pvData as a DRM_ANSI_CONST_STRING */

#if DRM_SUPPORT_PLAYLIST_BURN
    , DRM_STORE_LICENSE_NOTIFY_PLAYLIST_BURN_UPDATE = 3 /* Callbacks with this reason will have pvData as a DRM_PLAYLIST_NOTIFICATION_STRUCT */
#endif
    , DRM_STORE_LICENSE_NOTIFY_INCLUSION_LIST = 4 /* Callbacks with this reason will have pvData as a DRM_INCLUSION_LIST_CALLBACK_STRUCT, note that dwChain will
                                                          always be 0 as when licenses are stored they are evaluated individually, not in a chain */

} DRM_STORE_LICENSE_CALLBACK_ENUM;

typedef DRM_RESULT (DRM_CALL *pfnLicenseResponseCallback)( 
    IN DRM_VOID                        *pvData, 
    IN DRM_STORE_LICENSE_CALLBACK_ENUM  eReason, 
    IN DRM_VOID                        *pvContext );

#if DRM_SUPPORT_PLAYLIST_BURN
typedef struct 
{
    const DRM_CONST_STRING *pdstrKID;
    DRM_DWORD               cPlaylistBurnIncrement;
} DRM_PLAYLIST_NOTIFICATION_STRUCT;
#endif



typedef enum
{
    DRM_PLAY_OPL_CALLBACK = 0x1,
    DRM_COPY_OPL_CALLBACK = 0x2,
    DRM_INCLUSION_LIST_CALLBACK = 0x3,
    DRM_EXTENDED_RESTRICTION_CONDITION_CALLBACK = 0x4,
    DRM_EXTENDED_RESTRICTION_ACTION_CALLBACK = 0x5,
    DRM_EXTENDED_RESTRICTION_QUERY_CALLBACK = 0x6,
    DRM_SECURE_STATE_TOKEN_RESOLVE_CALLBACK = 0x7
} DRM_POLICY_CALLBACK_TYPE;

typedef DRM_RESULT (DRM_CALL* DRMPFNPOLICYCALLBACK)( 
    IN const DRM_VOID                 *f_pvCallbackData,
    IN       DRM_POLICY_CALLBACK_TYPE  f_dwCallbackType,
    IN const DRM_VOID                 *f_pv );

typedef struct
{
    DRM_CONST_STRING *pdstrAttributeName;

    /* On input contains the token data that now exists on disk, on output the
    ** callback function should update it to the value that should be stored */
    TOKEN            *pOnDisk;

    /* Contains the data that was originally intended to be stored during Bind */
    TOKEN            *pOriginallyIntendedValue;

    /* If TRUE, indicates that during Bind the token didn't exist on disk 
    ** so another instance must have created this attribute as well */
    DRM_BOOL          fNewAttribute;

    /*
    ** Right ID and restriction ID the secure store entry is associated with
    */
    DRM_WORD          wRightID;
    DRM_WORD          wRestrictionID;
    
} DRM_SECURE_STATE_TOKEN_RESOLVE_DATA;

typedef struct
{
    DRM_WORD                             wRightID;
    DRM_XMR_UNKNOWN_OBJECT              *pRestriction;
    DRM_XMR_LICENSE                     *pXMRLicense;
    DRM_SECSTORE_CONTEXT                *pContextSST;
} DRM_EXTENDED_RESTRICTION_CALLBACK_STRUCT;

/*
**  When recieving this struct via a callback you can determine which GUIDs in the list are usabale by checking the 
**  rgfInclusionListValid[dwChainDepth] array to see which ones are TRUE.  Those that are true means that
**  rgInclusionList[x] is usable where rgfInclusionListValid[x][dwChainDepth] is TRUE.
*/

typedef struct __tagDRM_INCLUSION_LIST_CALLBACK_STRUCT
{
    DRM_GUID  rgInclusionList       [DRM_MAX_INCLUSION_GUIDS];
    DRM_BOOL  rgfInclusionListValid [DRM_MAX_INCLUSION_GUIDS][DRM_MAX_LICENSE_CHAIN_DEPTH];
    DRM_DWORD dwChainDepth;
} DRM_INCLUSION_LIST_CALLBACK_STRUCT;

EXIT_PK_NAMESPACE;

#endif
