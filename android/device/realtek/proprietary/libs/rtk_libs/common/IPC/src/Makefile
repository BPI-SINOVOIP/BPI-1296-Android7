##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Apr 13 17:13:41 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
rm=/bin/rm -f

include ../../../system/include/MakeConfig

DEFS=  #-DRPC_DEBUG

CC= $(MYCC)
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../lib/libRPC.a

INCLUDES=  -I. -I../include/

#DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -D_GNU_SOURCE
CFLAGS= $(INCLUDES) $(DEFINES) $(DEFS) -DSYS_UNIX=1 -D_GNU_SOURCE -Wall -g

SUBDIRS = xdr

ifeq ($(TARGET_BOARD), YES)
CFLAGS+= -DTARGET_BOARD
endif


SRCS = RPCstruct.c rpcapi.c RPCProxy.c SVCFunc.c rpcstubapi.c ClientStub.c

OBJS = RPCstruct.o rpcapi.o RPCProxy.o SVCFunc.o rpcstubapi.o ClientStub.o

.o:
	$(rm) -f $@
	$(CC) $(CFLAGS) -c $*.c

all: $(LIBNAME) 
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
#	$(CC) UserTask.c -L./ -lcln -L./xdr/ -lrtxdr

#all: $(OBJS)
#	gcc -shared -o libcli.so $(OBJS)

$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS) 
	$(RANLIB) $@



clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done; rm -rf ../lib/*
	$(rm) $(OBJS) $(LIBNAME) libcln.so a.out core.* *~


