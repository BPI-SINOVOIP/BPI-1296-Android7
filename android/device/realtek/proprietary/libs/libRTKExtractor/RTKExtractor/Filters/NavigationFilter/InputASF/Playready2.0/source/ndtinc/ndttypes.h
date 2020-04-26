/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTTYPES_H__
#define __NDTTYPES_H__

#include <drmtypes.h>
#include <drmoutputleveltypes.h>

ENTER_PK_NAMESPACE;

typedef enum _tagDRM_REVOCATION_LIST_TYPE
{
    DRM_REVOCATION_LIST_ALL = -1,
    DRM_REVOCATION_LIST_RIV,
    DRM_REVOCATION_LIST_APPS,
    DRM_REVOCATION_LIST_DEVICE,
    DRM_REVOCATION_LIST_NET
} DRM_REVOCATION_LIST_TYPE;

typedef enum _NDTMGR_SESSION_TYPE
{
    NDTMGR_SESSION_TYPE_REGISTRATION = 0x00,
    NDTMGR_SESSION_TYPE_CONTENT      = 0x01
} NDTMGR_SESSION_TYPE;

typedef enum _NDT_ASFDELIVERY_MODE
{
    NDT_ASFDELIVERY_MODE_MUXED   = 0,
    NDT_ASFDELIVERY_MODE_SAMPLES = 1
} NDT_ASFDELIVERY_MODE;

typedef struct _NDT_SAMPLEINFO
{
    DRM_DWORD    dwStreamID;
    DRM_UINT64   qwSampleTimeNS;
    DRM_UINT64   qwSampleDurationNS;
    DRM_UINT64   qwSampleID;
} NDT_SAMPLEINFO;

typedef enum tagNDT_LICENSE_MODE
{
    NDT_LICENSE_MODE_SIMPLE  = 0,
    NDT_LICENSE_MODE_CHAINED = 1
} NDT_LICENSE_MODE;

typedef struct __tagDRM_GUIDLIST
{
    DRM_DWORD  cGUIDs;
    DRM_BYTE  *pguidBuffer;
} DRM_GUIDLIST;

#ifndef __wmdrmsdk_h__

typedef enum tagWMDRMNET_POLICY_TYPE
{
    WMDRMNET_POLICY_TYPE_UNDEFINED      = 0x0000,
    WMDRMNET_POLICY_TYPE_TRANSCRYPTPLAY = 0x0001,
} WMDRMNET_POLICY_TYPE;

typedef struct tagWMDRMNET_POLICY
{
    WMDRMNET_POLICY_TYPE ePolicyType;
    DRM_BYTE pbPolicy[1];
} WMDRMNET_POLICY;

#endif

typedef struct tagWMDRMNET_POLICY_MINIMUM_ENVIRONMENT
{
    DRM_WORD wMinimumSecurityLevel;
    DRM_DWORD dwMinimumAppRevocationListVersion;
    DRM_DWORD dwMinimumDeviceRevocationListVersion;
} WMDRMNET_POLICY_MINIMUM_ENVIRONMENT;

typedef struct tagWMDRMNET_POLICY_GLOBAL_REQUIREMENTS
{
    WMDRMNET_POLICY_MINIMUM_ENVIRONMENT MinimumEnvironment;
    DRM_DWORD                           dwSourceID;
    DRM_GUIDLIST                        InclusionList;
    DRM_DWORD                           dwRiv;
} WMDRMNET_POLICY_GLOBAL_REQUIREMENTS;

#ifndef __wmdrmsdk_h__

typedef struct tagWMDRMNET_POLICY_TRANSCRYPTPLAY
{
    WMDRMNET_POLICY_GLOBAL_REQUIREMENTS globals;
    DRM_PLAY_OPL playOPLs;
} WMDRMNET_POLICY_TRANSCRYPTPLAY;

#endif

EXIT_PK_NAMESPACE;

#endif /* __NDTTYPES_H__ */
