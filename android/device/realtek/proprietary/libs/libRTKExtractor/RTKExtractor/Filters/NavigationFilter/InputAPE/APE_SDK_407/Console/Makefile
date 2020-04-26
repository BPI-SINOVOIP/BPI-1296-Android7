##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
#-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
#include ../../../include/MakeConfig

TOOLCHAIN_ROOT = /usr/local/toolchain_mipsel/bin
MYCC = $(TOOLCHAIN_ROOT)/mipsel-linux-gcc
MYCXX = $(TOOLCHAIN_ROOT)/mipsel-linux-g++
MYAR = $(TOOLCHAIN_ROOT)/mipsel-linux-ar
MYRANLIB = $(TOOLCHAIN_ROOT)/mipsel-linux-ranlib
DEFINES+= -mips32r2


rm=/bin/rm -f
CC= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

#INCLUDES= -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system
#INCLUDES= -I./ -I../MACLib -I../Shared/
#LIBS = -L. -L../MACLib -lMACLib -L../Shared -lMACShared 
INCLUDES= -I./ -I../Shared/ -I../MACLib/
LIBS = -L. -L../Shared -L../../../../../../lib -lMACLib -lMACShared 

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
#CFLAGS= $(DEFINES) $(COMPILE_OPT) -g
CPPFLAG  = $(DEFINES)

#Saturn use hw_fpu
ifneq ($(filter 0x1%6, $(CHIP_MODEL)), $(CHIP_MODEL))
CPPFLAG += -msoft-float
endif

CPPFLAG += -static -g

MYPROG = Console

.PHONY  = clean
.PHONY  = link
.PHONY  = RM

all : $(MYPROG)
#   mipsel-linux-strip $(MYPROG)

.c.o:
	$(MYCC) $(CPPFLAG) $^ -c $<

.cpp.o:
	$(MYCC) $(CPPFLAG) $^ -c $<

$(MYPROG): Console.o
	$(MYCXX) $(CPPFLAG) $^ $(LIBS) -o $(MYPROG)

link: RM all
RM :
	rm -rf $(MYPROG)

clean:
	rm -rf *.bak *~ *.o
	rm -rf $(MYPROG)

