#include <utils/Log.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dlfcn.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <cutils/properties.h>

#include <jni.h>
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
//#include "Platform_Lib/HDMIControl/hdmi.h"
//#include "Platform_Lib/HDMIControl/HDMIControl.h"
//#include <Application/AppClass/VoutUtil.h>
#include <Application/AppClass/AudioUtil.h>

namespace android {
    static JavaVM * gVM;

    static jboolean realtek_server_RtkAoutUtilService_init(JNIEnv *env, jobject obj) {
        ALOGD("enter realtek_server_RtkAoutUtilService_init()\n");
        return true;
    }

    static jboolean realtek_server_RtkAoutUtilService_setAudioSpdifOutput(JNIEnv *env, jobject obj, jint mode) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioSpdifOutput(), mode = %d\n", mode);
        SetAudioSpdifOutputMode((ENUM_AUDIO_DIGITAL_OUTPUT_MODE) mode);

        return true;
    }

    static jboolean realtek_server_RtkAoutUtilService_setAudioHdmiOutput(JNIEnv *env, jobject obj, jint mode) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioHdmiOutput(), mode = %d\n", mode);
        SetAudioHdmiOutputMode((ENUM_AUDIO_DIGITAL_OUTPUT_MODE) mode);

        return true;
    }

    static jboolean realtek_server_RtkAoutUtilService_setAudioAGC(JNIEnv *env, jobject obj, jint mode) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioAGC(), mode = %d\n", mode);
        SetAudioAGCMode((ENUM_AUDIO_AGC_MODE) mode);

        return true;
    }

    static jboolean realtek_server_RtkAoutUtilService_setAudioForceChannelCtrl(JNIEnv *env, jobject obj, jint mode) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioForceChannelCtrl(), mode = %02X\n", mode);
        SetAudioForceChannelCtrl(mode);

        return true;
    }

    static jboolean realtek_server_RtkAoutUtilService_setAudioHdmiFreqMode(JNIEnv *env, jobject obj) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioHdmiFreqMode()\n");
        SetAudioHdmiFreqMode();

        return true;
    }

    static void realtek_server_RtkAoutUtilService_setAudioSurroundSoundMode(JNIEnv *env, jobject obj, jint mode) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioSurroundSoundMode(), mode = %d\n", mode);
        SetAudioSurroundSoundMode((long)mode);
    }

    static void realtek_server_RtkAoutUtilService_setAudioNativeSR(JNIEnv *env, jobject obj, jint mode) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioNativeSR(), mode = %d\n", mode);
        SetAudioNativeSR(mode);
    }

    static void realtek_server_RtkAoutUtilService_setAudioDelay(JNIEnv *env, jobject obj, jint delay) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioDelay(), delay = %d\n", delay);
        SetAudioDelay(delay);
    }

    static void realtek_server_RtkAoutUtilService_setAudioHdmiMute(JNIEnv *env, jobject obj, jboolean bMute) {
        ALOGD("enter realtek_server_RtkAoutUtilService_setAudioHdmiMute(), bMute = %d\n", bMute);
        SetAudioHdmiMute(bMute);
    }
    /*
     * Array of methods.
     *
     * Each entry has three fields: the name of the method,
     * the method signature, and a pointer to the native implementation.
     */
    static const JNINativeMethod RtkAoutUtilServiceMethods[] = {
        /* name, signature, funcPtr */
        {"_init", "()Z", (void*) realtek_server_RtkAoutUtilService_init },
        {"_setAudioSpdifOutput", "(I)Z", (void*) realtek_server_RtkAoutUtilService_setAudioSpdifOutput },
        {"_setAudioHdmiOutput", "(I)Z", (void*) realtek_server_RtkAoutUtilService_setAudioHdmiOutput },
        {"_setAudioAGC", "(I)Z", (void*) realtek_server_RtkAoutUtilService_setAudioAGC },
        {"_setAudioForceChannelCtrl", "(I)Z", (void*) realtek_server_RtkAoutUtilService_setAudioForceChannelCtrl },
        {"_setAudioHdmiFreqMode", "()Z", (void*) realtek_server_RtkAoutUtilService_setAudioHdmiFreqMode },
        {"_setAudioSurroundSoundMode", "(I)V", (void*) realtek_server_RtkAoutUtilService_setAudioSurroundSoundMode },
        {"_setAudioNativeSR", "(I)V", (void*) realtek_server_RtkAoutUtilService_setAudioNativeSR },
        {"_setAudioDelay", "(I)V", (void*) realtek_server_RtkAoutUtilService_setAudioDelay },
        {"_setAudioHdmiMute", "(Z)V", (void*) realtek_server_RtkAoutUtilService_setAudioHdmiMute },
    };


    /*
     * This is called by the VM when the shared library is first loaded.
     */
    int register_android_server_RtkAoutUtilService(JNIEnv *env) {

        jclass clazz = env->FindClass("com/realtek/server/RtkAoutUtilService");

        if (clazz == NULL)
        {
            ALOGE("Can't find com/realtek/server/RtkAoutUtilService");
            return -1;
        }

        // get the VM pointer
        env->GetJavaVM(&gVM);

        if(jniRegisterNativeMethods(env, "com/realtek/server/RtkAoutUtilService",
                    RtkAoutUtilServiceMethods, NELEM(RtkAoutUtilServiceMethods)) != 0) {
            ALOGD("Register method to com/realtek/server/RtkAoutUtilService failed!");
            return -1;
        }

        return 0;
    }


} /* namespace android */
