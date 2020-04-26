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
#*    Defines options specific to wmsp60_arm required for compiling/linking
#*    DRM PK source code
#*
#*    The file is divided into three segments
#*
#*      1. Define the paths where binaries, object files etc are
#*         placed
#*
#*
#*      2. Define the tool chain, compiler, lib, and linker
#*         flags.
#*
#*      3. Replace place holder in the obj target with actual path.
#*
#*  Outputs:
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
#*                   _DRM_OBJ_DIR with a trailing slash ( or back slash
#*                   depending on platform )
#*
#*    _DRM_OBJ_PATH_NO_SLASH: Same as _DRM_OBJ_PATH without trailing slash
#*
#*    _DRM_BIN_DIR: It defines the suffix to _DRM_BINS_PATH_PREFIX path where
#*                  binaries are placed
#*
#*    _DRM_BIN_PATH: It is _DRM_BIN_DIR with a trailing slash
#*
#*    _DRM_BIN_PATH_NO_SLASH: Same as _DRM_BIN_PATH without trailing slash
#*
#*    _DRM_COPY_PATH: It defines the location off of DRM_COPY_ROOT 
#*                    if DRM_COPY_ROOT isn't set
#*
#*    _DRM_POST_PROCESS_TARGET: The target to do post processing
#*
#*********************************************************************/



##############################################################################
#                                                                            #
#  Build definitions needed for composing target lists                       #
#  ---------------------------------------------------                       #
#                                                                            #
##############################################################################


#=============================================================================
# Define output paths based on build type
#=============================================================================

#-------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_DRM_OBJ_TARGETS=$(_DRM_OBJ_TARGETS:PLACEHOLDER_OBJ_PATH\=objchk\wmsp60_arm\)
_DRM_OBJ_DIR=objchk\wmsp60_arm
_DRM_BIN_DIR=objchk\wmsp60_arm
#-------------------------------
!elseif "$(DRM_BUILD_TYPE)" == "FREE"
_DRM_OBJ_TARGETS=$(_DRM_OBJ_TARGETS:PLACEHOLDER_OBJ_PATH\=objfre\wmsp60_arm\)
_DRM_OBJ_DIR=objfre\wmsp60_arm
_DRM_BIN_DIR=objfre\wmsp60_arm
#-------------------------------
!elseif "$(DRM_BUILD_TYPE)" == "PERF"
_DRM_OBJ_TARGETS=$(_DRM_OBJ_TARGETS:PLACEHOLDER_OBJ_PATH\=objperf\wmsp60_arm\)
_DRM_OBJ_DIR=objperf\wmsp60_arm
_DRM_BIN_DIR=objperf\wmsp60_arm
!endif
#-------------------------------

#---------------------------------------------------
# Replace *'s in libpath's with the architecture
#
# This allows user to perform magic like LINKLIBS = lib\*\foo.lib and
# #and if they ensure that there exists lib\wmsp60_arm\foo.lib for platform
# the library the include are automatically platform specific without
# any ugly #ifdef in sources file.
#---------------------------------------------------
LINKLIBS=$(LINKLIBS:*=wmsp60_arm)
TARGETLIBS=$(TARGETLIBS:*=wmsp60_arm)
DRM_COPY_ROOT=$(DRM_COPY_ROOT:*=wmsp60_arm)


#---------------------------------------------------
# Derive additional paths based on above definitions
#---------------------------------------------------

_DRM_OBJ_PATH_NO_TRAILING_SLASH=$(_DRM_OBJ_DIR)
_DRM_BIN_PATH_NO_TRAILING_SLASH=$(_DRM_BIN_DIR)

_DRM_OBJ_PATH=$(_DRM_OBJ_PATH_NO_TRAILING_SLASH)^\
_DRM_BIN_PATH=$(_DRM_BIN_PATH_NO_TRAILING_SLASH)^\
_DRM_COPY_PATH=$(DRM_COPY_ROOT)
!if "$(TARGET_DESTINATION)" != ""
_DRM_COPY_PATH=$(_DRM_COPY_PATH)$(TARGET_DESTINATION)^\
!endif



##############################################################################
#                                                                            #
#  Perform tasks identical in all platarch files                             #
#  -----------------------------------------------------------------------   #
#                                                                            #
##############################################################################

#------------------------------------------------------------------------------
# Define the right flags for the current type of build
#------------------------------------------------------------------------------
!IF "$(DRM_BUILD_TYPE)" == "PERF"
DRM_SUPPORT_PROFILING = 1
!ENDIF

#---------------------------------------
# Set defines based on chk/fre behavior
#   DBG         set to one if checked build, undefined for free.
#---------------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) DBG=1
!endif


#---------------------------------------
# If we're in the test code then set WINCE_TEST
#---------------------------------------
!if "$(_DRM_BUILD_TEST)" == "1"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) WINCE_TEST=1
!endif



##############################################################################
#                                                                            #
#  Build command-lines for tools used once target lists have been composed   #
#  -----------------------------------------------------------------------   #
#                                                                            #
##############################################################################


#=============================================================================
# Build up the compiler command-line
#=============================================================================

#-------------------------------------
# Select the appropriate compiler tool
#-------------------------------------

!if "$(DRM_BUILD_C_COMPILER_TOOL)" != ""
_C_COMPILER_TOOL = $(DRM_BUILD_C_COMPILER_TOOL)
!else
_C_COMPILER_TOOL = cl.exe
!endif


#---------------------------------------
# Add platform-specific defines
#   SDK: WIN32=300, WINVER=0x501      Standard definitions
#   SDK: WINCE, _WIN32_WCE=0x501      Standard definitions
#   SDK: _WINDOWS, UNDER_CE=0x501     Standard definitions
#   SDK: WIN32_PLATFORM_WFSP          Standard definitions
#   SDK: SMARTPHONE2003_UI_MODEL      Standard definitions
#   SDK: ARM                          Standard definitions
#   SDK: _DEBUG/NDEBUG                Standard definitions
#   SDK: MAGNETO                      Default to building for Magneto, currently selects which obfuscation constants to use
#---------------------------------------
_DRM_C_DEFINES = $(_DRM_C_DEFINES) WIN32=300 WINVER=0x501
_DRM_C_DEFINES = $(_DRM_C_DEFINES) WINCE _WIN32_WCE=0x501
_DRM_C_DEFINES = $(_DRM_C_DEFINES) _WINDOWS UNDER_CE=0x501
_DRM_C_DEFINES = $(_DRM_C_DEFINES) WIN32_PLATFORM_WFSP SMARTPHONE2003_UI_MODEL
_DRM_C_DEFINES = $(_DRM_C_DEFINES) ARM
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) _DEBUG
!else
_DRM_C_DEFINES = $(_DRM_C_DEFINES) NDEBUG
!endif
_DRM_C_DEFINES = $(_DRM_C_DEFINES) MAGNETO

#---------------------------------------
# Add architecture-specific defines (used by DRM and SDKs)
#   _ARM_       defined only for ARM compilation
#---------------------------------------
_DRM_C_DEFINES = $(_DRM_C_DEFINES) _ARM_


#---------------------------------------
# Set switches common to all builds
#   -nologo     don't display logo and version information during compilation
#   -FC         force full directory path information in output
#   -c          compile only, no link
#   -W3         use warning level 3
#   -WX         treat warnings as errors
#   -GS-        disable security checks (not a production configuration)
#   -Gy         place functions in separate COMDATs so linker can handle them separately
#   -Z7         place debug symbols in the objs for later use by the linker
#---------------------------------------
_C_COMPILER_SWITCHES = -nologo -FC -c -W4 -WX -GS- -Gy -Z7


#---------------------------------------
# Set switches based on chk/fre behavior
#   -Ob1        expands only functions marked as inline
#   -Ob2        expands functions marked as inline and at the compiler's discretion
#   -Od         disables optimization
#   -Oi         generates intrinsic functions
#   -Ot         favors fast code (-Os favors small size)
#   -Ox         uses maximum optimization (/Ob2gity /Gs)
#---------------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -Odib1
!else
_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -Oxt
!endif


#----------------------------------------------
# Set switch for C++ exception handling support
#----------------------------------------------
!if "$(DRM_BUILD_USE_CPPEH)" == "1"
_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -EHsc
!endif


#------------------------------------------------------------------------------
# Add the -D prefix to each define. Do this by using spaces as locations for
# insertion. Steps:
# 1. Canonicalize the spacing:
#    a. Convert tabs to spaces.
#    b. Ensure only one space exists between items.
#    c. Remove extraneous spaces at the beginning or end of the line.
# 2. Replace spaces with the prefix.
#------------------------------------------------------------------------------

_C_DEFINE_SWITCHES   =$(_DRM_C_DEFINES)

# Add sentinels. A space at the beginning/end of a line now has a neighboring space.
_C_DEFINE_SWITCHES   =[START_OF_LINE_SENTINEL] $(_C_DEFINE_SWITCHES) [END_OF_LINE_SENTINEL]

# Trim the tabs	
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:	= )

# Ensure no space borders another (optimal pattern is a fibonacci sequence).
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:                                  = )
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:                     = )
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:             = )
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:        = )
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:     = )
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:   = )
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:  = )

# Remove the sentinels, taking with them the only remaining spaces at the
# start/end of the line.
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES:[START_OF_LINE_SENTINEL] =)
_C_DEFINE_SWITCHES   =$(_C_DEFINE_SWITCHES: [END_OF_LINE_SENTINEL]=)

# Now add the "-D" prefix to each statement
!if "$(_C_DEFINE_SWITCHES)" != " "
_C_DEFINE_SWITCHES   =-D$(_C_DEFINE_SWITCHES: = -D)
!endif


#------------------------------------------------------------------------------
# Add the -I prefix to each include path and wrap each path in quotes. Do this
# by using separating ;'s as locations for insertion and wrapping. Steps:
# 1. Strip existing quotes
# 2. Remove any spaces after semicolons
# 3. Remove spaces at the beginning of the line
# 4. Use semicolons as locations to insert -I prefixes.
#------------------------------------------------------------------------------

_C_INCLUDE_SWITCHES  =$(INCLUDES)

# Add sentinels. A space at the beginning/end of a line now has a neighboring semicolon.
_C_INCLUDE_SWITCHES  =[START_OF_LINE_SENTINEL];$(_C_INCLUDE_SWITCHES);[END_OF_LINE_SENTINEL]

# Remove any " from the path. Adding quotes twice results in a error.
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:"=)

# Remove tabs after semicolon
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;	=;)

# Remove spaces after semicolons (optimal pattern is a fibonacci sequence).
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;                                  =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;                     =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;             =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;        =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;     =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;   =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;  =;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:; =;)

# Remove double semicolons
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;;=;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;;=;)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;;=;)

# Remove the sentinels, taking with them the only remaining semicolons at the
# start/end of the line.
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:[START_OF_LINE_SENTINEL];=)
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES:;[END_OF_LINE_SENTINEL]=)

# Replace semicolons with -I<path>
!if "" != "$(_C_INCLUDE_SWITCHES)"
_C_INCLUDE_SWITCHES  =-I"$(_C_INCLUDE_SWITCHES:;=" -I")"
!endif

# Add the current path.
_C_INCLUDE_SWITCHES  =$(_C_INCLUDE_SWITCHES) -I"."

			
#------------------------------------
# Define final compiler commandlines
#------------------------------------

# C++ compiler
_DRM_CPP_COMPILER_TOOL_COMMAND=$(_C_COMPILER_TOOL) \
    $(_C_COMPILER_SWITCHES) \
    $(_C_DEFINE_SWITCHES) \
    $(_C_INCLUDE_SWITCHES) \
    -Fo$(_DRM_OBJ_PATH)

# C compiler
_DRM_C_COMPILER_TOOL_COMMAND=$(_C_COMPILER_TOOL) \
    $(_C_COMPILER_SWITCHES) \
    $(_C_DEFINE_SWITCHES) \
    $(_C_INCLUDE_SWITCHES) \
    -Fo$(_DRM_OBJ_PATH)



#=============================================================================
# Build up the linker command-line
#=============================================================================

#--------------------------------------
# Select the appropriate linker tool
#--------------------------------------

!if "$(DRM_BUILD_C_LINKER_TOOL)" != ""
_C_LINKER_TOOL = $(DRM_BUILD_C_LINKER_TOOL)
!else
_C_LINKER_TOOL = link.exe
!endif


#---------------------------------------
# Set switches common to all builds
#   -nologo     don't display logo and version information during compilation
#   -machine    specifies architecture of lib file
#   -SUBSYSTEM  specifies Windows CE app
#   -NODEFAULTLIB:oldnames - Work around documented bug in VS2005. The
#       oldnames.lib library does not exist for WinMobile, only desktop. Due
#       to a bug in VS2005 it is getting implicitly included in the build and
#       shouldn't be, so need to tell VS to not link it.
#   -DEBUG      produce PDB files (changes default behavior of -OPT)
#
# Note: See _DRM_BIN_LINKER_TOOL_COMMAND for why nologo is handled specially.
#---------------------------------------
_C_LINKER_NOLOGO_SWITCH = -nologo
_C_LINKER_SWITCHES = -machine:THUMB -SUBSYSTEM:WINDOWSCE -NODEFAULTLIB:oldnames.lib -DEBUG


#---------------------------------------
# Set switches based on chk/fre behavior
#   -OPT:REF|NOREF  controls whether only referenced functions & data are kept
#   -OPT:ICF|NOICF  controls whether identical functions & data blocks are
#                   folded (merged)
#---------------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_C_LINKER_SWITCHES = $(_C_LINKER_SWITCHES) -OPT:NOREF -OPT:NOICF
!else
_C_LINKER_SWITCHES = $(_C_LINKER_SWITCHES) -OPT:REF -OPT:ICF
!endif


#------------------------------------------------------------------------------
# Add the -LIBPATH prefix to each lib path and wrap each path in quotes. Do
# this by using separating ;'s as locations for insertion and wrapping. Steps:
# 1. Strip existing quotes
# 2. Remove any spaces after semicolons
# 3. Remove spaces at the beginning of the line
# 4. Use semicolons as locations to insert prefixes.
#------------------------------------------------------------------------------

_LIBPATH_SWITCHES    =$(LIBPATH)

# Add sentinels. A space at the beginning/end of a line now has a neighboring semicolon.
_LIBPATH_SWITCHES    =[START_OF_LINE_SENTINEL];$(_LIBPATH_SWITCHES);[END_OF_LINE_SENTINEL]

# Remove any " from the path. Adding quotes twice results in a error.
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:"=)

# Remove tabs after semicolon
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;	=;)

# Remove spaces after semicolons (optimal pattern is a fibonacci sequence).
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;                                  =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;                     =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;             =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;        =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;     =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;   =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;  =;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:; =;)

# Remove double semicolons
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;;=;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;;=;)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;;=;)

# Remove the sentinels, taking with them the only remaining semicolons at the
# start/end of the line.
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:[START_OF_LINE_SENTINEL];=)
_LIBPATH_SWITCHES    =$(_LIBPATH_SWITCHES:;[END_OF_LINE_SENTINEL]=)

# Replace semicolons with -LIBPATH:<path>
!if "" != "$(_LIBPATH_SWITCHES)"
_LIBPATH_SWITCHES    =-LIBPATH:"$(_LIBPATH_SWITCHES:;=" -LIBPATH:")"
!endif


#--------------------------------
# Append standard libaries
#--------------------------------
TARGETLIBS = $(TARGETLIBS) libcmt.lib coredll.lib corelibc.lib gx.lib Aygshell.lib


#--------------------------------
# Define final linker commandline
#--------------------------------

# Note: build.exe watches for the pattern "link.exe -nologo -out:<blah>".
# That ordering must be preserved.
_DRM_BIN_LINKER_TOOL_COMMAND=$(_C_LINKER_TOOL) \
    $(_C_LINKER_NOLOGO_SWITCH) -OUT:$(_DRM_BIN_TARGET) \
    $(_C_LINKER_SWITCHES) \
    $(LINKLIBS) \
    $(TARGETLIBS) \
    $(_LIBPATH_SWITCHES) \
    -PDB:$(_DRM_SYM_TARGET)


# DLL LINKER
_DRM_DLL_LINKER_TOOL_COMMAND=$(_C_LINKER_TOOL) \
    $(_C_LINKER_NOLOGO_SWITCH) -OUT:$(_DRM_BIN_TARGET) \
    $(_C_LINKER_SWITCHES) \
    $(LINKLIBS) \
    $(TARGETLIBS) \
    $(_LIBPATH_SWITCHES) \
    /DLL \
    -PDB:$(_DRM_SYM_TARGET)

!if "" != "$(DLLDEF)"
_DRM_DLL_LINKER_TOOL_COMMAND=$(_DRM_DLL_LINKER_TOOL_COMMAND) \
    /DEF:$(DLLDEF)
!endif



#=============================================================================
# Build up the librarian command-line
#=============================================================================

#--------------------------------------
# Select the appropriate librarian tool
#--------------------------------------

!if "$(DRM_BUILD_C_LIBRARIAN_TOOL)" != ""
_C_LIBRARIAN_TOOL = $(DRM_BUILD_C_LIBRARIAN_TOOL)
!else
_C_LIBRARIAN_TOOL = lib.exe
!endif


#---------------------------------------
# Linker warnings to ignore (separated by comma):
#
# LNK4221 - Happens when an obj file lacking any contents is added to a lib.
#           This typically occurs when the contents of the file have been
#           disabled by #ifdef.
#---------------------------------------
_C_LIBRARIAN_IGNORE_LIST = 4221


#---------------------------------------
# Set switches common to all builds
#   -nologo     don't display logo and version information during linkage
#   -machine    specifies architecture of lib file
#   -IGNORE     specifies link warnings that should be ignored
#
# Note: See _DRM_LIBRARIAN_TOOL_COMMAND for why nologo is handled specially.
#---------------------------------------
_C_LIBRARIAN_NOLOGO_SWITCH = -nologo
_C_LIBRARIAN_SWITCHES = -machine:THUMB -IGNORE:$(_C_LIBRARIAN_IGNORE_LIST)


#-----------------------------------
# Define final librarian commandline
#-----------------------------------

# Note: build.exe watches for the pattern "lib.exe -nologo -out:<blah>".
# That ordering must be preserved.
_DRM_LIBRARIAN_TOOL_COMMAND=$(_C_LIBRARIAN_TOOL) \
    $(_C_LIBRARIAN_NOLOGO_SWITCH) -OUT:$(_DRM_BIN_TARGET) \
    $(_C_LIBRARIAN_SWITCHES) \
    $(OBJLIBFILES)           \
    $(_LIBPATH_SWITCHES)

					

#=============================================================================
# Build up the make command-line
#=============================================================================

#-------------------------------
# Set make switches
#-------------------------------

# Ensure spaces don't build up during each recursive step.
_MAKE_SWITCHES=$(MAKEFLAGS)
_MAKE_SWITCHES=$(_MAKE_SWITCHES:                                  = )
_MAKE_SWITCHES=$(_MAKE_SWITCHES:                     = )
_MAKE_SWITCHES=$(_MAKE_SWITCHES:             = )
_MAKE_SWITCHES=$(_MAKE_SWITCHES:        = )
_MAKE_SWITCHES=$(_MAKE_SWITCHES:     = )
_MAKE_SWITCHES=$(_MAKE_SWITCHES:   = )
_MAKE_SWITCHES=$(_MAKE_SWITCHES:  = )

!if "$(_MAKE_SWITCHES)" == " "
_MAKE_SWITCHES=
!endif

# If there are flags to recursively pass on, do so.
!if "$(_MAKE_SWITCHES)" != ""
_MAKE_SWITCHES=-$(_MAKE_SWITCHES)
!endif


#-------------------------------
# Define final nmake commandline
#-------------------------------

_DRM_MAKE_TOOL_COMMAND = $(MAKE) $(_MAKE_SWITCHES) /NOLOGO



#=============================================================================
# Build up shell tool command-lines
#=============================================================================

_DRM_DELETE_FILES_TOOL_COMMAND=del /f /Q
_DRM_MAKE_DIR_TOOL_COMMAND=>nul 2>nul md
_DRM_COPY_TO_DIR_COMMAND=@>nul xcopy /y



##############################################################################
#                                                                            #
#  Declare a generic post-build target that can be used for things like      #
#  deploying bits                                                            #
#  --------------------------------------------------------------------      #
#                                                                            #
##############################################################################

_DRM_POST_PROCESS_TARGET:
