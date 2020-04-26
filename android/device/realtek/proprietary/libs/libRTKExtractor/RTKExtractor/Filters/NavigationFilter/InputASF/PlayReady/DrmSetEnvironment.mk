
#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#*********************************************************************
#
#
# Description:
#    Defines the necessary environment variables
#
# Outputs:
#   - DRM_TARGET_TYPE: FREE, CHECKED, PERF
#   - DRM_BUILD_ARCH: X86, X64, ARM, ...
#   - DRM_BUILD_PLATFORM: ANSI, WIN32, WMSP50, ...
#   - DRM_ROOT_PATH: <some path, with trailing slash>
#   - _DRM_BUILD_TOOL: NMAKE, NTBUILD, ...
#   - _DRM_BUILD_PLATARCH: ANSI_X86, ...
#   - _DRM_BUILD_PATH: <off of DRM_ROOT_PATH, with trailing slash>
#   - Forward declared targets: DRM_BUILD_TARGET, DRM_CLEAN_TARGET
#   - For _DRM_BUILD_TOOL = NMAKE
#       - all: target rule (dependent on DRM_BUILD_TARGET)
#       - clean: target rule (dependent on DRM_CLEAN_TARGET)
#
#********************************************************************/



##############################################################################
#                                                                            #
#  Pick up build definitions from the shell environment                      #
#  ----------------------------------------------------			     #
#                                                                            #
##############################################################################

#------------------------------------
# Determine the build tool being used
#------------------------------------

_DRM_BUILD_TOOL=NMAKE

_NATIVE_COMPILE=NO

#Will come from MakeCOnfig in the future

ifeq ($(_NATIVE_COMPILE), NO)
   include $(_DRM_SOURCE_PATH)../../../../../../include/MakeConfig
   _DRM_LIB_PATH = $(_DRM_SOURCE_PATH)../../../../../../lib/
else
   MYCC=g++
   MYAR=ar
   MYRANLIB=ranlib
   _DRM_LIB_PATH = $(_DRM_SOURCE_PATH)Lib/
endif

#-------------------------------------
# Following environment variables ought to be predefined
#       DRM_BUILD_ARCH
#       DRM_BUILD_TYPE
#       DRM_ROOT_PATH
#       DRM_BUILD_PLATFORM
#-------------------------------------

DRM_BUILD_ARCH=ARM
DRM_BUILD_TYPE=FREE
DRM_ROOT_PATH=./
DRM_BUILD_PLATFORM=ANSI
DRM_BUILD_PROFILE=OEM

#-------------------------------------
# Since nmake is single-threaded (and declares all dependencies)
# build everything in the same pass.
#-------------------------------------
_DRM_BUILD_LIBRARY_TARGETS=1
_DRM_BUILD_BINARY_TARGETS=1
_DRM_BUILD_POST_PROCESS_TARGET=1



##############################################################################
#                                                                            #
#  Declare targets for build NMAKE and NTBUILD                               #
#  -------------------------------------------                               #
#                                                                            #
##############################################################################




##############################################################################
#                                                                            #
#  Validate resulting build parameters                                       #
#  -----------------------------------                                       #
#                                                                            #
##############################################################################



##############################################################################
#                                                                            #
#  Derive variables based on above parameters                                #
#  ------------------------------------------                                #
#                                                                            #
##############################################################################

#-------------------------------------
# Derive DRM_BUILD_PATH from DRM_ROOT_PATH
#-------------------------------------
DRM_BUILD_PATH=$(DRM_ROOT_PATH)build^\


#-------------------------------------
# Derive DRM_BUILD_PLATARCH
#-------------------------------------

DRM_BUILD_PLATARCH=$(DRM_BUILD_PLATFORM)_$(DRM_BUILD_ARCH)

#############################################################################
#
# From DrmOverrideFeatureDefaults.mk;                                       #
#
#############################################################################

#-------------------------------------
# Derive _DRM_BUILD_PROFILE_INT:
# values should match DRM_BUILD_PROFILE macros 
# in drmfeatures.h exactly.
# FIXED for OEM here      
#-------------------------------------

_DRM_BUILD_PROFILE_INT=10

_DRM_C_DEFINES+= -DDRM_BUILD_PROFILE=$(_DRM_BUILD_PROFILE_INT)

#------------------------------------------------------------------------------
# Set path for source   
#------------------------------------------------------------------------------
#_DRM_SOURCE_PATH=$(DRM_ROOT_PATH)source/
#_DRM_TEST_PATH=$(DRM_ROOT_PATH)test/


#############################################################################
#
# From DrmPlatform.mk or i386.mk;                                           #
#
#############################################################################

#------------------------------------------------------------------------------
# Define the right flags for the current type of build
#------------------------------------------------------------------------------
ifeq ($(DRM_BUILD_TYPE),PERF)
    DRM_SUPPORT_PROFILING = 1
endif


#---------------------------------------
# Set defines based on chk/fre behavior
#   DBG         set to one if checked build, undefined for free.
#---------------------------------------
ifeq ($(DRM_BUILD_TYPE),CHECKED)
    _DRM_C_DEFINES += -DDBG=1 -ggdb
else
    _DRM_C_DEFINES += -O1
endif

#---------------------------------------
# Set defines based on DRM_BUILD_ARCH behavior
#   
#---------------------------------------
ifeq ($(DRM_BUILD_ARCH),ARM)
    _DRM_C_DEFINES += -DARM
endif

#-------------------------------------------------------------
# If we're building the test framework we need to set PK_TEST
# Lots of test cases will fail if this isn't set
#-------------------------------------------------------------
ifeq ($(PK_TEST),1)
    _DRM_C_DEFINES += PK_TEST=1
endif

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

_DRM_C_DEFINES += -D_CRT_SECURE_NO_DEPRECATE -D_CRT_OBSOLETE_NO_DEPRECATE -D_CRT_SECURE_FORCE_DEPRECATE

# --------------------------------------------------------------------------
# On Win32 free/perf builds, use non-DRM CRT option for best option coverage
# --------------------------------------------------------------------------
#_DRM_C_DEFINES += -DNO_DRM_CRT

#---------------------------------------
# Add architecture-specific defines (used by DRM and SDKs)
#   _X86_, _M_IX86       defined only for x86 compilation
#---------------------------------------
_DRM_C_DEFINES += 



#-------------------------------------
# Set NATIVE_COMPILE, which will be used in Source/inc/drmcompiler.h for DRMFORCEINLINE
#-------------------------------------
ifeq ($(_NATIVE_COMPILE), YES)
    _DRM_C_DEFINES += -D_NATIVE_COMPILE
endif


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
_C_COMPILER_SWITCHES := -fpack-struct=8


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
#ifeq ($(DRM_BUILD_TYPE),CHECKED)
#    _C_COMPILER_SWITCHES += -Odib1y-
#else
#    _C_COMPILER_SWITCHES += -Oxt
#endif

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


#------------------------------------------------------------------------------
# Add the -I prefix to each include path and wrap each path in quotes. Do this
# by using separating ;'s as locations for insertion and wrapping. Steps:
# 1. Strip existing quotes
# 2. Remove any spaces after semicolons
# 3. Remove spaces at the beginning of the line
# 4. Use semicolons as locations to insert -I prefixes.
#------------------------------------------------------------------------------

_C_INCLUDE_SWITCHES  =$(INCLUDES)


#############################################################################
#
# From drmbuild.cmn;                                                        #
#
#############################################################################


INCLUDES += \
    -I. \
    -I.. \
    -I$(_DRM_SOURCE_PATH)inc                   \
    -I$(_DRM_SOURCE_PATH)crypto/bignum         \
    -I$(_DRM_SOURCE_PATH)crypto/drmsha256      \
    -I$(_DRM_SOURCE_PATH)crypto/pkcrypto       \
    -I$(_DRM_SOURCE_PATH)certs                 \
    -I$(_DRM_SOURCE_PATH)certs/nd              \
    -I$(_DRM_SOURCE_PATH)xmr                   \
    -I$(_DRM_SOURCE_PATH)ndtinc                \

INCLUDES += \
           -I$(_DRM_SOURCE_PATH)oem/common/inc \
           -I$(_DRM_SOURCE_PATH)oem/ansi/inc   \
           -I$(_DRM_SOURCE_PATH)oem/ansi/trace
