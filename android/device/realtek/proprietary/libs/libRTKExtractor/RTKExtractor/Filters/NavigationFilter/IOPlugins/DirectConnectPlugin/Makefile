-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
CXX= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../../Include -I../../../../Include/Utility

ifeq ($(USE_WIDI_ONLY), YES)
DEFINES += -DUSE_WIDI_ONLY
endif

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN -DSUPPORT_PREFETCH
CFLAGS= $(DEFINES) $(COMPILE_OPT)


LIBNAME= ../../../../../lib/libDirectConnectIOPlugin.a

SRCS = DirectConnectIOPlugin.cpp DirectConnectTools.cpp DirectConnectManager.cpp DirectConnectManager.cpp DirectConnectKernel.cpp

OBJS = DirectConnectIOPlugin.o DirectConnectTools.o DirectConnectManager.o DirectConnectManager.o DirectConnectKernel.o

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

all: version_info $(LIBNAME)

version_info:
	svn info ../../../../Unit_test/$(MAIN_PROJECT) | grep Revision | awk '{ printf("static char REVISION[]= \"%s\";\n", $$0) }' > ver.h

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
