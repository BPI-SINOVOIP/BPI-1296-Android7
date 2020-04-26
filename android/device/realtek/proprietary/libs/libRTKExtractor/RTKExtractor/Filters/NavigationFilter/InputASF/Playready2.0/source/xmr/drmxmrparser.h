/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_XMR_PARSER_H__
#define __DRM_XMR_PARSER_H__

#include <drmxmr.h>

ENTER_PK_NAMESPACE;

/*
** There is a finite set of objects which get allocated using
** DRM_STK_Alloc in the XMR parser which can potentially be larger
** than the original raw binary license data they contain.
**
** The smallest size of an XMR object is XMR_BASE_OBJECT_LENGTH.
**
** Therefore, the worst-case license, which contains nothing but
** instances of the highest-bloat object which needs to be allocated
** using DRM_STK_Alloc, will require this many bytes:
**  DRM_XMR_COMPUTED_MAX_UNPACK_ALLOC_FACTOR * sizeof(raw XMR license)
**
** In effect, that constant equals:
**  (Space needed for the Highest-Bloat object)
**    / (Smallest size that object could be in the raw XMR license)
**
** However, DRM_XMR_MAX_UNPACK_ALLOC_FACTOR is what should primarily
** be used by callers of DRM_XMR_UnpackLicense because that will
** ensure consistent allocation behavior on on architectures.
** Note: Inside DRM_XMR_UnpackLicense, there is a compile-time assert
** to validate that:
**     DRM_XMR_MAX_UNPACK_ALLOC_FACTOR
**  >= DRM_XMR_COMPUTED_MAX_UNPACK_ALLOC_FACTOR
**
** Therefore, callers of DRM_XMR_UnpackLicense that don't want
** to worry about potentially running out of space should ensure
** that f_pStack contains at least this many bytes:
**  (DRM_XMR_MAX_UNPACK_ALLOC_FACTOR * f_cbLicense)
**
** Additional notes about DRM_XMR_COMPUTED_MAX_UNPACK_ALLOC_FACTOR:
**  The extra "+ ( SIZEOF(DRM_DWORD_PTR) - 1 )" is due to the
**   fact that the memory allocation may need to be aligned.
**  The extra "+ 1" is due to the fact that the division will
**   round down.
*/

#define DRM_XMR_COMPUTED_MAX_UNPACK_ALLOC_FACTOR                                        \
    ( ( ( MAX(                                                                          \
              MAX(                                                                      \
                  MAX( SIZEOF(DRM_XMR_UNKNOWN_CONTAINER),                               \
                       SIZEOF(DRM_XMR_UNKNOWN_OBJECT) ),                                \
                  MAX( SIZEOF(DRM_XMR_VIDEO_OUTPUT_CONFIGURATION_LIST),                 \
                       SIZEOF(DRM_XMR_AUDIO_OUTPUT_CONFIGURATION_LIST) ) ),             \
              SIZEOF( DRM_XMR_POLICY_METADATA_LIST ) )                                  \
        + ( SIZEOF(DRM_DWORD_PTR) - 1 ) )                                               \
    / XMR_BASE_OBJECT_LENGTH ) + 1 )

#define DRM_XMR_MAX_UNPACK_ALLOC_FACTOR 7

DRM_API DRM_RESULT DRM_CALL DRM_XMR_UnpackLicense(
    __in_bcount( f_cbLicense ) const DRM_BYTE                    *f_pbLicense,
    __in                             DRM_DWORD                    f_cbLicense,
    __in                             DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    __out                            DRM_XMR_LICENSE             *f_pXmrLicense );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_GetContainer(
    IN  const   DRM_BYTE                     *f_pbLicense,
    IN          DRM_DWORD                     f_cbLicense,
    IN          DRM_WORD                      f_wContainerType,
    IN          DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack,
    IN  OUT     DRM_VOID                     *f_pvContainer );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_PopulateInclusionList(
    __in                                    DRM_XMR_INCLUSION_LIST *f_pInclusionList,
    __inout_ecount(DRM_MAX_INCLUSION_GUIDS) DRM_GUID                f_rgInclusionList[DRM_MAX_INCLUSION_GUIDS],
    __inout_ecount(DRM_MAX_INCLUSION_GUIDS) DRM_BOOL                f_rgfGuidValid[DRM_MAX_INCLUSION_GUIDS][DRM_MAX_LICENSE_CHAIN_DEPTH],
    __in                                    DRM_DWORD               f_dwChainDepth );

/******************************************************************************
**
** Function :   DRM_XMR_ValidateLicenseObjects
**
** Synopsis :   Determines if the objects in the given license conform
**                  to the XMR specification based on the license's version
**
** Arguments :
**      [f_pXmrLicense]   : License to validate
**
** Returns :    DRM_SUCCESS             if the license is valid
**              DRM_E_INVALIDLICENSE    if the license is not valid
**              DRM_E_INVALIDARG        if f_pXmrLicense is NULL
**
******************************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_XMR_ValidateLicenseObjects(
    IN          DRM_XMR_LICENSE             *f_pXmrLicense );

/******************************************************************************
**
** Function :   DRM_XMR_PARSE
**
** Synopsis :   Function Prototype to parse a serialized object buffer
**
** Arguments :
**      [f_pStack]   : Stack for intermediate usage
**      [f_pbBuffer] : Object Buffer
**      [f_iObject]  : Index marking beginning of object in the buffer
**      [f_cbObject] : Size of serialized object
**      [f_pvObject] : Actual object returned as void
**
** Returns :
**
** Notes :      Callers must keep the stack around as long as they use the
**              contents of the structure returned. The structure might have
**              some intermediate nodes allocated out of this stack
**
******************************************************************************/
typedef DRM_RESULT  ( DRM_CALL *DRM_XMR_PARSE )(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_Invalid(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_MinimumEnvironment(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_SecurityLevel(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_OPLs(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_Empty(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_UnknownContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbLicense,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_UnknownObject(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_WORD(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_DWORD_Versioned(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_DWORD(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_GUID(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_GUIDLIST(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_BYTEARRAY(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_DomainID(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_VideoOutputConfig(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_AudioOutputConfig(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_ContentKey(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_Expiration(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_PlaylistBurnRights(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_RsaPubkey(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_EccDevicekey(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_UserID(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_UplinkKID(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_UplinkKID2(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_OuterContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_GlobalContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_PlaybackContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_ExplicitAnalogVideoOutputProtectionContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_ExplicitDigitalAudioOutputProtectionContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_PolicyMetadata(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbLicense,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_RevocationContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbLicense,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_KeyMaterialContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_Signature(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_CopyToPCContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE                    *f_pbLicense,
    IN          DRM_DWORD                    f_iObject,
    IN          DRM_DWORD                    f_cbObject,
    IN  OUT     DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_CopyContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_PlaylistBurnContainer(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_PlaylistBurnRights(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_OptimizedContentKey(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_RemovalDate(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_AuxKey(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );
    
DRM_API DRM_RESULT DRM_CALL DRM_XMR_Parse_UplinkX(
    IN          DRM_STACK_ALLOCATOR_CONTEXT *f_pStack,
    IN  const   DRM_BYTE    *f_pbBuffer,
    IN          DRM_DWORD    f_iObject,
    IN          DRM_DWORD    f_cbObject,
    IN  OUT     DRM_VOID    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_FindObject(
    __in                             DRM_WORD   f_wObjectType,
    __in_bcount( f_cbLicense ) const DRM_BYTE  *f_pbLicense,
    __in                             DRM_DWORD  f_cbLicense,
    __out                            DRM_DWORD *f_piObject,
    __out                            DRM_DWORD *f_pcbObject );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_UpdateLicenseSizeForSymmKey(
    __inout_bcount( f_cbLicense )     DRM_BYTE                      *f_pbLicense,
    __in                              DRM_DWORD                      f_cbLicense,
    __inout_ecount( 1 )               DRM_DWORD                     *f_pcbLicenseUsed,
    __in                              DRM_DWORD                     *f_pdwInsertionPoint );

DRM_API DRM_RESULT DRM_CALL DRM_XMR_RequiresSST(
    __in     DRM_XMR_LICENSE *f_pXmrLicense,
    __out    DRM_BOOL        *f_pfRequiresSST );

typedef struct
{
    DRM_XMR_PARSE           fnParse;
    DRM_BOOL                rgfIsVersionSupported[XMR_VERSION_MAX];
} XMR_PARSER_INTERNAL_DEFINES;

EXIT_PK_NAMESPACE;

#endif  /* __DRM_XMR_PARSER_H__ */

