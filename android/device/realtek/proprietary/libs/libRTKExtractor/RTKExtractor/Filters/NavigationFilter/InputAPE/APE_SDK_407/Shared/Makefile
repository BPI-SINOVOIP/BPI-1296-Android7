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
LIBNAME= ../../../../../../lib/libMACShared.a 

#INCLUDES= -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system
INCLUDES= -I./ -I../MACLib/ 

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -g
#DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 
CFLAGS= $(DEFINES) $(COMPILE_OPT)

#ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
#CFLAGS+= -D__NAV_USE_DLL_PLUGIN__
#endif

#CFLAGS+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)
#CFLAGS+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

#SRCS = APEInfoDialog.cpp CharacterHelper.cpp CircleBuffer.cpp GlobalFunctions.cpp StdLibFileIO.cpp Unicows.cpp WAVInfoDialog.cpp WinFileIO.cpp 
#SRCS = CharacterHelper.cpp CircleBuffer.cpp GlobalFunctions.cpp StdLibFileIO.cpp Unicows.cpp 
SRCS = GlobalFunctions.cpp StdLibFileIO.cpp CharacterHelper.cpp CircleBuffer.cpp 

#OBJS = APEInfoDialog.o CharacterHelper.o CircleBuffer.o GlobalFunctions.o StdLibFileIO.o Unicows.o WAVInfoDialog.o WinFileIO.o 
#OBJS = CharacterHelper.o CircleBuffer.o GlobalFunctions.o StdLibFileIO.o Unicows.o 
OBJS = GlobalFunctions.o StdLibFileIO.o CharacterHelper.o CircleBuffer.o


.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) -O2 -c -Wall $*.cpp
# To remove the $(DEBUGFLAG) to use compiler optimization.
#$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.cpp

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
