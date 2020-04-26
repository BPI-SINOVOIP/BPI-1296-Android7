-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig

CXX = $(MYCXX)
AR = $(MYAR) cq
rm = /bin/rm -f

RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libGPIO.a

INCLUDES=  -I../../Include/

ifeq ($(TARGET_BOARD), YES)
DEFINES += -DTARGET_BOARD
endif

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS+= $(DEFINES)

SRCS = ./GPIO.cpp
OBJS = ./GPIO.o

.cpp.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
#	$(SHOW_COMMAND)$(CXX) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp
	$(SHOW_COMMAND)$(CXX) $(CFLAGS) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~

