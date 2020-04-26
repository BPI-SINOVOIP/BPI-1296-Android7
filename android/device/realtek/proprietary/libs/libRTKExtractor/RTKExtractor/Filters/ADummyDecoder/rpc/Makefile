##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Oct 28 13:37:18 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
cp=/bin/cp -f
mkdir=/bin/mkdir -p

CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
SMWLIBNAME= ../../../../lib/libsmw.a
INSTALLHEADER= ../../Include/AudioFilter
include ../MakeConfig

INCLUDES=  -I. -I../../../ -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES += $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= -g $(DEFINES) $(COMPILE_OPT)

SMWSRCS = smw_instmap.cpp smw_rpc.cpp

SMWOBJS = smw_instmap.o smw_rpc.o

HEADERS= *.h

%.o: %.cpp
	$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.cpp

all: $(SMWLIBNAME)


$(SMWLIBNAME) : $(SMWOBJS)
	$(rm) $@
	$(AR) $@ $(SMWOBJS)
	$(RANLIB) $@	
	
clean:
	$(rm) $(SMWOBJS) $(SMWLIBNAME) core *~
