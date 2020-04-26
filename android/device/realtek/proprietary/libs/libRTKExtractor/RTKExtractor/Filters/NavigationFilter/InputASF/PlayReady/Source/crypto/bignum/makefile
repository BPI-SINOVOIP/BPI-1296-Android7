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
LIBNAME= $(_DRM_LIB_PATH)libdrmbignum.a

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)


SRCS = bignum.c digitogcd.c divide.c divisibility.c ecex2001.c ecexpon.c ecppq.c ecurve.c field.c kdiv.c kinitpr.c kinvert.c     \
       kmuladd.c lucas.c modexp.c modmulch1.c modsqrt.c modular.c mpaddsubcmp.c mpalloc.c mpgcdex.c mpmul22.c mprand.c multlow.c  

OBJS = bignum.o digitogcd.o divide.o divisibility.o ecex2001.o ecexpon.o ecppq.o ecurve.o field.o kdiv.o kinitpr.o kinvert.o     \
       kmuladd.o lucas.o modexp.o modmulch1.o modsqrt.o modular.o mpaddsubcmp.o mpalloc.o mpgcdex.o mpmul22.o mprand.o multlow.o  


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

