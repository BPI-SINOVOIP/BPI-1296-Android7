/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_XMR_BUILDER_H__
#define __DRM_XMR_BUILDER_H__

#include <drmxmr.h>

ENTER_PK_NAMESPACE;

#define XMR_AES_OMAC_SIGNATURE_LENGTH     16
#define XMR_SHA256_HMAC_SIGNATURE_LENGTH  32
#define XMR_XPOLICY_HEADER_LEN 8

typedef struct __tagDRM_XMR_BUILDER_NODE
{
    DRM_WORD     wType;
    DRM_DWORD    cbLength;
    /*
    ** This can either be an object or a linked list of
    ** builder nodes depending on whether this is a
    ** leaf node or not
    */
    const DRM_VOID *pvObject;
} DRM_XMR_BUILDER_NODE;

typedef struct __tagDRM_XMR_BUILDER_LISTNODE
{
    DRM_XMR_BUILDER_NODE                  Node;
    struct __tagDRM_XMR_BUILDER_LISTNODE *pNext;
} DRM_XMR_BUILDER_LISTNODE;

typedef struct __tagDRM_XMR_BUILDER_UNKNOWNOBJECT_LISTNODE
{
    DRM_XMR_BUILDER_LISTNODE listNode;
    DRM_BOOL                 fDuplicateAllowed;
    DRM_WORD                 wFlags;
    DRM_WORD                 wParent;
    struct __tagDRM_XMR_BUILDER_UNKNOWNOBJECT_LISTNODE     *pNext;

} DRM_XMR_BUILDER_UNKNOWNOBJECT_LISTNODE;

typedef struct __tagDRM_XMR_BUILDER_CONTEXT_INTERNAL
{
    const DRM_ID                             *pidRights;
    DRM_DWORD                                 dwVersion;
    /*  In case of nodes which can have duplicates, the array below will hold the
    **  pointer to the last node of that type. In those cases, it is just used
    **  to keep track of the types of nodes added
    */
    DRM_XMR_BUILDER_NODE                    **rgpObjectNodes;
    DRM_XMR_BUILDER_UNKNOWNOBJECT_LISTNODE   *pUnknownObjects;
    DRM_STACK_ALLOCATOR_CONTEXT               contextStack;
} DRM_XMR_BUILDER_CONTEXT_INTERNAL;

#define DRM_XMR_BUILDER_CONTEXT_BUFFER_SIZE SIZEOF( DRM_XMR_BUILDER_CONTEXT_INTERNAL )

typedef struct __tagDRM_XMR_BUILDER_CONTEXT
{
    /*
    ** This data is Opaque.  Do not set any value in it.
    */
    DRM_BYTE rgbOpaqueBuffer[__CB_DECL( DRM_XMR_BUILDER_CONTEXT_BUFFER_SIZE )];
} DRM_XMR_BUILDER_CONTEXT;

DRM_API DRM_RESULT DRM_CALL DRM_XMR_StartLicense(
    __inout_bcount( f_cbStack ) DRM_BYTE                *f_pbStack,
    __in                        DRM_DWORD                f_cbStack,
    __in_ecount( 1 )      const DRM_ID                  *f_pidRights,
    __in                        DRM_DWORD                f_dwVersion,
    __inout_ecount( 1 )         DRM_XMR_BUILDER_CONTEXT *f_pcontextBuilder );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_InitializeObject(
    __in                DRM_WORD    f_wObjectType,
    __inout_ecount( 1 ) DRM_VOID   *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_AddObject(
    __inout_ecount( 1 )    DRM_XMR_BUILDER_CONTEXT *f_contextBuilder,
    __in                   DRM_WORD                 f_wObjectType,
    __in_ecount( 1 ) const DRM_VOID                *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_AddUnknownObject(
    __inout_ecount( 1 )                 DRM_XMR_BUILDER_CONTEXT *f_pcontextBuilder,
    __in                                DRM_WORD                 f_wObjectType,
    __in                                DRM_BOOL                 f_fDuplicateAllowed,
    __in                                DRM_WORD                 f_wParent,
    __in                                DRM_WORD                 f_wFlags,
    __in_bcount_opt( f_cbObject ) const DRM_BYTE                *f_pbObject,
    __in                                DRM_DWORD                f_cbObject );

DRM_API DRM_RESULT DRM_CALL  DRM_XMR_AddUnknownHierarchy (
    __inout_ecount( 1 )  DRM_XMR_BUILDER_CONTEXT_INTERNAL        *f_pcontextBuilder,
    __inout_ecount( 1 )  DRM_XMR_BUILDER_UNKNOWNOBJECT_LISTNODE  *f_plistnode );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_CreateExtensibleRestriction(
    __in_ecount( 1 )  const                   DRM_XMR_BUILDER_CONTEXT *f_pcontextBuilder,
    __in_bcount( f_wPolicyLen )               DRM_BYTE                *f_pbPolicy,
    __in                                      DRM_WORD                 f_wPolicyLen,
    __in                                      DRM_DWORD                f_dwSecureDataSize,
    __in                                      DRM_BYTE                 f_bStateType,
    __in_bcount( f_dwStateLen )               void                    *f_pvState,
    __in                                      DRM_WORD                 f_dwStateLen,
    __deref_inout_bcount( *f_cbRestriction )  DRM_BYTE               **f_ppbRestriction,
    __inout_ecount( 1 )                       DRM_DWORD               *f_cbRestriction );

/*
** Filters objects which are NOT must understand AND have a minimum
** version HIGHER than the specified verion
*/
DRM_API DRM_RESULT DRM_CALL DRM_XMR_FilterNonMustUnderstandObjectsByVersion(
    __in_ecount( 1 )       DRM_XMR_LICENSE  *f_pXmrLicense,
    __in                   DRM_DWORD         f_dwVersion );

/******************************************************************************
**
** Function :   DRM_XMR_CB_SIGNATURE
**
** Synopsis :   Callback function provided by caller of XMR Builder APIs to
**              create a signature using the integrity key. Integrity key is not
**              passed to the builder APIs for security reasons. In the client,
**              the clear integrity key should stay within the blackbox.
**
** Arguments :  f_pvSignatureContext  - Context needed by caller to retrieve
**                                      appropriate integrity key
**              f_pbData              - Data to be signed
**              f_cbData              - Lentgth of pbData
**              f_pbSignatureBuffer   - Buffer in which to put the signature
**              f_cbSignatureBuffer   - Length of signature buffer
**              f_ibSignatureBuffer   - Index in buffer at which signature should
**                                      be put
**
** Returns :
**
** Notes :
**
******************************************************************************/
typedef DRM_RESULT ( DRM_CALL *DRM_XMR_CB_SIGNATURE )(
    __in                                const DRM_VOID  *f_pvSignatureContext,
    __in_bcount( f_cbData )             const DRM_BYTE  *f_pbData,
    __in                                const DRM_DWORD  f_cbData,
    __out_bcount( f_cbSignatureBuffer )       DRM_BYTE  *f_pbSignatureBuffer,
    __in                                const DRM_DWORD  f_cbSignatureBuffer,
    __in                                const DRM_DWORD  f_ibSignatureBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_FinishLicense (
    __in_ecount( 1 ) const              DRM_XMR_BUILDER_CONTEXT *f_pcontextBuilder,
    __in_ecount( 1 ) const              DRM_VOID                *f_pvSignatureContext,
    __in                                DRM_XMR_CB_SIGNATURE     f_pfnSignature,
    __inout_bcount_opt( *f_pcbLicense ) DRM_BYTE                *f_pbLicense,
    __inout_ecount( 1 )                 DRM_DWORD               *f_pcbLicense );

/******************************************************************************
**
** Function :   DRM_XMR_SERIALIZE
**
** Synopsis :   Function Prototype to serialize the object ( sans the base ). The
**              base is assumed to have been serialized already.
**
** Arguments :
**      [f_pvObjectActual] : Object passed as void
**      [f_pbBuffer]       : Buffer in which to put the serialized object
**      [f_piBuffer]       : Index of buffer from where to start
**
** Returns :
**
** Notes :      It is assumed that the buffer has enough space to hold the
**              entire object. This is ensured because of the buffer calculation
**              before serialization begins
**
******************************************************************************/
typedef DRM_RESULT  ( DRM_CALL *DRM_XMR_SERIALIZE )(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

/******************************************************************************
**
** Function :   DRM_XMR_SET_DEFAULTS
**
** Synopsis :   Function Prototype to set the defaults in the object
**
** Arguments :
**      [f_pvObject] : Actual object passed as void
**
******************************************************************************/
typedef DRM_RESULT ( DRM_CALL *DRM_XMR_SET_DEFAULTS )( __in_ecount( 1 ) const DRM_VOID *f_pvObject );

/******************************************************************************
**
** Function :   DRM_XMR_PARSE_STRUCTURE
**
** Synopsis :   Function Prototype to parse the structure of a container and add
**              the children to the builder context
**
** Arguments :  f_pcontextBuilder - builder context
**              f_pvObject - Actual object passed as void
**
** Notes     :  This will add all the children of this container and also update
**              the length of f_pvObject
**
******************************************************************************/
typedef DRM_RESULT ( DRM_CALL *DRM_XMR_PARSE_STRUCTURE )(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_BOOL DRM_XMR_IsDuplicateAllowed( __in_ecount( 1 ) const DRM_XMR_BUILDER_LISTNODE *f_plistnode );

DRM_WORD DRM_XMR_GetParent( __in_ecount( 1 ) const DRM_XMR_BUILDER_LISTNODE *f_plistnode );

DRM_XMR_BUILDER_NODE* _DRM_XMR_GetBuilderNode(
    __in_ecount( 1 )  const DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in                    DRM_WORD                             f_wType );

DRM_API DRM_VOID DRM_CALL DRM_XMR_Serialize_BaseObject(
    __in                                                   DRM_WORD    f_wType,
    __in                                                   DRM_DWORD   f_dwLength,
    __inout_bcount( XMR_BASE_OBJECT_LENGTH + *f_piBuffer ) DRM_BYTE   *f_pbBuffer,
    __inout_ecount( 1 )                                    DRM_DWORD  *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_Container(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_DWORD DRM_CALL DRM_XMR_GetObjectLength(
    __in             DRM_WORD    f_wType,
    __in_opt const   DRM_VOID   *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_Invalid( __in_ecount( 1 ) const DRM_VOID *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_Invalid(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_Invalid(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_MinimumEnvironment(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_MinimumEnvironment(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_SecurityObject(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_OPLs(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_GenerationNumber(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_OPLs(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_Empty(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_WORD(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_WORD(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_DomainID(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_EccDevicekey(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_GenerationNumber(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_UnknownObject(
    __in_ecount( 1 )  const                    DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer )               DRM_BYTE    *f_pbBuffer,
    __in                                       DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )                        DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_DWORD(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_DWORD(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_DWORD_Versioned(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_GUID(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_GUID(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_GUIDLIST(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_GUIDLIST(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_BYTEARRAY(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_BYTEARRAY(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_VideoOutputConfig(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_AudioOutputConfig(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_VideoOutputConfig(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_AudioOutputConfig(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_PolicyMetadata(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_ContentKey(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_ContentKey(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_Expiration(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_Expiration(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_PlaylistBurnRights(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_PlaylistBurnRights(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_RsaPubkey(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_RsaPubkey(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_UserID(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_UserID(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_UplinkKID(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_UplinkKID(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_UplinkKID2(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Serialize_OptimizedContentKey(
    __in_ecount( 1 )  const      DRM_VOID    *f_pvObject,
    __inout_bcount( f_cbBuffer ) DRM_BYTE    *f_pbBuffer,
    __in                         DRM_DWORD    f_cbBuffer,
    __inout_ecount( 1 )          DRM_DWORD   *f_piBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_OuterContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_OuterContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_GlobalContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_GlobalContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_PlaybackContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_CopyToPCContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_PlaybackContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_ExplicitAnalogVideoOutputProtectionContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_ExplicitDigitalAudioOutputProtectionContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_PolicyMetadata(
    __in_ecount( 1 ) const  DRM_VOID  *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_ExplicitAnalogVideoOutputProtectionContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_ExplicitDigitalAudioOutputProtectionContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_PolicyMetadata(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_KeyMaterialContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_RevocationContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_KeyMaterialContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_CopyContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_CopyContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_PlaylistBurnContainer(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_PlaylistBurnContainer(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_PlaylistBurnRights(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_ParseStructure_PlaylistBurnRights(
    __inout_ecount( 1 )     DRM_XMR_BUILDER_CONTEXT_INTERNAL    *f_pcontextBuilder,
    __in_ecount( 1 ) const  DRM_VOID                            *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_NoOP(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_Rights(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_Issuedate(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_GracePeriod(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_Priority(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_IssueDate(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_ExpirationAfterFirstUse(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_ExpirationAfterFirstStore(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_Playcount(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_CopyProtectionLevel(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_CopyCount(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_SourceID(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_OptimizedContentKey(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_SetDefaults_EmbeddingBehavior(
    __in_ecount( 1 )  const   DRM_VOID    *f_pvObject );

typedef struct
{
    DRM_XMR_SET_DEFAULTS    fnSetDefaults;
    DRM_XMR_SERIALIZE       fnSerialize;
    DRM_XMR_PARSE_STRUCTURE fnParseStructure;
    DRM_BOOL                fDuplicateAllowed;
    DRM_BOOL                fIsOptional;
    DRM_DWORD               cbStructureSize;
} XMR_BUILDER_INTERNAL_DEFINES;

EXIT_PK_NAMESPACE;

#endif  /* __DRM_XMR_BUILDER_H__ */

