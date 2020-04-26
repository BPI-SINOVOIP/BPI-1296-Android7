
#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#*
#* Description:
#*    Includes the right processor specific definitions
#*
#* Defines the following commands and variables
#*
#*    _DRM_C_COMPILER_TOOL_COMMAND: The C compiler for this platform
#*
#*    _DRM_CPP_COMPILER_TOOL_COMMAND: The C++ compiler for this platform
#*
#*    _DRM_MAKE_DIR_TOOL_COMMAND: The command to create a directory
#*
#*    _DRM_DELETE_FILES_TOOL_COMMAND: The command to delete a file
#*
#*    _DRM_BIN_LINKER_TOOL_COMMAND: The command for linking objs and libraries
#*                             to a binary
#*
#*    _DRM_LIBRARIAN_TOOL_COMMAND: Command to create a library from a list of
#*                            objs
#*
#*    _DRM_OBJ_TARGETS: The list of object files used to create the final
#*                      binary target. This is an IN/OUT parameter. Input
#*                      is a list of files with their extensions replaced
#*                      with obj and PLACEHOLDER_OBJ_PATH prepended to
#*                      them. Output is PLACEHOLDER_OBJ_PATH with the path
#*                      where the obj files are placed
#*
#*    _DRM_OBJ_DIR: It defines the suffix to _DRM_OBJ_PATH_PREFIX path where
#*                  objs are placed
#*
#*    _DRM_OBJ_PATH: It is a contacatenation of _DRM_OBJ_PATH_PREFIX and
#*                   _DRM_OBJ_DIR with a leading slash( or back slash
#*                   depending on platform )
#*
#*    _DRM_OBJ_PATH_NO_SLASH: Same as _DRM_OBJ_PATH without leadin slash
#*
#*    _DRM_BIN_DIR: It defines the suffix to _DRM_BINS_PATH_PREFIX path where
#*                  binaries are placed
#*
#*    _DRM_BIN_PATH: It is a contacatenation of _DRM_BIN_PATH_PREFIX and
#*                   _DRM_BIN_DIR with a leading slash( or back slash
#*                   depending on platform )
#*
#*    _DRM_BIN_PATH_NO_SLASH: Same as _DRM_BIN_PATH without leadin slash
#*
#*********************************************************************/


#------------------------------------------------------------------------------
# For i386 and amd64 we might be using build as well.
# Get the right defintions based on whether we are
# using build or not
#------------------------------------------------------------------------------

!if "$(DRM_BUILD_PLATARCH)"=="ANSI_X86" || "$(DRM_BUILD_PLATARCH)"=="WIN32_X86"
!   include i386.mk
#-------------------------------
!elseif "$(DRM_BUILD_PLATARCH)"=="ANSI_X64" || "$(DRM_BUILD_PLATARCH)"=="WIN32_X64"
!   include amd64.mk
#-------------------------------
!elseif "$(DRM_BUILD_PLATARCH)"=="WMSP60_ARM"
!   include wmsp60_arm.mk
#-------------------------------

!endif

