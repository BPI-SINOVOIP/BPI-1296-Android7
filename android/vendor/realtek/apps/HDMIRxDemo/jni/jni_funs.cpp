#define LOG_TAG "HDMIRxDemo-JNI"
//#define LOG_NDEBUG 0

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <cutils/log.h>
#include <JNIHelp.h>

//#define CLASS_NAME_ACTIVITY     "com/example/hdmirxdemo/MainActivity3"
//#define CLASS_NAME_DBLISTENER   "com/example/hdmirxdemo/DbListenerImpl"

#define CLASS_NAME_UTILS            "com/example/hdmirxdemo/Utils"

static JavaVM *g_VM;

#define CREATE_CRASH 0

#define generate_crash() \
{ \
    char *pNull = (char *)0x2379; \
    *pNull = 100; \
}


static void native_prepare(JNIEnv * /*env*/ , jobject /*obj*/)
{
    ALOGV("call [%s]",__FUNCTION__);
}

static void native_init(JNIEnv * /*env*/ , jobject /*obj*/)
{
    ALOGV("call [%s]",__FUNCTION__);
}

// unused function
static void sendSchBC(JNIEnv * /*env*/, jobject /*obj*/, jint bc, jlong startTime, jlong endTime)
{
    const char* kCommand = "/system/bin/am";

    const char *broadcast;
    if(bc==0){
        broadcast = "android.intent.action.WAKE_UP_ADD_2";
    }else{
        broadcast = "android.intent.action.WAKE_UP_RM_2";
    }

    char start[128]={'\0'};
    char end[128]={'\0'};

    sprintf(start,"%lld",startTime);
    sprintf(end,"%lld",endTime);

    const char* const argv[] = {
            kCommand,
            "broadcast",
            "-a",
            broadcast,
            "--el",
            "timeStart",
            start,
            "--el",
            "timeEnd",
            end,
            NULL
    };


#if 0
    pid_t pid = fork();
    if (pid < 0) {
        ALOGE("[%s] fork error",__FUNCTION__);
        return;
    }else if (pid > 0) {
        int status;
        pid_t actualPid = TEMP_FAILURE_RETRY(waitpid(pid, &status, 0));
        if (actualPid != pid) {
            ALOGW("waitpid(%d) returned %d (errno=%d)", pid, actualPid, errno);
        } else if (status != 0) {
            ALOGW("'am broadcast' exited with status=%d", status);
        } else {
            ALOGV("'am broadcast' exited successfully");
        }
    }else
#endif
    {

#if 0
        ALOGV("closing stdout/stderr in child");
        int fd = open("/dev/null", O_WRONLY);
        if (fd >= 0) {
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }
#endif
        //execv(kCommand, const_cast<char* const*>(argv));
        //ALOGE("execv(%s) failed: %s\n", kCommand, strerror(errno));
        //exit(1);
    }
}

/*
static JNINativeMethod gDbListenerMethods[] = {
    {"sendSchBC","(IJJ)V",(void*)sendSchBC},
};

static JNINativeMethod gActivityMethods[] = {
    {"check","()V",(void*)native_check},
};
*/

static JNINativeMethod gUtilsMethods[] = {
    {"init","()V",(void*)native_init},
    {"prepare","()V",(void*)native_prepare},
};

static int register_jni(JNIEnv *env)
{
    return jniRegisterNativeMethods(env,CLASS_NAME_UTILS,gUtilsMethods,NELEM(gUtilsMethods));
}

extern "C" JNIEXPORT void JNI_OnUnload(JavaVM* /*vm*/, void* /*reserved*/)
{
    ALOGE("Call JNI_OnUnload!\n");
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env = NULL;
    jint result = -1;
    // cache vm pointer
    g_VM=vm;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	{
        ALOGE("GetEnv failed!\n");
        return result;
    }

    if(register_jni(env)<0)
    {
        return -1;
    }

    return JNI_VERSION_1_4;
}
