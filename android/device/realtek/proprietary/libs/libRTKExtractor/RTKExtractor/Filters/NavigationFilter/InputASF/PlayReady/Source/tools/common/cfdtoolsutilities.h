/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __CFDTOOLSUTILITIES_H
#define __CFDTOOLSUTILITIES_H

#include "drmcommon.h"
#include "drmutilities.h"
#include "drmtypes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

DRM_RESULT	DRM_API MSDB_UTL_LoadTestDataFile
(
     IN      DRM_WCHAR           *f_pwszDataFile,
     IN OUT  DRM_BYTE            **f_ppBuffer, 
     IN OUT  DRM_DWORD           *f_pSize 
);

DRM_RESULT DRM_API MSDB_UTL_SetLicensePolicy 
( 
    IN   DRM_CONST_STRING     f_dstrLicPolicyConfig,
    IN   DRM_BOOL             f_useRelativeTime,
    OUT  DRM_LICENSE_POLICY  *f_pLicPolicy
);

DRM_RESULT DRM_API MSDB_UTL_AddTime
(
 IN     DRMSYSTEMTIME   *f_pAdditionalDateTime,   
 IN OUT DRMSYSTEMTIME   *f_pDateTime
);

DRM_RESULT DRM_API MSDB_UTL_SaveDataToFile
(
    IN  void            *f_pData, 
    IN  DRM_DWORD        f_cbData, 
    IN  DRM_CONST_STRING f_dstrFileName
);

#ifdef __cplusplus      
}
#endif /*__cplusplus*/

#endif /*__CFDTOOLSUTILITIES_H*/
