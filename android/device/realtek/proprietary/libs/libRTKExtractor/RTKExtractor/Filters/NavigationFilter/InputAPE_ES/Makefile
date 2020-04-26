##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCXX) 
#CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../Include/Filters -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

CFLAGS += -Wno-multichar

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputAPE.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputAPE.a
endif

SRCS = InputAPE_ES.cpp InputAPE_Utils.cpp APEHeaderManager.cpp
OBJS = InputAPE_ES.o InputAPE_Utils.o APEHeaderManager.o

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) $(WARNING) $(LIBS) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(LIBS) -c $*.cpp

all: depend

depend: $(LIBNAME)

copy:
	rm -rf ../../../Include/Filters/InputAPE
	mkdir -p ../../../Include/Filters/InputAPE/APE_SDK_407/Shared
	mkdir -p ../../../Include/Filters/InputAPE/APE_SDK_407/MACLib
	cp -rfd ./APE_SDK_407/Shared/*.h ../../../Include/Filters/InputAPE/APE_SDK_407/Shared/
	cp -rfd ./APE_SDK_407/MACLib/*.h ../../../Include/Filters/InputAPE/APE_SDK_407/MACLib/

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

