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
#*    Locates the appropriate project-specific settings to use.
#*
#**********************************************************************

!if exists(.\project.mk)
_DRM_PROJECT_MK_PATH=.
!elseif exists(..\project.mk)
_DRM_PROJECT_MK_PATH=..
!elseif exists(..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..
!elseif exists(..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..
!elseif exists(..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..
!elseif exists(..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..
!elseif exists(..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..\..\..\..\..\..
!elseif exists(..\..\..\..\..\..\..\..\..\..\..\..\..\..\..\project.mk)
_DRM_PROJECT_MK_PATH=..\..\..\..\..\..\..\..\..\..\..\..\..\..\..
!endif

!if "$(_DRM_PROJECT_MK_PATH)" != ""
#!message "Including $(_DRM_PROJECT_MK_PATH)\project.mk"
!include $(_DRM_PROJECT_MK_PATH)\project.mk
!else
#!message "Unable to find project.mk. Update DrmSetProject.mk or create project.mk."
!endif

