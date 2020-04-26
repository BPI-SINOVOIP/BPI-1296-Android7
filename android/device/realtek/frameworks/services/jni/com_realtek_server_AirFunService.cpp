///////////////////////////////////////////////////////////////////////////////
//
// Author:
//
// Created:
////////////////////////////////////////////////////////////////////////////////
#include <JNIHelp.h>
#include <string.h>
#include <utils/Log.h>
#include "AirFunIpc.h"

#define CLASS_NAME		"com/realtek/server/AirFunService"

#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className); \
        var = jclass(env->NewGlobalRef(var));

#define GET_STATIC_METHOD_ID(var, clazz, methodName, fieldDescriptor) \
        var = env->GetStaticMethodID(clazz, methodName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find static method" methodName);

namespace android {
static JavaVM * g_JVM = NULL;

static struct {
    jclass clazz;
    jmethodID notify;
} g_AirfunRClassInfo;

////////////////////////////////////////////////////////////////////////////////
//  Callback func
////////////////////////////////////////////////////////////////////////////////
bool callNotify(char *data)
{
//	ALOGI("The Notify:%d is called", notifyID);

	if (!g_JVM)
		return false;

	jstring dataString = NULL;
 	JNIEnv *env = NULL;
 	g_JVM->AttachCurrentThread(&env, NULL);
 	if (data)
 	{
 		dataString = env->NewStringUTF(data);
 	}
 	bool bResult = env->CallStaticBooleanMethod(g_AirfunRClassInfo.clazz, g_AirfunRClassInfo.notify, dataString);
	g_JVM->DetachCurrentThread();

	return bResult;
}

/*
 * Class:     com_realtek_server_AirFunService
 * Method:    StartServer
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_realtek_server_AirFunService_StartServer
  (JNIEnv *, jobject)
{
	AirFunIpc *afIpc = AirFunIpc::GetInstance();
	if( afIpc )
	{
		afIpc->SetNotifyFunc(callNotify);
		return 1;
	}
	else
		return 0;
}

/*
 * Class:     com_realtek_server_AirFunService
 * Method:    StopServer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_realtek_server_AirFunService_StopServer
  (JNIEnv *, jobject)
{
	AirFunIpc::DelInstance();
}

/*
 * Class:     com_realtek_server_AirFunService
 * Method:    UpdatePlaybackStatus
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_realtek_server_AirFunService_UpdatePlaybackStatus
  (JNIEnv *, jobject, jint typeId, jint value)
{
	(AirFunIpc::GetInstance())->UpdatePlaybackStatus(typeId, value);
}

/*
 * Class:     com_realtek_server_AirFunService
 * Method:    UpdatePlaybackStatusByMime
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_com_realtek_server_AirFunService_UpdatePlaybackStatusByMime
  (JNIEnv *, jobject, jint mimeId, jint typeId, jint value)
{
	(AirFunIpc::GetInstance())->UpdatePlaybackStatusByMime(mimeId, typeId, value);
}

/*
 * Class:     com_realtek_server_AirFunService
 * Method:    OpenAndStartDownload
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_realtek_server_AirFunService_OpenAndStartDownload
  (JNIEnv *env, jobject, jstring url)
{
	char filename[128];
	const char* cstrValue = env->GetStringUTFChars(url, NULL);

	//char *strFriendlyName = strdup(cstrValue);
	memset( filename, 0, sizeof(filename) );
	(AirFunIpc::GetInstance())->OpenAndDownloadImage( cstrValue, filename, sizeof(filename) );

	env->ReleaseStringUTFChars(url, cstrValue);

	/*
    if(strFriendlyName) {
		free(strFriendlyName);
	}
    */

    return env->NewStringUTF(filename);
}

/*
 * Class:     com_realtek_server_AirFunService
 * Method:    extractMetaData
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_realtek_server_AirFunService_extractMetaData
  (JNIEnv *env, jobject, jstring url)
{
	AirFunMetaData metaData;
	const char* cstrValue = env->GetStringUTFChars(url, NULL);

	memset( &metaData, 0, sizeof(metaData) );
	if( (AirFunIpc::GetInstance())->ExtractMetaData(cstrValue, &metaData) )
	{
		jobjectArray ret;

	    ret= (jobjectArray)env->NewObjectArray(3,env->FindClass("java/lang/String"),env->NewStringUTF(""));

	    env->SetObjectArrayElement(ret,0,env->NewStringUTF(metaData.artist));
	    env->SetObjectArrayElement(ret,1,env->NewStringUTF(metaData.album));
	    env->SetObjectArrayElement(ret,2,env->NewStringUTF(metaData.title));
	    return(ret);
	}
	else
	{
		return NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  Register Native Methods
////////////////////////////////////////////////////////////////////////////////
#define NATIVE_METHOD(methodName, signature, functionName ) \
    { #methodName, signature, reinterpret_cast<void*>(functionName) },

static JNINativeMethod sMethods[] =
{
    NATIVE_METHOD(startServer, "()Z", Java_com_realtek_server_AirFunService_StartServer)
    NATIVE_METHOD(stopServer, "()V", Java_com_realtek_server_AirFunService_StopServer)
    NATIVE_METHOD(updatePlaybackStatus, "(II)V", Java_com_realtek_server_AirFunService_UpdatePlaybackStatus)
    NATIVE_METHOD(updatePlaybackStatusByMime, "(III)V", Java_com_realtek_server_AirFunService_UpdatePlaybackStatusByMime)
    NATIVE_METHOD(openAndStartDownload, "(Ljava/lang/String;)Ljava/lang/String;", Java_com_realtek_server_AirFunService_OpenAndStartDownload)
    NATIVE_METHOD(extractMetaData, "(Ljava/lang/String;)[Ljava/lang/String;", Java_com_realtek_server_AirFunService_extractMetaData)
};

/*
* This is called by the VM when the shared library is first loaded.
*/
int register_android_server_AirFunService(JNIEnv *env) {

    // for C call Java
    env->GetJavaVM(&g_JVM);
    FIND_CLASS(g_AirfunRClassInfo.clazz, CLASS_NAME);

    GET_STATIC_METHOD_ID(g_AirfunRClassInfo.notify, g_AirfunRClassInfo.clazz,
			             "notify", "(Ljava/lang/String;)Z");

    // for Java call C
    jniRegisterNativeMethods(env, CLASS_NAME, sMethods, NELEM(sMethods));

	return 0;
}
} /* namespace android */

