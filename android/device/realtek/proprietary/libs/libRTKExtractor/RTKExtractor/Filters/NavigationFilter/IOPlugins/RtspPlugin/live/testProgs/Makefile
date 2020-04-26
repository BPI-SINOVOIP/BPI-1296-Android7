-include ../../../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../../../include/MakeConfig
INCLUDES = -I../UsageEnvironment/include -I../groupsock/include -I../liveMedia/include -I../BasicUsageEnvironment/include
INCLUDES+= -I. -I../../../../../../Include -I../../../../../../Platform_Lib/HTTPC/src -I../../../../../../../../common/IPC/include -I../../../../../../../../common/IPC/generate/include/system

rm=/bin/rm -f
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
##### Change the following for your environment: 
CROSS_COMPILE?=		/usr/local/toolchain_mipsel/bin/mipsel-linux-
COMPILE_OPTS =		$(INCLUDES) -I. -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 
C =			c
C_COMPILER =		$(CROSS_COMPILE)gcc
C_FLAGS =		$(COMPILE_OPTS)
CPP =			cpp
CPLUSPLUS_COMPILER =	$(CROSS_COMPILE)g++
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1  -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN $(COMPILE_OPT) $(DEBUGFLAG)
OBJ =			o
LINK =			$(CROSS_COMPILE)g++ -static   -o
LINK_OPTS =		
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		$(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS =	$(LINK_OPTS)
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
##### End of variables to change

LIBNAME= ../../../../../../../lib/libRtspIOPlugin.a

UNICAST_RECEIVER_APPS =rtspIoplugin$(EXE) openRTSP$(EXE) 
UNICAST_APPS = $(UNICAST_RECEIVER_APPS)


#ALL =$(UNICAST_APPS) $(LIBNAME)
ALL = $(LIBNAME)
all: $(ALL)


.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<       

.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER)    -c $(CPLUSPLUS_FLAGS) $<


OPEN_RTSP_OBJS    = openRTSP.$(OBJ) playCommon.$(OBJ)
RTSP_IOPLUGIN_OBJS    =rtspIoplugin.$(OBJ) openRTSP.$(OBJ) 



openRTSP.$(CPP):	playCommon.hh
playCommon.$(CPP):	playCommon.hh


USAGE_ENVIRONMENT_DIR = ../UsageEnvironment
USAGE_ENVIRONMENT_LIB = $(USAGE_ENVIRONMENT_DIR)/libUsageEnvironment.$(LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_DIR = ../BasicUsageEnvironment
BASIC_USAGE_ENVIRONMENT_LIB = $(BASIC_USAGE_ENVIRONMENT_DIR)/libBasicUsageEnvironment.$(LIB_SUFFIX)
LIVEMEDIA_DIR = ../liveMedia
LIVEMEDIA_LIB = $(LIVEMEDIA_DIR)/libliveMedia.$(LIB_SUFFIX)
GROUPSOCK_DIR = ../groupsock
GROUPSOCK_LIB = $(GROUPSOCK_DIR)/libgroupsock.$(LIB_SUFFIX)
LOCAL_LIBS =	$(LIVEMEDIA_LIB) $(GROUPSOCK_LIB) \
		$(BASIC_USAGE_ENVIRONMENT_LIB) $(USAGE_ENVIRONMENT_LIB)
LIBS =			$(LOCAL_LIBS) $(LIBS_FOR_CONSOLE_APPLICATION)   -lpthread  


openRTSP$(EXE):	$(OPEN_RTSP_OBJS) $(LOCAL_LIBS)
	$(LINK)$@ $(CONSOLE_LINK_OPTS) $(OPEN_RTSP_OBJS) $(LIBS)
rtspIoplugin$(EXE):	$(RTSP_IOPLUGIN_OBJS) $(LOCAL_LIBS)
	$(LINK)$@  $(CONSOLE_LINK_OPTS) $(RTSP_IOPLUGIN_OBJS) $(LIBS) 

$(LIBNAME) : $(RTSP_IOPLUGIN_OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(RTSP_IOPLUGIN_OBJS) 
#	$(SHOW_COMMAND)$(RANLIB) $@


clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

##### Any additional, platform-specific rules come here:
