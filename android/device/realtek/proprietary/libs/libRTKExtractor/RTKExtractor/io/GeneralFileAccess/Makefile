-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig

MYDEFS=
rm=/bin/rm -f
CC= $(MYCC)
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libfileaccess.a

INCLUDES=  -I. -I../../Include -I../../Include/io

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)

SRCS = file_db.c file_util.c gfile_common.c array_funcs.c list_funcs.c simple_funcs.c fastcopy_wrapper.c

OBJS = file_db.o file_util.o gfile_common.o array_funcs.o list_funcs.o simple_funcs.o fastcopy_wrapper.o 

.c.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo --------- [$*.c] ---------------------------
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.c

.cpp.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(MYCXX) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

all: $(LIBNAME) 


$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS) 
	$(RANLIB) $@

clean:
	$(rm) $(OBJS) $(LIBNAME) core.* *~
