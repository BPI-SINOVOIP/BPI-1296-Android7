#ifndef __RTK_TOGO_H__
#define __RTK_TOGO_H__

#include "RtkTranscodeCommon.h"

#define MAX_TOGO_INSTANCES	1
#define MAX_SINGLE_PARAM_STRING_BYTES	256

enum {
	TOGO_STATE_IDLE = 0,
	TOGO_STATE_INITED,
	TOGO_STATE_RUN,
	TOGO_STATE_STOP
};

enum {
	TOGO_TYPE_CAMERA = 0,
	TOGO_TYPE_HDMI_RX,
	TOGO_TYPE_FILE,
	TOGO_TYPE_DTV,
	TOGO_TYPE_MAX
};

enum {
    MUXER_TS = 0,
    MUXER_MP4,
    MUXER_AAC,
    MUXER_MAX
};

enum {
    TOGO_OUTPUT_FILE = 0,
    TOGO_OUTPUT_STDOUT = 1,
    TOGO_OUTPUT_ASHMEM = 2,
    TOGO_OUTPUT_ASHMEM_RTP = 3,
    TOGO_OUTPUT_UDP = 4,
    TOGO_OUTPUT_UDP_RTP = 5,
    TOGO_OUTPUT_UDP_ASHMEM = 6,	// TOGO_OUTPUT_RTP_BOTH
	TOGO_OUTPUT_FD = 7,
    TOGO_OUTPUT_MAX
};

enum {
	CAM_PARAM_KEY_PREVIEW_SIZE,
	CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES,
	CAM_PARAM_KEY_PREVIEW_FRAME_RATE,
	CAM_PARAM_KEY_SUPPORTED_PREVIEW_FRAME_RATES,
	CAM_PARAM_MAX
};

//enum {
//	CAM_PROFILE_QUALITY_LOW = 0,
//	CAM_PROFILE_QUALITY_HIGH = 1
//};

class TvProxy;

namespace android {

class Camera;
class MediaRecorder;
class MediaRecorderListener;
class SurfaceComposerClient;
class Surface;
class SurfaceControl;
class RtkTranscodePlayer;
class RTMediaPlayer;
class RTDtvMediaPlayer;

struct ALooper;
class ToGoHandler;

struct ToGoFileAttributes {
	int width;
	int height;
};

struct ToGoSharedMemInfo {
	int client_sockfd;
	int sharefd;
	void* pMemHeader;
	int memSize;
};

struct ToGoParams {
	// params to Camera
	bool bIsPreview;
	int previewWidth;
	int previewHeight;
	int previewFrameRate;
	bool bIsEnableH264Capture;
	bool bH264Enabled;
	// params to MediaRecorder
	int muxerFormat;
	int outputType;
	int outputfd;	// used in TOGO_TYPE_CAMERA
	int duration;
	bool bRtpOption;
	char ip[32];
	int port;
	int vbps;
	int iFramesIntervalSeconds;
	int audioChannelNum;
	int audioSampleRate;
	int abps;
	bool bHasAshmem;
	int startTime;
	//int profile;
	char targetFilename[256];	// used in TOGO_TYPE_FILE
	RtkTranscodeConf outputConfig;
	AV_FORMAT_CONFIG targetFormat;
	char latGetParam[MAX_SINGLE_PARAM_STRING_BYTES];
};

struct ToGoInstance {
	int handle;	// equals to client id which give to client and client use it to request actions.
	int state;
	int type;
	int cameraId;	// when type is TOGO_TYPE_DTV, cameraId stores tunerId.
	sp<Camera> camera;
	sp<MediaRecorder> recorder;
	sp<MediaRecorderListener> listener;
	sp<SurfaceComposerClient> composerClient;
	sp<SurfaceControl> control;
	sp<Surface> surface;
	sp<IGraphicBufferProducer> gbp;
	sp<RtkTranscodePlayer> player;
	sp<ALooper> looper;
	sp<ToGoHandler> listenHandler;
	sp<ALooper> listenLooper;
	sp<RTMediaPlayer> rtMp;
	TvProxy	*tvProxy;
	int tunerAppType;
	sp<ToGoHandler> dtvListenHandler;
	sp<ALooper> dtvListenLooper;
	RTDtvMediaPlayer *rtDtv;
	ToGoParams params;
	ToGoFileAttributes attributes;
};

class RtkToGo
{
public:
	static RtkToGo* getInstance();
	static int releaseInstance();
	int init(int type, int cameraId, const sp<MediaRecorderListener>& listener);
	void uninit(int handle);
	// params to Camera
	void setPreview(int handle, bool bIsPreview);
	int setPreviewSize(int handle, int width, int height);
	int setVideoFrameRate(int handle, int vfps);
	void setEnableH264Capture(int handle, bool isEnableH264Capture);
	// params to MediaRecorder
	void setMuxerFormat(int handle, int muxerFormat);
	void setOutputType(int handle, int outputType);
	void setOutputFD(int handle, int fd);
	void setDuration(int handle, int duration);
	void setUdpIpPort(int handle, char* ip, int port);
	void setVideoBitrate(int handle, int vbps);
	void setIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds);
	void setAudioChannelCount(int handle, int audioChannelNum);
	void setAudioSampleRate(int handle, int audioSampleRate);
	void setAudioBitrate(int handle, int abps);
	//void setProfile(int handle, int profile);
	int setTargetFilename(int handle, char* filename);
	void configCompleted(int handle);
	int getDuration(int handle);
	int start(int handle);
	void stop(int handle);
	void seek(int handle, int s);
	const char* getCamParams(int handle, int paramType);
	int isHandleLegal(int handle);
	int isSourceAvailable(int type);
	int getHandleMax();
	int getHandleCount();
	int getHandleSourceType(int handle);
	int getHandleState(int handle);
	const char* getLCNList(int handle);
	int PlayChannel(int handle, int chIdx);
	int PlayNextChannel(int handle);
	int PlayPrevChannel(int handle);
	int GetFileWidth(int handle);
	int GetFileHeight(int handle);
	int StartListen(int handle);	// the return value is the tunerId occupied by handle
	void StopListen(int handle);

private:
	RtkToGo();
	~RtkToGo();
	status_t getGraphicBufferProducer(int handle, int type, int width, int height, sp<IGraphicBufferProducer> *gbp);
	status_t clearGraphicBufferProducer(int handle);
	status_t createSurface(int width, int height, sp<SurfaceComposerClient> *composerClient, sp<SurfaceControl> *control, sp<Surface> *surface);
	status_t openCamera(int togoHandle);
	status_t configCamera(ToGoInstance* pToGo);
	status_t previewCamera(sp<Camera> &camera, sp<IGraphicBufferProducer> &gbp);
	const char* getCameraParameters(sp<Camera> camera, int paramType, ToGoParams* pToGoParams=NULL);
	status_t setCameraParameters(sp<Camera> camera, ToGoParams* pToGoParams);
	void initMediaRecorderParameters(int cameraId, ToGoParams* pToGoParams, String8 &params);
	int checkHandle(int handle);
	int startToGoCamera(ToGoInstance* pToGo, int handle);
	int startToGoFile(ToGoInstance* pToGo, int handle);
	int startToGoHdmiRx(ToGoInstance* pToGo, int handle);
	void stopToGoHDMIRx(ToGoInstance* pToGo);
	
	int startToGoFileViaRTMediaPlayer(ToGoInstance* pToGo);
	int stopToGoFileViaRTMediaPlayer(ToGoInstance* pToGo);
	int startToGoDtv(ToGoInstance* pToGo);
	int stopToGoDtv(ToGoInstance* pToGo);

	static RtkToGo* m_instance;
	static int m_referCount;
	static bool m_bIsRecBufServiceRunning;

	ToGoInstance m_togo[MAX_TOGO_INSTANCES];
};

}	// namespace android

#endif
