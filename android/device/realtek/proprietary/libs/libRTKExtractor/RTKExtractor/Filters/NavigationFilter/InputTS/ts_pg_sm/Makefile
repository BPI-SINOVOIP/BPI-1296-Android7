
-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../../Include -I../../include -I../include

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS+= $(DEFINES) $(COMPILE_OPT)
LIBS=

__NAV_USE_DLL_PLUGIN__=NO
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../../lib/ts_pg_sm.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
# LIBS+=-L../../../../lib -lDVDCommon -lebase -ldvdaccess -lOSAL -ludfaccess -lCSS
else
LIBNAME= ../../../../../lib/libts_pg_sm.a
endif

CFLAGS+= -Wall

SRCS = ts_pg_sm.c avl.c

OBJS = ts_pg_sm.o avl.o

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
