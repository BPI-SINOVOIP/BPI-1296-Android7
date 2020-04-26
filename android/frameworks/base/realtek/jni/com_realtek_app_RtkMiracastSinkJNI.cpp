#define LOG_NDEBUG 0
#define TAG "RtkMiracastSink-JNI: "
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utils/Log.h>
//copy from com_realtek_hardware_RtkHDMIRxManager.cpp
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
#include <android_runtime/android_graphics_SurfaceTexture.h>
#include <android_runtime/android_view_Surface.h>

#include <cutils/properties.h>
#include <utils/Vector.h>

#include <gui/GLConsumer.h>
#include <gui/Surface.h>
#include <binder/IMemory.h>
#include "RTKWifiDisplaySink.h"

#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "IInbandService.h"


#define RTK_UIBC

namespace android {

bool bFullScreen = true;
static bool bPlay = false;
int surface_x = 0;
int surface_y = 0;
int surface_w = 0;
int surface_h = 0;
//static int msgid = 0;
//static RTKWifiDisplaySink* sink=NULL;
//static const int EVENT_BUFFER_SIZE = 256;
//RawEvent mEventBuffer[EVENT_BUFFER_SIZE];

static jint realtek_app_RtkMiracastSink_open(JNIEnv *env, jobject thiz, jstring clientPackageName)
{
	//const char* cStr = env->GetStringUTFChars(clientPackageName, NULL);
	ALOGI("[%s][%s][%d], euid=%d\n", __FILE__, __func__, __LINE__,geteuid());
    //ALOGV(TAG "RtkMiracastSink_open: %s",cStr);
/*
	FILE *lfp = NULL;	
    do {
    	lfp = fopen("/tmp/miracastipcid","r");
    	if (lfp != NULL) {
    		fread(&msgid,sizeof(int),1,lfp);
    		ALOGI("[%s][%s][%d], msgid=%d\n", __FILE__, __func__, __LINE__,msgid);
    		fclose()lfp;
    	}
    	else {
    		usleep(100000);
    	}
	}while(i++<50);
	ALOGV(TAG "RtkMiracastSink_open: hdcp ready");
    //player = new RTMediaPlayer();
    ALOGV(TAG "RtkMiracastSink_open ok");
*/    
    return 1;
}

static void realtek_app_RtkMiracastSink_close(JNIEnv *env, jobject thiz)
{
    ALOGV(TAG "RtkMiracastSink_close");
    //player = 0;
}

static void realtek_app_RtkMiracastSink_setDisplay(JNIEnv *env, jobject thiz, jobject jSurface)
{
    ALOGV(TAG "RtkMiracastSink_setDisplay");

    sp<IGraphicBufferProducer> gbp;
    sp<Surface> surface;
    if (jSurface) {
        surface = android_view_Surface_getSurface(env, jSurface);
        if (surface != NULL) {
            gbp = surface->getIGraphicBufferProducer();
        }
        else {
        	ALOGV(TAG "RtkMiracastSink_setDisplay 1");
        }
    }
	else {
		ALOGV(TAG "RtkMiracastSink_setDisplay 2");
	}
}

//FIXME: fix parameters, should not only set width and height
static void realtek_app_RtkMiracastSink_SetParameters(JNIEnv *env, jobject thiz,jint left, jint top,jint width, jint height,jint show)
{	
    //ALOGV(TAG "RtkMiracastSink_setParameters, x:%d, y:%d, w:%d, h:%d",left, top, width, height);
	int retry = 0;
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder;
	do {
		binder = sm->checkService(String16("android.inbandmanager"));
		if (binder != 0) {
			break;
		}
		usleep(100000);
	}while(retry++<50);
	if (binder != 0) {
		const sp<IInbandService>& service(interface_cast<IInbandService>(binder));
		//memset(value, 0, sizeof(value));
		service->SetParameters(left, top, width, height, show);
	}
	else {
		ALOGV(TAG "no inbandmanager service.");
	}
}

static jint realtek_app_RtkMiracastSink_GetReady(JNIEnv *env, jobject thiz)
{
	int ret = 0;
	int retry = 0;
    //ALOGV(TAG "RtkMiracastSink_GetReady");
    sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder;
	do {
		binder = sm->checkService(String16("android.inbandmanager"));
		if (binder != 0) {
			break;
		}
		usleep(100000);
	}while(retry++<50);
	if (binder != 0) {
		const sp<IInbandService>& service(interface_cast<IInbandService>(binder));
		//memset(value, 0, sizeof(value));
		ret = service->GetReady();
	}
	else {
		ALOGV(TAG "no inbandmanager service.");
	}
    return ret;
}

static void realtek_app_RtkMiracastSink_SetUIBC(JNIEnv *env, jobject thiz,jint on)
{	
    //ALOGV(TAG "RtkMiracastSink_setParameters, x:%d, y:%d, w:%d, h:%d",on);	
	int retry = 0;
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder;
	do {
		binder = sm->checkService(String16("android.inbandmanager"));
		if (binder != 0) {
			break;
		}
		usleep(100000);
	}while(retry++<50);
	if (binder != 0) {
		const sp<IInbandService>& service(interface_cast<IInbandService>(binder));
		//memset(value, 0, sizeof(value));
		service->SetUIBC(on);
	}
	else {
		ALOGV(TAG "no inbandmanager service.");
	}
}

static jint realtek_app_RtkMiracastSink_start(JNIEnv *env, jobject thiz,jstring ip, jint port)
{
    const char* ipStr = env->GetStringUTFChars(ip, NULL);    
    ALOGV(TAG "wfd_sink_Start. ip=%s.port=%d.", ipStr,port);
    //sTranscodePlayer->configureTargetFormat(TargetFormat);
/*
    MyCB* cb = new MyCB(player);
    ALOGV(TAG "wfd_sink_Start 1");
    sink = RTKWifiDisplaySink::GetInstance();
	ALOGV(TAG "wfd_sink_Start 2");
	sink->Init((char*)ipStr, port, cb);
	ALOGV(TAG "wfd_sink_Start 2");
	sink->Start();
#ifdef RTK_UIBC //UIBC, reference from EventHub	
	//static const int EVENT_BUFFER_SIZE = 256;
    //RawEvent mEventBuffer[EVENT_BUFFER_SIZE];
	mEventHub = new EventHub();
	mReader = new RTKInputReader(mEventHub);
	SetUIBCCallback(RTKUIBC);		
#endif
*/
	bPlay = true;
    return 1;
}

static jint realtek_app_RtkMiracastSink_stop(JNIEnv *env, jobject thiz)
{
    ALOGV(TAG "RtkMiracastSink_stop");
/*
    sink->Stop();
    player->SetVideoDisableReorder(0);
    player->stop();
    player->setVideoLowDelayMode(0);
    player->reset();
    sink->Stop();
    RTKWifiDisplaySink::ReleaseInstance();
	delete cb;
	cb = NULL;
		//msurface = NULL;
	surfaceTexture = NULL;
	surfaceControl = NULL;
	composerClient = NULL;	
	bPlay = false;
*/	
    return 0;
}

/*
 * Array of methods.
 *
 * Each entry has three fields: the name of the method,
 * the method signature, and a pointer to the native implementation.
 */
static const JNINativeMethod sMethods[] = {
    {"jni_wfd_sink_Open", "(Ljava/lang/String;)I", (void*) realtek_app_RtkMiracastSink_open },
    {"jni_wfd_sink_Close", "()V", (void*) realtek_app_RtkMiracastSink_close },
    {"jni_wfd_sink_SetDisplay", "(Landroid/view/Surface;)V", (void*) realtek_app_RtkMiracastSink_setDisplay },
    {"jni_wfd_sink_SetParameters", "(IIIII)V", (void*) realtek_app_RtkMiracastSink_SetParameters },
    {"jni_wfd_sink_GetReady", "()I", (void*) realtek_app_RtkMiracastSink_GetReady },
    {"jni_wfd_sink_SetUIBC", "(I)V", (void*) realtek_app_RtkMiracastSink_SetUIBC },
    {"jni_wfd_sink_Start", "(Ljava/lang/String;I)I", (void*) realtek_app_RtkMiracastSink_start },
    {"jni_wfd_sink_Stop", "()I", (void*) realtek_app_RtkMiracastSink_stop },    
};

/*
* This is called by the VM when the shared library is first loaded.
*/
int register_realtek_app_RtkMiracastSink(JNIEnv *env) {
	//ALOGI("[%s][%s][%d], euid=%d\n", __FILE__, __func__, __LINE__,geteuid());	
	jclass clazz = env->FindClass("com/realtek/app/RTKMiracastSinkJNI");
    if (clazz == NULL)
	{
		ALOGE("Can't find com/realtek/app/RTKMiracastSinkJNI");
		return -1;
	}
    return jniRegisterNativeMethods(env, "com/realtek/app/RTKMiracastSinkJNI",sMethods, NELEM(sMethods));        
}
} /* namespace android */
