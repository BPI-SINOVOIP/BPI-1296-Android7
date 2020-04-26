
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I../../../Include -I../include

DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS+= $(DEFINES) $(COMPILE_OPT)
LIBS=

__NAV_USE_DLL_PLUGIN__=NO
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputSTB.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
LIBS+=-L../../../../lib -lDVDCommon -lebase -ldvdaccess -lOSAL -ludfaccess -lCSS
else
LIBNAME= ../../../../lib/libInputSTB.a
endif

CFLAGS+= -Wall

SRCS = dma1394.c input_stb.c stb_handle.c stb_vm.c tsp_program.c tsp_psi.c

OBJS = dma1394.o input_stb.o stb_handle.o stb_vm.o tsp_program.o tsp_psi.o


# .cpp.o:
#	$(rm) -f $@
#	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
#	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) -shared -o $(LIBNAME) $(OBJS) $(LIBS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

$(OBJS): $(SRCS)
	$(SHOW_COMMAND)$(CC) -c $(CFLAGS) $(FLAGS) $(DEBUGFLAG) $(SRCS)

depend:
	makedepend $(SRCS) *.h

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
