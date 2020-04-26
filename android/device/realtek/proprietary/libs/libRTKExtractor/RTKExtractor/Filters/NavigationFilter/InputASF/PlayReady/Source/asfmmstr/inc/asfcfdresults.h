/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __ASFCFDRESULTS_H__
#define __ASFCFDRESULTS_H__

#ifdef __cplusplus
extern "C" {
#endif/* __cplusplus */


#define ASF_CFD_SEVERITY_ERROR          1uL
#define ASF_CFD_FACILITY_ITF            4uL
#define ASF_FACILITY_WIN32              7uL
#define ASF_S_BASECODE                  0xC000
#define ASF_CFD_E_BASECODE              0xC000

#define MAKE_CFD_DRM_RESULT(sev , fac, code ) \
    ( ( DRM_RESULT ) ( ( ( unsigned long ) ( sev ) << 31 ) | ( ( unsigned long ) ( fac ) << 16 ) | \
    ( ( unsigned long ) ( code ) ) ) )
        
/* ASF specific error codes */
#define ASF_E_CORRUPT_FILE          MAKE_CFD_DRM_RESULT(ASF_CFD_SEVERITY_ERROR,  ASF_CFD_FACILITY_ITF, ASF_CFD_E_BASECODE + 26 )
#define ASF_E_INVALIDOPERATION      MAKE_CFD_DRM_RESULT(ASF_CFD_SEVERITY_ERROR,  ASF_CFD_FACILITY_ITF, ASF_CFD_E_BASECODE + 27 )

#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /*__ASFCFDRESULTS_H__*/
