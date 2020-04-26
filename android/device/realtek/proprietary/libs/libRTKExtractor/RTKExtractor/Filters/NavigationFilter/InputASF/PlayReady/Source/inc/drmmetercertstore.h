/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_METERCERT_STORE__
#define __DRM_METERCERT_STORE__

ENTER_PK_NAMESPACE;

typedef struct _tagMeterCertContext
{    
    DRM_DST_SLOT_CONTEXT      contextDSTSlot;
    DRM_DST_NAMESPACE_CONTEXT contextNameSpace;
    DRM_DST                  *pDatastore;
    DRM_BYTE                 *pbBuffer;
    DRM_DWORD                 cbBuffer;
    DRM_BOOL                  fInited;
} DRM_METERCERT_CONTEXT_PRIVATE;

typedef struct _tagMeterCertEnumContext
{
    DRM_METERCERT_CONTEXT_PRIVATE *pcontextMeterCert;
    DRM_DST_ENUM_CONTEXT      contextDSTEnum;
    DRM_DST_SLOT_CONTEXT      contextDSTSlot;
    DRM_BOOL                  fInited;
    DRM_BOOL                  fAny;
    DRM_BOOL                  fSkipEnumNext;
} DRM_METERCERT_ENUM_PRIVATE;

typedef struct __tagDRM_METERCERT_CONTEXT
{
    DRM_BYTE rgbOpaqueData [__CB_DECL(SIZEOF(DRM_METERCERT_CONTEXT_PRIVATE) )];
} DRM_METERCERT_CONTEXT;

typedef struct __tagDRM_METERCERT_ENUM
{
    DRM_BYTE rgbOpaqueData [__CB_DECL(SIZEOF(DRM_METERCERT_ENUM_PRIVATE))];
} DRM_METERCERT_ENUM;

/* low-level access to the meter cert store */


/*********************************************************************
**
** Function: DRM_MCS_OpenStore
**
** Synopsis: Opens the metercert store
**
** Arguments:
**
** [f_pDataStore]           -- initialized data store.
** [f_pbBuffer]             -- Scratch buffer to be used by the store
** [f_cbBuffer]             -- Size of the scratch buffer, should be at least as big as the
**                             largest anticipated metercert
** [f_pcontextMeterCert]    -- returned meter cert context
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_OpenStore  (
    IN  DRM_DST                 *f_pDataStore,
    __in_bcount( f_cbBuffer ) DRM_BYTE *f_pbBuffer,
    IN  DRM_DWORD                f_cbBuffer,
    OUT DRM_METERCERT_CONTEXT   *f_pcontextMeterCert);

/*********************************************************************
**
** Function: DRM_MCS_CloseStore
**
** Synopsis: Closes the metercert store
**
** Arguments:
**
** [f_pcontextMeterCert]    -- initialized meter cert context
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_CloseStore (
    IN  DRM_METERCERT_CONTEXT *f_pcontextMeterCert);


/**********************************************************************
**
** Function:    DRM_MCS_InitEnum
**
** Synopsis:    Initializes an enumeration of metercert store entries
**              
**
** Arguments:   [pDrmContext]             -- Opaque DRM Manager context initialized by a call to Drm_Initialize
**              [f_pcontextMeterCertEnum] -- Opaque context initialized
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_InitEnum   (
    IN  DRM_METERCERT_CONTEXT       *f_pcontextMeterCert, 
    OUT DRM_METERCERT_ENUM          *f_pcontextMeterCertEnum);

/**********************************************************************
**
** Function:    DRM_MCS_EnumNext
**
** Synopsis:    Enumerates the next entry in the meter cert store
**              
**
** Arguments:   [f_pcontextMeterCertEnum] -- Opaque context initialized by a call to DRM_MCS_InitEnum
**              [f_pmid]                  -- pointer to the MID of the enumerated entry
**              [f_pslotHint]             -- slot hint of the enumerated entry
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_EnumNext   (
    IN  DRM_METERCERT_ENUM          *f_pcontextMeterCertEnum, 
    OUT DRM_MID                     *f_pmid,
    OUT DRM_DST_SLOT_HINT           *f_pslotHint);

/*********************************************************************
**
** Function: DRM_MCS_GetMeterCertByMID
**
** Synopsis: Looks up the LAINFO and meter cert for a given MID if it exists
**
** Arguments:
**
** [f_pcontextMeterCert]    -- initialized meter cert context
** [f_pmid]                 -- pointer to MID to be looked up
** [f_pwszLAINFO]           -- pointer to a buffer that will hold the LAINFO
** [f_pcchLAINFO]           -- pointer to the number of wide characters in the LAINFO
** [f_pfIsV2]               -- specifies whether the LAINFO is a V2 server or V4
** [f_pwszMeterCert]        -- pointer to a buffer that will hold the meter cert
** [f_pcchMeterCert]        -- pointer to the number of wide characters in the meter cert
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_GetMeterCertByMID(
    IN                                   DRM_METERCERT_CONTEXT   *f_pcontextMeterCert, 
    IN const                             DRM_MID                 *f_pmid,
       __out_ecount_opt(*f_pcchLAINFO)   DRM_WCHAR               *f_pwszLAINFO,
    IN OUT                               DRM_DWORD               *f_pcchLAINFO,
       OUT                               DRM_BOOL                *f_pfIsV2,
       __out_bcount_opt(*f_pcbMeterCert) DRM_BYTE                *f_pbMeterCert,
    IN OUT                               DRM_DWORD               *f_pcbMeterCert );

/*********************************************************************
**
** Function: DRM_MCS_UpdateMeterCert
**
** Synopsis: 
**
** Arguments:
**
** [f_pcontextMeterCert]          -- initialized meter cert context
** [f_pdstrLAINFO]                -- optional LAINFO to store
** [f_pfIsV2]                     -- specifies whether the LAINFO is a V2 server or V4
** [f_pbMeterCert]                -- optional meter cert to store
** [f_cbMeterCert]                -- optional size of meter cert to store
** [f_pmid]                       -- pointer to MID to be added/updated
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/


DRM_API DRM_RESULT DRM_CALL DRM_MCS_UpdateMeterCert(
    IN       DRM_METERCERT_CONTEXT      *f_pcontextMeterCert, 
    IN       DRM_CONST_STRING           *f_pdstrLAINFO,
    IN       DRM_BOOL                   *f_pfIsV2,
    __in_bcount_opt( f_cbMeterCert )
             DRM_BYTE                   *f_pbMeterCert,
    __in     DRM_DWORD                   f_cbMeterCert,
    IN const DRM_MID                    *f_pmid);


/*********************************************************************
**
** Function: DRM_MCS_DeleteMeterCert
**
** Synopsis: delete the indicated meter cert from the meter cert store; its absense
**           is not an error
**
** Arguments:
**
** [f_pcontextMeterCert]    -- initialized METERCERT context
** [f_pmid]                 -- pointer to MID to be added/updated
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_DeleteMeterCert(
    IN       DRM_METERCERT_CONTEXT  *f_pcontextMeterCert, 
    IN const DRM_MID                *f_pmid);


/*********************************************************************
**
** Function: DRM_MCS_InvalidateMeterCert
**
** Synopsis: invalidate the indicated meter cert from the meter cert store; its absense
**           is not an error
**
** Arguments:
**
** [f_pcontextMeterCert]    -- initialized METERCERT context
** [f_pmid]                 -- pointer to MID to be added/updated
**
** Returns:     DRM_SUCCESS on success or DRM_E_INVALIDARG if the arguments are invalid.
**
**********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_MCS_InvalidateMeterCert(
    IN       DRM_METERCERT_CONTEXT  *f_pcontextMeterCert, 
    IN const DRM_MID                *f_pmid);

EXIT_PK_NAMESPACE;

#endif /* __DRM_SYNC_STORE__ */
