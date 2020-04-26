/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMPLATFORM_H__
#define __OEMPLATFORM_H__

ENTER_PK_NAMESPACE;

/* OEM critical sectiion function. */
#define OEM_CRITICAL_SECTION_SIZE     24

typedef struct __tagOEM_CRITICAL_SECTION
{
    DRM_BYTE rgb[__CB_DECL(OEM_CRITICAL_SECTION_SIZE)];
} OEM_CRITICAL_SECTION;

/**********************************************************************
** Function:    Oem_CritSec_Initialize
** Synopsis:    Initializes critical section. 
** Arguments:   [f_pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              initialized.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_CritSec_Initialize(
    __inout OEM_CRITICAL_SECTION *f_pCS );

/**********************************************************************
** Function:    Oem_CritSec_Delete
** Synopsis:    Deletes existing critical section. 
** Arguments:   [f_pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              deleted.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_CritSec_Delete(
    __inout OEM_CRITICAL_SECTION *f_pCS );

/**********************************************************************
** Function:    Oem_CritSec_Enter
** Synopsis:    Enters critical section. 
** Arguments:   [f_pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              entered.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_CritSec_Enter(
    __inout OEM_CRITICAL_SECTION *f_pCS );

/**********************************************************************
** Function:    Oem_CritSec_Leave
** Synopsis:    Leaves critical section. 
** Arguments:   [f_pCS]--Pointer to OEM_CRITICAL_SECTION structure to be
**              left.
***********************************************************************/
DRM_API DRM_VOID DRM_CALL Oem_CritSec_Leave(
    __inout OEM_CRITICAL_SECTION *f_pCS );

EXIT_PK_NAMESPACE;

#endif  /* __OEMPLATFORM_H__ */
