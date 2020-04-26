-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig

MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR= $(MYAR) cq
RANLIB=$(MYRANLIB)
LIBNAME= ../../../../../lib/libVoutUtil.a

INCLUDES=  -I. -I../../../../Include -I../../../../Include/freetype2 -I../../../../../../common
INCLUDES+= -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system

ifeq ($(BUILD_CMDLINE_DVDPLAYER), YES)
ifeq ($(ENABLE_ANDROID), YES)
  MYDEFS+= -DTEST_ANDROID
endif
endif

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)

SRCS = ./VoutUtil.cpp 

OBJS = ./VoutUtil.o 

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
