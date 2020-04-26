/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTASFGUIDS_H__
#define __NDTASFGUIDS_H__

#include <drmcommon.h>
#include <drmtypes.h>

#ifndef DRM_IsEqualGUID
#define DRM_IsEqualGUID( rguid1, rguid2 ) ( ! MEMCMP( rguid1, rguid2, SIZEOF( DRM_GUID ) ) )
#endif /* DRM_IsEqualGUID */

/***************************************************************************/
/* WMA/ASF GUIDs */
DRM_DECLARE_GUID( CLSID_ASFHeaderObjectV0 );

DRM_DECLARE_GUID( CLSID_ASFContentEncryptionObject );

DRM_DECLARE_GUID( CLSID_ASFContentEncryptionObjectEx );

DRM_DECLARE_GUID( CLSID_ASFPlayReadyObject );

DRM_DECLARE_GUID( CLSID_ASFStreamPropertiesObjectEx );

DRM_DECLARE_GUID( CLSID_ASFStreamPropertiesObjectV1 );

DRM_DECLARE_GUID( CLSID_ASFAdvancedContentEncryptionObject );

DRM_DECLARE_GUID( CLSID_ASFHeaderExtensionObject );

DRM_DECLARE_GUID( CLSID_ASFPacketClock1 );

DRM_DECLARE_GUID( CLSID_ASFFilePropertiesObjectV2 );

DRM_DECLARE_GUID( CLSID_ASFStreamBitratePropertiesObject );

/* WMDRM-ND/AES specific GUIDs */
DRM_DECLARE_GUID( CLSID_ASF_Payload_Extension_Encryption_SampleID );

DRM_DECLARE_GUID( CLSID_ASF_ContentEncryptionSystem_WMDRM_ND_AES );

/* ASF Media Stream Types */
DRM_DECLARE_GUID( CLSID_ASFStreamTypeUnknown );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeAudioMedia );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeVideoMedia );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeCommandMedia );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeJFIFMedia );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeDegradableJPEGMedia );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeFileTransferMedia );

DRM_DECLARE_GUID( CLSID_ASFStreamTypeBinaryMedia );


#endif /* __NDTASFGUIDS_H__ */
