-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

#INCLUDES= -I. -I../../../../Include
INCLUDES= -I. -I../../../../Include -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system
INCLUDES += -I../../../../Include/Platform_Lib/SocketAVData -I../../../../Include/Filters/NavigationFilter/SiDvb/include 
INCLUDES += -I../../../../Include/Filters/NavigationFilter
#-I../../../../Include/Filters/NavigationFilter/SiDvb/include/system/si4

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
# -DSUPPORT_PREFETCH
CFLAGS= $(DEFINES) $(COMPILE_OPT)


LIBNAME= ../../../../../lib/libDummyFileIOPlugin.a

SRCS = DummyFileSetIOPlugin.c

OBJS = DummyFileSetIOPlugin.o

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
