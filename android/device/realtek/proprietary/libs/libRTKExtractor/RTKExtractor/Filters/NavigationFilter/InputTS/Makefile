-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../Include -I../include -I./include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS+= $(DEFINES) $(COMPILE_OPT)
LIBS=

__NAV_USE_DLL_PLUGIN__=NO
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputTS.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
# LIBS+=-L../../../../lib -lDVDCommon -lebase -ldvdaccess -lOSAL -ludfaccess -lCSS
else
LIBNAME= ../../../../lib/libInputTS.a
endif

SUBDIRS=  ts_io_handle ts_parser ts_pg_sm ts_ap_if

CFLAGS+= -Wall

SRCS = input_ts.c ts_si.c ts_stream.c ts_record.c ts_time.c ts_db.c ts_media.c ts_channel.c ts_speed.c

OBJS = input_ts.o ts_si.o ts_stream.o ts_record.o ts_time.o ts_db.o ts_media.o ts_channel.o ts_speed.o

# .cpp.o:
#	$(rm) -f $@
#	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
#	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done


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
	makedepend $(SRCS) $(INCLUDES)
	set -e; for i in $(SUBDIRS); do $(MAKE) depend -C $$i; done

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~ include/*~ *.bak
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

# DO NOT DELETE
