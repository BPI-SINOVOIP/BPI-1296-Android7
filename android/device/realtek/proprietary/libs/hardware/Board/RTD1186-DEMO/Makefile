-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig

MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR= $(MYAR) cq
RANLIB=$(MYRANLIB)
LIBNAME= ../../../../lib/libBoard.a

INCLUDES=  -I../../../Include/ -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)

SRCS = ./board.cpp
OBJS = ./board.o 
OBJS_COMMON = ../BoardCommon.o

.cpp.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS) $(OBJS_COMMON)
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~
