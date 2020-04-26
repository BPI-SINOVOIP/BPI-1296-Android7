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

DEFINES+= -DDTV_DIRECT_IO_WRITE_18382

ifeq ($(MAIN_PROJECT), Tinkerbell)
DEFINES+= -DENABLE_TS_ABREPEAT
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT)
LIBS=

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputDTV.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
LIBS+=-L../../../../lib -lOSAL
else
LIBNAME= ../../../../lib/libInputDTV.a
endif

SRCS = CInputDTV.cpp CDtvFileReader.cpp CDtvFileWriter.cpp CDtvBitRateEst.cpp CDtvContextInfo.cpp CDtvFileEditor.cpp CDtvSIInterface.cpp CDtvTunerMode.cpp

OBJS = CInputDTV.o CDtvFileReader.o CDtvFileWriter.o CDtvBitRateEst.o CDtvContextInfo.o CDtvFileEditor.o CDtvSIInterface.o CDtvTunerMode.o

ifeq ($(ENABLE_BLURAY_3D), YES)
ENABLE_DTV_3D = YES
  ifeq ($(ENABLE_DTV_3D), YES)
	SRCS += CDtvSubPath.cpp
	OBJS += CDtvSubPath.o
	DEFINES+= -DENABLE_DTV_3D
  endif
endif

#To fix bug 28712
#DEFINES+= -DFIX_BUG28712_CHECK_CC

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
