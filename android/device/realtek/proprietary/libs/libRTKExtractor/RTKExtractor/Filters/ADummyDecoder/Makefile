##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Oct 28 13:37:18 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig
rm=/bin/rm -f
cp=/bin/cp -f
mkdir=/bin/mkdir -p

CC= $(MYCXX) -m32
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libADecoderDummy.a
INSTALLHEADER= ../../Include/AudioFilter
include MakeConfig

INCLUDES=  -I. -I../../ -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system

DEFINES += $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS=  $(DEFINES) $(COMPILE_OPT)


SRCS = CAudioDecoder.cpp CAudioFilter.cpp \
	CAudioPin.cpp CMultiAllocator.cpp CMultiPin.cpp \
	CBinaryFileReader.cpp CAudioQueue.cpp \
	CAudioDecPin.cpp

OBJS = CAudioDecoder.o CAudioFilter.o CAudioPin.o  \
	CMultiAllocator.o CMultiPin.o CAudioQueue.o \
	CAudioDecPin.o

#CBinaryFileReader.o
HEADERS= *.h

%.o: %.cpp
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME) smwlib

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

smwlib :
	cd rpc; make

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	cd rpc; make clean
