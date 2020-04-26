#define LOG_TAG     "RtkHDMIService-JNI"
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
#include "Platform_Lib/HDMIControl/hdmi.h"
#include <Application/AppClass/VoutUtil.h>
#include <Application/AppClass/SetupClass.h>

#include <Platform_Lib/HDMIControl/HDMIController.h>
#include <Platform_Lib/HDMIControl/MHLController.h>

#ifdef HDCP1x_ENABLED
#include <Platform_Lib/HDMIControl/HDCPController.h>
#include <Platform_Lib/HDMIControl/RTKHDCPTx22/HDMI_HDCP_API.h>
#endif

#define JAVA_UPDATE_HDCP22_STATE_FUN        "native_updateHDCP22Enabled"
#define JAVA_CLASS_NAME                     "com/realtek/server/RtkHDMIService"

// dirty work-around, however, no one cares.
#define HDCP22_STATE_ENABLE             ( 0)
#define HDCP22_STATE_DISABLE            ( 1)
#define HDCP22_STATE_DISABLE_MANUAL     ( 2)
#define HDCP22_STATE_RESET_HDMI         ( 3)


#ifdef HDCP1x_ENABLED
// api in HDMIUtil
void jni_set_hdcp22_update_callback(hdcp22_update_callback cb);
#endif

namespace android {

static JavaVM* g_vm = NULL;
static jobject mThiz = NULL;

static int hdmi_initial_setup_menu_flag = -1;
static bool handleHdmiUnplgged = false;
static int mediaLinkMode = MEDIA_LINK_MODE_UNDEFINED;
static int tvSystemAutoMode = TV_SYSTEM_AUTO_MODE_HDMI_EDID;

static void reloadTVSystem(bool bCheck=true);
static int convertTVSystemToUserTvSystem(video_system videoSystem, video_standard videoStandard);
//static void setHdmiAVFormat(int lineNo);

#define tv_map_length 20
// map TV setting menu to video system and video standard
static int tv_map[tv_map_length][3] = {	{TV_SYS_NTSC, VIDEO_NTSC, VIDEO_INTERLACED},
						{TV_SYS_PAL, VIDEO_PAL, VIDEO_INTERLACED},
						{TV_SYS_480P, VIDEO_NTSC, VIDEO_PROGRESSIVE},
						{TV_SYS_576P, VIDEO_PAL, VIDEO_PROGRESSIVE},
						{TV_SYS_720P_50HZ, VIDEO_HD720_50HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_720P_60HZ, VIDEO_HD720_60HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_1080I_50HZ, VIDEO_HD1080_50HZ, VIDEO_INTERLACED},
						{TV_SYS_1080I_60HZ, VIDEO_HD1080_60HZ, VIDEO_INTERLACED},
						{TV_SYS_1080P_50HZ, VIDEO_HD1080_50HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_1080P_60HZ, VIDEO_HD1080_60HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_2160P_24HZ, VIDEO_HD2160_24HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_2160P_25HZ, VIDEO_HD2160_25HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_2160P_30HZ, VIDEO_HD2160_30HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_2160P_60HZ, VIDEO_HD2160_60HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_2160P_50HZ, VIDEO_HD2160_50HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_4096_2160P_24HZ, VIDEO_HD4096_2160_24HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_4096_2160P_25HZ, VIDEO_HD4096_2160_25HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_4096_2160P_30HZ, VIDEO_HD4096_2160_30HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_4096_2160P_50HZ, VIDEO_HD4096_2160_50HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_4096_2160P_60HZ, VIDEO_HD4096_2160_60HZ, VIDEO_PROGRESSIVE}};

// for MHL
#define mhl_tv_map_length 8
// map TV setting menu to video system and video standard
static int mhl_tv_map[mhl_tv_map_length][3] = {	{TV_SYS_NTSC, VIDEO_NTSC, VIDEO_INTERLACED},
						{TV_SYS_PAL, VIDEO_PAL, VIDEO_INTERLACED},
						{TV_SYS_480P, VIDEO_NTSC, VIDEO_PROGRESSIVE},
						{TV_SYS_576P, VIDEO_PAL, VIDEO_PROGRESSIVE},
						{TV_SYS_720P_50HZ, VIDEO_HD720_50HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_720P_60HZ, VIDEO_HD720_60HZ, VIDEO_PROGRESSIVE},
						{TV_SYS_1080I_50HZ, VIDEO_HD1080_50HZ, VIDEO_INTERLACED},
						{TV_SYS_1080I_60HZ, VIDEO_HD1080_60HZ, VIDEO_INTERLACED}};


static jboolean realtek_server_RtkHDMIService_turnOffHdmi(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_turnOffHdmi\n");
	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		HDMIController::sendAVMute(HDMI_SETAVM);
		HDMIController::setTVSupportxvYCC(0);
		HDMIController::setTVSupportDeepColorYCbCr444(0);
	} else {
		MHLController::sendAVMute(HDMI_SETAVM);
		MHLController::setTVSupportxvYCC(0);
		MHLController::setTVSupportDeepColorYCbCr444(0);
	}

	HDMI_TurnOffVideo();

	if(getWiDiMode())
	{
		system("rm -rf /tmp/edid.bin");
		//setHPDEvent(0);
	}

	return true;
}

static jboolean realtek_server_RtkHDMIService_checkIfHDCPKeyExist(JNIEnv *env, jobject obj) {
	HRESULT result = S_OK;
	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		result = HDMIController::HDCP_CkeckIfKeyExist();
	} else {
		result = MHLController::HDCP_CkeckIfKeyExist();
	}

	ALOGD("enter realtek_server_RtkHDMIService_checkIfHDCPKeyExist %.8x\n",result);

	if (result == S_OK)
		return true;
	else
		return false;
}


static jboolean realtek_server_RtkHDMIService_checkIfHDMIPlugged(JNIEnv *env, jobject obj) {
	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		if(HDMIController::isSinkConnected()) {
			// connected
			return true;
		} else {
			// disconnected
			return false;
		}
	} else {
		if(MHLController::isSinkConnected()) {
			// connected
			return true;
		} else {
			// disconnected
			return false;
		}
	}
}

static jboolean realtek_server_RtkHDMIService_checkIfHDMIReady(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_checkIfHDMIReady\n");
	if(getHDMIReady() == 1) {
		return true;
	}
	else {
		return false;
	}
}

static jboolean realtek_server_RtkHDMIService_checkIfHDMIEDIDReady(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_checkIfHDMIEDIDReady\n");
	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		if(HDMIController::isEDIDReady() == 1) {
			return true;
		}
		else {
			return false;
		}
	} else {
		if(MHLController::isEDIDReady() == 1) {
			return true;
		}
		else {
			return false;
		}
	}
}

static jboolean realtek_server_RtkHDMIService_sendAudioMute(JNIEnv *env, jobject obj, jint select) {
	ALOGD("enter realtek_server_RtkHDMIService_sendAudioMute: %d\n", select);

	jboolean result = false;

	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		result = HDMIController::sendAVMute(select);
	} else {
		result = MHLController::sendAVMute(select);
	}

	return result;
}

static void realtek_server_RtkHDMIService_setHDMIVideoColorMode(JNIEnv *env, jobject obj, jint mode) {
	ALOGD("enter realtek_server_RtkHDMIService_setHDMIVideoColorMode\n");
	setHDMIVideoColorMode((char)mode);
	reloadTVSystem();
}

static void realtek_server_RtkHDMIService_setHDMIDeepColorMode(JNIEnv *env, jobject obj, jint mode) {
	ALOGD("enter realtek_server_RtkHDMIService_setHDMIDeepColorMode\n");
	setHDMIDeepColorMode((char)mode);
	reloadTVSystem();

}

static void realtek_server_RtkHDMIService_setHDMIFormat3D(JNIEnv *env, jobject obj, jint format3d, jfloat fps) {
	ALOGD("enter realtek_server_RtkHDMIService_setHDMIFormat3D\n");
	
	VoutUtil::instance().SetFormat3d(format3d, fps);
	
}

static jint realtek_server_RtkHDMIService_setHDCPState(JNIEnv *env, jobject obj, jint state) {
	ALOGD("enter realtek_server_RtkHDMIService_setHDCPState\n");
	return HDMI_SetHDCPState((char) state);
}

static jobjectArray realtek_server_RtkHDMIService_getVideoFormat(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_getVideoFormat\n");

	jobjectArray arrRtkTVSystem;
	int len = 0;
	struct HDMI_TvSystemAndStandard arrTvSS[64];

	// Get the available TV Video format.
	if(HDMI_GetVideoFormat(&len, (HDMI_TvSystemAndStandard *)&arrTvSS) != 1 || len > 64) {
		len = 0;
	}

	jclass RtkTVSystemCls = env->FindClass("com/realtek/hardware/RtkTVSystem");
	jmethodID mID = env->GetMethodID(RtkTVSystemCls, "<init>", "()V"); //Constructor

	// Create the array of RtkTVSystem
	arrRtkTVSystem = env->NewObjectArray(len, RtkTVSystemCls, NULL);

	for(int i = 0; i < len; i++) {
		// create RtkTVSystem objects
		jobject RtkTVSystemObj = env->NewObject(RtkTVSystemCls, mID);

		// get the field of RtkTVSystem
		jfieldID fid_intVideoSystem = env->GetFieldID(RtkTVSystemCls, "intVideoSystem", "I");
		jfieldID fid_intVideoStandard = env->GetFieldID(RtkTVSystemCls, "intVideoStandard", "I");
		jfieldID fid_intIsWideScreen = env->GetFieldID(RtkTVSystemCls, "intIsWideScreen", "I");

		// copy the TV system and standard information into RtkTVSystemObject
		env->SetIntField(RtkTVSystemObj, fid_intVideoSystem, arrTvSS[i].videoSystem);
		env->SetIntField(RtkTVSystemObj, fid_intVideoStandard, arrTvSS[i].videoStandard);
		env->SetIntField(RtkTVSystemObj, fid_intIsWideScreen, arrTvSS[i].bIsWidescreen);

		// Set the object to the object array
		env->SetObjectArrayElement(arrRtkTVSystem, i, RtkTVSystemObj);

		env->DeleteLocalRef(RtkTVSystemObj);
	}

	return arrRtkTVSystem;
}

static jobject realtek_server_RtkHDMIService_getNextNVideoFormat(JNIEnv *env, jobject obj, jint nextN) {
	ALOGD("enter realtek_server_RtkHDMIService_getNextNVideoFormat\n");

	struct HDMI_TvSystemAndStandard strTvSS;

	HDMI_GetNextVideoFormat_N(&strTvSS, nextN);

	jclass RtkTVSystemCls = env->FindClass("com/realtek/hardware/RtkTVSystem");
	jmethodID mID = env->GetMethodID(RtkTVSystemCls, "<init>", "()V"); //Constructor

	// create RtkTVSystem object
	jobject rtkTVSystemObject = env->NewObject(RtkTVSystemCls, mID);

	// get the field of RtkTVSystem
	jfieldID fid_intVideoSystem = env->GetFieldID(RtkTVSystemCls, "intVideoSystem", "I");
	jfieldID fid_intVideoStandard = env->GetFieldID(RtkTVSystemCls, "intVideoStandard", "I");
	jfieldID fid_intIsWideScreen = env->GetFieldID(RtkTVSystemCls, "intIsWideScreen", "I");

	// copy the TV system and standard information into RtkTVSystemObject
	env->SetIntField(rtkTVSystemObject, fid_intVideoSystem, strTvSS.videoSystem);
	env->SetIntField(rtkTVSystemObject, fid_intVideoStandard, strTvSS.videoStandard);
	env->SetIntField(rtkTVSystemObject, fid_intIsWideScreen, strTvSS.bIsWidescreen);

	return rtkTVSystemObject;
}

static jint realtek_server_RtkHDMIService_getTVMaxLuminace(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_getTVMaxLuminace\n");

	int result;
	result = getTVMaxLuminace();

	return result;
}

static jint realtek_server_RtkHDMIService_getTV3DCapability(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_getTV3DCapability\n");

	int result;
	result = (int) getTV3DCapability();

	return result;
}

static jintArray realtek_server_RtkHDMIService_getTVSupport3D(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_getTVSupport3D\n");

	jintArray arrTVSupport3D;
	jint arr3DCapability[2] = {0};

	arrTVSupport3D = env->NewIntArray(2);
	if(arrTVSupport3D == NULL) {
		return NULL; // Out of memory
	}

	getTVSupport3D((unsigned char*)arr3DCapability);

	ALOGD("arr3DCapability[0]: %d\n", arr3DCapability[0]);
	ALOGD("arr3DCapability[1]: %d\n", arr3DCapability[1]);

	env->SetIntArrayRegion(arrTVSupport3D, 0, 2, arr3DCapability);

	return arrTVSupport3D;
}

static jintArray realtek_server_RtkHDMIService_getTVSupport3DResolution(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_getTVSupport3DResolution\n");

	jintArray arrTVSupport3DResolution;
	jint arrTV3DResolution[16] = {0};

	arrTVSupport3DResolution = env->NewIntArray(2);
	if(arrTVSupport3DResolution == NULL) {
		return NULL; // Out of memory
	}

	getTVSupport3DResolution((unsigned char *)&arrTV3DResolution);

	env->SetIntArrayRegion(arrTVSupport3DResolution, 0, 16, arrTV3DResolution);

	return arrTVSupport3DResolution;
}

static jint realtek_server_RtkHDMIService_initMediaLinkController(JNIEnv *env, jobject obj, jint mode) {
	ALOGD("realtek_server_RtkHDMIService_initMediaLinkController, mode: %d\n", mode);

	// update the link mode in JNI
	mediaLinkMode = mode;

	// update the link mode in HDMIUtil
	setUtilMediaLinkMode(mode);

	switch (mode) {
		case MEDIA_LINK_MODE_MHL:
			MHLController::initMHLController();
			break;
		case MEDIA_LINK_MODE_HDMI:
		default:
			HDMIController::initHDMIController();
			break;
	}

#ifdef HDCP1x_ENABLED
	// initialize \ establish the connection with HDCP HAL
	HDCPController::instance();
#endif

	return 0;
}

static void realtek_server_RtkHDMIService_nativeReloadTVSystem(JNIEnv *env, jobject obj, jboolean bResetVideoRect, jboolean saveTvSystemToNand, jboolean bCheckNand, jboolean bUpdateVideo, jint vSystem)
{
	ALOGD("realtek_server_RtkHDMIService_nativeReloadTVSystem\n");

	reloadTVSystem();


}

static void realtek_server_RtkHDMIService_handleHotPlug(JNIEnv *env, jobject obj, jint state)
{
	unsigned char EDID_buf[256] = {0};
	HRESULT sinkResult = S_FALSE;
	bool isSinkChange = false;

	ALOGD("handleHotPlug: %d\n", state);
	hdmi_initial_setup_menu_flag = 1;
	
	setHDMIPlugged(state);

	if(state) {	// hdmi is plugged in
		ALOGD("hdmi is plugged in\n");
		if(hdmi_initial_setup_menu_flag == 1) {
			struct HDMI_TvSystemAndStandard TvSS;
			handleHdmiUnplgged = true;
			if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
				sinkResult = HDMIController::getSinkCapability();
#ifndef KYLIN_TARGET_BOARD		
				/** Patch for Notify Rx physcal address**/
				{
#define VIDIOC_SET_EDID_PHYSICAL_ADDR  _IOWR('V', 105, int)
					int fd = open("/dev/video250", O_RDWR);
					int addr=getHDMISourceAddress();/* CEC Physical Addr: 1.0.0.0 */
					if (fd == -1) {
						ALOGE("Failed to open MIPI device: %s", strerror(errno));
					}
					else
					{
						int ret = ioctl(fd, VIDIOC_SET_EDID_PHYSICAL_ADDR, &addr);
						if(ret != 0){
							ALOGE("Failed to set physical address: %s", strerror(errno));
						}
						close(fd);
					}
				}
				/** Patch for Notify Rx physcal address**/
#endif				
			} else {
				sinkResult = MHLController::getSinkCapability();
			}
			HDMI_UpdateVDB();

			//save EDID raw data for Widi
			if(sinkResult == S_OK) {
				if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
					HDMIController::getEDID(EDID_buf);
					HDMIController::SaveEDIDToTmp(EDID_buf);
				} else {
					MHLController::getEDID(EDID_buf);
					MHLController::SaveEDIDToTmp(EDID_buf);
				}
			}

			// write EDID checksum to factory area for one_step
			isSinkChange = writeNandEDIDCheckSum(EDID_buf[127], EDID_buf[255]);

#if defined ONE_STEP
			VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT out;
			VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN disp_plane_id = {(VO_VIDEO_PLANE)0};
            VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM configOut;
			int realVid = 0;
			VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM nandTvSystemConfig;
			memset(&nandTvSystemConfig, 0 ,sizeof(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM));
			checkNandTVSystem(&nandTvSystemConfig, UPDATE_NAND_READ_ONLY);
			if (nandTvSystemConfig.hdmiInfo.hdmiMode == VO_HDMI_ON)
			{
				ALOGD("Call VoutUtil::instance().QueryDisplayWin().\n");
				HRESULT hr = VoutUtil::instance().QueryDisplayWin(disp_plane_id, &out);
                HRESULT hr2 = VoutUtil::instance().QueryConfigTVSystem(&configOut);
				if (hr != E_FAIL) {
					ALOGD("result: %d; standard: %d tv config standard %d; enProg: %d\n",
                        out.result,
                        out.standard,
                        configOut.videoInfo.standard,
                        out.enProg);
				}
				realVid = HDMI_ConvertVOtoVid(configOut.videoInfo.standard, out.enProg);
                ALOGD("realVid %d",realVid);

                if(configOut.hdmiInfo.hdmiMode != VO_HDMI_ON) {
                    ALOGD("HDMI is disabled now, reset realVid to 0");
                    realVid = 0;
                }
                ALOGD("realVid %d",realVid);

			}
			else
			{
				ALOGD("Set HDMI mode = %d before, don't QueryDisplayWin\n", nandTvSystemConfig.hdmiInfo.hdmiMode);
			}
#endif

			if(tvSystemAutoMode == TV_SYSTEM_AUTO_MODE_HDMI_EDID) {
				ALOGD("TV_SYSTEM_AUTO_MODE_HDMI_EDID\n");
#if defined ONE_STEP
				int linkStatus = LINK_MODE_UNKNOWN;

				if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
					linkStatus = HDMIController::checkLinkStatus();
				}

				// HDMI (ENABLED), the sink is the same, video format is correct: don't set TV system again.
				if(linkStatus ==  LINK_HDMI_ENABLED && !isSinkChange){
					if (HDMI_CheckAutoVideoFormat(realVid)) {
						if (HDMIController::getTVSupportHDR())
						{
							ALOGD("[HDMI] TV support HDR, need reload tv system\n");
							//reload TV system
							reloadTVSystem();
						}
						else
						{
							ALOGD("[HDMI] one step is already set up TV system: Auto\n");
						}
					} else if (IsCurrentHDMI3D()){
						ALOGD("[HDMI] Last time setting is 3D, just reload tv system\n");
						//reload TV system
						reloadTVSystem();
					} else {
						HDMI_GetAutoVideoFormat(&TvSS);
						// save the setting
						ALOGD("Update videoSystem: %d, videoStandard: %d\n", TvSS.videoSystem, TvSS.videoStandard);
						updateHDMISetting((int)TvSS.videoSystem, (int)TvSS.videoStandard);
						setHDMIVideoColorMode(255);
						SetupClass::GetInstance()->SetTvSystem(TvSS.videoSystem);
						SetupClass::GetInstance()->SetTvStandard(TvSS.videoStandard);
						//reload TV system
						reloadTVSystem();
					}
				} else {
#endif
					HDMI_GetAutoVideoFormat(&TvSS);
					// save the setting
					ALOGD("Update videoSystem: %d, videoStandard: %d\n", TvSS.videoSystem, TvSS.videoStandard);
					updateHDMISetting((int)TvSS.videoSystem, (int)TvSS.videoStandard);
					setHDMIVideoColorMode(255);
					SetupClass::GetInstance()->SetTvSystem(TvSS.videoSystem);
					SetupClass::GetInstance()->SetTvStandard(TvSS.videoStandard);
					//reload TV system
					reloadTVSystem();
#if defined ONE_STEP
				}
#endif
			} else {
				ALOGD("NOT TV_SYSTEM_AUTO_MODE_HDMI_EDID\n");
				char vid = HDMI_CheckVideoFormat();
				int userTVSystem = -1;
				char value[PROPERTY_VALUE_MAX];
				property_get("persist.rtk.hdmitx.ctsmode", value, "off");
				if (strcmp(value, "on") == 0)
				{
					ALOGD("[HDMI / MHL]: CTS mode on, just reload TV system\n");
					reloadTVSystem(false);
				}
				else if(vid) {
#if defined ONE_STEP
					int linkStatus = LINK_MODE_UNKNOWN;

					if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
						linkStatus = HDMIController::checkLinkStatus();
					}

					// HDMI (enabled), the sink is the same, video format is correct: don't set TV system again.\r
					if(linkStatus == LINK_HDMI_ENABLED && !isSinkChange && realVid == vid) {
						if (HDMIController::getTVSupportHDR())
						{
							ALOGD("[HDMI] TV support HDR, need reload tv system\n");
							//reload TV system
							reloadTVSystem();
						}
						else
						{
							ALOGD("[HDMI] one step is already set up TV system to Vid: %d\n", vid);	
						}
					} else {
#endif
						ALOGD("[HDMI / MHL]: reload TV system: %d\n", vid);
						setHDMIVideoColorMode(255);
						reloadTVSystem();
#if defined ONE_STEP
					}
#endif
				} else {
					ALOGD("[HDMI] Call HDMI_GetAutoVideoFormat() because current video system is not support in this HDMI TV system!! \n");
					HDMI_GetAutoVideoFormat(&TvSS);
					// save the setting
					ALOGD("Update videoSystem: %d, videoStandard: %d\n", TvSS.videoSystem, TvSS.videoStandard);
					updateHDMISetting((int)TvSS.videoSystem, (int)TvSS.videoStandard);
					setHDMIVideoColorMode(255);
					SetupClass::GetInstance()->SetTvSystem(TvSS.videoSystem);
					SetupClass::GetInstance()->SetTvStandard(TvSS.videoStandard);
					// update the setting item "realtek_setup_tv_system_user"
					userTVSystem = convertTVSystemToUserTvSystem(TvSS.videoSystem, TvSS.videoStandard);
					ALOGD("Update the item realtek_setup_tv_system_user: %d\n", userTVSystem);
					SetupClass::GetInstance()->SetUserTvSystem((ENUM_TV_SYSTEM_USER)userTVSystem);
					// reload TV system
					reloadTVSystem();
				}
			}

			//in case hdcp not enabled becuase of one step
			ALOGD("in case hdcp not enabled becuase of one step\n");
			setHDCPEnabled(true);

			// start MHL-RCP thread
			if(mediaLinkMode == MEDIA_LINK_MODE_MHL) {
				enableRCPThread(1);
			}

			hdmi_initial_setup_menu_flag = 0;
		}
	} else {	// hdmi is pulled out
		ALOGD("HDMI / MHL cable is pulled out\n");

		// start MHL-RCP thread
		if(mediaLinkMode == MEDIA_LINK_MODE_MHL) {
			enableRCPThread(0);
		}

		if(handleHdmiUnplgged == true) {
			if(tvSystemAutoMode == TV_SYSTEM_AUTO_MODE_HDMI_EDID) {
				// It was to change the TV system to VideoSystemManual(). We removed it for now. (Don't change TV system when HDMI is pulled out.)
				ALOGD("[HDMI] %s(%d) video is HDMI_AUTO=ON, do nothing here..!! \n", __func__, __LINE__);
			} else {
				ALOGD("[HDMI] %s(%d) video is HDMI_AUTO=OFF, do nothing here..!! \n", __func__, __LINE__);
			}
			handleHdmiUnplgged=false;
		}
	}
}

static jboolean realtek_server_RtkHDMIService_isTVSupport3D(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_isTVSupport3D\n");
	return isTVSupport3d();
}

static jboolean realtek_server_RtkHDMIService_saveHDCPToTmp(JNIEnv *env, jobject obj) {
#ifdef HDCP1x_ENABLED
	ALOGD("enter realtek_server_RtkHDMIService_saveHDCPToTmp\n");
	return HDCPController::instance().saveHDCPToTemp();
#else
	ALOGD("*** realtek_server_RtkHDMIService_saveHDCPToTmp: HDCP is NOT enabled.\n");
	return false;
#endif
}

static void realtek_server_RtkHDMIService_setHdcpEnabledByConfig(JNIEnv *env, jobject obj, jboolean enabled) {
#ifdef HDCP1x_ENABLED
	ALOGD("enter realtek_server_RtkHDMIService_saveHDCPToTmp, enabled: %d\n", enabled);
	setHDCPEnabled_by_MakeConfig(enabled);
#else
	ALOGD("*** realtek_server_RtkHDMIService_setHdcpEnabledByConfig: HDCP is NOT enabled.\n");
#endif
}

static jbyteArray realtek_server_RtkHDMIService_getEDIDRawData(JNIEnv *env, jobject obj) {
	ALOGD("enter realtek_server_RtkHDMIService_getEDIDRawData\n");

	jbyteArray arrEDIDRawData;
	unsigned char edidBuf[256] = {0};

	arrEDIDRawData = env->NewByteArray(256);
	if(arrEDIDRawData == NULL) {
		return NULL; // Out of memory
	}

	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		HDMIController::getEDID(edidBuf);
	} else {
		MHLController::getEDID(edidBuf);
	}

	ALOGD("The first 8 byte edid: %d, %d, %d, %d, %d, %d, %d, %d", edidBuf[0], edidBuf[1], edidBuf[2], edidBuf[3], edidBuf[4], edidBuf[5], edidBuf[6], edidBuf[7]);
	env->SetByteArrayRegion(arrEDIDRawData, 0, 256, reinterpret_cast<jbyte*>(edidBuf));

	return arrEDIDRawData;
}

static void realtek_server_RtkHDMIService_updateHDMISetting(JNIEnv *env, jobject obj, int videoSystem, int videoStandard, int autoMode) {
	ALOGD("enter realtek_server_RtkHDMIService_updateHDMISetting: videoSystem: %d, videoStandard: %d, autoMode: %d\n", videoSystem, videoStandard, autoMode);
	updateHDMISetting(videoSystem, videoStandard);
	tvSystemAutoMode = autoMode;
}

static jint realtek_server_RtkHDMIService_checkTVSystem(JNIEnv *env, jobject obj, int videoSystem, int videoStandard, int color_space, int deep_color) {
	ALOGD("enter realtek_server_RtkHDMIService_checkTVSystem: videoSystem: %d, videoStandard: %d, color_space: %d, deep_color = %d\n", videoSystem, videoStandard, color_space, deep_color);
	return CheckTVSystem(videoSystem, videoStandard, color_space, deep_color);
}

static jint realtek_server_RtkHDMIService_setHDREotfMode(
    JNIEnv *env,
    jobject obj,
    jint mode)
{
    ALOGD("%s: E",__FUNCTION__);
    HDMIController::setEotfMode( (int) mode);
    reloadTVSystem();
    return 0;
}

static jint realtek_server_RtkHDMIService_getHDREotfValue(
    JNIEnv *env,
    jobject obj)
{
    ALOGD("%s: E",__FUNCTION__);
    return HDMIController::getEotfValue();
}

static jint realtek_server_RtkHDMIService_getDeepColorValue(
    JNIEnv *env,
    jobject obj)
{
    ALOGD("%s: E",__FUNCTION__);

    char *path = "/sys/devices/platform/9800d000.hdmitx/edid_info";
    if(access(path,R_OK) != 0) {
        path = "/sys/devices/platform/passthrough/9800d000.hdmitx/edid_info";
    }

    FILE *fd = fopen(path,"r");

    int result = -1;
    if(fd != NULL) {
        size_t len = 0;
        char *line = NULL;

        while(1){
            ssize_t size = getline(&line, &len, fd);
            if(size == -1)
                break;

            // has ":" or not
            char *val = strchr(line, ':');
            if(val == NULL){
                ALOGV("[%s]: invalid format",__FUNCTION__);
                continue;
            }

            if(strncmp(line, "Deep Color 420:0x", strlen("Deep Color 420:0x")) == 0) {
                val = line + strlen("Deep Color 420:0x");
                result = atoi(val);
                ALOGD("Deep Color 420 value :%d",result);
                break;
            }
        }

        if(line != NULL)
            free(line);

        fclose(fd);
    }

    return result;
}

static jint realtek_server_RtkHDMIService_getHDRColorValue(
    JNIEnv *env,
    jobject obj)
{
    ALOGD("%s: E",__FUNCTION__);
    return 0;
}

static jint realtek_server_RtkHDMIService_getColorSpaceValue(
    JNIEnv *env,
    jobject obj)
{
    int rst = HDMIController::intColorSpaceValue;
    ALOGD("%s: E return %d",__FUNCTION__,rst);
    return rst;
}

static jint realtek_server_RtkHDMIService_setHDCPVersion(
    JNIEnv *env,
    jobject obj,
    jint version)
{
    ALOGD("%s : E",__FUNCTION__);

#ifdef HDCP1x_ENABLED

    if(version == HDCP_VERSION_NONE) {
        ALOGD("%s HDCP version is HDCP_VERSION_NONE, disable HDCP",__FUNCTION__);
        setHDCPEnabled(false);
        return 1;
    }

    if(version != HDCP_VERSION_22 && version != HDCP_VERSION_14) {
        ALOGE("Bad HDCP version %d , return",version);
        return -1;
    }

    int sinkHDCPVersion = getSinkHDCPVersion();
    if(version > sinkHDCPVersion) {
        ALOGE("HDCP version %d is not supported in current sink device",version);
        return -1;
    }

    setManualHDCPVersion(version);
    setHDCPEnabled(false);
    usleep(1E6);
    setHDCPEnabled(true);
    return 1;

#else
    ALOGE("HDCP is not build in, un-supported");
    return -1;
#endif
}

static void realtek_server_RtkHDMIService_regObj(
    JNIEnv *env,
    jobject obj)
{
    mThiz = env->NewGlobalRef(obj);;
}

static void jni_hdcp22UpdateCallback2(int32_t state)
{
    ALOGD("[%s] state:%d",__FUNCTION__,state);
    char value[PROPERTY_VALUE_MAX];
    property_get("ro.config.hdcp.policy", value, "0");

    bool doDisableHDMI = true;
    if(strcmp(value,"1") == 0) {
        doDisableHDMI = false;
    }

    switch(state) {
    case HDCP22_STATE_ENABLE:
        break;
    case HDCP22_STATE_DISABLE:
        if(doDisableHDMI) {
            ALOGD("[%s] turnOffHdmi HDCP22 failed",__FUNCTION__);
            HDMIController::sendAVMute(HDMI_SETAVM);
            HDMIController::setTVSupportxvYCC(0);
            HDMIController::setTVSupportDeepColorYCbCr444(0);
	        HDMI_TurnOffVideo();
        }
        break;
    case HDCP22_STATE_DISABLE_MANUAL:
        ALOGD("[%s] HDCP22 disabled manually do nothing",__FUNCTION__);
        break;
    case HDCP22_STATE_RESET_HDMI:
        ALOGD("[%s] HDCP22 request to reset HDMI",__FUNCTION__);
        HDMIController::sendAVMute(HDMI_SETAVM);
        HDMIController::setTVSupportxvYCC(0);
        HDMIController::setTVSupportDeepColorYCbCr444(0);
        HDMI_TurnOffVideo();

        reloadTVSystem();
        break;
    default:
        break;
    }

}

static void jni_hdcp22UpdateCallback(int32_t state)
{

    if(g_vm == NULL) {
        return;
    }

    JNIEnv* env = NULL;

    int getEnvStat = g_vm->GetEnv((void **)&env, JNI_VERSION_1_4);

    if(getEnvStat == JNI_EDETACHED)
    {
        int attachRst = g_vm->AttachCurrentThread(&env, NULL);
        if (attachRst != 0) {
            ALOGE("%s AttachCurrentThread failed",__FUNCTION__);
            return;
        }
    }

    // then call java function
    jclass clz = env->GetObjectClass(mThiz);
    jmethodID jmID = env->GetMethodID(clz,JAVA_UPDATE_HDCP22_STATE_FUN,"(I)V");
    env->CallVoidMethod(mThiz, jmID, (jint) state);

    if(getEnvStat == JNI_EDETACHED)
    {
        g_vm->DetachCurrentThread();
    }
}

static void realtek_server_RtkHDMIService_getSinkCapability(
    JNIEnv *env,
    jobject obj)
{
    HDMIController::getSinkCapability();
}

/*
 * Array of methods.
 *
 * Each entry has three fields: the name of the method,
 * the method signature, and a pointer to the native implementation.
 */
static const JNINativeMethod RtkHDMIServiceMethods[] = {
		{"_turnOffHdmi", "()Z", (void*) realtek_server_RtkHDMIService_turnOffHdmi },
		{"_checkIfHDCPKeyExist", "()Z", (void*) realtek_server_RtkHDMIService_checkIfHDCPKeyExist },
		{"_checkIfHDMIPlugged", "()Z", (void*) realtek_server_RtkHDMIService_checkIfHDMIPlugged },
		{"_checkIfHDMIReady", "()Z", (void*) realtek_server_RtkHDMIService_checkIfHDMIReady },
		{"_checkIfHDMIEDIDReady", "()Z", (void*) realtek_server_RtkHDMIService_checkIfHDMIEDIDReady },
		{"_sendAudioMute", "(I)Z", (void*) realtek_server_RtkHDMIService_sendAudioMute },
		{"_setHDMIDeepColorMode", "(I)V", (void*) realtek_server_RtkHDMIService_setHDMIDeepColorMode },
		{"_setHDMIFormat3D", "(IF)V", (void*) realtek_server_RtkHDMIService_setHDMIFormat3D },
		{"_setHDCPState", "(I)I", (void*) realtek_server_RtkHDMIService_setHDCPState },
		{"_getVideoFormat", "()[Lcom/realtek/hardware/RtkTVSystem;", (void*) realtek_server_RtkHDMIService_getVideoFormat },
		{"_getNextNVideoFormat", "(I)Lcom/realtek/hardware/RtkTVSystem;", (void*) realtek_server_RtkHDMIService_getNextNVideoFormat },
		{"_getTV3DCapability", "()I", (void*) realtek_server_RtkHDMIService_getTV3DCapability },
		{"_getTVSupport3D", "()[I", (void*) realtek_server_RtkHDMIService_getTVSupport3D },
		{"_getTVSupport3DResolution", "()[I", (void*) realtek_server_RtkHDMIService_getTVSupport3DResolution },
		{"initMediaLinkController", "(I)I", (void*) realtek_server_RtkHDMIService_initMediaLinkController },
		{"nativeReloadTVSystem", "(ZZZZI)V", (void*) realtek_server_RtkHDMIService_nativeReloadTVSystem },
		{"handleHotPlug", "(I)V", (void*) realtek_server_RtkHDMIService_handleHotPlug },
		{"nativeIsTVSupport3D", "()Z", (void*) realtek_server_RtkHDMIService_isTVSupport3D },
		{"nativeGetEDIDRawData", "()[B", (void*) realtek_server_RtkHDMIService_getEDIDRawData },
		{"_saveHDCPToTmp", "()Z", (void*) realtek_server_RtkHDMIService_saveHDCPToTmp },
		{"_setHdcpEnabledByConfig", "(Z)V", (void*) realtek_server_RtkHDMIService_setHdcpEnabledByConfig },
		{"_updateHDMISetting", "(III)V", (void*) realtek_server_RtkHDMIService_updateHDMISetting },
		{"_checkTVSystem", "(IIII)I", (void*) realtek_server_RtkHDMIService_checkTVSystem },
		{"_setHDMIVideoColorMode", "(I)V", (void*) realtek_server_RtkHDMIService_setHDMIVideoColorMode },
		{"_getTVMaxLuminace", "()I", (void*) realtek_server_RtkHDMIService_getTVMaxLuminace },
        {"_setHDREotfMode", "(I)I", (void*) realtek_server_RtkHDMIService_setHDREotfMode },
        {"_getHDREotfValue", "()I", (void*) realtek_server_RtkHDMIService_getHDREotfValue },
        {"_getDeepColorValue", "()I", (void*) realtek_server_RtkHDMIService_getDeepColorValue },
        {"_getHDRColorValue", "()I", (void*) realtek_server_RtkHDMIService_getHDRColorValue },
        {"_getColorSpaceValue", "()I", (void*) realtek_server_RtkHDMIService_getColorSpaceValue },
        {"_setHDCPVersion",     "(I)I", (void*) realtek_server_RtkHDMIService_setHDCPVersion },
        {"_regObj",             "()V", (void*) realtek_server_RtkHDMIService_regObj },
        {"_getSinkCapability",  "()V", (void*) realtek_server_RtkHDMIService_getSinkCapability },
};

void set_RtkHDMIService_gVm(JavaVM *vm)
{
    g_vm = vm;
}

/*
* This is called by the VM when the shared library is first loaded.
*/
int register_android_server_RtkHDMIService(JNIEnv *env) {

	jclass clazz = env->FindClass("com/realtek/server/RtkHDMIService");

	if (clazz == NULL)
	{
		ALOGE("Can't find com/realtek/server/RtkHDMIService");
		return -1;
	}

	if(jniRegisterNativeMethods(env, "com/realtek/server/RtkHDMIService",
		RtkHDMIServiceMethods, NELEM(RtkHDMIServiceMethods)) != 0) {
		ALOGD("Register method to com/realtek/server/RtkHDMIService failed!");
		return -1;
	}

#ifdef HDCP1x_ENABLED
    // then, register HDCP 2.2 callback here
    jni_set_hdcp22_update_callback(&jni_hdcp22UpdateCallback2);
#endif
	return 0;
}

static void reloadTVSystem(bool bCheck) {
	VO_COLOR blueBorder;
	VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM tvSystemConfig;
	VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM nandTvSystemConfig;
	int retValue = 0;

	blueBorder.c1 = 0;
	blueBorder.c2 = 0;
	blueBorder.c3 = 255;
	blueBorder.isRGB = 1;

	memset(&tvSystemConfig, 0 ,sizeof(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM));
	memset(&nandTvSystemConfig, 0 ,sizeof(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM));

	ALOGD("[%s:%d:%s] read from NAND START................ \n",__FILE__,__LINE__,__func__);
	checkNandTVSystem(&nandTvSystemConfig, UPDATE_NAND_READ_ONLY);
	ALOGD("[%s:%d:%s] read from NAND END................ \n",__FILE__,__LINE__,__func__);

	ALOGD("Call VoutUtil::instance().ApplyVideoStandardSetup.\n");
	VoutUtil::instance().ApplyVideoStandardSetup(&tvSystemConfig, &nandTvSystemConfig, true, VIDEO_SYSTEM_NUM, bCheck);

	ALOGD("Call VoutUtil::instance().ApplyVoutDisplayWindowSetup.\n");
	VoutUtil::instance().ApplyVoutDisplayWindowSetup(blueBorder, 0, VIDEO_SYSTEM_NUM);

	retValue = memcmp((const char *)&nandTvSystemConfig, (const char *)&tvSystemConfig, sizeof(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM));
	ALOGD("[%s:%d:%s] compare current TV system & Nand data , result = %d \n",__FILE__,__LINE__,__func__, retValue);

	if(retValue != 0) {
		ALOGD("[%s:%d:%s] write to NAND START................ \n",__FILE__,__LINE__,__func__);
		checkNandTVSystem(&tvSystemConfig, UPDATE_NAND_WRITE_ONLY);
		ALOGD("[%s:%d:%s] write to NAND END................ \n",__FILE__,__LINE__,__func__);
	}
}

static int convertTVSystemToUserTvSystem(video_system videoSystem, video_standard videoStandard) {
	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		for(int i = 0; i < tv_map_length; i++) {
			if(videoSystem != tv_map[i][1]
				|| videoStandard != tv_map[i][2]) {
				continue;
			} else {
				return tv_map[i][0];
			}
		}
	} else {
		for(int i = 0; i < mhl_tv_map_length; i++) {
			if(videoSystem != mhl_tv_map[i][1]
				|| videoStandard != mhl_tv_map[i][2]) {
				continue;
			} else {
				return mhl_tv_map[i][0];
			}
		}
	}

	// if no valid User TV System is found, set it to 480p
	return tv_map[2][0];
}

#if 0
static void setHdmiAVFormat(int lineNo) {
#if defined ONE_STEP
	int linkStatus = LINK_MODE_UNKNOWN;

	if(mediaLinkMode == MEDIA_LINK_MODE_HDMI) {
		linkStatus = HDMIController::checkLinkStatus();
	} else {
		linkStatus = MHLController::checkLinkStatus();
	}

	if(linkStatus ==  LINK_MODE_UNKNOWN || linkStatus == LINK_HDMI_DISABLED) {
		ALOGD("[HDMI] %s(%d) call HDMI_SetHDMIAudioVideoFormat()\n", __func__, lineNo);
		HDMI_SetHDMIAudioVideoFormat();
	} else {
		ALOGD("[HDMI] %s(%d) call HDMI_SetHDMIAudioVideoFormat() XXXXX\n", __func__, lineNo);
		setHDMIReady(1);
	}
#else
	ALOGD("[HDMI] %s(%d) call setHDMIAudioVideoFormat()\n", __func__, lineNo);
	HDMI_SetHDMIAudioVideoFormat();
#endif
}
#endif

} /* namespace android */
