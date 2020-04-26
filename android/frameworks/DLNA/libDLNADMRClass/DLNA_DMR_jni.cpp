#include <jni.h>
#include <JNIHelp.h>
#include "DLNA_DMR.h"

#define UPNP_PROC_STR_LEN        ENTRIES_LENGTH

////////////////////////////////////////////////////////////////////////////////
//  global value
////////////////////////////////////////////////////////////////////////////////
static RTK_DLNA_DMR* g_pDMR = NULL;
static jobject g_DMR_Obj = NULL;
static JavaVM * g_JVM = NULL;

static struct {
    jclass clazz;

    jmethodID notify;
} gDLNA_DMRClassInfo;

#define CLASS_NAME		"com/realtek/DLNA_DMR/DLNA_DMR"

#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className); \
        var = jclass(env->NewGlobalRef(var));

#define GET_STATIC_METHOD_ID(var, clazz, methodName, fieldDescriptor) \
        var = env->GetStaticMethodID(clazz, methodName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find static method" methodName);

#define GET_METHOD_ID(var, clazz, methodName, fieldDescriptor) \
        var = env->GetMethodID(clazz, methodName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find method" methodName);

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
	    var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
	    LOG_FATAL_IF(! var, "Unable to find field " fieldName);

#define NATIVE_METHOD(methodName, functionName, signature) \
    { #methodName, signature, reinterpret_cast<void*>(functionName) },

////////////////////////////////////////////////////////////////////////////////
//  Callback func
////////////////////////////////////////////////////////////////////////////////
bool callNotify(int notifyID, void *data)
{
//	ALOGI("The Notify:%d is called", notifyID);

	if (!g_JVM)
		return false;

 	JNIEnv *env = NULL;
	jint jni_result = g_JVM->GetEnv((void**) &env, JNI_VERSION_1_4);
	if (jni_result != JNI_OK) {
		ULOGE("The ERROR is %d", jni_result);
	}

	if (env)
	{
		jstring eventValue = NULL;
	 	if (data)
	 	{
	 		eventValue = env->NewStringUTF(reinterpret_cast<char*>(data));
	 	}
	 	bool bResult = env->CallStaticBooleanMethod(gDLNA_DMRClassInfo.clazz, gDLNA_DMRClassInfo.notify,
	 												notifyID, eventValue);
		if (eventValue)
			env->DeleteLocalRef(eventValue);

		return bResult;
	}

	return false;
}

int EventNotify(int eventID, void *data)
{
	char *pstrValue = NULL;
	char strValue[255];
	if (data)
	{
		switch (eventID)
		{
		case DLNA_DMR_NOTIFY_ID_SEEK:
			{
				long *pValue = reinterpret_cast<long*>(data);
				ULOGV("DLNA_DMR_NOTIFY_ID_SEEK (%ld)", *pValue);
				snprintf(strValue, sizeof(strValue), "%ld", *pValue);
				pstrValue = strValue;
			}
			break;
		case DLNA_DMR_NOTIFY_ID_SETRATE:
			{
				int *pValue = reinterpret_cast<int*>(data);
				ULOGV("DLNA_DMR_NOTIFY_ID_SETRATE (%d)", *pValue);
				snprintf(strValue, sizeof(strValue), "%d", *pValue);
				pstrValue = strValue;
			}
			break;
		case DLNA_DMR_NOTIFY_ID_SETVOLUME:
			{
				int *pValue = reinterpret_cast<int*>(data);
				ULOGV("DLNA_DMR_NOTIFY_ID_SETVOLUME (%d)", *pValue);
				snprintf(strValue, sizeof(strValue), "%d", *pValue);
				pstrValue = strValue;
			}
			break;
		case DLNA_DMR_NOTIFY_ID_SETMUTE:
			{
				int *pValue = reinterpret_cast<int*>(data);
				ULOGV("DLNA_DMR_NOTIFY_ID_SETMUTE (%d)", *pValue);
				snprintf(strValue, sizeof(strValue), "%d", *pValue);
				pstrValue = strValue;
			}
			break;
		case DLNA_DMR_NOTIFY_ID_SETCONTRAST:
			{
				int *pValue = reinterpret_cast<int*>(data);
				ULOGV("DLNA_DMR_NOTIFY_ID_SETCONTRAST (%d)", *pValue);
				snprintf(strValue, sizeof(strValue), "%d", *pValue);
				pstrValue = strValue;
			}
			break;
		case DLNA_DMR_NOTIFY_ID_SETBRIGHTNESS:
			{
				int *pValue = reinterpret_cast<int*>(data);
				ULOGV("DLNA_DMR_NOTIFY_ID_SETBRIGHTNESS (%d)", *pValue);
				snprintf(strValue, sizeof(strValue), "%d", *pValue);
				pstrValue = strValue;
			}
			break;
		}
	}

	callNotify(eventID, pstrValue);
	return 0;
}

int DmrRestart(int eventID, void *data)
{
	ULOGV("DLNA_DMR_NOTIFY_ID_RESTART_DMR");
	callNotify(DLNA_DMR_NOTIFY_ID_RESTART_DMR, NULL);
	return S_OK;
}

void DMRJNIInit(bool bInit)
{
	ULOGD("DMRJNIInit(%d) is called. PID(%d)", bInit, getpid());
	if (bInit)
	{
	 	JNIEnv *env = NULL;
	 	if (g_JVM && g_JVM->AttachCurrentThread(&env, NULL) == JNI_OK)
	 	{
			ULOGD("AttachCurrentThread is done.");
	 	}
	}
	else
	{
	 	if (g_JVM && g_JVM->DetachCurrentThread() == JNI_OK)
	 	{
			ULOGD("DetachCurrentThread is done.");
	 	}
	}
}

////////////////////////////////////////////////////////////////////////////////
//  Android Native API
////////////////////////////////////////////////////////////////////////////////
static void DLNA_DMR_Start(JNIEnv* env, jobject obj, jstring friendlyName, jstring uuid)
{
	ULOGD("DLNA_DMR_Start is called");

    if (!g_pDMR)
    {
        if (friendlyName == NULL ||
        	uuid == NULL)
        {
            jniThrowNullPointerException(env, NULL);
            return;
        }

		char *strFriendlyName = NULL, *strUUID = NULL;

    	const char* cstrValue = env->GetStringUTFChars(friendlyName, NULL);
    	strFriendlyName = strdup(cstrValue);
    	env->ReleaseStringUTFChars(friendlyName, cstrValue);

    	cstrValue = env->GetStringUTFChars(uuid, NULL);
   		strUUID = strdup(cstrValue);
		env->ReleaseStringUTFChars(uuid, cstrValue);

		if (strFriendlyName && strUUID)
		{
			g_pDMR = new RTK_DLNA_DMR(strFriendlyName, strUUID, DMRJNIInit);
			if (g_pDMR)
			{
				g_pDMR->RegisterDlnaDmrCallbackFunc(EventNotify, NULL, DLNA_DMR_CB_EVENT);
				g_pDMR->RegisterDlnaDmrCallbackFunc(DmrRestart, NULL, DLNA_DMR_CB_RESTART);
			}
		}

        if(strFriendlyName) {
			free(strFriendlyName);
		}
        if(strUUID) {
			free(strUUID);
		}
    }

    if (!g_DMR_Obj)
        g_DMR_Obj = env->NewGlobalRef(obj);
}

static void DLNA_DMR_Stop(JNIEnv* env, jobject obj)
{
	ULOGD("DLNA_DMR_Stop is called");

    if (g_pDMR)
    {
        delete g_pDMR;
        g_pDMR = NULL;
    }
	ULOGD("DLNA_DMR_Stop: g_pDMR is deleted ");

    if (!g_DMR_Obj)
    {
        env->DeleteGlobalRef(g_DMR_Obj);
        g_DMR_Obj = NULL;
    }

	ULOGD("DLNA_DMR_Stop: g_DMR_Obj is deleted ");
}

static jstring DLNA_DMR_GetAVTransportURI(JNIEnv* env, jobject obj)
{
    if (g_pDMR)
    {
	    char *pURI = g_pDMR->GetRenderMediaFullname();
	    if (pURI)
	    {
		    char tmp_filename[1024]; memset(tmp_filename, 0, sizeof(tmp_filename));
	    	sprintf(tmp_filename,"%s", pURI);

	    	SUBRENDERER_TYPE type = g_pDMR->GetRendererType();
	    	if (type == DMR_SUBR_AUDIO || type == DMR_SUBR_VIDEO)
	    	{
				char *pExtraString = NULL;
			#ifdef DLNADMRCTT
				pExtraString = " malloc=1 readTimeout=60 connectTimeout=60 DLNACTT";
			#else
		//	    pExtraString = " forceSeek readTimeout=20 connectTimeout=20 autoReconnect";
			    pExtraString = " forceSeek autoReconnect";
			#endif

				if (pExtraString) {
					strcat(tmp_filename, pExtraString);
				}

			    long filelength = g_pDMR->GetRenderMediaFilesize();
			    if(filelength > 0) {
					char tmp_string[64];
					sprintf(tmp_string, " contentlength=%ld", filelength);
					strcat(tmp_filename, tmp_string);
				}

				long duration = g_pDMR->GetRenderMediaDuration();
				if(duration > 0) {
					char tmp_string[64];
					sprintf(tmp_string, " duration=%ld", duration);
					strcat(tmp_filename, tmp_string);
				}
			}

	    	return env->NewStringUTF(tmp_filename);
	    }
    }

    return NULL;
}

static jstring DLNA_DMR_GetMIMEType(JNIEnv* env, jobject obj)
{
    if (g_pDMR)
    {
	    SUBRENDERER_TYPE type = g_pDMR->GetRendererType();
	    switch (type)
	    {
	    case DMR_SUBR_AUDIO:
    	case DMR_SUBR_IMAGE:
    	case DMR_SUBR_VIDEO:
			{
				char* pMimetype = g_pDMR->GetMediaMimeTypeProtocol();
				return env->NewStringUTF(pMimetype);
			}
		default:
			break;
	    }
    }

    return NULL;
}

static jstring DLNA_DMR_GetMediaProtocolInfo(JNIEnv* env, jobject obj)
{
	if (g_pDMR)
	{
	    const char *pPInfo = g_pDMR->GetMediaProtocolInfo();
		return env->NewStringUTF(pPInfo);
	}

	return NULL;
}

static void DLNA_DMR_UpdatePlaybackStatus(JNIEnv* env, jobject obj, jint typeID, jint value)
{
    if (g_pDMR)
    {
	    g_pDMR->UpdateStatus((DMR_UPDATE_STATUS_TYPE) typeID, (long) value);
    }
}

static jstring DLNA_DMR_GetMedialInfo(JNIEnv* env, jobject obj, jint typeID)
{
	enum {
		MedialInfoType_Filename,
		MedialInfoType_Filesize,
		MedialInfoType_Duration,
		MedialInfoType_ResolutinoX,
		MedialInfoType_ResolutinoY,
		MedialInfoType_ColorDepth,
	};

    char stringPool[128]; memset(stringPool, 0, sizeof(stringPool));
    char* retString = stringPool;
    if (g_pDMR)
    {
        switch (typeID)
        {
		case MedialInfoType_Filename:
			retString = (char*)g_pDMR->GetRenderMediaFilename();
			break;
		case MedialInfoType_Filesize:
			snprintf(stringPool, sizeof(stringPool), "%ld", g_pDMR->GetRenderMediaFilesize());
			break;
		case MedialInfoType_Duration:
			snprintf(stringPool, sizeof(stringPool), "%ld", g_pDMR->GetRenderMediaDuration());
			break;
		case MedialInfoType_ResolutinoX:
			snprintf(stringPool, sizeof(stringPool), "%d", g_pDMR->GetRenderMediaResolutinoX());
			break;
		case MedialInfoType_ResolutinoY:
			snprintf(stringPool, sizeof(stringPool), "%d", g_pDMR->GetRenderMediaResolutionY());
			break;
		case MedialInfoType_ColorDepth:
			snprintf(stringPool, sizeof(stringPool), "%d", g_pDMR->GetRenderMediaColorDepth());
			break;
		default:
			break;
        }
	}
    return env->NewStringUTF(retString);
}

static jstring DLNA_DMR_GetExtraPlaybackInfo(JNIEnv* env, jobject obj)
{
	if (g_pDMR)
	{
		if(g_pDMR->GetMediaTpye() == MEDIASUBTYPE_PCM)
		{
			NAVLPCMINFO mediaInfo;
			memset(&mediaInfo, 0, sizeof(NAVLPCMINFO));
			if(!g_pDMR->GetMediaMimeTypeProtocol())
			{
				ULOGD("[DMR avRenderer] Play PCM 6()");
				mediaInfo.bitsPerSample = g_pDMR->GetPCMBitPerSample();
				mediaInfo.samplingRate = g_pDMR->GetPCMSampleRate();
				mediaInfo.numChannels = g_pDMR->GetPCMChannel();
			}
			else if ( strcmp(g_pDMR->GetMediaMimeTypeProtocol(), MIME_TYPE_AUDIO_LPCM) == 0)
			{
				ULOGD("[DMR avRenderer] Play PCM 1()");
				mediaInfo.bitsPerSample = 16;
				mediaInfo.samplingRate = 44100;
				mediaInfo.numChannels = 2;
			}
			else if ( strcmp(g_pDMR->GetMediaMimeTypeProtocol(), MIME_TYPE_AUDIO_L16_SLPCM) == 0)
			{
				ULOGD("[DMR avRenderer] Play PCM 2()");
				mediaInfo.bitsPerSample = 16;
				mediaInfo.samplingRate = 44100;
				mediaInfo.numChannels = 1;
			}
			else if ( strcmp(g_pDMR->GetMediaMimeTypeProtocol(), MIME_TYPE_AUDIO_L16_48_SLPCM) == 0)
			{
				ULOGD("[DMR avRenderer] Play PCM 3()");
				mediaInfo.bitsPerSample = 16;
				mediaInfo.samplingRate = 48000;
				mediaInfo.numChannels = 1;
			}
			else if ( strcmp(g_pDMR->GetMediaMimeTypeProtocol(), MIME_TYPE_AUDIO_L16_48_LPCM) == 0)
			{
				ULOGD("[DMR avRenderer] Play PCM 4()");
				mediaInfo.bitsPerSample = 16;
				mediaInfo.samplingRate = 48000;
				mediaInfo.numChannels = 2;
			}
			else
			{
				ULOGD("[DMR avRenderer] Play PCM 5()");
				mediaInfo.bitsPerSample = g_pDMR->GetPCMBitPerSample();
				mediaInfo.samplingRate = g_pDMR->GetPCMSampleRate();
				mediaInfo.numChannels = g_pDMR->GetPCMChannel();
			}

			char strMediaInfo[512];
			snprintf(strMediaInfo, sizeof(strMediaInfo), "MediaType=%d,PCM.bitsPerSample=%d,PCM.samplingRate=%d,PCM.numChannels=%d", 
						MEDIASUBTYPE_PCM, mediaInfo.bitsPerSample, mediaInfo.samplingRate, mediaInfo.numChannels);
			return env->NewStringUTF(strMediaInfo);
	    }
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//  Register Native Methods
////////////////////////////////////////////////////////////////////////////////
static JNINativeMethod sMethods[] =
{
    NATIVE_METHOD(Start,				DLNA_DMR_Start,					"(Ljava/lang/String;Ljava/lang/String;)V")
    NATIVE_METHOD(Stop,					DLNA_DMR_Stop,					"()V")
    NATIVE_METHOD(GetAVTransportURI,	DLNA_DMR_GetAVTransportURI,		"()Ljava/lang/String;")
    NATIVE_METHOD(GetMIMEType,			DLNA_DMR_GetMIMEType,			"()Ljava/lang/String;")
    NATIVE_METHOD(GetMediaProtocolInfo,	DLNA_DMR_GetMediaProtocolInfo,	"()Ljava/lang/String;")
    NATIVE_METHOD(UpdatePlaybackStatus,	DLNA_DMR_UpdatePlaybackStatus,	"(II)V")
    NATIVE_METHOD(GetMediaInfo,			DLNA_DMR_GetMedialInfo,			"(I)Ljava/lang/String;")
    NATIVE_METHOD(GetExtraPlaybackInfo,	DLNA_DMR_GetExtraPlaybackInfo,	"()Ljava/lang/String;")
};

void register_DLNA_DMR(JNIEnv* env)
{
    env->GetJavaVM(&g_JVM);
    FIND_CLASS(gDLNA_DMRClassInfo.clazz, CLASS_NAME);

    GET_STATIC_METHOD_ID(gDLNA_DMRClassInfo.notify, gDLNA_DMRClassInfo.clazz,
			             "notify", "(ILjava/lang/String;)Z");

    jniRegisterNativeMethods(env, CLASS_NAME, sMethods, NELEM(sMethods));
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ULOGE("GetEnv failed!");
        return JNI_ERR;
    }

    ALOG_ASSERT(env, "Could not retrieve the env!");

    register_DLNA_DMR(env);
    return JNI_VERSION_1_4;
}

