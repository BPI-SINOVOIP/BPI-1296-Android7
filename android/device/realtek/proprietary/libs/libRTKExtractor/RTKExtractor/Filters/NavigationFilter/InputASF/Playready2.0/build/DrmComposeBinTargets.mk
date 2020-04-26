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
#* Outputs:   
#*  _DRM_BIN_TARGET: The name of the binary appended with the right extension
#*      depending on the type of binary being built (library or executable) 
#*  _DRM_SYM_TARGET: Name of any symbol file produced along-side the bin
#*  _DRM_IMPORT_LIB_TARGET: Name of any import lib produced along-side the DLL
#*    
#*********************************************************************/

#------------------------------------------------------------------------------
# Define the necessary extension target 
#------------------------------------------------------------------------------


#----------------------------
!if "$(TARGETTYPE)" == "PROGRAM"
_DRM_BIN_TARGET=$(_DRM_BIN_PATH)$(TARGETNAME).exe
_DRM_SYM_TARGET=$(_DRM_BIN_PATH)$(TARGETNAME).pdb
#----------------------------
!elseif "$(TARGETTYPE)" == "LIBRARY"
_DRM_BIN_TARGET=$(_DRM_BIN_PATH)$(TARGETNAME).lib
#----------------------------
!elseif "$(TARGETTYPE)" == "DYNLINK"
_DRM_BIN_TARGET=$(_DRM_BIN_PATH)$(TARGETNAME).dll
_DRM_SYM_TARGET=$(_DRM_BIN_PATH)$(TARGETNAME).pdb
_DRM_IMPORT_LIB_TARGET=$(_DRM_BIN_PATH)$(TARGETNAME).lib
#----------------------------
!elseif "$(TARGETTYPE)" == "NOTARGET"
#----------------------------
!else
!   error invalid TARGETTYPE = $(TARGETTYPE)
#----------------------------
!endif

!if "$(DRM_EXPORT_APIS_TO_DLL)" != "1"
_DRM_IMPORT_LIB_TARGET=
!endif

