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
#*    Build the list of dirs required to recursively traverse
#*    the directory structure to build the binary
#*
#*  Outputs:
#*
#*  _DRM_BUILD_SUBDIR_TARGETS: Target for building all the subdirectories. This
#*      is the list of all subdirectories which need to be built with
#*      .[TARGET_BUILD_DIR] string appended to them. This list may contain
#*      .[TARGET_BUILD_IGNORE_DIR] entries as well.
#*
#*  _DRM_CLEAN_SUBDIR_TARGETS: Target for cleaning all the subdirectories. This
#*      is the list of all subdirectories which need to be built with
#*      .[TARGET_CLEAN_DIR] string appended to them. This list may contain
#*      .[TARGET_CLEAN_IGNORE_DIR] entries as well.
#*
#*
#*********************************************************************/


###############################################################################

#------------------------------------------------------------------------------
# Include DIRS file if present
#------------------------------------------------------------------------------

!if exist(.\dirs)

#==============================================================================
# Select DRM_DIRS if it is there, otherwise just DIRS.
#==============================================================================

TARGETTYPE=NOTARGET
!include .\dirs

!if defined(DRM_DIRS)
_DRM_DIRS_TARGETS_T1   =$(DRM_DIRS)
!else
_DRM_DIRS_TARGETS_T1   =$(DIRS)
!endif

#------------------------------------------------------------------------------
# Note: The general algorithm for processing tags (X86, ANSI, etc) in lines
# like "DIRS=FOO{X86} BAR{ANSI} ..." is as follows:
#
# Example: Building ANSI ARM
#
#   DIRS=Foo Bar{x86, ansi,arm} Baz{winpc_amd64 } -- Original
#   DIRS=Foo Bar{x86,ansi,arm} Baz{winpc_amd64}   -- Canonicalize spacing
#
#     ---> Call DrmPlatformComposeDirsTargets.mk
#
#   DIRS=Foo Bar{x86,Y,arm} Baz{N_amd64}          -- Replace plats with Y & N
#   DIRS=Foo Bar{N,Y,Y} Baz{N}                    -- Replace archs with Y & N
#
#     <--- which returns
#
#   DIRS=Foo Bar{N,Y,Y}Baz{N}                     -- Remove spaces after }
#   DIRS=Foo Bar{Y,Y}Baz{}                        -- Remove N's
#   DIRS=Foo Bar{Y}Baz{}                          -- Collapse Y's
#   DIRS=Foo.Y Bar{Y}Baz{}                        -- Add .Y to nondecorated sections
#   DIRS=Foo.Y Bar.Y Baz.N                        -- Convert brackets to Y & N
#
# Then use inferrence rules or rules that check their extension to build
# everything ending in .Y and skip everything ending in .N
#
# Note: The use of intermediary variables (_DRM...1 through 3) is only to aid
#       in debugging.
#------------------------------------------------------------------------------


#==============================================================================
# Perform non-architecture specific pre-processing steps:
# 1. Canonicalize spacing
# 2. Prefix tags (X86, etc) with "TARGET."
#==============================================================================

#------------------------------------------------------------------------------
# Canonicalize the spacing:
# 1. Convert tabs to spaces.
# 2. Ensure only one space exists between items.
# 3. Remove any spaces at the beginning or end of the line.
#------------------------------------------------------------------------------

# Add sentinels. A space at the beginning/end of a line now has a neighboring space.
_DRM_DIRS_TARGETS_T1 =[START_OF_LINE_SENTINEL] $(_DRM_DIRS_TARGETS_T1) [END_OF_LINE_SENTINEL]

# Trim the tabs
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:	= )

# Ensure no space borders another (optimal pattern is a fibonacci sequence).
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:                                  = )
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:                     = )
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:             = )
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:        = )
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:     = )
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:   = )
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:  = )

# Remove the sentinels, taking with them the only remaining spaces at the
# start/end of the line.
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:[START_OF_LINE_SENTINEL] =)
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1: [END_OF_LINE_SENTINEL]=)


#------------------------------------------------------------------------------
# More canonicalization:
# 1. Ensure no spaces after commas.
# 2. Ensure no spaces on the inside of brackets.
#------------------------------------------------------------------------------
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:, =,)
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1: }=})
_DRM_DIRS_TARGETS_T1 =$(_DRM_DIRS_TARGETS_T1:{ ={)

#------------------------------------------------------------------------------
# Prefix all platform, architecture, and platarch tags with "TARGET."
# This will ensure macro transformation steps targetted at the tags won't
# accidentially 'rename' a directory that just happened to have ANSI, x86, etc
# in its name.
#------------------------------------------------------------------------------
_DRM_DIRS_TARGETS_T1=$(_DRM_DIRS_TARGETS_T1:{={TARGET.)
_DRM_DIRS_TARGETS_T1=$(_DRM_DIRS_TARGETS_T1:,=,TARGET.)
_DRM_DIRS_TARGETS=$(_DRM_DIRS_TARGETS_T1)


#==============================================================================
# Call the platform specific routine. It will transform all {TARGET.FOO} tags
# into either {TARGET.SKIP} or {TARGET.BUILD}.
#==============================================================================
!include DrmPlatformComposeDirsTargets.mk


#==============================================================================
# Perform postprocesing steps on the tags:
# 1. Treat undecorated targets as if they have {TARGET.BUILD} attached.
# 2. Remove all TARGET.SKIP tags
# 3. Collapse together all TARGET.BUILD tags
# 4. If left with {}, don't build it. If left with {TARGET.BUILD}, build it.
#==============================================================================

# Remove spaces after closing bracket tags. FOO{X86} BAR becomes FOO{X86}BAZ
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS)
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:} =})

# Add a space to the end *only* if the final target isn't decorated with a tag.
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2) END_OF_LINE_SENTINEL
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:} END_OF_LINE_SENTINEL=})
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:END_OF_LINE_SENTINEL=)

# All undecorated directories now have trailing spaces, and decorated
# directories do not. This lets us...

# Add {TARGET.BUILD} to undecorated entries (and trim the space at same time).
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2: ={TARGET.BUILD})

# Remove all skips.
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:TARGET.SKIP,=)
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:,TARGET.SKIP=)

# Collapse together multiple build tags. In reality, at most two should exist:
# one from the current platform, one from the current arch.
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:TARGET.BUILD,TARGET.BUILD=TARGET.BUILD)
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:TARGET.BUILD,TARGET.BUILD=TARGET.BUILD)
_DRM_DIRS_TARGETS_T2 =$(_DRM_DIRS_TARGETS_T2:TARGET.BUILD,TARGET.BUILD=TARGET.BUILD)

#------------------------------------------------------------------------------
# Add final build decorations (and add back spaces)
#------------------------------------------------------------------------------
_DRM_DIRS_TARGETS_T3 =$(_DRM_DIRS_TARGETS_T2)
_DRM_DIRS_TARGETS_T3 =$(_DRM_DIRS_TARGETS_T3:{TARGET.BUILD}=.[TARGET_BUILD_DIR] )
_DRM_DIRS_TARGETS_T3 =$(_DRM_DIRS_TARGETS_T3:{TARGET.SKIP}=.[TARGET_BUILD_IGNORE_DIR] )




#------------------------------------------------------------------------------
# We need to build a separate target for clean since we want to explicitly
# specify target "clean" when we are recursively calling nmake. nmake does not
# keep track of target in a clean environment variable. For non-leaf directory
# (i.e.  files with dirs in directory) we add .[TARGET_CLEAN_DIR] at the end of
# every target. When are are trying to build clean target for non-leaf
# directory we get the name of the directory back by using the macro $*
#
# While building list of targets. We do
#
# DirName->DirName.[TARGET_CLEAN_DIR]
#
# and when we are building the target
#
# $(_DRM_CLEAN_SUBDIR_TARGETS)::
# we do "@cd $*" to change directory to DirName directory.
#
#
# Create unique target names for build as well.
# We need to do this for build dirs as well to prevent anything going wrong
# if there is a directory named "clean" or "all"
#------------------------------------------------------------------------------

_DRM_BUILD_SUBDIR_TARGETS=$(_DRM_DIRS_TARGETS_T3)
_DRM_CLEAN_SUBDIR_TARGETS=$(_DRM_DIRS_TARGETS_T3:.[TARGET_BUILD=.[TARGET_CLEAN)


!endif # of !if exist(.\dirs)
###############################################################################

