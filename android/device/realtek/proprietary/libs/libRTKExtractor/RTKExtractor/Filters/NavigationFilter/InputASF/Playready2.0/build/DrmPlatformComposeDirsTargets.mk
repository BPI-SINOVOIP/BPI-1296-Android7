#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#* Description:
#*    This file assists in the construction of a list of dirs to build or clean.
#*
#* Inputs:
#*    DRM_BUILD_ARCH: X86, X64, ARM, ...
#*    DRM_BUILD_PLATFORM: ANSI, WMSP60, ...
#*        and
#*    _DRM_DIRS_TARGETS: This is of the form:
#*        FOO BAR{TARGET.PLAT, TARGET.ARCH, TARGET.PLAT_ARCH}
#*
#* Outputs:
#*    _DRM_DIRS_TARGETS: This should be of the form:
#*        FOO BAR{TARGET.SKIP, TARGET.BUILD, TARGET.SKIP}
#*
#*********************************************************************/


##############################################################################
#                                                                            #
#  Process the platform directives                                           #
#  -------------------------------                                           #
#                                                                            #
##############################################################################


#=============================================================================
# For ANSI...
#=============================================================================

#---------------------------------------------------
# Step 1: Canonicalize the platform name
#
# Step 2: Enable/disable compilation by replacing platform directives of the
#         form Dir{PLAT,PLAT_ARCH} with
#             Dir{TARGET.BUILD,TARGET.BUILD_ARCH} or
#             Dir{TARGET.SKIP,TARGET.SKIP_ARCH}
#---------------------------------------------------
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.Ansi=TARGET.ANSI)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.ansi=TARGET.ANSI)

!if "$(DRM_BUILD_PLATFORM)" == "ANSI"
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.ANSI=TARGET.BUILD)
!else
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.ANSI=TARGET.SKIP)
!endif


#=============================================================================
# For WINPC...
#=============================================================================

#---------------------------------------------------
# Step 1: Canonicalize the platform name
#
# Step 2: Enable/disable compilation by replacing platform directives of the
#         form Dir{PLAT,PLAT_ARCH} with
#             Dir{TARGET.BUILD,TARGET.BUILD_ARCH} or
#             Dir{TARGET.SKIP,TARGET.SKIP_ARCH}
#---------------------------------------------------
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WinPC=TARGET.WINPC)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WinPc=TARGET.WINPC)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.Winpc=TARGET.WINPC)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.winpc=TARGET.WINPC)

!if "$(DRM_BUILD_PLATFORM)" == "WIN32"
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WINPC=TARGET.BUILD)
!else
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WINPC=TARGET.SKIP)
!endif


#=============================================================================
# For WMSP60...
#=============================================================================

#---------------------------------------------------
# Step 1: Canonicalize the platform name
#
# Step 2: Enable/disable compilation by replacing platform directives of the
#         form Dir{PLAT,PLAT_ARCH} with
#             Dir{TARGET.BUILD,TARGET.BUILD_ARCH} or
#             Dir{TARGET.SKIP,TARGET.SKIP_ARCH}
#---------------------------------------------------
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WMSP60=TARGET.WMSP60)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WmSp60=TARGET.WMSP60)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.Wmsp60=TARGET.WMSP60)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.wmsp60=TARGET.WMSP60)

!if "$(DRM_BUILD_PLATFORM)" == "WMSP60"
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WMSP60=TARGET.BUILD)
!else
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.WMSP60=TARGET.SKIP)
!endif



##############################################################################
#                                                                            #
#  Process the architecture directives                                       #
#  -----------------------------------                                       #
#                                                                            #
##############################################################################

#=============================================================================
# For X86...
#=============================================================================

#---------------------------------------------------
# Step 1: Canonicalize the architecture
# Step 2: Collapse the various PLAT_ARCH combos by replacing
#             TARGET.BUILD_ARCH with TARGET.ARCH
#             TARGET.SKIP_ARCH with TARGET.SKIP
# Step 3: Enable/disable architectures to compile by replacing
#             TARGET.ARCH with TARGET.BUILD or TARGET.SKIP
#---------------------------------------------------
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_x86=TARGET.BUILD_X86)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_x86=TARGET.SKIP_X86)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.x86=TARGET.X86)

_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_X86=TARGET.X86)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_X86=TARGET.SKIP)

!if "$(DRM_BUILD_ARCH)" == "X86"
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.X86=TARGET.BUILD)
!else
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.X86=TARGET.SKIP)
!endif


#=============================================================================
# For X64...
#=============================================================================

#---------------------------------------------------
# Step 1: Canonicalize the architecture
# Step 2: Collapse the various PLAT_ARCH combos by replacing
#             TARGET.BUILD_ARCH with TARGET.ARCH
#             TARGET.SKIP_ARCH with TARGET.SKIP
# Step 3: Enable/disable architectures to compile by replacing
#             TARGET.ARCH with TARGET.BUILD or TARGET.SKIP
#---------------------------------------------------
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_AMD64=TARGET.BUILD_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_Amd64=TARGET.BUILD_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_amd64=TARGET.BUILD_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_x64=TARGET.BUILD_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_AMD64=TARGET.SKIP_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_Amd64=TARGET.SKIP_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_amd64=TARGET.SKIP_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_x64=TARGET.SKIP_X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.AMD64=TARGET.X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.Amd64=TARGET.X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.amd64=TARGET.X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.x64=TARGET.X64)

_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_X64=TARGET.X64)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_X64=TARGET.SKIP)

!if "$(DRM_BUILD_ARCH)" == "X64"
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.X64=TARGET.BUILD)
!else
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.X64=TARGET.SKIP)
!endif


#=============================================================================
# For ARM...
#=============================================================================

#---------------------------------------------------
# Step 1: Canonicalize the architecture
# Step 2: Collapse the various PLAT_ARCH combos by replacing
#             TARGET.BUILD_ARCH with TARGET.ARCH
#             TARGET.SKIP_ARCH with TARGET.SKIP
# Step 3: Enable/disable architectures to compile by replacing
#             TARGET.ARCH with TARGET.BUILD or TARGET.SKIP
#---------------------------------------------------
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_Arm=TARGET.BUILD_ARM)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_arm=TARGET.BUILD_ARM)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_Arm=TARGET.SKIP_ARM)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_arm=TARGET.SKIP_ARM)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.Arm=TARGET.ARM)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.arm=TARGET.ARM)

_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.BUILD_ARM=TARGET.ARM)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.SKIP_ARM=TARGET.SKIP)

!if "$(DRM_BUILD_ARCH)" == "ARM"
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.ARM=TARGET.BUILD)
!else
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS:TARGET.ARM=TARGET.SKIP)
!endif

