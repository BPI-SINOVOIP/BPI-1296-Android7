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
#*    Defines options specific to i386 required for compiling/linking
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
#*    _DRM_VERSIONING_TOOL_COMMAND: Command to create a versioning object for 
#*                            linking with binaries.
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
#*    _DRM_BIN_EXT: The extension for a binary on current platform
#*
#*    _DRM_POST_PROCESS_TARGET: The target to do post processing
#*
#*********************************************************************/


##############################################################################
#                                                                            #
#  Build definitions needed for composing target lists                       #
#  ---------------------------------------------------			     #
#                                                                            #
##############################################################################


#=============================================================================
# Define output paths based on build type
#=============================================================================

#-------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_DRM_OBJ_TARGETS=$(_DRM_OBJ_TARGETS:PLACEHOLDER_OBJ_PATH=objchk\i386\)
_DRM_OBJ_DIR=objchk\i386
_DRM_BIN_DIR=objchk\i386
_DRM_DROP_DIR=bind\i386
#-------------------------------
!elseif "$(DRM_BUILD_TYPE)" == "FREE"
_DRM_OBJ_TARGETS=$(_DRM_OBJ_TARGETS:PLACEHOLDER_OBJ_PATH=objfre\i386\)
_DRM_OBJ_DIR=objfre\i386
_DRM_BIN_DIR=objfre\i386
_DRM_DROP_DIR=bin\i386
#-------------------------------
!elseif "$(DRM_BUILD_TYPE)" == "PERF"
_DRM_OBJ_TARGETS=$(_DRM_OBJ_TARGETS:PLACEHOLDER_OBJ_PATH=objperf\i386\)
_DRM_OBJ_DIR=objperf\i386
_DRM_BIN_DIR=objperf\i386
_DRM_DROP_DIR=binp\i386
!endif
#-------------------------------

#---------------------------------------------------
# Derive additional paths based on above definitions
#---------------------------------------------------

_DRM_OBJ_PATH_NO_TRAILING_SLASH=$(_DRM_OBJ_PATH_PREFIX)\$(_DRM_OBJ_DIR)
_DRM_BIN_PATH_NO_TRAILING_SLASH=$(_DRM_BIN_PATH_PREFIX)\$(_DRM_BIN_DIR)
!if "$(_DRM_BUILD_TEST)" == "1"
_DRM_DROP_PATH_NO_TRAILING_SLASH=$(_DRM_TEST_PATH)\$(_DRM_DROP_DIR)
!else
_DRM_DROP_PATH_NO_TRAILING_SLASH=$(_DRM_SOURCE_PATH)\$(_DRM_DROP_DIR)
!endif

_DRM_OBJ_PATH=$(_DRM_OBJ_PATH_NO_TRAILING_SLASH)^\
_DRM_BIN_PATH=$(_DRM_BIN_PATH_NO_TRAILING_SLASH)^\
_DRM_DROP_PATH=$(_DRM_DROP_PATH_NO_TRAILING_SLASH)^\

#---------------------------------------------------
# Replace *'s in libpath's with the architecture
#
# This allows user to perform magic like LINKLIBS = lib\*\foo.lib and
# #and if they ensure that there exists lib\i386\foo.lib for X86 platform
# the library the include are automatically platform specific without
# any ugly #ifdef in sources file.
#---------------------------------------------------
LINKLIBS=$(LINKLIBS:*=i386)
TARGETLIBS=$(TARGETLIBS:*=i386)


#=============================================================================
# Define extensions for various file types
#=============================================================================

_DRM_BIN_EXT = exe



##############################################################################
#                                                                            #
#  Perform Microsoft-specific tasks for internal builds                      #
#  -----------------------------------------------------------------------   #
#                                                                            #
##############################################################################

!if "$(_DRM_BUILD_TOOL)" == "NTBUILD"

#------------------------------------------------------------------------------
# If we are using build, make sure we have the include path defined, and where
# the common library path is defined
#------------------------------------------------------------------------------

_DRM_COMMON_LIB_DIR=$(SDXROOT)\multimedia\pk\common\lib

INCLUDES=$(INCLUDES); \
         $(PUBLIC_ROOT)\oak\inc; \
         $(PUBLIC_ROOT)\sdk\inc; \
         $(PUBLIC_ROOT)\sdk\inc\crt; \
         $(DS_INC_PATH)\crypto;  \

SDK_LIB_PATH=$(PUBLIC_ROOT)\sdk\lib\i386

LIBPATH= $(LIBPATH);$(SDK_LIB_PATH)

# Also, select the versioning template file.
_VERSIONING_TEMPLATE_FILE=$(SDXROOT)\multimedia\pk\wmdrm-md\source\inc\PlayReadyVersions.ver

# --------------------------------------------------------------------------
# On Win32 free/perf builds, use non-DRM CRT option for best option coverage
# --------------------------------------------------------------------------
!if "$(DRM_BUILD_TYPE)" != "CHECKED"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) NO_DRM_CRT
!endif

!endif # of !if "$(_DRM_BUILD_TOOL)" == "NTBUILD"



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

#-------------------------------------------------------------
# If we're building the test framework we need to set PK_TEST
# Lots of test cases will fail if this isn't set
#-------------------------------------------------------------
!if "$(PK_TEST)" == "1"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) PK_TEST=1
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
!elseif "$(_DRM_BUILD_TOOL)" == "NTBUILD" && "$(BUILD_OACR)" == "1"
_C_COMPILER_TOOL = oacrcl.exe
!else
_C_COMPILER_TOOL = cl.exe
!endif


#---------------------------------------
# Add platform-specific defines
#   _CRT_SECURE_NO_DEPRECATE    Allow use of ANSI CRT functions deprecated for security
#   _CRT_SECURE_FORCE_DEPRECATE Same as above but for CoreXT (internal) sdk
#   _CRT_OBSOLETE_NO_DEPRECATE  Allow use of ANSI CRT functions that are obsoleted
#
#   SDK: WIN32_LEAN_AND_MEAN=1        Brings in minimal Win32 definitions from windows.h
#   SDK: WIN32=100                    Standard definitions
#   SDK: WINVER=XXX                   Either 0x600 (vista) or 0x500 (NO_MUI)
#   SDK: WINNT=1                      Standard definitions
#   SDK: _WIN32_WINNT=XXX             Either 0x600 (vista) or 0x500 (NO_MUI)
#   DRM: _WIN32 		      Note: use is inconsistent
#
# Note 1: Since Win32 isn't an official platform yet, infer Win32 from use of
#         NTBUILD and ANSI from use of nmake.
#---------------------------------------

!if "$(_DRM_BUILD_TOOL)" == "NTBUILD"
!if "$(_NT_TARGET_VERSION)" == "0x501"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) WIN32_LEAN_AND_MEAN=1 WIN32=100 WINVER=0x500 WINNT=1 _WIN32_WINNT=0x500 _WIN32
!else
_DRM_C_DEFINES = $(_DRM_C_DEFINES) WIN32_LEAN_AND_MEAN=1 WIN32=100 WINVER=0x600 WINNT=1 _WIN32_WINNT=0x600 _WIN32
!endif
!else #if "$(_DRM_BUILD_TOOL)" == "NMAKE"
_DRM_C_DEFINES = $(_DRM_C_DEFINES) _CRT_SECURE_NO_DEPRECATE _CRT_OBSOLETE_NO_DEPRECATE _CRT_SECURE_FORCE_DEPRECATE
!endif

#---------------------------------------
# Add architecture-specific defines (used by DRM and SDKs)
#   _X86_, _M_IX86       defined only for x86 compilation
#---------------------------------------
_DRM_C_DEFINES = $(_DRM_C_DEFINES) _X86_ _M_IX86


#---------------------------------------
# Set switches common to all builds
#   -nologo     don't display logo and version information during compilation
#   -FC         force full directory path information in output
#   -c          compile only, no link
#   -W4         use warning level 4
#   -WX         treat warnings as errors
#   -GS         enable security checks
#   -Gy         place functions in separate COMDATs so linker can handle them separately
#   -Gz         stdcall calling convention by default (optimal perf-wise)
#   -Zp8        pack structs on 8 byte boundaries
#   -Z7         place debug symbols in the objs for later use by the linker
#---------------------------------------
_C_COMPILER_SWITCHES = -nologo -FC -c -W4 -WX -GS -Gy -Zp8 -Z7 -Gz


#---------------------------------------
# Set switches based on chk/fre behavior
#   -Ob1        expands only functions marked as inline
#   -Ob2        expands functions marked as inline and at the compiler's discretion
#   -Od         disables optimization
#   -Oi         generates intrinsic functions
#   -Ot         favors fast code (-Os favors small size)
#   -Ox         uses maximum optimization (/Ob2gity /Gs)
#   -Oy         enable x86 frame pointer optimization (-Oy- disables it)
#   -Gm         enable minimal rebuild (-Gm- disables it)
#---------------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -Odib1y- -Gm-
!else
_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -Oxt
!endif

#---------------------------------------
# Set switches based on platform.
#   -Za         disable language extensions, i.e. force ANSI compliance.
#   -TP         force compilation using C++, regardless of file extension
#
# Note 1: Since Win32 isn't an official platform yet, infer Win32 from use of
#         NTBUILD and ANSI from use of nmake.
# Note 2: For best coverage we use -TP in Win32 builds but not in ANSI builds.
#---------------------------------------
!if "$(_DRM_BUILD_TOOL)" != "NTBUILD"

#-------------------------------------------------------------
# This section - compilation with nmake from installed package
# If we're building the test framework we can't test for ANSI
# Compliant code.
#
# This is all turned off for now because of cross project
# dependencies.
#-------------------------------------------------------------
#!if "$(_DRM_BUILD_TEST)" != "1"
#_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -Za
#!endif

!else

#-------------------------------------------------------------
# This section - internal compilation with bcz
# We enable C++ compilation of all files to ensure that code is
# C++ compliable
#-------------------------------------------------------------

_C_COMPILER_SWITCHES = $(_C_COMPILER_SWITCHES) -TP

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

			
#-------------------------------------
# Define source canonicalization macro
#-------------------------------------
!if "$(SDXROOT)" != ""
_DRM_CANONICALIZE_SOURCE_COMMAND=@perl -0777 -e "foreach $$file (@ARGV){open(FH,'+<',$$file);$$RxD=\"@\x23\x23\x23GNO_JNF_URER\x23\x23\x23@\";$$RxD=~tr/NORUEFGJ/ABEHRSTW/;while (<FH>){$$n=s/\t/$$RxD/g;$$Ft=$$_};if ($$n != 0){seek(FH,0,0);print FH $$Ft;}close(FH);}"
!else
_DRM_CANONICALIZE_SOURCE_COMMAND=@rem
!endif


#------------------------------------
# Define final compiler commandlines
#------------------------------------

# C++ compiler
_DRM_CPP_COMPILER_TOOL_COMMAND=$(_C_COMPILER_TOOL) \
    $(_C_COMPILER_SWITCHES) \
    $(_C_DEFINE_SWITCHES) \
    $(_C_INCLUDE_SWITCHES) \
    -Fo$(_DRM_OBJ_PATH)/

# C compiler
_DRM_C_COMPILER_TOOL_COMMAND=$(_C_COMPILER_TOOL) \
    $(_C_COMPILER_SWITCHES) \
    $(_C_DEFINE_SWITCHES) \
    $(_C_INCLUDE_SWITCHES) \
    -Fo$(_DRM_OBJ_PATH)/



#=============================================================================
# Build up the version tool command-line
#=============================================================================

!if ("$(_DRM_BUILD_TOOL)" == "NTBUILD") && ("$(TARGETTYPE)" == "PROGRAM")
#--------------------------------
# Define versioning target
#--------------------------------

_VERSIONING_TARGET = $(_DRM_OBJ_PATH)Version.res

#--------------------------------
# Define versioning tool
#--------------------------------

_DRM_VERSIONING_TOOL = rc.exe

#---------------------------------------
# Set switches
#   -r          emit .res file
#   -D          C includes to pass to the rc compiler
#---------------------------------------
!if "$(VER_FILEDESCRIPTION_STR)" == ""
#!error "VER_FILEDESCRIPTION_STR must be defined in SOURCES and contain a description of the application"
VER_FILEDESCRIPTION_STR="Precompiled Tool"
!endif

!if "$(BETA)" == ""
_BETA_VAL=0
!else
_BETA_VAL=$(BETA)
!endif

!if "$(OFFICIAL_BUILD)" == ""
_OFFICIAL_BUILD_VAL=0
!else
_OFFICIAL_BUILD_VAL=$(OFFICIAL_BUILD)
!endif

_DRM_VERSIONING_TOOL_SWITCHES = -r \
	-DOFFICIAL_BUILD=$(_OFFICIAL_BUILD_VAL) \
	-DBETA=$(_BETA_VAL) \
	-DPLAYREADY_PRODUCT_DEVICE_PK=1 \
	-DVER_FILEDESCRIPTION_STR=\"$(VER_FILEDESCRIPTION_STR)\" \
	-DVER_FILETYPE=VFT_APP \
	-DVER_FILESUBTYPE=VFT_UNKNOWN \
	-DVER_INTERNALNAME_STR=\"$(TARGETNAME).$(_DRM_BIN_EXT)\"

#--------------------------------
# Define versioning tool commandline
#--------------------------------

_DRM_VERSIONING_TOOL_COMMAND=$(_DRM_VERSIONING_TOOL)\
    $(_DRM_VERSIONING_TOOL_SWITCHES) \
    $(_C_DEFINE_SWITCHES) \
    $(_C_INCLUDE_SWITCHES) \
    -Fo $(_VERSIONING_TARGET) \
    $(_VERSIONING_TEMPLATE_FILE)

!endif



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
#   -nologo        Don't display logo and version information during
#                  compilation.
#   -machine       Specifies architecture of lib file.
#   -DEBUG         Produce PDB files (changes default behavior of -OPT).
#   -SAFESEH       Include a table of legal exception handlers enforcable by
#                  the OS.
#   -NXCOMPAT      Code was validated with no-execution of data segments.
#   -DYNAMICBASE   On Vista and later, let OS select the binary's base address
#                  Requires VS2005 or VS2008.
#
# Note: See _DRM_BIN_LINKER_TOOL_COMMAND for why nologo is handled specially.
#---------------------------------------
_C_LINKER_NOLOGO_SWITCH = -nologo
_C_LINKER_SWITCHES = $(_C_LINKER_SWITCHES) -machine:X86 -DEBUG -SAFESEH -NXCOMPAT -DYNAMICBASE

#---------------------------------------
# Set switches based on chk/fre behavior
#   -OPT:REF|NOREF  controls whether only referenced functions & data are kept
#   -OPT:ICF|NOICF  controls whether identical functions & data blocks are
#                   folded (merged)
#   -DEBUGTYPE:cv,fixup
#		    put debugging information in CodeView format
#   -INCREMENTAL:no disable incremental linking
#---------------------------------------
!if "$(DRM_BUILD_TYPE)" == "CHECKED"
_C_LINKER_SWITCHES = $(_C_LINKER_SWITCHES) -OPT:NOREF -OPT:NOICF -DEBUGTYPE:cv,fixup -INCREMENTAL:no
!else
_C_LINKER_SWITCHES = $(_C_LINKER_SWITCHES) -OPT:REF -OPT:ICF
!endif


#---------------------------------------
# Set switches controlling Win32 file output type
#---------------------------------------

!ifdef UMTYPE
_C_LINKER_SWITCHES = $(_C_LINKER_SWITCHES) -SUBSYSTEM:$(UMTYPE)
!endif

#---------------------------------------
# Include a C runtime library for those needing one
#---------------------------------------
TARGETLIBS=$(TARGETLIBS) $(_VERSIONING_TARGET) LIBCMT.LIB


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
# Define final linker commandline
#--------------------------------

# Note: build.exe watches for the pattern "link.exe -nologo -out:<blah>".
# That ordering must be preserved.
_DRM_BIN_LINKER_TOOL_COMMAND=$(_C_LINKER_TOOL) \
    $(_C_LINKER_NOLOGO_SWITCH) -OUT:$(_DRM_BIN_PATH)$(_DRM_BIN_TARGETS) \
    $(_C_LINKER_SWITCHES) \
    $(LINKLIBS) \
    $(TARGETLIBS) \
    $(_LIBPATH_SWITCHES) \
    -PDB:$(_DRM_BIN_PATH)$(TARGETNAME).pdb


# DLL LINKER
_DRM_DLL_LINKER_TOOL_COMMAND=$(_C_LINKER_TOOL) \
    $(_C_LINKER_NOLOGO_SWITCH) -OUT:$(_DRM_BIN_PATH)$(_DRM_BIN_TARGETS) \
    $(_C_LINKER_SWITCHES) \
    $(LINKLIBS) \
    $(TARGETLIBS) \
    $(_LIBPATH_SWITCHES) \
    /DLL \
    -PDB:$(_DRM_BIN_PATH)$(TARGETNAME).pdb

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
_C_LIBRARIAN_SWITCHES = -machine:X86 -IGNORE:$(_C_LIBRARIAN_IGNORE_LIST)


#-----------------------------------
# Define final librarian commandline
#-----------------------------------

# Note: build.exe watches for the pattern "lib.exe -nologo -out:<blah>".
# That ordering must be preserved.
_DRM_LIBRARIAN_TOOL_COMMAND=$(_C_LIBRARIAN_TOOL) \
    $(_C_LIBRARIAN_NOLOGO_SWITCH) -OUT:$(_DRM_BIN_PATH)$(_DRM_BIN_TARGETS) \
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



##############################################################################
#                                                                            #
#  Declare a generic post-build target that can be used for things like      #
#  deploying bits                                                            #
#  --------------------------------------------------------------------      #
#                                                                            #
##############################################################################

_DRM_POST_PROCESS_TARGET:
!if "$(TARGETTYPE)" == "PROGRAM" || "$(TARGETTYPE)" == "LIBRARY" || ( "$(TARGETTYPE)" == "DYNLINK" && "$(_DRM_BUILD_BINARY_TARGETS)" == "1" )
    @-$(_DRM_MAKE_DIR_TOOL_COMMAND) $(_DRM_DROP_PATH_NO_TRAILING_SLASH)
    @copy $(_DRM_BIN_PATH)$(_DRM_BIN_TARGETS) $(_DRM_DROP_PATH) >nul
!   if "$(TARGETTYPE)" == "PROGRAM" || ( "$(TARGETTYPE)" == "DYNLINK" && "$(_DRM_BUILD_BINARY_TARGETS)" == "1" )
        @copy $(_DRM_BIN_PATH)$(TARGETNAME).pdb $(_DRM_DROP_PATH) >nul
!       if "$(TARGETTYPE)" == "DYNLINK" && "$(_DRM_BUILD_BINARY_TARGETS)" == "1" && "$(DRM_EXPORT_APIS_TO_DLL)" == "1"
            @copy $(_DRM_BIN_PATH)$(TARGETNAME).lib $(_DRM_DROP_PATH) >nul
!       endif
!   endif
!endif

