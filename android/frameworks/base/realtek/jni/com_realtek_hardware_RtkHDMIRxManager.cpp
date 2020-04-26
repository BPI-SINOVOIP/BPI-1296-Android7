#define LOG_NDEBUG 0
#define LOG_TAG "RtkDHMIRxManager-JNI"
#include <utils/Log.h>

#include <jni.h>
#include <JNIHelp.h>
#include <android_runtime/AndroidRuntime.h>
#include <android_runtime/android_graphics_SurfaceTexture.h>
#include <android_runtime/android_view_Surface.h>

#include <cutils/properties.h>
#include <utils/Vector.h>
#include <utils/Mutex.h>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IMemory.h>
#include <camera/Camera.h>
#include <gui/GLConsumer.h>
#include <gui/Surface.h>
#include "RtkTranscodeCommon.h"
#include "RtkTranscodePlayer.h"
// added for binder service mode.
#include "RtkIpc/TranscodeService.h"
#include "RtkIpc/IRtkTranscodePlayerServiceClient.h"

// use binder service
#define USE_RTK_TRANSCODEPLAYER_SERVICE

#define JAVA_CALLBACK_FUN_NAME  "handleEventFromNative"

#define LOG_ERROR_LINE()    ALOGE("[%s][%d] TranscodePlayer instance in NULL",__FUNCTION__,__LINE__)

namespace android {

enum {
    EVENT_CAMERA_ERROR = 1,
};

static JavaVM *g_jvm;

static Mutex  mTranscodePlayerLock;

#ifdef USE_RTK_TRANSCODEPLAYER_SERVICE

/**
 * Basic definiation of the RxHandler
 */
class RxHandler : public AHandler
{
public:
    RxHandler(JNIEnv *env, jobject thiz){
        mEnv = env;
        mThiz = mEnv->NewGlobalRef(thiz);;
    }

protected:
    virtual ~RxHandler();
    virtual void onMessageReceived(const sp<AMessage> &msg);

    jobject mThiz;
    JNIEnv *mEnv;
};

RxHandler::~RxHandler()
{
    ALOGD("~RxHandler() remove jobject");
    mEnv->DeleteGlobalRef(mThiz);
}

void RxHandler::onMessageReceived(const sp<AMessage> &msg)
{
    //ALOGV("RxHandler::onMessageReceived %d",msg->what());
    switch (msg->what()) {
        case kWhatCameraError:
        {
            JNIEnv *env;
            if(g_jvm->AttachCurrentThread(&env, NULL) != JNI_OK)
            {
                ALOGE("RxHandler::onMessageReceived get JNIEnv failed");
                return;
            }

            const char * fnName = JAVA_CALLBACK_FUN_NAME;
            jint event = EVENT_CAMERA_ERROR;
            jclass cls = env->GetObjectClass(mThiz);
            jmethodID messageMe = env->GetMethodID(cls, fnName, "(I)V");
            env->CallVoidMethod(mThiz, messageMe, event);
            g_jvm->DetachCurrentThread();
            break;
        }
        default:
            break;
    }
}

// single instance should be okay since they are exist in different process.
static sp<IRtkTranscodePlayerServiceClient> sTranscodePlayer;
static sp<Camera> mCamera;
static sp<ALooper> sLooper = NULL;
static sp<RxHandler> sHandler = NULL;

#else
static sp<RtkTranscodePlayer> sTranscodePlayer;
#endif


static AV_FORMAT_CONFIG TargetFormat = {
    {
            640,     // width
            480,     // height
            30,      // fps;
            9000000, // bitrate
            1,       // iframeInterval
            1,       // rateControl , default is variable
            0,       // aspectRatio
            0,       // interlace
            0,       // shareWBbuf
    },
    {
            2,      // channelCount;
            0,      // channelMode
            48000,  // sampleRate
            192000, // bitrate
    }
};

static RtkTranscodeConf config;

static jint realtek_hardware_RtkHDMIRxManager_open(JNIEnv *env, jobject thiz, jstring clientPackageName)
{
    ALOGV("RtkHDMIRxManager_open");
    Mutex::Autolock l(mTranscodePlayerLock);
    // Convert jstring to String16
    const jchar *rawClientName = env->GetStringChars(clientPackageName, NULL);
    jsize rawClientNameLen = env->GetStringLength(clientPackageName);
    String16 clientName((const char16_t *)rawClientName, rawClientNameLen);
    env->ReleaseStringChars(clientPackageName, rawClientName);

#ifdef USE_RTK_TRANSCODEPLAYER_SERVICE
    ProcessState::self()->startThreadPool();
    sp<IRtkTranscodePlayerService> sm(TranscodeService::getTranscodeService());

    int clientCount = sm->getClientsCount();
    ALOGD("RtkTranscodePlayer instance count: %d",clientCount);

    int loopCnt = 0;

    while(clientCount > 0 && loopCnt < 10) {
        ALOGI("HDMIRx is opened by another process, wait some times");
        clientCount = sm->getClientsCount();
        usleep(1E3);
        loopCnt++;
    }

    if(clientCount > 0){
        ALOGE("HDMIRx is opened by another process, close the resource first");
        return -1;
    }

    sTranscodePlayer = sm->create(0);

    if(sLooper != NULL){
        sLooper->stop();
        sLooper.clear();
        sLooper = NULL;
    }

    if(sHandler != NULL){
        sHandler.clear();
        sHandler = NULL;
    }

    // support callback message
    sLooper = new ALooper();
    sLooper->setName("RxManagerLooper");
    sHandler = new RxHandler(env,thiz);
    sLooper->registerHandler(sHandler);
    sLooper->start();
    sTranscodePlayer->registerStatusHandler(static_cast<AHandler *>(sHandler.get()));

#else
    sTranscodePlayer = new RtkTranscodePlayer(true, false);
#endif

    sTranscodePlayer->initiate(/*RtkTranscodePlayer::*/TranscodeTypeHdmiRx);
#ifdef USE_RTK_TRANSCODEPLAYER_SERVICE
    int cameraId = 1;
    mCamera = Camera::connect(cameraId, clientName, Camera::USE_CALLING_UID, Camera::USE_CALLING_PID);
    if(mCamera.get() == NULL) {
        ALOGE("camera connect failed");

        if(sLooper != NULL){
            sLooper->stop();
            sLooper.clear();
            sLooper = NULL;
        }

        if(sHandler != NULL){
            sHandler.clear();
            sHandler = NULL;
        }

        if(sTranscodePlayer != NULL) {
            sTranscodePlayer->release();
            sTranscodePlayer = NULL;
        }
        return -1;
    }

    mCamera->unlock();
    sTranscodePlayer->setCameraSource(/*mCameraId, clientName,*/ mCamera->remote());
    sm->increaseClientCount();
#else
    sTranscodePlayer->setCameraSource(1, clientName);
#endif
    sTranscodePlayer->setAudioSource(9);
    ALOGD("[%s] sTranscodePlayer %p",__FUNCTION__,sTranscodePlayer.get());
    return 0;
}

static void realtek_hardware_RtkHDMIRxManager_release(JNIEnv *env, jobject thiz)
{
    Mutex::Autolock l(mTranscodePlayerLock);
    ALOGV("RtkHDMIRxManager_release");
#ifdef USE_RTK_TRANSCODEPLAYER_SERVICE
    //IPCThreadState::self()->stopProcess();
    if(sLooper != NULL){
        sLooper->stop();
        sLooper.clear();
        sLooper = NULL;
    }

    if(sHandler != NULL){
        sHandler.clear();
        sHandler = NULL;
    }

    // only call release in binder mode
    if(sTranscodePlayer != NULL)
    {
        sTranscodePlayer->release();
        sp<IRtkTranscodePlayerService> sm(TranscodeService::getTranscodeService());
        sm->decreaseClientCount();
    }

#endif
    if (sTranscodePlayer!=NULL)
    {
        sTranscodePlayer = NULL;
    }
}

static void realtek_hardware_RtkHDMIRxManager_setPreviewDisplay(JNIEnv *env, jobject thiz, jobject jSurface)
{
    ALOGV("RtkHDMIRxManager_setPreviewDisplay");

    if(sTranscodePlayer.get() == NULL){
        LOG_ERROR_LINE();
        return;
    }

    sp<IGraphicBufferProducer> gbp;
    sp<Surface> surface;
    if (jSurface) {
        surface = android_view_Surface_getSurface(env, jSurface);
        if (surface != NULL) {
            gbp = surface->getIGraphicBufferProducer();
        }
    }
    sTranscodePlayer->setSurface(gbp);
    sTranscodePlayer->setPlayback(true, true); 
}

//FIXME: fix parameters, should not only set width and height
static void realtek_hardware_RtkHDMIRxManager_setParameters(JNIEnv *env, jobject thiz,jint width, jint height, jint fps)
{
    ALOGV("RtkHDMIRxManager_setParameters, width:%d, height:%d, fps:%d", width, height, fps);
    TargetFormat.videoFmt.width = width;
    TargetFormat.videoFmt.height = height;
	TargetFormat.videoFmt.fps = (fps > 0)? fps : 30;

#if 0
    // if fps is 60 , down drop to 30, if fps is 24, does nothing.
    if(TargetFormat.videoFmt.fps == 60){
        TargetFormat.videoFmt.fps = 30;
    }
#endif

} 

static jstring realtek_hardware_RtkHDMIRxManager_getParameters(JNIEnv *env, jobject thiz)
{
    ALOGV("RtkHDMIRxManager_getParameters");

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return 0;
    }

    String8 params8 = sTranscodePlayer->getCameraParameters();
    if (params8.isEmpty()) {
        jniThrowRuntimeException(env, "getParameters failed (empty parameters)");
        return 0;
    }
    return env->NewStringUTF(params8.string());
}

static jint realtek_hardware_RtkHDMIRxManager_startPreview(JNIEnv *env, jobject thiz)
{
    ALOGV("RtkHDMIRxManager_startPreview");

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return -1;
    }

    sTranscodePlayer->configureRxPreviewResolution(TargetFormat.videoFmt.width,TargetFormat.videoFmt.height);
    return sTranscodePlayer->configureTargetFormat(TargetFormat);
}

static jint realtek_hardware_RtkHDMIRxManager_stopPreview(JNIEnv *env, jobject thiz)
{
    ALOGV("RtkHDMIRxManager_stopPreview");

    if(sTranscodePlayer.get() == NULL){
        LOG_ERROR_LINE();
        return -1;
    }


    jint rst = sTranscodePlayer->stop();

#ifdef USE_RTK_TRANSCODEPLAYER_SERVICE
    mCamera->disconnect();
#endif

    return rst;
}

static jbyteArray realtek_hardware_RtkHDMIRxManager_getSnapshot(JNIEnv *env, jobject thiz, jint format, jint x, jint y, jint cropWidth, jint cropHeight, jint outWidth, jint outHeight)
{
    CAMERA_SNAPSHOT snapshot;
    snapshot.format = format;
    snapshot.x = x;
    snapshot.y = y;
    snapshot.width = cropWidth;
    snapshot.height = cropHeight;
    snapshot.outWidth = outWidth;
    snapshot.outHeight = outHeight;

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return NULL;
    }

#ifdef USE_RTK_TRANSCODEPLAYER_SERVICE
    sp<IMemory> buffer = sTranscodePlayer->getCameraSnapshot(&snapshot);
    if(buffer == 0){
        ALOGE("failed to get camera snapshot");
        return NULL;
    }else{
        jbyteArray jbuff = env->NewByteArray(buffer->size());
        env->SetByteArrayRegion(jbuff, 0, buffer->size(), (jbyte*)buffer->pointer());
        return jbuff;
    }
#else
    int ret = sTranscodePlayer->getCameraSnapshot(&snapshot);
    ALOGD("getSnapshot ret:%d data:%p size:%d fmt:%d (%d,%d) %dx%d => %dx%d", ret, snapshot.data, snapshot.size, snapshot.format, snapshot.x, snapshot.y, snapshot.width, snapshot.height, snapshot.outWidth, snapshot.outHeight);
    if(ret==0) {
        jbyteArray buff = env->NewByteArray(snapshot.size);
        env->SetByteArrayRegion(buff, 0, snapshot.size, (jbyte*)snapshot.data);
        return buff;
    }
    else {
        ALOGE("failed to get camer snapshot");
        return NULL;
    }
#endif
}

/*
 * Class:     com_realtek_hardware_RtkHDMIRxManager
 * Method:    native_setTranscode
 * Signature: (Z)V
 */
static void Java_com_realtek_hardware_RtkHDMIRxManager_native_1setTranscode
  (JNIEnv *env, jobject thiz, jboolean on)
{
    ALOGD("[native_setTranscode] on:%d",on);

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return;
    }

    sTranscodePlayer->setTranscode((bool)on);
}

/*
 * Class:     com_realtek_hardware_RtkHDMIRxManager
 * Method:    native_setTargetFd
 * Signature: (II)V
 */
static void Java_com_realtek_hardware_RtkHDMIRxManager_native_1setTargetFd
  (JNIEnv *env, jobject thiz, jint fd, jint format)
{
    ALOGD("[native_setTargetFd] fd:%d format:%d",fd,format);


    config.outputType = OUTPUT_FD;
    config.outputFormat = format;
    config.fileDesc = fd;
    config.fileName = "";
    config.ashmemData = NULL;
    config.ashmemInfo = NULL;
    config.udpIp = "";
    config.udpPort = 0;
    config.rtpHeader = false;

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return;
    }

    sTranscodePlayer->setOutputFormat(config);
}

/*
 * Class:     com_realtek_hardware_RtkHDMIRxManager
 * Method:    native_configureTargetFormat
 * Signature: (IIIIIIIIIIIII)V
 */
static void Java_com_realtek_hardware_RtkHDMIRxManager_native_1configureTargetFormat
  (JNIEnv *env, jobject thiz, jint w, jint h, jint fps, jint vbitrate,
    jint iframeInterval, jint rateControl, jint aspectRatio,
    jint interlace, jint shareWBbuf,
    jint channelCount, jint channelMode, jint sampleRate, jint abitrate)
{
    ALOGD("[native_configureTargetFormat] -> call resetTargetFormat");
    TargetFormat.videoFmt.width = w;
    TargetFormat.videoFmt.height = h;

    // done in setParameter
    //TargetFormat.videoFmt.fps = (fps==60)?30:fps;

    TargetFormat.videoFmt.bitrate = vbitrate;
    TargetFormat.videoFmt.iframeInterval = iframeInterval;
    TargetFormat.videoFmt.rateControl = rateControl;
    TargetFormat.videoFmt.aspectRatio = aspectRatio;
    TargetFormat.videoFmt.interlace = interlace;
    TargetFormat.videoFmt.shareWBbuf = shareWBbuf;

    TargetFormat.audioFmt.channelCount = channelCount;
    TargetFormat.audioFmt.channelMode = channelMode;
    TargetFormat.audioFmt.sampleRate = sampleRate;
    TargetFormat.audioFmt.bitrate = abitrate;

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return;
    }

    sTranscodePlayer->resetTargetFormat(TargetFormat);

}

/*
 * Class:     com_realtek_hardware_RtkHDMIRxManager
 * Method:    native_setPlayback
 * Signature: (Z)V
 */
static void Java_com_realtek_hardware_RtkHDMIRxManager_native_1setPlayback
  (JNIEnv *env, jobject thiz, jboolean videoEn, jboolean audioEn)
{
    ALOGD("[native_setPlayback] video:%d audio:%d",videoEn,audioEn);

    if(sTranscodePlayer.get() == NULL) {
        LOG_ERROR_LINE();
        return;
    }

    sTranscodePlayer->setPlayback((bool)videoEn, (bool)audioEn);
}

static void Java_com_realtek_hardware_RtkHDMIRxManager_native_notifyHDMIRxAudioHotplug(
        JNIEnv *env,
        jobject thiz,
        jint state)
{
    Mutex::Autolock l(mTranscodePlayerLock);
    ALOGD("[RXA][%s] : E trandcodePlayer %p",
            __FUNCTION__,
            sTranscodePlayer.get());
    if(sTranscodePlayer.get() != NULL) {
        sTranscodePlayer->notifyHDMIRxAudioHotplug(state);
    }
}

//----------------------------------------------------------------------

/*
 * Array of methods.
 *
 * Each entry has three fields: the name of the method,
 * the method signature, and a pointer to the native implementation.
 */
static const JNINativeMethod RtkHDMIRxManagerMethods[] = {
    {"native_open", "(Ljava/lang/String;)I", (void*) realtek_hardware_RtkHDMIRxManager_open },
    {"native_release", "()V", (void*) realtek_hardware_RtkHDMIRxManager_release },
    {"native_setPreviewDisplay", "(Landroid/view/Surface;)V", (void*) realtek_hardware_RtkHDMIRxManager_setPreviewDisplay },
    {"native_setParameters", "(III)V", (void*) realtek_hardware_RtkHDMIRxManager_setParameters },//FIXME: fix parameter
    {"native_getParameters", "()Ljava/lang/String;", (void*) realtek_hardware_RtkHDMIRxManager_getParameters },
    {"native_startPreview", "()I", (void*) realtek_hardware_RtkHDMIRxManager_startPreview },
    {"native_stopPreview", "()I", (void*) realtek_hardware_RtkHDMIRxManager_stopPreview },
    {"native_getSnapshot", "(IIIIIII)[B", (void*) realtek_hardware_RtkHDMIRxManager_getSnapshot },
    {"native_setTranscode", "(Z)V", (void*) Java_com_realtek_hardware_RtkHDMIRxManager_native_1setTranscode },
    {"native_setTargetFd", "(II)V", (void*) Java_com_realtek_hardware_RtkHDMIRxManager_native_1setTargetFd },
    {"native_configureTargetFormat", "(IIIIIIIIIIIII)V", (void*) Java_com_realtek_hardware_RtkHDMIRxManager_native_1configureTargetFormat },
    {"native_setPlayback", "(ZZ)V",(void*) Java_com_realtek_hardware_RtkHDMIRxManager_native_1setPlayback },
    {"native_notifyHDMIRxAudioHotplug","(I)V",(void*) Java_com_realtek_hardware_RtkHDMIRxManager_native_notifyHDMIRxAudioHotplug},
};

void set_realtek_hardware_RtkHDMIRxManager_JavaVMInstance(JavaVM *vm)
{
    g_jvm = vm;
}

/*
* This is called by the VM when the shared library is first loaded.
*/
int register_realtek_hardware_RtkHDMIRxManager(JNIEnv *env) {

    jclass clazz = env->FindClass("com/realtek/hardware/RtkHDMIRxManager");

    if (clazz == NULL)
    {
        ALOGE("Can't find com/realtek/hardware/RtkHDMIRxManager");
        return -1;
    }

    if(jniRegisterNativeMethods(env, "com/realtek/hardware/RtkHDMIRxManager",
        RtkHDMIRxManagerMethods, NELEM(RtkHDMIRxManagerMethods)) != 0) {
        ALOGD("Register method to com/realtek/hardware/RtkHDMIRxManager failed!");
        return -1;
    }

    return 0;
}


} /* namespace android */
