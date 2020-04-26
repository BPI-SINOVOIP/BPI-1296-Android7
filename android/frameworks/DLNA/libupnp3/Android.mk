LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    DMR_MicroStack.c \
    ILibAsyncServerSocket.c \
    ILibAsyncSocket.c \
    ILibAsyncUDPSocket.c \
    ILibParsers.c \
    ILibSSDPClient.c \
    ILibThreadPool.c \
    ILibWebClient.c \
    ILibWebServer.c \
    Main.c \
    MediaServerCP_ControlPoint.c \
    MediaServer_MicroStack.c \
    MyString.c \
    CdsObjects/CdsDidlSerializer.c \
    CdsObjects/CdsErrors.c \
    CdsObjects/CdsMediaClass.c \
    CdsObjects/CdsObject.c \
    DmsIntegration/DmsIntegration.c \
    DmsIntegration/FileSystemEnumerator/FileSystemEnumerator.c \
    DownloadController/DownloadController.c \
    FileAbstractionFiles/FileIoAbstraction.c \
    HttpFiles/DlnaHttp.c \
    HttpFiles/DlnaHttpClient.c \
    HttpFiles/DlnaHttpServer.c \
    MediaRenderer/DMR.c \
    MediaServerAbstraction/MediaServerAbstraction.c \
    MediaServerBrowser/FilteringBrowser.c \
    MediaServerBrowser/MediaServerControlPoint.c \
    PlaylistTrackManager/BitArray.c \
    PlaylistTrackManager/CircularBuffer.c \
    PlaylistTrackManager/IndexBlocks.c \
    PlaylistTrackManager/PlayListManager.c \
    PlaylistTrackManager/PlaylistManagerPC.c \
    PlaylistTrackManager/PlaylistManagerS.c \
    PlaySingleUri/PlaySingleUri.c \
    ProtocolInfoParser/DLNAProtocolInfo.c \
    StringUtils/MimeTypes.c \
    StringUtils/UTF8Utils.c \
    FileAccess/array_funcs.cpp \
    FileAccess/file_db.cpp \
    FileAccess/file_util.cpp \
    FileAccess/gfile_common.cpp \
    FileAccess/list_funcs.cpp \
    FileAccess/simple_funcs.cpp \
    FileAccess/fastcopy_wrapper.cpp \
    FileAccess/OSAL.cpp \
    FileAccess/OpenFile.cpp

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include \
    $(LOCAL_PATH)/PlaylistTrackManager \
    $(LOCAL_PATH)/MediaServerBrowser \
    $(LOCAL_PATH)/PlaySingleUri \
	$(LOCAL_PATH)/FileAccess

LOCAL_CFLAGS := \
    -DINCLUDE_FEATURE_VOLUME \
    -D_POSIX \
    -D_DEBUG \
    -D_VERBOSE \
	-D_UNIX \
    -DMMSCP_LEAN_AND_MEAN \
    -D_MT_CONNECTION_MANAGER \
    -D_MT_RENDERINGCONTROL \
    -D_MT_AVTRANSPORT \
    -DSYS_UNIX:=1 \
    -D_REENTRANT \
	-D_ANDROID

ifeq ($(DLNADMRCTT), YES)
  LOCAL_CFLAGS += -DDLNADMRCTT
endif

LOCAL_SHARED_LIBRARIES := libcutils

LOCAL_MODULE := libupnp3

LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)