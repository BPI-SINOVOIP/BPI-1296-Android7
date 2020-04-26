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
# NOTE: This file should ONLY do the following:
# 1. Set appropriate sub-profile flags based on DRM_BUILD_PROFILE,
#    e.g. DRM_SUPPORT_SECURE_CLOCK
# 2. Set appropriate module flags based on #1,
#    e.g. DRM_MODULES_SECURE_CLOCK_ENABLE or DRM_MODULES_SECURE_CLOCK_DISABLE
# 3. Set _DRM_C_DEFINES based on #1.
#
# It should not define anything else.  Use drmbuild.cmn for general
# definitions, e.g. source paths, obj paths, other variables, etc.
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

#------------------------------------------------------------------------------
# Derive _DRM_BUILD_PROFILE_INT:
# values should match DRM_BUILD_PROFILE macros
# in drmfeatures.h exactly.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Derive other feature flags:
# values should match corresponding #defines
# in the corresponding drmbuild_*.h header file exactly
#------------------------------------------------------------------------------
!if "$(DRM_BUILD_PROFILE)" == "PK"

!INCLUDE "DrmProfilePK.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "OEM"

!INCLUDE "DrmProfileOEM.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PC"

!INCLUDE "DrmProfilePC.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_1"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_2"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_3"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_4"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_5"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_6"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_7"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_8"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_9"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_10"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_11"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_12"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_13"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_14"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_15"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_TESTFLAGSET_16"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "PK_NO_WMDRM"

!INCLUDE "DrmProfileTEST.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "WM7"

!INCLUDE "DrmProfileWM7.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "XBOX"

!INCLUDE "DrmProfileXBOX.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "RMSDK"

!INCLUDE "drmprofilermsdk.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "IOS"

!INCLUDE "drmprofileios.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "MPR"

!INCLUDE "drmprofilempr.mk"

!elseif "$(DRM_BUILD_PROFILE)" == "MPRCLEAN"

!INCLUDE "drmprofilemprclean.mk"

!else

!error DRM_BUILD_PROFILE=$(DRM_BUILD_PROFILE), invalid value.

!endif

#------------------------------------------------------------------------------
# It is not recommended but still possible to overwrite compiler options
# directly from command line.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# DRM_SUPPORT_TEST_SETTINGS section
#------------------------------------------------------------------------------

!IF "$(DRM_SUPPORT_TEST_SETTINGS)" == ""
DRM_SUPPORT_TEST_SETTINGS=$(DRM_SUPPORT_TEST_SETTINGS_VIA_PROFILE)
!ENDIF

#
# Sanity check: at this point, test settings should be set to 1 or 0.
#
!IF "$(DRM_SUPPORT_TEST_SETTINGS)" == ""
!ERROR DRM_SUPPORT_TEST_SETTINGS is not set
!ENDIF

#
# Each test setting can be directly enabled via profile
# and then indirectly enabled via DRM_SUPPORT_TEST_SETTINGS
# At this time, the only test setting implicitly set by
# DRM_SUPPORT_TEST_SETTINGS is DRM_SUPPORT_CONTRACT
#

!IF "$(DRM_SUPPORT_CONTRACT)" == ""
DRM_SUPPORT_CONTRACT=$(DRM_SUPPORT_CONTRACT_VIA_PROFILE)
!ENDIF

#
# If (and only if) build type is checked,
# DRM_SUPPORT_CONTRACT is implicitly enabled via DRM_SUPPORT_TEST_SETTINGS
# Else it defaults to 0.
#
!IF "$(DRM_SUPPORT_CONTRACT)" == ""
!IF "$(DRM_BUILD_TYPE)" != "CHECKED"
DRM_SUPPORT_CONTRACT=0
!ELSE
DRM_SUPPORT_CONTRACT=$(DRM_SUPPORT_TEST_SETTINGS)
!ENDIF
!ENDIF

!IF "$(DRM_SUPPORT_CONTRACT)" == "1"
DRM_MODULES_CONTRACT_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_CONTRACT)" == "0"
DRM_MODULES_CONTRACT_DISABLE=1
!ENDIF

#------------------------------------------------------------------------------
# DRM_MODULES section
#------------------------------------------------------------------------------

!IF "$(DRM_SUPPORT_ACTIVATION)" == ""
DRM_SUPPORT_ACTIVATION=$(DRM_SUPPORT_ACTIVATION_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_ACTIVATION)" == "1"
DRM_MODULES_ACTIVATION_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_ACTIVATION)" == "0"
DRM_MODULES_ACTIVATION_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_ANTIROLLBACK_CLOCK)" == ""
DRM_SUPPORT_ANTIROLLBACK_CLOCK=$(DRM_SUPPORT_ANTIROLLBACK_CLOCK_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_ANTIROLLBACK_CLOCK)" == "1"
DRM_MODULES_ANTIROLLBACK_CLOCK_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_ANTIROLLBACK_CLOCK)" == "0"
DRM_MODULES_ANTIROLLBACK_CLOCK_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_APP_POLICY)" == ""
DRM_SUPPORT_APP_POLICY=$(DRM_SUPPORT_APP_POLICY_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_APP_POLICY)" == "1"
DRM_MODULES_APP_POLICY_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_APP_POLICY)" == "0"
DRM_MODULES_APP_POLICY_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_APPREVOCATION)" == ""
DRM_SUPPORT_APPREVOCATION=$(DRM_SUPPORT_APPREVOCATION_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_APPREVOCATION)" == "1"
DRM_MODULES_APPREVOCATION_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_APPREVOCATION)" == "0"
DRM_MODULES_APPREVOCATION_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_CERTCACHE)" == ""
DRM_SUPPORT_CERTCACHE=$(DRM_SUPPORT_CERTCACHE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_CERTCACHE)" == "1"
DRM_MODULES_CERTCACHE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_CERTCACHE)" == "0"
DRM_MODULES_CERTCACHE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_CERTPARSERCACHE)" == ""
DRM_SUPPORT_CERTPARSERCACHE=$(DRM_SUPPORT_CERTPARSERCACHE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_CERTPARSERCACHE)" == "1"
DRM_MODULES_CERTPARSERCACHE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_CERTPARSERCACHE)" == "0"
DRM_MODULES_CERTPARSERCACHE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_CLEANSTORE)" == ""
DRM_SUPPORT_CLEANSTORE=$(DRM_SUPPORT_CLEANSTORE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_CLEANSTORE)" == "1"
DRM_MODULES_CLEANSTORE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_CLEANSTORE)" == "0"
DRM_MODULES_CLEANSTORE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_CONTENTREVOCATION)" == ""
DRM_SUPPORT_CONTENTREVOCATION=$(DRM_SUPPORT_CONTENTREVOCATION_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_CONTENTREVOCATION)" == "1"
DRM_MODULES_CONTENTREVOCATION_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_CONTENTREVOCATION)" == "0"
DRM_MODULES_CONTENTREVOCATION_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_COPYOPL)" == ""
DRM_SUPPORT_COPYOPL=$(DRM_SUPPORT_COPYOPL_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_COPYOPL)" == "1"
DRM_MODULES_COPYOPL_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_COPYOPL)" == "0"
DRM_MODULES_COPYOPL_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_CRT)" == ""
DRM_SUPPORT_CRT=$(DRM_SUPPORT_CRT_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_CRT)" == "1"
DRM_MODULES_CRT_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_CRT)" == "0"
DRM_MODULES_CRT_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_DELETEDSTORE)" == ""
DRM_SUPPORT_DELETEDSTORE=$(DRM_SUPPORT_DELETEDSTORE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_DELETEDSTORE)" == "1"
DRM_MODULES_DELETEDSTORE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_DELETEDSTORE)" == "0"
DRM_MODULES_DELETEDSTORE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_DEVCERTKEYGEN)" == ""
DRM_SUPPORT_DEVCERTKEYGEN=$(DRM_SUPPORT_DEVCERTKEYGEN_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_DEVCERTKEYGEN)" == "1"
DRM_MODULES_DEVCERTKEYGEN_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_DEVCERTKEYGEN)" == "0"
DRM_MODULES_DEVCERTKEYGEN_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_DEVICEREVOCATION)" == ""
DRM_SUPPORT_DEVICEREVOCATION=$(DRM_SUPPORT_DEVICEREVOCATION_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_DEVICEREVOCATION)" == "1"
DRM_MODULES_DEVICEREVOCATION_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_DEVICEREVOCATION)" == "0"
DRM_MODULES_DEVICEREVOCATION_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_DEVICESTORE)" == ""
DRM_SUPPORT_DEVICESTORE=$(DRM_SUPPORT_DEVICESTORE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_DEVICESTORE)" == "1"
DRM_MODULES_DEVICESTORE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_DEVICESTORE)" == "0"
DRM_MODULES_DEVICESTORE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_DLA)" == ""
DRM_SUPPORT_DLA=$(DRM_SUPPORT_DLA_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_DLA)" == "1"
DRM_MODULES_DLA_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_DLA)" == "0"
DRM_MODULES_DLA_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_DOMAIN)" == ""
DRM_SUPPORT_DOMAIN=$(DRM_SUPPORT_DOMAIN_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_DOMAIN)" == "1"
DRM_MODULES_DOMAIN_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_DOMAIN)" == "0"
DRM_MODULES_DOMAIN_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_EST)" == ""
DRM_SUPPORT_EST=$(DRM_SUPPORT_EST_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_EST)" == "1"
DRM_MODULES_EST_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_EST)" == "0"
DRM_MODULES_EST_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_HDS)" == ""
DRM_SUPPORT_HDS=$(DRM_SUPPORT_HDS_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_HDS)" == "1"
DRM_MODULES_HDS_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_HDS)" == "0"
DRM_MODULES_HDS_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_HDSBLOCKHEADERCACHE)" == ""
DRM_SUPPORT_HDSBLOCKHEADERCACHE=$(DRM_SUPPORT_HDSBLOCKHEADERCACHE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_HDSBLOCKHEADERCACHE)" == "1"
DRM_MODULES_HDSBLOCKHEADERCACHE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_HDSBLOCKHEADERCACHE)" == "0"
DRM_MODULES_HDSBLOCKHEADERCACHE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_LICENSE_SYNC)" == ""
DRM_SUPPORT_LICENSE_SYNC=$(DRM_SUPPORT_LICENSE_SYNC_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_LICENSE_SYNC)" == "1"
DRM_MODULES_LICENSE_SYNC_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_LICENSE_SYNC)" == "0"
DRM_MODULES_LICENSE_SYNC_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_METERING)" == ""
DRM_SUPPORT_METERING=$(DRM_SUPPORT_METERING_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_METERING)" == "1"
DRM_MODULES_METERING_ENABLE=1
DRM_MODULES_METERCERTREVOCATION_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_METERING)" == "0"
DRM_MODULES_METERING_DISABLE=1
DRM_MODULES_METERCERTREVOCATION_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_MODELREVOCATION)" == ""
DRM_SUPPORT_MODELREVOCATION=$(DRM_SUPPORT_MODELREVOCATION_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_MODELREVOCATION)" == "1"
DRM_MODULES_MODELREVOCATION_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_MODELREVOCATION)" == "0"
DRM_MODULES_MODELREVOCATION_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_MOVE)" == ""
DRM_SUPPORT_MOVE=$(DRM_SUPPORT_MOVE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_MOVE)" == "1"
DRM_MODULES_MOVE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_MOVE)" == "0"
DRM_MODULES_MOVE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_PERFORMANCE)" == ""
DRM_SUPPORT_PERFORMANCE=$(DRM_SUPPORT_PERFORMANCE_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_PERFORMANCE)" == "1"
DRM_MODULES_PERFORMANCE_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_PERFORMANCE)" == "0"
DRM_MODULES_PERFORMANCE_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_SECURE_CLOCK)" == ""
DRM_SUPPORT_SECURE_CLOCK=$(DRM_SUPPORT_SECURE_CLOCK_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_SECURE_CLOCK)" == "1"
DRM_MODULES_SECURE_CLOCK_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_SECURE_CLOCK)" == "0"
DRM_MODULES_SECURE_CLOCK_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_SYMOPT)" == ""
DRM_SUPPORT_SYMOPT=$(DRM_SUPPORT_SYMOPT_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_SYMOPT)" == "1"
DRM_MODULES_SYMOPT_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_SYMOPT)" == "0"
DRM_MODULES_SYMOPT_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_THUMBNAIL)" == ""
DRM_SUPPORT_THUMBNAIL=$(DRM_SUPPORT_THUMBNAIL_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_THUMBNAIL)" == "1"
DRM_MODULES_THUMBNAIL_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_THUMBNAIL)" == "0"
DRM_MODULES_THUMBNAIL_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_VIEWRIGHTS)" == ""
DRM_SUPPORT_VIEWRIGHTS=$(DRM_SUPPORT_VIEWRIGHTS_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_VIEWRIGHTS)" == "1"
DRM_MODULES_VIEWRIGHTS_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_VIEWRIGHTS)" == "0"
DRM_MODULES_VIEWRIGHTS_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_WMDRM)" == ""
DRM_SUPPORT_WMDRM=$(DRM_SUPPORT_WMDRM_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_WMDRM)" == "1"
DRM_MODULES_WMDRM_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_WMDRM)" == "0"
DRM_MODULES_WMDRM_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_WMDRMNET)" == ""
DRM_SUPPORT_WMDRMNET=$(DRM_SUPPORT_WMDRMNET_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_WMDRMNET)" == "1"
DRM_MODULES_WMDRMNET_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_WMDRMNET)" == "0"
DRM_MODULES_WMDRMNET_DISABLE=1
!ENDIF

!IF "$(DRM_SUPPORT_XMLHASH)" == ""
DRM_SUPPORT_XMLHASH=$(DRM_SUPPORT_XMLHASH_VIA_PROFILE)
!ENDIF
!IF "$(DRM_SUPPORT_XMLHASH)" == "1"
DRM_MODULES_XMLHASH_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_XMLHASH)" == "0"
DRM_MODULES_XMLHASH_DISABLE=1
!ENDIF

#------------------------------------------------------------------------------
# Compile-time profile option section
#------------------------------------------------------------------------------

!INCLUDE "DrmProfileCompileTimeOptions.mk"

#------------------------------------------------------------------------------
# The following compiler options are not included into profiles
# meaning that should be set from a command line or a script only.
# But their default values will still be defined in drmfeatures.h.
#------------------------------------------------------------------------------

!IF "$(DRM_SUPPORT_PROFILING)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_SUPPORT_PROFILING=$(DRM_SUPPORT_PROFILING)
!IF "$(DRM_SUPPORT_PROFILING)" == "1"
DRM_MODULES_PROFILING_ENABLE=1
!ELSEIF "$(DRM_SUPPORT_PROFILING)" == "0"
DRM_MODULES_PROFILING_DISABLE=1
!ENDIF
!ENDIF

!IF "$(DRM_EXPORT_APIS_TO_DLL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_EXPORT_APIS_TO_DLL=$(DRM_EXPORT_APIS_TO_DLL)
!ENDIF

!IF "$(DRM_TEST_LINK_TO_DRMAPI_DLL)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_TEST_LINK_TO_DRMAPI_DLL=$(DRM_TEST_LINK_TO_DRMAPI_DLL)
!ENDIF

!IF "$(DRM_INTERACTIVE_OEM_MAIN)" != ""
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DRM_INTERACTIVE_OEM_MAIN=$(DRM_INTERACTIVE_OEM_MAIN)
!ENDIF

#------------------------------------------------------------------------------
# The revocation module should be enabled if any of the following are set.
#
#     DRM_SUPPORT_APPREVOCATION
#  || DRM_SUPPORT_CONTENTREVOCATION
#  || DRM_SUPPORT_DEVICEREVOCATION
#  || DRM_SUPPORT_METERING
#  || DRM_SUPPORT_WMDRMNET
#------------------------------------------------------------------------------
!IF "$(DRM_SUPPORT_APPREVOCATION)" == "1"
DRM_MODULES_REVOCATION_ENABLE_VIA_PROFILE_WITH_OVERRIDE=1
!ENDIF

!IF "$(DRM_SUPPORT_CONTENTREVOCATION)" == "1"
DRM_MODULES_REVOCATION_ENABLE_VIA_PROFILE_WITH_OVERRIDE=1
!ENDIF

!IF "$(DRM_SUPPORT_DEVICEREVOCATION)" == "1"
DRM_MODULES_REVOCATION_ENABLE_VIA_PROFILE_WITH_OVERRIDE=1
!ENDIF

!IF "$(DRM_SUPPORT_METERING)" == "1"
DRM_MODULES_REVOCATION_ENABLE_VIA_PROFILE_WITH_OVERRIDE=1
!ENDIF

!IF "$(DRM_SUPPORT_WMDRMNET)" == "1"
DRM_MODULES_REVOCATION_ENABLE_VIA_PROFILE_WITH_OVERRIDE=1
!ENDIF

!IF "$(DRM_MODULES_REVOCATION_ENABLE_VIA_PROFILE_WITH_OVERRIDE)" == "1"
DRM_MODULES_REVOCATION_ENABLE=1
!ELSE
DRM_MODULES_REVOCATION_DISABLE=1
!ENDIF

#------------------------------------------------------------------------------
# Some feature settings are strictly incompatible
#------------------------------------------------------------------------------
!IF "$(DRM_SUPPORT_ACTIVATION)"=="1"
!IF "$(DRM_SUPPORT_DEVCERTKEYGEN)"=="1"
!ERROR Feature settings DRM_SUPPORT_ACTIVATION=1 and DRM_SUPPORT_DEVCERTKEYGEN=1 are strictly incompatible!
!ENDIF
!IF "$(DRM_SUPPORT_CERTPARSERCACHE)"=="1"
!ERROR Feature settings DRM_SUPPORT_ACTIVATION=1 and DRM_SUPPORT_CERTPARSERCACHE=1 are strictly incompatible!
!ENDIF
!IF "$(DRM_SUPPORT_WMDRM)"=="1"
!ERROR Feature settings DRM_SUPPORT_ACTIVATION=1 and DRM_SUPPORT_WMDRM=1 are strictly incompatible!
!ENDIF
!IF "$(DRM_SUPPORT_NONVAULTSIGNING)"=="1"
!ERROR Feature settings DRM_SUPPORT_ACTIVATION=1 and DRM_SUPPORT_NONVAULTSIGNING=1 are strictly incompatible!
!ENDIF
!ENDIF

!IF "$(DRM_SUPPORT_VIEWRIGHTS)"=="0"
!IF "$(DRM_SUPPORT_LICENSE_SYNC)"=="1"
!ERROR Feature settings DRM_SUPPORT_VIEWRIGHTS=0 and DRM_SUPPORT_LICENSE_SYNC=1 are strictly incompatible!
!ENDIF
!IF "$(DRM_SUPPORT_DELETEDSTORE)"=="1"
!ERROR Feature settings DRM_SUPPORT_VIEWRIGHTS=0 and DRM_SUPPORT_DELETEDSTORE=1 are strictly incompatible!
!ENDIF
!ENDIF

!IF "$(DRM_SUPPORT_WMDRM)"=="0"
!IF "$(DRM_SUPPORT_DLA)"=="1"
!ERROR Feature settings DRM_SUPPORT_WMDRM=0 and DRM_SUPPORT_DLA=1 are strictly incompatible!
!ENDIF
!IF "$(DRM_SUPPORT_CONTENTREVOCATION)"=="1"
!ERROR Feature settings DRM_SUPPORT_WMDRM=0 and DRM_SUPPORT_CONTENTREVOCATION=1 are strictly incompatible!
!ENDIF
!IF "$(DRM_SUPPORT_CERTPARSERCACHE)"=="1"
!ERROR Feature settings DRM_SUPPORT_WMDRM=0 and DRM_SUPPORT_CERTPARSERCACHE=1 are strictly incompatible!
!ENDIF
!ENDIF

#------------------------------------------------------------------------------
# The below inclusion ensures people including this header or a calling header
# from their sources can get the corresponding CDefines
#------------------------------------------------------------------------------
!if defined(MAKEDIR_RELATIVE_TO_BASEDIR)
!include "$(DRM_ROOT_PATH)DrmCDefines2NtBuild.mk"
!endif

