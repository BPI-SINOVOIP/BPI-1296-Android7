/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMNDT_H__
#define __OEMNDT_H__

#include <ndttypes.h>
#include <oemcommon.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL Oem_Ndt_AllowNewDeviceRegistration(
    __in     DRM_BYTEBLOB f_DeviceCertificate,
    __in     DRM_BYTEBLOB f_DeviceSerialNumber
);


DRM_API DRM_RESULT DRM_CALL Oem_Ndt_GetProximityDetectionAddress(
    __out_bcount_opt(*f_pcbAddress) DRM_BYTE       *f_pbAddress,
    __inout                         DRM_WORD       *f_pcbAddress
);


extern DRM_BOOL g_fOverride_OPL_Defaults;
extern DRM_WORD g_wAnalogVideo;
extern DRM_WORD g_wCompressedDigitalAudio;
extern DRM_WORD g_wCompressedDigitalVideo;
extern DRM_WORD g_wUncompressedDigitalAudio;
extern DRM_WORD g_wUncompressedDigitalVideo;
DRM_API DRM_RESULT DRM_CALL OEM_NDT_GetLicenseOPLs(
    __out_opt DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS *f_MinPlayOpls,
    __out     DRM_BOOL                             *f_OplSet
);


extern DRM_BOOL  g_fOverride_InclusionList_Defaults;
extern DRM_DWORD g_cInclusionListGUIDs;
DRM_API DRM_RESULT DRM_CALL OEM_NDT_GetLicenseInclusionList(
    __inout_opt DRM_GUIDLIST *f_InclusionList,
    __out       DRM_BOOL     *f_fInclusionListSet
);


extern DRM_DWORD g_dwSourceID;
DRM_API DRM_RESULT DRM_CALL OEM_NDT_GetSourceID ( 
    __out_ecount(1)    DRM_DWORD                               *f_pdwSourceID );


extern DRM_DWORD g_dwRevInfo;
DRM_API DRM_RESULT DRM_CALL OEM_NDT_GetLicenseRevInfoVersion ( 
    __out_ecount(1)     DRM_DWORD                               *f_pdwRevInfo );


extern DRM_DWORD g_dwMinAppSec;
DRM_API DRM_RESULT DRM_CALL OEM_NDT_GetWMDRMNETMinAppSec(
    __out_ecount(1) DRM_DWORD                               *f_pdwMinAppSec );

EXIT_PK_NAMESPACE;

#endif /*  __OEMNDT_H__ */
