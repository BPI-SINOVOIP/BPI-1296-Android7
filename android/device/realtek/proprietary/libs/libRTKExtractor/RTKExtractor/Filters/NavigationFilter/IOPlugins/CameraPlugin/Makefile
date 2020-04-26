-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES  = -I.
INCLUDES += -I../../../../Include
INCLUDES += -I../../../../Include/Platform_Lib/CameraControl -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
CFLAGS= $(DEFINES) $(COMPILE_OPT)
#CFLAGS += -DRECORD_VIDEO
CFLAGS += -DENABLE_DDR_COPY

LIBNAME= ../../../../../lib/libCameraIOPlugin.a

SRCS = CameraIOPlugin.cpp avilib.cpp

OBJS = CameraIOPlugin.o avilib.o

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
