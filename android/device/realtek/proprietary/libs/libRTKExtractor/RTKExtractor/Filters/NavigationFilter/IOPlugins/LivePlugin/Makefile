-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
INCLUDES = -I../../../../Platform_Lib/live/live/UsageEnvironment/include -I../../../../Platform_Lib/live/live/groupsock/include -I../../../../Platform_Lib/live/live/liveMedia/include -I../../../../Platform_Lib/live/live/BasicUsageEnvironment/include
INCLUDES+= -I. -I../../../../Include -I../../../../Platform_Lib/HTTPC/src -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system -I../../../../Platform_Lib/live/

rm=/bin/rm -f
CC= $(MYCC)
CXX=$(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
	
DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN 
CFLAGS= $(DEFINES) $(COMPILE_OPT)

LIBNAME= ../../../../../lib/libLiveIOPlugin.a


SRCS =liveIoplugin.cpp   openRTSP.cpp  rtsp_rm.c rtsp_asf.c  common.c  rtspdown.c  net_wrap.c sdplive.c  rtsp_real_live_dss.c  librtsp.c  sdpreal.c  sdpwms.c rtsp_wms.c

OBJS =liveIoplugin.o    openRTSP.o   rtsp_rm.o   rtsp_asf.o  common.o  rtspdown.o  net_wrap.o  sdplive.o  rtsp_real_live_dss.o  librtsp.o  sdpreal.o sdpwms.o rtsp_wms.o

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~

