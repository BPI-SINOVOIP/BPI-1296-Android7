#include <jni.h>
#include <JNIHelp.h>
#include <android/log.h>
#include <utils/Log.h>
#include <android_runtime/AndroidRuntime.h>
#include <ctype.h>
#include <media/mediarecorder.h>
#include "RtkToGo.h"

#define TAG "ToGoJNI: "

namespace android {
	static RtkToGo* g_pRtkToGo = NULL;

	class ToGoMediaRecorderListener : public MediaRecorderListener
	{
	public:
		ToGoMediaRecorderListener();
		~ToGoMediaRecorderListener();
		void notify(int msg, int ext1, int ext2);		
		void setHandle(int handle);
		int getHandle();
	private:
		int mHandle;
	};

	ToGoMediaRecorderListener::ToGoMediaRecorderListener()
	{
		ALOGV(TAG "ToGoMediaRecorderListener::ToGoMediaRecorderListener.");
		mHandle = -1;
	}

	ToGoMediaRecorderListener::~ToGoMediaRecorderListener()
	{
		ALOGV(TAG "ToGoMediaRecorderListener::~ToGoMediaRecorderListener.");
	}

	void ToGoMediaRecorderListener::notify(int msg, int ext1, int ext2)
	{
		ALOGV(TAG "ToGoMediaRecorderListener::notify.msg=%d.ext1=%d.ext2=%d.",msg,ext1,ext2);
	}

	void ToGoMediaRecorderListener::setHandle(int handle) {
		mHandle = handle;
	}

	int ToGoMediaRecorderListener::getHandle() {
		return mHandle;
	}

	//static sp<ToGoMediaRecorderListener> listener[MAX_TOGO_INSTANCES] = {new ToGoMediaRecorderListener(), new ToGoMediaRecorderListener()};
	static sp<ToGoMediaRecorderListener> listener[MAX_TOGO_INSTANCES] = {new ToGoMediaRecorderListener()};

	static int findAvailableListener()
	{
		for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
			if (listener[i]->getHandle() == -1) {
				return i;
			}
		}
		ALOGE(TAG "no available listener.");
		return -1;
	}

	static int findListenerByHandle(int handle) {
		for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
			if (listener[i]->getHandle() == handle) {
				return i;
			}
		}
		ALOGE(TAG "no matched listener.handle=%d.",handle);
		return -1;
	}

	static jint realtek_server_ToGoService_jniInit(JNIEnv* env, jobject obj, jint type, jint cameraId) {
		ALOGV(TAG "jniInit.type=%d.cameraId=%d.", type,cameraId);
		int handle = -1;

		g_pRtkToGo = RtkToGo::getInstance();
		if (g_pRtkToGo != NULL) {
			int index = findAvailableListener();
			if (index < 0) {
				ALOGE(TAG "jniInit.findAvailableListener() NG.");
				return handle;
			}

			handle = g_pRtkToGo->init(type, cameraId, listener[index]);
			if (handle != (-1)) {
				listener[index]->setHandle(handle);
				ALOGV(TAG "jniInit.occupy an available listener.index=%d.handle=%d.", index, handle);
			}
		}
		return handle;
	}

	static void realtek_server_ToGoService_jniUnInit(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniUnInit.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->uninit(handle);
		int index = findListenerByHandle(handle);
		if (index >= 0) {
			ALOGV(TAG "jniUnInit.release listener to available.index=%d.handle=%d.", index,listener[index]->getHandle());
			listener[index]->setHandle(-1);
		}		
		int ref = RtkToGo::releaseInstance();
		if (ref == 0) {
		g_pRtkToGo = NULL;
			ALOGV("jniUnInit.set g_pRtkToGo to NULL.");
		}
	}

	static void realtek_server_ToGoService_jniSetPreview(JNIEnv* env, jobject obj, jint handle, jboolean isPreview) {
		ALOGV(TAG "jniSetPreview.handle=%d.isPreview=%d.", handle,isPreview);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setPreview(handle, isPreview);
	}

	static int realtek_server_ToGoService_jniSetPreviewSize(JNIEnv* env, jobject obj, jint handle, jint width, jint height) {
		ALOGV(TAG "jniSetVideoWidthHeight.handle=%d.width=%d.height=%d.", handle,width,height);
		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->setPreviewSize(handle, width, height);
	}

	static int realtek_server_ToGoService_jniSetVideoFrameRate(JNIEnv* env, jobject obj, jint handle, jint vfps) {
		ALOGV(TAG "jniSetVideoFrameRate.handle=%d.vfps=%d.", handle,vfps);
		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->setVideoFrameRate(handle, vfps);
	}

	static void realtek_server_ToGoService_jniSetEnableH264Capture(JNIEnv* env, jobject obj, jint handle, jboolean isEnableH264Capture) {
		ALOGV(TAG "jniSetEnableH264Capture.handle=%d.isEnableH264Capture=%d.", handle,isEnableH264Capture);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setEnableH264Capture(handle, isEnableH264Capture);
	}

	static void realtek_server_ToGoService_jniSetMuxerFormat(JNIEnv* env, jobject obj, jint handle, jint muxerFormat) {
		ALOGV(TAG "jniSetMuxerFormat.handle=%d.muxerFormat=%d.", handle,muxerFormat);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setMuxerFormat(handle, muxerFormat);
	}

	static void realtek_server_ToGoService_jniSetOutputType(JNIEnv* env, jobject obj, jint handle, jint outputType) {
		ALOGV(TAG "jniSetOutputType.handle=%d.outputType=%d.", handle,outputType);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setOutputType(handle, outputType);
	}

	static void realtek_server_ToGoService_jniSetOutputFD(JNIEnv* env, jobject obj, jint handle, jobject fileDescriptor) {
		ALOGV(TAG "jniSetOutputFD.handle=%d.fileDescriptor=%p.", handle,fileDescriptor);
		if (fileDescriptor == NULL) {
			ALOGE(TAG "jniSetOutputFD.fileDescriptor == NULL.");
			return;
		}
		if (NULL == g_pRtkToGo) {
			return;
		}
		int fd = jniGetFDFromFileDescriptor(env, fileDescriptor);
		g_pRtkToGo->setOutputFD(handle, fd);
	}

	static void realtek_server_ToGoService_jniSetDuration(JNIEnv* env, jobject obj, jint handle, jint duration) {
		ALOGV(TAG "jniSetDuration.handle=%d.duration=%d.", handle,duration);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setDuration(handle, duration);
	}

	static void realtek_server_ToGoService_jniSetUdpIpPort(JNIEnv* env, jobject obj, jint handle, jstring ip, jint port) {
		const char* ipStr = env->GetStringUTFChars(ip, NULL);
		ALOGV(TAG "jniSetUdpIpPort.handle=%d.ip=%s.port=%d.", handle,ipStr,port);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setUdpIpPort(handle, (char*)ipStr, port);
	}

	static void realtek_server_ToGoService_jniSetVideoBitrate(JNIEnv* env, jobject obj, jint handle, jint vbps) {
		ALOGV(TAG "jniSetVideoBitrate.handle=%d.vbps=%d.", handle,vbps);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setVideoBitrate(handle, vbps);
	}

	static void realtek_server_ToGoService_jniSetIFrameIntervalSeconds(JNIEnv* env, jobject obj, jint handle, jint iFramesIntervalSeconds) {
		ALOGV(TAG "jniSetIFrameIntervalSeconds.handle=%d.iFramesIntervalSeconds=%d.", handle,iFramesIntervalSeconds);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setIFrameIntervalSeconds(handle, iFramesIntervalSeconds);
	}

	static void realtek_server_ToGoService_jniSetAudioChannelCount(JNIEnv* env, jobject obj, jint handle, jint audioChannelNum) {
		ALOGV(TAG "jniSetAudioChannelCount.handle=%d.audioChannelNum=%d.", handle,audioChannelNum);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setAudioChannelCount(handle, audioChannelNum);
	}

	static void realtek_server_ToGoService_jniSetAudioSampleRate(JNIEnv* env, jobject obj, jint handle, jint audioSampleRate) {
		ALOGV(TAG "jniSetAudioSampleRate.handle=%d.audioSampleRate=%d.", handle,audioSampleRate);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setAudioSampleRate(handle, audioSampleRate);
	}

	static void realtek_server_ToGoService_jniSetAudioBitrate(JNIEnv* env, jobject obj, jint handle, jint abps) {
		ALOGV(TAG "jniSetAudioBitrate.handle=%d.abps=%d.", handle,abps);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->setAudioBitrate(handle, abps);
	}

	// void realtek_server_ToGoService_jniSetProfile(JNIEnv* env, jobject obj, jint handle, jint profile) {
	//	ALOGV(TAG "jniSetProfile.handle=%d.profile=%d.", handle,profile);
	//	if (NULL == g_pRtkToGo) {
	//		return;
	//	}
	//	g_pRtkToGo->setProfile(handle, profile);
	//}

	static void realtek_server_ToGoService_jniConfigCompleted(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniConfigCompleted.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->configCompleted(handle);
	}

	static jint realtek_server_ToGoService_jniStart(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniStart.handle=%d.", handle);
		int ret = -1;

		if (NULL == g_pRtkToGo) {
			return ret;
		}
		ret = g_pRtkToGo->start(handle);
		return ret;
	}

	static void realtek_server_ToGoService_jniStop(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniStop.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->stop(handle);
	}

	static jstring realtek_server_ToGoService_jniGetCamParams(JNIEnv* env, jobject obj, jint handle, jint paramType) {
		ALOGV(TAG "jniGetCamParams.handle=%d.paramType=%d.", handle,paramType);
		if (NULL == g_pRtkToGo) {
			return env->NewStringUTF("");
		}
		const char* params = g_pRtkToGo->getCamParams(handle, paramType);
		ALOGV(TAG "jniGetCamParams.params=%s.", params);
		if (params == NULL) {
			return env->NewStringUTF("");
		}
		else {
			jstring jStr = env->NewStringUTF(params);
			//const char* cStr = env->GetStringUTFChars(jStr, NULL);
			//ALOGV(TAG "jniGetCamParams.cStr=%s.", cStr);
			return jStr;
		}
	}

	static jint realtek_server_ToGoService_jniIsHandleLegal(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniIsHandleLegal.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->isHandleLegal(handle);
	}

	static jobject realtek_server_ToGoService_jniGetSourceStatus(JNIEnv* env, jobject obj) {
		ALOGV(TAG "jniGetSourceStatus.");
		
		RtkToGo* pToGo = RtkToGo::getInstance();

		jclass jToGoSourceStatus = env->FindClass("com/realtek/app/togo/ToGoSourceStatus");
		jmethodID mID = env->GetMethodID(jToGoSourceStatus, "<init>", "()V");

		jobject togoSourceStatusObj = env->NewObject(jToGoSourceStatus, mID);

		jfieldID fid_mCount = env->GetFieldID(jToGoSourceStatus, "mCount", "I");
		jfieldID fid_avail = env->GetFieldID(jToGoSourceStatus, "mIsSourceAvailable", "[I");

		env->SetIntField(togoSourceStatusObj, fid_mCount, 4);

		jintArray jAvailArray;
		jint* nativeAvailArray;
		jboolean isCopy;

		jAvailArray = env->NewIntArray(4);
		if (jAvailArray != NULL) {
			nativeAvailArray = env->GetIntArrayElements(jAvailArray, &isCopy);
			if (pToGo->isSourceAvailable(TOGO_TYPE_CAMERA) == 0) {
				nativeAvailArray[TOGO_TYPE_CAMERA] = 1;	// camera
			}
			if (pToGo->isSourceAvailable(TOGO_TYPE_FILE) == 0) {
				nativeAvailArray[TOGO_TYPE_FILE] = 1;	// file
			}
			if (pToGo->isSourceAvailable(TOGO_TYPE_DTV) == 0) {
				nativeAvailArray[TOGO_TYPE_DTV] = 1;	// DTV
			}
			if (pToGo->isSourceAvailable(TOGO_TYPE_HDMI_RX) == 0) {
				nativeAvailArray[TOGO_TYPE_HDMI_RX] = 1;	// HDMIRx
			}
			

			env->SetObjectField(togoSourceStatusObj, fid_avail, jAvailArray);
			
			env->ReleaseIntArrayElements(jAvailArray, nativeAvailArray, 0);
		}

		RtkToGo::releaseInstance();

		return togoSourceStatusObj;
	}

	static jint realtek_server_ToGoService_jniSetTargetFilename(JNIEnv* env, jobject obj, jint handle, jstring filename) {
		const char* cStr = env->GetStringUTFChars(filename, NULL);
		ALOGV(TAG "jniIsHandleLegal.handle=%d.filename=%s.", handle,cStr);
		
		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->setTargetFilename(handle, (char*)cStr);
	}

	static jint realtek_server_ToGoService_jniGetHandleMax(JNIEnv* env, jobject obj) {
		ALOGV(TAG "jniGetHandleMax.");
		RtkToGo* pToGo = RtkToGo::getInstance();
		if (NULL == pToGo) {
			return -1;
		}

		jint handleMax = pToGo->getHandleMax();
		RtkToGo::releaseInstance();
		
		return handleMax;
	}


	static jint realtek_server_ToGoService_jniGetHandleCount(JNIEnv* env, jobject obj) {
		ALOGV(TAG "jniGetHandleCount.");
		RtkToGo* pToGo = RtkToGo::getInstance();
		if (NULL == pToGo) {
			return -1;
		}
		jint count = pToGo->getHandleCount();
		RtkToGo::releaseInstance();
		
		return count;;
	}

	static jint realtek_server_ToGoService_jniGetHandleSourceType(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniGetHandleSourceType.handle=%d.",handle);
		RtkToGo* pToGo = RtkToGo::getInstance();
		if (NULL == pToGo) {
			return -1;
		}
		jint type = pToGo->getHandleSourceType(handle);
		RtkToGo::releaseInstance();
		return type;
	}

	static jint realtek_server_ToGoService_jniGetHandleState(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniGetHandleState.handle=%d.",handle);
		RtkToGo* pToGo = RtkToGo::getInstance();
		if (NULL == pToGo) {
			return -1;
		}
		jint state = pToGo->getHandleState(handle);
		RtkToGo::releaseInstance();
		return state;
	}

	static jstring realtek_server_ToGoService_jniGetLCNList(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniGetCamParams.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return env->NewStringUTF("");
		}
		const char* list = g_pRtkToGo->getLCNList(handle);
		ALOGV(TAG "jniGetLCNList.list=%s.", list);
		if (list == NULL) {
			return env->NewStringUTF("");
		}
		else {
			jstring jStr = env->NewStringUTF(list);
			const char* cStr = env->GetStringUTFChars(jStr, NULL);
			ALOGV(TAG "jniGetLCNList.cStr=%s.", cStr);
			return jStr;
		}
	}

	static jint realtek_server_ToGoService_jniPlayChannel(JNIEnv* env, jobject obj, jint handle, jint chIdx) {
		ALOGV(TAG "jniPlayChannel.handle=%d.chIdx=%d.",handle,chIdx);

		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->PlayChannel(handle, chIdx);
	}

	static jint realtek_server_ToGoService_jniPlayNextChannel(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniPlayNextChannel.handle=%d.",handle);

		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->PlayNextChannel(handle);
	}

	static jint realtek_server_ToGoService_jniPlayPrevChannel(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniPlayPrevChannel.handle=%d.",handle);

		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->PlayPrevChannel(handle);
	}

	static jint realtek_server_ToGoService_jniGetFileWidth(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniGetFileWidth.handle=%d.",handle);

		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->GetFileWidth(handle);
	}

	static jint realtek_server_ToGoService_jniGetFileHeight(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniGetFileHeight.handle=%d.",handle);

		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->GetFileHeight(handle);
	}

	static jint realtek_server_ToGoService_jniStartListen(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniStartListen.handle=%d.",handle);

		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->StartListen(handle);
	}

	static void realtek_server_ToGoService_jniStopListen(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniStopListen.handle=%d.",handle);

		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->StopListen(handle);
	}

	static void realtek_server_ToGoService_jniSeek(JNIEnv* env, jobject obj, jint handle, jint msec) {
		ALOGV(TAG "jniSeek.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return;
		}
		g_pRtkToGo->seek(handle, msec);
	}

	static int realtek_server_ToGoService_jniGetDuration(JNIEnv* env, jobject obj, jint handle) {
		ALOGV(TAG "jniGetDuration.handle=%d.", handle);
		if (NULL == g_pRtkToGo) {
			return -1;
		}
		return g_pRtkToGo->getDuration(handle);
	}

	static JNINativeMethod sMethods[] = {
		{"jniInit", "(II)I", (void*)realtek_server_ToGoService_jniInit},
		{"jniUnInit", "(I)V", (void*)realtek_server_ToGoService_jniUnInit},
		{"jniSetPreview", "(IZ)V", (void*)realtek_server_ToGoService_jniSetPreview},
		{"jniSetPreviewSize", "(III)I", (void*)realtek_server_ToGoService_jniSetPreviewSize},
		{"jniSetVideoFrameRate", "(II)I", (void*)realtek_server_ToGoService_jniSetVideoFrameRate},
		{"jniSetEnableH264Capture", "(IZ)V", (void*)realtek_server_ToGoService_jniSetEnableH264Capture},
		{"jniSetMuxerFormat", "(II)V", (void*)realtek_server_ToGoService_jniSetMuxerFormat},
		{"jniSetOutputType", "(II)V", (void*)realtek_server_ToGoService_jniSetOutputType},
		{"jniSetOutputFD", "(ILjava/io/FileDescriptor;)V", (void*)realtek_server_ToGoService_jniSetOutputFD},
		{"jniSetDuration", "(II)V", (void*)realtek_server_ToGoService_jniSetDuration},
		{"jniSetUdpIpPort", "(ILjava/lang/String;I)V", (void*)realtek_server_ToGoService_jniSetUdpIpPort},
		{"jniSetVideoBitrate", "(II)V", (void*)realtek_server_ToGoService_jniSetVideoBitrate},		
		{"jniSetIFrameIntervalSeconds", "(II)V", (void*)realtek_server_ToGoService_jniSetIFrameIntervalSeconds},
		{"jniSetAudioChannelCount", "(II)V", (void*)realtek_server_ToGoService_jniSetAudioChannelCount},
		{"jniSetAudioSampleRate", "(II)V", (void*)realtek_server_ToGoService_jniSetAudioSampleRate},
		{"jniSetAudioBitrate", "(II)V", (void*)realtek_server_ToGoService_jniSetAudioBitrate},
		//{"jniSetProfile", "(II)V", (void*)realtek_server_ToGoService_jniSetProfile},
		{"jniConfigCompleted", "(I)V", (void*)realtek_server_ToGoService_jniConfigCompleted},
		{"jniStart", "(I)I", (void*)realtek_server_ToGoService_jniStart},
		{"jniStop", "(I)V", (void*)realtek_server_ToGoService_jniStop},
		{"jniGetCamParams", "(II)Ljava/lang/String;", (void*)realtek_server_ToGoService_jniGetCamParams},
		{"jniIsHandleLegal", "(I)I", (void*)realtek_server_ToGoService_jniIsHandleLegal},
		{"jniGetSourceStatus", "()Lcom/realtek/app/togo/ToGoSourceStatus;", (void*) realtek_server_ToGoService_jniGetSourceStatus },
		{"jniSetTargetFilename", "(ILjava/lang/String;)I", (void*)realtek_server_ToGoService_jniSetTargetFilename},
		{"jniGetHandleMax", "()I", (void*)realtek_server_ToGoService_jniGetHandleMax},
		{"jniGetHandleCount", "()I", (void*)realtek_server_ToGoService_jniGetHandleCount},
		{"jniGetHandleSourceType", "(I)I", (void*)realtek_server_ToGoService_jniGetHandleSourceType},
		{"jniGetHandleState", "(I)I", (void*)realtek_server_ToGoService_jniGetHandleState},
		{"jniGetLCNList", "(I)Ljava/lang/String;", (void*)realtek_server_ToGoService_jniGetLCNList},
		{"jniPlayChannel", "(II)I", (void*)realtek_server_ToGoService_jniPlayChannel},
		{"jniPlayNextChannel", "(I)I", (void*)realtek_server_ToGoService_jniPlayNextChannel},
		{"jniPlayPrevChannel", "(I)I", (void*)realtek_server_ToGoService_jniPlayPrevChannel},
		{"jniGetFileWidth", "(I)I", (void*)realtek_server_ToGoService_jniGetFileWidth},
		{"jniGetFileHeight", "(I)I", (void*)realtek_server_ToGoService_jniGetFileHeight},
		{"jniStartListen", "(I)I", (void*)realtek_server_ToGoService_jniStartListen},
		{"jniStopListen", "(I)V", (void*)realtek_server_ToGoService_jniStopListen},
		{"jniSeek", "(II)V", (void*)realtek_server_ToGoService_jniSeek},
		{"jniGetDuration", "(I)I", (void*)realtek_server_ToGoService_jniGetDuration}
	};

	int register_android_server_ToGoService(JNIEnv* env) {
		return jniRegisterNativeMethods(env, "com/realtek/server/ToGoService",
                                    sMethods, NELEM(sMethods));
	}
}	// namespace android
