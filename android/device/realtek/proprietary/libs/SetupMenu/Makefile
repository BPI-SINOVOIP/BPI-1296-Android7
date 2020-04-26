-include ../../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig
include ../../../../../include/MakeConfig.internal

MYDEFS=
rm=/bin/rm -f
cp=/bin/cp -f
CC= $(MYCXX)
AR= $(MYAR) cq
RANLIB=$(MYRANLIB)
LIBNAME= ../../../../../lib/libSetupAppClass.a

INCLUDES=  -I../../../../Include/ -I../../../../Include/freetype2/ -I../../../../../../common/IPC/include -I../../../../../../common/IPC/generate/include/system

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)
ifeq ($(ENABLE_DFB_VKBD), YES)
#for compile dfb_vkd
CFLAGS += -fPIC
endif

SRCS = ./SetupClass.cpp
OBJS = ./SetupClass.o

ifneq ($(filter APCFG_USE_SQLITE_SETUP,$(subst |, ,$(APCFG_FLAGS))), )	
  SRCS += ./SetupClassSqlite.cpp
  OBJS += ./SetupClassSqlite.o
else
  SRCS += ./SetupClassBinary.cpp
  OBJS += ./SetupClassBinary.o
  ifeq ($(BUILD_COLUMBUS_SETUPCLASS), Y)
    SRCS += ./SetupClassAp.cpp
    OBJS += ./SetupClassAp.o
  endif
endif

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -c $*.cpp

#all : GEN_SETUP_ENUM $(LIBNAME)    ##### this is for auto generate setup_enum.h
all : $(LIBNAME)

GEN_SETUP_ENUM:
ifneq ($(filter APCFG_USE_SQLITE_SETUP,$(subst |, ,$(APCFG_FLAGS))), )	
	@./gen_setup_enum.sh ../DataObject/Setup.png >./setup_enum.h
else
	@echo
endif

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
