##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Oct 28 13:37:18 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
rm=/bin/rm -f
cp=/bin/cp -f
mkdir=/bin/mkdir -p

include ../../../../../system/include/MakeConfig

DEFS=
CC= $(MYCC)
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libsystemrpc.a
#INSTALLHEADER= ../../../../Include/AudioFilter
#include ../../../../../include/MakeConfig

INCLUDES=  -I. -I../../../include -I../../include/system

ifeq ($(ENABLE_USE_BIONIC),YES)
DEFINES += $(INCLUDES) $(DEFS)
else
DEFINES= $(INCLUDES) $(DEFS)
endif
CFLAGS= -g $(DEFINES) $(COMPILE_OPT)

ifeq ($(RTD_1195),YES)
CFLAGS+= -DRTD_1195
endif
ifeq ($(RPC_PROGRAM_REGISTER),YES)
CFLAGS+= -DRPC_PROGRAM_REGISTER
endif

SRCS = AudioRPC_Agent_svc.c AudioRPC_System_clnt.c AudioRPCBaseDS_xdr.c AudioRPC_Agent_xdr.c AudioRPC_System_xdr.c RPCBaseDS_xdr.c
OBJS = AudioRPC_Agent_svc.o AudioRPC_System_clnt.o AudioRPCBaseDS_xdr.o AudioRPC_Agent_xdr.o AudioRPC_System_xdr.o RPCBaseDS_xdr.o

HEADERS= defs.h

%.o : %.c
	$(rm) -f $@
	$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.c

all: $(LIBNAME)
	$(MAKE) -f ./Makefile-video

$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@
clean:
	$(rm) $(OBJS) $(LIBNAME) core *~
	$(MAKE) -f ./Makefile-video clean
