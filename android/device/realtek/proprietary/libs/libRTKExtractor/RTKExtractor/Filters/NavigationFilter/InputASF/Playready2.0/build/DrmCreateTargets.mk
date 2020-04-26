#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#* Description:
#*    Define the rules for building different kind of targets
#*
#*    Outputs:
#*
#*      DRM_BUILD_TARGET: Compiles, builds libraraies and links
#*                        them to create an executable
#*
#*      DRM_CLEAN_TARGET: Deletes all objs, libs and executables
#*
#*
#*********************************************************************/


# Inference rules for C/C++ files that produce object files.
!include "DrmCreateObjTargets.mk"

# Define targets for creating, cleaning, and traversing the directories.
!include "DrmCreateDirsTargets.mk"

# Define targets for building library/exe
!include "DrmCreateBinTargets.mk"


#------------------------------------------------------------------------------
# Define the master targets.
# If we have dirs file present, we build _DRM_BUILD_SUBDIR_TARGETS
# target otherwise we build the source files
#------------------------------------------------------------------------------


DRM_BUILD_TARGET:  \
#-------------------------------
!if "$(TARGETTYPE)" == "LIBRARY" && "$(_DRM_BUILD_LIBRARY_TARGETS)" == "1"
    $(_DRM_BIN_TARGET) \
#-------------------------------
!elseif "$(TARGETTYPE)" == "LIBRARY" && "$(_DRM_BUILD_OBJECT_TARGETS)" == "1"
    _DRM_CREATE_OBJDIR_TARGET $(_DRM_OBJ_TARGETS) \
#-------------------------------
!elseif ("$(TARGETTYPE)" == "PROGRAM" || "$(TARGETTYPE)" == "DYNLINK") && "$(_DRM_BUILD_BINARY_TARGETS)" == "1"
    $(_DRM_BIN_TARGET) \
#-------------------------------
!elseif exist(.\dirs)
    $(_DRM_BUILD_SUBDIR_TARGETS) \
!endif
#-------------------------------
!if "$(_DRM_BUILD_POST_PROCESS_TARGET)" == "1"
    _DRM_POST_PROCESS_TARGET
!endif


DRM_CLEAN_TARGET: \
!if "$(_DRM_BUILD_TOOL)" == "NMAKE"
    _DRM_CLEAN_CURRDIR_TARGET $(_DRM_CLEAN_SUBDIR_TARGETS)
!endif

