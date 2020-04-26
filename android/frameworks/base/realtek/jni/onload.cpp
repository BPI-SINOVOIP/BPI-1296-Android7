#include <JNIHelp.h>
#include <jni.h>
#include <utils/Log.h>
#include <utils/misc.h>

namespace android {
#ifdef ENABLE_RTK_HDMIRX_MANAGER
int register_realtek_hardware_RtkHDMIRxManager(JNIEnv* env);
void set_realtek_hardware_RtkHDMIRxManager_JavaVMInstance(JavaVM *vm);
#endif
#ifdef ENABLE_RTK_MIRACAST_SINK_SERVICE
int register_realtek_app_RtkMiracastSink(JNIEnv* env);
#endif
};

using namespace android;

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("GetEnv failed!");
        return result;
    }
    ALOG_ASSERT(env, "Could not retrieve the env!");

#ifdef ENABLE_RTK_HDMIRX_MANAGER
    if (register_realtek_hardware_RtkHDMIRxManager(env) < 0) {
        ALOGE("ERROR: RtkHDMIRxManager native registration failed\n");
        goto bail;
    }
    set_realtek_hardware_RtkHDMIRxManager_JavaVMInstance(vm);
#endif

#ifdef ENABLE_RTK_MIRACAST_SINK_SERVICE
    if (register_realtek_app_RtkMiracastSink(env) < 0) {
        ALOGE("ERROR: RtkMiracastSink native registration failed\n");
        goto bail;
    }
#endif

    result = JNI_VERSION_1_4;

    ALOGD("Successfully load JNI library");

    bail:
        return result;
}

