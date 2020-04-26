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
#*    Build the list of obj targets required to build the binary
#*
#*  Outputs:
#*
#*  _DRM_OBJ_TARGETS: The list of obj targets. This is a list of files 
#*      with their extensions replaced with obj and PLACEHOLDER_OBJ_PATH
#*      prepended to them. PLACEHOLDER_OBJ_PATH is later replaced with
#*      the location where the object files are placed
#*
#*  _DRM_OBJ_PATH_PREFIX: Prefix for the path where objs will
#*      be placed
#*  
#*  _DRM_BIN_PATH_PREFIX: Prefix for the path where binaries will be
#*      placed
#*
#*********************************************************************/


###############################################################################

!if exists(.\sources)

#------------------------------------------------------------------------------
# include user supplied project sources file
#------------------------------------------------------------------------------
!include .\sources


#------------------------------------------------------------------------------
# Required Sources Varaiables
#------------------------------------------------------------------------------
!if !defined(TARGETNAME) || !defined(TARGETTYPE) || !defined(SOURCES)
!    error .\sources file must define TARGETNAME, TARGETTYPE and SOURCES
!endif


#------------------------------------------------------------------------------
# Create a list of objects from the list of source file
# The steps involved are as follows.
# 1. Ensure that all the files in the list have just one space in the between
# 2. Substitute cpp/c/CPP/C with obj
# 3. Ensure that there is no trailing space in the end or front
# 4. Replace each space with path where object file goes
#     For e.g. " a.obj" becomes "objd\i386\a.obj"
# 5. Enjoy your OBJECTS
# 
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Canonicalize the spacing:( only one space between list members )
# 1. Convert tabs to spaces.
# 2. Ensure only one space exists between items.
# 3. Remove any spaces at the beginning or end of the line.
#------------------------------------------------------------------------------

SRC_LIST    =$(SOURCES)


# Add sentinels. A space at the beginning/end of a line now has a neighboring space.
_SRC_LIST    =[START_OF_LINE_SENTINEL $(_SRC_LIST) [END_OF_LINE_SENTINEL]

# Trim the tabs
_SRC_LIST    =$(_SRC_LIST:	= )

# Ensure no space borders another (optimal pattern is a fibonacci sequence).
_SRC_LIST    =$(_SRC_LIST:                                  = )
_SRC_LIST    =$(_SRC_LIST:                     = )
_SRC_LIST    =$(_SRC_LIST:             = )
_SRC_LIST    =$(_SRC_LIST:        = )
_SRC_LIST    =$(_SRC_LIST:     = )
_SRC_LIST    =$(_SRC_LIST:   = )
_SRC_LIST    =$(_SRC_LIST:  = )

# Remove the sentinels, taking with them the only remaining spaces at the 
# start/end of the line.
_SRC_LIST    =$(_SRC_LIST:[START_OF_LINE_SENTINEL] =)
_SRC_LIST    =$(_SRC_LIST: [END_OF_LINE_SENTINEL]=)



#------------------------------------------------------------------------------
# Start with source and change extensions to obj.
#------------------------------------------------------------------------------
_OBJ_LIST      =$(SRC_LIST)
_OBJ_LIST      =$(_OBJ_LIST:.cpp=.obj)
_OBJ_LIST      =$(_OBJ_LIST:.CPP=.obj)
_OBJ_LIST      =$(_OBJ_LIST:.c=.obj)
_OBJ_LIST      =$(_OBJ_LIST:.C=.obj)

# Ensure no space borders another (optimal pattern is a fibonacci sequence).
_OBJ_LIST    =$(_OBJ_LIST:                                  = )
_OBJ_LIST    =$(_OBJ_LIST:                     = )
_OBJ_LIST    =$(_OBJ_LIST:             = )
_OBJ_LIST    =$(_OBJ_LIST:        = )
_OBJ_LIST    =$(_OBJ_LIST:     = )
_OBJ_LIST    =$(_OBJ_LIST:   = )
_OBJ_LIST    =$(_OBJ_LIST:  = )

#------------------------------------------------------------------------------
# Remove any occurence of ..\
#------------------------------------------------------------------------------
_OBJ_LIST      =$(_OBJ_LIST:..\=)
    
  
#------------------------------------------------------------------------------
# Add PLACEHOLDER_OBJ_PATH. We will replace this with actual path where obj 
# files need to live
#------------------------------------------------------------------------------


# Add space to front. This will allows us to replace
# all the spaces with PLACEHOLDER_OBJ_PATH.
_OBJ_LIST      =[START_OF_LINE_SENTINEL] $(_OBJ_LIST)
_OBJ_LIST      =$(_OBJ_LIST:[START_OF_LINE_SENTINEL]=)


_DRM_OBJ_TARGETS    =$(_OBJ_LIST: = PLACEHOLDER_OBJ_PATH\)


!endif #if exists(.\sources)
###############################################################################
