##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system -I../../../Include/Filters
INCLUDES+= -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system
INCLUDES+= -I../../../Platform_Lib/openssl/include

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif
ifeq ($(EXCLUDE_OPENSSL), YES)
DEFINES += -DEXCLUDE_OPENSSL
endif

ifeq ($(USE_ACETRAX_PRODUCTION_SERVER), YES)
DEFINES += -DUSE_ACETRAX_PRODUCTION
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputASF.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputASF.a
endif

###########################################
# check each drm library if need to build #
###########################################
ifeq ($(SUPPORT_WMDRM_FLAG), YES)
	SUBDIRS += WMDRMPD_1007
endif
ifeq ($(SUPPORT_PLAYREADY_FLAG), YES)
	SUBDIRS += PlayReady
endif

ifeq ($(SUPPORT_PLAYREADY20_FLAG), YES)
	SUBDIRS += Playready2.0
endif

########################################
# decide which flag need to be defined #
########################################
ifeq ($(SUPPORT_WMDRM_FLAG), YES)
	include ./WMDRMPD_1007/makefile.inc.2
	INCLUDES+= -I./WMDRMPD_1007/Source/inc
	
	DEFINES += -DSUPPORT_WMDRM_PD -DASF_SUPPORT_DRM
else
	ifeq ($(SUPPORT_PLAYREADY_FLAG), YES)
		DRM_BUILD_ARCH=ARM
		DRM_BUILD_TYPE=CHECKED
		DRM_BUILD_PLATFORM=ANSI
		DRM_BUILD_PROFILE=OEM
		DRM_BUILD_PLATARCH=$(DRM_BUILD_PLATFORM)_$(DRM_BUILD_ARCH)
		_DRM_BUILD_PROFILE_INT=10
		DEFINES += -DDRM_BUILD_PROFILE=$(_DRM_BUILD_PROFILE_INT)
		ifeq ($(DRM_BUILD_ARCH),ARM)
		    DEFINES += -DARM
		endif
		_DRM_SOURCE_PATH = ../InputASF/PlayReady/Source/
		
		INCLUDES += \
		    -I$(_DRM_SOURCE_PATH)inc                   \
		    -I$(_DRM_SOURCE_PATH)crypto/bignum         \
		    -I$(_DRM_SOURCE_PATH)crypto/drmsha256      \
		    -I$(_DRM_SOURCE_PATH)crypto/pkcrypto       \
		    -I$(_DRM_SOURCE_PATH)certs                 \
		    -I$(_DRM_SOURCE_PATH)certs/nd              \
		    -I$(_DRM_SOURCE_PATH)xmr                   \
		    -I$(_DRM_SOURCE_PATH)ndtinc                \
		
		INCLUDES += \
		           -I$(_DRM_SOURCE_PATH)oem/common/inc \
		           -I$(_DRM_SOURCE_PATH)oem/ansi/inc   \
		           -I$(_DRM_SOURCE_PATH)oem/ansi/trace
		
		DEFINES += -DSUPPORT_PLAYREADY -DASF_SUPPORT_DRM
	else
		ifeq ($(SUPPORT_PLAYREADY20_FLAG), YES)
			# DEFINES  will be overwritten after DrmSetEnvironment.mk is included
			# because MakeConfig is included twice.
			ORG_DEFINES = $(DEFINES) 
				
			_DRM_SOURCE_PATH=./Playready2.0/source/
			_DRM_ROOT_PATH=Playready2.0/
			_DMP_ROOT_PATH=../../../../
			include ./Playready2.0/DrmSetEnvironment.mk
				
			#some compile flags has to be defined first
			include ./Playready2.0/DrmProfileCompileTimeOptions.mk                 
			DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) $(_DRM_C_DEFINES)
  	
			DEFINES += -DSUPPORT_PLAYREADY20 -DASF_SUPPORT_DRM
		endif
	endif
endif

ifeq ($(ENABLE_NRD), YES)
NETFLIX_TEST = YES
SRCS = InputASF.c ASFGUID.c asf_parse.c Security.c
OBJS = InputASF.o ASFGUID.o asf_parse.o Security.o
else
NETFLIX_TEST = NO
SRCS = ASFGUID.c Security.c asf_parse_headers.c asf_index.c
OBJS = ASFGUID.o Security.o asf_parse_headers.o asf_index.o
SRCS += InputASF_new.c asf_parse_new.c
OBJS += InputASF_new.o asf_parse_new.o 

  ifeq ($(SUPPORT_PLAYREADY_FLAG), YES)
	SRCS += asf_drm.c
	OBJS += asf_drm.o
  else
	ifeq ($(SUPPORT_WMDRM_FLAG), YES)
		SRCS += asf_drm.c
		OBJS += asf_drm.o
	else
		ifeq ($(SUPPORT_PLAYREADY20_FLAG), YES)
	    	SRCS += asf_drm.c
	    	OBJS += asf_drm.o
	  	endif
	endif
  endif

endif


ifeq ($(NETFLIX_TEST), YES)
DEFINES += -DNETFLIX_TEST
endif


.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.c

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done



ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) -shared -o $(LIBNAME) $(OBJS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
