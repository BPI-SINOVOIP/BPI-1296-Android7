#define LOG_NDEBUG 0
#define LOG_TAG "ToGoProxyService-jni"
#include <utils/Log.h>
#include <jni.h>
#include <JNIHelp.h>
#include <RtkIpc/IRecordBuffer.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/Surface.h>
#include <gui/SurfaceControl.h>
#include <android_runtime/AndroidRuntime.h>
#include <android_util_Binder.h>
#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>
#include "RtkIpc/RecordBufferService.h"
#include "RtkToGo.h"

namespace android {
#if 0
// RecordBufferService ------------------------------------------------------------
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
// RecordBufferService ------------------------------------------------------------
#endif

//static bool gbIsRecBufServiceRunning = false;
//static int gToGoHandle[MAX_TOGO_INSTANCES] = {-1, -1};
//static int gToGoType[MAX_TOGO_INSTANCES] = {-1, -1};
//static sp<SurfaceComposerClient> gComposerClient[MAX_TOGO_INSTANCES] = {NULL, NULL};
//static sp<SurfaceControl> gControl[MAX_TOGO_INSTANCES] = {NULL, NULL};
//static sp<Surface> gSurface[MAX_TOGO_INSTANCES] = {NULL, NULL};
static int gToGoHandle[MAX_TOGO_INSTANCES] = {-1};
static int gToGoType[MAX_TOGO_INSTANCES] = {-1};
static sp<SurfaceComposerClient> gComposerClient[MAX_TOGO_INSTANCES] = {NULL};
static sp<SurfaceControl> gControl[MAX_TOGO_INSTANCES] = {NULL};
static sp<Surface> gSurface[MAX_TOGO_INSTANCES] = {NULL};

int findAvailableSurfaceSpace()
{
    for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
        if (gToGoHandle[i] == -1) {
            return i;
        }
    }
    ALOGE("no available surface space.");
    return -1;
}

int getSurfaceSpaceByHandle(int handle)
{
    for (int i=0;i<MAX_TOGO_INSTANCES;i++) {
        if (gToGoHandle[i] == handle) {
            return i;
        }
    }
    ALOGE("no matched surface space.handle=%d.",handle);
    return -1;
}

JNIEXPORT jobject JNICALL Java_com_realtek_server_ToGoProxyService_jniGetGraphicBufferProducer(
    JNIEnv* env, jobject thiz, jint handle, jint type, jint width, jint height)
{
    ALOGV("enter.jniGetGraphicBufferProducer.handle=%d.type=%d.width=%d.height=%d.",handle,type,width,height);

    // RecordBufferService
#if 0
    if (!gbIsRecBufServiceRunning) {
        RecordBufferService::instantiate(MAX_TOGO_INSTANCES);
        ProcessState::self()->startThreadPool();
        ALOGV("RecordBufferService Server is up and running.");
        gbIsRecBufServiceRunning = true;
    }
#endif
    //

    int index = findAvailableSurfaceSpace();
    if (index < 0) {
        return NULL;
    }
    ALOGV("create surface on space=%d.",index);

    gComposerClient[index] = new SurfaceComposerClient;

    if (gComposerClient[index]->initCheck() != OK) {
        ALOGE("composerClient->initCheck() NG.");
        return NULL;
    }

    if (type == TOGO_TYPE_FILE) {
        sp<IBinder> display(SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
        DisplayInfo info;
        SurfaceComposerClient::getDisplayInfo(display, &info);
        ssize_t displayWidth = info.w;
        ssize_t displayHeight = info.h;
        ALOGD("display is %d x %d\n", displayWidth, displayHeight); //1280x720

        gControl[index] = gComposerClient[index]->createSurface(String8("ToGoSurface"), displayWidth, displayHeight, PIXEL_FORMAT_RGB_565, 0);
    }
    else {
        gControl[index] = gComposerClient[index]->createSurface(String8("ToGoSurface"), width, height, PIXEL_FORMAT_RGB_565, 0);
    }

    if ((gControl[index] == NULL) || (gControl[index]->isValid() == false)) {
        ALOGE("control == NULL or isValid() NG.");
        return NULL;
    }

    SurfaceComposerClient::openGlobalTransaction();

    if (gControl[index]->setLayer(INT_MAX) != OK) {
        ALOGE("composerClient->setLayer(INT_MAX) NG.");
        return NULL;
    }

    if (gControl[index]->show() != OK) {
        ALOGE("composerClient->show() NG.");
        return NULL;
    }

    SurfaceComposerClient::closeGlobalTransaction();

    gSurface[index] = gControl[index]->getSurface();
    if (gSurface[index] == NULL) {
        ALOGE("surface == NULL.");
        return NULL;
    }

    sp<IBinder> gbp = IInterface::asBinder(gSurface[index]->getIGraphicBufferProducer());
    ALOGV("IBinder of IGraphicBufferProducer=%p.",gbp.get());

    gToGoHandle[index] = handle;
    gToGoType[index] = type;
    ALOGV("occupy space=%d.handle=%d.type=%d.",index,gToGoHandle[index],gToGoType[index]);

    return javaObjectForIBinder(env, gbp);
}

JNIEXPORT void JNICALL Java_com_realtek_server_ToGoProxyService_jniClearGraphicBufferProducer(
    JNIEnv* env, jobject thiz, jint handle)
{
    ALOGV("enter.jniClearGraphicBufferProducer.handle=%d.",handle);

    int index = getSurfaceSpaceByHandle(handle);
    if (index < 0) {
        return;
    }
    ALOGV("release surface on space=%d.",index);

    if (gToGoType[index] == TOGO_TYPE_FILE) {
        gComposerClient[index]->dispose();
        ALOGV("do gComposerClient[index]->->dispose().");
    }

    gToGoHandle[index] = -1;
    gToGoType[index] = -1;
    gComposerClient[index] = NULL;
    gControl[index] = NULL;
    gSurface[index] = NULL;
}

static const char *classPathName = "com/realtek/server/ToGoProxyService";

static JNINativeMethod methods[] = {
     {"jniGetGraphicBufferProducer", "(IIII)Landroid/os/IBinder;" , (void*)Java_com_realtek_server_ToGoProxyService_jniGetGraphicBufferProducer},
     {"jniClearGraphicBufferProducer", "(I)V" , (void*)Java_com_realtek_server_ToGoProxyService_jniClearGraphicBufferProducer},
};

int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class=%s.", className);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for class=%s.", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

int register_android_server_ToGoProxyService(JNIEnv* env)
{
    if (!registerNativeMethods(env, classPathName, methods, sizeof(methods) / sizeof(methods[0]))) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

}   // namespace android
/*
typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    ALOGI("JNI_OnLoad.");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed.");
        goto bail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        ALOGE("ERROR: registerNatives failed.");
        goto bail;
    }

    result = JNI_VERSION_1_4;

bail:
    return result;
}
*/
