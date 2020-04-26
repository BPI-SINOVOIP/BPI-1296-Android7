##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC) 
CXX= $(MYCXX) 

LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include  -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DHAVE_SYS_TYPES_H


ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT) 


ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputCommonUtility.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__ 
else
LIBNAME= ../../../../lib/libInputCommonUtility.a
endif


OBJS = 
OBJS += ID3.o AAC_Utility.o TS_Utility.o

ifeq ($(ENABLE_SOCKET_INPUT_PLUGIN), YES)
	OBJS += SocketAVData_Utility.o
	OBJS += MiniSIEngine/MiniSIEngine.o MiniSIEngine/TsParser.o
	
else
	ifeq ($(ENABLE_NRD3_INPUT_PLUGIN), YES)
	OBJS += SocketAVData_Utility.o
	endif
endif

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -O -c $< -o $@ 

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -O -c $< -o $@ 


all:$(LIBNAME)


ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CXX) -shared -o $(LIBNAME) $(OBJS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
