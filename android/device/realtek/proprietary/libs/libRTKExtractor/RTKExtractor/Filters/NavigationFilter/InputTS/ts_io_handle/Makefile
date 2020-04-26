
-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../../Include -I../../include -I../include

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS+= $(DEFINES) $(COMPILE_OPT)
LIBS=

__NAV_USE_DLL_PLUGIN__=NO
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../../lib/ts_io_handle.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
# LIBS+=-L../../../../lib -lDVDCommon -lebase -ldvdaccess -lOSAL -ludfaccess -lCSS
else
LIBNAME= ../../../../../lib/libts_io_handle.a
endif

CFLAGS+= -Wall

SRCS = ts_io_handle.c ts_io_tuner.c ts_io_file.c ts_util_ring.c

OBJS = ts_io_handle.o ts_io_tuner.o ts_io_file.o ts_util_ring.o

# .cpp.o:
#	$(rm) -f $@
#	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
#	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(CC) -shared -o $(LIBNAME) $(OBJS) $(LIBS)
else
$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@
endif

$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $(FLAGS) $(DEBUGFLAG) $(SRCS)

depend:
	makedepend $(SRCS) $(INCLUDES) *.h

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~ *.bak
# DO NOT DELETE
