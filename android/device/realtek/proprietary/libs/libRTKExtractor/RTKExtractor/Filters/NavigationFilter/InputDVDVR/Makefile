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
MYDEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES=  -I. -I../../../Include -I../../Include/Filters -I../../Include/Utility/database
INCLUDES+= -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system


MYDEFS+= -DSYS_UNIX=1 
#MYDEFS+= -D_DEBUG

#USE_IOCACHE = YES
ifeq ($(USE_IOCACHE), YES)
MYDEFS+= -DUSE_IOCACHE=1
endif

#USE_VOB_CACHE_DATA = YES
ifeq ($(USE_VOB_CACHE_DATA), YES)
MYDEFS+= -DVOB_CACHE_DATA=1
endif

CFLAGS= $(INCLUDES) $(DEFINES) $(MYDEFS) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputDVDVR.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputDVDVR.a
endif

SRCS = input_vr.cpp

OBJS = input_vr.o

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.cpp

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
