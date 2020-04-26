-include ../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../system/include/MakeConfig

MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCC)
CXX = $(MYCXX)
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../lib/libOSAL.a

INCLUDES=  -I. -I../Include -I../Include/OSAL -I../../../common/IPC/include



ifeq ($(HOST), mipsel-linux)
MYDEFS+=-D_GNU_SOURCE -D_MIPSEL_LINUX
endif

ifneq ($(TARGET_BOARD), NO)
MYDEFS+= -DTARGET_BOARD
endif

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)
ifeq ($(DFB_MULTI_APP), YES)
CFLAGS+= -fPIC
endif

ifeq ($(ENABLE_DFB_VKBD), YES)
#for compile dfb_vkd
CFLAGS += -fPIC
endif


SRCS = OSAL.cpp CThread.cpp PLI.cpp Threads.cpp IO.cpp remote_malloc_svc.c remote_malloc_svr.c remote_malloc_xdr.c debug_level_clnt.c debug_level_xdr.c udf_check.c

OBJS = OSAL.o CThread.o PLI.o Threads.o IO.o remote_malloc_svc.o remote_malloc_svr.o remote_malloc_xdr.o debug_level_clnt.o debug_level_xdr.o udf_check.o


.cpp.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.cpp

.c.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo --------- [$*.c] ---------------------------
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.c

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~
miniclean: clean

