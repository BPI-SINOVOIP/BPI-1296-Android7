#include "rtksmb_jni.h"
#include "rtksmblib.h"

static jstring Samba_NetBiosName2Ip_jni(JNIEnv* env, jobject thiz, jstring NetBiosName)
{
	struct in_addr ia_ip;
	char *finialIP=0;
	
	char* pNetBiosName = (char*)env->GetStringUTFChars(NetBiosName, NULL);
	ALOGV("pNetBiosName = %s", pNetBiosName);
	int ret = Samba_NetBiosName2Ip(pNetBiosName, &ia_ip);
	env->ReleaseStringUTFChars(NetBiosName, pNetBiosName);
	ALOGV("ret of Samba_NetBiosName2Ip = %d", ret);
	if (ret == 0)
	{
		
		finialIP = inet_ntoa(ia_ip);
		return env->NewStringUTF(finialIP);
	}
	else
	{
		return env->NewStringUTF("0.0.0.0");
	}
}

static jlong Samba_QueryDomainList_jni(JNIEnv* env, jobject thiz, jobject num)
{
	jclass c = env->FindClass("java/lang/Integer");
    jfieldID id = env->GetFieldID(c, "value", "I");
	void *handle = NULL;
    int retNum = 0;
	int ret = Samba_QueryDomainList(&handle, &retNum);
	ALOGV("ret of Samba_QueryDomainList = %d", ret);
	if (ret == 0)
	{
		 env->SetIntField(num, id, retNum);
		 return (long)handle;
	}
	else
	{
		return 0;
	}
	
}


static jlong Samba_QueryServerList_jni(JNIEnv* env, jobject thiz, jobject num, jstring domain)
{
	jclass c = env->FindClass("java/lang/Integer");
    jfieldID id = env->GetFieldID(c, "value", "I");
	char* pDomain = (char*)env->GetStringUTFChars(domain, NULL);
	ALOGV("domain = %s", pDomain);
	void *handle = NULL;
    int retNum = 0;
	int ret = Samba_QueryServerList(&handle, &retNum, pDomain);
	ALOGV("ret of Samba_QueryServerList = %d", ret);
	env->ReleaseStringUTFChars(domain, pDomain);
	if (ret == 0)
	{
		 env->SetIntField(num, id, retNum);
		 return (long)handle;
	}
	else
	{
		return 0;
	}

}

static jlong Samba_QueryServerShareList_jni(JNIEnv* env, jobject thiz, jobject pRet, jobject num, jstring ip, jstring username, jstring password, jstring domain)
{
	jclass c = env->FindClass("java/lang/Integer");
    jfieldID id = env->GetFieldID(c, "value", "I");
	char* pIP = (char*)env->GetStringUTFChars(ip, NULL);
	char* pUserName = (char*)env->GetStringUTFChars(username, NULL);
	char* pPassword = (char*)env->GetStringUTFChars(password, NULL);
	char* pDomain = (char*)env->GetStringUTFChars(domain, NULL);
	ALOGV("ip = %s", pIP);
	ALOGV("username= %s", pUserName);
	ALOGV("password = %s", pPassword);
	ALOGV("domain = %s", pDomain);
	void *handle = NULL;
    int retNum = 0;
	int ret = Samba_QueryServerShareList(&handle, &retNum, pIP, pUserName, pPassword, pDomain);
	env->ReleaseStringUTFChars(ip, pIP);
	env->ReleaseStringUTFChars(username, pUserName);
	env->ReleaseStringUTFChars(password, pPassword);
	env->ReleaseStringUTFChars(domain, pDomain);

	jclass cRet = env->FindClass("java/lang/Integer");
    jfieldID fRet = env->GetFieldID(cRet, "value", "I");
	env->SetLongField(pRet, fRet, ret);
	if (ret == 0)
	{
		 env->SetIntField(num, id, retNum);
		 return (long)handle;
	}
	else
	{
		return 0;
	}
}

static jstring Samba_GetDomainList_jni(JNIEnv* env, jobject thiz, jlong handle, jint num)
{
	char *pDomain = NULL;
	ALOGV("num = %d", num);
	int ret = Samba_GetDomainList((void *)handle, num, &pDomain);
	if (ret == 0)
	{
		return env->NewStringUTF(pDomain);
	}
	else
	{
		return env->NewStringUTF("");
	}
}

static jstring Samba_GetServerList_jni(JNIEnv* env, jobject thiz, jlong handle, jint num)
{
	return Samba_GetDomainList_jni(env, thiz, handle, num);
}

static jstring Samba_GetServerShareList_jni(JNIEnv* env, jobject thiz, jlong handle, jint num, jobject type)
{
	char *pShare = NULL;
	int retType = 0;
	jclass c = env->FindClass("java/lang/Integer");
    jfieldID id = env->GetFieldID(c, "value", "I");
	ALOGV("num = %d", num);
	int ret = Samba_GetServerShareList((void *)handle, num, &pShare, &retType);

	if (ret == 0)
	{
		env->SetIntField(type, id, retType);
		return env->NewStringUTF(pShare);
	}
	else
	{
		return env->NewStringUTF("");
	}
	
}


static void Samba_freeHandle_jni(JNIEnv* env, jobject thiz, jlong handle)
{
	Samba_freeHandle((void *)handle);		
}

////////////////////////////////////////////////////////////////////////////////
//  Register Native Methods
////////////////////////////////////////////////////////////////////////////////
static JNINativeMethod sMethods[] =
{
     /* name, signature, funcPtr */
    { "Samba_NetBiosName2Ip", "(Ljava/lang/String;)Ljava/lang/String;", (void*)Samba_NetBiosName2Ip_jni},
    { "Samba_QueryDomainList", "(Ljava/lang/Object;)J", (void*)Samba_QueryDomainList_jni},
    { "Samba_QueryServerList", "(Ljava/lang/Object;Ljava/lang/String;)J", (void*)Samba_QueryServerList_jni},
    { "Samba_QueryServerShareList", "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)J", (void*)Samba_QueryServerShareList_jni},
    { "Samba_GetDomainList", "(JI)Ljava/lang/String;", (void*)Samba_GetDomainList_jni},
    { "Samba_GetServerList", "(JI)Ljava/lang/String;", (void*)Samba_GetServerList_jni},
    { "Samba_GetServerShareList", "(JILjava/lang/Object;)Ljava/lang/String;", (void*)Samba_GetServerShareList_jni},
	{ "Samba_freeHandle", "(J)V", (void*)Samba_freeHandle_jni},
};

void register_rtksmb(JNIEnv* env)
{
    jniRegisterNativeMethods(env, "com/realtek/rtksmb/SmbFunction", sMethods, NELEM(sMethods));
}
