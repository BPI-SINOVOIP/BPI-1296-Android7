#define LOG_NDEBUG 0
#define LOG_TAG	"togo_CGI"
#include <stdlib.h>
#include <cutils/log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "fcgi_stdio.h"
#include "togo.h"
#include "util.h"
//#include "response.h"
#include "ToGoService.h"
#include "ToGoSourceStatus.h"
#include "ToGoClientStatus.h"

using namespace android;

static ToGoService* g_pToGoService = NULL;
//static int g_togoHandle = -1;


void responseHDMIRxParams(CGI_RESULT_T* pCgiRet, int cid)
{
	const char* supported_preview_sizes = g_pToGoService->getCamParams(cid, CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES);
	if (supported_preview_sizes == NULL) {
		ALOGV("HDMIRx getCamParams fail");
		return;
	}
	strncpy(pCgiRet->TogoResult.mSupportedPreviewSizes, supported_preview_sizes, MAX_LONG_SINGLE_PARAM_STRING_BYTES);
}

void responseCamParams(CGI_RESULT_T* pCgiRet, int cid)
{
	const char* preview_size = g_pToGoService->getCamParams(cid, CAM_PARAM_KEY_PREVIEW_SIZE);
	if (preview_size == NULL) {
		return;
	}
	//pCgiRet->TogoResult.mPreviewSize = preview_size;
	strncpy(pCgiRet->TogoResult.mPreviewSize, preview_size, MAX_SHORT_SINGLE_PARAM_STRING_BYTES);

	const char* supported_preview_sizes = g_pToGoService->getCamParams(cid, CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES);
	if (supported_preview_sizes == NULL) {
		return;
	}
	//pCgiRet->TogoResult.mSupportedPreviewSizes = supported_preview_sizes;
	strncpy(pCgiRet->TogoResult.mSupportedPreviewSizes, supported_preview_sizes, MAX_LONG_SINGLE_PARAM_STRING_BYTES);

	const char* preview_frame_rate = g_pToGoService->getCamParams(cid, CAM_PARAM_KEY_PREVIEW_FRAME_RATE);
	if (preview_frame_rate == NULL) {
		return;
	}
	//pCgiRet->TogoResult.mPreviewFrameRate = preview_frame_rate;
	strncpy(pCgiRet->TogoResult.mPreviewFrameRate, preview_frame_rate, MAX_SHORT_SINGLE_PARAM_STRING_BYTES);

	const char* supported_preview_frameRates = g_pToGoService->getCamParams(cid, CAM_PARAM_KEY_SUPPORTED_PREVIEW_FRAME_RATES);
	if (supported_preview_frameRates == NULL) {
		return;
	}
	//pCgiRet->TogoResult.mSupportedPreviewFrameRates = supported_preview_frameRates;
	strncpy(pCgiRet->TogoResult.mSupportedPreviewFrameRates, supported_preview_frameRates, MAX_LONG_SINGLE_PARAM_STRING_BYTES);

}

void responseDtvParams(CGI_RESULT_T* pCgiRet, int cid)
{
	const char* lcn_list = g_pToGoService->getLCNList(cid);
	if (lcn_list == NULL) {
		return;
	}
	strncpy(pCgiRet->TogoResult.mLCNList, lcn_list, MAX_LONG_SINGLE_PARAM_STRING_BYTES);
}

void responseFileParams(CGI_RESULT_T* pCgiRet, int cid)
{
	pCgiRet->TogoResult.width = g_pToGoService->GetFileWidth(cid);
	pCgiRet->TogoResult.height = g_pToGoService->GetFileHeight(cid);
	pCgiRet->TogoResult.duration = g_pToGoService->getDuration(cid);
}

void freeRsultMemOfToGoSourceStatus(CGI_RESULT_T* pCgiRet)
{
	if (pCgiRet == NULL) {
		return;
	}
	if (pCgiRet->TogoResult.mSrcCount > 0) {
		if (pCgiRet->TogoResult.mSrcName) {
			for (unsigned int i=0;i<pCgiRet->TogoResult.mSrcCount;i++) {
				free(pCgiRet->TogoResult.mSrcName+i);
			}
			pCgiRet->TogoResult.mSrcName = NULL;
		}
		if (pCgiRet->TogoResult.mSrcId) {
			free(pCgiRet->TogoResult.mSrcId);
			pCgiRet->TogoResult.mSrcId = NULL;
		}
		if (pCgiRet->TogoResult.mSrcAvail) {
			free(pCgiRet->TogoResult.mSrcAvail);
			pCgiRet->TogoResult.mSrcAvail = NULL;
		}
	}
	if (pCgiRet->TogoResult.mSessionCount > 0) {
		if (pCgiRet->TogoResult.mSessionClientId) {
			free(pCgiRet->TogoResult.mSessionClientId);
			pCgiRet->TogoResult.mSessionClientId = NULL;
		}
		if (pCgiRet->TogoResult.mSessionSource) {
			free(pCgiRet->TogoResult.mSessionSource);
			pCgiRet->TogoResult.mSessionSource = NULL;
		}
		if (pCgiRet->TogoResult.mSessionState) {
			free(pCgiRet->TogoResult.mSessionState);
			pCgiRet->TogoResult.mSessionState = NULL;
		}
	}
	

}

int checkClientId(CGI_RESULT_T* pCgiRet)
{
	int cid = -1;

	if (NULL == g_pToGoService) {
		ALOGE("g_pToGoService is NULL!");
		snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
		return SERVICE_ERROR;
	}

	// client ID
	if (cgi_get_ivalue("cid", &cid) != CGI_OK) {
		ALOGE("miss cid for config in url!");
		snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: cid");
		return MISSING_PARAMETER;
	}
	
	if (g_pToGoService->isHandleLegal(cid) < 0) {
		ALOGE("cid=%d is illegal!",cid);
		snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "INVALID CLIENT_ID: cid=%d", cid);
		return INVALID_CLIENT_ID;
	}

	return cid;
}

int handleQueryAllCmd(CGI_RESULT_T* pCgiRet)
{
	int ret = 0;
	ToGoSourceStatus* pToGoSourceStatus = NULL;
	ToGoClientStatus* pToGoClientStatus = NULL;
	
	do{
		if (NULL == g_pToGoService) {
			ALOGE("g_pToGoService is NULL!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
			ret = SERVICE_ERROR;
			break;
		}
	

		pToGoSourceStatus = new ToGoSourceStatus();
		if (pToGoSourceStatus == NULL) {
			ALOGE("pToGoSourceStatus is NULL!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
			ret = SERVICE_ERROR;
			break;
		}
		g_pToGoService->getSourceStatus(pToGoSourceStatus);
		pCgiRet->TogoResult.mSrcCount = pToGoSourceStatus->getCount();
		int srcCount = pCgiRet->TogoResult.mSrcCount;
		if (srcCount) {
			pCgiRet->TogoResult.mSrcName = (char**)malloc(sizeof(char*)*srcCount);
			if (pCgiRet->TogoResult.mSrcName) {
				for (int i=0;i<srcCount;i++) {
					pCgiRet->TogoResult.mSrcName[i] = (char*)malloc(sizeof(char)*MAX_SOURCE_NAME_LENGTH);
				}
			}
			pCgiRet->TogoResult.mSrcId = (int*)malloc(sizeof(int)*srcCount);
			pCgiRet->TogoResult.mSrcAvail = (int*)malloc(sizeof(int)*srcCount);
			if ((pCgiRet->TogoResult.mSrcName == NULL) || (pCgiRet->TogoResult.mSrcId == NULL) || (pCgiRet->TogoResult.mSrcAvail == NULL)) {
				ALOGE("malloc for ToGoSourceStatus NG!");
				freeRsultMemOfToGoSourceStatus(pCgiRet);
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
				ret = SERVICE_ERROR;
				break;
			}
	
			// fill results
			for (int i=0;i<srcCount;i++) {
				strncpy(pCgiRet->TogoResult.mSrcName[i], pToGoSourceStatus->getName(i), MAX_SOURCE_NAME_LENGTH);
				pCgiRet->TogoResult.mSrcId[i] = pToGoSourceStatus->getId(i);
				pCgiRet->TogoResult.mSrcAvail[i] = pToGoSourceStatus->getAvailable(i);
			}
		}
		
		// Sessions
		pToGoClientStatus = new ToGoClientStatus();
		if (pToGoClientStatus == NULL) {
			ALOGE("pToGoClientStatus is NULL!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
			ret = SERVICE_ERROR;
			break;
		}
		g_pToGoService->getClientStatus(pToGoClientStatus);
		if (pToGoClientStatus == NULL) {
			ALOGE("pToGoSourceStatus is NULL!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
			ret = SERVICE_ERROR;
			break;
		}
		pCgiRet->TogoResult.mSessionCount = pToGoClientStatus->getCount();
		srcCount = pCgiRet->TogoResult.mSessionCount;
		pCgiRet->TogoResult.mSessionMax = pToGoClientStatus->getClientMax();
		if (srcCount) {
			pCgiRet->TogoResult.mSessionClientId = (int*)malloc(sizeof(int)*srcCount);
			pCgiRet->TogoResult.mSessionSource = (int*)malloc(sizeof(int)*srcCount);
			pCgiRet->TogoResult.mSessionState = (int*)malloc(sizeof(int)*srcCount);
			if ((pCgiRet->TogoResult.mSessionClientId == NULL) 
				|| (pCgiRet->TogoResult.mSessionSource == NULL) 
				|| (pCgiRet->TogoResult.mSessionState == NULL)) {
				ALOGE("malloc for ToGoClientStatus NG!");
				freeRsultMemOfToGoSourceStatus(pCgiRet);
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
				ret = SERVICE_ERROR;
				break;
			}
	
			// fill results
			for (int i=0;i<srcCount;i++) {
				pCgiRet->TogoResult.mSessionClientId[i] = pToGoClientStatus->getId(i);
				pCgiRet->TogoResult.mSessionSource[i] = pToGoClientStatus->getSourceType(i);
				pCgiRet->TogoResult.mSessionState[i] = pToGoClientStatus->getState(i);
			}
		}
	}while(0);

	delete pToGoSourceStatus;
	delete pToGoClientStatus;
	return ret;
}

int handleInitCmd(CGI_RESULT_T* pCgiRet)
{
	int ret = 0;
	int source = -1;
	int cameraId = -1;
	int clientId = -1;
	char* infile = NULL;
	ToGoSourceStatus* pToGoSourceStatus = NULL;

	do{
		if (NULL == g_pToGoService) {
			ALOGE("g_pToGoService is NULL!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
			ret = SERVICE_ERROR;
			break;
		}
		
		// init
		if (cgi_get_ivalue("src", &source) != CGI_OK) {
			ALOGE("Miss src in url!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: src");
			ret = MISSING_PARAMETER;
			break;
		}

		// check source type
		if (source >= TOGO_TYPE_MAX) {
			ALOGE("src=%d out of range!",source);
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "INVALID PARAMETER: src=%d", source);
			ret =INVALID_PARAMETER;
			break;
		}
		
		// check if source is available
		pToGoSourceStatus = new ToGoSourceStatus();
		if (pToGoSourceStatus == NULL) {
			ALOGE("pToGoSourceStatus is NULL!");
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "SERVICE ERROR");
			ret = SERVICE_ERROR;
			break;
		}
		
		g_pToGoService->getSourceStatus(pToGoSourceStatus);
		if (pToGoSourceStatus->getAvailable(source) != 1) {
			ALOGE("src=%d is not available!",source);
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "INVALID PARAMETER: src=%d", source);
			ret = INVALID_PARAMETER;
			break;
		}
		
		if (source == TOGO_TYPE_CAMERA) {
			cameraId = 0;
		}
		else if (source == TOGO_TYPE_HDMI_RX) {
			cameraId = 1;
		}
		else if (source == TOGO_TYPE_FILE) {
			if (cgi_get_svalue("in", &infile) != CGI_OK) {
				ALOGE("Miss full name of togo file in url!");
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: in");
				ret = MISSING_PARAMETER;
				break;
			}
		}
		
		// debug log
		if (source == TOGO_TYPE_FILE) {
			ALOGV("cmd=init&src=%d&in=%s.",source,infile);
		}
		else {
			ALOGV("cmd=init&src=%d.",source);
		}

		// init resource
		clientId = g_pToGoService->init(source, cameraId);
		if (clientId < 0) {
			ALOGE("Invalid clientId=%d.", clientId);
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC FAIL: init");
			ret = FUNC_FAIL;
			break;
		}
		ALOGV("occupy clientId=%d.", clientId);
		pCgiRet->TogoResult.mClientId = clientId;

		// response
		if (source == TOGO_TYPE_FILE) {
			if (g_pToGoService->setTargetFilename(clientId, infile) < 0) {
				ALOGE("setTargetFilename() NG.");
				g_pToGoService->uninit(clientId);
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC FAIL: setTargetFilename");
				ret = FUNC_FAIL;
				break;
			}

			responseFileParams(pCgiRet, clientId);
		}
		else if (source == TOGO_TYPE_CAMERA) {
			responseCamParams(pCgiRet, clientId);
		}
		else if (source == TOGO_TYPE_DTV) {
			responseDtvParams(pCgiRet, clientId);
		}
		else if (source == TOGO_TYPE_HDMI_RX) {
			responseHDMIRxParams(pCgiRet, clientId);
		}
	} while(0);

	delete pToGoSourceStatus;	// delete NULL is safe
	return ret;
}

int handleConfigCmd(CGI_RESULT_T* pCgiRet)
{
	int ret = -1;
	int cid = -1;
	int width=0, height=0;
	int vfps = 0;
	int h264 = 0;
	int muxer = MUXER_TS;
	int output = OUTPUT_ASHMEM;
	int t = 0;	// start time
	
	char* ipStr = NULL;
	int port = -1;

	int vbps=0;
	int vIframeIntervalPerSecond = 0;
	int audioChCount=0, audioSampleRate=0, abps=0;
	//int profile = 0;

	cid = checkClientId(pCgiRet);
	if (cid < 0) {
		return cid;
	}

	ALOGV("cmd=config&cid=%d.",cid);

	// preview size
	if ((cgi_get_ivalue("w", &width) == CGI_OK) && (cgi_get_ivalue("h", &height) == CGI_OK)) {
		if (width < 0 || height < 0) {
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "UNSUPPORTED PARAMETER: wxh=%dx%d", width,height);
			return UNSUPPORTED_PARAMETER;
		}
		ALOGV("config w=%d.h=%d.",width,height);
		ret = g_pToGoService->setPreviewSize(cid, width, height);
		if (ret < 0) {
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC_FAIL: wxh=%dx%d", width,height);
			return FUNC_FAIL;
		}
	}
	//

	// video frame rate
	if (cgi_get_ivalue("vfps", &vfps) == CGI_OK) {
		ALOGV("config vfps=%d.",vfps);
		ret= g_pToGoService->setVideoFrameRate(cid, vfps);
		if (ret < 0) {
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "UNSUPPORTED PARAMETER: vfps=%d", vfps);
			return UNSUPPORTED_PARAMETER;
		}
	}
	//

	// enable H264 capture
	if (cgi_get_ivalue("h264", &h264) == CGI_OK) {
		ALOGV("config h264=%d.",h264);
		bool bIsEnableH264Capture = h264 ? true : false;
		g_pToGoService->setEnableH264Capture(cid, bIsEnableH264Capture);
	}
	//

	// muxer format
	if ((cgi_get_ivalue("mux", &muxer) != CGI_OK) || (muxer >= MUXER_MAX)) {
		muxer = MUXER_TS;
		ALOGV("use default config mux=MUXER_TS.");
	}
	else {
		ALOGV("config mux=%d.",muxer);
	}
	g_pToGoService->setMuxerFormat(cid, muxer);
	//

	// output type
	if ((cgi_get_ivalue("out", &output) != CGI_OK) || (output >= OUTPUT_MAX)) {
		output = OUTPUT_ASHMEM;
		ALOGV("use default config output=OUTPUT_ASHMEM.");
	}
	else {
		ALOGV("config output=%d.",output);
	}

	switch (output) {
		case OUTPUT_ASHMEM:
			g_pToGoService->setOutputType(cid, OUTPUT_ASHMEM);
			break;
		case OUTPUT_UDP_RTP:
			{
				g_pToGoService->setOutputType(cid, OUTPUT_UDP_RTP);
				if (cgi_get_svalue("ip", &ipStr) != CGI_OK) {
					ALOGE("Miss config ip in url!");
					snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: ip");
					return MISSING_PARAMETER;
				}
				if (cgi_get_ivalue("port", &port) != CGI_OK) {
					ALOGE("Miss config port in url!");
					snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: port");
					return MISSING_PARAMETER;
				}
				ALOGV("config ip=%s.port=%d.",ipStr,port);
				g_pToGoService->setUdpIpPort(cid, ipStr, port);
			}
			break;
		default:
			ALOGE("Not support yet!output=%d.",output);
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "UNSUPPORTED PARAMETER: output=%d", output);
			return UNSUPPORTED_PARAMETER;
	}
	//

	// video bitrate
	if (cgi_get_ivalue("vbps", &vbps) == CGI_OK) {
		ALOGV("config vbps=%d.",vbps);
		g_pToGoService->setVideoBitrate(cid, vbps);
	}
	//

	// video I frame interval per second
	if (cgi_get_ivalue("vifps", &vIframeIntervalPerSecond) == CGI_OK) {
		ALOGV("config vifps=%d.",vIframeIntervalPerSecond);
		g_pToGoService->setIFrameIntervalSeconds(cid, vIframeIntervalPerSecond);
	}
	//

	// audio channel count
	if (cgi_get_ivalue("ach", &audioChCount) == CGI_OK) {
		ALOGV("config ach=%d.",audioChCount);
		g_pToGoService->setAudioChannelCount(cid, audioChCount);
	}

	// audio sample rate
	if (cgi_get_ivalue("asample", &audioSampleRate) == CGI_OK) {
		ALOGV("config asample=%d.",audioSampleRate);
		g_pToGoService->setAudioSampleRate(cid, audioSampleRate);
	}
	//

	// audio bitrate
	if (cgi_get_ivalue("abps", &abps) == CGI_OK) {
		ALOGV("config abps=%d.",abps);
		g_pToGoService->setAudioBitrate(cid, abps);
	}
	//

	// start time
	if (cgi_get_ivalue("t", &t) == CGI_OK) {
		ALOGV("config start time=%ds.",t);
		g_pToGoService->seek(cid, t*1000);
	}

	// profile
	//if (cgi_get_ivalue("profile", &profile) == CGI_OK) {
	//	ALOGV("got config profile=%d.",profile);
	//	g_pToGoService->setProfile(cid, profile);
	//}
	//

	g_pToGoService->configCompleted(cid);

	responseCamParams(pCgiRet, cid);

	return 0;
}

int handleStartCmd(CGI_RESULT_T* pCgiRet)
{
	int cid = -1;

	cid = checkClientId(pCgiRet);
	if (cid < 0) {
		return cid;
	}

	ALOGV("cmd=start&cid=%d",cid);

	if (g_pToGoService->start(cid) < 0) {
		ALOGE("start() NG.cid=%d.",cid);
		snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC FAIL: start");
		return FUNC_FAIL;
	}

	return 0;
}

int handleControlCmd(CGI_RESULT_T* pCgiRet)
{
	int cid = -1;

	char* act = NULL;
	int ch = 0;

	cid = checkClientId(pCgiRet);
	if (cid < 0) {
		ALOGV("handleControlCmd, checkClientId=%d", cid);
		return cid;
	}

	if (cgi_get_svalue("act", &act) == CGI_OK) {
		ALOGV("handleControlCmd, act=%s", act);
		if (strcmp(act, "play") == 0) {
			if (cgi_get_ivalue("ch", &ch) != CGI_OK) {
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: ch");
				return MISSING_PARAMETER;
			}
			ALOGV("cmd=ctl&act=%s&ch=%d.",act,ch);
			if (g_pToGoService->PlayChannel(cid, ch) < 0) {
				ALOGE("PlayChannel() NG.cid=%d.ch=%d.",cid,ch);
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC FAIL: PlayChannel");
				return FUNC_FAIL;
			}
		}
		else if (strcmp(act, "next") == 0) {
			ALOGV("cmd=ctl&act=%s",act);
			if (g_pToGoService->PlayNextChannel(cid) < 0) {
				ALOGE("PlayNextChannel() NG.cid=%d.",cid);
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC FAIL: PlayNextChannel");
				return FUNC_FAIL;
			}
		}
		else if (strcmp(act, "prev") == 0) {
			ALOGV("cmd=ctl&act=%s",act);
			if (g_pToGoService->PlayPrevChannel(cid) < 0) {
				ALOGE("PlayPrevChannel() NG.cid=%d.",cid);
				snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "FUNC FAIL: PlayPrevChannel");
				return FUNC_FAIL;
			}
		}
		else if (strcmp(act, "seek") == 0) {
			int ms = 0;
			if(cgi_get_ivalue("ms", &ms) == CGI_OK ){
				ALOGV("cmd=ctl&act=seek,ms=",ms);
				g_pToGoService->seek(cid, ms);
			}
		}
	}

	return 0;
}

int handleStopCmd(CGI_RESULT_T* pCgiRet)
{
	int cid = -1;

	cid = checkClientId(pCgiRet);
	if (cid < 0) {
		return cid;
	}

	ALOGV("cmd=stop&cid=%d.",cid);

	KillStreamingProc(cid);
	g_pToGoService->stop(cid);
	g_pToGoService->uninit(cid);
	//ToGoService::ReleaseInstance();
	//g_pToGoService = NULL;
	return 0;
}

int handleTogo(void)
{
	char* cmd_str = NULL;
	int cmd = 0;
	int ret = 0;

	CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));
	memset(pCgiRet, 0, sizeof(CGI_RESULT_T));
	pCgiRet->TogoResult.mClientId = -1;
	pCgiRet->TogoResult.width = -1;
	pCgiRet->TogoResult.height = -1;

	pCgiRet->type = CGI_RESULT_TOGO;

	if (NULL == g_pToGoService) {
		g_pToGoService = ToGoService::GetInstance();
	}

	do
	{
		pCgiRet->TogoResult.mVersion = TOGO_API_VER;
		if (cgi_get_svalue("cmd", &cmd_str) != CGI_OK)
		{
			pCgiRet->TogoResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "MISSING PARAMETER: cmd");
			break;	
		}

		cmd = cmd_string2id(cmd_str, tbl_togo_commands, SIZEOF_ARRAY(tbl_togo_commands));
		if (cmd == -1)
		{
			pCgiRet->TogoResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "UNSUPPORTED PARAMETER: cmd=%s", cmd_str);
			break;
		}

		switch (cmd) {
			case CMD_QUERY_ALL:
				ret = handleQueryAllCmd(pCgiRet);
				break;
			case CMD_INIT:
				ret = handleInitCmd(pCgiRet);
				break;
			//case CMD_QUERY:
			//	ret = handleQueryCmd(pCgiRet);
			//	break;
			case CMD_CONFIG:
				ret = handleConfigCmd(pCgiRet);
				break;
			case CMD_START:
				ret = handleStartCmd(pCgiRet);
				break;
			case CMD_CONTROL:
				ret = handleControlCmd(pCgiRet);
				break;
			case CMD_STOP:
				ret = handleStopCmd(pCgiRet);
				break;
		}

		if (ret >= 0) {
			pCgiRet->TogoResult.mRet = CGI_OK;
		}
		else {
			pCgiRet->TogoResult.mRet = CGI_FAIL;
		}

		if (ret == -1) {
			pCgiRet->TogoResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->TogoResult.mErrStr, MAX_SIZE_ERROR_STR, "UNKNOWN ERROR");
			break;
		}
		
	} while(0);

	dumpCgiResult(pCgiRet);
	freeRsultMemOfToGoSourceStatus(pCgiRet);

	ToGoService::ReleaseInstance();
	g_pToGoService = NULL;
	
	SAFE_FREE(pCgiRet);	
	SAFE_FREE(cmd_str);

	return CGI_OK;
}
