-include ../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../include/MakeConfig

rm=/bin/rm -f
CC= $(MYCXX)
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../lib/libstream.a
LIBNAME_ION= ../../lib/libstream_ion.a

INCLUDES=  -I. -I../Include



ifneq ($(USE_VIDEO_DECODER_SIMULATOR), NO)
MYDEFS+= -DUSE_VIDEO_DECODER_SIMULATOR
endif

ifneq ($(USE_SMOOTH_REVERSE), NO)
MYDEFS+= -DUSE_SMOOTH_REVERSE
endif



SRCS = CMediaType.cpp CPin.cpp CFilter.cpp CMediaSample.cpp CMemListAllocator.cpp CMemRingAllocator.cpp CReadPointerHandle.cpp CEnumPins.cpp CFlowManager.cpp CEnumFilters.cpp CReferenceClock.cpp

OBJS = CMediaType.o CPin.o CFilter.o CMediaSample.o CMemListAllocator.o CMemRingAllocator.o CReadPointerHandle.o CEnumPins.o CFlowManager.o CEnumFilters.o CReferenceClock.o
OBJS_ION = $(patsubst %.o,%_ion.o,$(OBJS))

#USE_IOCACHE = YES
#USE_IOCACHE = NO
ifeq ($(USE_IOCACHE), YES)
MYDEFS+= -DUSE_IOCACHE=1
SRCS += IOCache_MS.cpp
OBJS += IOCache_MS.o
endif

CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)

all: $(LIBNAME) $(LIBNAME_ION)

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$^.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -o $@ -c $^

$(LIBNAME) : $(OBJS) 
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

$(OBJS_ION): %_ion.o: %.cpp
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$^.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -DUSE_ION -o $@ -c $^

$(LIBNAME_ION) : $(OBJS_ION) 
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS_ION)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) $(OBJS_ION) $(LIBNAME_ION) core *~
miniclean: clean

