-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../../Include

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN 
CFLAGS= $(DEFINES) $(COMPILE_OPT)

LIB_PPS=../../../../../../../ppsvod_for_realtek/
LIBNAME= ../../../../../lib/libPPSIOPlugin.a
EXENAME = main

SRCS = PPSPlugin.c

OBJS = PPSPlugin.o
CFLAGS += -g -O2 
  
#Saturn use hw_fpu
ifneq ($(filter 0x1%6, $(CHIP_MODEL)), $(CHIP_MODEL))
CFLAGS += -msoft-float
endif

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c  $*.c

all: $(LIBNAME) 
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

$(EXENAME) : $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(EXENAME)  $(SRCS) -L../../../../../lib/ -L$(LIB_PPS) -lpthread -lppsbase -lppssg -lppsapi -lppsvodres -lppsfds -lppsvodnet -msoft-float

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~ $(EXENAME)
