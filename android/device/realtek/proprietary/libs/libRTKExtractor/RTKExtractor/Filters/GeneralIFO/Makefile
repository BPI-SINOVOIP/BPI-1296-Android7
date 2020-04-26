include ../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)
LIBS=

LIBNAME= ../../../lib/libGeneralIFO.a

SRCS = GeneralIFO.cpp GeneralIFO_Persist.cpp EditingInfo.cpp

OBJS = GeneralIFO.o GeneralIFO_Persist.o EditingInfo.o

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
