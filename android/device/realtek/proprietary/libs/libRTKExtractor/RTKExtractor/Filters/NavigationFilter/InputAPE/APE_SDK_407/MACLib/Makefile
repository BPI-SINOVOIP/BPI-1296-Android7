##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
#-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../../include/MakeConfig

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
LIBNAME= ../../../../../../lib/libMACLib.a 

#INCLUDES= -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system
INCLUDES= -I./ -I../Shared/ 
LIBS = -L. -L../Shared -L../../../../../../lib -lMACShared -lMACLib_OLD

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT) -g
#CFLAGS= $(DEFINES) $(COMPILE_OPT) 

SUBDIRS += Old

#ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
#CFLAGS+= -D__NAV_USE_DLL_PLUGIN__
#endif

#CFLAGS+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)
#CFLAGS+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

SRCS = MACLib.cpp APECompressCore.cpp APECompress.cpp APECompressCreate.cpp APEDecompress.cpp APEHeader.cpp APEInfo.cpp APELink.cpp APESimple.cpp APETag.cpp BitArray.cpp MACProgressHelper.cpp MD5.cpp NewPredictor.cpp NNFilter.cpp Prepare.cpp UnBitArrayBase.cpp UnBitArray.cpp WAVInputSource.cpp

OBJS = MACLib.o APECompressCore.o APECompress.o APECompressCreate.o APEDecompress.o APEHeader.o APEInfo.o APELink.o APESimple.o APETag.o BitArray.o MACProgressHelper.o MD5.o NewPredictor.o NNFilter.o Prepare.o UnBitArrayBase.o UnBitArray.o WAVInputSource.o


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
