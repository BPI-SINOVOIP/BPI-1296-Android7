#**@@@+++@@@@*********************************************************
#*
#* Microsoft Portable Build
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#include ../../../include/MakeConfig
_DRM_SOURCE_PATH := ../
include ../../DrmSetEnvironment.mk

rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= $(_DRM_LIB_PATH)libmsdbcore.a

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)

INCLUDES += \
           -I$(_DRM_SOURCE_PATH)xmr

SRCS = drmcfdalloc.c            \
    drmcfdbuildcrlupdate.c   \
    drmcfdbuildheader.c      \
    drmcfdbuildlicense.c     \
    drmcfddevicecertctx.c    \
    drmcfdlicenseutils.c     \
    drmcfdPackageCtx.c       \
    drmcfdpk.c               \
    drmcfd_crl_pack.c        \
    drmconstants_cfd.c       \
    drmbindevicecertctx.c    \
    drmmsdbbuildXMRlicense.c 

OBJS = drmcfdalloc.o            \
    drmcfdbuildcrlupdate.o   \
    drmcfdbuildheader.o      \
    drmcfdbuildlicense.o     \
    drmcfddevicecertctx.o    \
    drmcfdlicenseutils.o     \
    drmcfdPackageCtx.o       \
    drmcfdpk.o               \
    drmcfd_crl_pack.o        \
    drmconstants_cfd.o       \
    drmbindevicecertctx.o    \
    drmmsdbbuildXMRlicense.o 


.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.c

all: $(LIBNAME)
	
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

