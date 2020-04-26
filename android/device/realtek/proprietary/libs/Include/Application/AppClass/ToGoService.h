#ifndef _TOGO_SERVICE_H_
#define _TOGO_SERVICE_H_

#include <utils/StrongPointer.h>
#include <binder/IBinder.h>

class ToGoSourceStatus;
class ToGoClientStatus;

using namespace android;

class ToGoService {
public:
	static ToGoService* GetInstance();
	static void ReleaseInstance();

	int bindToGoAppService();
	sp<IBinder> getToGoAppService();

	int init(int type, int cameraId);
	void uninit(int handle);
	void setPreview(int handle, bool isPreview);
	int setPreviewSize(int handle, int width, int height);
	int setVideoFrameRate(int handle, int vfps);
	void setEnableH264Capture(int handle, bool isEnableH264Capture);
	void setMuxerFormat(int handle, int muxerFormat);
	void setOutputType(int handle, int outputType);
	void setOutputFd(int handle);
	void setDuration(int handle, int duration);
	void setUdpIpPort(int handle, const char* ip, int port);
	void setVideoBitrate(int handle, int vbps);
	void setIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds);
	void setAudioChannelCount(int handle, int audioChannelNum);
	void setAudioSampleRate(int handle, int audioSampleRate);
	void setAudioBitrate(int handle, int abps);
	//void setProfile(int handle, int profile);
	void configCompleted(int handle);
	int start(int handle);
	void stop(int handle);
	void seek(int handle, int ms);
	int getDuration(int handle);

	const char* getCamParams(int handle, int paramType);
	int isHandleLegal(int handle);
	void getSourceStatus(ToGoSourceStatus* pStatus);
	void getClientStatus(ToGoClientStatus* pStatus);
	int setTargetFilename(int handle, const char* filename);
	const char* getLCNList(int handle);
	int PlayChannel(int handle, int chIdx);
	int PlayNextChannel(int handle);
	int PlayPrevChannel(int handle);
	int GetFileWidth(int handle);
	int GetFileHeight(int handle);
	int StartListen(int handle);
	void StopListen(int handle);
	int getHandleSourceType(int handle);

private:
	ToGoService();
	~ToGoService();	

	static ToGoService *m_instance;
	static int m_refCount;
	sp<IBinder>	mToGoProxyService;
	sp<IBinder> mToGoServiceBinder;

	sp<IBinder> GetToGoServiceBinder();
};








#endif
