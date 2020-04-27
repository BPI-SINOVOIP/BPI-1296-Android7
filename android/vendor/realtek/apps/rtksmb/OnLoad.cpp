#include "rtksmb_jni.h"

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("GetEnv failed!");
        return result;
    }

    ALOG_ASSERT(env, "Could not retrieve the env!");

    register_rtksmb(env);
    ALOGV("rtksmb onload OK!");
    return JNI_VERSION_1_4;
}
