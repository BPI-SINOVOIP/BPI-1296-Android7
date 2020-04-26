#**@@@+++@@@@******************************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@******************************************************************

include ./$(_DRM_ROOT_PATH)DrmProfileOEM.mk
#------------------------------------------------------------------------------
# Compile-time option section
#------------------------------------------------------------------------------

###########################################################################
# define DRM_BUILD_PROFILE
###########################################################################

_DRM_BUILD_PROFILE_INT=$(_DRM_BUILD_PROFILE_INT_VIA_PROFILE)
_DRM_C_DEFINES += -DDRM_BUILD_PROFILE=$(_DRM_BUILD_PROFILE_INT)

###########################################################################
# define DRM_SUPPORT_LOCKING
###########################################################################
DRM_SUPPORT_LOCKING=$(DRM_SUPPORT_LOCKING_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_LOCKING), 1)
  ifneq ($(DRM_SUPPORT_LOCKING), 0)
	$(error DRM_SUPPORT_LOCKING must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_LOCKING=$(DRM_SUPPORT_LOCKING)

###########################################################################
# define DRM_SUPPORT_MULTI_THREADING
###########################################################################
DRM_SUPPORT_MULTI_THREADING=$(DRM_SUPPORT_MULTI_THREADING_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_MULTI_THREADING), 1)
  ifneq ($(DRM_SUPPORT_MULTI_THREADING), 0)
	$(error DRM_SUPPORT_MULTI_THREADING must be set to 1 or 0)
  endif
endif

ifeq ($(DRM_SUPPORT_LOCKING), 0)
  ifeq ($(DRM_SUPPORT_MULTI_THREADING), 1)
	$(error We cannot be thread safe unless file locking is supported)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_MULTI_THREADING=$(DRM_SUPPORT_MULTI_THREADING)

###########################################################################
# define DRM_SUPPORT_DATASTORE_PREALLOC
###########################################################################
DRM_SUPPORT_DATASTORE_PREALLOC=$(DRM_SUPPORT_DATASTORE_PREALLOC_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_DATASTORE_PREALLOC), 1)
  ifneq ($(DRM_SUPPORT_DATASTORE_PREALLOC), 0)
	$(error DRM_SUPPORT_DATASTORE_PREALLOC must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_DATASTORE_PREALLOC=$(DRM_SUPPORT_DATASTORE_PREALLOC)

###########################################################################
# define DRM_SUPPORT_NATIVE_64BIT_TYPES
###########################################################################
DRM_SUPPORT_NATIVE_64BIT_TYPES=$(DRM_SUPPORT_NATIVE_64BIT_TYPES_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_NATIVE_64BIT_TYPES), 1)
  ifneq ($(DRM_SUPPORT_NATIVE_64BIT_TYPES), 0)
	$(error DRM_SUPPORT_NATIVE_64BIT_TYPES must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_NATIVE_64BIT_TYPES=$(DRM_SUPPORT_NATIVE_64BIT_TYPES)

###########################################################################
# define DRM_SUPPORT_FORCE_ALIGN
###########################################################################
DRM_SUPPORT_FORCE_ALIGN=$(DRM_SUPPORT_FORCE_ALIGN_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_FORCE_ALIGN), 1)
  ifneq ($(DRM_SUPPORT_FORCE_ALIGN), 0)
	$(error DRM_SUPPORT_FORCE_ALIGN must be set to 1 or 0)
  endif
endif

_DRM_C_DEFINES += -DDRM_SUPPORT_FORCE_ALIGN=$(DRM_SUPPORT_FORCE_ALIGN)

###########################################################################
# define DRM_SUPPORT_ASSEMBLY
###########################################################################
DRM_SUPPORT_ASSEMBLY=$(DRM_SUPPORT_ASSEMBLY_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_ASSEMBLY), 1)
  ifneq ($(DRM_SUPPORT_ASSEMBLY), 0)
	$(error DRM_SUPPORT_ASSEMBLY must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_ASSEMBLY=$(DRM_SUPPORT_ASSEMBLY)

###########################################################################
# define DRM_SUPPORT_ASSEMBLY
###########################################################################
DRM_SUPPORT_PRECOMPUTE_GTABLE=$(DRM_SUPPORT_PRECOMPUTE_GTABLE_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_PRECOMPUTE_GTABLE), 1)
  ifneq ($(DRM_SUPPORT_PRECOMPUTE_GTABLE), 0)
	$(error DRM_SUPPORT_PRECOMPUTE_GTABLE must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_PRECOMPUTE_GTABLE=$(DRM_SUPPORT_PRECOMPUTE_GTABLE)

###########################################################################
# define DRM_SUPPORT_TRACING
###########################################################################
DRM_SUPPORT_TRACING=$(DRM_SUPPORT_TRACING_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_TRACING), 1)
  ifneq ($(DRM_SUPPORT_TRACING), 0)
	$(error DRM_SUPPORT_TRACING must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_TRACING=$(DRM_SUPPORT_TRACING)

###########################################################################
# define DRM_SUPPORT_PRIVATE_KEY_CACHE
###########################################################################
DRM_SUPPORT_PRIVATE_KEY_CACHE=$(DRM_SUPPORT_PRIVATE_KEY_CACHE_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_PRIVATE_KEY_CACHE), 1)
  ifneq ($(DRM_SUPPORT_PRIVATE_KEY_CACHE), 0)
	$(error DRM_SUPPORT_PRIVATE_KEY_CACHE must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_PRIVATE_KEY_CACHE=$(DRM_SUPPORT_PRIVATE_KEY_CACHE)

###########################################################################
# define DRM_SUPPORT_EMBEDDED_CERTS
###########################################################################
DRM_SUPPORT_EMBEDDED_CERTS=$(DRM_SUPPORT_EMBEDDED_CERTS_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_EMBEDDED_CERTS), 1)
  ifneq ($(DRM_SUPPORT_EMBEDDED_CERTS), 0)
	$(error DRM_SUPPORT_EMBEDDED_CERTS must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_EMBEDDED_CERTS=$(DRM_SUPPORT_EMBEDDED_CERTS)

###########################################################################
# define DRM_SUPPORT_DEVICE_SIGNING_KEY
###########################################################################
DRM_SUPPORT_DEVICE_SIGNING_KEY=$(DRM_SUPPORT_DEVICE_SIGNING_KEY_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_DEVICE_SIGNING_KEY), 1)
  ifneq ($(DRM_SUPPORT_DEVICE_SIGNING_KEY), 0)
	$(error DRM_SUPPORT_DEVICE_SIGNING_KEY must be set to 1 or 0)
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_DEVICE_SIGNING_KEY=$(DRM_SUPPORT_DEVICE_SIGNING_KEY)

###########################################################################
# define DRM_SUPPORT_NONVAULTSIGNING
###########################################################################
DRM_SUPPORT_NONVAULTSIGNING=$(DRM_SUPPORT_NONVAULTSIGNING_VIA_PROFILE)

ifneq ($(DRM_SUPPORT_DEVICE_SIGNING_KEY), 1)
  ifneq ($(DRM_SUPPORT_DEVICE_SIGNING_KEY), 0)
	$(error DRM_SUPPORT_DEVICE_SIGNING_KEY must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -DDRM_SUPPORT_NONVAULTSIGNING=$(DRM_SUPPORT_NONVAULTSIGNING)

###########################################################################
# define DRM_USE_IOCTL_HAL_GET_DEVICE_INFO
###########################################################################
DRM_USE_IOCTL_HAL_GET_DEVICE_INFO=$(DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE)

ifneq ($(DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE), 1)
  ifneq ($(DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE), 0)
	$(error DRM_USE_IOCTL_HAL_GET_DEVICE_INFO_VIA_PROFILE must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -DDRM_USE_IOCTL_HAL_GET_DEVICE_INFO=$(DRM_USE_IOCTL_HAL_GET_DEVICE_INFO)

###########################################################################
# define _DATASTORE_WRITE_THRU
###########################################################################
_DATASTORE_WRITE_THRU=$(_DATASTORE_WRITE_THRU_VIA_PROFILE)

ifneq ($(_DATASTORE_WRITE_THRU), 1)
  ifneq ($(_DATASTORE_WRITE_THRU), 0)
	$(error _DATASTORE_WRITE_THRU must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -D_DATASTORE_WRITE_THRU=$(_DATASTORE_WRITE_THRU)

###########################################################################
# define _ADDLICENSE_WRITE_THRU
###########################################################################
_ADDLICENSE_WRITE_THRU=$(_ADDLICENSE_WRITE_THRU_VIA_PROFILE)

ifneq ($(_ADDLICENSE_WRITE_THRU), 1)
  ifneq ($(_ADDLICENSE_WRITE_THRU), 0)
	$(error _ADDLICENSE_WRITE_THRU must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -D_ADDLICENSE_WRITE_THRU=$(_ADDLICENSE_WRITE_THRU)

###########################################################################
# define DRM_HDS_COPY_BUFFER_SIZE
###########################################################################
DRM_HDS_COPY_BUFFER_SIZE=$(DRM_HDS_COPY_BUFFER_SIZE_VIA_PROFILE)

#!IF "$(DRM_HDS_COPY_BUFFER_SIZE)" == ""
#!ERROR DRM_HDS_COPY_BUFFER_SIZE must be set to a valid value
#!ENDIF
_DRM_C_DEFINES += -DDRM_HDS_COPY_BUFFER_SIZE=$(DRM_HDS_COPY_BUFFER_SIZE)

###########################################################################
# define DRM_TEST_SUPPORT_NET_IO
###########################################################################
DRM_TEST_SUPPORT_NET_IO=$(DRM_TEST_SUPPORT_NET_IO_VIA_PROFILE)

ifneq ($(DRM_TEST_SUPPORT_NET_IO), 1)
  ifneq ($(DRM_TEST_SUPPORT_NET_IO), 0)
	$(error DRM_TEST_SUPPORT_NET_IO must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -DDRM_TEST_SUPPORT_NET_IO=$(DRM_TEST_SUPPORT_NET_IO)

###########################################################################
# define USE_PK_NAMESPACES
###########################################################################
USE_PK_NAMESPACES=$(USE_PK_NAMESPACES_VIA_PROFILE)


ifneq ($(USE_PK_NAMESPACES), 1)
  ifneq ($(USE_PK_NAMESPACES), 0)
	$(error USE_PK_NAMESPACES must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -DUSE_PK_NAMESPACES=$(USE_PK_NAMESPACES)


###########################################################################
# define DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT
###########################################################################
DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT=$(DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT_VIA_PROFILE)

ifneq ($(DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT), 1)
  ifneq ($(DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT), 0)
	$(error DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT must be set to 1 or 0)  
  endif
endif
_DRM_C_DEFINES += -DDRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT=$(DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT)

###########################################################################
# define DRM_KEYFILE_VERSION
###########################################################################
DRM_KEYFILE_VERSION=$(DRM_KEYFILE_VERSION_VIA_PROFILE)

#!IF "$(DRM_KEYFILE_VERSION)" == ""
#!ERROR DRM_KEYFILE_VERSION must be set to a valid value
#!ENDIF
_DRM_C_DEFINES += -DDRM_KEYFILE_VERSION=$(DRM_KEYFILE_VERSION)
