#ifndef __RTK_TRANSCODE_H__
#define __RTK_TRANSCODE_H__

#include "Errors.h"
#include "StrongPointer.h"
#include "String8.h"

enum {
    MUXER_TS = 0,
    MUXER_MP4 = 1,
    MUXER_AAC = 2,
    MUXER_MAX
};

enum {
    OUTPUT_FILE = 0,
    OUTPUT_STDOUT = 1,
    OUTPUT_ASHMEM = 2,
    OUTPUT_ASHMEM_RTP = 3,
    OUTPUT_UDP = 4,
    OUTPUT_UDP_RTP = 5,
    OUTPUT_RTP_BOTH = 6,
    OUTPUT_MAX
};

enum {
	STATE_IDLE = 0,
	STATE_INIT = 1,
	STATE_RUN = 2
};

namespace android {

class SurfaceControl;
class Surface;
class Camera;
class CameraSource;
class OMXClient;
class MediaSource;
class AudioSource;
class MediaRecorder;

class RtkTranscode
{
public:
	static RtkTranscode* GetInstance();
	static void ReleaseInstance();

	bool Init();
	void UnInit();
	bool Start();
	bool Stop();

	void SetUseMediaRecorder(bool isEnable);
	void SetMuxerFormat(int muxerFormat);
	void SetOutputType(int outputType);
	void SetOutputFilename(char* outputFilename);
	void SetOutputFD(int fd);
	void SetDuration(int duration);
	void SetUdpIpPort(char* ip, int port);
	void SetOutputVideoStream(bool isOutputVideoStream);
	void SetVideoBitrate(int vbps);
	void SetColorFormat(int colorFormat);
	void SetVideoFrameRate(int vfps);
	void SetIFrameIntervalSeconds(int iFramesIntervalSeconds);
	void SetVideoFramesToBeRecorded(int nFrames);
	void SetVideoWidthHeight(int width, int height);
	void SetVideoCodec(int videoCodec);
	void SetPreferSoftwareCodec(bool isPreferSoftwareCodec);
	void SetEncTest(bool isEncTest);
	void SetPreviewTimeBeforeEncoding(int previewTime);
	void SetPreview(bool isPreview);
	void SetCameraId(int cameraId);
	void SetStoreMetaDataInVideoBuffers(bool isStoreMetaDataInVideoBuffers);
	void SetEnableH264Capture(bool isEnableH264Capture);
	void SetOutputAudioStream(bool isOutputAudioStream);
	void SetAudioChannelCount(int audioChannelNum);
	void SetAudioSampleRate(int audioSampleRate);
	void SetAudioBitrate(int abps);

private:
	RtkTranscode();
	~RtkTranscode();

	status_t setupSurface(sp<SurfaceControl> *control, sp<Surface> *surface);
	status_t setupCamera(sp<Camera> *camera);
	void previewCamera(sp<Surface> &surface, sp<Camera> &camera);
	status_t setupVideoSource(sp<Surface> &surface, sp<Camera> &camera, sp<CameraSource> *cameraSource);
	status_t setupVideoEncoder(OMXClient &client, sp<CameraSource> &cameraSource, sp<MediaSource> *videoEncoder);
	status_t setupAudioSource(sp<AudioSource> *audioSource);
	status_t setupAudioEncoder(OMXClient &client, sp<AudioSource> &audioSource, sp<MediaSource> *audioEncoder);
	status_t startVideoEncoderTest(sp<MediaSource> &videoEncoder);
	status_t startMuxerTest(sp<MediaSource> &videoSource, sp<MediaSource> &audioSource);
	void initMediaRecorderParameters(String8 &params);
	status_t startMediaRecorder();
	
	
	static RtkTranscode *m_instance;
	static int m_refCount;
	bool m_bUseMediaRecorder;
	int m_muxerFormat;
	int m_outputType;
	char* m_outputFilename;
	int m_outputFD;
	int m_duration;
	char* m_udpIp;
	int m_udpPort;
	bool m_bHasVideo;
	int m_vbps;
	int m_colorFormat;
	int m_vfps;
	int m_iFramesIntervalSeconds;
	int m_nFrames;
	int m_width;
	int m_height;
	int m_videoCodec;
	bool m_bPreferSoftwareCodec;
	bool m_bEncTest;
	int m_previewTime;
	bool m_bHasPreview;
	int m_cameraId;
	bool m_bStoreMetaDataInVideoBuffers;
	bool m_bEnableH264Capture;
	bool m_bHasAudio;
	int m_audioChannelNum;
	int m_audioSampleRate;
	int m_abps;
	bool m_bRtpOption;
	bool m_bHasAshmem;
	bool m_bH264Enabled;
	int m_level;	// Encoder specific default
	int m_profile;	// Encoder specific default
	int m_quit;
	int m_state;
	sp<MediaRecorder> mr;
	sp<Camera> camera;
};

}	// namespace android

#endif

