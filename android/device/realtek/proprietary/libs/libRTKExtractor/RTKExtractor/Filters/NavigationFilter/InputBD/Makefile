include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
DEFINES+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)

ifeq ($(RESET_DTV_FILE_PLAYBACK_RESYNC_THRESHOLD), YES)
	DEFINES+= -DDTV_FILE_PLAYBACK_RESYNC_THRESHOLD=$(DTV_FILE_PLAYBACK_RESYNC_THRESHOLD)
else
	DEFINES+= -DDTV_FILE_PLAYBACK_RESYNC_THRESHOLD=962560
endif

#DEFINES+= -DREMOVEIT
DEFINES+= -DFIX_ISSUE_CHANGE_SPEED_AT_ITEM_BOUNDARY

CFLAGS= $(DEFINES) $(COMPILE_OPT)
LIBS=

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputBD.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
LIBS+=-L../../../../lib -lOSAL
else
LIBNAME= ../../../../lib/libInputBD.a
endif

SRCS = CInputBD.cpp CBdReader.cpp CBdParser.cpp subpath_parse.cpp

OBJS = CInputBD.o CBdReader.o CBdParser.o subpath_parse.o

#SATURN
#ifeq ($(filter 0x1%6, $(CHIP_MODEL)), $(CHIP_MODEL))
	ifeq ($(ENABLE_BLURAY_3D), YES)
		SRCS += CBdSubPath.cpp EPMap.cpp CInputBD_3D.cpp
		OBJS += CBdSubPath.o EPMap.o CInputBD_3D.o
	endif
#endif

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
