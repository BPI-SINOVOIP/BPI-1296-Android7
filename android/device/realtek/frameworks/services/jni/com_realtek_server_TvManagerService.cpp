#define LOG_TAG "TvManagerService"
#include <utils/Log.h>

#include "JNIHelp.h"
#include "jni.h"
#include <utils/Log.h>
#include <android_runtime/AndroidRuntime.h>
#include <ctype.h>
#include "RpcClient.h"
#include "RpcCommandDemuxer.h"
#include "DtvServiceDefs.h"
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dirent.h>

#include "RpcCallbackDecl.h"
DECLARE_RPC_CALLBACK_2_PARAM(SignalStautsCallback, int, SIGNAL_STATUS);
DECLARE_RPC_CALLBACK_3_PARAM(SiMessageCallback, int, int, int);
DECLARE_RPC_CALLBACK_2_PARAM(RecordEventCallback, int, int);


#define TVSERVER_NAME "RealtekRpcServer"
#define TVSERVER_NAME_ROOT "/tmp/RealtekRpcServerRoot"
#define TAG "TvJNI: "

using namespace rtk::rpc;

static RpcClient client;
static bool isConnected = false;

namespace android {

static jobject gTvManagerServiceObj = NULL;
static JavaVM * gVM;
static jmethodID gSignalStautsCallback;
static jmethodID gSiMessageCallback;
static jmethodID gRecordEventCallback;

static struct {
	jclass clazz;
	jmethodID init;
	jfieldID logicalChNum;
	jfieldID userChNum;
	jfieldID frequency;
	jfieldID bandwidth;
	jfieldID type;
	jfieldID flags;
	jfieldID channelNameLen;
	jfieldID channelName;
} gChInfo;
/*
static struct {
	jclass clazz;
	jmethodID init;
	jfieldID pid;
	jfieldID langCode;
	jfieldID shortLangStr;
	jfieldID isHardOfHearing;
} gDvbSubInfo;
*/
static struct {
	jclass clazz;
	jmethodID init;
	jfieldID isValid;
	jfieldID eventID;
	jfieldID beginDateTime;
	jfieldID endDateTime;
	jfieldID iso639LanguageCode;
	jfieldID titleLen;
	jfieldID title;
	jfieldID detailDescrLen;
	jfieldID detailDescr;
	jfieldID rating;
	jfieldID audioNum;
	jfieldID hasSubtitle;
	jfieldID hasTeletext;
	jfieldID hasTTXSubtitle;
	jfieldID isScrambled;
} gEpgProgram;

// Callbacks for DtvService on AndroidDvdPlayer -- start --
static void SignalStautsCallback(int tunerId, SIGNAL_STATUS status);
static void SiMessageCallback(int tunerId, int message, int data);
static void RecordEventCallback(int tunerId, int event);
// Callbacks for DtvService on AndroidDvdPlayer -- end --

static bool connect_TvServer()
{
    if (isConnected == false) {
        if (access(TVSERVER_NAME_ROOT, F_OK) != 0) {
            ALOGE("%s does not exist, RpcClient will not connect", TVSERVER_NAME_ROOT);

            return false;
        }

        ALOGW("trying to connect to %s", TVSERVER_NAME);
        if (client.Connect(TVSERVER_NAME, 0)) {
            ALOGW("connect to %s ok", TVSERVER_NAME);
            isConnected = true;

			client.Push(SignalStautsCallback);
			client.Push(SiMessageCallback);
			client.Push(RecordEventCallback);
			if (client.Invoke("RegisterCallback") == true) {
				ALOGW(TAG "RegisterCallback OK.\n");
			}
			else {
				ALOGE(TAG "RegisterCallback NG.\n");
			}
        }

    }

    return isConnected;
}

// Callbacks for DtvService on AndroidDvdPlayer -- start --
static void SignalStautsCallback(int tunerId, SIGNAL_STATUS status)
{
	JNIEnv *env;
	
	ALOGW(TAG "SignalStautsCallback.tunerId=%d.status=%d.", tunerId,(int)status);
	gVM->AttachCurrentThread(&env, NULL);
	env->CallVoidMethod(gTvManagerServiceObj, gSignalStautsCallback);
	gVM->DetachCurrentThread();
}

static void SiMessageCallback(int tunerId, int message, int data)
{
	JNIEnv *env;
	
	ALOGW(TAG "SiMessageCallback.tunerId=%d.message=%d.data=%d.", tunerId,message,data);
	gVM->AttachCurrentThread(&env, NULL);
	env->CallVoidMethod(gTvManagerServiceObj, gSiMessageCallback);
	gVM->DetachCurrentThread();
}

static void RecordEventCallback(int tunerId, int event)
{
	JNIEnv *env;
	
	ALOGW(TAG "RecordEventCallback.tunerId=%d.event=%d.", tunerId,event);
	gVM->AttachCurrentThread(&env, NULL);
	env->CallVoidMethod(gTvManagerServiceObj, gRecordEventCallback);
	gVM->DetachCurrentThread();
}
// Callbacks for DtvService on AndroidDvdPlayer -- end --

static jint realtek_server_TvManagerService_init(JNIEnv* env, jobject obj)
{
	if (gTvManagerServiceObj == NULL) {
		gTvManagerServiceObj = env->NewGlobalRef(obj);
		ALOGV(TAG "init.gTvManagerServiceObj=0x%p.", gTvManagerServiceObj);
	}

    if (connect_TvServer() != true) {
        ALOGE(TAG "connect error\n");
        return -1;
    }
    ALOGW(TAG "connect ok\n");
	
    return 0;
}

static jboolean realtek_server_TvManagerService_connectToServer(JNIEnv* env, jobject obj)
{
	if (gTvManagerServiceObj == NULL) {
		gTvManagerServiceObj = env->NewGlobalRef(obj);
		ALOGV(TAG "connectToServer.gTvManagerServiceObj=0x%p.", gTvManagerServiceObj);
	}

    if (connect_TvServer() != true) {
        ALOGE(TAG "connectToServer.connect_TvServer() NG.\n");
        return false;
    }
    ALOGW(TAG "connectToServer.connect_TvServer() OK.\n");
    return true;
}

static void realtek_server_TvManagerService_close(JNIEnv* env, jobject obj)
{
	if (isConnected == true) {
        client.Disconnect();
        isConnected = false;
    }
}

static jstring realtek_server_TvManagerService_getServerName(JNIEnv* env, jobject obj) {
	return env->NewStringUTF(client.GetServerName());
}

static jboolean realtek_server_TvManagerService_invoke(JNIEnv* env, jobject obj, jstring funcName)
{
    const char* funcNameStr = env->GetStringUTFChars(funcName, NULL);
    bool result = true;
    //int type;

    ALOGV(TAG "invoke %s", funcNameStr);

    if ((NULL == gTvManagerServiceObj) || (false == isConnected)) {
    	ALOGV(TAG "invoke.gTvManagerServiceObj==NULL or isConnected==flase.");
    	return false;
    }

    if (!client.Invoke(funcNameStr)) {
        ALOGE(TAG "invoke %s fail without result demuxer", funcNameStr);
    	return false;
    }

    if (strcmp(funcNameStr, "StopRpcServer") == 0) {
        ALOGW("force disconnect %s to suspend system", TVSERVER_NAME);
        isConnected = false;
	ALOGD(TAG "Disconnect TvServer\n");
	client.Disconnect();
    }

    env->ReleaseStringUTFChars(funcName, funcNameStr);

    return result;
}

static void realtek_server_TvManagerService_pushBoolean(JNIEnv* env, jobject obj, jboolean value)
{
    client.Push(value);
}

static void realtek_server_TvManagerService_pushInteger(JNIEnv* env, jobject obj, jint value)
{
    client.Push((int)value);
}

static void realtek_server_TvManagerService_pushLong(JNIEnv* env, jobject obj, jlong value)
{
	client.Push((long)value);
}

static void realtek_server_TvManagerService_pushDouble(JNIEnv* env, jobject obj, jdouble value)
{
	client.Push((double)value);
}

static void realtek_server_TvManagerService_pushFloat(JNIEnv* env, jobject obj, jfloat value)
{
	client.Push((float)value);
}

static void realtek_server_TvManagerService_pushChar(JNIEnv* env, jobject obj, jchar value)
{
	client.Push((char)value);
}

static void realtek_server_TvManagerService_pushShort(JNIEnv* env, jobject obj, jshort value)
{
	client.Push((short)value);
}

static void realtek_server_TvManagerService_pushString(JNIEnv* env, jobject obj, jstring value)
{
    const char* valueStr = env->GetStringUTFChars(value, NULL);

    ALOGW(TAG "String from java is %s", valueStr);
    client.Push((const char *)(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

// get primitive methods
static jboolean realtek_server_TvManagerService_getBoolean(JNIEnv* env, jobject obj)
{
    return client.GetReturn<bool>();
}

static jint realtek_server_TvManagerService_getInteger(JNIEnv* env, jobject obj)
{
    return client.GetReturn<int>();
}

static jlong realtek_server_TvManagerService_getLong(JNIEnv* env, jobject obj)
{
    return client.GetReturn<long>();
}

static jdouble realtek_server_TvManagerService_getDouble(JNIEnv* env, jobject obj)
{
	return client.GetReturn<double>();
}

static jfloat realtek_server_TvManagerService_getFloat(JNIEnv* env, jobject obj)
{
	return client.GetReturn<float>();
}

static jchar realtek_server_TvManagerService_getChar(JNIEnv* env, jobject obj)
{
	return client.GetReturn<char>();
}

static jshort realtek_server_TvManagerService_getShort(JNIEnv* env, jobject obj)
{
	return client.GetReturn<short>();
}

static jstring realtek_server_TvManagerService_getString(JNIEnv* env, jobject obj)
{
    const char* valueStr = client.GetReturn<const char*>();
    int i, len;
    if (!valueStr) {    // valueStr NULL
        ALOGW(TAG "valueStr is null, return empty string");

        return env->NewStringUTF("");
    }

    // check if it is a ascii string
    len = strlen(valueStr);
    for(i = 0; i < len; i++) {
        if (isascii(valueStr[i]) == 0) {
            ALOGW(TAG "idx %d: 0x%x is not an ascii, return empty string", i, (unsigned int)valueStr[i] & 0xff);
            return env->NewStringUTF("");
        }
    }

   return env->NewStringUTF(valueStr);
}

static jobject realtek_server_TvManagerService_getChannelInfo(JNIEnv* env, jobject obj)
{
	CHANNEL_INFO chInfo;
	jobject jInfo;

	chInfo = client.GetReturn<CHANNEL_INFO>();
	if (false == chInfo.isValid) {
		ALOGV(TAG "GetReturn<CHANNEL_INFO>().got NULL.\n");
		return NULL;
	}

	jInfo = env->NewObject(gChInfo.clazz, gChInfo.init);
	if (NULL == jInfo) {
		ALOGV(TAG "NewObject fail.\n");
		return NULL;
	}

	ALOGV(TAG "CHANNEL_INFO.logicalChNum=%d.userChNum=%d.frequency=%d.bandwidth=%d.type=%d.flags=%d.channelNameLen=%d.channelName=%s.\n", 
		chInfo.logicalChNum,chInfo.userChNum,chInfo.frequency,chInfo.bandwidth,chInfo.type,chInfo.flags,chInfo.channelNameLen,chInfo.channelName);
	env->SetIntField(jInfo, gChInfo.logicalChNum, chInfo.logicalChNum);
	env->SetIntField(jInfo, gChInfo.userChNum, chInfo.userChNum);
	env->SetIntField(jInfo, gChInfo.frequency, chInfo.frequency);
	env->SetIntField(jInfo, gChInfo.bandwidth, chInfo.bandwidth);
	env->SetIntField(jInfo, gChInfo.type, chInfo.type);
	env->SetIntField(jInfo, gChInfo.flags, chInfo.flags);
	env->SetIntField(jInfo, gChInfo.channelNameLen, chInfo.channelNameLen);
	
	jstring jstrTmp = env->NewStringUTF((const char*)(chInfo.channelName));
	env->SetObjectField(jInfo, gChInfo.channelName, jstrTmp);

	return jInfo;
}

/*
static jobject realtek_server_TvManagerService_getDvbSubInfo(JNIEnv* env, jobject obj)
{
	SUBTITLE_LIST dvbSubInfo;
	jobject jInfo;

	dvbSubInfo = client.GetReturn<SUBTITLE_LIST>();

	jInfo = env->NewObject(gDvbSubInfo.clazz, gDvbSubInfo.init);
	if (jInfo == NULL) {
		return NULL;
	}

	ALOGV(TAG "DvbSubInfo %d %d %s %d", dvbSubInfo.list[0].pid, dvbSubInfo.list[0].langCode, dvbSubInfo.list[0].shortLangStr, dvbSubInfo.list[0].isHardOfHearing);
	env->SetIntField(jInfo, gDvbSubInfo.pid, dvbSubInfo.list[0].pid);
	env->SetIntField(jInfo, gDvbSubInfo.langCode, dvbSubInfo.list[0].langCode);
	jstring jstrTmp = env->NewStringUTF((const char*)(dvbSubInfo.list[0].shortLangStr));
	env->SetObjectField(jInfo, gDvbSubInfo.shortLangStr, jstrTmp);
	env->SetIntField(jInfo, gDvbSubInfo.isHardOfHearing, dvbSubInfo.list[0].isHardOfHearing);

	return jInfo;
}
*/
static jobjectArray realtek_server_TvManagerService_getDvbSubInfo(JNIEnv* env, jobject obj) {
	SUBTITLE_LIST cDvbSubInfo;
	jobjectArray jArrDvbSubInfo;

	cDvbSubInfo = client.GetReturn<SUBTITLE_LIST>();
	if (0 == cDvbSubInfo.count) {
		ALOGV(TAG "GetReturn<SUBTITLE_LIST>().No dvb subtitles.\n");
		return NULL;
	}

	jclass jDvbSubInfoCls = env->FindClass("com/realtek/app/tv/DvbSubInfo");
	jmethodID mID = env->GetMethodID(jDvbSubInfoCls, "<init>", "()V");

	jArrDvbSubInfo = env->NewObjectArray(cDvbSubInfo.count, jDvbSubInfoCls, NULL);
	if (NULL == jArrDvbSubInfo) {
		ALOGV(TAG "NewObjectArray fail.\n");
		return NULL;
	}

	for (int i=0;i<cDvbSubInfo.count;i++) {
		jobject dvbSubInfoObj = env->NewObject(jDvbSubInfoCls, mID);
		
		jfieldID fid_pid = env->GetFieldID(jDvbSubInfoCls, "pid", "I");
		jfieldID fid_langCode = env->GetFieldID(jDvbSubInfoCls, "langCode", "I");
		jfieldID fid_shortLangStr = env->GetFieldID(jDvbSubInfoCls, "shortLangStr", "Ljava/lang/String;");
		jfieldID fid_isHardOfHearing = env->GetFieldID(jDvbSubInfoCls, "isHardOfHearing", "I");

		env->SetIntField(dvbSubInfoObj, fid_pid, cDvbSubInfo.list[i].pid);
		env->SetIntField(dvbSubInfoObj, fid_langCode, cDvbSubInfo.list[i].langCode);
		jstring jstrTmp = env->NewStringUTF((const char*)(cDvbSubInfo.list[i].shortLangStr));
		env->SetObjectField(dvbSubInfoObj, fid_shortLangStr, jstrTmp);
		env->SetIntField(dvbSubInfoObj, fid_isHardOfHearing, cDvbSubInfo.list[i].isHardOfHearing);

		env->SetObjectArrayElement(jArrDvbSubInfo, i, dvbSubInfoObj);

		env->DeleteLocalRef(dvbSubInfoObj);
	}

	return jArrDvbSubInfo;
}

static jobjectArray realtek_server_TvManagerService_getTtxSubInfo(JNIEnv* env, jobject obj) {
	TTX_SUBTITLE_LIST cTtxSubInfo;
	jobjectArray jArrTtxSubInfo;

	cTtxSubInfo = client.GetReturn<TTX_SUBTITLE_LIST>();
	if (0 == cTtxSubInfo.count) {
		ALOGV(TAG "GetReturn<TTX_SUBTITLE_LIST>().No ttx subtitles.\n");
		return NULL;
	}

	jclass jTtxSubInfoCls = env->FindClass("com/realtek/app/tv/TtxSubInfo");
	jmethodID mID = env->GetMethodID(jTtxSubInfoCls, "<init>", "()V");

	jArrTtxSubInfo = env->NewObjectArray(cTtxSubInfo.count, jTtxSubInfoCls, NULL);
	if (NULL == jArrTtxSubInfo) {
		ALOGV(TAG "NewObjectArray fail.\n");
		return NULL;
	}

	for (int i=0;i<cTtxSubInfo.count;i++) {
		jobject ttxSubInfoObj = env->NewObject(jTtxSubInfoCls, mID);
		
		jfieldID fid_pid = env->GetFieldID(jTtxSubInfoCls, "pid", "I");
		jfieldID fid_langCode = env->GetFieldID(jTtxSubInfoCls, "langCode", "I");
		jfieldID fid_shortLangStr = env->GetFieldID(jTtxSubInfoCls, "shortLangStr", "Ljava/lang/String;");
		jfieldID fid_magazineNum = env->GetFieldID(jTtxSubInfoCls, "magazineNum", "I");
		jfieldID fid_pageNum = env->GetFieldID(jTtxSubInfoCls, "pageNum", "I");

		env->SetIntField(ttxSubInfoObj, fid_pid, cTtxSubInfo.list[i].pid);
		env->SetIntField(ttxSubInfoObj, fid_langCode, cTtxSubInfo.list[i].langCode);
		jstring jstrTmp = env->NewStringUTF((const char*)(cTtxSubInfo.list[i].shortLangStr));
		env->SetObjectField(ttxSubInfoObj, fid_shortLangStr, jstrTmp);
		env->SetIntField(ttxSubInfoObj, fid_magazineNum, cTtxSubInfo.list[i].magazineNum);
		env->SetIntField(ttxSubInfoObj, fid_pageNum, cTtxSubInfo.list[i].pageNum);

		env->SetObjectArrayElement(jArrTtxSubInfo, i, ttxSubInfoObj);

		env->DeleteLocalRef(ttxSubInfoObj);
	}

	return jArrTtxSubInfo;
}

static jobjectArray realtek_server_TvManagerService_getTeletextInfo(JNIEnv* env, jobject obj) {
	TELETEXT_LIST cTtxSubInfo;
	jobjectArray jArrTtxSubInfo;

	cTtxSubInfo = client.GetReturn<TELETEXT_LIST>();
	if (0 == cTtxSubInfo.count) {
		ALOGV(TAG "GetReturn<TELETEXT_LIST>().No teletexts.\n");
		return NULL;
	}

	jclass jTtxSubInfoCls = env->FindClass("com/realtek/app/tv/TtxSubInfo");
	jmethodID mID = env->GetMethodID(jTtxSubInfoCls, "<init>", "()V");

	jArrTtxSubInfo = env->NewObjectArray(cTtxSubInfo.count, jTtxSubInfoCls, NULL);
	if (NULL == jArrTtxSubInfo) {
		ALOGV(TAG "NewObjectArray fail.\n");
		return NULL;
	}

	for (int i=0;i<cTtxSubInfo.count;i++) {
		jobject ttxSubInfoObj = env->NewObject(jTtxSubInfoCls, mID);
		
		jfieldID fid_pid = env->GetFieldID(jTtxSubInfoCls, "pid", "I");
		jfieldID fid_langCode = env->GetFieldID(jTtxSubInfoCls, "langCode", "I");
		jfieldID fid_shortLangStr = env->GetFieldID(jTtxSubInfoCls, "shortLangStr", "Ljava/lang/String;");
		jfieldID fid_magazineNum = env->GetFieldID(jTtxSubInfoCls, "magazineNum", "I");
		jfieldID fid_pageNum = env->GetFieldID(jTtxSubInfoCls, "pageNum", "I");

		env->SetIntField(ttxSubInfoObj, fid_pid, cTtxSubInfo.list[i].pid);
		env->SetIntField(ttxSubInfoObj, fid_langCode, cTtxSubInfo.list[i].langCode);
		jstring jstrTmp = env->NewStringUTF((const char*)(cTtxSubInfo.list[i].shortLangStr));
		env->SetObjectField(ttxSubInfoObj, fid_shortLangStr, jstrTmp);
		env->SetIntField(ttxSubInfoObj, fid_magazineNum, cTtxSubInfo.list[i].magazineNum);
		env->SetIntField(ttxSubInfoObj, fid_pageNum, cTtxSubInfo.list[i].pageNum);

		env->SetObjectArrayElement(jArrTtxSubInfo, i, ttxSubInfoObj);

		env->DeleteLocalRef(ttxSubInfoObj);
	}

	return jArrTtxSubInfo;
}

static jobjectArray realtek_server_TvManagerService_getAudioInfo(JNIEnv* env, jobject obj) {
	ALOGW(TAG "Enter!!\n");
	AUDIO_LIST cAudioInfo;
	jobjectArray jArrAudioInfo;

	cAudioInfo = client.GetReturn<AUDIO_LIST>();
	if (0 == cAudioInfo.count) {
		ALOGV(TAG "GetReturn<AUDIO_LIST>().No audios.\n");
		return NULL;
	}

	jclass jAudioInfoCls = env->FindClass("com/realtek/app/tv/AudioInfo");
	jmethodID mID = env->GetMethodID(jAudioInfoCls, "<init>", "()V");

	jArrAudioInfo = env->NewObjectArray(cAudioInfo.count, jAudioInfoCls, NULL);
	if (NULL == jArrAudioInfo) {
		ALOGV(TAG "NewObjectArray fail.\n");
		return NULL;
	}

	for (int i=0;i<cAudioInfo.count;i++) {
		jintArray javaLangCodeArray;
		jint* nativeLangCodeArray;
		jboolean isCopy;
		jobjectArray jArrShortLangStr;
	
		jobject audioInfoObj = env->NewObject(jAudioInfoCls, mID);
		
		jfieldID fid_pid = env->GetFieldID(jAudioInfoCls, "pid", "I");
		jfieldID fid_streamType = env->GetFieldID(jAudioInfoCls, "streamType", "I");
		jfieldID fid_type = env->GetFieldID(jAudioInfoCls, "type", "I");
		jfieldID fid_langCount = env->GetFieldID(jAudioInfoCls, "langCount", "I");
		jfieldID fid_langCode = env->GetFieldID(jAudioInfoCls, "langCode", "[I");
		jfieldID fid_shortLangStr = env->GetFieldID(jAudioInfoCls, "shortLangStr", "[Ljava/lang/String;");

		env->SetIntField(audioInfoObj, fid_pid, cAudioInfo.list[i].pid);
		env->SetIntField(audioInfoObj, fid_streamType, cAudioInfo.list[i].streamType);
		env->SetIntField(audioInfoObj, fid_type, cAudioInfo.list[i].type);
		env->SetIntField(audioInfoObj, fid_langCount, cAudioInfo.list[i].langCount);

		ALOGV(TAG "prepare new int array.\n");
		javaLangCodeArray = env->NewIntArray(2);
		if (NULL != javaLangCodeArray) {
			nativeLangCodeArray = env->GetIntArrayElements(javaLangCodeArray, &isCopy);
			ALOGV(TAG "AUDIO_INFO.langCode[0]=%d.langCode[1]=%d.size=%d.\n", cAudioInfo.list[i].langCode[0], cAudioInfo.list[i].langCode[1],sizeof(cAudioInfo.list[i].langCode));
			memcpy(nativeLangCodeArray, cAudioInfo.list[i].langCode, sizeof(cAudioInfo.list[i].langCode));
			ALOGV(TAG "int.langCode[0]=%d.langCode[1]=%d.\n", nativeLangCodeArray[0], nativeLangCodeArray[1]);

			env->SetObjectField(audioInfoObj, fid_langCode, javaLangCodeArray);
			
			env->ReleaseIntArrayElements(javaLangCodeArray, nativeLangCodeArray, 0);
		}

		// get 2 shorLangStr of AUDIO_INFO in cAudioInfo, transfer it to jstring, and set it to jArrShortLangStr.
		jArrShortLangStr = (jobjectArray)(env->GetObjectField(audioInfoObj, fid_shortLangStr));
		ALOGV(TAG "AUDIO_INFO.shortLangStr[0]=%s.shortLangStr[1]=%s.\n", cAudioInfo.list[i].shortLangStr[0], cAudioInfo.list[i].shortLangStr[1]);
		jstring jShortLangStr0 = env->NewStringUTF((const char*)(cAudioInfo.list[i].shortLangStr[0]));
		jstring jShortLangStr1 = env->NewStringUTF((const char*)(cAudioInfo.list[i].shortLangStr[1]));
		env->SetObjectArrayElement(jArrShortLangStr, 0, jShortLangStr0);		
		env->SetObjectArrayElement(jArrShortLangStr, 1, jShortLangStr1);
		ALOGV(TAG "pass 2.\n");

		env->SetObjectArrayElement(jArrAudioInfo, i, audioInfoObj);

		env->DeleteLocalRef(audioInfoObj);
	}

	return jArrAudioInfo;
}

static jobject realtek_server_TvManagerService_getEpgProgram(JNIEnv* env, jobject obj)
{
	EPG_PROGRAM epgProgram;
	jobject jInfo;

	epgProgram = client.GetReturn<EPG_PROGRAM>();
	if (false == epgProgram.isValid) {
		ALOGV(TAG "GetReturn<EPG_PROGRAM>().got NULL.\n");
		return NULL;
	}

	jInfo = env->NewObject(gEpgProgram.clazz, gEpgProgram.init);
	if (NULL == jInfo) {
		ALOGV(TAG "NewObject fail.\n");
		return NULL;
	}

	ALOGV(TAG "EPG_PROGRAM.isValid=%d.eventID=%d.beginDateTime=%d.endDateTime=%d.iso639LanguageCode=%s.\n", 
		epgProgram.isValid,epgProgram.eventID,epgProgram.beginDateTime,epgProgram.endDateTime,epgProgram.iso639LanguageCode);
	ALOGV(TAG "EPG_PROGRAM.titleLen=%d.title=%s.detailDescrLen=%d.detailDescr=%s.\n", 
		epgProgram.titleLen,epgProgram.title,epgProgram.detailDescrLen,epgProgram.detailDescr);
	ALOGV(TAG "EPG_PROGRAM.rating=%d.audioNum=%d.hasSubtitle=%d.hasTeletext=%d.hasTTXSubtitle=%d.isScrambled=%d.\n", 
		epgProgram.rating,epgProgram.audioNum,epgProgram.hasSubtitle,epgProgram.hasTeletext,epgProgram.hasTTXSubtitle,epgProgram.isScrambled);
	env->SetIntField(jInfo, gEpgProgram.isValid, epgProgram.isValid);
	env->SetIntField(jInfo, gEpgProgram.eventID, epgProgram.eventID);
	env->SetIntField(jInfo, gEpgProgram.beginDateTime, epgProgram.beginDateTime);
	env->SetIntField(jInfo, gEpgProgram.endDateTime, epgProgram.endDateTime);
	jstring jstrLang = env->NewStringUTF((const char*)(epgProgram.iso639LanguageCode));
	env->SetObjectField(jInfo, gEpgProgram.iso639LanguageCode, jstrLang);

	env->SetIntField(jInfo, gEpgProgram.titleLen, epgProgram.titleLen);
	jstring jstrTitle = env->NewStringUTF((const char*)(epgProgram.title));
	env->SetObjectField(jInfo, gEpgProgram.title, jstrTitle);

	env->SetIntField(jInfo, gEpgProgram.detailDescrLen, epgProgram.detailDescrLen);
	jstring jstrDescr = env->NewStringUTF((const char*)(epgProgram.detailDescr));
	env->SetObjectField(jInfo, gEpgProgram.detailDescr, jstrDescr);

	env->SetByteField(jInfo, gEpgProgram.rating, epgProgram.rating);
	env->SetByteField(jInfo, gEpgProgram.audioNum, epgProgram.audioNum);
	env->SetByteField(jInfo, gEpgProgram.hasSubtitle, epgProgram.hasSubtitle);
	env->SetByteField(jInfo, gEpgProgram.hasTeletext, epgProgram.hasTeletext);
	env->SetByteField(jInfo, gEpgProgram.hasTTXSubtitle, epgProgram.hasTTXSubtitle);
	env->SetByteField(jInfo, gEpgProgram.isScrambled, epgProgram.isScrambled);

	return jInfo;
}

static JNINativeMethod sMethods[] = {
     /* name, signature, funcPtr */
    { "init", "()I", (void*)realtek_server_TvManagerService_init },
    { "connectToServer", "()Z", (void*)realtek_server_TvManagerService_connectToServer },
    { "close", "()V", (void*)realtek_server_TvManagerService_close },
	{ "getServerName", "()Ljava/lang/String;", (void*)realtek_server_TvManagerService_getServerName },
    { "invoke", "(Ljava/lang/String;)Z", (void*) realtek_server_TvManagerService_invoke },

    { "pushBoolean", "(Z)V", (void*) realtek_server_TvManagerService_pushBoolean },
    { "pushInteger", "(I)V", (void*) realtek_server_TvManagerService_pushInteger },
	{ "pushLong", "(J)V", (void*) realtek_server_TvManagerService_pushLong },
	{ "pushDouble", "(D)V", (void*) realtek_server_TvManagerService_pushDouble},
	{ "pushFloat", "(F)V", (void*) realtek_server_TvManagerService_pushFloat},
	{ "pushChar", "(C)V", (void*) realtek_server_TvManagerService_pushChar},
	{ "pushShort", "(S)V", (void*) realtek_server_TvManagerService_pushShort},
    { "pushString", "(Ljava/lang/String;)V", (void*) realtek_server_TvManagerService_pushString },

    { "getBoolean", "()Z", (void*) realtek_server_TvManagerService_getBoolean },
    { "getInteger", "()I", (void*) realtek_server_TvManagerService_getInteger },
	{ "getLong", "()J", (void*) realtek_server_TvManagerService_getLong },
	{ "getDouble", "()D", (void*) realtek_server_TvManagerService_getDouble},
	{ "getFloat", "()F", (void*) realtek_server_TvManagerService_getFloat},
	{ "getChar", "()C", (void*) realtek_server_TvManagerService_getChar},
	{ "getShort", "()S", (void*) realtek_server_TvManagerService_getShort},
	{ "getString", "()Ljava/lang/String;", (void*)realtek_server_TvManagerService_getString },
    { "getChannelInfo", "()Lcom/realtek/app/tv/DvbChannelInfo;", (void*) realtek_server_TvManagerService_getChannelInfo },
    //{ "getDvbSubInfo", "()Lcom/realtek/app/tv/DvbSubInfo;", (void*) realtek_server_TvManagerService_getDvbSubInfo },
    { "getDvbSubInfo", "()[Lcom/realtek/app/tv/DvbSubInfo;", (void*) realtek_server_TvManagerService_getDvbSubInfo },
    { "getTtxSubInfo", "()[Lcom/realtek/app/tv/TtxSubInfo;", (void*) realtek_server_TvManagerService_getTtxSubInfo },
    { "getTeletextInfo", "()[Lcom/realtek/app/tv/TtxSubInfo;", (void*) realtek_server_TvManagerService_getTeletextInfo },
    { "getAudioInfo", "()[Lcom/realtek/app/tv/AudioInfo;", (void*) realtek_server_TvManagerService_getAudioInfo },
    { "getEpgProgram", "()Lcom/realtek/app/tv/EpgProgram;", (void*) realtek_server_TvManagerService_getEpgProgram },
};

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
        var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find field " fieldName);

int register_android_server_TvManagerService(JNIEnv* env)
{
    jclass clazz = env->FindClass("com/realtek/server/TvManagerService");

    if (clazz == NULL)
    {
        ALOGE("Can't find com/realtek/server/TvManagerService");
        return -1;
    }
	
	env->GetJavaVM(&gVM);

	// Callbacks for DtvService on AndroidDvdPlayer -- start --
	gSignalStautsCallback = env->GetMethodID(clazz, "SignalStautsCallback", "(II)V");
	gSiMessageCallback = env->GetMethodID(clazz, "SiMessageCallback", "(III)V");
	gRecordEventCallback = env->GetMethodID(clazz, "RecordEventCallback", "(II)V");
	// Callbacks for DtvService on AndroidDvdPlayer -- end --	

    gChInfo.clazz = env->FindClass("com/realtek/app/tv/DvbChannelInfo");
    LOG_FATAL_IF(gChInfo.clazz == NULL, "Unable to find class com.realtek.app.tv.DvbChannelInfo");
    gChInfo.clazz = (jclass) env->NewGlobalRef(gChInfo.clazz);
    gChInfo.init = env->GetMethodID(gChInfo.clazz, "<init>", "()V");
    LOG_FATAL_IF(gChInfo.init == NULL, "Unable to find constructor for com.realtek.app.tv.DvbChannelInfo");
    GET_FIELD_ID(gChInfo.logicalChNum, gChInfo.clazz, "logicalChNum", "I");
	GET_FIELD_ID(gChInfo.userChNum, gChInfo.clazz, "userChNum", "I");
	GET_FIELD_ID(gChInfo.frequency, gChInfo.clazz, "frequency", "I");
	GET_FIELD_ID(gChInfo.bandwidth, gChInfo.clazz, "bandwidth", "I");
	GET_FIELD_ID(gChInfo.type, gChInfo.clazz, "type", "I");
	GET_FIELD_ID(gChInfo.flags, gChInfo.clazz, "flags", "I");
	GET_FIELD_ID(gChInfo.channelNameLen, gChInfo.clazz, "channelNameLen", "I");    
    GET_FIELD_ID(gChInfo.channelName, gChInfo.clazz, "channelName", "Ljava/lang/String;");
    
/*
    gDvbSubInfo.clazz = env->FindClass("com/realtek/app/tv/DvbSubInfo");
    LOG_FATAL_IF(gDvbSubInfo.clazz == NULL, "Unable to find class com.realtek.app.tv.DvbSubInfo");
    gDvbSubInfo.clazz = (jclass) env->NewGlobalRef(gDvbSubInfo.clazz);
    gDvbSubInfo.init = env->GetMethodID(gDvbSubInfo.clazz, "<init>", "()V");
    LOG_FATAL_IF(gDvbSubInfo.init == NULL, "Unable to find constructor for com.realtek.app.tv.DvbSubInfo");
    GET_FIELD_ID(gDvbSubInfo.pid, gDvbSubInfo.clazz, "pid", "I");
    GET_FIELD_ID(gDvbSubInfo.langCode, gDvbSubInfo.clazz, "langCode", "I");
    GET_FIELD_ID(gDvbSubInfo.shortLangStr, gDvbSubInfo.clazz, "shortLangStr", "Ljava/lang/String;");
    GET_FIELD_ID(gDvbSubInfo.isHardOfHearing, gDvbSubInfo.clazz, "isHardOfHearing", "I");
*/
	gEpgProgram.clazz = env->FindClass("com/realtek/app/tv/EpgProgram");
	LOG_FATAL_IF(gEpgProgram.clazz == NULL, "Unable to find class com.realtek.app.tv.EpgProgram");
	gEpgProgram.clazz = (jclass) env->NewGlobalRef(gEpgProgram.clazz);
	gEpgProgram.init = env->GetMethodID(gEpgProgram.clazz, "<init>", "()V");
	LOG_FATAL_IF(gEpgProgram.init == NULL, "Unable to find constructor for com.realtek.app.tv.EpgProgram");
	GET_FIELD_ID(gEpgProgram.isValid, gEpgProgram.clazz, "isValid", "I");
	GET_FIELD_ID(gEpgProgram.eventID, gEpgProgram.clazz, "eventID", "I");
	GET_FIELD_ID(gEpgProgram.beginDateTime, gEpgProgram.clazz, "beginDateTime", "I");
	GET_FIELD_ID(gEpgProgram.endDateTime, gEpgProgram.clazz, "endDateTime", "I");
	GET_FIELD_ID(gEpgProgram.iso639LanguageCode, gEpgProgram.clazz, "iso639LanguageCode", "Ljava/lang/String;");
	GET_FIELD_ID(gEpgProgram.titleLen, gEpgProgram.clazz, "titleLen", "I");
	GET_FIELD_ID(gEpgProgram.title, gEpgProgram.clazz, "title", "Ljava/lang/String;");
	GET_FIELD_ID(gEpgProgram.detailDescrLen, gEpgProgram.clazz, "detailDescrLen", "I");
	GET_FIELD_ID(gEpgProgram.detailDescr, gEpgProgram.clazz, "detailDescr", "Ljava/lang/String;");
	GET_FIELD_ID(gEpgProgram.rating, gEpgProgram.clazz, "rating", "B");
	GET_FIELD_ID(gEpgProgram.audioNum, gEpgProgram.clazz, "audioNum", "B");
	GET_FIELD_ID(gEpgProgram.hasSubtitle, gEpgProgram.clazz, "hasSubtitle", "B");
	GET_FIELD_ID(gEpgProgram.hasTeletext, gEpgProgram.clazz, "hasTeletext", "B");
	GET_FIELD_ID(gEpgProgram.hasTTXSubtitle, gEpgProgram.clazz, "hasTTXSubtitle", "B");
	GET_FIELD_ID(gEpgProgram.isScrambled, gEpgProgram.clazz, "isScrambled", "B");

    return jniRegisterNativeMethods(env, "com/realtek/server/TvManagerService",
                                    sMethods, NELEM(sMethods));
}


} /* namespace android */
