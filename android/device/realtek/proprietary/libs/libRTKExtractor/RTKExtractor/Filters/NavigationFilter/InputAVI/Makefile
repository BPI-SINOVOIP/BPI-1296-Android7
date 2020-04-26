-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG 
DEFINES+= -DLINUX -DDIVXINT_NATIVE_64BIT -DLARGE_FILE_SUPPORT -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS+= $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputAVI.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../lib/libInputAVI.a
endif

ifeq ($(DIVX_PROFILE), HOME_THEATER)
  DEFINES += -DDIVX_HOME_THEATER
  INCLUDES += -IADP-3.0.2 -IADP-3.0.2/common
  SUBDIRS = ADP-3.0.2
else
  ifeq ($(DIVX_PROFILE), DIVX_ULTRA)
  DEFINES += -DDIVX_ULTRA
  INCLUDES += -IADP-3.0.2 -IADP-3.0.2/common -IlibMenuController
  SUBDIRS = ADP-3.0.2 libMenuController
  else
    ifeq ($(DIVX_PROFILE), DIVX_HD)
      DEFINES += -DDIVX_HD
      INCLUDES += -IlibDrmDevice -IlibDrmDevice/common
      SUBDIRS = libDrmDevice libMenuController
    else
      ifeq ($(DIVX_PROFILE), DIVX_HD_51)
        DEFINES += -DDIVX_HD_51
        INCLUDES += -IlibDrmDevice51 -IlibDrmDevice51/common
        SUBDIRS = libDrmDevice51 libMenuController
      else
        ifeq ($(DIVX_PROFILE), DIVX_PLUS)
          DEFINES += -DDIVX_PLUS
          INCLUDES += -I../libDRM6.1/Common -I../libDRM6.1/ComponentCryptoLayer -I../libDRM6.1/DRMCommon
        endif
      endif
    endif
  endif
endif

CFLAGS += -Wno-multichar

SRCS = InputAVI.c AVI2.c OggDecode.c

OBJS = InputAVI.o AVI2.o OggDecode.o

WARNING += -Wno-multichar

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c  $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

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

