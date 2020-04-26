include ../../../../include/MakeConfig

rm			= /bin/rm -f
CC			= $(MYCC)
LINK		= -=LINK=-
DEFS		=
AR			= $(MYAR) cq
RANLIB		= $(MYRANLIB)

INCLUDES	= -I. -I../../../Include -I../../../Include/Filters/NavigationFilter/SiAtsc \
				-I../../../Include/Filters/NavigationFilter/ -I../include

DEFINES		+= $(INCLUDES) $(DEFS) -DSI_USE_FULL_VCT 
#-DBUILD_MIN_SI_SIZE
#-DDVR 
CFLAGS		+= $(DEFINES) $(COMPILE_OPT)
LIBS		=

__NAV_USE_DLL_PLUGIN__ = NO
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
	LIBNAME	= ../../../../lib/SiAtsc.so
	CFLAGS	+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
	LIBNAME	= ../../../../lib/libSiAtsc.a
#	LIBNAME	= libInputTSAtscSi.a
endif

SUBDIRS		=

CFLAGS		+= -Wall

OBJS		= siOs.o siUtil.o siSystem.o siTable.o siEvent.o siSection.o siDescriptor.o siRating.o \
				siChannel.o siCache.o siChannelApi.o siApi.o siTp.o siFilterMgr.o siBean.o siBeanGen.o \
				siEngine.o siDvrTsApi.o siRingBuf.o siMessage.o siConfig.o

# Default target.
all : $(OBJS)
	$(SHOW_COMMAND)$(AR) $(LIBNAME) $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $(LIBNAME)
	
ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(rm) $@
	$(CC) -shared -o $(LIBNAME) $(OBJS) $(LIBS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

.c.o:
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(FLAGS) $(DEBUGFLAG) -c $*.c

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~ include/*~ *.bak
	$(SHOW_COMMAND)set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

# DO NOT DELETE
