##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../include/MakeConfig
rm=/bin/rm -f
CC= $(MYCC) 
CXX= $(MYCXX) 

LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)

INCLUDES= -I. -I../../../Include  -I../../../../../common/IPC/include -I../../../../../common/IPC/generate/include/system
INCLUDES+= -I../../../Include/directfb
INCLUDES+= -I/usr/local/toolchain_mipsel/mipsel-linux-uclibc/sys-include/asm/mach-venus

INCLUDES+= -I../../../Include/freetype2

INCLUDES+= -I./Common/Include -I./Common/SystemHeaders -I./Common/StringUtils -I./Common/Threads -I./Common/SysUtils -I./Common/BitPacker -I./Common/FileSystem -I./Common/Geometry  -I./Common/StreamUtils
INCLUDES+= -I./Common/SurfaceUtils/Common -I./Common/SurfaceUtils/Surface -I./Common/SurfaceUtils/Render
#INCLUDES+=         -I./Common/Cache 
INCLUDES+= -I./Formats/Caches -I./Formats/BitPackers -I./Formats/BD_FS_Layer
INCLUDES+= -I./Players/PrecisionClock
INCLUDES+= -I./Players/Renderer -I./Players/BD
#INCLUDES+= -I./AS_Imaging/inc
INCLUDES+= -I./Players/HDMVDecoders

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 -DHAVE_STRING_H -DRDDEBUG -DHAVE_SYS_TYPES_H

DEFINES+= -DERROR_CONCEALMENT_LEVEL=$(ERROR_CONCEALMENT_LEVEL)
DEFINES+= -DRECOVERY_JUMP_LEVEL=$(RECOVERY_JUMP_LEVEL)

DEFINES+= -DREALTEK_MODIFY
DEFINES+= -DREALTEK_MODIFY_DISABLE_PLAYBACKINFO_CALLBACK
DEFINES+= -DREALTEK_MODIFY_DISABLE_SONICTHREAD
DEFINES+= -D_DIRECTFB
DEFINES+= -DREALTEK_MODIFY_IG
DEFINES+= -DREALTEK_MODIFY_TEXTST
#DEFINES+= -DREALTEK_MODIFY_DISABLE_CACHE_OBJECT
DEFINES+= -DREALTEK_MODIFY_EVENT
DEFINES+= -DREALTEK_MODIFY_USE_DIRECT_IO -DREALTEK_MODIFY_BUG_25303
DEFINES+= -DHDMV_DIRECT_IO_BLOCK_SIZE=4096
DEFINES+= -DREALTEK_MODIFY_BUG_22229
DEFINES+= -DREALTEK_MODIFY_INDEX_SURFACE
DEFINES+= -DREALTEK_MODIFY_22235
DEFINES+= -DREALTEK_MODIFY_BUG_22686
DEFINES+= -DREALTEK_MODIFY_REPEAT_TITLE
DEFINES+= -DREALTEK_MODIFY_BUG_23034

#disable partial update because of bug 28083
#object of normal state is null. we have no way to clear highlight
#DEFINES+= -DREALTEK_MODIFY_IG_PARTIAL_UPDATE

DEFINES+= -DREALTEK_MODIFY_DISABLE_PIP
DEFINES+= -DREALTEK_MODIFY_INDEX_DISPLAY_FRAME
DEFINES+= -DREALTEK_MODIFY_BUG_23195
DEFINES+= -DREALTEK_MODIFY_BUG23636_SKIP_UNKNOWN_SETSYSTEMOPTION
DEFINES+= -DREALTEK_MODIFY_BUG_23714
DEFINES+= -DREALTEK_MODIFY_BUG_23399
DEFINES+= -DREALTEK_MODIFY_RESUMEPLAY -DREALTEK_MODIFY_BUG_30406 -DREALTEK_MODIFY_BUG_28722
DEFINES+= -DREALTEK_MODIFY_BUG_24773
DEFINES+= -DREALTEK_MODIFY_BUG_25435
DEFINES+= -DREALTEK_MODIFY_BUG_25301
DEFINES+= -DREALTEK_MODIFY_BUG_25925
DEFINES+= -DREALTEK_MODIFY_BUG_25883
DEFINES+= -DREALTEK_MODIFY_ONLYONESEG #BUG26644
DEFINES+= -DREALTEK_MODIFY_BUG_26672
DEFINES+= -DREALTEK_MODIFY_BUG_26796
DEFINES+= -DREALTEK_MODIFY_BUG_26993
DEFINES+= -DREALTEK_MODIFY_BUG_26983
DEFINES+= -DREALTEK_MODIFY_BUG_27132
DEFINES+= -DREALTEK_MODIFY_BUG_29218_29802
DEFINES+= -DREALTEK_MODIFY_BUG_30241
DEFINES+= -DREALTEK_MODIFY_BUG_31050
DEFINES+= -DREALTEK_MODIFY_BUG_32509
DEFINES+= -DREALTEK_MODIFY_BUG_33171

ifneq ($(TARGET_BOARD), NO)
DEFINES += -DTARGET_BOARD
endif

ifeq ($(BUILD_CMDLINE_DVDPLAYER), YES)
ifeq ($(ENABLE_ANDROID), YES)
  MYDEFS+= -DTEST_ANDROID
endif
endif

CFLAGS= $(DEFINES) $(COMPILE_OPT) $(MYDEFS)


ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
LIBNAME= ../../../../lib/InputHDMV.so
CFLAGS+= -fPIC -D__NAV_USE_DLL_PLUGIN__ 
else
LIBNAME= ../../../../lib/libInputHDMV.a
endif


OBJS = 
OBJS += Players/BD/BDROM_TimerManager.o  
OBJS += Players/BD/ModuleManager.o Players/BD/HDMV_Module.o
OBJS += Players/BD/BDROM_ElapsedTitleTimer.o
OBJS += Players/BD/BDROM_NavigationTimer.o
OBJS += Players/BD/BDROM_Navigator.o
OBJS += Players/BD/BDROM_NavInfoUtils.o
#OBJS += Players/BD/BDROM_PlaybackInfo.o
OBJS += Players/BD/BDROM_PlayList_PipMetaData.o
#OBJS += Players/BD/BDROM_SecondaryVideoPositionTimer.o
OBJS += Players/BD/BDROM_StreamControl.o
OBJS += Players/BD/PTSCallbackHandler.o
OBJS += Players/BD/BDROM_TimedStillTimer.o
OBJS += Players/BD/BDROM_Timer.o
OBJS += Players/BD/NotificationEventManager.o
OBJS += Players/BD/SonicPBCE.o
OBJS += Players/BD/SonicPBCE_Clips.o
OBJS += Players/BD/SonicPBCE_Paths.o
OBJS += Players/BD/SonicPBCE_Registers.o
OBJS += Players/BD/UserEventManager.o
OBJS += Players/BD/SonicPBCE_PlayListManager.o
OBJS += Players/BD/BDROM_SoundPlayer.o


OBJS += Players/HDMVDecoders/DisplayLayerClientImpl.o
OBJS += Players/HDMVDecoders/HDMVDecoder_CodedDataBuffer.o
OBJS += Players/HDMVDecoders/HDMVDecoder_CompositionBuffer.o
OBJS += Players/HDMVDecoders/HDMVDecoder_DecodedDataBuffer.o
OBJS += Players/HDMVDecoders/HDMVDecoder_EB.o
OBJS += Players/HDMVDecoders/HDMVDecoder_GraphicsController.o
OBJS += Players/HDMVDecoders/HDMVDecoder_StreamGraphicsProc.o
OBJS += Players/HDMVDecoders/HDMVDecoder_Types.o
OBJS += Players/HDMVDecoders/IGDecoder.o
OBJS += Players/HDMVDecoders/IGDecoder_Buffers.o
OBJS += Players/HDMVDecoders/IGDecoder_GraphicsController.o
OBJS += Players/HDMVDecoders/IGDecoder_StreamGraphicsProc.o
OBJS += Players/HDMVDecoders/WinHDMVSysObject.o
OBJS += Players/HDMVDecoders/WinHDMVDecoder_DisplayFrameUtils.o
OBJS += Players/HDMVDecoders/WinIGDecoder.o
OBJS += Players/HDMVDecoders/WinIGDecoder_Buffers.o
OBJS += Players/HDMVDecoders/WinIGDecoder_GraphicsController.o
OBJS += Players/HDMVDecoders/WinHDMVDecoderFactory.o
OBJS += Players/HDMVDecoders/WinPGDecoder.o

OBJS += Players/HDMVDecoders/TextSTDecoder.o
OBJS += Players/HDMVDecoders/TextSTDecoder_Buffers.o
OBJS += Players/HDMVDecoders/TextSTDecoder_GraphicsController.o
OBJS += Players/HDMVDecoders/TextSTDecoder_StreamGraphicsProc.o
OBJS += Players/HDMVDecoders/WinTextSTDecoder.o
OBJS += Players/HDMVDecoders/WinTextSTDecoder_Buffers.o
OBJS += Players/HDMVDecoders/WinTextSTDecoder_GraphicsController.o


OBJS += Players/PrecisionClock/BasePrecisionClock.o
OBJS += Players/PrecisionClock/SysPrecisionClock.o

OBJS += Formats/BD_FS_Layer/BD_FS_Layer.o
OBJS += Formats/BD_FS_Layer/BD_FS_rt_stream_Linux.o
OBJS += Formats/Caches/BDROMNavigationCache.o
OBJS += Formats/BitPackers/BDROMClipInfo.o
OBJS += Formats/BitPackers/BDROMCommonTypes.o
OBJS += Formats/BitPackers/BDROMCPI.o
OBJS += Formats/BitPackers/BDROMIndex.o
OBJS += Formats/BitPackers/BDROMPlaylist.o
OBJS += Formats/BitPackers/BDROMSound.o

OBJS += Common/BitPacker/AggregateBitField.o
OBJS += Common/BitPacker/BitField.o
OBJS += Common/BitPacker/BitFieldArray.o
OBJS += Common/BitPacker/BitPackerTypes.o
OBJS += Common/BitPacker/BitPtr.o
OBJS += Common/BitPacker/IntField.o
OBJS += Common/BitPacker/ScalarBitField.o
OBJS += Common/BitPacker/str.o
OBJS += Common/BitPacker/BitPackingEngine.o
OBJS += Common/FileSystem/FilePath.o
OBJS += Common/StreamUtils/md_stream.o
OBJS += Common/StreamUtils/ring_rt_stream.o
OBJS += Common/StreamUtils/rt_stream.o
OBJS += Common/StringUtils/UnicodeUtilities.o
OBJS += Common/Geometry/PiPoint.o
OBJS += Common/Geometry/PiRect.o
OBJS += Common/Geometry/PiSize.o
OBJS += Common/SurfaceUtils/Common/ColorPalette.o
OBJS += Common/SurfaceUtils/Surface/BaseSurface.o
OBJS += Common/SurfaceUtils/Surface/BaseSurfaceUtils.o
OBJS += Common/SurfaceUtils/Surface/DirectFBSurface.o
OBJS += Common/SurfaceUtils/Surface/DirectFBSurfaceUtils.o
OBJS += Common/SurfaceUtils/Surface/IDFBSurface.o
OBJS += Common/SurfaceUtils/Surface/IDFB.o
OBJS += Common/SurfaceUtils/Surface/IDFBBase.o
OBJS += Common/SurfaceUtils/Surface/IDFBSurface.o
OBJS += Common/SurfaceUtils/Surface/IDFBDataBuffer.o
OBJS += Common/SurfaceUtils/Surface/IDFBDisplayLayer.o
OBJS += Common/SurfaceUtils/Surface/IDFBEventBuffer.o
OBJS += Common/SurfaceUtils/Surface/IDFBFont.o
OBJS += Common/SurfaceUtils/Surface/IDFBGL.o
OBJS += Common/SurfaceUtils/Surface/IDFBImageProvider.o
OBJS += Common/SurfaceUtils/Surface/IDFBInputDevice.o
OBJS += Common/SurfaceUtils/Surface/IDFBPalette.o
OBJS += Common/SurfaceUtils/Surface/IDFBScreen.o
OBJS += Common/SurfaceUtils/Surface/IDFBVideoProvider.o
OBJS += Common/SurfaceUtils/Surface/IDFBWindow.o
#OBJS += Common/SurfaceUtils/Render/BasePrimitives.o
OBJS += Common/SurfaceUtils/Render/BaseText.o
#OBJS += Common/SurfaceUtils/Render/DirectFBImageUtils.o
#OBJS += Common/SurfaceUtils/Render/DirectFBPrimitives.o
OBJS += Common/SurfaceUtils/Render/DirectFBText.o


OBJS += Players/Renderer/BaseDisplayFrame.o
OBJS += Players/Renderer/DisplayLayer.o
OBJS += Players/Renderer/WinDisplayFrame.o
OBJS += Players/Renderer/CEStreamDescriptor.o
OBJS += Players/Renderer/BaseRenderer.o
OBJS += Players/Renderer/CERenderer.o
OBJS += Players/Renderer/BaseSprite.o
OBJS += Players/Renderer/ImageRef.o
OBJS += Players/Renderer/TextRenderer.o

OBJS += CInputHDMV.o
OBJS += HDMVSignature.o

#.c.o:
#	$(SHOW_COMMAND)$(rm) -f $@
#	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
#	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG)  $(WARNING) -O -c $*.c

.cpp.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.cpp] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CXX) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -O -c $< -o $@ 


all:$(LIBNAME)


ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CXX) -shared -o $(LIBNAME) $(OBJS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
