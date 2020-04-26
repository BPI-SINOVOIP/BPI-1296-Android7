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
LIBNAME= $(_DRM_LIB_PATH)libdrmconstants.a

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)


SRCS = drmconstants.c    \
          drmlicacqv3constants.c     \
          drmxmlsigconstants.c       \
          drmversionconstants.c      \
          drmsoapxmlconstants.c      \
          drmpubkeydeviceexclusion.c 

OBJS = drmconstants.o             \
          drmlicacqv3constants.o     \
          drmxmlsigconstants.o       \
          drmversionconstants.o      \
          drmsoapxmlconstants.o      \
          drmpubkeydeviceexclusion.o 

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

