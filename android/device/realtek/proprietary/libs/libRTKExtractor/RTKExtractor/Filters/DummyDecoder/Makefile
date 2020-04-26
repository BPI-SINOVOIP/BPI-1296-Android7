##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig


rm=/bin/rm -f
CC= $(MYCXX)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../../lib/libVDecoderDummy.a

INCLUDES+=  -I. -I../../../Include -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system
INCLUDES+= -I../VP8_decoder/inc -I../VP8_decoder/inc/common -I../VP8_decoder/inc/decoder 

#ifneq ($(USE_VIDEO_DECODER_SIMULATOR), NO)
#MYDEFS += -DUSE_VIDEO_DECODER_SIMULATOR
#SUBDIRS = VideoDecoderSimulator
#endif

ifneq ($(TARGET_BOARD), NO)
MYDEFS += -DTARGET_BOARD
endif

SRCS = CDummyDecoder.cpp CDummyInputPin.cpp CVideoFilter.cpp   CMixerInputPin.cpp CVideoOutFilter.cpp CVideoOutInputPin.cpp CVideoSystemRPC.cpp

OBJS = CDummyDecoder.o CDummyInputPin.o CVideoFilter.o    CMixerInputPin.o	CVideoOutFilter.o	CVideoOutInputPin.o CVideoSystemRPC.o 

ifeq ($(ENABLE_SKYPE), YES)
  SRCS += CVideoMixerFilter.cpp
  OBJS += CVideoMixerFilter.o
  MYDEFS += -DSINGLE_MIXER
endif

ifeq ($(ENCODER_COMPONENT), YES)
  SRCS += CVideoMixerFilter.cpp
  OBJS += CVideoMixerFilter.o
endif
CFLAGS= $(INCLUDES) $(DEFINES) -DSYS_UNIX=1 $(MYDEFS)


.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -O  -c $*.cpp

all:  submake $(LIBNAME)

submake: 
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@


clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
