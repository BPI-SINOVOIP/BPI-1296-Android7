#ifndef TOGO_H
#define TOGO_H

#include "libcgi.h"
#include "response.h"


/**
 * Version 1 [2015/1/14]
 *  - add session information in queryall response
 *
 * Version 2 [2015/12/1]
 *  - add "t" parameter for file togo config (start time)
 *  - add "file_duration" in file togo init response
 */
#define TOGO_API_VER	2

 
enum {

    FEEDBACK_ERROR_BASE = -100,
    SERVICE_ERROR = FEEDBACK_ERROR_BASE - 1,
    UNSUPPORTED_PARAMETER = FEEDBACK_ERROR_BASE - 2,
    MISSING_PARAMETER = FEEDBACK_ERROR_BASE - 3,
    INVALID_PARAMETER = FEEDBACK_ERROR_BASE - 4,
    INVALID_CLIENT_ID = FEEDBACK_ERROR_BASE - 5,
    FUNC_FAIL = FEEDBACK_ERROR_BASE - 6,
};

enum {
    CMD_QUERY_ALL = 0,
    CMD_INIT,
    //CMD_QUERY,
	CMD_CONFIG,
	CMD_START,
    CMD_CONTROL,
	CMD_STOP
};

// the enums below must be sync with RtkToGo.h
enum {
	TOGO_TYPE_CAMERA = 0,
	TOGO_TYPE_HDMI_RX,
	TOGO_TYPE_FILE,
	TOGO_TYPE_DTV,
	TOGO_TYPE_MAX
};

enum {
    MUXER_TS = 0,
    MUXER_MP4,
    MUXER_AAC,
    MUXER_MAX
};

enum {
    OUTPUT_FILE = 0,
    OUTPUT_STDOUT = 1,
    OUTPUT_ASHMEM = 2,
    OUTPUT_ASHMEM_RTP = 3,
    OUTPUT_UDP = 4,
    OUTPUT_UDP_RTP = 5,
    OUTPUT_UDP_ASHMEM = 6,    // OUTPUT_RTP_BOTH
    OUTPUT_FD = 7,
    OUTPUT_MAX
};

enum {
    CAM_PARAM_KEY_PREVIEW_SIZE,
    CAM_PARAM_KEY_SUPPORTED_PREVIEW_SIZES,
    CAM_PARAM_KEY_PREVIEW_FRAME_RATE,
    CAM_PARAM_KEY_SUPPORTED_PREVIEW_FRAME_RATES,
    CAM_PARAM_MAX
};

static const PARAMETER_TABLE tbl_togo_commands[] = {
    INSERT_PARAM("queryall", CMD_QUERY_ALL),
    INSERT_PARAM("init", CMD_INIT),
    //INSERT_PARAM("query", CMD_QUERY),
	INSERT_PARAM("config", CMD_CONFIG),
	INSERT_PARAM("start", CMD_START),
    INSERT_PARAM("ctl", CMD_CONTROL),
	INSERT_PARAM("stop", CMD_STOP)
};

void responseCamParams(CGI_RESULT_T* pCgiRet, int cid);
void responseDtvParams(CGI_RESULT_T* pCgiRet, int cid);
void freeRsultMemOfToGoSourceStatus(CGI_RESULT_T* pCgiRet);
int checkClientId(CGI_RESULT_T* pCgiRet);
int handleQueryAllCmd(CGI_RESULT_T* pCgiRet);
int handleInitCmd(CGI_RESULT_T* pCgiRet);
//int handleQueryCmd(CGI_RESULT_T* pCgiRet);
int handleConfigCmd(CGI_RESULT_T* pCgiRet);
int handleStartCmd(CGI_RESULT_T* pCgiRet);
int handleStopCmd(CGI_RESULT_T* pCgiRet);
int handleTogo(void);


#endif
