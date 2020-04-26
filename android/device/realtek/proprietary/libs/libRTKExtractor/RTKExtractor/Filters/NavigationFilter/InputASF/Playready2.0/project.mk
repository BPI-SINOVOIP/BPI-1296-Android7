#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#* Description:
#*    This file contains project-specific settings for the PK.
#*
#*********************************************************************/


#------------------------------------------------------------------------------
# MS internal builds only:
# The Microsoft internal build tools natively process project.mk and sources.
# If the PK makefile system hasn't been brought into the loop, do so here.
#------------------------------------------------------------------------------

!if "$(_DRM_BUILD_TOOL)" == ""
!    if "$(DRM_ROOT_PATH)" == "" && defined(BASEDIR)
DRM_ROOT_PATH=$(BASEDIR)\source\pk\WMDRM-MD^\
!    endif
!include "$(DRM_ROOT_PATH)DrmMakefile.def"
!endif


#------------------------------------------------------------------------------
# Retrieve settings overrides
#------------------------------------------------------------------------------

#
# This should really happen here, but there's a catch: doing so requires any
# feature flag overrides to happen in project.mk's, never sources files. All
# such overrides will need to move to project.mk's if sources files are to
# shared with gnuSources (that implicitly disallows any !includes or !ifs.)
#
#!include "$(DRM_ROOT_PATH)DrmOverrideFeatureDefaults.mk"


#------------------------------------------------------------------------------
# Set some paths suffixes
#------------------------------------------------------------------------------

!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_DRM_NMAKE_OBJ_SUFFIX=bind^\
!elseif "$(DRM_BUILD_TYPE)" == "FREE"
_DRM_NMAKE_OBJ_SUFFIX=bin^\
!elseif "$(DRM_BUILD_TYPE)" == "PERF"
_DRM_NMAKE_OBJ_SUFFIX=binp^\
!else
!error "Unknown DRM_BUILD_TYPE: $(DRM_BUILD_TYPE)"
!endif

!if "$(DRM_BUILD_ARCH)" == "X86"
_DRM_NMAKE_OBJ_SUFFIX=$(_DRM_NMAKE_OBJ_SUFFIX)i386
!elseif "$(DRM_BUILD_ARCH)" == "X64"
_DRM_NMAKE_OBJ_SUFFIX=$(_DRM_NMAKE_OBJ_SUFFIX)amd64
!elseif "$(DRM_BUILD_ARCH)" == "ARM"
_DRM_NMAKE_OBJ_SUFFIX=$(_DRM_NMAKE_OBJ_SUFFIX)wmsp60_arm
!elseif "$(DRM_BUILD_ARCH)" == "XBOX"
_DRM_NMAKE_OBJ_SUFFIX=$(_DRM_NMAKE_OBJ_SUFFIX)XBOX
!else
!error "Unknown DRM_BUILD_ARCH: $(DRM_BUILD_ARCH)"
!endif


#------------------------------------------------------------------------------
# Construct the source paths and binplace variables
#------------------------------------------------------------------------------

!if "$(_WINCEROOT)" != ""
_DRM_SOURCE_PATH=$(_WINCEROOT)\private\directx\wmt40\filters\PlayReady\PK^\
!else
_DRM_SOURCE_PATH=$(DRM_ROOT_PATH)source^\
!endif

!if "$(DRM_COPY_ROOT)" != ""
_DRM_PK_SOURCE_BIN_ROOT=$(DRM_COPY_ROOT)
_DRM_PK_SOURCE_BIN_DESTINATION=pksrc
!elseif "$(_DRM_BUILD_TOOL)" == "NMAKE"
_DRM_PK_SOURCE_BIN_ROOT=$(_DRM_SOURCE_PATH)$(_DRM_NMAKE_OBJ_SUFFIX)^\
_DRM_PK_SOURCE_BIN_DESTINATION=
!elseif "$(_WINCEROOT)" != ""
_DRM_PK_SOURCE_BIN_ROOT=$(_WINCEROOT)\public\directx\oak\lib\$(_CPUINDPATH)^\
_DRM_PK_SOURCE_BIN_DESTINATION=
!elseif "$(_DRM_BUILD_TOOL)" == "NTBUILD"
_DRM_PK_SOURCE_BIN_ROOT=$(_NTTREE)^\
_DRM_PK_SOURCE_BIN_DESTINATION=pksrc
!endif

DRM_PK_SOURCE_LIB_PATH=$(_DRM_PK_SOURCE_BIN_ROOT)
!if "$(_DRM_PK_SOURCE_BIN_DESTINATION)" != ""
DRM_PK_SOURCE_LIB_PATH=$(DRM_PK_SOURCE_LIB_PATH)$(_DRM_PK_SOURCE_BIN_DESTINATION)^\
!endif


#------------------------------------------------------------------------------
# Construct the test paths and binplace variables
#------------------------------------------------------------------------------

!if "$(_WINCEROOT)" != ""
_DRM_TEST_PATH=$(_WINCEROOT)\private\test\Multimedia\DirectX\drm\PlayReady\PK^\
!else
_DRM_TEST_PATH=$(DRM_ROOT_PATH)test^\
!endif

!if "$(DRM_COPY_ROOT)" != ""
_DRM_PK_TEST_BIN_ROOT=$(DRM_COPY_ROOT)
_DRM_PK_TEST_BIN_DESTINATION=pktest
!elseif "$(_WINCEROOT)" != ""
_DRM_PK_TEST_BIN_ROOT=$(TEST_OAK_LIB_PATH) #TEST_OAK_LIB_PATH is $(_WINCEROOT)\public\test\oak\lib\$(_CPUINDPATH)
_DRM_PK_TEST_BIN_DESTINATION=
!elseif "$(_DRM_BUILD_TOOL)" == "NMAKE"
_DRM_PK_TEST_BIN_ROOT=$(_DRM_TEST_PATH)$(_DRM_NMAKE_OBJ_SUFFIX)^\
_DRM_PK_TEST_BIN_DESTINATION=
!elseif "$(_DRM_BUILD_TOOL)" == "NTBUILD"
_DRM_PK_TEST_BIN_ROOT=$(_NTTREE)^\
_DRM_PK_TEST_BIN_DESTINATION=pktest
!endif

DRM_PK_TEST_LIB_PATH=$(_DRM_PK_TEST_BIN_ROOT)
!if "$(_DRM_PK_TEST_BIN_DESTINATION)" != ""
DRM_PK_TEST_LIB_PATH=$(DRM_PK_TEST_LIB_PATH)$(_DRM_PK_TEST_BIN_DESTINATION)^\
!endif


#------------------------------------------------------------------------------
# Set the default DRM_COPY_ROOT and TARGET_DESTINATION
# - Note: For NTBUILD folks DRM_COPY_ROOT is equivalent to 
#   binplace.exe's /R:$(_NTTREE), i.e. $(TARGETPATH)
#------------------------------------------------------------------------------

!if "$(_DRM_BUILD_TEST)" == "1"

!    if "$(DRM_COPY_ROOT)" == ""
DRM_COPY_ROOT=$(_DRM_PK_TEST_BIN_ROOT)
!    endif
TARGET_DESTINATION=$(_DRM_PK_TEST_BIN_DESTINATION)

!else

!    if "$(DRM_COPY_ROOT)" == ""
DRM_COPY_ROOT=$(_DRM_PK_SOURCE_BIN_ROOT)
!    endif
TARGET_DESTINATION=$(_DRM_PK_SOURCE_BIN_DESTINATION)

!endif


#------------------------------------------------------------------------------
# Modify TARGET_DESTINATION for the MS-internal MTP simulator tools
#------------------------------------------------------------------------------

!if "$(_DRM_BUILD_MTP)" == "USB"
TARGET_DESTINATION=$(TARGET_DESTINATION)\USB
!elseif "$(_DRM_BUILD_MTP)" == "IP"
TARGET_DESTINATION=$(TARGET_DESTINATION)\IP
!endif
