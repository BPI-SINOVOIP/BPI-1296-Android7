##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libResource.a
INSTALLHEADER= ../../../include

ftcfg = ../../Platform_Lib/Graphics/FontRenderer/freetype-config
CXXFLAGS := `$(ftcfg) --cflags` -I$(INC_DIR) -Wall
LDFLAGS := `$(ftcfg) --libs`
ifeq ($(ENABLE_DFB_VKBD), YES)
#for compile dfb_vkd
CXXFLAGS += -fPIC
DEFINES  += -DDFB_VKBD
endif

INCLUDES=  -I. -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS= $(COMPILE_OPT)


SRCS = resource.cpp ioResource.cpp

OBJS = resource.o ioResource.o

.cpp.o:
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CXXFLAGS) $(CFLAGS) $(DEBUGFLAG) $(DEFINES) -c $(SRCS)

all: $(LIBNAME)

dep:
#	$(MAKE) -C ../../Resource/supplement/

$(LIBNAME) : dep $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~
