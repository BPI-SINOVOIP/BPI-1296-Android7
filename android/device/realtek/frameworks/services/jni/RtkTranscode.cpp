#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utils/Log.h>
#include <ui/GraphicBuffer.h>
#include <ui/DisplayInfo.h>
#include <gui/Surface.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <media/mediarecorder.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/AudioPlayer.h>
#include <media/stagefright/AudioSource.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MPEG4Writer.h>
#include <media/stagefright/MPEG2TSWriter.h>
#include <media/stagefright/AACWriter.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/OMXCodec.h>
#include <media/stagefright/CameraSource.h>
#include <media/MediaPlayerInterface.h>
#include <media/IRecordBuffer.h>
#include "RtkTranscode.h"

#define LOG_TAG	"RtkTranscode"

#if 0
namespace android {

//#define RECORD_DATA_SIZE	  188*1024
#define RECORD_DATA_SIZE    (1500 + 4)* 200
#define RECORD_INFO_SIZE    32
#define MEMORY_SIZE (RECORD_DATA_SIZE + RECORD_INFO_SIZE)
	
class RecordBufferService : public BnRecordBuffer {
	public:
		static void instantiate();
		RecordBufferService();
		virtual ~RecordBufferService();
		virtual sp<IMemory> getBuffer(int id);
	private:
		sp<MemoryHeapBase> mMemHeap;
		sp<MemoryBase> mBufferData;
		sp<MemoryBase> mBufferInfo;
};
	
sp<IMemory> RecordBufferService::getBuffer(int id)
{
	switch(id){
		case RECBUF_TYPE_DATA:
			return mBufferData;
		case RECBUF_TYPE_INFO:
			return mBufferInfo;
		default:
			return NULL;
	}
}
	
void RecordBufferService::instantiate()
{
	status_t status;
	status = defaultServiceManager()->addService(String16("vendor.realtek.RecordBuffer"), new RecordBufferService());
}
	
RecordBufferService::RecordBufferService()
{
	//The memory is allocated using a MemoryHeapBase, and thereby is using ashmem
	mMemHeap = new MemoryHeapBase(MEMORY_SIZE);
	mBufferData = new MemoryBase(mMemHeap, 0, RECORD_DATA_SIZE);
	mBufferInfo = new MemoryBase(mMemHeap, RECORD_DATA_SIZE, RECORD_INFO_SIZE);
}
	
RecordBufferService::~RecordBufferService()
{
	mMemHeap = NULL;
	mBufferData = NULL;
	mBufferInfo = NULL;
}
	
int getRecordBuffer(sp<IMemory> *data, sp<IMemory> *info)
{
	static sp<IRecordBuffer> recordBuffer;
	
	/* Get the buffer service */
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
		return -1; /* return an errorcode... */
	}
	else
	{
		*data = recordBuffer->getBuffer(RECBUF_TYPE_DATA);
		*info = recordBuffer->getBuffer(RECBUF_TYPE_INFO);
		return 0;
	}
}

}	// namespace android
#endif


using namespace android;
/*
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
*/
RtkTranscode* RtkTranscode::m_instance = NULL;
int RtkTranscode::m_refCount = 0;

RtkTranscode::RtkTranscode()
{
	m_bUseMediaRecorder = true;
	m_muxerFormat = 0;
	m_outputType = 0;
	m_outputFilename = NULL;
	m_outputFD = -1;
	m_duration = 0;
	m_udpIp = NULL;
	m_udpPort = 0;
	m_bHasVideo = false;
	m_vbps = 800000;
	m_colorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	m_vfps = 5;
	m_iFramesIntervalSeconds = 1;
	m_nFrames = 300;
	m_width = 320;
	m_height = 240;
	m_videoCodec = 0;
	m_bPreferSoftwareCodec = false;
	m_bEncTest = false;
	m_previewTime = 0;
	m_cameraId = 0;
	m_bStoreMetaDataInVideoBuffers = false;
	m_bEnableH264Capture = false;
	m_bHasAudio = false;
	m_audioChannelNum = 1;
	m_audioSampleRate = 16000;
	m_abps = 64000;
	m_bRtpOption = false;
	m_bHasAshmem = false;
	m_bH264Enabled = false;
	m_level = -1;
	m_profile = -1;
	m_quit = 0;
	m_state = STATE_IDLE;
	sp<MediaRecorder> mr = NULL;
	sp<Camera> camera = NULL;
}

RtkTranscode::~RtkTranscode()
{
}

RtkTranscode* RtkTranscode::GetInstance()
{
	if (NULL == m_instance) {
		m_instance = new RtkTranscode();
		m_refCount++;
	}
	return m_instance;
}

void RtkTranscode::ReleaseInstance()
{
	m_refCount--;
	if (0 == m_refCount) {
		delete m_instance;
		m_instance = NULL;
	}
}

bool RtkTranscode::Init()
{
	ALOGV("enter Init.\n");
	
	if (m_state != STATE_IDLE) {
		ALOGE("m_state=%d ISNOT STATE_IDLE!\n", m_state);
		return false;
	}
	m_state = STATE_INIT;
	return true;
}

void RtkTranscode::UnInit()
{
	ALOGV("enter UnInit.\n");
	m_state = STATE_IDLE;
}

void RtkTranscode::SetUseMediaRecorder(bool isEnable)
{
	m_bUseMediaRecorder = isEnable;
}

void RtkTranscode::SetMuxerFormat(int muxerFormat)
{
	m_muxerFormat = muxerFormat;
}

void RtkTranscode::SetOutputType(int outputType)
{
	m_outputType = outputType;
}

void RtkTranscode::SetOutputFilename(char* outputFilename)
{
	m_outputFilename = outputFilename;
}

void RtkTranscode::SetOutputFD(int fd)
{
	m_outputFD = fd;
}

void RtkTranscode::SetDuration(int duration)
{
	m_duration = duration;
}

void RtkTranscode::SetUdpIpPort(char* ip, int port)
{
	m_udpIp = ip;
	m_udpPort = port;
}

void RtkTranscode::SetOutputVideoStream(bool isOutputVideoStream)
{
	m_bHasVideo = isOutputVideoStream;
}

void RtkTranscode::SetVideoBitrate(int vbps)
{
	m_vbps = vbps;
}

void RtkTranscode::SetColorFormat(int colorFormat)
{
	m_colorFormat = colorFormat;
}

void RtkTranscode::SetVideoFrameRate(int vfps)
{
	m_vfps = vfps;
}

void RtkTranscode::SetIFrameIntervalSeconds(int iFramesIntervalSeconds)
{
	m_iFramesIntervalSeconds = iFramesIntervalSeconds;
}

void RtkTranscode::SetVideoFramesToBeRecorded(int nFrames)
{
	m_nFrames = nFrames;
}

void RtkTranscode::SetVideoWidthHeight(int width, int height)
{
	m_width = width;
	m_height = height;
}

void RtkTranscode::SetVideoCodec(int videoCodec)
{
	m_videoCodec = videoCodec;
}

void RtkTranscode::SetPreferSoftwareCodec(bool isPreferSoftwareCodec)
{
	m_bPreferSoftwareCodec = isPreferSoftwareCodec;
}

void RtkTranscode::SetEncTest(bool isEncTest)
{
	m_bEncTest = isEncTest;
}

void RtkTranscode::SetPreviewTimeBeforeEncoding(int previewTime)
{
	m_previewTime = previewTime;
}

void RtkTranscode::SetPreview(bool isPreview)
{
	m_bHasPreview = isPreview;
}

void RtkTranscode::SetCameraId(int cameraId)
{
	m_cameraId = cameraId;
}

void RtkTranscode::SetStoreMetaDataInVideoBuffers(bool isStoreMetaDataInVideoBuffers)
{
	m_bStoreMetaDataInVideoBuffers = isStoreMetaDataInVideoBuffers;
}

void RtkTranscode::SetEnableH264Capture(bool isEnableH264Capture)
{
	m_bEnableH264Capture = isEnableH264Capture;
}

void RtkTranscode::SetOutputAudioStream(bool isOutputAudioStream)
{
	m_bHasAudio = isOutputAudioStream;
}

void RtkTranscode::SetAudioChannelCount(int audioChannelNum)
{
	m_audioChannelNum = audioChannelNum;
}

void RtkTranscode::SetAudioSampleRate(int audioSampleRate)
{
	m_audioSampleRate = audioSampleRate;
}

void RtkTranscode::SetAudioBitrate(int abps)
{
	m_abps = abps;
}

bool RtkTranscode::Start()
{
	ALOGV("enter Start.\n");
	
	if (!m_bHasVideo && !m_bHasAudio) {
		ALOGE("No audio or video specified\n");
		return false;
	}

	if ((MUXER_AAC == m_muxerFormat) && (!m_bHasAudio || m_bHasVideo)) {
		ALOGE("Muxer AAC must be audio only\n");
		return false;
	}

	if ((OUTPUT_FILE == m_outputType) && (NULL == m_outputFilename) && (NULL == m_outputFD)) {
		ALOGE("file name must be specified\n");
		return false;
	}

	if ((OUTPUT_UDP==m_outputType || OUTPUT_UDP_RTP==m_outputType || OUTPUT_RTP_BOTH==m_outputType) &&
		(NULL==m_udpIp || 0==m_udpPort)) {
		 ALOGE("udp ip and port must be specified\n");
		 return false;
	}

	if (OUTPUT_ASHMEM_RTP==m_outputType || OUTPUT_UDP_RTP==m_outputType || OUTPUT_RTP_BOTH==m_outputType) {
		m_bRtpOption = true;
	}

	if (OUTPUT_ASHMEM==m_outputType || OUTPUT_ASHMEM_RTP==m_outputType || OUTPUT_RTP_BOTH==m_outputType) {
		m_bHasAshmem = true;
#if 0
        RecordBufferService::instantiate();
        //Create binder threads for this "server"
        ProcessState::self()->startThreadPool();
        ALOGD("RecordBufferService Server is up and running");
#endif
	}

	if (m_bUseMediaRecorder) {
		startMediaRecorder();
		ALOGI("leave Start 1.\n");
		return true;
	}

	OMXClient client;
	CHECK_EQ(client.connect(), (status_t)OK);

	// Video ================================================================
    sp<Surface> surface = NULL;
    sp<SurfaceControl> control;
    //sp<Camera> camera;
	sp<CameraSource> cameraSource = NULL;
    sp<MediaSource> videoEncoder = NULL;

    if (m_bHasVideo) {
    	CHECK_EQ(setupSurface(&control, &surface), (status_t)OK);
    	CHECK_EQ(setupCamera(&camera), (status_t)OK);

    	if (m_bHasPreview && m_previewTime && !m_bH264Enabled) {
    		previewCamera(surface, camera);
    	}

    	CHECK_EQ(setupVideoSource(surface, camera, &cameraSource), (status_t)OK);

    	if (!m_bH264Enabled) {
    		CHECK_EQ(setupVideoEncoder(client, cameraSource, &videoEncoder), (status_t)OK);
    	}
    }

    // Audio ================================================================
    sp<AudioSource> audioSource = NULL;
    sp<MediaSource> audioEncoder = NULL;

    if (m_bHasAudio) {
    	CHECK_EQ(setupAudioSource(&audioSource), (status_t)OK);
    	CHECK_EQ(setupAudioEncoder(client, audioSource, &audioEncoder), (status_t)OK);    	
    }

    // Start testing ================================================================
    if (m_bHasVideo && m_bEncTest) {
    	startVideoEncoderTest(videoEncoder);
    }
    else {
    	if (m_bH264Enabled) {
    		sp<MediaSource> videoSource = cameraSource;
    		startMuxerTest(videoSource, audioEncoder);
    	}
    	else {
    		startMuxerTest(videoEncoder, audioEncoder);
    	}
    }

    client.disconnect();

    if (m_bHasVideo) {
    	camera->disconnect();
    }

	ALOGI("leave Start 2.\n");
	return true;
}

bool RtkTranscode::Stop()
{
	ALOGV("enter Stop.\n");
	//m_quit = 1;

	if (m_bUseMediaRecorder) {
		status_t err = mr->stop();
		
		if (err != OK) {
			ALOGE("record failed: %d\n", err);
			return false;
		}
		
		if (m_bHasVideo) {
			camera->disconnect();
		}
	}

	return true;
}

status_t RtkTranscode::setupSurface(sp<SurfaceControl> *control, sp<Surface> *surface)
{
	ALOGV("setupSurface");

	// setup surface
	sp<SurfaceComposerClient> composerClient = new SurfaceComposerClient;
	CHECK_EQ(composerClient->initCheck(), (status_t)OK);

	*control = composerClient->createSurface(String8("CamSurface"), m_width, m_height, PIXEL_FORMAT_RGB_565, 0);
	CHECK(*control != NULL);
	CHECK((*control)->isValid());

	SurfaceComposerClient::openGlobalTransaction();
	CHECK_EQ((*control)->setLayer(INT_MAX), (status_t)OK);
	CHECK_EQ((*control)->show(), (status_t)OK);
	SurfaceComposerClient::closeGlobalTransaction();

	*surface = (*control)->getSurface();
	CHECK(*surface != NULL);

	return OK;
}

status_t RtkTranscode::setupCamera(sp<Camera> *camera)
{
	ALOGV("setupCamera");

	*camera = Camera::connect(m_cameraId);
	CHECK(*camera != NULL);

	CameraParameters *params = new CameraParameters();
	params->unflatten((*camera)->getParameters());

	if (m_bEnableH264Capture) {
		const char *supportedPreviewFormats = params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS);
		CHECK(supportedPreviewFormats != NULL);
		if (strcasestr(supportedPreviewFormats, CameraParameters::PIXEL_FORMAT_H264))
		{
			ALOGI("=== Camera supports H264 format ===");
			/* set VIDEO_FRAME_FORMAT to camera */
			params->set(CameraParameters::KEY_VIDEO_FRAME_FORMAT, CameraParameters::PIXEL_FORMAT_H264);
			m_bH264Enabled = true;
			ALOGI("=== set Camera preview format to H264 ===");
		}		
	}

	params->setPreviewSize(m_width, m_height);
	params->setPreviewFrameRate(m_vfps);
	params->set(CameraParameters::KEY_PREVIEW_ENABLED, m_bHasPreview ? CameraParameters::TRUE : CameraParameters::FALSE);

	CHECK_EQ((*camera)->setParameters(params->flatten()), (status_t)OK);

	(*camera)->unlock(); //unlock camera, so the other process (ex. StagefrightRecorder@mediaserver) can use it
	return OK;
}

void RtkTranscode::previewCamera(sp<Surface> &surface, sp<Camera> &camera)
{
    camera->lock();
    camera->setPreviewDisplay(surface);
    camera->startPreview();
    sleep(m_previewTime); //preview test
    camera->stopPreview();
    camera->unlock();
}

status_t RtkTranscode::setupVideoSource(sp<Surface> &surface, sp<Camera> &camera, sp<CameraSource> *cameraSource)
{
	Size videoSize;
	videoSize.width = m_width;
	videoSize.height = m_height;

	*cameraSource = CameraSource::CreateFromCamera(camera->remote(), camera->getRecordingProxy(), m_cameraId, videoSize, m_vfps, surface, m_bStoreMetaDataInVideoBuffers);

	if(*cameraSource == NULL){
		printf("CreateFromCamera failed\n");
		return UNKNOWN_ERROR;
	}

	if ((*cameraSource)->initCheck() != OK) {
		(*cameraSource).clear();
		return NO_INIT;
	}

	return OK;
}

status_t RtkTranscode::setupVideoEncoder(OMXClient &client, sp<CameraSource> &cameraSource, sp<MediaSource> *videoEncoder)
{
    ALOGV("setupVideoEncoder");

	// setup encoder
	sp<MetaData> enc_meta = new MetaData;
	switch (m_videoCodec) {
		case 1:
			enc_meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG4);
			break;
		case 2:
			enc_meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_H263);
			break;
		default:
			enc_meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
			break;
	}
	enc_meta->setInt32(kKeyWidth, m_width);
	enc_meta->setInt32(kKeyHeight, m_height);
	enc_meta->setInt32(kKeyFrameRate, m_vfps);
	enc_meta->setInt32(kKeyBitRate, m_vbps);
	enc_meta->setInt32(kKeyStride, m_width);
	enc_meta->setInt32(kKeySliceHeight, m_height);
	enc_meta->setInt32(kKeyIFramesInterval, m_iFramesIntervalSeconds);
	enc_meta->setInt32(kKeyColorFormat, m_colorFormat);
	
	if (m_level != -1) {
		enc_meta->setInt32(kKeyVideoLevel, m_level);
	}
	if (m_profile != -1) {
		enc_meta->setInt32(kKeyVideoProfile, m_profile);
	}
	
	uint32_t flags = 0;
	if (m_bPreferSoftwareCodec) {
		flags |= OMXCodec::kPreferSoftwareCodecs;
	}
	if (cameraSource->isMetaDataStoredInVideoBuffers()) {	//make sure metadata store in video buffers is enabled
		flags |= OMXCodec::kStoreMetaDataInVideoBuffers;
	}
	ALOGD("camera %ssupport storing meta data in video buffers", cameraSource->isMetaDataStoredInVideoBuffers() ? "" : "doesn't ");
	*videoEncoder = OMXCodec::Create(client.interface(), enc_meta, true /* createEncoder */, cameraSource, 0, flags);
	
	if (*videoEncoder == NULL) {
		ALOGE("create video encoder failed");
		return UNKNOWN_ERROR;
	}

	return OK;
}

status_t RtkTranscode::setupAudioSource(sp<AudioSource> *audioSource)
{
    ALOGV("setupAudioSource");

	*audioSource = new AudioSource(AUDIO_SOURCE_DEFAULT, m_audioSampleRate, m_audioChannelNum);
	CHECK_EQ((*audioSource)->initCheck(), (status_t)OK);

	return OK;
}

status_t RtkTranscode::setupAudioEncoder(OMXClient &client, sp<AudioSource> &audioSource, sp<MediaSource> *audioEncoder)
{
	ALOGV("setupAudioEncoder");

	sp<MetaData> encMeta = new MetaData;
	encMeta->setInt32(kKeyAACProfile, OMX_AUDIO_AACObjectLC);
	encMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AAC);

	int32_t maxInputSize;
	CHECK(audioSource->getFormat()->findInt32(kKeyMaxInputSize, &maxInputSize));

	encMeta->setInt32(kKeyMaxInputSize, maxInputSize);
	encMeta->setInt32(kKeyChannelCount, m_audioChannelNum);
	encMeta->setInt32(kKeySampleRate, m_audioSampleRate);
	encMeta->setInt32(kKeyBitRate, m_abps);

	*audioEncoder = OMXCodec::Create(client.interface(), encMeta, true /* createEncoder */, audioSource);
	CHECK(*audioEncoder != NULL);

	return OK;
}

status_t RtkTranscode::startVideoEncoderTest(sp<MediaSource> &videoEncoder)
{
	ALOGV("startVideoEncoderTest");

	// OMXCodec test ================================================================
	CHECK_EQ(videoEncoder->start(), (status_t)OK);
	
	sp<MetaData> _meta = videoEncoder->getFormat();
	//_meta->dumpToLog();
	const char *mime;
	CHECK(_meta->findCString(kKeyMIMEType, &mime));
	if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
		uint32_t type;
		const void *data;
		size_t size;
		if (!_meta->findData(kKeyAVCC, &type, &data, &size)) {
			// Codec specific data better be part of the data stream then.
			ALOGD("Can't find AVC codec specific data");
		} else {
			ALOGD("AVC codec specific data found");
		}
	}
	
	int gotSPSPPS = 0;
	MediaBuffer *buffer;
	status_t err;
	const uint8_t prefix[] = { 0x00, 0x00, 0x00, 0x01 };
	int fd = open(m_outputFilename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	nsecs_t start = systemTime();

	while (!m_quit) {		
		if (m_duration && systemTime() - start < m_duration * 1000000000) {
			break;
		}
		err = videoEncoder->read(&buffer);
		if (err != OK) {
			ALOGD("Error happens:%d", err);
			break;
		}
		const uint8_t *dd = (const uint8_t *)buffer->data();
		ALOGV("reading %d bytes to buffer %x(%02x) offset %x", buffer->range_length(), (unsigned int)buffer->data(), dd[0], buffer->range_offset());
		if (fd >= 0) {
			if (gotSPSPPS) {
				write(fd, prefix, 4);
			}
			gotSPSPPS = 1;
			write(fd, (const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
		}
		buffer->release();
	}

	nsecs_t end = systemTime();
	ALOGI("totoal record time: %fsec\n", (end - start) / 1000000000.0);
	//ALOGI("encoding %d frames in %lld us\n", nFrames, (end-start)/1000);
	//ALOGI("encoding speed is: %.2f fps\n", (nFrames * 1E9) / (end-start));

	return OK;
}

status_t RtkTranscode::startMuxerTest(sp<MediaSource> &videoSource, sp<MediaSource> &audioSource)
{
	ALOGV("startMuxerTest");
	
    // Muxer test ================================================================
	sp<MediaWriter> writer;
	switch (m_muxerFormat) {
		case MUXER_TS:
		{
			sp<IMemory> recData = NULL;
			sp<IMemory> recInfo = NULL;
			if (m_bHasAshmem) {
				CHECK(getRecordBuffer(&recData, &recInfo) == 0);
			}
			if (m_outputType == OUTPUT_FILE) {
				writer = new MPEG2TSWriter(m_outputFilename);
			}
			else if (m_outputType == OUTPUT_STDOUT) {
				writer = new MPEG2TSWriter(stdout);
			}
			else if (m_outputType == OUTPUT_UDP) {
				writer = new MPEG2TSWriter(m_udpPort, m_udpIp, m_bRtpOption);
			}
			else if (m_outputType == OUTPUT_UDP_RTP) {
				writer = new MPEG2TSWriter(m_udpPort, m_udpIp, m_bRtpOption);
			}
			else if (m_outputType == OUTPUT_ASHMEM) {
				writer = new MPEG2TSWriter(recData, recInfo, m_bRtpOption);
			}
			else if (m_outputType == OUTPUT_ASHMEM_RTP) {
				writer = new MPEG2TSWriter(recData, recInfo, m_bRtpOption);
			}
			else if (m_outputType == OUTPUT_RTP_BOTH) {
				writer = new MPEG2TSWriter(recData, recInfo, m_bRtpOption, m_udpPort, m_udpIp);
			}
			break;
		}
		case MUXER_MP4:
			writer = new MPEG4Writer(m_outputFilename);
			break;
		case MUXER_AAC:
			writer = new AACWriter(m_outputFilename);
			break;
		default:
			ALOGE("Invalid muxer format:%d!\n", m_muxerFormat);
			return UNKNOWN_ERROR;
	}

	if (m_bHasVideo && videoSource != NULL) {
		writer->addSource(videoSource);
	}

	if (m_bHasAudio && audioSource != NULL) {
		writer->addSource(audioSource);
	}

	nsecs_t start = systemTime();
	CHECK_EQ((status_t)OK, writer->start());

	while (!writer->reachedEOS() && !m_quit) {
		if (m_duration && systemTime() - start < m_duration * 1000000000) {
			break;
		}
		usleep(100000);
	}

	status_t err = writer->stop();
	nsecs_t end = systemTime();

	
	ALOGI("totoal record time: %fsec\n", (end - start) / 1000000000.0);
	//ALOGI"encoding %d frames in %lld us\n", nFrames, (end-start)/1000);
	//ALOGI("encoding speed is: %.2f fps\n", (nFrames * 1E9) / (end-start));

	if (err != OK && err != ERROR_END_OF_STREAM) {
	    ALOGE("record failed: %d\n", err);
		return UNKNOWN_ERROR;
	}

	return err;
}

//check StagefrightRecorder::setParameter() for detail
void RtkTranscode::initMediaRecorderParameters(String8 &params)
{
    const size_t SIZE = 256;
    char buffer[SIZE];

    if (m_duration) {
        snprintf(buffer, SIZE, "max-duration=%d;", m_duration * 1000); //in ms
        params.append(buffer);
    }

    //muxer parameters
    snprintf(buffer, SIZE, "muxer-param-output-type=%d;", m_outputType);
    params.append(buffer);
    if (m_udpIp && m_udpPort) {
        snprintf(buffer, SIZE, "muxer-param-udp-ip=%s;", m_udpIp);
        params.append(buffer);
        snprintf(buffer, SIZE, "muxer-param-udp-port=%d;", m_udpPort);
        params.append(buffer);
    }
    if (m_bRtpOption) {
        snprintf(buffer, SIZE, "muxer-param-rtp-option=%d;", m_bRtpOption);
        params.append(buffer);
    }

    //audio parameters
    snprintf(buffer, SIZE, "audio-param-sampling-rate=%d;", m_audioSampleRate);
    params.append(buffer);
    snprintf(buffer, SIZE, "audio-param-number-of-channels=%d;", m_audioChannelNum);
    params.append(buffer);
    snprintf(buffer, SIZE, "audio-param-encoding-bitrate=%d;", m_abps);
    params.append(buffer);

    //video parameters
    snprintf(buffer, SIZE, "video-param-encoding-bitrate=%d;", m_vbps);
    params.append(buffer);
    snprintf(buffer, SIZE, "video-param-i-frames-interval=%d;", m_iFramesIntervalSeconds);
    params.append(buffer);
    if (m_profile != -1) {
        snprintf(buffer, SIZE, "video-param-encoder-profile=%d;", m_profile);
        params.append(buffer);
    }
    if (m_level != -1){
        snprintf(buffer, SIZE, "video-param-encoder-level=%d;", m_level);
        params.append(buffer);
    }
    if (m_bH264Enabled){
        snprintf(buffer, SIZE, "video-param-input-format=%d;", 1);
        params.append(buffer);
    }
    snprintf(buffer, SIZE, "video-param-camera-id=%d", m_cameraId);
    params.append(buffer);

    //the parameter string must not end with semicolon ';'
}

status_t RtkTranscode::startMediaRecorder()
{
    sp<Surface> surface = NULL;
    sp<SurfaceControl> control = NULL;
    //sp<Camera> camera = NULL;

    if (m_bHasVideo) {
        CHECK_EQ(setupSurface(&control, &surface), (status_t)OK);
        CHECK_EQ(setupCamera(&camera), (status_t)OK);

        if (m_bHasPreview && m_previewTime && !m_bH264Enabled) {
            previewCamera(surface, camera);
		}
    }

    //sp<MediaRecorder> mr = new MediaRecorder();
    mr = new MediaRecorder();
    CHECK(mr != NULL);

    CHECK(mr->initCheck() == NO_ERROR);

    String8 params;
    initMediaRecorderParameters(params);
    CHECK(mr->setParameters(params) == OK);

    //@Idle
    if (m_bHasVideo) CHECK(mr->setCamera(camera->remote(), camera->getRecordingProxy()) == OK);
    if (m_bHasVideo) CHECK(mr->setVideoSource(VIDEO_SOURCE_CAMERA) == OK);
    if (m_bHasVideo) CHECK(mr->setAudioSource(AUDIO_SOURCE_MIC) == OK);

    //@Initialized
	if (m_muxerFormat == MUXER_TS) {
		CHECK(mr->setOutputFormat(OUTPUT_FORMAT_MPEG2TS) == OK);
	}
	else if (m_muxerFormat == MUXER_MP4) {
		CHECK(mr->setOutputFormat(OUTPUT_FORMAT_MPEG_4) == OK);
	}
	else if (m_muxerFormat == MUXER_AAC) {
		CHECK(mr->setOutputFormat(OUTPUT_FORMAT_AAC_ADTS) == OK);
	}

    //@DataSourceConfigured
    if (m_bHasVideo) {
		CHECK(mr->setPreviewSurface(surface) == OK);
		CHECK(mr->setVideoSize(m_width, m_height) == OK);
		CHECK(mr->setVideoFrameRate(m_vfps) == OK);
		CHECK(mr->setVideoEncoder(VIDEO_ENCODER_H264) == OK);
    }
    if (m_bHasAudio) {
		CHECK(mr->setAudioEncoder(AUDIO_ENCODER_AAC) == OK);
	}

	//CHECK(mr->setOutputFile(fileName) == OK); //not supported
	if (m_outputType == OUTPUT_FILE) {
		//ALOGE("m_outputFilename=%s.",m_outputFilename);
		//int fd = open(m_outputFilename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		//ALOGE("fd=%d.",fd);
		//CHECK(fd >= 0);
		//CHECK(mr->setOutputFile(fd, 0, 0) == OK);
		ALOGD("m_outputFD=%d.",m_outputFD);
		CHECK(m_outputFD >= 0);
		CHECK(mr->setOutputFile(m_outputFD, 0, 0) == OK);
    }
    else if (m_outputType == OUTPUT_STDOUT) {
		CHECK(mr->setOutputFile(1 /*stdout*/, 0, 0) == OK);
    }
    else if (m_bHasAshmem) {
		sp<IMemory> bufferData = NULL;
		sp<IMemory> bufferInfo = NULL;
		CHECK(getRecordBuffer(&bufferData, &bufferInfo) == 0);
		ALOGD("output buffer data:%p:%d info:%p:%d", bufferData->pointer(), bufferData->size(), bufferInfo->pointer(), bufferInfo->size());
		CHECK(mr->setOutputBuffer(bufferData, bufferInfo) == OK);
	}

	CHECK(mr->prepare() == OK);

	//@Prepared
	CHECK(mr->start() == OK);
	m_state = STATE_RUN;

	return OK;

// ToDo: need another way to implement m_duration config.
/*
	//@Recording
	nsecs_t start = systemTime();

	while (!m_quit) {
        if (m_duration && systemTime() - start < m_duration * 1000000000) {
            break;
		}
        usleep(100000);
    }
	status_t err = mr->stop();

    //@Idle
	nsecs_t end = systemTime();
	ALOGI("totoal record time: %fsec\n", (end - start) / 1000000000.0);

	if (err != OK) {
		ALOGE("record failed: %d\n", err);
		return UNKNOWN_ERROR;
	}

	if (m_bHasVideo) {
        camera->disconnect();
	}

    return err;
*/
}

