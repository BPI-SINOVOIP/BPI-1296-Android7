-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig

MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR= $(MYAR) cq
RANLIB=$(MYRANLIB)
LIBNAME= ../../../lib/libDataTransportServer.a

INCLUDES= -I. -I../../Include

CFLAGS	= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 

ifeq ($(ENABLE_MO_SCREEN_CAPTURE), YES)
CFLAGS+= -fPIC
endif

ifeq ($(USE_WEBKIT_VIDEO_EXTENSION),YES)
CFLAGS+= -fPIC
endif
OBJS 	= DataTransportServer.o SocketTransport.o SharedMemTransport.o

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
