#include "DLNA_DMP_1p5_jni.h"

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ULOGE("GetEnv failed!");
        return result;
    }

    ALOG_ASSERT(env, "Could not retrieve the env!");

    register_DLNA_DMP_1p5(env);
    return JNI_VERSION_1_4;
}
