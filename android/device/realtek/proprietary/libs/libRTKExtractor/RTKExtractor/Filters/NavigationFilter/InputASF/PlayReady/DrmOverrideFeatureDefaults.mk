#**@@@+++@@@@******************************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@******************************************************************

#####################################################################
#
# This is the configuration file to specify the flavor of the
# DRM PK code which will be built. 
#
# Defines _DRM_C_DEFINES:
# _DRM_C_DEFINES is a list of definitions to be passed to 
# the compiler. These definintions are all separated by a space.
# Appropriate syntax for compiler is inserted in architecture
# specific file. For e.g. -D is inserted before
# each definition for compilers which use this syntax for C/C++
# defines
#
#####################################################################

#------------------------------------------------------------------------------
# These flags are based on the above configuration flags. To turn a feature
# on/off, change the configuration option above rather than changing the 
# compiler flag directly.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Define the compiler conditional flags
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Preferable way to set compiler flags is to use DRM_BUILD_PROFILE 
# environment variable
#------------------------------------------------------------------------------

#-------------------------------------
# Derive _DRM_BUILD_PROFILE_INT:
# values should match DRM_BUILD_PROFILE macros 
# in drmfeatures.h exactly.
#-------------------------------------
!if "$(DRM_BUILD_PROFILE)" == "PK"
_DRM_BUILD_PROFILE_INT=1
!elseif "$(DRM_BUILD_PROFILE)" == "OEM"
_DRM_BUILD_PROFILE_INT=10
!elseif "$(DRM_BUILD_PROFILE)" == "PC"
_DRM_BUILD_PROFILE_INT=21
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_1"
_DRM_BUILD_PROFILE_INT=101
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_2"
_DRM_BUILD_PROFILE_INT=102
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_3"
_DRM_BUILD_PROFILE_INT=103
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_4"
_DRM_BUILD_PROFILE_INT=104
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_5"
_DRM_BUILD_PROFILE_INT=105
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_6"
_DRM_BUILD_PROFILE_INT=106
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_7"
_DRM_BUILD_PROFILE_INT=107
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_8"
_DRM_BUILD_PROFILE_INT=108
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_9"
_DRM_BUILD_PROFILE_INT=109
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_10"
_DRM_BUILD_PROFILE_INT=110
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_11"
_DRM_BUILD_PROFILE_INT=111
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_12"
_DRM_BUILD_PROFILE_INT=112
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_13"
_DRM_BUILD_PROFILE_INT=113
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_14"
_DRM_BUILD_PROFILE_INT=114
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_15"
_DRM_BUILD_PROFILE_INT=115
!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_16"
_DRM_BUILD_PROFILE_INT=116
!else
!error DRM_BUILD_PROFILE=$(DRM_BUILD_PROFILE), invalid value.
!endif

!IF "$(DRM_BUILD_PROFILE)" != ""
_DRM_C_DEFINES= $(_DRM_C_DEFINES) DRM_BUILD_PROFILE=$(_DRM_BUILD_PROFILE_INT)
!ENDIF


#------------------------------------------------------------------------------
# It is not recommended but still possible to overwrite compiler options 
# directly from command line.
#------------------------------------------------------------------------------
!IF "$(DRM_SUPPORT_ANTIROLLBACK_CLOCK)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_ANTIROLLBACK_CLOCK=$(DRM_SUPPORT_ANTIROLLBACK_CLOCK)
!ENDIF

!IF "$(DRM_SUPPORT_APP_REVOCATION)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_APP_REVOCATION=$(DRM_SUPPORT_APP_REVOCATION)
!ENDIF

!IF "$(DRM_SUPPORT_BLOCK_HEADER_CACHING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_BLOCK_HEADER_CACHING=$(DRM_SUPPORT_BLOCK_HEADER_CACHING)
!ENDIF

!IF "$(DRM_SUPPORT_CERTIFICATE_CACHING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_CERTIFICATE_CACHING=$(DRM_SUPPORT_CERTIFICATE_CACHING)
!ENDIF

!IF "$(DRM_SUPPORT_CLEANUP)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_CLEANUP=$(DRM_SUPPORT_CLEANUP)
!ENDIF

!IF "$(DRM_SUPPORT_DATASTORE_PREALLOC)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_DATASTORE_PREALLOC=$(DRM_SUPPORT_DATASTORE_PREALLOC)
!ENDIF

!IF "$(DRM_SUPPORT_DEVICE_REVOCATION)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_DEVICE_REVOCATION=$(DRM_SUPPORT_DEVICE_REVOCATION)
!ENDIF

!IF "$(DRM_SUPPORT_DEVICESTORE)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_DEVICESTORE=$(DRM_SUPPORT_DEVICESTORE)
!ENDIF

!IF "$(DRM_SUPPORT_DLA)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_DLA=$(DRM_SUPPORT_DLA)
!ENDIF

!IF "$(DRM_SUPPORT_LICENSE_SYNC)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_LICENSE_SYNC=$(DRM_SUPPORT_LICENSE_SYNC)
!ENDIF

!IF "$(DRM_SUPPORT_LOCKING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_LOCKING=$(DRM_SUPPORT_LOCKING)
!ENDIF

!IF "$(DRM_SUPPORT_MULTI_THREADING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_MULTI_THREADING=$(DRM_SUPPORT_MULTI_THREADING)
!ENDIF

!IF "$(DRM_SUPPORT_METERING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_METERING=$(DRM_SUPPORT_METERING)
!ENDIF

!IF "$(DRM_SUPPORT_SECURE_CLOCK)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_SECURE_CLOCK=$(DRM_SUPPORT_SECURE_CLOCK)
!ENDIF

!IF "$(DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS=$(DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS)
!ENDIF

!IF "$(DRM_SUPPORT_WMDRMNET)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_WMDRMNET=$(DRM_SUPPORT_WMDRMNET)
!ENDIF



!IF "$(DRM_SUPPORT_NATIVE_64BIT_TYPES)" == "0"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_NO_NATIVE_64_TYPES=1
!ENDIF

!IF "$(DRM_SUPPORT_NATIVE_64BIT_TYPES)" == "1"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_NO_NATIVE_64_TYPES=0
!ENDIF

!IF "$(DRM_SUPPORT_PC_FLAGS)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_PC_FLAGS=$(DRM_SUPPORT_PC_FLAGS)
!ENDIF

!IF "$(DRM_ALLOW_XMR_CANNOT_PERSIST)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_ALLOW_XMR_CANNOT_PERSIST=$(DRM_ALLOW_XMR_CANNOT_PERSIST)
!ENDIF

!IF "$(DRM_NO_FORCE_ALIGN)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_NO_FORCE_ALIGN=$(DRM_NO_FORCE_ALIGN)
!ENDIF

!IF "$(DRM_SUPPORT_CONTENT_REVOCATION)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_CONTENT_REVOCATION=$(DRM_SUPPORT_CONTENT_REVOCATION)
!ENDIF

!IF "$(DRM_SUPPORT_COPY_OPL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_COPY_OPL=$(DRM_SUPPORT_COPY_OPL)
!ENDIF

!IF "$(DRM_SUPPORT_DELETEDSTORE)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_DELETEDSTORE=$(DRM_SUPPORT_DELETEDSTORE)
!ENDIF

!IF "$(DRM_SUPPORT_GRL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_GRL=$(DRM_SUPPORT_GRL)
!ENDIF

!IF "$(DRM_SUPPORT_PLAYLIST_BURN)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_PLAYLIST_BURN=$(DRM_SUPPORT_PLAYLIST_BURN)
!ENDIF

!IF "$(DRM_SUPPORT_PMLICENSE)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_PMLICENSE=$(DRM_SUPPORT_PMLICENSE)
!ENDIF

!IF "$(DRM_BACKUP_RESTORE_SUPPORT)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_BACKUP_RESTORE_SUPPORT=$(DRM_BACKUP_RESTORE_SUPPORT)
!ENDIF

!IF "$(DRM_SUPPORT_THUMBNAIL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_THUMBNAIL=$(DRM_SUPPORT_THUMBNAIL)
!ENDIF

!IF "$(DRM_SUPPORT_V1_LICENSES)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_V1_LICENSES=$(DRM_SUPPORT_V1_LICENSES)
!ENDIF

!IF "$(DRM_HDS_COPY_BUFFER_SIZE)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_HDS_COPY_BUFFER_SIZE=$(DRM_HDS_COPY_BUFFER_SIZE)
!ENDIF

!IF "$(_DATASTORE_WRITE_THRU)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) _DATASTORE_WRITE_THRU=$(_DATASTORE_WRITE_THRU)
!ENDIF

!IF "$(DRM_TEMP_LICENSE_STORE_HDS)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_TEMP_LICENSE_STORE_HDS=$(DRM_TEMP_LICENSE_STORE_HDS)
!ENDIF

!IF "$(NO_DRM_CRT)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) NO_DRM_CRT=$(NO_DRM_CRT)
!ENDIF


#------------------------------------------------------------------------------
# The following compiler options are not included into profiles
# meaning that should be set from a command line or a script only.
# But their default values will still be defined in drmfeatures.h.
#------------------------------------------------------------------------------

!IF "$(DRM_SUPPORT_PROFILING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_PROFILING=$(DRM_SUPPORT_PROFILING)
!ENDIF

!IF "$(DRM_EXPORT_APIS_TO_DLL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_EXPORT_APIS_TO_DLL=$(DRM_EXPORT_APIS_TO_DLL)
!ENDIF            

!IF "$(DRM_TEST_LINK_TO_DRMAPI_DLL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_TEST_LINK_TO_DRMAPI_DLL=$(DRM_TEST_LINK_TO_DRMAPI_DLL)
!ENDIF            

!IF "$(DRM_SUPPORT_TEST_SETTINGS)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_TEST_SETTINGS=$(DRM_SUPPORT_TEST_SETTINGS)
!ENDIF

!IF "$(TEST_USE_OFFSET_CLOCK)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) TEST_USE_OFFSET_CLOCK=$(TEST_USE_OFFSET_CLOCK)
!ENDIF

!IF "$(DRM_TEST_IGNORE_UNIQUE_ID)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_TEST_IGNORE_UNIQUE_ID=$(DRM_TEST_IGNORE_UNIQUE_ID)
!ENDIF

!IF "$(DRM_TEST_MEM_ALLOC_CHECK)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_TEST_MEM_ALLOC_CHECK=$(DRM_TEST_MEM_ALLOC_CHECK)
!ENDIF

!IF "$(DRM_TEST_SUPPORT_NET_IO)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_TEST_SUPPORT_NET_IO=$(DRM_TEST_SUPPORT_NET_IO)
!ENDIF


!IF "$(DRM_ERROR_CONTRACT_TEST_MODE)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_ERROR_CONTRACT_TEST_MODE=$(DRM_ERROR_CONTRACT_TEST_MODE)
!ENDIF


!IF "$(DRM_INTERACTIVE_OEM_MAIN)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_INTERACTIVE_OEM_MAIN=$(DRM_INTERACTIVE_OEM_MAIN)
!ENDIF


!IF "$(DRM_SUPPORT_REVOCATION)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_REVOCATION=$(DRM_SUPPORT_REVOCATION)
!ENDIF

!IF "$(DRM_SUPPORT_TRACING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_TRACING=$(DRM_SUPPORT_TRACING)
!ENDIF

!IF "$(DRM_PRECOMPUTE_GTABLE)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_PRECOMPUTE_GTABLE=$(DRM_PRECOMPUTE_GTABLE)
!ENDIF

#------------------------------------------------------------------------------
# Set path for source   
#------------------------------------------------------------------------------
_DRM_SOURCE_PATH=$(DRM_ROOT_PATH)source^\
_DRM_TEST_PATH=$(DRM_ROOT_PATH)test^\

