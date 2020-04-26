#include "JNIHelp.h"
#include "jni.h"
#include "utils/Log.h"
#include "utils/misc.h"

namespace android {
#ifdef ENABLE_RTK_HDMI_SERVICE
int register_android_server_RtkHDMIService(JNIEnv* env);
void set_RtkHDMIService_gVm(JavaVM *vm);
#endif
#ifdef ENABLE_RTK_HDMIRX_SERVICE
int register_realtek_server_RtkHDMIRxService(JNIEnv* env);
#endif
#ifdef ENABLE_RTK_DPTX_SERVICE
int register_realtek_server_RtkDPTxService(JNIEnv* env);
#endif
int register_android_server_TvManagerService(JNIEnv* env);
int register_android_server_AirFunService(JNIEnv *env);
#ifdef USE_TOGO_MANAGER
int register_android_server_ToGoService(JNIEnv* env);
int register_android_server_ToGoProxyService(JNIEnv* env);
#endif
#ifdef USE_RTK_AOUT_UTIL
int register_android_server_RtkAoutUtilService(JNIEnv* env);
#endif
#ifdef USE_RTK_VOUT_UTIL
int register_android_server_RtkVoutUtilService(JNIEnv* env);
#endif
};

using namespace android;

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("GetEnv failed!");
        return result;
    }
    ALOG_ASSERT(env, "Could not retrieve the env!");
	
#ifdef ENABLE_RTK_HDMI_SERVICE
	if(register_android_server_RtkHDMIService(env) < 0) {
		ALOGE("ERROR: RtkHDMIService native registration failed\n");
		goto bail;
	}
    set_RtkHDMIService_gVm(vm);

#endif
#ifdef ENABLE_RTK_HDMIRX_SERVICE
    if (register_realtek_server_RtkHDMIRxService(env) < 0) {
        ALOGE("ERROR: RtkHDMIRxService native registration failed\n");
        goto bail;
    }
#endif
#ifdef ENABLE_RTK_DPTX_SERVICE
        if(register_realtek_server_RtkDPTxService(env) < 0) {
                ALOGE("ERROR: RtkDPTxService native registration failed\n");
                goto bail;
        }
#endif
#ifdef USE_TV_MANAGER
	if(register_android_server_TvManagerService(env) < 0) {
		ALOGE("ERROR: TvManagerService native registration failed\n");
		goto bail;
	}
#endif

#ifdef USE_AIRFUN_MANAGER
	if(register_android_server_AirFunService(env) < 0) {
		ALOGE("ERROR: TvManagerService native registration failed\n");
		goto bail;
	}
#endif

#ifdef USE_TOGO_MANAGER
	if (register_android_server_ToGoService(env) < 0) {
		ALOGE("ERROR: ToGoService native registration failed\n");
		goto bail;
	}
	if (register_android_server_ToGoProxyService(env) < 0) {
		ALOGE("ERROR: ToGoProxyService native registration failed\n");
		goto bail;
	}
#endif

#ifdef USE_RTK_AOUT_UTIL
	if(register_android_server_RtkAoutUtilService(env) < 0) {
		ALOGE("ERROR: RtkAoutUtilService native registration failed\n");
		goto bail;
	}
#endif
#ifdef USE_RTK_VOUT_UTIL
	if(register_android_server_RtkVoutUtilService(env) < 0) {
		ALOGE("ERROR: RtkVoutUtilService native registration failed\n");
		goto bail;
	}
#endif
	result = JNI_VERSION_1_4;
	
	ALOGD("Successfully load JNI library");

	bail:
		return result;
}
