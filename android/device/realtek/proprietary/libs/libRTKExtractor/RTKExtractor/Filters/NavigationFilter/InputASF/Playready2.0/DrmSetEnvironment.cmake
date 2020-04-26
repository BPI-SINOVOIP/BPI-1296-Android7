#-------------------------------------
# Following environment variables ought to be predefined
#       DRM_BUILD_ARCH
#       DRM_BUILD_TYPE
#       DRM_ROOT_PATH
#       DRM_BUILD_PLATFORM
#-------------------------------------
set(DRM_BUILD_ARCH ARM)
set(DRM_BUILD_TYP CHECKED)
set(DRM_BUILD_PLATFORM ANSI)
set(DRM_BUILD_PROFILE OEM)


IF(DRM_BUILD_ARCH STREQUAL ARM)
add_definitions( -DARM )
ENDIF(DRM_BUILD_ARCH STREQUAL ARM)

include_directories( ${RTK_PLAYREADY}/inc )
include_directories( ${RTK_PLAYREADY}/results )
include_directories( ${RTK_PLAYREADY}/certs )
include_directories( ${RTK_PLAYREADY}/keyfile )
include_directories( ${RTK_PLAYREADY}/keyfileformat )
include_directories( ${RTK_PLAYREADY}/xmr )
include_directories( ${RTK_PLAYREADY}/initiators )
include_directories( ${RTK_PLAYREADY}/ndtinc )
include_directories( ${RTK_PLAYREADY}/oem/common/inc )
include_directories( ${RTK_PLAYREADY}/oem/common/cbc )
include_directories( ${RTK_PLAYREADY}/oem/ansi/inc )
include_directories( ${RTK_PLAYREADY}/ndr )