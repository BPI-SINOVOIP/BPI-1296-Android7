/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "RTKBT"

#define LOG_NDEBUG 0

#include "utils/Log.h"
#include <string.h>
#include "com_android_bluetooth.h"
#include "hardware/bluetooth.h"
#include "hardware/bt_rtkbt.h"
#include "android_runtime/AndroidRuntime.h"

namespace android {
static jmethodID method_onGenericEvent;

static const bt_interface_t* sBtIf;
static rtkbt_interface_t* sRtkbtIf = NULL;

static jobject mCallbacksObj = NULL;
static JNIEnv *sCallbackEnv = NULL;

/*
*utilits function
*/
static void dumpData(jbyte* data, int len) {
       ALOGE(" the len is:%d, the data is : ", len);
       for(int i=0; i<len; i++)
              ALOGE("%2x ", data[i]);
}


static bool checkCallbackThread() {
    // Always fetch the latest callbackEnv from AdapterService.
    // Caching this could cause this sCallbackEnv to go out-of-sync
    // with the AdapterService's ENV if an ASSOCIATE/DISASSOCIATE event
    // is received
    sCallbackEnv = getCallbackEnv();

    JNIEnv* env = AndroidRuntime::getJNIEnv();
    if (sCallbackEnv != env || sCallbackEnv == NULL) return false;
    return true;
}

static int GenericEvent(int id, int event, unsigned char * data, int len) {
    jbyteArray evt_data;
    jbyte lenth = (jbyte)len;
    ALOGI("%s, data_len = %d", __FUNCTION__, len);

    if (!checkCallbackThread()) {
        ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
        return -1;
    }

    if(sCallbackEnv == NULL) {
        ALOGI("%s, sCallbackEnv is NULL!!!!", __FUNCTION__);
        return -2;
    }

    evt_data = sCallbackEnv->NewByteArray(lenth);
    if (!evt_data) {
        ALOGE("Fail to new jbyteArray GenericEvent");
        checkAndClearExceptionFromCallback(sCallbackEnv, __FUNCTION__);
        return -3;
    }

    sCallbackEnv->SetByteArrayRegion(evt_data, 0, lenth, (jbyte*)data);
    ALOGI("%s, CallVoidMethod1", __FUNCTION__);
    if(mCallbacksObj == NULL)
        ALOGI("%s, mCallbacksObj is NULL", __FUNCTION__);
    if(method_onGenericEvent == NULL)
        ALOGI("%s, method_onGenericEvent is NULL", __FUNCTION__);
    if(mCallbacksObj != NULL && method_onGenericEvent != NULL)
        sCallbackEnv->CallVoidMethod(mCallbacksObj, method_onGenericEvent, id, event, evt_data, len);

    ALOGI("%s, CallVoidMethod2", __FUNCTION__);
    checkAndClearExceptionFromCallback(sCallbackEnv, __FUNCTION__);
    sCallbackEnv->DeleteLocalRef(evt_data);
    return 0;
}

/*
*3dd callback
*/
static rtkbt_callbacks_t sRtkbtCallbacks = {
    sizeof(rtkbt_callbacks_t),
    GenericEvent,
};
static void classInitNative(JNIEnv* env, jclass clazz) {
    method_onGenericEvent = env->GetMethodID(clazz, "onGenericEvent","(II[BI)V");
}
/*
* native function definition
*/
static void InitNative(JNIEnv *env, jobject object) {
    int status;
    ALOGI("%s", __FUNCTION__);
    /*bluetooth interface provide by bluedroid*/
    if(sBtIf == NULL)  {
    if ( (sBtIf = getBluetoothInterface()) == NULL) {
        ALOGE("Bluetooth module is not loaded");
        return;
    }
    }

    if ( (sRtkbtIf = (rtkbt_interface_t *)
          sBtIf->get_profile_interface(BT_PROFILE_RTKBT_ID)) == NULL) {
        ALOGE("Failed to get RTKBT Interface");
        return;
    }
    if(sRtkbtIf->init)
        sRtkbtIf->init(&sRtkbtCallbacks);
    ALOGE("Bluetooth rtkbt successfully");
    mCallbacksObj = env->NewGlobalRef(object);
}

static void CleanupNative(JNIEnv *env, jobject object) {
    int status;

    if (mCallbacksObj != NULL) {
        env->DeleteGlobalRef(mCallbacksObj);
        mCallbacksObj = NULL;
    }
    if (!sBtIf) return;
    if (!sRtkbtIf) return;

    if(sRtkbtIf->cleanup)
        sRtkbtIf->cleanup();

    sBtIf = NULL;
    sRtkbtIf = NULL;
    return;
}

static int GetFeatureNative(JNIEnv *env, jobject object, jint id) {
    ALOGI("%s", __FUNCTION__);
    jbyte *data;
    uint8_t len;
    int res;
    if (!sBtIf) return -1;
    if (!sRtkbtIf) return -2;
    if(sRtkbtIf->GetFeature == NULL) return -3;
    ALOGI("GetFeatureNative,id=%d", id);

    res = sRtkbtIf->GetFeature(id);
    ALOGE("GetFeatureNative->GetFeature return res: %d", res);
    return res;
}

static int GenericCommandNative(JNIEnv *env, jobject object, jint id, jint command,jbyteArray args,jint len) {
    ALOGI("%s", __FUNCTION__);
    jbyte * data = NULL;
    int res;
    if (!sBtIf) return -1;
    if (!sRtkbtIf) return -2;
    if(sRtkbtIf->GenericCommand == NULL) return -3;

    /*get command data and length*/
    if(len > 0)
    {
        data = env->GetByteArrayElements(args, NULL);
        if (!data) {
            jniThrowIOException(env, EINVAL);
            return JNI_FALSE;
        }
    }else
        data = NULL;

    ALOGI("GenericCommandNative,id=%d len = %d", id, len);

    res = sRtkbtIf->GenericCommand(id, command, (uint8_t*)data, len);

    ALOGE("GenericCommandNative->GenericCommand return res: %d", res);
    if(len > 0)
        env->ReleaseByteArrayElements(args, data, 0);
    return res;
}

/**
 * JNI function definitinos
 */
static JNINativeMethod sMethods[] = {
    {"classInitNative", "()V", (void *) classInitNative},
    {"InitNative", "()V", (void *) InitNative},
    {"CleanupNative", "()V", (void *) CleanupNative},
    {"GetFeatureNative", "(I)I", (void *) GetFeatureNative},
    {"GenericCommandNative", "(II[BI)I", (void *) GenericCommandNative},
};

/*
* register 3dd native method and get java methods
*/
int register_com_android_bluetooth_rtkbt(JNIEnv* env)
{
    ALOGE("jniRegisterNativeMethods");
    return jniRegisterNativeMethods(env, "com/android/bluetooth/rtkbt/RtkbtService",
                                    sMethods, NELEM(sMethods));
}
}
