
#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#**********************************************************************
#*
#* Description:
#*    Inference rules for C/C++ files that produce object files.
#*    
#*    The following rules have ability to compile C/C++ files
#*    present in either current directory or parent directory
#*    
#*  This file is defining target rules for two compiling C/C++
#*  files for two scnearios.  
#*  1.   foo.c( or foo.cpp) file is present in the current directory 
#*       and _DRM_OBJ_TARGETS contain $(_DRM_OBJ_DIR)\foo.obj
#*
#*  2.   foo.c( or foo.cpp) file is present in the parent directory 
#*       and _DRM_OBJ_TARGETS contain $(_DRM_OBJ_DIR)\foo.obj
#*
#*  These rules are meant to apply to obj files in _DRM_OBJ_TARGETS, 
#*  but are not restricted to that list.
#*********************************************************************/


.SUFFIXES: .c .cpp


#
# This rule converts foo.c to foo.obj
#
.c{$(_DRM_OBJ_DIR)/}.obj::
    $(_DRM_C_COMPILER_TOOL_COMMAND) $<

#
# This rule converts ..\foo.c to foo.obj
#         
{../}.c{$(_DRM_OBJ_DIR)/}.obj::
    $(_DRM_C_COMPILER_TOOL_COMMAND) $<    

#
# This rule converts foo.cpp to foo.obj
#        
.cpp{$(_DRM_OBJ_DIR)/}.obj::
    $(_DRM_CPP_COMPILER_TOOL_COMMAND) $<

#
# This rule converts ..\foo.cpp to foo.obj
#
{../}.cpp{$(_DRM_OBJ_DIR)/}.obj::
    $(_DRM_CPP_COMPILER_TOOL_COMMAND) $<    


