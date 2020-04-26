##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Jun 29 13:52:53 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCXX)
DEFS=  
PROGNAME= player
INCLUDES=-I. -I../../inc


DEFINES += $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -fshort-wchar -D_ANSI -DTARGET=10 
CFLAGS = $(DEFINES) 
ifeq ($(TARGET_BOARD), YES)
CFLAGS += -static
endif

include ../../../makefile.inc.2


LIBS += -Wl,--start-group
LIBS += -L. -L../../../../../../../../lib -lm -lpthread -lrt -ldl

ADP_LIB_ROOT = ../../../../../../../../lib/
LIBS += $(ADP_LIB_ROOT)libWMDRMPD.a

#LIBS+= -L. -lefence_344
LIBS += -Wl,--end-group

SRCS = player.c 


OBJS = player.o 

.c.o:
	$(rm) $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(WARNING) $(DEBUGFLAG) -c $*.c

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(LDFLAGS) $(OBJS) $(LIBS)


clean:
	$(rm) $(OBJS) $(PROGNAME) core* *~

