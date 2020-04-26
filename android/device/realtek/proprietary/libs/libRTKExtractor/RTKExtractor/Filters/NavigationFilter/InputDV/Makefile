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

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include -I../../../../../common/IPC/generate/include/audio/


DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -Dohci_1394_device 
DEFINES+= -DDBG_MSG_EN -UDBG_NAVBUFFER_EN -DEXT_AVC_CMD
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS+= $(DEFINES) $(COMPILE_OPT) -O2 -ggdb
CFLAGS+= -DCONFIG_DV_DISPLAY_CTRL

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputDV.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN_ 
else
LIBNAME= ../../../../lib/libInputDV.a
endif

ifeq ($(USE_1394), YES)
SRCS = InputDV_mips.c
OBJS = InputDV_mips.o
INCS = InputDV_mips.h avc_uapi.h uapi.h
else
SRCS = InputDV.cpp
OBJS = InputDV.o
INCS = InputDV.h
SUBDIRS= ohci1394
endif

#.c.o:
#	$(rm) -f $@
#	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
#	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.c

.cpp.o: $(SRCS) $(OBJS)
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp
#	$(CC) $(CFLAGS) $(DEBUGFLAG) -static -c $*.cpp

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
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
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

