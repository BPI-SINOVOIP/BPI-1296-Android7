/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTCONSTANTS_H__
#define __NDTCONSTANTS_H__

#include "ndttypes.h"

ENTER_PK_NAMESPACE;

#define MAXIMUM_LIMIT_FOR_OPENED_DEVICES      10
#define MS_TO_HNS_MULTIPLIER                  10000
#define NDT_FEATURE_RECEIVER                  0x1
#define NDT_FEATURE_TRANSMITTER               0x2
#define NDT_FEATURE_SHARED_CERT               0x4
#define NDTMGR_ASF_WMDRMPROTECTED             0x00000001
#define NDTMGR_ASF_LICENSE_PLAYAVAILABLE      0x00000002

/* Strings for expression evaluator variable evaluation */
extern const DRM_ANSI_CONST_STRING g_dastrTagWMDRMReceiver;         
extern const DRM_ANSI_CONST_STRING g_dastrTagWMDRMTransmitter;      
extern const DRM_ANSI_CONST_STRING g_dastrTagWMDRMSharedCertificate;
extern const DRM_ANSI_CONST_STRING g_dastrDRM_RIGHT_PLAYBACK;

extern const DRM_CONST_STRING      g_dstrDRM_RIGHT_PLAYBACK;
extern const DRM_ID                g_idNULLSessionID;
extern const DRM_CONST_STRING      g_dstrDeviceAttrLastValidationTime;

EXIT_PK_NAMESPACE;

#endif /*  __NDTCONSTANTS_H__ */
