
#define LOG_TAG "android_net_pppoe_natvie"

<<<<<<< HEAD
#include <stdio.h>
#include <string.h>
#include <log/log.h>
#include <sys/system_properties.h>
#include "nativehelper/JNIHelp.h"
#include "jni.h"

static jint DBG = 1; // false; update by toto
static const int NAP_TIME = 200;   /* wait for 200ms at a time */
                                  /* when polling for property values */

/*
 * Wait for a system property to be assigned a specified value.
 * If desired_value is NULL, then just wait for the property to
 * be created with any value. maxwait is the maximum amount of
 * time in seconds to wait before giving up.
 */
static int wait_for_property(const char *name, const char *desired_value, int maxwait)
{
    char value[PROP_VALUE_MAX] = {'\0'};
    int maxnaps = (maxwait * 1000) / NAP_TIME;

    if (maxnaps < 1) {
        maxnaps = 1;
    }

    while (maxnaps-- >= 0) {
        if (__system_property_get(name, value)) {
            if (desired_value == NULL ||
                    strcmp(value, desired_value) == 0) {
                return 0;
            }
        }
        if (maxnaps >= 0) {
            usleep(NAP_TIME * 1000);
        }
    }
    return -1; /* failure */
}
=======
#include <utils/misc.h>
#include <android_runtime/AndroidRuntime.h>
#include <utils/Log.h>
#include <cutils/properties.h>
#include "jni.h"

static jint DBG = true; // false; update by toto
static const int NAP_TIME = 200;   /* wait for 200ms at a time */
                                  /* when polling for property values */
>>>>>>> remotes/origin/QA160215

/*
 * Wait for a system property to be assigned a specified value.
 * If desired_value is NULL, then just wait for the property to
 * be created with any value. maxwait is the maximum amount of
 * time in seconds to wait before giving up.
 */
static int wait_for_property(const char *name, const char *desired_value, int maxwait)
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int maxnaps = (maxwait * 1000) / NAP_TIME;

    if (maxnaps < 1) {
        maxnaps = 1;
    }

    while (maxnaps-- >= 0) {
        if (property_get(name, value, NULL)) {
            if (desired_value == NULL ||
                    strcmp(value, desired_value) == 0) {
                return 0;
            }
        }
        if (maxnaps >= 0) {
            usleep(NAP_TIME * 1000);
        }
    }
    return -1; /* failure */
}

/*
static jint setupPppoeNative(JNIEnv* env, 
                                    jobject clazz,
                                    jstring iface, 
                                    jstring user,
                                    jstring password,                                    
                                    jstring dns1, 
                                    jstring dns2)
*/
static jint setupPppoeNative(JNIEnv* env, 
                                        jobject clazz,
                                        jstring iface, 
                                        jstring user,
                                        jstring password)
{
    int attempt;
    int err = 0;
    char cmd[PROP_VALUE_MAX] = {0};
    
    ALOGD("setupPppoeNative : here!!");
    char *c_iface = (char *)env->GetStringUTFChars(iface, NULL);
    char *c_user = (char *)env->GetStringUTFChars(user, NULL);
    char *c_password = (char *)env->GetStringUTFChars(password, NULL);
    /*
    char *c_dns1 = (char *)env->GetStringUTFChars(dns1, NULL);
    char *c_dns2 = (char *)env->GetStringUTFChars(dns2, NULL);

    if (!strcmp(c_dns1, "")) {
        strcpy(c_dns1, "server");
    }*/
    
    //sprintf(cmd, "pppoe_setup:%s %s no %s %s %s NONE", 
    //        c_user, c_iface, c_dns1, c_dns2, c_password);

    sprintf(cmd, "pppoe-setup:%s %s %s", c_iface, c_user, c_password);

    if(DBG) ALOGD("setupPppoeNative : setprop ctl.start %s", cmd);

    if (__system_property_set("ctl.start", cmd) < 0) {
        ALOGE("Failed to start pppoe-setup");
        err = -1;
        goto end;
    }

    if (wait_for_property("net.pppoe.status", "setuped", 5) < 0) {
        ALOGE("%s: Timeout waiting for pppoe-setup", __FUNCTION__);
        err = -1;
        goto end;
    }
 
    err = 0;
end:
    return err;
}

static jint startPppoeNative(JNIEnv* env, jobject clazz)
{
    int attempt;
    int err = 0;
    
    if(DBG) ALOGD("%s", __FUNCTION__);

    if (__system_property_set("ctl.start", "pppoe-start") < 0) {
        ALOGE("%s: Failed to start pppoe-start", __FUNCTION__);
        err = -1;
        goto end;
    }
    
    if (wait_for_property("net.pppoe.status", "connected", 30) < 0) {
        ALOGE("%s: Timeout waiting for pppoe-start", __FUNCTION__);
        err = -1;
        goto end;
    } else {
        ALOGE("%s: start pppoe-start is done!", __FUNCTION__);
    }

    err = 0;
end:
    return err;
}


static jint stopPppoeNative(JNIEnv* env, jobject clazz)
{
    int attempt;
    int err = 0;
    
    if(DBG) ALOGD("%s", __FUNCTION__);
    
    if (__system_property_set("ctl.start", "pppoe-stop") < 0) {
        ALOGE("Failed to start pppoe-stop");
        err = -1;
        goto end;
    }
    
    if (wait_for_property("net.pppoe.status", "disconnected", 10) < 0) {
        ALOGE("%s: Timeout waiting for pppoe-stop", __FUNCTION__);
        __system_property_set("ctl.stop", "pppoe-stop");
    } else {
        __system_property_set("ctl.stop", "pppoe-stop");
        err = 0;
        goto end;
    }
    
    err = -1;
end:
    return err;
}

static jint isUpPppoeNative(JNIEnv* env, jobject clazz)
{
    int attempt;
    int err = -1;
    char prop[PROP_VALUE_MAX] = {0};
 
    __system_property_get("net.pppoe.status", prop);
    if (!strcmp(prop, "connected")) {
        err = 1;
        goto end;
    }
end:
    ALOGD("%s : pppoe state=%s \n", __FUNCTION__, prop);
    return err;
}
   
/*---------------------------------------------------------------------------*/

/*
 * JNI registration.
 */
static JNINativeMethod gPppoeMethods[] = {
    /* name,                    method descriptor,                              funcPtr */
    { "setup",       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",    (void *)setupPppoeNative },
    { "start",       "()I",                                          (void *)startPppoeNative },
    { "stop",        "()I",                                          (void *)stopPppoeNative },
    { "isUp",        "()I",                                          (void *)isUpPppoeNative }
//  { "connectPppoe",        "()I",                                         	 (void *)connectPppoe },
};

int register_android_net_pppoe_natvie(JNIEnv* env)
{
    return jniRegisterNativeMethods(env, "com/android/server/PppoeService", gPppoeMethods, NELEM(gPppoeMethods) );
}
	
/* User to register native functions */
extern "C"
jint Java_com_android_server_pppoe_PppoeNative_registerNatives(JNIEnv* env, jclass clazz) {
    return jniRegisterNativeMethods(env,
            "com/android/server/pppoe/PppoeNative", gPppoeMethods, NELEM(gPppoeMethods));
}
