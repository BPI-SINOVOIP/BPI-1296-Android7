##############################################
# Tool Chain
##############################################
TOOLCHAIN_PATH = ~/toolchain/asdk-4.8.1-a7-EL-3.10-0.9.33-a32nt-130828/

CROSS_COMPILER = arm-linux
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILER)-gcc
AR= $(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILER)-ar cr
RANLIB=$(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILER)-ranlib
STRIP=$(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILER)-strip

LIBNAME= ../lib/libion.a

OBJS = ionapi.o

CFLAGS += $(DEFINES) $(MYDEFS)
CFLAGS += -I.

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
#	make -C widget
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

.c.o:
	$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$<] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $<

clean:
	$(rm) $(OBJS) $(LIBNAME) core *~

