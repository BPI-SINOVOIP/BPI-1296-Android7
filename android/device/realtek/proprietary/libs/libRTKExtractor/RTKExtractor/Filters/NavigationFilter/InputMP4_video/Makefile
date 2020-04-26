##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../Include/Filters  
INCLUDES+= -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

ifeq ($(USE_ZLIB), YES)
#DEFINES += -DENABLE_CMOV
endif

#DEFINES += -DENABLE_BIG_FILE

#CFLAGS= $(DEFINES) $(COMPILE_OPT)
CFLAGS= $(DEFINES)
#CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputMP4.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__ 
else
LIBNAME= ../../../../lib/libInputMP4.a
endif


SRCS = InputMP4.c mp4_parse.c mp4ff.c mp4atom.c mp4util.c mp4_bits.c mp4.c mp4_progconfig.c mp4_stream.c

OBJS = InputMP4.o mp4_parse.o mp4ff.o mp4atom.o mp4util.o mp4_bits.o mp4.o mp4_progconfig.o mp4_stream.o

PLAY_BIG_FILE = YES
ifeq ($(PLAY_BIG_FILE), YES)
SRCS += mp4_getinfo.c
OBJS += mp4_getinfo.o
DEFINES += -DBIG_FILE
endif
.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) -shared -o $(LIBNAME) $(OBJS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~

