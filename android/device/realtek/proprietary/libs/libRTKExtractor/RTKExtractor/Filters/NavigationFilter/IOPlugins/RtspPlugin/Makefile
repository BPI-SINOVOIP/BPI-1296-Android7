-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
#CC = gcc
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../../Include -I../../../../Platform_Lib/HTTPC/src -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN 
CFLAGS= $(DEFINES) $(COMPILE_OPT)


LIBNAME= ../../../../../lib/libRtspIOPlugin.a
#MAIN = ./main

#ifneq ($(filter APCFG_MODULE_IMS,$(subst |, ,$(APCFG_FLAGS))), )
SRCS = RtspIOPlugin.c rtsp_rm.c rtsp_asf.c common.c rtspdown.c net_wrap.c sdplive.c rtsp_real_live_dss.c librtsp.c sdpreal.c sdpwms.c rtsp_wms.c

OBJS = RtspIOPlugin.o rtsp_rm.o rtsp_asf.o common.o rtspdown.o net_wrap.o sdplive.o rtsp_real_live_dss.o librtsp.o sdpreal.o sdpwms.o rtsp_wms.o


#else
#SRCS = HttpIOPlugin_empty.c TranscodeIOPlugin.c io_helper.c

#OBJS = HttpIOPlugin_empty.o TranscodeIOPlugin.o io_helper.o
#endif

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME) $(MAIN)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
#$(MAIN) : $(OBJS)
#	$(CC) -o $(MAIN) -static $(OBJS) -lpthread

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~

