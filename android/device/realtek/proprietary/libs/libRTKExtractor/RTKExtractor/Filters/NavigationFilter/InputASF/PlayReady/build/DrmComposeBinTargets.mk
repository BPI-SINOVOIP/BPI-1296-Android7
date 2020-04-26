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
#*    Build the list of binary targets( dll/lib/exe ) to be built.
#*  
#*  _DRM_BIN_EXT is defined in platform specific file. Rules for
#*  nmake allow us to use "forward declared" variables
#*  
#* Outputs:   
#*  _DRM_BIN_TARGETS: The name of the binary appended with the right extension
#*      depending on the type of binary being built (library or executable) 
#*    
#*********************************************************************/

#------------------------------------------------------------------------------
# Define the necessary extension target 
#------------------------------------------------------------------------------


#----------------------------
!if "$(TARGETTYPE)" == "PROGRAM"
_DRM_BIN_TARGETS=$(TARGETNAME).$(_DRM_BIN_EXT)
#----------------------------
!elseif "$(TARGETTYPE)" == "LIBRARY"
_DRM_BIN_TARGETS=$(TARGETNAME).lib
#----------------------------
!elseif "$(TARGETTYPE)" == "DYNLINK"
_DRM_BIN_TARGETS=$(TARGETNAME).dll
#----------------------------
!elseif "$(TARGETTYPE)" == "NOTARGET"
#----------------------------
!else
!   error invalid TARGETTYPE = $(TARGETTYPE)
#----------------------------
!endif

