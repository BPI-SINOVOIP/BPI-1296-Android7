#**@@@+++@@@@******************************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@******************************************************************


include( "${RTK_PLAYREADY}/../DrmProfileOEM.cmake"  )
#------------------------------------------------------------------------------
# Compile-time option section
#------------------------------------------------------------------------------

###########################################################################
# define DRM_BUILD_PROFILE
###########################################################################
set( _DRM_BUILD_PROFILE_INT ${_DRM_BUILD_PROFILE_INT_VIA_PROFILE} )
add_definitions( -DDRM_BUILD_PROFILE=${_DRM_BUILD_PROFILE_INT} )


###########################################################################
# define DRM_SUPPORT_LOCKING
###########################################################################

set( DRM_SUPPORT_LOCKING ${DRM_SUPPORT_LOCKING_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_LOCKING STREQUAL 1)
	IF(NOT DRM_SUPPORT_LOCKING STREQUAL 0)
		message(DRM_SUPPORT_LOCKING=${DRM_SUPPORT_LOCKING})
		message( FATAL_ERROR "DRM_SUPPORT_LOCKING must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_LOCKING STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_LOCKING STREQUAL 1)


add_definitions( -DDRM_SUPPORT_LOCKING=${DRM_SUPPORT_LOCKING} )

###########################################################################
# define DRM_SUPPORT_MULTI_THREADING
###########################################################################
set( DRM_SUPPORT_MULTI_THREADING ${DRM_SUPPORT_MULTI_THREADING_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_MULTI_THREADING STREQUAL 1)
	IF(NOT DRM_SUPPORT_MULTI_THREADING STREQUAL 0)
		message(DRM_SUPPORT_MULTI_THREADING=${DRM_SUPPORT_MULTI_THREADING})
		message( FATAL_ERROR "DRM_SUPPORT_MULTI_THREADING must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_MULTI_THREADING STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_MULTI_THREADING STREQUAL 1)

IF(DRM_SUPPORT_MULTI_THREADING STREQUAL 1)
	IF(DRM_SUPPORT_LOCKING STREQUAL 0)
		message( FATAL_ERROR "We cannot be thread safe unless file locking is supported")
	ENDIF(DRM_SUPPORT_LOCKING STREQUAL 0)
ENDIF(DRM_SUPPORT_MULTI_THREADING STREQUAL 1)


add_definitions( -DDRM_SUPPORT_MULTI_THREADING=${DRM_SUPPORT_MULTI_THREADING} )
###########################################################################
# define DRM_SUPPORT_DATASTORE_PREALLOC
###########################################################################

set( DRM_SUPPORT_DATASTORE_PREALLOC ${DRM_SUPPORT_DATASTORE_PREALLOC_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_DATASTORE_PREALLOC STREQUAL 1)
	IF(NOT DRM_SUPPORT_DATASTORE_PREALLOC STREQUAL 0)
		message(DRM_SUPPORT_DATASTORE_PREALLOC=${DRM_SUPPORT_DATASTORE_PREALLOC})
		message( FATAL_ERROR "DRM_SUPPORT_DATASTORE_PREALLOC must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_DATASTORE_PREALLOC STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_DATASTORE_PREALLOC STREQUAL 1)


add_definitions( -DDRM_SUPPORT_DATASTORE_PREALLOC=${DRM_SUPPORT_DATASTORE_PREALLOC} )

###########################################################################
# define DRM_SUPPORT_NATIVE_64BIT_TYPES
###########################################################################
set( DRM_SUPPORT_NATIVE_64BIT_TYPES ${DRM_SUPPORT_NATIVE_64BIT_TYPES_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_NATIVE_64BIT_TYPES STREQUAL 1)
	IF(NOT DRM_SUPPORT_NATIVE_64BIT_TYPES STREQUAL 0)
		message(DRM_SUPPORT_NATIVE_64BIT_TYPES=${DRM_SUPPORT_NATIVE_64BIT_TYPES})
		message( FATAL_ERROR "DRM_SUPPORT_NATIVE_64BIT_TYPES must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_NATIVE_64BIT_TYPES STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_NATIVE_64BIT_TYPES STREQUAL 1)


add_definitions( -DDRM_SUPPORT_NATIVE_64BIT_TYPES=${DRM_SUPPORT_NATIVE_64BIT_TYPES} )

###########################################################################
# define DRM_SUPPORT_FORCE_ALIGN
###########################################################################
set( DRM_SUPPORT_FORCE_ALIGN ${DRM_SUPPORT_FORCE_ALIGN_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_FORCE_ALIGN STREQUAL 1)
	IF(NOT DRM_SUPPORT_FORCE_ALIGN STREQUAL 0)
		message(DRM_SUPPORT_FORCE_ALIGN=${DRM_SUPPORT_FORCE_ALIGN})
		message( FATAL_ERROR "DRM_SUPPORT_FORCE_ALIGN must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_FORCE_ALIGN STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_FORCE_ALIGN STREQUAL 1)

add_definitions( -DDRM_SUPPORT_FORCE_ALIGN=${DRM_SUPPORT_FORCE_ALIGN} )
###########################################################################
# define DRM_SUPPORT_ASSEMBLY
###########################################################################
set( DRM_SUPPORT_ASSEMBLY ${DRM_SUPPORT_ASSEMBLY_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_ASSEMBLY STREQUAL 1)
	IF(NOT DRM_SUPPORT_ASSEMBLY STREQUAL 0)
		message(DRM_SUPPORT_ASSEMBLY=${DRM_SUPPORT_ASSEMBLY})
		message( FATAL_ERROR "DRM_SUPPORT_ASSEMBLY must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_ASSEMBLY STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_ASSEMBLY STREQUAL 1)


add_definitions( -DDRM_SUPPORT_ASSEMBLY=${DRM_SUPPORT_ASSEMBLY} )
###########################################################################
# define DRM_SUPPORT_ASSEMBLY
###########################################################################
set( DRM_SUPPORT_PRECOMPUTE_GTABLE ${DRM_SUPPORT_PRECOMPUTE_GTABLE_VIA_PROFILE} )


IF(NOT DRM_SUPPORT_PRECOMPUTE_GTABLE STREQUAL 1)
	IF(NOT DRM_SUPPORT_PRECOMPUTE_GTABLE STREQUAL 0)
		message(DRM_SUPPORT_PRECOMPUTE_GTABLE=${DRM_SUPPORT_PRECOMPUTE_GTABLE})
		message( FATAL_ERROR "DRM_SUPPORT_PRECOMPUTE_GTABLE must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_PRECOMPUTE_GTABLE STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_PRECOMPUTE_GTABLE STREQUAL 1)


add_definitions( -DDRM_SUPPORT_PRECOMPUTE_GTABLE=${DRM_SUPPORT_PRECOMPUTE_GTABLE} )
###########################################################################
# define DRM_SUPPORT_TRACING
###########################################################################
set( DRM_SUPPORT_TRACING ${DRM_SUPPORT_TRACING_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_TRACING STREQUAL 1)
	IF(NOT DRM_SUPPORT_TRACING STREQUAL 0)
		message(DRM_SUPPORT_TRACING=${DRM_SUPPORT_TRACING})
		message( FATAL_ERROR "DRM_SUPPORT_TRACING must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_TRACING STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_TRACING STREQUAL 1)


add_definitions( -DDRM_SUPPORT_TRACING=${DRM_SUPPORT_TRACING} )
###########################################################################
# define DRM_SUPPORT_PRIVATE_KEY_CACHE
###########################################################################
set( DRM_SUPPORT_PRIVATE_KEY_CACHE ${DRM_SUPPORT_PRIVATE_KEY_CACHE_VIA_PROFILE} )


IF(NOT DRM_SUPPORT_PRIVATE_KEY_CACHE STREQUAL 1)
	IF(NOT DRM_SUPPORT_PRIVATE_KEY_CACHE STREQUAL 0)
		message(DRM_SUPPORT_PRIVATE_KEY_CACHE=${DRM_SUPPORT_PRIVATE_KEY_CACHE})
		message( FATAL_ERROR "DRM_SUPPORT_PRIVATE_KEY_CACHE must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_PRIVATE_KEY_CACHE STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_PRIVATE_KEY_CACHE STREQUAL 1)


add_definitions( -DDRM_SUPPORT_PRIVATE_KEY_CACHE=${DRM_SUPPORT_PRIVATE_KEY_CACHE} )
###########################################################################
# define DRM_SUPPORT_EMBEDDED_CERTS
###########################################################################
set( DRM_SUPPORT_EMBEDDED_CERTS ${DRM_SUPPORT_EMBEDDED_CERTS_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_EMBEDDED_CERTS STREQUAL 1)
	IF(NOT DRM_SUPPORT_EMBEDDED_CERTS STREQUAL 0)
		message(DRM_SUPPORT_EMBEDDED_CERTS=${DRM_SUPPORT_EMBEDDED_CERTS})
		message( FATAL_ERROR "DRM_SUPPORT_EMBEDDED_CERTS must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_EMBEDDED_CERTS STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_EMBEDDED_CERTS STREQUAL 1)


add_definitions( -DDRM_SUPPORT_EMBEDDED_CERTS=${DRM_SUPPORT_EMBEDDED_CERTS} )
###########################################################################
# define DRM_SUPPORT_DEVICE_SIGNING_KEY
###########################################################################
set( DRM_SUPPORT_DEVICE_SIGNING_KEY ${DRM_SUPPORT_DEVICE_SIGNING_KEY_VIA_PROFILE} )

IF(NOT DRM_SUPPORT_DEVICE_SIGNING_KEY STREQUAL 1)
	IF(NOT DRM_SUPPORT_DEVICE_SIGNING_KEY STREQUAL 0)
		message(DRM_SUPPORT_DEVICE_SIGNING_KEY=${DRM_SUPPORT_DEVICE_SIGNING_KEY})
		message( FATAL_ERROR "DRM_SUPPORT_DEVICE_SIGNING_KEY must be set to 1 or 0")
	ENDIF(NOT DRM_SUPPORT_DEVICE_SIGNING_KEY STREQUAL 0)
ENDIF(NOT DRM_SUPPORT_DEVICE_SIGNING_KEY STREQUAL 1)


add_definitions( -DDRM_SUPPORT_DEVICE_SIGNING_KEY=${DRM_SUPPORT_DEVICE_SIGNING_KEY} )
###########################################################################
# define DRM_SUPPORT_NONVAULTSIGNING
###########################################################################
set( DRM_SUPPORT_NONVAULTSIGNING ${DRM_SUPPORT_NONVAULTSIGNING_VIA_PROFILE} )


add_definitions( -DDRM_SUPPORT_NONVAULTSIGNING=${DRM_SUPPORT_NONVAULTSIGNING} )
###########################################################################
# define DRM_USE_IOCTL_HAL_GET_DEVICE_INFO
###########################################################################
set( DRM_USE_IOCTL_HAL_GET_DEVICE_INFO ${DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE} )

IF(NOT DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE STREQUAL 1)
	IF(NOT DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE STREQUAL 0)
		message(DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE=${DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE})
		message( FATAL_ERROR "DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE must be set to 1 or 0")
	ENDIF(NOT DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE STREQUAL 0)
ENDIF(NOT DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE STREQUAL 1)

add_definitions( -DDRM_USE_IOCTL_HAL_GET_DEVICE_INFO=${DRM_USE_IOCTL_HAL_GET_DEVICE_INFO} )
###########################################################################
# define _DATASTORE_WRITE_THRU
###########################################################################
set( _DATASTORE_WRITE_THRU ${_DATASTORE_WRITE_THRU_VIA_PROFILE} )

IF(NOT _DATASTORE_WRITE_THRU STREQUAL 1)
	IF(NOT _DATASTORE_WRITE_THRU STREQUAL 0)
		message(_DATASTORE_WRITE_THRU=${_DATASTORE_WRITE_THRU})
		message( FATAL_ERROR "_DATASTORE_WRITE_THRU must be set to 1 or 0")
	ENDIF(NOT _DATASTORE_WRITE_THRU STREQUAL 0)
ENDIF(NOT _DATASTORE_WRITE_THRU STREQUAL 1)


add_definitions( -D_DATASTORE_WRITE_THRU=${_DATASTORE_WRITE_THRU} )

###########################################################################
# define _ADDLICENSE_WRITE_THRU
###########################################################################
set( _ADDLICENSE_WRITE_THRU ${_ADDLICENSE_WRITE_THRU_VIA_PROFILE} )

IF(NOT _ADDLICENSE_WRITE_THRU STREQUAL 1)
	IF(NOT _ADDLICENSE_WRITE_THRU STREQUAL 0)
		message(_ADDLICENSE_WRITE_THRU=${_ADDLICENSE_WRITE_THRU})
		message( FATAL_ERROR "_ADDLICENSE_WRITE_THRU must be set to 1 or 0")
	ENDIF(NOT _ADDLICENSE_WRITE_THRU STREQUAL 0)
ENDIF(NOT _ADDLICENSE_WRITE_THRU STREQUAL 1)


add_definitions( -D_ADDLICENSE_WRITE_THRU=${_ADDLICENSE_WRITE_THRU} )

###########################################################################
# define DRM_HDS_COPY_BUFFER_SIZE
###########################################################################
set( DRM_HDS_COPY_BUFFER_SIZE ${DRM_HDS_COPY_BUFFER_SIZE_VIA_PROFILE} )


#!IF "$(DRM_HDS_COPY_BUFFER_SIZE)" == ""
#!ERROR DRM_HDS_COPY_BUFFER_SIZE must be set to a valid value
#!ENDIF

add_definitions( -DDRM_HDS_COPY_BUFFER_SIZE=${DRM_HDS_COPY_BUFFER_SIZE} )
###########################################################################
# define DRM_TEST_SUPPORT_NET_IO
###########################################################################
set( DRM_TEST_SUPPORT_NET_IO ${DRM_TEST_SUPPORT_NET_IO_VIA_PROFILE} )


IF(NOT DRM_TEST_SUPPORT_NET_IO STREQUAL 1)
	IF(NOT DRM_TEST_SUPPORT_NET_IO STREQUAL 0)
		message(DRM_TEST_SUPPORT_NET_IO=${DRM_TEST_SUPPORT_NET_IO})
		message( FATAL_ERROR "DRM_TEST_SUPPORT_NET_IO must be set to 1 or 0")
	ENDIF(NOT DRM_TEST_SUPPORT_NET_IO STREQUAL 0)
ENDIF(NOT DRM_TEST_SUPPORT_NET_IO STREQUAL 1)


add_definitions( -DDRM_TEST_SUPPORT_NET_IO=${DRM_TEST_SUPPORT_NET_IO} )
###########################################################################
# define USE_PK_NAMESPACES
###########################################################################
set( USE_PK_NAMESPACES ${USE_PK_NAMESPACES_VIA_PROFILE} )

IF(NOT USE_PK_NAMESPACES STREQUAL 1)
	IF(NOT USE_PK_NAMESPACES STREQUAL 0)
		message(USE_PK_NAMESPACES=${USE_PK_NAMESPACES})
		message( FATAL_ERROR "USE_PK_NAMESPACES must be set to 1 or 0")
	ENDIF(NOT USE_PK_NAMESPACES STREQUAL 0)
ENDIF(NOT USE_PK_NAMESPACES STREQUAL 1)

add_definitions( -DUSE_PK_NAMESPACES=${USE_PK_NAMESPACES} )

###########################################################################
# define DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT
###########################################################################
set( DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT ${DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT_VIA_PROFILE} )

IF(NOT DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT STREQUAL 1)
	IF(NOT DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT STREQUAL 0)
		message(DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT=${DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT})
		message( FATAL_ERROR "DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT must be set to 1 or 0")
	ENDIF(NOT DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT STREQUAL 0)
ENDIF(NOT DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT STREQUAL 1)


add_definitions( -DDRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT=${DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT} )
###########################################################################
# define DRM_KEYFILE_VERSION
###########################################################################
set( DRM_KEYFILE_VERSION ${DRM_KEYFILE_VERSION_VIA_PROFILE} )


#!IF "$(DRM_KEYFILE_VERSION)" == ""
#!ERROR DRM_KEYFILE_VERSION must be set to a valid value
#!ENDIF

add_definitions( -DDRM_KEYFILE_VERSION=${DRM_KEYFILE_VERSION} )
