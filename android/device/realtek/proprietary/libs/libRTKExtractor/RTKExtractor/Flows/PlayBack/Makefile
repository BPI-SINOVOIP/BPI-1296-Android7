-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libplaybackflow.a

INCLUDES= -I../../Include

DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)

SRCS= CPlaybackFlowManager.cpp

OBJS= CPlaybackFlowManager.o

.cpp.o:
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS) $(INSTALL)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
