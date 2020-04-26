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

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputNRD.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputNRD.a
endif

ifeq ($(SUPPORT_PLAYREADY_FLAG), YES)
#_DRM_SOURCE_PATH := ../InputASF/PlayReady/Source/
#include ../InputASF/PlayReady/DrmSetEnvironment.mk

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

DEFINES += -DSUPPORT_PLAYREADY 
endif

ifeq ($(SUPPORT_WMDRM_FLAG), YES)
include ../InputASF/WMDRMPD_1007/makefile.inc.2
INCLUDES+= -I../InputASF/WMDRMPD_1007/Source/inc
DEFINES += -DSUPPORT_WMDRM_PD
endif

SRCS = InputNRD NRDGUID.c nrd_parse.c Security.c
OBJS = InputNRD.o NRDGUID.o nrd_parse.o Security.o

DEFINES += -DNETFLIX_TEST


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
