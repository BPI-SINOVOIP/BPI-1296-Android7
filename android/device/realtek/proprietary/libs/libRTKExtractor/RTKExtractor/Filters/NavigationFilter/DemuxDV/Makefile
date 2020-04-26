##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
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

INCLUDES= -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include -I../../../../../common/IPC/generate/include/audio/

DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -Dohci_1394 -Dohci_1394_device -DTARGET_BOARD
CFLAGS = $(DEFINES) $(COMPILE_OPT)
CFLAGS+= -DDBG_MSG_EN -UDBG_DROPFRAME_EN
#CFLAGS+= -O2 #-g
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/DemuxDV.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN_ _
else
LIBNAME= ../../../../lib/libDemuxDV.a
endif

SRCS = CDemuxDV_mips.cpp CDemuxDVFile.cpp
OBJS = $(SRCS:.cpp=.o)

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
	$(rm) $(OBJS) $(LIBNAME) core *~
