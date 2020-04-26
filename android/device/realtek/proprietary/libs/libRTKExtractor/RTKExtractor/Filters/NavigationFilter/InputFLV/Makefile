-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system -I./libMenuController

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG 
DEFINES+= -DLINUX -DDIVXINT_NATIVE_64BIT -DLARGE_FILE_SUPPORT -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputFLV.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputFLV.a
endif

#DEFINES += -DHACK

CFLAGS += -Wno-multichar 
#temporary solution
ifneq ($(filter 0x1%6, $(CHIP_MODEL)), $(CHIP_MODEL))
CFLAGS += -msoft-float
endif
#CFLAGS += -Wno-multichar

SRCS = InputFLV.c flv_internals.c Security.c Ibcti.cpp

OBJS = InputFLV.o flv_internals.o Security.o Ibcti.o

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.cpp

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) -shared -o $(LIBNAME) $(OBJS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

