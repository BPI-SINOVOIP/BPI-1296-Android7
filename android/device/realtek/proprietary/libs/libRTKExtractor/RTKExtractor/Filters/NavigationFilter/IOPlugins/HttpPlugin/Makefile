-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CXX=$(MYCXX)
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../../Include -I../../../../Platform_Lib/HTTPC/src -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system
INCLUDES+= -I../../../../Include/Utility -I../../../../Include/Utility/playlist -I../../../../Platform_Lib/openssl/include
ifeq ($(ENABLE_DTCP_IP), YES)
INCLUDES+= -I../../../../Include/Utility/dtcp-ip
endif


DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN 

ifeq ($(EXCLUDE_OPENSSL), YES)
DEFINES+= -DEXCLUDE_OPENSSL
endif

ifeq ($(USE_DEBUG_MEM_DMALLOC), YES)
DEFINES+= -D_DEBUG_MEM_DMALLOC_
INCLUDE+= -I../../../../Unit_test/Casablanca
endif

#if need to enable record and replay
#INCLUDE+= -I../FilePlugin
#DEFINES+= -D__HLS_DEBUG_REPLAY_AND_RELOAD__

CFLAGS= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS += $(INCLUDE) $(CFLAGS)

LIBNAME= ../../../../../lib/libHttpIOPlugin.a

#ifeq ($(ENABLE_NRD), YES)
ifneq (,$(filter YES,$(ENABLE_NRD) $(USE_DIRECT_CONNECT)))
SRCS = httpBufferThread.c HttpIOPlugin_asy.c HttpIOPluginProxy.cpp HttpLiveStreamManager.cpp PreloadClip.cpp
OBJS = httpBufferThread.o HttpIOPlugin_asy.o HttpIOPluginProxy.o HttpLiveStreamManager.o PreloadClip.o
else
ifneq ($(filter APCFG_MODULE_IMS,$(subst |, ,$(APCFG_FLAGS))), )
SRCS = httpBufferThread.c HttpIOPlugin_asy.c HttpIOPluginProxy.cpp HttpLiveStreamManager.cpp PreloadClip.cpp
OBJS = httpBufferThread.o HttpIOPlugin_asy.o HttpIOPluginProxy.o HttpLiveStreamManager.o PreloadClip.o
else
    ifneq ($(filter APCFG_APP_HTTPCLIENT,$(subst |, ,$(APCFG_FLAGS))), )
    SRCS = HttpIOPlugin.c 
    OBJS = HttpIOPlugin.o
    else
SRCS = HttpIOPlugin_empty.c
OBJS = HttpIOPlugin_empty.o
    endif
endif
endif
ifeq ($(ENABLE_MPEG_DASH_PLAYBACK), YES)
INCLUDES+= -I../../../../Include/Utility/MPDParser -I../../../../Utility/MediaSourcePlayer -I../../../../Utility/MediaSourcePlayer/standalone -I../../../../Utility/MediaSourcePlayer/chromium -I../../../../Include/Platform_Lib/SocketAVData -I../../../../Platform_Lib/DataTransportServer
SRCS += DynamicAdaptiveStreamHttpManager.cpp
OBJS += DynamicAdaptiveStreamHttpManager.o
endif
.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(CXXFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
