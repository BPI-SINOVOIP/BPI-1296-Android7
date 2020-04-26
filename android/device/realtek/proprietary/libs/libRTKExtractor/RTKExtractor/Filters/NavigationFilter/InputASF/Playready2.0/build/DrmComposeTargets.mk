
#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#* Build the list of intermediate targets required to build the binary
#* Description:
#*
#*  Outputs the list of following targets.
#*
#*  _DRM_OBJ_TARGETS: The list of obj targets. This is a list of files
#*      with their extensions replaced with obj and PLACEHOLDER_OBJ_PATH
#*      prepended to them. PLACEHOLDER_OBJ_PATH is later replaced with
#*      the location where the object files are placed
#*
#*  _DRM_BUILD_SUBDIR_TARGETS: Target for building all the subdirectories. This
#*      is the list of all subdirectories which need to be built with
#*      .[TARGET_BUILD_DIR] string appended to them
#*
#*  _DRM_CLEAN_SUBDIR_TARGETS: Target for cleaning all the subdirectories. This
#*      is the list of all subdirectories which need to be built with
#*      .[TARGET_CLEAN_DIR] string appended to them
#*
#*  _DRM_BIN_TARGET: The name of the binary appended with the right extension
#*      depending on the type of binary being built (library or executable)
#*
#*  _DRM_OBJ_PATH_PREFIX: Prefix for the path where object files will
#*      be placed
#*
#*  _DRM_BIN_PATH_PREFIX: Prefix for the path where binaries will be
#*      placed
#*
#*********************************************************************/

#------------------------------------------------------------------------------
# It defines the list of obj files that need to be generated to
# build the final binary. Outputs _DRM_OBJ_TARGETS
#------------------------------------------------------------------------------
!include "DrmComposeObjTargets.mk"

#------------------------------------------------------------------------------
# Build the list of dirs required to recursively traverse
# the directory structure to build the binary
# Outputs _DRM_BUILD_SUBDIR_TARGETS and _DRM_CLEAN_SUBDIR_TARGETS
#------------------------------------------------------------------------------
!include "DrmComposeDirsTargets.mk"

#------------------------------------------------------------------------------
# Build the list of binary targets( lib/exe etc.)
# Outputs _DRM_BIN_TARGET
#------------------------------------------------------------------------------
!include "DrmComposeBinTargets.mk"
