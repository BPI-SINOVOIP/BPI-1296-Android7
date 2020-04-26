#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#* Description:
#*
#*  Define target rules for building library/exe.
#*
#*  Outputs:
#*
#*  _DRM_BIN_TARGET: The rule for building the binary.
#*
#*********************************************************************/




#------------------------------------------------------------------------------
# Our target when we are trying to build source files.
# we should create directories to put the object files,
# after that our targets are obj files that we need to
# build and all the libs that we need to link with
# If we are building a libarary, we invoke the library manager
# otherwise we call the linker
#------------------------------------------------------------------------------
!if "$(TARGETTYPE)" != "NOTARGET"
$(_DRM_BIN_TARGET): _DRM_CREATE_BINDIR_TARGET _DRM_CREATE_OBJDIR_TARGET $(_DRM_OBJ_TARGETS)
!   if "$(TARGETTYPE)" == "PROGRAM"
	$(_DRM_VERSIONING_TOOL_COMMAND)
        $(_DRM_BIN_LINKER_TOOL_COMMAND) $(_DRM_OBJ_TARGETS)
!   elseif "$(TARGETTYPE)" == "LIBRARY"
        $(_DRM_LIBRARIAN_TOOL_COMMAND) $(_DRM_OBJ_TARGETS)
!   elseif "$(TARGETTYPE)" == "DYNLINK"
        $(_DRM_DLL_LINKER_TOOL_COMMAND) $(_DRM_OBJ_TARGETS)
!   endif
        $(_DRM_COPY_TO_DIR_COMMAND) $(_DRM_BIN_TARGET) $(_DRM_COPY_PATH)
!   if "$(_DRM_SYM_TARGET)" != ""
        $(_DRM_COPY_TO_DIR_COMMAND) $(_DRM_SYM_TARGET) $(_DRM_COPY_PATH)
!   endif
!   if "$(_DRM_IMPORT_LIB_TARGET)" != ""
        $(_DRM_COPY_TO_DIR_COMMAND) $(_DRM_IMPORT_LIB_TARGET) $(_DRM_COPY_PATH)
!   endif
!endif
