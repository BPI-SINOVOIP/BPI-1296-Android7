-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)


ORIGINAL_MAKEFILE = makefile.inc
FEATURE_MAKEFILE = makefile.inc.2

INCLUDES= -I. -I../../../../Include -I../../../../Include/Filters

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DINPUT_USE_IO_PLUGIN
ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

CFLAGS= -fshort-wchar $(DEFINES) $(COMPILE_OPT)
CXXFLAGS +=  $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../../lib/libWMDRMPD.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__
else
LIBNAME= ../../../../../lib/libWMDRMPD.a
endif

include $(FEATURE_MAKEFILE)
include $(ORIGINAL_MAKEFILE)

SRCS = $(BNG_SRCS) $(PKC_SRCS) $(DRC_SRCS) $(COR_SRCS) $(BBX_SRCS) 	\
	$(CMD_SRCS) $(MGR_SRCS) $(DDC_SRCS) $(DCP_SRCS) $(MTR_SRCS) 	\
	$(SCK_SRCS) $(SYN_SRCS) $(WMC_SRCS) $(TCM_SRCS) $(OEM_SRCS)

OBJS = $(BNG_OBJS) $(PKC_OBJS) $(DRC_OBJS) $(COR_OBJS) $(BBX_OBJS) 	\
	$(CMD_OBJS) $(MGR_OBJS) $(DDC_OBJS) $(DCP_OBJS) $(MTR_OBJS) 	\
	$(SCK_OBJS) $(SYN_OBJS) $(WMC_OBJS) $(TCM_OBJS) $(OEM_OBJS)

ifeq ($(DRM_SUPPORT_TOOLCOMMON), 1)
SRCS += $(TCM_SCRS)
OBJS += $(TCM_OBJS)
endif

.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c $*.c -o $@

#PREBUILT_LIB = YES
ifeq ($(PREBUILT_LIB), YES)
all: $(LIBNAME)
	cp $(LIBNAME) ../../../../../lib_release/prebuilt_$(TOOLCHAIN_VER)/
else
all:
	echo "skip to build "$(LIBNAME)
endif

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
