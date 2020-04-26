##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../../Include -I../../../../../../common/IPC/include -I../../../../Include/Filters  
INCLUDES+= -I../../../../Include -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif
ifeq ($(TOOLCHAIN_VER), 344)
DEFINES += -DTOOLCHAIN344
endif
USE_ZLIB = NO
ifeq ($(USE_ZLIB), YES)
DEFINES += -DENABLE_CMOV
endif
#DEFINES += -DDBG
#DEFINES += -DENABLE_BIG_FILE
#DEFINES += -DHACK
#CFLAGS= $(DEFINES) $(COMPILE_OPT)
CFLAGS= $(DEFINES)
#CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES)

ifeq ($(REMOVE_PRINTF_FLOW), YES)
CFLAGS += -include "noprintf.h"
endif

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../../lib/InputMP4.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__ 
else
LIBNAME= ../../../../../lib/libInputMP4_foryoutube.a
endif

CFLAGS += -Wno-multichar
SRCS = InputMP4.c mp4_parse.c mp4ff.c mp4atom.c mp4util.c mp4_bits.c mp4.c mp4_progconfig.c mp4_stream.c mp4_getinfo.c Security.c mp4_foryoutube.c
OBJS = InputMP4.o mp4_parse.o mp4ff.o mp4atom.o mp4util.o mp4_bits.o mp4.o mp4_progconfig.o mp4_stream.o mp4_getinfo.o Security.o mp4_foryoutube.o
SRCFILEDIR = ..

ifeq (YES, $(USE_DTA_LIB))
DEFINES += -UUSE_DTA_LIB
USE_DTA_LIB = NO
endif
ifeq (YES, $(USE_DTA_LIB))
DEFINES += -DBESTVDTA
SRCS += Ibcti.cpp
OBJS += Ibcti.o
endif

ifeq (YES, $(ENABLE_FILELIST_PLAYBACK))
DEFINES += -UENABLE_FILELIST_PLAYBACK
endif

FOR_YOUTUBE = YES
ifeq (YES, $(FOR_YOUTUBE))
DEFINES += -DFOR_YOUTUBE
CFLAGS += -fPIC
endif

%.o : $(SRCFILEDIR)/%.c
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$<] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(INCLUDES) $(DEBUGFLAG) -c -o $@ $< 

%.o : $(SRCFILEDIR)/%.cpp
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$<] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(INCLUDES) $(DEBUGFLAG) -c -o $@ $< 

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

