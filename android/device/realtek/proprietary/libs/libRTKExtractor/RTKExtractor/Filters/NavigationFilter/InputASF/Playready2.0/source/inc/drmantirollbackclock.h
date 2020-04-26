/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMANTIROLLBACKCLOCK_H__
#define __DRMANTIROLLBACKCLOCK_H__

#include <drmcommon.h>
#include <drmliceval.h>
#include <drmlicstore.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_ARCLK_IsAntirollbackClockSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_ARCLK_IsAntirollbackClockUnsupported(DRM_VOID);

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_LEVL_OverrideMachineDateTimeWithSavedValue(
    __inout     DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    __out       DRM_UINT64          *f_pui64MachineDateTime ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_LEVL_GetMachineDatetime(
    IN OUT DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    OUT    DRM_UINT64          *f_pui64MachineDateTime ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_ARCLK_ASD_CheckRollback(
    IN OUT DRM_LICEVAL_CONTEXT *f_pContextLicEval );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_LEVL_DisableOnRollback(
    IN OUT DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    IN     DRM_XMR_LICENSE     *f_pXMR,
    OUT    DRM_BOOL            *f_pfDisabled ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_LEVL_SaveLicenseStoreTime(
    IN OUT DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    IN     DRM_XMR_LICENSE     *f_pXMR ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_LEVL_ReportClockRollback(
    IN OUT DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    IN     DRM_XMR_LICENSE     *f_pXMR ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_DOMST_GetDomainSavedDateTime(
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE           f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    __inout_opt                        DRM_VOID                *f_pOEMContext,
    IN                                 DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN                                 const DRM_GUID          *f_poAccountID,
    OUT                                DRM_UINT64              *f_puiDomainSavedDateTime ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_DOMST_ClockRollbackDetected(
    IN  DRM_BB_CONTEXT          *f_poBBXContext,
    IN  DRM_UINT64               ui64GlobalSavedTime ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_ARCLK_DOMST_CreateDomainStorePassword(
    IN  const DRM_GUID          *f_poAccountID,
    IN  DRM_BB_CONTEXT          *f_poBBXContext,
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)] ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_ARCLK_LIC_CheckClockRollback(  
    IN DRM_LICEVAL_CONTEXT         *f_pLicEval,
    IN DRM_LICSTORE_CONTEXT        *f_pLicStoreXML,
    IN DRM_LICSTORE_CONTEXT        *f_pLicStoreXMR,
    IN DRM_LICSTOREENUM_CONTEXT    *f_pLicEnum,
    IN DRM_BYTE                    *f_pbBuffer,
    IN DRM_DWORD                    f_cbBuffer,
    IN DRM_DST                     *f_pDatastore,
    IN DRM_STACK_ALLOCATOR_CONTEXT *f_poStack );

DRM_API DRM_RESULT DRM_CALL DRM_ARCLK_LIC_ResetClockRollback( 
    __in DRM_LICEVAL_CONTEXT       *f_pLicEval );

EXIT_PK_NAMESPACE;

#endif /*__DRMANTIROLLBACKCLOCK_H__ */

