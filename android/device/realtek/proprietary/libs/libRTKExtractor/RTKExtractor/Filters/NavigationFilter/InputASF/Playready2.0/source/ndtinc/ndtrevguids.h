/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTREVGUIDS_H__
#define __NDTREVGUIDS_H__

#include <drmcommon.h>
#include <drmtypes.h>


#ifndef DRM_IsEqualGUID
#define DRM_IsEqualGUID( rguid1, rguid2 ) ( ! MEMCMP( rguid1, rguid2, SIZEOF( DRM_GUID ) ) )
#endif /* DRM_IsEqualGUID */

DRM_DECLARE_GUID( GUID_WMDRMRevocationTypeApp );

DRM_DECLARE_GUID( GUID_WMDRMRevocationTypeWmdrmpd );

DRM_DECLARE_GUID( GUID_WMDRMRevocationTypeWmdrmnd );

DRM_DECLARE_GUID( GUID_WMDRMRevocationTypeRiv );

#endif /* __NDTREVGUIDS_H__ */
