##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
include ../../../../../../../include/MakeConfig

#TOOLCHAIN_ROOT = /usr/local/toolchain_mipsel/bin
#MYCC = $(TOOLCHAIN_ROOT)/mipsel-linux-gcc
#MYCXX = $(TOOLCHAIN_ROOT)/mipsel-linux-g++
#MYAR = $(TOOLCHAIN_ROOT)/mipsel-linux-ar
#MYRANLIB = $(TOOLCHAIN_ROOT)/mipsel-linux-ranlib
#DEFINES+= -mips32r2


rm=/bin/rm -f
CC= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../../../../../lib/libMACLib_OLD.a 

#INCLUDES= -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system
INCLUDES= -I./ -I../../Shared/ 
LIBS = -L. -L../../Shared -L../../../../../../../lib -lMACShared

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT) -g
#CFLAGS= $(DEFINES) $(COMPILE_OPT) 

#ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
#CFLAGS+= -D__NAV_USE_DLL_PLUGIN__
#endif

#CFLAGS+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)
#CFLAGS+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

SRCS = Anti-Predictor.cpp AntiPredictorFast.cpp AntiPredictorNormal.cpp APEDecompressOld.cpp UnMAC.cpp AntiPredictorExtraHigh.cpp AntiPredictorHigh.cpp APEDecompressCore.cpp UnBitArrayOld.cpp

OBJS = Anti-Predictor.o AntiPredictorFast.o AntiPredictorNormal.o APEDecompressOld.o UnMAC.o AntiPredictorExtraHigh.o AntiPredictorHigh.o APEDecompressCore.o UnBitArrayOld.o


.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) -O2 $(LIBS) -c -Wall $*.cpp
# To remove the $(DEBUGFLAG) to use compiler optimization.
#$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(LIBS) -c -Wall $*.cpp

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
