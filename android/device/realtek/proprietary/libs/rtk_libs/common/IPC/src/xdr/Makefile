##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Apr 13 17:13:41 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
rm=/bin/rm -f

include ../../../../system/include/MakeConfig

DEFS=  #-DRPC_DEBUG
LIBNAME= ../../lib/librtxdr.a

INCLUDES=  -I. -I../../include

CC= $(MYCC)
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES)

ifeq ($(HOST), mipsel-linux)
DEFINES+= -D_MIPSEL_LINUX
endif

ifeq ($(TARGET_BOARD), YES)
DEFINES+= -DTARGET_BOARD
endif

SRCS = xdr.c xdr_mem.c xdr_array.c xdr_float.c xdr_intXX_t.c xdr_ref.c xdr_sizeof.c

OBJS = xdr.o xdr_mem.o xdr_array.o xdr_float.o xdr_intXX_t.o xdr_ref.o xdr_sizeof.o

.o:
	$(rm) -f $@
	$(CC) $(CFLAGS) -c $*.c

all: $(LIBNAME)

#all: $(OBJS)
#	gcc -shared -o librtxdr.so $(OBJS)

$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) librtxdr.so core *~

