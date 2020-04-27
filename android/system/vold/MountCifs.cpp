#include "VolumeManager.h"
#include <utils/Log.h>
#include <nativehelper/jni.h>
#include <nativehelper/JNIHelp.h>
#include <android_runtime/AndroidRuntime.h>

using namespace android;

static jint cifsMount(JNIEnv* env,
                        jobject clazz,
                        jstring smbPath,
                        jstring mountPath,
                        jstring user,
                        jstring passwd,
                        jstring domain)
{
    return 0;
    const char *nsmbPath = env->GetStringUTFChars(smbPath, 0);
    const char *nmountPath = env->GetStringUTFChars(mountPath, 0);
    const char *nuser = env->GetStringUTFChars(user, 0);
    const char *npasswd = env->GetStringUTFChars(passwd, 0);
    const char *ndomain = env->GetStringUTFChars(domain, 0);
    VolumeManager *vm = VolumeManager::Instance();
    int res = vm->SmbMountShare(nsmbPath, nmountPath, nuser, npasswd, ndomain);
    env->ReleaseStringUTFChars(smbPath, nsmbPath);
    env->ReleaseStringUTFChars(mountPath, nmountPath);
    env->ReleaseStringUTFChars(user, nuser);
    env->ReleaseStringUTFChars(passwd, npasswd);
    env->ReleaseStringUTFChars(domain, ndomain);
    return res;
}

static jint cifsUmount(JNIEnv* env, jobject clazz)
{
    return 0;
    VolumeManager *vm = VolumeManager::Instance();
    return vm->SmbUmountShare();
}

/*---------------------------------------------------------------------------*/

/*
 * JNI registration.
 */
static JNINativeMethod gMethods[] = {
    /* name,                    method descriptor,                              funcPtr */
    { "cifsMount",       "(Ljava/lang/String;java/lang/String;java/lang/String;Ljava/lang/String;Ljava/lang/String;)I",    (void *)cifsMount },
    { "cifsUmount",       "()I",                                          (void *)cifsUmount },
};

/* User to register native functions */
extern "C"
jint Java_com_android_system_vold_MountCifs_registerNatives(JNIEnv* env, jclass clazz) {
    return AndroidRuntime::registerNativeMethods(env,
            "com/android/system/vold/MountCifs", gMethods, NELEM(gMethods));
}