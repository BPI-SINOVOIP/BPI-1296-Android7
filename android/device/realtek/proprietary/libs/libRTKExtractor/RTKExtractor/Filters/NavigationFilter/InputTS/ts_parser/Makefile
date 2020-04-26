
-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../../Include -I../../include -I../include

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS+= $(DEFINES) $(COMPILE_OPT)
LIBS=

__NAV_USE_DLL_PLUGIN__=NO
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../../lib/ts_parser.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
# LIBS+=-L../../../../lib -lDVDCommon -lebase -ldvdaccess -lOSAL -ludfaccess -lCSS
else
LIBNAME= ../../../../../lib/libts_parser.a
endif

CFLAGS+= -Wall

SRCS = tsp.c tsp_crc32.c

OBJS = tsp.o tsp_crc32.o

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
	makedepend $(SRCS) $(INCLUDES)

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~ *.bak
# DO NOT DELETE
