-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)
LIBS=

#USE_IOCACHE = YES
#USE_IOCACHE = NO
ifeq ($(USE_IOCACHE), YES)
DEFINES+= -DUSE_IOCACHE=1
endif

#USE_VOB_CACHE_DATA = YES
#USE_VOB_CACHE_DATA = NO
ifeq ($(USE_VOB_CACHE_DATA), YES)
DEFINES+= -DVOB_CACHE_DATA=1
endif

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputDVDVideo.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
LIBS+=-L../../../../lib -lDVDCommon -lebase -ldvdaccess -lOSAL -ludfaccess -lCSS
else
LIBNAME= ../../../../lib/libInputDVDVideo.a
endif

CFLAGS+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

SRCS = CDVDVideoCmdHandler.cpp CDVDVideoVM.cpp NavRead.cpp CDVDVideoNav.cpp ifoHelpers.cpp

OBJS = CDVDVideoCmdHandler.o CDVDVideoVM.o NavRead.o CDVDVideoNav.o ifoHelpers.o

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.cpp

all: $(LIBNAME)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) -shared -o $(LIBNAME) $(OBJS) $(LIBS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
