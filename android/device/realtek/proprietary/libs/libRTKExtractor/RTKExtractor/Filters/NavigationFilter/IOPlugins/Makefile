-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig

SUBDIRS = FilePlugin TranscodePlugin HttpPlugin Custom1 Custom2 Custom3 FileLinkPlugin

ifeq ($(ENABLE_BUFFER_IOPLUGIN), YES)
SUBDIRS += BufferPlugin
endif

ifeq ($(USE_DIRECT_CONNECT), YES)
SUBDIRS += DirectConnectPlugin
endif

ifeq ($(USE_UDP_IO_PLUGIN), YES)
  SUBDIRS += UdpPlugin
endif

ifeq ($(USE_STREAM_PPS_IOPLUGIN), YES)
SUBDIRS += PPSPlugin
endif

ifeq ($(ENABLE_PPTV), YES)
SUBDIRS += PPTVPlugin
endif

ifeq ($(ENABLE_BESTV), YES)
SUBDIRS += BesTVPlugin
endif
ifeq ($(ENABLE_NETTV), YES)
SUBDIRS += OMADRMPlugin
endif
ifeq ($(ENABLE_VOOLETV), YES)
SUBDIRS += VooleTVPlugin
endif

ifeq ($(USE_STREAM_RTSP_IOPLUGIN), YES)
  ifeq ($(USE_LIVE_LIBRARY), YES)
    SUBDIRS += LivePlugin
  else
    SUBDIRS += RtspPlugin
  endif
endif

ifeq ($(USE_STREAM_RTP_IOPLUGIN), YES)
SUBDIRS += RtpPlugin
endif

ifeq ($(USE_STREAM_MMS_IOPLUGIN), YES)
SUBDIRS += MmsPlugin
endif

ifeq ($(ENABLE_THUNDER), YES)
SUBDIRS += ThunderRemotePlugin
SUBDIRS += ThunderPlugin
endif

ifeq ($(ENABLE_NRD2X), YES)
SUBDIRS += NRDPlugin
endif


ifeq ($(ENABLE_IPCSTREAMING), YES)
SUBDIRS += IPCStreamingPlugin
endif

ifeq ($(USE_CAMERA), YES)
SUBDIRS += CameraPlugin
endif

ifeq ($(ENABLE_SOCKET_IO_PLUGIN), YES)
SUBDIRS += SocketPlugin
endif

ifeq ($(ENABLE_MIRACAST_IO_PLUGIN), YES)
SUBDIRS += MiracastPlugin
endif

ifeq ($(ENABLE_SNDA), YES)
SUBDIRS += SndaPlugin
endif

ifeq ($(ENABLE_SERVER_IOPLUGIN), YES)
SUBDIRS += ServerPlugin
endif

all: 
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done 
clean: 
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
