##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../../Include -L/usr/local/lib 

DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)
CFLAGS+= -g
LIBNAME= ../../../../../lib/libohci1394.a

VPATH = ./avc1394 ./common ./rom1394 ./raw1394

SRCS = ohci_read.cpp avc1394_internal.c avc1394_simple.c avc1394_vcr.c raw1394util.c arm.c errors.c eventloop.c fcp.c iso.c main.c readwrite.c rom1394_internal.c rom1394_main.c
OBJS = ohci_read.o avc1394_internal.o avc1394_simple.o avc1394_vcr.o raw1394util.o arm.o errors.o eventloop.o fcp.o iso.o main.o readwrite.o rom1394_internal.o rom1394_main.o


HEADERS=
%.o: %.c
	$(rm) -f $@
	$(CC) $(CFLAGS) $(DEBUGFLAG) -c $<

%.o: %.cpp
	$(rm) -f $@
	$(CC) $(CFLAGS) $(DEBUGFLAG) -c $<

all: $(LIBNAME)
$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~

