/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRM_DEVICESTORE_H__
#define __DRM_DEVICESTORE_H__

#include <drmtypes.h>
#include <drmcontextsizes.h>
#include <drmmanager.h>
#include <drmdatastore.h>

ENTER_PK_NAMESPACE;

#if DRM_SUPPORT_DEVICESTORE

/* Number of seconds for validation timeout, should be 48 hours as per NDT spec */
#define NDTDEVICESTORE_VALIDATION_TIMEOUT 172800

/* Initial size of the dirty attribute buffer */
#define DEVICESTORE_NUMDIRTYATTRIBUTES_INTIAL 5

/* Value set in the dwNumAttributes member of a device handle indicating that the number of 
   attributes for the device is not known */
#define DEVICESTORE_NUMATTRIBUTES_UNKNOWN 0


/*
** Device store context stucture. This contains the DST namespace context it is using and
** the local stack info for internal use
*/
typedef struct _tag_DeviceStoreContext
{    
    DRM_DST_SLOT_CONTEXT        oSlotContext;
    DRM_DST_NAMESPACE_CONTEXT   oNamespaceContext;
    DRM_DST                    *pDatastore;
    DRM_BB_CONTEXT             *pBBXContext;
    DRM_SECSTORE_CONTEXT       *pSecureStoreContext;
    DRM_SECSTOREENUM_CONTEXT    oSecStoreEnum;
    DRM_BOOL                    fEnumInited;
} _DeviceStoreContext;

typedef struct _tag_DeviceHandle
{
    DRM_BYTE                   *pbEntry;
    DRM_DWORD                   cbEntry;
    DRM_BYTEBLOB                oDeviceID;
    _DeviceStoreContext        *pDeviceStoreContext;
    DRM_DWORD                  *pdwDirtyAttributes;   /* List of the indices of attributes that have been modified */
    DRM_DWORD                   dwDirtyAttributesSize; /* Size of the dirty attribute buffer as number of DRM_DWORD elements */
    DRM_DWORD                   dwNumDirtyAttributes;  /* Number of attributes marked as dirty */
    DRM_DWORD                   dwNumAttributes;        /* Number of attributes of the device, 0 if unknown (there should always be at least one attribute, DEVICEID) */
    DRM_BOOL                    fDontCommitOnFree;      /* Don't automatically commit when the handle is freed when this flag is set */
} _DeviceHandle;

typedef struct __tagDRM_DEVICESTORE_CONTEXT{
    DRM_BYTE rgbOpaqueBuffer[ __CB_DECL( SIZEOF( _DeviceStoreContext ) ) ];
} DRM_DEVICESTORE_CONTEXT;

typedef DRM_VOID* DRM_DEVICESTORE_HANDLE;

/* 
** API Functions 
*/

/**********************************************************************
** Function:    DRM_DVS_Initialize
**
**
** Parameters
  f_pDeviceStoreContext [in]  Pointer to an allocated context that will be initialized. Must not be NULL.

Return Values
If the function succeeds, it returns DRM_SUCCESS and f_pDeviceStoreContext points to an initialized context. If the function fails, an appropriate error code will be returned. 
Remarks
Defined in OEM Layer (See architectural diagram)
The NDT PK must call DRM_DVS_Initialize with an allocated context before it can invoke any other device store APIs with that context. This function will initialize the incoming context as the OEM deems fit.

***********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_DVS_Initialize(
    IN     DRM_DEVICESTORE_CONTEXT  *f_pDeviceStoreContext,
    IN     DRM_APP_CONTEXT      *f_pDeviceStoreInitData

);



DRM_API DRM_VOID DRM_CALL DRM_DVS_Uninitialize(
    IN OUT DRM_DEVICESTORE_CONTEXT  *f_pDeviceStoreContext
);



DRM_API DRM_VOID DRM_CALL DRM_DVS_FreeDevice(
    IN    DRM_DEVICESTORE_HANDLE     f_hDeviceHandle
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_RegisterDevice(
    IN OUT DRM_DEVICESTORE_CONTEXT         *f_pDeviceStoreContext,
    IN     DRM_BYTEBLOB                     f_DeviceID,
       OUT DRM_DEVICESTORE_HANDLE          *f_pDeviceHandle
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_UnRegisterDevice(
    IN OUT DRM_DEVICESTORE_CONTEXT         *f_pDeviceStoreContext,
    IN     DRM_BYTEBLOB                     f_DeviceID
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetFirstDevice(
    IN OUT DRM_DEVICESTORE_CONTEXT      *f_pDeviceStoreContext,
       OUT DRM_DEVICESTORE_HANDLE       *f_pDeviceHandle
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetNextDevice(
    IN OUT DRM_DEVICESTORE_CONTEXT      *f_pDeviceStoreContext,
       OUT DRM_DEVICESTORE_HANDLE       *f_pDeviceHandle
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetDeviceByID(
    IN OUT DRM_DEVICESTORE_CONTEXT      *f_pDeviceStoreContext,
    IN     DRM_BYTEBLOB                  f_DeviceID,
       OUT DRM_DEVICESTORE_HANDLE       *f_pDeviceHandle
);


DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetDeviceID(
    IN     DRM_DEVICESTORE_HANDLE           f_hDeviceHandle,
       OUT DRM_BYTE                        *f_pbDeviceID,
    IN OUT DRM_DWORD                       *f_pcbDeviceID
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetDeviceAttr(
    IN     DRM_DEVICESTORE_HANDLE           f_hDeviceHandle,
    IN     DRM_BYTEBLOB                     f_AttrName,
       OUT DRM_BYTE                        *f_pbAttrValue,
    IN OUT DRM_DWORD                       *f_pcbAttrValue
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetDeviceAttrCount(
    IN     DRM_DEVICESTORE_HANDLE           f_hDeviceHandle,
       OUT DRM_DWORD                       *f_pdwAttrCount
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_GetDeviceAttrByIndex(
    IN     DRM_DEVICESTORE_HANDLE           f_hDeviceHandle,
    IN     DRM_DWORD                        f_dwAttrIndex,
       OUT DRM_BYTE                        *f_pbAttrName,
    IN OUT DRM_DWORD                       *f_pcbAttrName,
       OUT DRM_BYTE                        *f_pbAttrValue,
    IN OUT DRM_DWORD                       *f_pcbAttrValue
);


DRM_API DRM_RESULT DRM_CALL DRM_DVS_SetDeviceAttr(
    IN     DRM_DEVICESTORE_HANDLE           f_hDeviceHandle,
    IN     DRM_BYTEBLOB                     f_AttrName,
    IN     DRM_BYTEBLOB                     f_AttrValue
);



DRM_API DRM_RESULT DRM_CALL DRM_DVS_CommitDevice(
    IN     DRM_DEVICESTORE_HANDLE           f_hDeviceHandle
);

#endif

EXIT_PK_NAMESPACE;

#endif  /* __DRM_DEVICESTORE_H__ */


