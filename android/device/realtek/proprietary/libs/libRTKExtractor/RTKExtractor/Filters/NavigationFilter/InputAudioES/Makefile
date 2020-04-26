##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES += $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
DEFINES+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)
DEFINES+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputAudioES.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputAudioES.a
endif

SRCS = InputAudioES.c 

OBJS = InputAudioES.o 


.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

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
