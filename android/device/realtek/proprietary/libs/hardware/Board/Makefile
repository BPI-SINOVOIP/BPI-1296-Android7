-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig

MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)

INCLUDES=  -I../../Include/ -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)

SRCS = ./BoardCommon.cpp
OBJS = ./BoardCommon.o 

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

SUBDIRS = $(BOARD_ID)

all: $(OBJS)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
clean: 
	$(SHOW_COMMAND)$(rm) $(OBJS) *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done; rm -rf ../lib/*
