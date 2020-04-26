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
#   - DRM_BUILD_PLATFORM: ANSI, ...
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
#  ----------------------------------------------------                      #
#                                                                            #
##############################################################################

#------------------------------------
# Determine the build tool being used
#------------------------------------


_DRM_BUILD_TOOL=NMAKE

_NATIVE_COMPILE=NO

#Will come from MakeCOnfig in the future

ifeq ($(_NATIVE_COMPILE), NO)
   include $(_DMP_ROOT_PATH)include/MakeConfig
   _DRM_LIB_PATH = $(_DMP_ROOT_PATH)lib/
else
   MYCC=g++
   MYAR=ar
   MYRANLIB=ranlib
   _DRM_LIB_PATH = Lib/
endif

#-------------------------------------
# Following environment variables ought to be predefined
#       DRM_BUILD_ARCH
#       DRM_BUILD_TYPE
#       DRM_ROOT_PATH
#       DRM_BUILD_PLATFORM
#-------------------------------------
DRM_BUILD_ARCH=ARM
DRM_BUILD_TYP=CHECKED
#DRM_ROOT_PATH=./
DRM_BUILD_PLATFORM=ANSI
DRM_BUILD_PROFILE=OEM
#DRM_OBJ_ROOT_PATH=$(DRM_ROOT_PATH)


#------------------------------------------------------------------------------
# Set path for source   
#------------------------------------------------------------------------------
#move to Makefile 
#_DRM_SOURCE_PATH=$(DRM_ROOT_PATH)source/
#_DRM_TEST_PATH=$(DRM_ROOT_PATH)test/

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
#DRM_BUILD_PATH=$(DRM_ROOT_PATH)build^\


#-------------------------------------
# Derive DRM_BUILD_PLATARCH
#-------------------------------------

DRM_BUILD_PLATARCH=$(DRM_BUILD_PLATFORM)_$(DRM_BUILD_ARCH)

#---------------------------------------
# Set defines based on DRM_BUILD_ARCH behavior
#   
#---------------------------------------
ifeq ($(DRM_BUILD_ARCH),ARM)
    _DRM_C_DEFINES += -DARM
endif

#------------------------------------------------------------------------------
# Add the -I prefix to each include path and wrap each path in quotes. Do this
# by using separating ;'s as locations for insertion and wrapping. Steps:
# 1. Strip existing quotes
# 2. Remove any spaces after semicolons
# 3. Remove spaces at the beginning of the line
# 4. Use semicolons as locations to insert -I prefixes.
#------------------------------------------------------------------------------

_C_INCLUDE_SWITCHES  =$(INCLUDES)

INCLUDES += \
    -I. \
    -I.. \
    -I$(_DRM_SOURCE_PATH)inc                   \
    -I$(_DRM_SOURCE_PATH)results               \
    -I$(_DRM_SOURCE_PATH)certs                 \
    -I$(_DRM_SOURCE_PATH)keyfile               \
    -I$(_DRM_SOURCE_PATH)keyfileformat         \
    -I$(_DRM_SOURCE_PATH)xmr                   \
    -I$(_DRM_SOURCE_PATH)initiators            \
    -I$(_DRM_SOURCE_PATH)ndtinc                \
    -I$(_DRM_SOURCE_PATH)ndr                \
    -I$(_DRM_SOURCE_PATH)oem/common/inc        \
    -I$(_DRM_SOURCE_PATH)oem/common/cbc        \
    -I$(_DRM_SOURCE_PATH)oem/ansi/inc   