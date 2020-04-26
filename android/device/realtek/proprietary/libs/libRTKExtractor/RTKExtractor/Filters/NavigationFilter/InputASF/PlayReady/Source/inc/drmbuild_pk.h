/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBUILD_PK__
#define __DRMBUILD_PK__

#ifdef __cplusplus
extern "C" {
#endif


#if DRM_BUILD_PROFILE != DRM_BUILD_PROFILE_PK
#error "Wrong build profile header file"
#endif

/*
#------------------------------------------------------------------------------
# Set this flag to '1' to include the Anti-rollback clock feature. If this
# flag is set to '0', device clock rollback will not be detected. Hence all
# date-time based content/licenses would not be able to be maintained
# accurately.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_ANTIROLLBACK_CLOCK
#define DRM_SUPPORT_ANTIROLLBACK_CLOCK          1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable application revocation functionality.
#
# If this flag is '0', the application revocation list storage and application revocation checks
# do not compile.
#
# If this flag is '1', it will set DRM_SUPPORT_REVOCATION to 1.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_APP_REVOCATION
#define DRM_SUPPORT_APP_REVOCATION              0
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable block header caching support
#
# If this flag is '1', HDS block headers may be cached for improved
# performace. Note that the sample OEM implementation in oemimpl.c
# is not adequate if multiple instances of the porting kit are running
# concurrently. The cache must be global to all instances sharing the
# same HDS file, and should have appropriate resource locking.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_BLOCK_HEADER_CACHING
#define DRM_SUPPORT_BLOCK_HEADER_CACHING        0
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable optimizations that cache the certificates
# validated when verifying a certificate chain.  This allows the asymmetric
# validation of a particular certificate to be performed only once.  Each
# subsequent validation is a symmetric validation, and is therefore much quicker.
# If this flag is set to '0', devices will use more expensive asymmetric operations.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_CERTIFICATE_CACHING
#define DRM_SUPPORT_CERTIFICATE_CACHING         1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable expired license cleanup functionality
#
# If this flag is '1', calling Drm_StoreMgmt_CleanupStore will iterate
# through all licenses and delete those that have expired and are no
# longer usable.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_CLEANUP
#define DRM_SUPPORT_CLEANUP                     1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable device revocation functionality.
#
# If this flag is '0', the Device revocation list storage and device revocation checks
# do not compile.
#
# If this flag is '1', it will set DRM_SUPPORT_REVOCATION to 1.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_DEVICE_REVOCATION
#define DRM_SUPPORT_DEVICE_REVOCATION           1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable device store functionality.
#
# If this flag is '0', the device store functionality will not be compiled.
# The device store is used by the WMDRM-NDT porting kit, and so is only needed if
# linking with it.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_DEVICESTORE
#define DRM_SUPPORT_DEVICESTORE                 1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to include the supoort for the Direct License
# Acquisition. If this flag is '0', devices will only acquire licenses
# via content transfer via a host PC or the License Sync (see below).
#
# The Following License Acquisition API and the supporting 
# functions will be included (if this flag is set to '1'):
# - Drm_LicenseAcq_GenerateChallenge
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_DLA
#define DRM_SUPPORT_DLA                         1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to include the support for License store Sync'ing
# with PC host. It this flag is '0', devices will only be able to acquire
# licenses via content transfer from a host, or through Direct License
# Acquisition with a WMRM license server.
#
# The following License Acquisition, Sync, and Sync List APIs and the 
# supporting functions will be included (if this flag is set to '1'):
# - Drm_LicenseAcq_GetContentHeader,
# - Drm_Sync_GenerateChallenge,
# - Drm_SyncListItem_InitEnum (also requires DRM_SUPPORT_DLA), and
# - Drm_SyncListItem_EnumNext (also requires DRM_SUPPORT_DLA)
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_LICENSE_SYNC
#define DRM_SUPPORT_LICENSE_SYNC                1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable file locking support
# 
# If this flag is '1', file locking support will be enabled
# in oemfileio.c. File locking support is required if the porting
# kit code is ever executed concurrently using the same HDS file
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_LOCKING
#define DRM_SUPPORT_LOCKING                     1
#endif
/*
#------------------------------------------------------------------------------
#
# Set this flag to '1' to enable multithreading support
#
# Note that you must fully implement critical section and locking support in the 
# OEM layer.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_MULTI_THREADING
#define DRM_SUPPORT_MULTI_THREADING             1
#endif
#if DRM_SUPPORT_MULTI_THREADING && !DRM_SUPPORT_LOCKING
#error "We cannot be thread safe unless file locking is supported"
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to include the Metering feature. If this flag is '0',
# licenses with a Metering ID (MID) will not be usable.
#
# The following Metering and Metering Certificate APIs and the supporting 
# functions will be included (if this flag is set to '1'):
# - Drm_MeterCert_InitEnum,
# - Drm_MeterCert_EnumNext,
# - Drm_MeterCert_Update,
# - Drm_MeterCert_Invalidate,
# - Drm_MeterCert_Delete,
# - Drm_Metering_GenerateChallenge,
# - Drm_Metering_ProcessResponse,
# - Drm_MeterCert_GenerateChallenge, and
# - Drm_MeterCert_ProcessResponse
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_METERING
#define DRM_SUPPORT_METERING                    1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to include the Secure Clock feature when clock is set 
# by using a secure time Web service and support a grace period 
# if the content license allows it. Setting the flag into '0' disables this capability.
#
# The Following Secure Clock APIs and the supporting functions will be
# included (if this flag is set to '1'):
# - Drm_SecureClock_ProcessResponse,
# - Drm_SecureClock_GenerateChallenge, and
# - Drm_SecureClock_GetValue
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_SECURE_CLOCK
#define DRM_SUPPORT_SECURE_CLOCK                1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable optimizations that offload asymmetric crypto
# operations to symmetric operations.  For example, some signatures after being
# verified once asymmetrically can be converted to a symmetric signature such that
# subsequent verifications are faster.  The same is true for some asymmetric decrypt
# operations.  If this flag is set to '0', devices will always choose more expensive
# asymmetric operations.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS
#define DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS     1
#endif
/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable revocation support for WMDRM Network Devices 
# Transmitter functionality. Setting this flag will enable support for WMDRMNET 
# revocation list storage and revocation checks.
#
# If this flag is '0', the WMDRMNET revocation storage checks do not compile.
#
# If this flag is '1', it will set DRM_SUPPORT_REVOCATION to 1.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_WMDRMNET
#define DRM_SUPPORT_WMDRMNET                    1
#endif
/*
#------------------------------------------------------------------------------
#
# Set this flag to '1' to disable native 64 bit types
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_NO_NATIVE_64_TYPES
#if defined( _WIN64)
/* When building for a 64bit target, there needs to be native 64-bit type support */
#define DRM_NO_NATIVE_64_TYPES                  0
#else
/* Otherwise, we'll not use the native 64-bit types even if available */
#define DRM_NO_NATIVE_64_TYPES                  0
#endif
#endif

/*
#------------------------------------------------------------------------------
# DRM_SUPPORT_TEST_SETTINGS
# Set this flag to '1' to enable various test flags:
# TEST_USE_OFFSET_CLOCK
# DRM_TEST_MEM_ALLOC_CHECK
# DRM_TEST_SUPPORT_NET_IO
# DRM_ERROR_CONTRACT_TEST_MODE
# DRM_TEST_IGNORE_UNIQUE_ID
# 
# If the flag is not set it is 0 by default.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_TEST_SETTINGS
#define DRM_SUPPORT_TEST_SETTINGS               1
#endif

/*
#------------------------------------------------------------------------------
# DRM_SUPPORT_PC_FLAGS
#
# Turns several compile flags for use in PC/host components only. 
# They should be unset or set to 0 for all PlayReady implementations:
#    DRM_SUPPORT_V1_LICENSES
#    DRM_SUPPORT_GRL                 
#    DRM_SUPPORT_THUMBNAIL           
#    DRM_BACKUP_RESTORE_SUPPORT      
#    DRM_SUPPORT_CONTENT_REVOCATION  
#    DRM_SUPPORT_PLAYLIST_BURN       
#    DRM_SUPPORT_PMLICENSE           
#    DRM_SUPPORT_COPY_OPL            
#    DRM_SUPPORT_DELETEDSTORE        
#    DRM_NO_FORCE_ALIGN              
#    DRM_TEMP_LICENSE_STORE_HDS      
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_PC_FLAGS
#define DRM_SUPPORT_PC_FLAGS    0
#endif

/*
#------------------------------------------------------------------------------
# NO_DRM_CRT
# Choose CRT functions implementation, using own DRM version is default for PK
#------------------------------------------------------------------------------
*/
#ifndef NO_DRM_CRT
#define NO_DRM_CRT                      0
#endif

/*
#------------------------------------------------------------------------------
# DRM_USE_ASSEMBLY
# Set to 1 to enable assembly optimized routines where available
#------------------------------------------------------------------------------
*/
#ifndef DRM_USE_ASSEMBLY
#define DRM_USE_ASSEMBLY                0
#endif

/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable pre-computation of G table to enable speed-up
# in ECC operations at a cost of higher memory footprint
#------------------------------------------------------------------------------
*/
#ifndef DRM_PRECOMPUTE_GTABLE
#define DRM_PRECOMPUTE_GTABLE           0
#endif


#ifdef __cplusplus
}
#endif

#endif  /* __DRMBUILD_PKNEXT_DBG__ */
