-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
CXX= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)


SRC_DIR = ../../../..

INCLUDES= -I. -I../../../../Include
INCLUDES += -I$(SRC_DIR)/../../common/IPC/generate/include/system/ -I$(SRC_DIR)/../../common/IPC/include
INCLUDES += -I$(SRC_DIR)/Include/freetype2

INCLUDES += -I$(SRC_DIR)/Include/Utility/tinyxml
INCLUDES += -I$(SRC_DIR)/Application/Win32/RSSClient/RSSDialogView
INCLUDES += -I$(SRC_DIR)/Application/Win32/RSSClient/RSSParser

ifeq ($(ENABLE_IMS3D), YES)
INCLUDES += -I$(SRC_DIR)/Include/Filters/NavigationFilter
INCLUDES += -I$(SRC_DIR)/Include/Filters/NavigationFilter/SiDvb/include
INCLUDES += -I$(SRC_DIR)/Include/Filters/NavigationFilter/SiAtsc
INCLUDES+= -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES
INCLUDES+=  -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES/OGLES2/Include -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES/PVRTools/OGLES2
INCLUDES += -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES/PVRShell -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES/PVRShell/API/KEGL -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES/PVRShell/OS/LinuxRaw
INCLUDES += -I$(SRC_DIR)/Application/Win32/RSSClient/RSSOGLES/ftgles/ftgles_src
endif

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN 
CFLAGS= $(DEFINES) $(COMPILE_OPT) -save-temps

LIB_PPS=../../../../../../../ppsvod_for_realtek/
LIBNAME= ../../../../../lib/libBesTVIOPlugin.a
EXENAME = main

ifeq (YES, $(USE_DTA_LIB))
SRCS = BesTVPlugin_DTA.cpp
else
SRCS = BesTVPlugin.c
endif

ifeq (YES, $(USE_DTA_LIB))
OBJS = BesTVPlugin_DTA.o
else
OBJS = BesTVPlugin.o
endif



CFLAGS += -g -O2 

#Saturn use hw_fpu
ifneq ($(filter 0x1%6, $(CHIP_MODEL)), $(CHIP_MODEL))
CFLAGS += -msoft-float
endif

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c  $*.c

%.o:	%.cpp
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c  $*.cpp

all: $(LIBNAME) 
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

$(EXENAME) : $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(EXENAME)  $(SRCS) -L../../../../../lib/ -L$(LIB_PPS) -lpthread -lppsbase -lppssg -lppsapi -lppsvodres -lppsfds -lppsvodnet -msoft-float

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~ $(EXENAME)
