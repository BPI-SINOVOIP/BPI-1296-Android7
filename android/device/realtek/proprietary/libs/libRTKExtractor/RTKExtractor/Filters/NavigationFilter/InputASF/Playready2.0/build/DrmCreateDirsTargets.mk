
#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#* Description:
#*    Define targets for creating, cleaning, and traversing
#*    the directory structure.
#*
#*    Outputs:
#*
#*      _DRM_CREATE_OBJDIR_TARGET: Creates a directory where binaries/objs
#*          will be placed
#*
#*      _DRM_CREATE_BINDIR_TARGET: Creates a directory where binaries/objs
#*          will be placed
#*
#*      _DRM_CLEAN_CURRDIR_TARGET: Cleans all the intermediate object files
#*          and libraries created.
#*
#*    Rules for the following lists of pseudo-targets are defined.
#*
#*      _DRM_CLEAN_SUBDIR_TARGETS: Cleans all the intermediate object files
#*          and libraries created.
#*
#*      _DRM_BUILD_SUBDIR_TARGETS: Used to recursivley traverse the
#*          subdirectories so that entire subtree can be built.
#*
#*********************************************************************/

#------------------------------------------------------------------------------
# The following targets areused to create a directory where binaries/objs
# will be placed
#------------------------------------------------------------------------------

#Target to create obj directories
_DRM_CREATE_OBJDIR_TARGET:
!if !exist($(_DRM_OBJ_PATH_NO_TRAILING_SLASH))
    @$(_DRM_MAKE_DIR_TOOL_COMMAND) $(_DRM_OBJ_PATH_NO_TRAILING_SLASH)
!endif

#Target to create bin directories
_DRM_CREATE_BINDIR_TARGET:
!if !exist($(_DRM_BIN_PATH_NO_TRAILING_SLASH)) && "$(_DRM_BIN_PATH_NO_TRAILING_SLASH)" != "$(_DRM_OBJ_PATH_NO_TRAILING_SLASH)"
    @$(_DRM_MAKE_DIR_TOOL_COMMAND) $(_DRM_BIN_PATH_NO_TRAILING_SLASH)
!endif

#------------------------------------------------------------------------------
# _DRM_CLEAN_CURRDIR_TARGET
# Remove the binaries in the current directory.
#------------------------------------------------------------------------------

# Remove the binaries in the current directory.
_DRM_CLEAN_CURRDIR_TARGET:
!if "$(_DRM_OBJ_TARGETS)" != "" && exist($(_DRM_OBJ_PATH_NO_TRAILING_SLASH))
    @$(_DRM_DELETE_FILES_TOOL_COMMAND) $(_DRM_OBJ_PATH)*
!endif



#------------------------------------------------------------------------------
# Before we build _DRM_BUILD/CLEAN_SUBDIR_TARGETS target, we need to pass nmake
# switches to the subdirectories we are trying to build
#------------------------------------------------------------------------------



!if exist(.\dirs)

#------------------------------------------------------------------------------
# Each directory in this target list ends with the extension .[TARGET_BUILD_DIR]
# or .[TARGET_BUILD_IGNORE_DIR]. The script uses FOR to extract the extension
# (not elegant, but it works). Non-present directories are skipped by design
# as a shipped DIRS might include directories that haven't been packaged in
# the customer's drop. Note also that this is done inline here, as a launched
# command script would need slightly complex logic to handle multi-arg
# parameters like $(_DRM_MAKE_TOOL_COMMAND). It's simple to use one long line.
#------------------------------------------------------------------------------
$(_DRM_BUILD_SUBDIR_TARGETS)::
    @FOR /F "usebackq delims=" %V IN ('$@') DO \
        @IF "%~xV" == ".[TARGET_BUILD_DIR]" ( \
	    @IF EXIST "$*" ( \
                @echo ^>^>^> Now building %CD%\$* & \
	        cd $* & \
		$(_DRM_MAKE_TOOL_COMMAND) \
            ) ELSE ( \
	        @echo ^>^>^> Skipping directory %CD%\$* ^(not present^) \
	    ) \
	) ELSE ( \
	    @echo ^>^>^> Skipping directory %CD%\$* ^(not requested^) \
	)
    @cd $(MAKEDIR)
!endif


#------------------------------------------------------------------------------
# _DRM_CLEAN_SUBDIR_TARGETS target
# Iterate over all the directories if dirs file
# is present in the current working directory
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# All the directory names have been appended with .[TARGET_CLEAN_DIR]
# So sample directory gets modified to sample.[TARGET_CLEAN_DIR]. The only
# exception is directories for platforms & architectures not being built;
# these get sample.[TARGET_CLEAN_IGNORE_DIR].
#
# When we are building the clean target, we do "@cd $*" to change directory
# to the sub-directory.
#
# $* is an internal nmake macro that represents file name without extension.
#
#------------------------------------------------------------------------------
!if exist(.\dirs)

$(_DRM_CLEAN_SUBDIR_TARGETS)::
    @FOR /F "usebackq delims=" %V IN ('$@') DO \
        @IF "%~xV" == ".[TARGET_CLEAN_DIR]" ( \
	    @IF EXIST "$*" ( \
                @echo ^>^>^> Cleaning directory %CD%\$* & \
                cd $* & \
		$(_DRM_MAKE_TOOL_COMMAND) clean \
            ) ELSE ( \
	        @echo ^>^>^> Skipping directory %CD%\$* ^(not present^) \
	    ) \
	) ELSE ( \
	    @echo ^>^>^> Skipping directory %CD%\$* ^(not requested^) \
	)
    @cd $(MAKEDIR)
!endif

