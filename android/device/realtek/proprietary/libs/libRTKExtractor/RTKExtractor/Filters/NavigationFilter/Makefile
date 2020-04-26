##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Thu Sep  2 15:30:38 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= ../../../lib/libNavigationFilter.a


# modified by Richard for TS demuxer testing
ifeq ($(TEST_TS), YES)
	SUBDIRS = DemuxMPEGProgram DemuxPassthrough DemuxMPEGTransport InputDVDVideo InputVCD InputNetwork InputDTV
	SUBDIRS += InputBD
	
	ifeq ($(BUILD_ATSC_T), Y)
	SUBDIRS += SiAtsc
	endif
	
	ifeq ($(BUILD_DVB_T), Y)
	SUBDIRS += SiDvb
	endif
	
else
SUBDIRS= DemuxMPEGProgram DemuxPassthrough InputDVDVideo InputVCD InputNetwork 
ohci_1394_device =YES
endif

ifneq ($(findstring APCFG_HW_1394,$(APCFG_FLAGS)),)
SUBDIRS+= DemuxDV InputDV 
ifeq ($(ohci_1394_device), YES)
  DEFINES += -Dohci_1394_device
  LIBS+= ohci1394
endif
endif

ifeq ($(filter APCFG_PRJ_PHOTO_VIEWER,$(subst |, ,$(APCFG_FLAGS))),)
  SUBDIRS += InputDVDVR
endif


# by stream
SUBDIRS += IOPlugins
ifeq ($(INPUT_USE_IO_PLUGIN), YES)

ifeq ($(ENABLE_WAVE_INPUT_PLUGIN), YES)
SUBDIRS += InputWAVE
endif

ifeq ($(ENABLE_RM_INPUT_PLUGIN), YES)
SUBDIRS += InputRM
DEFINES += -DENABLE_RM_INPUT_PLUGIN
endif

ifeq ($(ENABLE_FLASH_INPUT_PLUGIN), YES)
SUBDIRS += InputFlashES

endif

ifeq ($(ENABLE_ASF_INPUT_PLUGIN), YES)
SUBDIRS += InputASF
DEFINES += -DENABLE_ASF_INPUT_PLUGIN
endif
ifeq ($(ENABLE_MP4_INPUT_PLUGIN), YES)
SUBDIRS += InputMP4
DEFINES += -DENABLE_MP4_INPUT_PLUGIN
endif
ifeq ($(ENABLE_OGG_INPUT_PLUGIN), YES)
SUBDIRS += InputOGG
DEFINES += -DENABLE_OGG_INPUT_PLUGIN
endif
ifeq ($(USE_STREAM_RTSP_IOPLUGIN), YES)
SUBDIRS += InputRTP
endif
ifeq ($(ENABLE_SNDAOGG_INPUT_PLUGIN),YES)
SUBDIRS += InputSndaOGG
DEFINES += -DENABLE_SNDAOGG_INPUT_PLUGIN
endif
ifeq ($(ENABLE_HDMV_PLAYBACK), YES)
SUBDIRS += InputHDMV
endif


ifeq ($(ENABLE_NRD2X), YES)
SUBDIRS += InputNRD
DEFINES += -DENABLE_NRD2XX_INPUT_PLUGIN -DNRD_2XX_IOPLUGIN
endif

ifeq ($(ENABLE_NRD3_INPUT_PLUGIN), YES)
SUBDIRS += InputNRD3
endif

ifeq ($(ENABLE_SOCKET_INPUT_PLUGIN), YES)
SUBDIRS += InputSocket
endif
ifeq ($(ENABLE_NETTV), YES)
DEFINES += -DENABLE_NETTV
endif
SUBDIRS +=  InputPlaylist InputVideoES InputAudioES InputMPEGFile InputAVI InputMKV InputFLV InputAIFF InputLPCM

ifeq ($(ENABLE_AUDIO_APE), YES)
SUBDIRS +=  InputAPE_ES
else
SUBDIRS +=  InputAPE
endif


DEFINES += -DINPUT_USE_IO_PLUGIN

  ifeq ($(DIVX_PROFILE), DIVX_PLUS)
    SUBDIRS += libDRM6.1
  endif

else
SUBDIRS += InputFile
endif

ifeq ($(ENABLE_NEPTUNE_TP_PLUGIN), YES)
SUBDIRS += InputTP
DEFINES += -DENABLE_NEPTUNE_TP_PLUGIN
endif

ifeq ($(USE_AVSYNC_FREE_RUN), YES)
DEFINES += -DUSE_AVSYNC_FREE_RUN
endif

SUBDIRS += CommonUtility

ifeq ($(ENABLE_CONFERENCE), YES)
SUBDIRS += InputAV
endif
ifeq ($(USE_CAMERA), YES)
SUBDIRS += InputAV
endif

ifeq ($(ENABLE_PPTV), YES)
SUBDIRS += InputPPTV
endif

INCLUDES= -I../../Include -I../../../../common/IPC/include -I../../../../common/IPC/generate/include/system

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)

ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
CFLAGS+= -D__NAV_USE_DLL_PLUGIN__
endif

CFLAGS+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)
CFLAGS+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

SRCS = CNavigationFilter.cpp NavPluginFactory.cpp NavDebug.cpp

OBJS = CNavigationFilter.o NavPluginFactory.o NavDebug.o


.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.cpp

ifeq ($(DIVX_PROFILE), DIVX_PLUS)
DIVX_SRC_PATH=http://172.21.0.100/svn/col/DVR/venus/software/system/third_party/src/divx/libDRM6.1/
all: $(LIBNAME)
	svn co $(DIVX_SRC_PATH) ./libDRM6.1
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
else
all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
endif

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
