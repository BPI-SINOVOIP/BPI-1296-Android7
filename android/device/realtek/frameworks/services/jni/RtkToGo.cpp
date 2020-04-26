//#define LOG_NDEBUG 0
#define LOG_TAG "RtkToGo"
#include <utils/Log.h>
#include <RtkIpc/IRecordBuffer.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <camera/CameraParameters.h>
//#include <camera/ICamera.h>
//#include <camera/ICameraRecordingProxy.h>
#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <utils/String16.h>
#include <binder/Parcel.h>
#include "RtkToGo.h"
#include <sys/stat.h> 
#include <fcntl.h>
#include <camera/Camera.h>
#include <media/mediarecorder.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/Surface.h>
#include <gui/SurfaceControl.h>
#include <ALooper.h>
#include "RtkTranscodePlayer.h"
#include "RtkTranscodeCommon.h"
#include "RTMediaPlayer.h"
#include "TvProxy.h"
#include "DtvServiceDefs.h"
#include "RTDtvMediaPlayer.h"
#include "DvbChannelInfo.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <Platform_Lib/SocketAVData/SocketAVData.h>
#include <sys/mman.h>

#define TOGO_FILE_VIA_RTMEDIAPLAYER


#define CAMERA_ID_HDMIRX 1

// [0]Default [1]MIC [5]CAMCORDER [9]HDMI Rx 
#define AUDIO_SOURCE_HDMIRX 9

namespace android {
	
// Transact to ToGoProxyService
int TRANSACTION_bindToGoAppService = (IBinder::FIRST_CALL_TRANSACTION + 0);
int TRANSACTION_getToGoAppService = (IBinder::FIRST_CALL_TRANSACTION + 1);
int TRANSACTION_getGraphicBufferProducer = (IBinder::FIRST_CALL_TRANSACTION + 2);
int TRANSACTION_clearGraphicBufferProducer = (IBinder::FIRST_CALL_TRANSACTION + 3);

class ToGoHandler : public AHandler
{
public:
	ToGoHandler();
	virtual ~ToGoHandler();
	void setToGoInstance(ToGoInstance* pToGo) {
		m_pToGo = pToGo;
	}
protected:
	virtual void onMessageReceived(const sp<AMessage> &msg);
private:
	ToGoInstance* m_pToGo;
};

ToGoHandler::ToGoHandler()
{	
}

ToGoHandler::~ToGoHandler()
{
}

void ToGoHandler::onMessageReceived(const sp<AMessage> &msg)
{
	switch (msg->what()) {
		case kWhatEndOfStream:
			ALOGE("handle=%d.received.kWhatEndOfStream.",m_pToGo->handle);
			//looper()->stop();
			break;
		default:
			ALOGV("handle=%d.received.what()=%d.",m_pToGo->handle,msg->what());
			break;
	}
}

/*	
#define RECORD_DATA_SIZE    (1500 + 4)* 200
#define RECORD_INFO_SIZE    32
#define MEMORY_SIZE (RECORD_DATA_SIZE + RECORD_INFO_SIZE)
	
class RecordBufferService : public BnRecordBuffer {
	public:
		static void instantiate();
		RecordBufferService();
		virtual ~RecordBufferService();
		virtual sp<IMemory> getBuffer(int id);
		virtual sp<IMemory> getBuffer(int id, int handle);
	private:
		sp<MemoryHeapBase> mMemHeap0;
		sp<MemoryBase> mBufferData0;
		sp<MemoryBase> mBufferInfo0;
		sp<MemoryHeapBase> mMemHeap1;
		sp<MemoryBase> mBufferData1;
		sp<MemoryBase> mBufferInfo1;
};
	
sp<IMemory> RecordBufferService::getBuffer(int id)
{
	return NULL;
}
	
sp<IMemory> RecordBufferService::getBuffer(int id, int handle)
{
	if (handle < 0 || handle >= MAX_TOGO_INSTANCES) {
		ALOGE("Invalid handle=%d.",handle);
		return NULL;
	}

	if (handle == 0) {
	switch(id){
		case RECBUF_TYPE_DATA:
				return mBufferData0;
		case RECBUF_TYPE_INFO:
				return mBufferInfo0;
		default:
			return NULL;
	}
}
	else if (handle == 1) {
		switch(id) {
			case RECBUF_TYPE_DATA:
				return mBufferData1;
			case RECBUF_TYPE_INFO:
				return mBufferInfo1;
			default:
				return NULL;
		}
	}

	return NULL;
}
	
void RecordBufferService::instantiate()
{
	status_t status;
	status = defaultServiceManager()->addService(String16("vendor.realtek.RecordBuffer"), new RecordBufferService());
}
	
RecordBufferService::RecordBufferService()
{
	//The memory is allocated using a MemoryHeapBase, and thereby is using ashmem
	
	mMemHeap0 = new MemoryHeapBase(MEMORY_SIZE);
	mBufferData0 = new MemoryBase(mMemHeap0, 0, RECORD_DATA_SIZE);
	mBufferInfo0 = new MemoryBase(mMemHeap0, RECORD_DATA_SIZE, RECORD_INFO_SIZE);

	mMemHeap1 = new MemoryHeapBase(MEMORY_SIZE);
	mBufferData1 = new MemoryBase(mMemHeap1, 0, RECORD_DATA_SIZE);
	mBufferInfo1 = new MemoryBase(mMemHeap1, RECORD_DATA_SIZE, RECORD_INFO_SIZE);	
}
	
RecordBufferService::~RecordBufferService()
{
	mMemHeap0 = NULL;
	mBufferData0 = NULL;
	mBufferInfo0 = NULL;

	mMemHeap1 = NULL;
	mBufferData1 = NULL;
	mBufferInfo1 = NULL;	
}

int getRecordBuffer(int handle, sp<IMemory> *data, sp<IMemory> *info)
{
	if (handle < 0 || handle >= MAX_TOGO_INSTANCES) {
		ALOGE("Invalid handle=%d.",handle);
		return -1;
	}

	static sp<IRecordBuffer> recordBuffer;
	
	if (recordBuffer == NULL)
	{
		sp<IServiceManager> sm = defaultServiceManager();
		sp<IBinder> binder;
		binder = sm->getService(String16("vendor.realtek.RecordBuffer"));
		if (binder != 0)
		{
			recordBuffer = IRecordBuffer::asInterface(binder);
		}
	}
	if (recordBuffer == NULL)
	{
		ALOGE("The buffer service is not published");
		return -1;
	}
	else
	{
		*data = recordBuffer->getBuffer(RECBUF_TYPE_DATA, handle);
		*info = recordBuffer->getBuffer(RECBUF_TYPE_INFO, handle);
		return 0;
	}
}
*/

}	// namespace android

using namespace android;

RtkToGo* RtkToGo::m_instance = NULL;
int RtkToGo::m_referCount = 0;
bool RtkToGo::m_bIsRecBufServiceRunning = false;

RtkToGo* RtkToGo::getInstance()
{
	if (m_instance == NULL) {
		m_instance = new RtkToGo();
	}
	m_referCount++;
	return m_instance;
}

int RtkToGo::releaseInstance()
{
	m_referCount--;
	if (m_referCount == 0) {
		delete m_instance;
		m_instance = NULL;
	}
	return m_referCount;
}

RtkToGo::RtkToGo()
{
	ALOGV("enter.RtkToGo.constructor.");
	memset(m_togo, 0, sizeof(ToGoInstance)*MAX_TOGO_INSTANCES);
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		m_togo[i].handle = i;
		m_togo[i].cameraId = -1;
		// params to Camera
		m_togo[i].params.bIsPreview = false;
		m_togo[i].params.previewWidth = 320;
		m_togo[i].params.previewHeight = 240;
		m_togo[i].params.previewFrameRate = 5;
		m_togo[i].params.bIsEnableH264Capture = false;
		m_togo[i].params.bH264Enabled = false;
		// params to MediaRecorder
		m_togo[i].params.muxerFormat = MUXER_TS;
		m_togo[i].params.outputType = TOGO_OUTPUT_ASHMEM;
		m_togo[i].params.outputfd = -1;
		m_togo[i].params.duration = 0;
		m_togo[i].params.bRtpOption = false;
		m_togo[i].params.port = 0;
		m_togo[i].params.vbps = 800000;
		m_togo[i].params.iFramesIntervalSeconds = 1;
		m_togo[i].params.audioChannelNum = 1;
		m_togo[i].params.audioSampleRate = 16000;
		m_togo[i].params.abps = 64000;
		m_togo[i].params.bHasAshmem = true;	// it must sync with m_togo[i].params.outputType
		m_togo[i].params.startTime = 0;

		//m_togo[i].params.profile = -1;

		m_togo[i].params.targetFormat.videoFmt.width = 1280;
		m_togo[i].params.targetFormat.videoFmt.height = 720;
		m_togo[i].params.targetFormat.videoFmt.fps = 30;
		m_togo[i].params.targetFormat.videoFmt.bitrate = 2000000;	// 2Mbps
		m_togo[i].params.targetFormat.videoFmt.iframeInterval = 1;
		m_togo[i].params.targetFormat.videoFmt.rateControl = 2; // constant bitrate
		m_togo[i].params.targetFormat.videoFmt.aspectRatio = 0; // reserved

		m_togo[i].params.targetFormat.audioFmt.channelCount = 2;
		m_togo[i].params.targetFormat.audioFmt.sampleRate = 48000;
		m_togo[i].params.targetFormat.audioFmt.bitrate = 96000;

		m_togo[i].tunerAppType = -1;

		//
		m_togo[i].attributes.width = -1;
		m_togo[i].attributes.height = -1;

	}

}

RtkToGo::~RtkToGo()
{
	
}

int RtkToGo::init(int type, int cameraId, const sp<MediaRecorderListener>& listener)
{
	ToGoInstance* pToGo = NULL;

	if (type < TOGO_TYPE_CAMERA || type > TOGO_TYPE_DTV) {
		ALOGE("Invalid type=%d.",type);
		return -1;
	}

	// Get available ToGoInstance.
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if ((type == TOGO_TYPE_CAMERA) && (cameraId == m_togo[i].cameraId)) {
			ALOGE("cameraId=%d has been occupied.",cameraId);
			break;
		}
		if (m_togo[i].state == TOGO_STATE_IDLE) {
			pToGo = &(m_togo[i]);
			break;
		}
	}

	if (pToGo == NULL) {
		ALOGE("No available ToGoInstance.cameraId=%d.",cameraId);
		return -1;
	}
/*
	if (createSurface(pToGo->params.previewWidth, pToGo->params.previewHeight, 
			&(pToGo->composerClient), &(pToGo->control), &(pToGo->surface)) != OK) {
		ALOGE("createSurface() NG.");
		return -1;
	}
	if ((pToGo->control == NULL) || (pToGo->surface == NULL)) {
		ALOGE("control==NULL || pToGo->surface==NULL.");
		return -1;
	}
*/

	if (type == TOGO_TYPE_CAMERA) {
		pToGo->cameraId = cameraId;
		if (openCamera(pToGo->handle) != OK) {
			ALOGE("openCamera() NG.");
			pToGo->cameraId = -1;
			return -1;
		}
	}
	else if (type == TOGO_TYPE_HDMI_RX ) {
		pToGo->cameraId = CAMERA_ID_HDMIRX;
		pToGo->params.outputConfig.outputType = 2;

		// test if HDMIRx is available
		ALOGE("Open Camera(HDMIRx) for testing");
		if (openCamera(pToGo->handle) != OK) {
			ALOGE("openCamera() NG. HDMIRx is not available");
			return -1;
		}
		
		ALOGE("Camera connect OK, HDMIRx available");
	}
	else if (type == TOGO_TYPE_FILE) {
		pToGo->params.outputConfig.outputType = 2;
		ALOGV("set pToGo->params.outputConfig.outputType=%d.",pToGo->params.outputConfig.outputType);
	}	else if (type == TOGO_TYPE_DTV) {
		ALOGV("begin TOGO_TYPE_DTV init.");

		// Todo: Here to check if the specified tuner id has already the corresponding RTDtvMediaPlayer.

#ifndef ENABLE_1195_DTV_FLOW
		pToGo->rtDtv = new RTDtvMediaPlayer();

		if (pToGo->rtDtv->initCheck() != NO_ERROR) {
			ALOGE("initCheck() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}

		if (pToGo->rtDtv->setDataSource("stb://0", NULL) != NO_ERROR) {
			ALOGE("setDataSource() NG.targetFilename=%s.","stb://0");
			stopToGoDtv(pToGo);
			return -1;
		}
	
		if (pToGo->rtDtv->prepare() != NO_ERROR) {
			ALOGE("prepare() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}
		/*
		if (pToGo->rtDtv->setVideoSurfaceTexture(pToGo->gbp) != NO_ERROR) {
			ALOGE("setVideoSurfaceTexture() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}
		*/
#endif	//	#ifndef ENABLE_1195_DTV_FLOW

		pToGo->tvProxy = new TvProxy();
		if (pToGo->tvProxy == NULL) {
			ALOGE("new TvProxy() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}
		if (pToGo->tvProxy->ConnectToServer() == false) {
			ALOGE("ConnectToServer() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}

		pToGo->cameraId = pToGo->tvProxy->GetAvailableTunerId();
		if (pToGo->cameraId < 0) {
			ALOGE("GetAvailableTunerId() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}
		ALOGV("tunerId=%d.",pToGo->cameraId);

		pToGo->tunerAppType = pToGo->tvProxy->Init(pToGo->cameraId, APP_TOGO);
		if (pToGo->tunerAppType < 0) {
			ALOGE("Init() NG.tunerId=%d.",pToGo->cameraId);
			stopToGoDtv(pToGo);
			return -1;
		}

		pToGo->params.outputConfig.outputType = 2;
		ALOGV("set pToGo->params.outputConfig.outputType=%d.",pToGo->params.outputConfig.outputType);
		ALOGV("end TOGO_TYPE_DTV init.");
	}

	pToGo->type = type;
	pToGo->listener = listener;

	if (getGraphicBufferProducer(pToGo->handle, pToGo->type, pToGo->params.previewWidth, pToGo->params.previewHeight, &(pToGo->gbp)) != OK) {
		ALOGE("getGraphicBufferProducer() NG.");
		return -1;
	}
	ALOGV("handle=%d.pToGo->gbp=%p.",pToGo->handle,pToGo->gbp.get());

	pToGo->state = TOGO_STATE_INITED;

	return pToGo->handle;
}

void RtkToGo::uninit(int handle)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		return;
	}

	if (pToGo->state != TOGO_STATE_STOP) {
		ALOGE("Not allow uninit on NON stop state.");
		return;
	}

	int oriHandle = pToGo->handle;
	memset(pToGo, 0, sizeof(ToGoInstance));
	pToGo->handle = oriHandle;
	pToGo->cameraId = -1;
	// params to Camera
	pToGo->params.bIsPreview = false;
	pToGo->params.previewWidth = 320;
	pToGo->params.previewHeight = 240;
	pToGo->params.previewFrameRate = 5;
	pToGo->params.bIsEnableH264Capture = false;
	pToGo->params.bH264Enabled = false;
	// params to MediaRecorder
	pToGo->params.muxerFormat = MUXER_TS;
	pToGo->params.outputType = TOGO_OUTPUT_ASHMEM;
	pToGo->params.outputfd = -1;
	pToGo->params.duration = 0;
	pToGo->params.bRtpOption = false;
	pToGo->params.port = 0;
	pToGo->params.vbps = 800000;
	pToGo->params.iFramesIntervalSeconds = 1;
	pToGo->params.audioChannelNum = 1;
	pToGo->params.audioSampleRate = 16000;
	pToGo->params.abps = 64000;
	pToGo->params.bHasAshmem = true;	// it must sync with m_togo[i].params.outputType
	//pToGo->params.profile = -1;

	pToGo->params.targetFormat.videoFmt.width = 1280;
	pToGo->params.targetFormat.videoFmt.height = 720;
	pToGo->params.targetFormat.videoFmt.fps = 30;
	pToGo->params.targetFormat.videoFmt.bitrate = 2000000;	// 2Mbps
	pToGo->params.targetFormat.videoFmt.iframeInterval = 1;
	pToGo->params.targetFormat.videoFmt.rateControl = 2; // constant bitrate
    pToGo->params.targetFormat.videoFmt.aspectRatio = 0; // reserved

	pToGo->params.targetFormat.audioFmt.channelCount = 2;
	pToGo->params.targetFormat.audioFmt.sampleRate = 48000;
	pToGo->params.targetFormat.audioFmt.bitrate = 96000;

	pToGo->tunerAppType = -1;

	pToGo->attributes.width = -1;
	pToGo->attributes.height = -1;

	pToGo->state = TOGO_STATE_IDLE;

	ALOGE("uninit ToGoInstance.handle=%d.",pToGo->handle);
}

void RtkToGo::setPreview(int handle, bool bIsPreview)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	pToGo->params.bIsPreview = bIsPreview;
}

int RtkToGo::setPreviewSize(int handle, int width, int height)
{
	char sizeStr[16];

	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		memset(sizeStr, 0, sizeof(char)*16);
		snprintf(sizeStr, 16, "%dx%d", width,height);
		//ALOGD("sizeStr=%s.strlen=%d.",sizeStr,strlen(sizeStr));

		const char* supported_preview_size = getCameraParameters(pToGo->camera, CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES, &(pToGo->params));
		if (strcasestr(supported_preview_size, sizeStr) == NULL) {
			ALOGE("size %s is not supported!",sizeStr);
			return -1;
		}

		pToGo->params.previewWidth = width;
		pToGo->params.previewHeight = height;
	}
	else {
		pToGo->params.targetFormat.videoFmt.width = width;
		pToGo->params.targetFormat.videoFmt.height = height;
	}
	ALOGV("setPreviewSize.w=%d.h=%d.",width,height);

	return 0;
}

int RtkToGo::setVideoFrameRate(int handle, int vfps)
{
	char str[4];

	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		memset(str, 0, sizeof(char)*4);
		snprintf(str, 4, "%d", vfps);
		//ALOGV("str=%s.strlen=%d.",str,strlen(str));

		const char* supported_preview_frame_rates = getCameraParameters(pToGo->camera, CAM_PARAM_KEY_SUPPORTED_PREVIEW_FRAME_RATES, &(pToGo->params));
		if (strcasestr(supported_preview_frame_rates, str) == NULL) {
			ALOGE("vfps %s is not supported!",str);
			return -1;
		}

		pToGo->params.previewFrameRate = vfps;
	}
	else {
		pToGo->params.targetFormat.videoFmt.fps = vfps;
	}
	ALOGV("setVideoFrameRate.vfps=%d.",vfps);

	return 0;
}

void RtkToGo::setEnableH264Capture(int handle, bool bIsEnableH264Capture)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	pToGo->params.bIsEnableH264Capture = bIsEnableH264Capture;
}

void RtkToGo::setMuxerFormat(int handle, int muxerFormat)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	pToGo->params.muxerFormat = muxerFormat;
}

void RtkToGo::setOutputType(int handle, int outputType)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	pToGo->params.outputType = outputType;

	if (TOGO_OUTPUT_ASHMEM_RTP==outputType || TOGO_OUTPUT_UDP_RTP==outputType || TOGO_OUTPUT_UDP_ASHMEM==outputType) {
		pToGo->params.bRtpOption = true;
	}

	if (TOGO_OUTPUT_ASHMEM==outputType || TOGO_OUTPUT_ASHMEM_RTP==outputType || TOGO_OUTPUT_UDP_ASHMEM==outputType) {
		pToGo->params.bHasAshmem = true;
	}
}

void RtkToGo::setOutputFD(int handle, int fd)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		pToGo->params.outputfd = fd;
	}
	else {
		pToGo->params.outputConfig.outputType = android::OUTPUT_FD;
		pToGo->params.outputConfig.fileDesc = fd;
		ALOGV("outputType=%d.fileDesc=%d.",pToGo->params.outputConfig.outputType,pToGo->params.outputConfig.fileDesc);
	}
}

void RtkToGo::setDuration(int handle, int duration)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	pToGo->params.duration = duration;
}

void RtkToGo::setUdpIpPort(int handle, char* ip, int port)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	memset(pToGo->params.ip, 0, sizeof(char)*32);
	strcpy(pToGo->params.ip, ip);
	pToGo->params.port = port;	
}

void RtkToGo::setVideoBitrate(int handle, int vbps)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		pToGo->params.vbps = vbps;
	}
	else {
		pToGo->params.targetFormat.videoFmt.bitrate = vbps;
	}
	ALOGV("setVideoBitrate.vbps=%d.",vbps);
}

void RtkToGo::setIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		pToGo->params.iFramesIntervalSeconds = iFramesIntervalSeconds;
	}
	else {
		pToGo->params.targetFormat.videoFmt.iframeInterval = iFramesIntervalSeconds;
	}
	ALOGV("setIFrameIntervalSeconds.ifis=%d.",iFramesIntervalSeconds);
}

void RtkToGo::setAudioChannelCount(int handle, int audioChannelNum)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}
	if (audioChannelNum < 1 || audioChannelNum > 2) {
		ALOGE("Illegal audioChannelNum=%d.",audioChannelNum);
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		pToGo->params.audioChannelNum = audioChannelNum;
	}
	else {
		pToGo->params.targetFormat.audioFmt.channelCount = audioChannelNum;		
	}
	ALOGV("setAudioChannelCount.audioChannelNum=%d.",audioChannelNum);
}

void RtkToGo::setAudioSampleRate(int handle, int audioSampleRate)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		pToGo->params.audioSampleRate = audioSampleRate;
	}
	else {
		pToGo->params.targetFormat.audioFmt.sampleRate = audioSampleRate;
	}
	ALOGV("setAudioSampleRate.audioSampleRate=%d.",audioSampleRate);
}

void RtkToGo::setAudioBitrate(int handle, int abps)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		pToGo->params.abps = abps;
	}
	else {
		pToGo->params.targetFormat.audioFmt.bitrate = abps;
	}
	ALOGV("setAudioBitrate.abps=%d.",abps);
}
/*
void RtkToGo::setProfile(int handle, int profile)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		return;
	}

	pToGo->params.profile = profile;
}
*/

int RtkToGo::setTargetFilename(int handle, char* filename)
{
	if (filename == NULL) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->type != TOGO_TYPE_FILE) {
		ALOGE("ToGo FILE doesn't need input file parameter.");
		return -1;
	}

	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		ALOGE("input file NOT exist.filename=%s.",filename);
		return -1;
	}
	close(fd);

	strcpy(pToGo->params.targetFilename, filename);

	// load media for get width/height of file
	pToGo->rtMp = new RTMediaPlayer();

	if (pToGo->rtMp->initCheck() != NO_ERROR) {
		ALOGE("initCheck() NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	if (pToGo->rtMp->setDataSource(pToGo->params.targetFilename, NULL) != NO_ERROR) {
		ALOGE("setDataSource() NG.targetFilename=%s.",pToGo->params.targetFilename);
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	if (pToGo->rtMp->prepareAsync() != NO_ERROR) {
		ALOGE("prepareAsync() NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	if (pToGo->rtMp->setVideoSurfaceTexture(pToGo->gbp) != NO_ERROR) {
		ALOGE("setVideoSurfaceTexture() NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	int checkCount = 0;
	bool checkOK = false;
	while (checkCount < 10) {
		int ret = pToGo->rtMp->checkLoadMedia();
		if (ret == 2) {
			ALOGE("checkLoadMedia() NG.");
			stopToGoFileViaRTMediaPlayer(pToGo);
			return -1;
		}
		else if (ret == 1) {
			checkOK = true;
			break;
		}
		checkCount++;
		usleep(200000);		
	}
	if (checkOK == false) {
		ALOGE("checkLoadMedia() NG.checkCount=%d.",checkCount);
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}
	ALOGD("checkLoadMedia() OK.checkCount=%d.",checkCount);
	
	pToGo->attributes.width = pToGo->rtMp->getVideoWidth();
	pToGo->attributes.height = pToGo->rtMp->getVideoHeight();
	ALOGD("The resolution is %dx%d(WxH).",pToGo->attributes.width,pToGo->attributes.height);

	return 0;
}

void RtkToGo::configCompleted(int handle)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		configCamera(pToGo);
	}
}

int RtkToGo::getDuration(int handle) {
	//status_t ret = OK;
	int duration = 0;

	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->type == TOGO_TYPE_FILE) {
		if(pToGo->rtMp->getDuration(&duration) == NO_ERROR)
			return duration;
	} else {
		ALOGE("Only FILE type support this operation");
		return -1;
	}

	return -1;
}

int RtkToGo::start(int handle)
{
	//status_t ret = OK;

	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->state != TOGO_STATE_INITED) {
		ALOGE("state=%d ISNOT TOGO_STATE_INITED, not allow to start.",pToGo->state);
		return -1;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		if(startToGoCamera(pToGo, handle) != OK)
			return -1;
    }else if (pToGo->type == TOGO_TYPE_FILE) {
		if(startToGoFile(pToGo, handle) != OK)
			return -1;
    }else if (pToGo->type == TOGO_TYPE_HDMI_RX) {
		if(startToGoHdmiRx(pToGo, handle) != OK)
			return -1;
    }
    else if (pToGo->type == TOGO_TYPE_DTV) {
    	ALOGV("begin TOGO_TYPE_DTV start.");

    	if (pToGo->params.bHasAshmem) {
			sp<IMemory> bufferData = NULL;
			sp<IMemory> bufferInfo = NULL;
			if (getRecordBuffer(pToGo->params.outputConfig.ashmemData, pToGo->params.outputConfig.ashmemInfo, handle) != 0) {
				ALOGE("getRecordBuffer() NG.");
				return -1;
			}
			ALOGV("handle=%d.ashmemData=%p.ashmemInfo=%p",handle,pToGo->params.outputConfig.ashmemData.get(),pToGo->params.outputConfig.ashmemInfo.get());
		}

    	if (startToGoDtv(pToGo) < 0) {
    		ALOGE("startToGoDtv() NG.");
    		stopToGoDtv(pToGo);
    		return -1;
    	}

    	// pToGo->tunerAppType==APP_TOGO means only APP_TOGO use tuner, PlayChannel is necessary.
    	bool bIsPlayChannel = (pToGo->tunerAppType==APP_TOGO)?true:false;

    	if (bIsPlayChannel) {
			DvbChannelInfo info;
	    	int filterOutFlags = CHANNEL_FLAGS_IS_DELETED|CHANNEL_FLAGS_IS_HIDDEN|CHANNEL_FLAGS_DISAPPEARED;
	    	if (pToGo->tvProxy->GetFirstChannel(&info, filterOutFlags) < 0) {
	    		ALOGE("GetFirstChannel() NG.");
	    		stopToGoDtv(pToGo);
	    		return -1;
	    	}

	    	ALOGV("PlayChannel(%d).",info.m_logicalChNum);
	    	if (pToGo->tvProxy->PlayChannel(pToGo->cameraId, info.m_logicalChNum, true, true) == false) {
	    		ALOGE("PlayChannel() NG.tunerId=%d.",pToGo->cameraId);
	    		stopToGoDtv(pToGo);
	    		return -1;
	    	}
    	}

    	ALOGV("end TOGO_TYPE_DTV start.");
    }	// else if (pToGo->type == TOGO_TYPE_DTV)

    pToGo->state = TOGO_STATE_RUN;
    return OK;
}

void RtkToGo::stop(int handle)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		if (pToGo->recorder != NULL) {
			if (pToGo->recorder->stop() != OK) {
				ALOGE("stop() NG.");
			}

			if (pToGo->recorder->release() != OK) {
				ALOGE("release() NG.");
			}
			//delete pToGo->recorder;
			pToGo->recorder = NULL;
		}	

		if (pToGo->camera != NULL) {
			pToGo->camera->disconnect();
			pToGo->camera = NULL;
		}	
	}	// if (pToGo.type == TOGO_TYPE_CAMERA)
	else if (pToGo->type == TOGO_TYPE_FILE) {
#ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
		stopToGoFileViaRTMediaPlayer(pToGo);		
#else	// #ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
		if (pToGo->player != NULL) {
			pToGo->player->stop();
			pToGo->looper->unregisterHandler(pToGo->player->id());
			pToGo->player.clear();
			//pToGo->composerClient->dispose();
			pToGo->looper->stop();

			// unregister and stop ToGo listen looper.
			pToGo->listenLooper->unregisterHandler(pToGo->listenHandler->id());
			pToGo->listenLooper->stop();
			pToGo->player = NULL;
		}
#endif	// #ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
	}	// else if (pToGo->type == TOGO_TYPE_FILE)
	else if (pToGo->type == TOGO_TYPE_HDMI_RX) {
		stopToGoHDMIRx(pToGo);
	}// else if (pToGo->type == TOGO_TYPE_HDMI_RX)
	else if (pToGo->type == TOGO_TYPE_DTV) {
		ALOGV("begin TOGO_TYPE_DTV stop.");
		stopToGoDtv(pToGo);
		ALOGV("end TOGO_TYPE_DTV stop.");
	}	// else if (pToGo->type == TOGO_TYPE_DTV)

	clearGraphicBufferProducer(handle);

	pToGo->state = TOGO_STATE_STOP;
}



void RtkToGo::seek(int handle, int msec)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if (pToGo->type == TOGO_TYPE_CAMERA) {
		
	}	// if (pToGo.type == TOGO_TYPE_CAMERA)
	else if (pToGo->type == TOGO_TYPE_FILE) {
#ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
		//if(pToGo->rtMp->isPlaying()) {
			pToGo->rtMp->seekTo(msec);
		//} else {
			//pToGo->params.startTime = msec;
			//pToGo->rtMp->setStartTime(msec);
		//}
		
#else	// #ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
		if (pToGo->player != NULL) {
			//pToGo->player->pause();
		}
#endif	// #ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
	}	// else if (pToGo->type == TOGO_TYPE_FILE)
	else if (pToGo->type == TOGO_TYPE_HDMI_RX) {
		
	}// else if (pToGo->type == TOGO_TYPE_HDMI_RX)
	else if (pToGo->type == TOGO_TYPE_DTV) {
		
	}	// else if (pToGo->type == TOGO_TYPE_DTV)

}

const char* RtkToGo::getCamParams(int handle, int paramType)
{
	if (checkHandle(handle) < 0) {
		return NULL;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if ((pToGo == NULL) || (pToGo->camera == NULL)) {
		ALOGE("pToGo==NULL.");
		return NULL;
	}

	if (pToGo->type != TOGO_TYPE_CAMERA && pToGo->type != TOGO_TYPE_HDMI_RX) {
		ALOGE("getCamParams fail, wrong resource type=%d", pToGo->type);
		return NULL;
	}

	const char* ret = getCameraParameters(pToGo->camera, paramType, &(pToGo->params));
	ALOGD("getCamParams.handle=%d.paramType=%d.ret=%s.",handle,paramType,ret);

	return ret;
}

int RtkToGo::isHandleLegal(int handle)
{
	return checkHandle(handle);
}

int RtkToGo::isSourceAvailable(int type)
{
	int device_count = 0;
	int used_count = 0;

	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if (m_togo[i].state >= TOGO_STATE_INITED) {
			used_count++;
		}
	}
	if (used_count == MAX_TOGO_INSTANCES) {
		ALOGE("No more togo instances available.type=%d.",type);
		return -1;
	}

	switch (type) {
		case TOGO_TYPE_CAMERA:
			{
				device_count = Camera::getNumberOfCameras();
				ALOGV("camera count=%d.",device_count);
				if (device_count == 0) {
					ALOGE("No camera found.");
					return -1;
				}
			}
			break;
		case TOGO_TYPE_FILE:
			device_count = MAX_TOGO_INSTANCES;
			return 0;
		case TOGO_TYPE_DTV:
			device_count = 1;
			{
				TvProxy *tvProxy = new TvProxy();
				if (tvProxy->ConnectToServer() == false) {
					ALOGE("ConnectToServer() NG.");
					delete tvProxy;
					return -1;
				}
				if (tvProxy->GetAvailableTunerId() < 0) {
					ALOGE("GetAvailableTunerId() NG.");
					delete tvProxy;
					return -1;
				}
				delete tvProxy;
			}
			break;
		case TOGO_TYPE_HDMI_RX:
			device_count = 1;
			break;
	}

	// check the same type used
	used_count = 0;
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if ((type == m_togo[i].type) && (m_togo[i].state >= TOGO_STATE_INITED)) {
			used_count++;
		}
	}
	if (used_count == device_count) {
		ALOGE("No more device available.type=%d.",type);
		return -1;
	}

	return 0;
}

int RtkToGo::getHandleMax() 
{
	ALOGE("getHandleMax=%d", MAX_TOGO_INSTANCES);
	return MAX_TOGO_INSTANCES;
}

int RtkToGo::getHandleCount()
{
	int count = 0;
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if (m_togo[i].state >= TOGO_STATE_INITED) {
			count++;
		}
	}
	ALOGE("getHandleCount=%d", count);
	return count;
}

int RtkToGo::getHandleSourceType(int handle)
{
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if (m_togo[i].handle == handle) {
			return m_togo[i].type;
		}
	}
	return -1;
}

int RtkToGo::getHandleState(int handle)
{
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if (m_togo[i].handle == handle) {
			return m_togo[i].state;
		}
	}
	return -1;
}

const char* RtkToGo::getLCNList(int handle)
{
	if (checkHandle(handle) < 0) {
		return NULL;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return NULL;
	}

	if (pToGo->state != TOGO_STATE_INITED) {
		ALOGE("state=%d ISNOT TOGO_STATE_INITED, not allow to start.",pToGo->state);
		return NULL;
	}

	if ((pToGo->type != TOGO_TYPE_DTV) || (pToGo->tvProxy == NULL)) {
		ALOGE("type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL.");
		return NULL;
	}

	int filterOutFlags = CHANNEL_FLAGS_IS_DELETED|CHANNEL_FLAGS_IS_HIDDEN|CHANNEL_FLAGS_DISAPPEARED;
	int chCount = pToGo->tvProxy->GetChannelList(filterOutFlags);
	if (chCount == 0) {
		ALOGE("chCount == 0.");
		return NULL;
	}

	memset(pToGo->params.latGetParam, 0, sizeof(char)*MAX_SINGLE_PARAM_STRING_BYTES);

	DvbChannelInfo info;
	for (int i=0;i<chCount;i++) {
		if (pToGo->tvProxy->GetChannelByChannelIndex(&info, i) == 0) {
			sprintf(pToGo->params.latGetParam, "%s%d,", pToGo->params.latGetParam,info.m_logicalChNum);
		}
	}
	ALOGV("LCN_list=%s.",pToGo->params.latGetParam);

	pToGo->tvProxy->FreeChannelList();

	return pToGo->params.latGetParam;
}

int RtkToGo::PlayChannel(int handle, int chIdx)
{
	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->state != TOGO_STATE_RUN) {
		ALOGE("state=%d ISNOT TOGO_STATE_RUN, not allow to play next channel.",pToGo->state);
		return -1;
	}

	if ((pToGo->type != TOGO_TYPE_DTV) || (pToGo->tvProxy == NULL)) {
		ALOGE("type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL.");
		return -1;
	}

	if (pToGo->tvProxy->PlayChannel(pToGo->cameraId, chIdx, true, true) != true) {
		ALOGE("PlayChannel() NG.tunerId=%d.",pToGo->cameraId);
		return -1;
	}

	return 0;
}

int RtkToGo::PlayNextChannel(int handle)
{
	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->state != TOGO_STATE_RUN) {
		ALOGE("state=%d ISNOT TOGO_STATE_RUN, not allow to play next channel.",pToGo->state);
		return -1;
	}

	if ((pToGo->type != TOGO_TYPE_DTV) || (pToGo->tvProxy == NULL)) {
		ALOGE("type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL.");
		return -1;
	}

	int filterOutFlags = CHANNEL_FLAGS_IS_DELETED|CHANNEL_FLAGS_IS_HIDDEN|CHANNEL_FLAGS_DISAPPEARED;
	if (pToGo->tvProxy->PlayNextChannel(pToGo->cameraId, true, true, filterOutFlags) != true) {
		ALOGE("PlayNextChannel() NG.tunerId=%d.",pToGo->cameraId);
		return -1;
	}

	return 0;
}

int RtkToGo::PlayPrevChannel(int handle)
{
	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if (pToGo->state != TOGO_STATE_RUN) {
		ALOGE("state=%d ISNOT TOGO_STATE_RUN, not allow to play prev channel.",pToGo->state);
		return -1;
	}

	if ((pToGo->type != TOGO_TYPE_DTV) || (pToGo->tvProxy == NULL)) {
		ALOGE("type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL.");
		return -1;
	}

	int filterOutFlags = CHANNEL_FLAGS_IS_DELETED|CHANNEL_FLAGS_IS_HIDDEN|CHANNEL_FLAGS_DISAPPEARED;
	if (pToGo->tvProxy->PlayPrevChannel(pToGo->cameraId, true, true, filterOutFlags) != true) {
		ALOGE("PlayPrevChannel() NG.tunerId=%d.",pToGo->cameraId);
		return -1;
	}

	return 0;
}

int RtkToGo::GetFileWidth(int handle)
{
	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if ((pToGo->state != TOGO_STATE_INITED) && (pToGo->state != TOGO_STATE_RUN)) {
		ALOGE("invalid state=%d.",pToGo->state);
		return -1;
	}

	if (pToGo->type != TOGO_TYPE_FILE) {
		ALOGE("type != TOGO_TYPE_FILE.");
		return -1;
	}

	return pToGo->attributes.width;
}

int RtkToGo::GetFileHeight(int handle)
{
	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if ((pToGo->state != TOGO_STATE_INITED) && (pToGo->state != TOGO_STATE_RUN)) {
		ALOGE("invalid state=%d.",pToGo->state);
		return -1;
	}

	if (pToGo->type != TOGO_TYPE_FILE) {
		ALOGE("type != TOGO_TYPE_FILE.");
		return -1;
	}

	return pToGo->attributes.height;
}

int RtkToGo::StartListen(int handle)
{
	if (checkHandle(handle) < 0) {
		return -1;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return -1;
	}

	if ((pToGo->state != TOGO_STATE_INITED) && (pToGo->state != TOGO_STATE_RUN)) {
		ALOGE("invalid state=%d.",pToGo->state);
		return -1;
	}

	if (pToGo->type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL) {
		ALOGE("type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL.");
		return -1;
	}

	ALOGV("do StartListen().tunerId=%d.",pToGo->cameraId);
	pToGo->tvProxy->StartListen(pToGo->cameraId);
	return pToGo->cameraId;
}

void RtkToGo::StopListen(int handle)
{
	if (checkHandle(handle) < 0) {
		return;
	}

	ToGoInstance* pToGo = &(m_togo[handle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return;
	}

	if ((pToGo->state != TOGO_STATE_INITED) && (pToGo->state != TOGO_STATE_RUN)) {
		ALOGE("invalid state=%d.",pToGo->state);
		return;
	}

	if (pToGo->type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL) {
		ALOGE("type != TOGO_TYPE_DTV || pToGo->tvProxy == NULL.");
		return;
	}

	ALOGV("do StopListen().tunerId=%d.",pToGo->cameraId);
	pToGo->tvProxy->StopListen(pToGo->cameraId);
}

status_t RtkToGo::getGraphicBufferProducer(int handle, int type, int width, int height, sp<IGraphicBufferProducer> *gbp)
{
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> toGoProxyService;

	do {
		toGoProxyService = sm->getService(String16("ToGoProxy"));
		if (toGoProxyService == NULL) {
			ALOGE("ToGoProxyService is not published.");
			return INVALID_OPERATION;
		}
		ALOGV("got ToGoProxyService.");
	} while (0);

	Parcel data, reply;
	data.writeInterfaceToken(String16("com.realtek.hardware.IToGoProxyService"));
	data.writeInt32(handle);
	data.writeInt32(type);
	data.writeInt32(width);
	data.writeInt32(height);
	toGoProxyService->transact(TRANSACTION_getGraphicBufferProducer, data, &reply);
    int32_t exception;
    exception = reply.readInt32();
	*gbp = interface_cast<IGraphicBufferProducer>(reply.readStrongBinder());

	return OK;
}

status_t RtkToGo::clearGraphicBufferProducer(int handle)
{
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> toGoProxyService;

	do {
		toGoProxyService = sm->getService(String16("ToGoProxy"));
		if (toGoProxyService == NULL) {
			ALOGE("ToGoProxyService is not published.");
			return INVALID_OPERATION;
		}
		ALOGV("got ToGoProxyService.");
	} while (0);

	Parcel data, reply;
	data.writeInterfaceToken(String16("com.realtek.hardware.IToGoProxyService"));
	data.writeInt32(handle);
	toGoProxyService->transact(TRANSACTION_clearGraphicBufferProducer, data, &reply);
    int32_t exception;
    exception = reply.readInt32();

	return OK;
}

status_t RtkToGo::createSurface(int width, int height, sp<SurfaceComposerClient> *composerClient, sp<SurfaceControl> *control, sp<Surface> *surface)
{
	//sp<SurfaceComposerClient> composerClient = new SurfaceComposerClient;
	*composerClient = new SurfaceComposerClient;

	if ((*composerClient)->initCheck() != OK) {
		ALOGE("*composerClient->initCheck() NG.");
		return INVALID_OPERATION;
	}

	*control = (*composerClient)->createSurface(String8("CamSurface"), width, height, PIXEL_FORMAT_RGB_565, 0);

	if ((*control == NULL) || ((*control)->isValid() == false)) {
		ALOGE("*control == NULL or isValid() NG.");
		return INVALID_OPERATION;
	}

	SurfaceComposerClient::openGlobalTransaction();

	if ((*control)->setLayer(INT_MAX) != OK) {
		ALOGE("*control->setLayer(INT_MAX) NG.");
		return INVALID_OPERATION;
	}

	if ((*control)->show() != OK) {
		ALOGE("*control->show() NG.");
		return INVALID_OPERATION;
	}

	SurfaceComposerClient::closeGlobalTransaction();

	*surface = (*control)->getSurface();
	if (*surface == NULL) {
		ALOGE("*surface == NULL.");
		return INVALID_OPERATION;
	}

	return OK;
}

status_t RtkToGo::openCamera(int togoHandle)
{
	ToGoInstance* pToGo = &(m_togo[togoHandle]);
	if (pToGo == NULL) {
		ALOGE("pToGo==NULL.");
		return BAD_VALUE;
	}

	String16 clientPackageName("com.realtek.app.togo");
	pToGo->camera = Camera::connect(pToGo->cameraId, clientPackageName, Camera::USE_CALLING_UID, Camera::USE_CALLING_PID);
	if (pToGo->camera == NULL) {
		ALOGE("pToGo->camera==NULL.");
		return BAD_VALUE;
	}
	//getCameraParameters(pToGo->camera, -1);
	return OK;
}

status_t RtkToGo::configCamera(ToGoInstance* pToGo)
{
	setCameraParameters(pToGo->camera, &(pToGo->params));

	//getCameraParameters(pToGo->camera, -1);

	return OK;
}

status_t RtkToGo::previewCamera(sp<Camera> &camera, sp<IGraphicBufferProducer> &gbp)
{
	camera->lock();
	camera->setPreviewTarget(gbp);
	camera->startPreview();
	camera->stopPreview();
	camera->unlock();

	return OK;
}

const char* RtkToGo::getCameraParameters(sp<Camera> camera, int paramType, ToGoParams* pToGoParams)
{
	const char* ret = NULL;
	CameraParameters *params = new CameraParameters();
	params->unflatten(camera->getParameters());

	switch (paramType) {
		case CAM_PARAM_KEY_PREVIEW_SIZE:
			ret = params->get(CameraParameters::KEY_PREVIEW_SIZE);
			//ALOGD("KEY_PREVIEW_SIZE=%s.",ret);
			break;
		case CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES:
			ret = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES);
			//ALOGD("KEY_SUPPORTED_PREVIEW_SIZES=%s.",ret);
			break;
		case CAM_PARAM_KEY_PREVIEW_FRAME_RATE:
			ret = params->get(CameraParameters::KEY_PREVIEW_FRAME_RATE);
			//ALOGD("KEY_PREVIEW_FRAME_RATE=%s.",ret);
			break;
		case CAM_PARAM_KEY_SUPPORTED_PREVIEW_FRAME_RATES:
			ret = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATES);
			//ALOGD("KEY_SUPPORTED_PREVIEW_FRAME_RATES=%s.",ret);
			break;
		default:
			break;
	}

	if ((paramType != (-1)) && ( ret != NULL) && (pToGoParams != NULL)) {
		strcpy(pToGoParams->latGetParam, ret);
		//memcpy(&(pToGoParams->latGetParam), ret, sizeof(ret));
		ret = pToGoParams->latGetParam;
		//ALOGD("ret=%s.latGetParam=%s.",ret,pToGoParams->latGetParam);
	}

	//if (paramType == CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES) {
	//	ret = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES);
	//	ALOGD("KEY_SUPPORTED_PREVIEW_SIZES=%s.",ret);
	//}

	if (paramType == (-1)) {
		// Get current value of parameters and its supported range.
		ALOGD("getParameters=%s.",(camera->getParameters()).string());

		int psw=0, psh=0;
		params->getPreviewSize(&psw, &psh);
		ALOGD("getPreviewSize.width=%d.height=%d.",psw,psh);

		Vector<Size> vSps;
		params->getSupportedPreviewSizes(vSps);
		ALOGD("getSupportedPreviewSizes.size=%d.",vSps.size());

		int vsw=0, vsh=0;
		params->getVideoSize(&vsw, &vsh);
		ALOGD("getVideoSize.width=%d.height=%d.",vsw,vsh);

		Vector<Size> vSvs;
		params->getSupportedVideoSizes(vSvs);	
		ALOGD("getSupportedVideoSizes.size=%d.",vSvs.size());

		int ppsvw=0, ppsvh=0;
		params->getPreferredPreviewSizeForVideo(&ppsvw, &ppsvh);
		ALOGD("getPreferredPreviewSizeForVideo.width=%d.height=%d.",ppsvw,ppsvh);

		ALOGD("getPreviewFrameRate=%d.",params->getPreviewFrameRate());

		int min_fps=0, max_fps=0;
		params->getPreviewFpsRange(&min_fps, &max_fps);
		ALOGD("getPreviewFpsRange.min_fps=%d.max_fps=%d.",min_fps,max_fps);

		ALOGD("getPreviewFormat=%s.",params->getPreviewFormat());

		int picw=0, pich=0;
		params->getPictureSize(&picw, &pich);
		ALOGD("getPictureSize.width=%d.height=%d.",picw,pich);

		Vector<Size> vSpics;
		params->getSupportedPictureSizes(vSpics);	
		ALOGD("getSupportedPictureSizes.size=%d.",vSpics.size());

		ALOGD("getPictureFormat=%s.",params->getPictureFormat());

		const char *preview_size = params->get(CameraParameters::KEY_PREVIEW_SIZE);
		ALOGD("KEY_PREVIEW_SIZE=%s.",preview_size);

		const char *supported_preview_size = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES);
		ALOGD("KEY_SUPPORTED_PREVIEW_SIZES=%s.",supported_preview_size);

		const char *preview_fps_range = params->get(CameraParameters::KEY_PREVIEW_FPS_RANGE);
		ALOGD("KEY_PREVIEW_FPS_RANGE=%s.",preview_fps_range);

		const char *supported_preview_fps_range = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FPS_RANGE);
		ALOGD("KEY_SUPPORTED_PREVIEW_FPS_RANGE=%s.",supported_preview_fps_range);

		const char *preview_format = params->get(CameraParameters::KEY_PREVIEW_FORMAT);
		ALOGD("KEY_PREVIEW_FORMAT=%s.",preview_format);

		const char *supported_preview_format = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS);
		ALOGD("KEY_SUPPORTED_PREVIEW_FORMATS=%s.",supported_preview_format);

		const char *preview_frame_rate = params->get(CameraParameters::KEY_PREVIEW_FRAME_RATE);
		ALOGD("KEY_PREVIEW_FRAME_RATE=%s.",preview_frame_rate);

		const char *supported_preview_frame_rate = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATES);
		ALOGD("KEY_SUPPORTED_PREVIEW_FRAME_RATES=%s.",supported_preview_frame_rate);

		const char *picture_size = params->get(CameraParameters::KEY_PICTURE_SIZE);
		ALOGD("KEY_PICTURE_SIZE=%s.",picture_size);

		const char *supported_picture_size = params->get(CameraParameters::KEY_SUPPORTED_PICTURE_SIZES);
		ALOGD("KEY_SUPPORTED_PICTURE_SIZES=%s.",supported_picture_size);

		const char *picture_format = params->get(CameraParameters::KEY_PICTURE_FORMAT);
		ALOGD("KEY_PICTURE_FORMAT=%s.",picture_format);

		const char *supported_picture_format = params->get(CameraParameters::KEY_SUPPORTED_PICTURE_FORMATS);
		ALOGD("KEY_SUPPORTED_PICTURE_FORMATS=%s.",supported_picture_format);

		const char *flash_mode = params->get(CameraParameters::KEY_FLASH_MODE);
		ALOGD("KEY_FLASH_MODE=%s.",flash_mode);

		const char *supported_flash_mode = params->get(CameraParameters::KEY_SUPPORTED_FLASH_MODES);
		ALOGD("KEY_SUPPORTED_FLASH_MODES=%s.",supported_flash_mode);

		const char *focus_mode = params->get(CameraParameters::KEY_FOCUS_MODE);
		ALOGD("KEY_FOCUS_MODE=%s.",focus_mode);

		const char *supported_focus_mode = params->get(CameraParameters::KEY_SUPPORTED_FOCUS_MODES);
		ALOGD("KEY_SUPPORTED_FOCUS_MODES=%s.",supported_focus_mode);

		const char *video_size = params->get(CameraParameters::KEY_VIDEO_SIZE);
		ALOGD("KEY_VIDEO_SIZE=%s.",video_size);

		const char *supported_video_size = params->get(CameraParameters::KEY_SUPPORTED_VIDEO_SIZES);
		ALOGD("KEY_SUPPORTED_VIDEO_SIZES=%s.",supported_video_size);

		const char *preferred_preview_size_for_video = params->get(CameraParameters::KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO);
		ALOGD("KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO=%s.",preferred_preview_size_for_video);

		const char *video_frame_format = params->get(CameraParameters::KEY_VIDEO_FRAME_FORMAT);
		ALOGD("KEY_VIDEO_FRAME_FORMAT=%s.",video_frame_format);

		const char *video_snapshot_supported = params->get(CameraParameters::KEY_VIDEO_SNAPSHOT_SUPPORTED);
		ALOGD("KEY_VIDEO_SNAPSHOT_SUPPORTED=%s.",video_snapshot_supported);

	}	

	delete params;
	return ret;
}

status_t RtkToGo::setCameraParameters(sp<Camera> camera, ToGoParams* pToGoParams)
{
	CameraParameters* params = new CameraParameters();
	params->unflatten(camera->getParameters());

	if (pToGoParams->bIsEnableH264Capture) {
		const char *support_preview_formats = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS);
		if ((support_preview_formats != NULL) && (strcasestr(support_preview_formats, CameraParameters::PIXEL_FORMAT_H264))) {
			ALOGD("Camera support H264.");
			params->set(CameraParameters::KEY_VIDEO_FRAME_FORMAT, CameraParameters::PIXEL_FORMAT_H264);
			ALOGD("Set H264 to video frame format.");
			pToGoParams->bH264Enabled = true;
		}
	}

	params->set(CameraParameters::KEY_PREVIEW_ENABLED, pToGoParams->bIsPreview ? CameraParameters::TRUE : CameraParameters::FALSE);
	params->setPreviewSize(pToGoParams->previewWidth, pToGoParams->previewHeight);
	params->setPreviewFrameRate(pToGoParams->previewFrameRate);

	if (camera->setParameters(params->flatten()) != OK) {
		delete params;
		ALOGE("setParameters NG.");
		return INVALID_OPERATION;
	}

	delete params;
	return OK;
}

//check StagefrightRecorder::setParameter() for detail
void RtkToGo::initMediaRecorderParameters(int cameraId, ToGoParams* pToGoParams, String8 &params)
{
	const size_t SIZE = 256;
	char buffer[SIZE];

	if (pToGoParams->duration) {
		snprintf(buffer, SIZE, "max-duration=%d;", pToGoParams->duration * 1000); //in ms
		params.append(buffer);
	}

	// muxer parameters
	snprintf(buffer, SIZE, "muxer-param-output-type=%d;", pToGoParams->outputType);
	params.append(buffer);
	if (pToGoParams->port) {
		snprintf(buffer, SIZE, "muxer-param-udp-ip=%s;", pToGoParams->ip);
		params.append(buffer);
		snprintf(buffer, SIZE, "muxer-param-udp-port=%d;", pToGoParams->port);
		params.append(buffer);
	}
	if (pToGoParams->bRtpOption) {
		snprintf(buffer, SIZE, "muxer-param-rtp-option=%d;", pToGoParams->bRtpOption);
		params.append(buffer);
	}

	// audio parameters
	snprintf(buffer, SIZE, "audio-param-sampling-rate=%d;", pToGoParams->audioSampleRate);
	params.append(buffer);
	snprintf(buffer, SIZE, "audio-param-number-of-channels=%d;", pToGoParams->audioChannelNum);
	params.append(buffer);
	snprintf(buffer, SIZE, "audio-param-encoding-bitrate=%d;", pToGoParams->abps);
	params.append(buffer);

	// video parameters
	snprintf(buffer, SIZE, "video-param-encoding-bitrate=%d;", pToGoParams->vbps);
	params.append(buffer);
	snprintf(buffer, SIZE, "video-param-i-frames-interval=%d;", pToGoParams->iFramesIntervalSeconds);
	params.append(buffer);

	//if (pToGoParams->profile != -1) {
	//    snprintf(buffer, SIZE, "video-param-encoder-profile=%d;", pToGoParams->profile);
	//    params.append(buffer);
	//}

	//if (m_level != -1){
	//    snprintf(buffer, SIZE, "video-param-encoder-level=%d;", m_level);
	//    params.append(buffer);
	//}
	if (pToGoParams->bH264Enabled) {
		snprintf(buffer, SIZE, "video-param-input-format=%d;", 1);
		params.append(buffer);
	}
	snprintf(buffer, SIZE, "video-param-camera-id=%d", cameraId);
	params.append(buffer);

	// the parameter string must not end with semicolon ';'
}

int RtkToGo::checkHandle(int handle)
{
	for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
		if ((m_togo[i].state >= TOGO_STATE_INITED) && (m_togo[i].handle == handle)) {
			return 0;
		}
	}
	ALOGE("checkHandle fail.handle=%d.",handle);
	return -1;
}

int RtkToGo::startToGoCamera(ToGoInstance* pToGo, int handle) 
{
	ALOGV("enter.TOGO_TYPE_CAMERA.");

	configCamera(pToGo);
	pToGo->camera->unlock();
/*
	if (pToGo->params.bHasAshmem && (!m_bIsRecBufServiceRunning)) {		
        RecordBufferService::instantiate();
        //Create binder threads for this "server"
        ProcessState::self()->startThreadPool();
        ALOGD("RecordBufferService Server is up and running.");
        m_bIsRecBufServiceRunning = true;
	}
*/
	if ((pToGo->params.bIsPreview == true) && (previewCamera(pToGo->camera, pToGo->gbp) != OK)) {
		ALOGE("previewCamera() NG.");
		return -1;
	}

	pToGo->recorder = new MediaRecorder(String16()); //FIXME: need to pass package name as parameter
	if (pToGo->recorder == NULL) {
		ALOGE("pToGo->recorder==NULL.");
		return -1;
	}

	if (pToGo->recorder->initCheck() != NO_ERROR) {
		ALOGE("initCheck() NG.");
		return -1;
	}

	pToGo->recorder->setListener(pToGo->listener);

	String8 params;
    initMediaRecorderParameters(pToGo->cameraId, &(pToGo->params), params);
    if (pToGo->recorder->setParameters(params) != OK) {
    	ALOGE("setParameters() NG.");
		return -1;
    }

    if (pToGo->recorder->setCamera(pToGo->camera->remote(), pToGo->camera->getRecordingProxy()) != OK) {
    	ALOGE("setCamera() NG.");
		return -1;
    }

    if (pToGo->recorder->setVideoSource(VIDEO_SOURCE_CAMERA) != OK) {
    	ALOGE("setVideoSource() NG.");
		return -1;
    }

    if (pToGo->recorder->setAudioSource(AUDIO_SOURCE_MIC) != OK) {
    	ALOGE("setAudioSource() NG.");
		return -1;
    }

    if ((pToGo->params.muxerFormat == MUXER_TS) && (pToGo->recorder->setOutputFormat(OUTPUT_FORMAT_MPEG2TS) != OK)) {
    	ALOGE("setOutputFormat(OUTPUT_FORMAT_MPEG2TS) NG.");
		return -1;
    }
    else if ((pToGo->params.muxerFormat == MUXER_MP4) && (pToGo->recorder->setOutputFormat(OUTPUT_FORMAT_MPEG_4) != OK)) {
    	ALOGE("setOutputFormat(OUTPUT_FORMAT_MPEG_4) NG.");
		return -1;
    }
    if ((pToGo->params.muxerFormat == MUXER_AAC) && (pToGo->recorder->setOutputFormat(OUTPUT_FORMAT_AAC_ADTS) != OK)) {
    	ALOGE("setOutputFormat(OUTPUT_FORMAT_AAC_ADTS) NG.");
		return -1;
    }


    //if (pToGo->recorder->setPreviewSurface(pToGo->surface->getIGraphicBufferProducer()) != OK) {
    if (pToGo->recorder->setPreviewSurface(pToGo->gbp) != OK) {
    	ALOGE("setPreviewSurface() NG.");
		return -1;
    }

    if (pToGo->recorder->setVideoSize(pToGo->params.previewWidth, pToGo->params.previewHeight) != OK) {
    	ALOGE("setVideoSize() NG.");
		return -1;
    }

    if (pToGo->recorder->setVideoFrameRate(pToGo->params.previewFrameRate) != OK) {
    	ALOGE("setVideoFrameRate() NG.");
		return -1;
    }

    if (pToGo->recorder->setVideoEncoder(VIDEO_ENCODER_H264) != OK) {
    	ALOGE("setVideoEncoder() NG.");
		return -1;
    }

    if (pToGo->recorder->setAudioEncoder(AUDIO_ENCODER_AAC) != OK) {
    	ALOGE("setAudioEncoder() NG.");
		return -1;
    }

	    if ((pToGo->params.outputType == TOGO_OUTPUT_FILE) && (pToGo->params.outputfd != (-1))) {
    	if (pToGo->recorder->setOutputFile(pToGo->params.outputfd, 0, 0) != OK) {
    		ALOGE("setOutputFile() NG.");
			return -1;
    	}
    }
    else if (pToGo->params.bHasAshmem) {
    	sp<IMemory> bufferData = NULL;
		sp<IMemory> bufferInfo = NULL;
		if (getRecordBuffer(bufferData, bufferInfo, handle) != 0) {
			ALOGE("getRecordBuffer() NG.");
			return -1;
		}
		ALOGV("handle=%d.bufferData=%p.bufferInfo=%p",handle,bufferData.get(),bufferInfo.get());
		if (pToGo->recorder->setOutputBuffer(bufferData, bufferInfo) != OK) {
			ALOGE("setOutputBuffer() NG.");
			return -1;
		}
    }

    if (pToGo->recorder->prepare() != OK) {
    	ALOGE("prepare() NG.");
		return -1;
    }

    if (pToGo->recorder->start() != OK) {
    	ALOGE("start() NG.");
    	if (pToGo->recorder->reset() == OK) {
    		ALOGE("reset() OK.");
    	}
    	//	reconnect is important and necessary, or when re-set params to camera, error happened:
    	//	W/CameraClient(  376): attempt to use a locked camera from a different process (old pid 0, new pid 664)
    	pToGo->camera->reconnect();
		return -1;
    }

	return OK;
}

int RtkToGo::startToGoFile(ToGoInstance* pToGo, int handle) 
{
	ALOGV("enter.TOGO_TYPE_FILE.");
/*
	if (pToGo->params.bHasAshmem && (!m_bIsRecBufServiceRunning)) { 	
		RecordBufferService::instantiate();
		//Create binder threads for this "server"
		ProcessState::self()->startThreadPool();
		ALOGD("RecordBufferService Server is up and running.");
		m_bIsRecBufServiceRunning = true;
	}
*/
	if (pToGo->params.bHasAshmem) {
		sp<IMemory> bufferData = NULL;
		sp<IMemory> bufferInfo = NULL;
		if (getRecordBuffer(pToGo->params.outputConfig.ashmemData, pToGo->params.outputConfig.ashmemInfo, handle) != 0) {
			ALOGE("getRecordBuffer() NG.");
			return -1;
		}
		ALOGV("handle=%d.ashmemData=%p.ashmemInfo=%p",handle,pToGo->params.outputConfig.ashmemData.get(),pToGo->params.outputConfig.ashmemInfo.get());
	}

#ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
	if (startToGoFileViaRTMediaPlayer(pToGo) < 0) {
		return -1;
	}
#else	// #ifdef TOGO_FILE_VIA_RTMEDIAPLAYER
	pToGo->player = new RtkTranscodePlayer();
	pToGo->looper = new ALooper();

	pToGo->looper->setName("PlayerLooper");
	ret = pToGo->looper->start();
	if (ret != OK) {
		ALOGE("PlayerLooper.start() NG.ret=%d.",ret);
		return -1;
	}
	pToGo->looper->registerHandler(pToGo->player);

	// new AHandler and ALooper to receive messages from RtkTranscodePlayer.
	pToGo->listenHandler = new ToGoHandler();
	pToGo->listenHandler->setToGoInstance(pToGo);
	pToGo->listenLooper = new ALooper();
	pToGo->listenLooper->setName("ToGoListenHandler");
	pToGo->listenLooper->registerHandler(pToGo->listenHandler);
	ret = pToGo->listenLooper->start();
	if (ret != OK) {
		ALOGE("ToGoListenHandler.start() NG.ret=%d.",ret);
		return -1;
	}

	// register id of pToGo->listenHandler to pToGo->player,
	// thus pToGo->listenHandler can receive messages from pToGo->player.
	pToGo->player->registerStatusHandler(pToGo->listenHandler);

	ret = pToGo->player->initiate(/*RtkTranscodePlayer::*/TranscodeTypeFile);
	if (ret != OK) {
		ALOGE("initiate(%d) NG.ret=%d.",/*RtkTranscodePlayer::*/TranscodeTypeFile,ret);
		return -1;
	}
	//strcpy(pToGo->params.targetFilename, "/storage/sda1/720p.mp4");
	//strcpy(pToGo->params.targetFilename, "/data/system/720p.mp4");
	ret = pToGo->player->setDataSource(pToGo->params.targetFilename);
	if (ret != OK) {
		ALOGE("setDataSource() NG.ret=%d.",ret);
		return -1;
	}
	ALOGV("setDataSource.targetFilename=%s.",pToGo->params.targetFilename);

	ret = pToGo->player->setSurface(pToGo->gbp);
	if (ret != OK) {
		ALOGE("setSurface() NG.ret=%d.",ret);
		return -1;
	}

	ret = pToGo->player->setPlayback(false, false);
	if (ret != OK) {
		ALOGE("setPlayback() NG.ret=%d.",ret);
		return -1;
	}

	ret = pToGo->player->setDecode();
	if (ret != OK) {
		ALOGE("setDecode() NG.ret=%d.",ret);
		return -1;
	}

	ret = pToGo->player->setOutputFormat(pToGo->params.outputConfig);
	if (ret != OK) {
		ALOGE("setOutputFormat() NG.ret=%d.",ret);
		return -1;
	}

	ret = pToGo->player->setTranscode(true);
	if (ret != OK) {
		ALOGE("setTranscode() NG.ret=%d.",ret);
		return -1;
	}

	ALOGV("leave.TOGO_TYPE_FILE.");
#endif	// #ifdef TOGO_FILE_VIA_RTMEDIAPLAYER

	return OK;
}

int RtkToGo::startToGoHdmiRx(ToGoInstance* pToGo, int handle) 
{
	ALOGV("enter.TOGO_TYPE_HDMI_RX.");
	status_t ret = OK;

	do {
		// init will connect camera just for test if HDMIRx is available 
		// and keep connected for getting camera parameters from CGI 
		if (pToGo->camera != NULL) {
			pToGo->camera->disconnect();
			pToGo->camera = NULL;
		}
			
		if (pToGo->params.bHasAshmem) {
			sp<IMemory> bufferData = NULL;
			sp<IMemory> bufferInfo = NULL;
			if (getRecordBuffer(pToGo->params.outputConfig.ashmemData, pToGo->params.outputConfig.ashmemInfo, handle) != 0) {
				ALOGE("getRecordBuffer() NG.");
				ret = -1;
				break;
			}
			ALOGV("handle=%d.ashmemData=%p.ashmemInfo=%p",handle,pToGo->params.outputConfig.ashmemData.get(),pToGo->params.outputConfig.ashmemInfo.get());
		}
	
		pToGo->player = new RtkTranscodePlayer();
		pToGo->looper = new ALooper();
		pToGo->looper->setName("PlayerLooper");
		ret = pToGo->looper->start();
		if (ret != OK) {
			ALOGE("PlayerLooper.start() NG.ret=%d.", ret);
			break;
		}
		pToGo->looper->registerHandler(pToGo->player);
	
		// new AHandler and ALooper to receive messages from RtkTranscodePlayer.
		pToGo->listenHandler = new ToGoHandler();
		pToGo->listenHandler->setToGoInstance(pToGo);
		pToGo->listenLooper = new ALooper();
		pToGo->listenLooper->setName("ToGoListenHandler");
		pToGo->listenLooper->registerHandler(pToGo->listenHandler);
		ret = pToGo->listenLooper->start();
		if (ret != OK) {
			ALOGE("ToGoListenHandler.start() NG.ret=%d.",ret);
			break;
		}
	
		// register id of pToGo->listenHandler to pToGo->player,
		// thus pToGo->listenHandler can receive messages from pToGo->player.
		pToGo->player->registerStatusHandler(pToGo->listenHandler);
	
		ret = pToGo->player->initiate(/*RtkTranscodePlayer::*/TranscodeTypeHdmiRx);
		if (ret != OK) {
			ALOGE("initiate(%d) NG.ret=%d.",/*RtkTranscodePlayer::*/TranscodeTypeHdmiRx, ret);
			break;
		}
	
		String16 clientName("com.realtek.app.togo");
		ret = pToGo->player->setCameraSource(pToGo->cameraId, clientName);
		if (ret != OK) {
			ALOGE("setCameraSource() NG.ret=%d.",ret);
			break;
		}
	
		ret = pToGo->player->setAudioSource(AUDIO_SOURCE_HDMIRX);
		if (ret != OK) {
			ALOGE("setAudioSource() NG.ret=%d.",ret);
			break;
		}
	
		ret = pToGo->player->setSurface(pToGo->gbp);
		if (ret != OK) {
			ALOGE("setSurface() NG.ret=%d.",ret);
			break;
		}
	
		ret = pToGo->player->setPlayback(false, false);
		if (ret != OK) {
			ALOGE("setPlayback() NG.ret=%d.",ret);
			break;
		}
	
	/*
		pToGo->params.targetFormat.videoFmt.width = 640;
		pToGo->params.targetFormat.videoFmt.height = 480;
		pToGo->params.targetFormat.videoFmt.bitrate = 1000000;
		pToGo->params.targetFormat.videoFmt.rateControl = 1;
	*/
		ALOGV("transcode player configureTargetFormat()");
		ret = pToGo->player->configureTargetFormat(pToGo->params.targetFormat);
		if (ret != OK) {
			ALOGE("configureTargetFormat() NG.ret=%d.",ret);
			break;
		}
		ALOGV("configureTargetFormat OK");
		
		ret = pToGo->player->setOutputFormat(pToGo->params.outputConfig);
		if (ret != OK) {
			ALOGE("setOutputFormat() NG.ret=%d.",ret);
			break;
		}

		ret = pToGo->player->setTranscode(true);
		if (ret != OK) {
			ALOGE("setTranscode() NG.ret=%d.",ret);
			break;
		}

		
	} while(0);

	if(ret != OK) {
		stopToGoHDMIRx(pToGo);
	}
	
	ALOGV("leave.TOGO_TYPE_HDMI_RX, ret=%d", ret);
	return ret;

}

void RtkToGo::stopToGoHDMIRx(ToGoInstance* pToGo)
{
	if (pToGo->camera != NULL) {
		
		pToGo->camera->disconnect();
		pToGo->camera = NULL;
	}

	if (pToGo->player != NULL) {
		pToGo->player->stop();
		if(pToGo->looper != NULL) {
			pToGo->looper->unregisterHandler(pToGo->player->id());
		}
		pToGo->player.clear();
		if(pToGo->looper != NULL) {
			pToGo->looper->stop();
			pToGo->looper = NULL;
		}
		// unregister and stop ToGo listen looper.
		if(pToGo->listenLooper != NULL) {
			pToGo->listenLooper->unregisterHandler(pToGo->listenHandler->id());
			pToGo->listenLooper->stop();
			pToGo->listenLooper = NULL;
		}
		pToGo->player = NULL;
	}
}


int RtkToGo::startToGoFileViaRTMediaPlayer(ToGoInstance* pToGo)
{
	ALOGV("enter.startToGoFileViaRTMediaPlayer.");
	status_t ret = OK;
	sp<RTMediaPlayer> rtMp = pToGo->rtMp;
	if (rtMp->setFocus(0) != NO_ERROR) {
		ALOGE("setFocus(0) NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	if (rtMp->setTranscodeMode(true) != NO_ERROR) {
		ALOGE("setTranscodeMode(true) NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	/* test output to file
	unlink("/storage/sda1/transcode.ts");
	pToGo->params.outputConfig.outputType = TOGO_OUTPUT_FD-100;
	pToGo->params.outputConfig.fileDesc = open("/storage/sda1/transcode.ts", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pToGo->params.outputConfig.fileDesc == (-1)) {
		ALOGE("open fileDesc NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}
	ALOGV("hardcode outputType=%d.fileDesc=%d.",pToGo->params.outputConfig.outputType,pToGo->params.outputConfig.fileDesc);
	*/
	if (rtMp->setOutputFormat(pToGo->params.outputConfig) != NO_ERROR) {
		ALOGE("setOutputFormat() NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	ALOGV("V:w=%d.h=%d.fps=%d.bps=%d.ifps=%d.rC=%d",pToGo->params.targetFormat.videoFmt.width,pToGo->params.targetFormat.videoFmt.height,pToGo->params.targetFormat.videoFmt.fps,
		pToGo->params.targetFormat.videoFmt.bitrate,pToGo->params.targetFormat.videoFmt.iframeInterval,pToGo->params.targetFormat.videoFmt.rateControl);
	ALOGV("A:ch=%d.sampleRate=%d.bitrate=%d.",pToGo->params.targetFormat.audioFmt.channelCount,pToGo->params.targetFormat.audioFmt.sampleRate,
		pToGo->params.targetFormat.audioFmt.bitrate);

	if (rtMp->configureTargetFormat(pToGo->params.targetFormat) != NO_ERROR) {
		ALOGE("configureTargetFormat() NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	// new AHandler and ALooper to receive messages from RTMediaPlayer.
	pToGo->listenHandler = new ToGoHandler();
	pToGo->listenHandler->setToGoInstance(pToGo);
	pToGo->listenLooper = new ALooper();
	pToGo->listenLooper->setName("ToGoListenHandler");
	pToGo->listenLooper->registerHandler(pToGo->listenHandler);
	ret = pToGo->listenLooper->start();
	if (ret != OK) {
		ALOGE("ToGoListenHandler.start() NG.ret=%d.",ret);
		stopToGoFileViaRTMediaPlayer(pToGo);
		return -1;
	}

	// register id of pToGo->listenHandler to pToGo->rtMp,
	// thus pToGo->listenHandler can receive messages from pToGo->rtMp.
    sp<AHandler> hdl = static_cast<AHandler *>(pToGo->listenHandler.get());
 	rtMp->registerStatusHandler(hdl);
	//rtMp->setTranscodeVOWriteback(TRANSCODE_VOWRITE_BACK_ENABLE | TRANSCODE_VOWRITE_BACK_INCLUDE_SUB);
	if ((ret = rtMp->startTranscode()) != NO_ERROR) {
		ALOGE("startTranscode() NG.ret=%d.",ret);
		stopToGoFileViaRTMediaPlayer(pToGo);
		// unregister and stop ToGo listen looper.
		//pToGo->listenLooper->unregisterHandler(pToGo->listenHandler->id());
		//pToGo->listenLooper->stop();
		//delete pToGo->rtMp;
		//pToGo->rtMp = NULL;
		return -1;
	}

	if (rtMp->start() != NO_ERROR) {
		ALOGE("start() NG.");
		stopToGoFileViaRTMediaPlayer(pToGo);
		//if (pToGo->rtMp->stopTranscode() != NO_ERROR) {
		//	ALOGE("stopTranscode() NG.");
		//}
		//if (pToGo->rtMp->stop() != NO_ERROR) {
		//	ALOGE("stop() NG.");
		//}
		//if (pToGo->rtMp->reset() != NO_ERROR) {
		//	ALOGE("reset() NG.");
		//}
		// unregister and stop ToGo listen looper.
		//pToGo->listenLooper->unregisterHandler(pToGo->listenHandler->id());
		//pToGo->listenLooper->stop();
		//delete pToGo->rtMp;
		//pToGo->rtMp = NULL;
		return -1;
	}

/*
	// subtitle test
	// setSubtitleInfo(int streamNum,int enable,int textEncoding,int textColor,int fontSize,int syncTime,int offset);
	Parcel trackInfo, request;
	int count = 0;
	request.writeInt32(INVOKE_ID_GET_TRACK_INFO);
	request.setDataPosition(0);
	//rtMp->getTrackInfo(&trackInfo);
	rtMp->invoke(request, &trackInfo);
	trackInfo.setDataPosition(0);
	trackInfo.readInt32(&count);
	ALOGV("getTrackInfo: trackCount=%d", count);
	rtMp->setSubtitleInfo(2, 1, 0, 0, 24, 0, 0);
*/
	ALOGV("leave.startToGoFileViaRTMediaPlayer.");
	return 0;
}

int RtkToGo::stopToGoFileViaRTMediaPlayer(ToGoInstance* pToGo)
{
	ALOGV("enter.stopToGoFileViaRTMediaPlayer.");
	if (pToGo->rtMp != NULL) {
		// stopTranscode() is called in RTMediaPlayer::stop().
		//if (pToGo->rtMp->stopTranscode() != NO_ERROR) {
		//	ALOGE("stopTranscode() NG.");
		//}
		if (pToGo->rtMp->stop() != NO_ERROR) {
			ALOGE("stop() NG.");
		}

		if (pToGo->rtMp->setFocus(1) != NO_ERROR) {
			ALOGE("setFocus(1) NG.");
		}

		if (pToGo->rtMp->reset() != NO_ERROR) {
			ALOGE("reset() NG.");
		}
		// unregister and stop ToGo listen looper.
		if (pToGo->listenLooper != NULL) {
			pToGo->listenLooper->unregisterHandler(pToGo->listenHandler->id());
			pToGo->listenLooper->stop();
		}

		//delete pToGo->rtMp;
		//pToGo->rtMp = NULL;
		pToGo->rtMp.clear();
	}

	ALOGV("leave.stopToGoFileViaRTMediaPlayer.");
	return 0;
}

int RtkToGo::startToGoDtv(ToGoInstance* pToGo)
{
	ALOGV("enter.startToGoDtv.");
#if 0
	status_t ret = OK;

	pToGo->rtDtv = new RTDtvMediaPlayer();

	if (pToGo->rtDtv->initCheck() != NO_ERROR) {
		ALOGE("initCheck() NG.");
		stopToGoDtv(pToGo);
		return -1;
	}

	if (pToGo->rtDtv->setDataSource("stb://0", NULL) != NO_ERROR) {
		ALOGE("setDataSource() NG.targetFilename=%s.","stb://0");
		stopToGoDtv(pToGo);
		return -1;
	}
/*
	if (pToGo->rtDtv->prepareAsync() != NO_ERROR) {
		ALOGE("prepareAsync() NG.");
		stopToGoDtv(pToGo);
		return -1;
	}
*/
	if (pToGo->rtDtv->setVideoSurfaceTexture(pToGo->gbp) != NO_ERROR) {
		ALOGE("setVideoSurfaceTexture() NG.");
		stopToGoDtv(pToGo);
		return -1;
	}
/*
	int checkCount = 0;
	bool checkOK = false;
	while (checkCount < 10) {
		int ret = pToGo->rtDtv->checkLoadMedia();
		if (ret == 2) {
			ALOGE("checkLoadMedia() NG.");
			stopToGoDtv(pToGo);
			return -1;
		}
		else if (ret == 1) {
			checkOK = true;
			break;
		}
		checkCount++;
		usleep(200000);		
	}
	if (checkOK == false) {
		ALOGE("checkLoadMedia() NG.checkCount=%d.",checkCount);
		stopToGoDtv(pToGo);
		return -1;
	}
	ALOGE("checkLoadMedia() OK.checkCount=%d.",checkCount);
*/
#endif	// #if 0

#ifndef ENABLE_1195_DTV_FLOW
	if (pToGo->rtDtv->setFocus(0) != NO_ERROR) {
		ALOGE("setFocus(0) NG.");
		stopToGoDtv(pToGo);
		return -1;
	}

	if (pToGo->rtDtv->setTranscodeMode(true) != NO_ERROR) {
		ALOGE("setTranscodeMode(true) NG.");
		stopToGoDtv(pToGo);
		return -1;
	}

	/* test output to file
	unlink("/storage/sda1/transcode.ts");
	pToGo->params.outputConfig.outputType = TOGO_OUTPUT_FD-100;
	pToGo->params.outputConfig.fileDesc = open("/storage/sda1/transcode.ts", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pToGo->params.outputConfig.fileDesc == (-1)) {
		ALOGE("open fileDesc NG.");
		stopToGoDtv(pToGo);
		return -1;
	}
	ALOGV("hardcode outputType=%d.fileDesc=%d.",pToGo->params.outputConfig.outputType,pToGo->params.outputConfig.fileDesc);
	*/
	if (pToGo->rtDtv->setOutputFormat(pToGo->params.outputConfig) != NO_ERROR) {
		ALOGE("setOutputFormat() NG.");
		stopToGoDtv(pToGo);
		return -1;
	}

	ALOGV("V:w=%d.h=%d.fps=%d.bps=%d.ifps=%d.rC=%d.",pToGo->params.targetFormat.videoFmt.width,pToGo->params.targetFormat.videoFmt.height,pToGo->params.targetFormat.videoFmt.fps,pToGo->params.targetFormat.videoFmt.bitrate,pToGo->params.targetFormat.videoFmt.iframeInterval,pToGo->params.targetFormat.videoFmt.rateControl);
	ALOGV("A:ch=%d.sampleRate=%d.bitrate=%d.",pToGo->params.targetFormat.audioFmt.channelCount,pToGo->params.targetFormat.audioFmt.sampleRate,
		pToGo->params.targetFormat.audioFmt.bitrate);

	if (pToGo->rtDtv->configureTargetFormat(pToGo->params.targetFormat) != NO_ERROR) {
		ALOGE("configureTargetFormat() NG.");
		stopToGoDtv(pToGo);
		return -1;
	}

	// new AHandler and ALooper to receive messages from RTMediaPlayer.
	pToGo->dtvListenHandler = new ToGoHandler();
	pToGo->dtvListenHandler->setToGoInstance(pToGo);
	pToGo->dtvListenLooper = new ALooper();
	pToGo->dtvListenLooper->setName("ToGoListenHandler");
	pToGo->dtvListenLooper->registerHandler(pToGo->dtvListenHandler);
	ret = pToGo->dtvListenLooper->start();
	if (ret != OK) {
		ALOGE("ToGoListenHandler.start() NG.ret=%d.",ret);
		stopToGoDtv(pToGo);
		return -1;
	}

	// register id of pToGo->dtvListenHandler to pToGo->rtDtv,
	// thus pToGo->dtvListenHandler can receive messages from pToGo->rtDtv.
	pToGo->rtDtv->registerStatusHandler(pToGo->dtvListenHandler);

	if ((ret = pToGo->rtDtv->startTranscode()) != NO_ERROR) {
		ALOGE("startTranscode() NG.ret=%d.",ret);
		stopToGoDtv(pToGo);
		return -1;
	}

	if (pToGo->rtDtv->start() != NO_ERROR) {
		ALOGE("start() NG.");
		stopToGoDtv(pToGo);
		return -1;
	}
#endif	//	#ifndef ENABLE_1195_DTV_FLOW

	ALOGV("leave.startToGoDtv.");
	return 0;
}



int RtkToGo::stopToGoDtv(ToGoInstance* pToGo)
{
	ALOGV("enter.stopToGoDtv.");
	if (pToGo->rtDtv != NULL) {
		// stopTranscode() is called in RTMediaPlayer::stop().
		//if (pToGo->rtDtv->stopTranscode() != NO_ERROR) {
		//	ALOGE("stopTranscode() NG.");
		//}
		if (pToGo->rtDtv->stop() != NO_ERROR) {
			ALOGE("stop() NG.");
		}

		if (pToGo->rtDtv->setFocus(1) != NO_ERROR) {
			ALOGE("setFocus(1) NG.");
		}
		
		if (pToGo->rtDtv->reset() != NO_ERROR) {
			ALOGE("reset() NG.");
		}
	}

	if (pToGo->cameraId != -1) {
		pToGo->tunerAppType = pToGo->tvProxy->UnInit(pToGo->cameraId, APP_TOGO);
		if (pToGo->tunerAppType < 0) {
			ALOGE("UnInit(0) NG.tunerId=%d.",pToGo->cameraId);
		}
		pToGo->cameraId = -1;
	}

		// unregister and stop ToGo listen looper.
		if (pToGo->dtvListenLooper != NULL) {
			pToGo->dtvListenLooper->unregisterHandler(pToGo->dtvListenHandler->id());
			pToGo->dtvListenLooper->stop();	
		}

	if (pToGo->tvProxy) {
		delete pToGo->tvProxy;
		pToGo->tvProxy = NULL;
	}

	if (pToGo->rtDtv) {
		delete pToGo->rtDtv;
		pToGo->rtDtv = NULL;
	}

	ALOGV("leave.stopToGoDtv.");
	return 0;
}

