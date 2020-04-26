#**@@@+++@@@@*********************************************************
#*
#* Microsoft Portable Build
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#include ../../../include/MakeConfig
_DRM_SOURCE_PATH := ../../
include ../../../DrmSetEnvironment.mk

rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= $(_DRM_LIB_PATH)libdrmtoolscommon.a

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) -fshort-wchar -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)

INCLUDES += \
           -I$(_DRM_SOURCE_PATH)asfmmstr/inc

SRCS = drmtoolsconstants.c     \
	drmtoolsutils.c	    \
	drmtoolsasfparser.c \
	drmtoolsmediafile.c       

OBJS = drmtoolsconstants.o     \
	drmtoolsutils.o	    \
	drmtoolsasfparser.o \
	drmtoolsmediafile.o


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
