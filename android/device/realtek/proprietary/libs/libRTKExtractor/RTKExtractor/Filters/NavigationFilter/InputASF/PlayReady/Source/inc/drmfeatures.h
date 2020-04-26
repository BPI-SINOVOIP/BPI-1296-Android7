/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMSETFEATURES_H__
#define __DRMSETFEATURES_H__

#ifdef __cplusplus
extern "C" {
#endif

/*                                                                                  
** PK Test build - To validate the ported code build with this profile. It
** enables various test options. Code built under this profile should *NEVER* 
** be shipped.
*/                                                                                  
#define DRM_BUILD_PROFILE_PK             1

/*                                                                                  
** Sample OEM profile - OEMs can take make variants of the two profiles above
** and give them names here.
*/                                                                                  
#define DRM_BUILD_PROFILE_OEM           10

/*                                                                                  
** Microsoft internal profiles: (No value to OEMs)
** -----------------------------------------------
** PK V.NEXT profile - used internally to ensure code for possible future 
**     products compiles.
** PC profile - used internally by PC team (and server).
** MIN and MAX TEST profiles - This range of values is used internally for testing
*/                                                                                  
#define DRM_BUILD_PROFILE_PC                   21
#define DRM_BUILD_PROFILE_PK_TEST_MIN          100
#define DRM_BUILD_PROFILE_PK_TEST_MAX          199

/* 
** Ensure the build profile is properly set. 
*/
#ifndef DRM_BUILD_PROFILE
    #ifndef __BUILDMACHINE__
        #error "DRM_BUILD_PROFILE must be set to one of the above values either by the build environment or here."
        /* Example: #define DRM_BUILD_PROFILE DRM_BUILD_PROFILE_PK */
    #else
        /* For MS internal builds (detected via __BUILDMACHINE__), default to PC profile */
        #define DRM_BUILD_PROFILE DRM_BUILD_PROFILE_PC
    #endif
#endif

/*
** Pick correct compilation options based on a build profile
*/
#if DRM_BUILD_PROFILE == DRM_BUILD_PROFILE_PK
#include <drmbuild_pk.h>
#elif DRM_BUILD_PROFILE == DRM_BUILD_PROFILE_OEM
#include <drmbuild_oem.h>
#elif DRM_BUILD_PROFILE == DRM_BUILD_PROFILE_PC
#include <drmbuild_pc.h>
#elif DRM_BUILD_PROFILE >= DRM_BUILD_PROFILE_PK_TEST_MIN && DRM_BUILD_PROFILE <= DRM_BUILD_PROFILE_PK_TEST_MAX
#include <drmbuild_pk_TestFlagSets.h>
#else
#error "Invalid value of DRM_BUILD_PROFILE in drmfeatures.h"
#endif


/****************************************************************************
** Special case: DRM_SUPPORT_REVOCATION is calculated here and nowhere else.
****************************************************************************/
#if DRM_SUPPORT_APP_REVOCATION  ||  DRM_SUPPORT_DEVICE_REVOCATION  || DRM_SUPPORT_WMDRMNET || DRM_SUPPORT_CONTENT_REVOCATION
#define DRM_SUPPORT_REVOCATION          1
#else
#define DRM_SUPPORT_REVOCATION          0
#endif

/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable the datastore to be created with a specific size.
# This reduces the fragmentation of the datastore, and increases performance during
# datastore access.
# The following two defines in drmmanager.c control the initial size, and the subsequent
# increment in datastore size.  Set them to suit your specific implementation.
#    #define DATASTORE_INITSIZE      512      prealloc 512 K for initial datastore size
#    #define DATASTORE_GROWSIZE      512      grow 512 K for each datastore expansion
#
# If this flag is '0', the datastore blocks are allocated as they are needed.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_DATASTORE_PREALLOC
#define DRM_SUPPORT_DATASTORE_PREALLOC          1
#endif

/*
#------------------------------------------------------------------------------
# DRM_SUPPORT_DOMAINS
# Set this flag to '1' to include the support for DRM domain
# and be able to play back a domain license. All PlayReady implementations
# must have this feature enabled.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_DOMAINS
#define DRM_SUPPORT_DOMAINS                     1
#endif


/*
#------------------------------------------------------------------------------
# DRM_SUPPORT_PC_FLAGS
# Set this flag to '1' for PC build profile.
#
# The flag must be set into 0 or 1 in drmbuild_*.h
#
# The following compile flags are for use in PC/host components only and should
# be unset or set to 0 for all PlayReady implementations:
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
#error "DRM_SUPPORT_PC_FLAGS is not defined."
#endif

#if DRM_SUPPORT_PC_FLAGS==1

#ifndef DRM_NO_FORCE_ALIGN
#define DRM_NO_FORCE_ALIGN              1
#endif
#ifndef DRM_SUPPORT_CONTENT_REVOCATION
#define DRM_SUPPORT_CONTENT_REVOCATION  0
#endif
#ifndef DRM_SUPPORT_COPY_OPL
#define DRM_SUPPORT_COPY_OPL            1
#endif
#ifndef DRM_SUPPORT_DELETEDSTORE
#define DRM_SUPPORT_DELETEDSTORE        0
#endif
#ifndef DRM_SUPPORT_GRL
#define DRM_SUPPORT_GRL                 1
#endif
#ifndef DRM_SUPPORT_PLAYLIST_BURN
#define DRM_SUPPORT_PLAYLIST_BURN       1
#endif
#ifndef DRM_SUPPORT_PMLICENSE
#define DRM_SUPPORT_PMLICENSE           0
#endif
#ifndef DRM_BACKUP_RESTORE_SUPPORT
#define DRM_BACKUP_RESTORE_SUPPORT      1
#endif
#ifndef DRM_SUPPORT_THUMBNAIL
#define DRM_SUPPORT_THUMBNAIL           1
#endif
#ifndef DRM_SUPPORT_V1_LICENSES
#define DRM_SUPPORT_V1_LICENSES         0
#endif
#ifndef DRM_TEMP_LICENSE_STORE_HDS
#define DRM_TEMP_LICENSE_STORE_HDS      1
#endif

#else

#ifndef DRM_NO_FORCE_ALIGN
#define DRM_NO_FORCE_ALIGN              0
#endif
#ifndef DRM_SUPPORT_CONTENT_REVOCATION
#define DRM_SUPPORT_CONTENT_REVOCATION  0
#endif
#ifndef DRM_SUPPORT_COPY_OPL
#define DRM_SUPPORT_COPY_OPL            0
#endif
#ifndef DRM_SUPPORT_DELETEDSTORE
#define DRM_SUPPORT_DELETEDSTORE        0
#endif
#ifndef DRM_SUPPORT_GRL
#define DRM_SUPPORT_GRL                 0
#endif
#ifndef DRM_SUPPORT_PLAYLIST_BURN
#define DRM_SUPPORT_PLAYLIST_BURN       0
#endif
#ifndef DRM_SUPPORT_PMLICENSE
#define DRM_SUPPORT_PMLICENSE           0
#endif
#ifndef DRM_BACKUP_RESTORE_SUPPORT
#define DRM_BACKUP_RESTORE_SUPPORT      0
#endif
#ifndef DRM_SUPPORT_THUMBNAIL
#define DRM_SUPPORT_THUMBNAIL           0
#endif
#ifndef DRM_SUPPORT_V1_LICENSES
#define DRM_SUPPORT_V1_LICENSES         0
#endif
#ifndef DRM_TEMP_LICENSE_STORE_HDS
#define DRM_TEMP_LICENSE_STORE_HDS      0
#endif

#endif // DRM_SUPPORT_PC_FLAGS 

/*
#------------------------------------------------------------------------------
# DRM_SUPPORT_TEST_SETTINGS
# Set this flag to '1' to enable various test flags:
#   - TEST_USE_OFFSET_CLOCK
#   - DRM_TEST_MEM_ALLOC_CHECK
#   - DRM_TEST_SUPPORT_NET_IO
#   - DRM_ERROR_CONTRACT_TEST_MODE
#   - DRM_TEST_IGNORE_UNIQUE_ID
# 
# If the flag is not set it is 0 by default.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_TEST_SETTINGS
#define DRM_SUPPORT_TEST_SETTINGS               0
#endif


/*
# If it's a test mode then set test flags, otherwise turn them off.
*/
#if DRM_SUPPORT_TEST_SETTINGS == 1

#define TEST_USE_OFFSET_CLOCK           1
/*
** mem alloc check should be disabled by default because we run AppVerifier
*/
#ifndef DRM_TEST_MEM_ALLOC_CHECK
#define DRM_TEST_MEM_ALLOC_CHECK        0
#endif

#ifndef DRM_TEST_SUPPORT_NET_IO
#define DRM_TEST_SUPPORT_NET_IO         1
#endif

#ifndef DRM_ERROR_CONTRACT_TEST_MODE
#if DBG
#define DRM_ERROR_CONTRACT_TEST_MODE    1
#else
#define DRM_ERROR_CONTRACT_TEST_MODE    0
#endif
#endif

#ifndef DRM_TEST_IGNORE_UNIQUE_ID
#if defined(WINCE)
#define DRM_TEST_IGNORE_UNIQUE_ID       1
#else
#define DRM_TEST_IGNORE_UNIQUE_ID       0
#endif
#endif


#else
/*
# Test settings off
*/
#ifndef TEST_USE_OFFSET_CLOCK       
#define TEST_USE_OFFSET_CLOCK           0
#endif
#ifndef DRM_TEST_MEM_ALLOC_CHECK    
#define DRM_TEST_MEM_ALLOC_CHECK        0
#endif
#ifndef DRM_TEST_SUPPORT_NET_IO     
#define DRM_TEST_SUPPORT_NET_IO         0
#endif
#ifndef DRM_ERROR_CONTRACT_TEST_MODE
#define DRM_ERROR_CONTRACT_TEST_MODE    0
#endif
#ifndef DRM_TEST_IGNORE_UNIQUE_ID
#define DRM_TEST_IGNORE_UNIQUE_ID       0
#endif
#ifndef DRM_TEST_NO_BUFFER_FLUSH
#define DRM_TEST_NO_BUFFER_FLUSH        0
#endif

#endif /* DRM_SUPPORT_TEST_SETTINGS */

/*
#------------------------------------------------------------------------------
# DRM_SUPPORT_PROFILING
# Set this flag to '1' to enable profiling functionality
# Profiling macros compile to gather performance data and write to a binary
# file named perf.dat or perfagg.dat. The file can be used by external tools
# for further analysis.
# Profiling must be activated by a runtime call of DRM_Start_Profiling(),
# and it can also be turned off at runtime by calling DRM_Stop_Profiling().
# To minimize overhead introduced by the profiling, samples collected during
# profiling are buffered first and flushed to a file when the buffer becomes full.
# The file drmprofile.h defines several contstants to modify the behavior of
# the profiling.
#
# If this flag is '0', the profiling macros compile to nothing.
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_SUPPORT_PROFILING
#define DRM_SUPPORT_PROFILING           0
#endif

/*
#------------------------------------------------------------------------------
# DRM_EXPORT_APIS_TO_DLL
# Set this flag to '1' to mark DRM_API functions to be exported from a DLL.
#------------------------------------------------------------------------------
*/
#ifndef DRM_EXPORT_APIS_TO_DLL
#define DRM_EXPORT_APIS_TO_DLL          0
#endif
/*
#------------------------------------------------------------------------------
# DRM_TEST_LINK_TO_DRMAPI_DLL
# Set this flag to '1' to allow test code executables to link with a DLL.
#------------------------------------------------------------------------------
*/
#ifndef DRM_TEST_LINK_TO_DRMAPI_DLL
#define DRM_TEST_LINK_TO_DRMAPI_DLL     0
#endif

/*
#------------------------------------------------------------------------------
# This setting specifies the size in bytes of the HDS copy buffer. A larger
# size will increase HDS performace, but will require a larger DRM_APP_CONTEXT.
# Also, no performance benefit will be had for buffer sizes above the block 
# size (which is a default of 32kb).
#
# There are no top level APIs directly affected by this flag.
#
#------------------------------------------------------------------------------
*/
#ifndef DRM_HDS_COPY_BUFFER_SIZE      
#define DRM_HDS_COPY_BUFFER_SIZE                32768
#endif

/*
#------------------------------------------------------------------------------
# If the _DATASTORE_WRITE_THRU is set to 0, most HDS writes will be
# immediately flushed to disk with Oem_File_FlushBuffers. So long as all file 
# handles to the same file will see written data immediately, and writes make
# it to physical media in an expidient manner, this option may be set to 1.
#------------------------------------------------------------------------------
*/
#ifndef _DATASTORE_WRITE_THRU
#define _DATASTORE_WRITE_THRU                   1
#endif

/*
#------------------------------------------------------------------------------
# Total number of licenses in a chain support by this DRM implementation 
# Must be set to 2.
#------------------------------------------------------------------------------
*/
#ifndef DRM_MAX_LICENSE_CHAIN_DEPTH
#define DRM_MAX_LICENSE_CHAIN_DEPTH   2                                    
#endif

/*
#------------------------------------------------------------------------------
# MSDB (by default) should use dynamic memory allocation (Oem_Mem_Alloc)
# Legacy alternative is stack allocation (DRM_STK_Alloc)
# For dynamic memory allocation set value to 1
# For stack memory allocation set value to 0
#------------------------------------------------------------------------------
*/
#ifndef DRM_MSDB_USE_DYNAMIC_MEM_ALLOCATION
#define DRM_MSDB_USE_DYNAMIC_MEM_ALLOCATION   1
#endif



/*

To enforce feature value for ALL profiles add the following pragma and then define a feature:

#pragma warning(disable:4005)
#define DRM_SUPPORT_SOMEFEATURE 1

*/

#ifdef __cplusplus
}
#endif

#endif  /* __DRMSETFEATURES_H__ */










