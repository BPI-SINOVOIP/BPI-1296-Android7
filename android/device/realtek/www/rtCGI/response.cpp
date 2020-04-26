#include "fcgi_stdio.h"	/* fcgi library; put it fist */

#include "response.h"

#include <string.h>
#include <jansson.h>
#include "cgi_json_keys.h"

#define NUM_SPACE_PER_TAB	4
#define CGI_JSON_FMT JSON_INDENT(NUM_SPACE_PER_TAB)|JSON_ESCAPE_SLASH|JSON_PRESERVE_ORDER


/*
 * Macro
 */

#define JSON_OBJ_CHECK_SET_STRING_NEW(o, k, v) \
	if (v) \
		json_object_set_new(o, k, json_string(v));


static const char *mapResponseTypeString(RESPONSE_TYPE_E eType)
{
    switch (eType)
    {
    case RESPONSE_TYPE_REMOTE:
        return RESPONSE_TYPE_STR_REMOTE;
    case RESPONSE_TYPE_INFO:
        return RESPONSE_TYPE_STR_INFO;
    case RESPONSE_TYPE_SETTING:
        return RESPONSE_TYPE_STR_SETTING;        
    case RESPONSE_TYPE_FILELIST:
        return RESPONSE_TYPE_STR_FILELIST;
    case RESPONSE_TYPE_PHOTOLIST:
        return RESPONSE_TYPE_STR_PHOTOLIST;
    case RESPONSE_TYPE_MUSICLIST:
        return RESPONSE_TYPE_STR_MUSICLIST;
    case RESPONSE_TYPE_ALBUMLIST:
        return RESPONSE_TYPE_STR_ALBUMLIST;
    case RESPONSE_TYPE_ARTISTLIST:
        return RESPONSE_TYPE_STR_ARTISTLIST;
    case RESPONSE_TYPE_GENRELIST:
        return RESPONSE_TYPE_STR_GENRELIST;
    case RESPONSE_TYPE_VIDEOLIST:
        return RESPONSE_TYPE_STR_VIDEOLIST;
    case RESPONSE_TYPE_VIEWCONTENT:
        return RESPONSE_TYPE_STR_VIEWCONTENT;
    case RESPONSE_TYPE_WIFI:
        return RESPONSE_TYPE_STR_WIFI;
    case RESPONSE_TYPE_NONE:
    default:
        return RESPONSE_TYPE_STR_NONE;
    }
}

static const char *mapTypeString(FILE_ITEM_TYPE_E eType)
{
	switch (eType)
	{
	case FILE_ITEM_TYPE_SOURCE:
		return FILE_ITEM_TYPE_STR_SOURCE;
	case FILE_ITEM_TYPE_MEDIA:	
		return FILE_ITEM_TYPE_STR_MEDIA;
	case FILE_ITEM_TYPE_HDD:
		return FILE_ITEM_TYPE_STR_HDD;
	case FILE_ITEM_TYPE_USB:
		return FILE_ITEM_TYPE_STR_USB;
	case FILE_ITEM_TYPE_CARD:
		return FILE_ITEM_TYPE_STR_CARD;
	case FILE_ITEM_TYPE_FOLDER:
		return FILE_ITEM_TYPE_STR_FOLDER;
	case FILE_ITEM_TYPE_PHOTO:
		return FILE_ITEM_TYPE_STR_PHOTO;
	case FILE_ITEM_TYPE_MUSIC:
		return FILE_ITEM_TYPE_STR_MUSIC;
	case FILE_ITEM_TYPE_VIDEO:
		return FILE_ITEM_TYPE_STR_VIDEO;
	case FILE_ITEM_TYPE_UNKNOWN:
	default:
		return FILE_ITEM_TYPE_STR_FILE;
	}
}

static int dumpCgiGeneralResult(GENERAL_RESULT_T *result)
{
	char *jsonBuf = NULL;
	json_t *obj = json_object();

    json_object_set_new(obj, CGI_JSON_KEY_RESPONSE_TYPE, json_string(mapResponseTypeString(result->mResponseType)));
	if (result->mRet == CGI_OK) {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_OK));        
	}
	else {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_FAIL));
		json_object_set_new(obj, CGI_JSON_KEY_ERROR_DESC, json_string(result->mErrStr));
	}
	
	jsonBuf = json_dumps(obj, CGI_JSON_FMT);
	json_object_clear(obj);

    printf("Content-type: application/json\r\n"
           "\r\n"
           "%s\n",
           jsonBuf);

	free(jsonBuf);
	return 0;
}

static int dumpCgiQueryInfoResult(QUERY_INFO_RESULT_T *result)
{
	char *jsonBuf = NULL;
	json_t *obj = json_object();

    json_object_set_new(obj, CGI_JSON_KEY_RESPONSE_TYPE, json_string(RESPONSE_TYPE_STR_INFO));
	if (result->mRet == CGI_OK) {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_OK));

		// Device name
		JSON_OBJ_CHECK_SET_STRING_NEW(obj, CGI_JSON_KEY_DEVICE_NAME,result->mDeviceName);

		// Software info
		if (result->mSwInfo)
		{	
			json_t *sw_info = json_object();

			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_VENDOR, result->mSwInfo->mVendor);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_PRODUCT, result->mSwInfo->mProduct);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_ANDROID_VERSION, result->mSwInfo->mAndroidVersion);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_KERNEL_VERSION, result->mSwInfo->mKernelVersion);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_ANDROID_VERSION_DETAIL, result->mSwInfo->mAndroidVersionDetail);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_CGI_VERSION, result->mSwInfo->mCgiVersion);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_DVDPLAYER_VERSION, result->mSwInfo->mDvdPlayerVersion);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_AUDIO_VERSION, result->mSwInfo->mAudioVersion);
			JSON_OBJ_CHECK_SET_STRING_NEW(sw_info, CGI_JSON_KEY_VIDEO_VERSION, result->mSwInfo->mVideoVersion);

			json_object_set_new(obj, CGI_JSON_KEY_SOFTWARE_INFO, sw_info);
		}

		// Hardware info
		if (result->mHwInfo)
		{
			json_t *hw_info = json_object();

			JSON_OBJ_CHECK_SET_STRING_NEW(hw_info, CGI_JSON_KEY_CHIPSET, result->mHwInfo->mChip);
			JSON_OBJ_CHECK_SET_STRING_NEW(hw_info, CGI_JSON_KEY_VENDOR, result->mHwInfo->mVendor);

			json_object_set_new(obj, CGI_JSON_KEY_HARDWARE_INFO, hw_info);
		}		
	}
	else {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_FAIL));
		json_object_set_new(obj, CGI_JSON_KEY_ERROR_DESC, json_string(result->mErrStr));
	}

	jsonBuf = json_dumps(obj, CGI_JSON_FMT);
	json_object_clear(obj);

    printf("Content-type: application/json\r\n"
           "\r\n"
           "%s\n",
           jsonBuf);

	free(jsonBuf);	
	return 0;
}

static int dumpCgiGetSetSettingItemResult(GETSET_SETTING_ITEM_T *result)
{
	char *jsonBuf 	= NULL;
	json_t *obj 	= json_object();

    json_object_set_new(obj, CGI_JSON_KEY_RESPONSE_TYPE, json_string(RESPONSE_TYPE_STR_SETTING));
	if (result->mRet == CGI_OK) {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_OK));

		// item value
		JSON_OBJ_CHECK_SET_STRING_NEW(obj, CGI_JSON_KEY_SETTING_ITEM_VALUE, result->mValue);	
	}
	else {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_FAIL));
		json_object_set_new(obj, CGI_JSON_KEY_ERROR_DESC, json_string(result->mErrStr));
	}

	jsonBuf = json_dumps(obj, CGI_JSON_FMT);
	json_object_clear(obj);

    printf("Content-type: application/json\r\n"
           "\r\n"
           "%s\n",
           jsonBuf);

	free(jsonBuf);	
	return 0;
}

static int dumpCgiQueryFSResult(QUERY_FS_T *result)
{
	char *jsonBuf 	= NULL;
	json_t *obj 	= json_object();

    json_object_set_new(obj, CGI_JSON_KEY_RESPONSE_TYPE, json_string(RESPONSE_TYPE_STR_FILELIST));
	if (result->mRet == CGI_OK) {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_OK));		
		if (result->mParent) {
			json_t *parentObj = json_object();

			json_object_set_new(parentObj, CGI_JSON_KEY_FILEPROP_PATH, json_string(result->mParent->mPath));
			json_object_set_new(parentObj, CGI_JSON_KEY_FILEPROP_DISP_NAME, json_string(result->mParent->mDispName));						

			json_object_set_new(obj, CGI_JSON_KEY_PARENT, parentObj);
		}

		if (result->mCurrent) {
			json_t *currentObj = json_object();

			json_object_set_new(currentObj, CGI_JSON_KEY_FILEPROP_PATH, json_string(result->mCurrent->mPath));
			json_object_set_new(currentObj, CGI_JSON_KEY_FILEPROP_DISP_NAME, json_string(result->mCurrent->mDispName));						

			json_object_set_new(obj, CGI_JSON_KEY_CURRENT, currentObj);
		}
		
		if (result->mHead) {			
			// Only shows directory and file
			FILE_ITEM_T *pFileItem = result->mHead->pNext;

			if(pFileItem != NULL)
			{
				json_object_set_new(obj, CGI_JSON_KEY_TOTAL_NUM, json_integer(result->mNumItems));
				json_object_set_new(obj, CGI_JSON_KEY_OFFSET, json_integer(result->mOffset));
				json_t *file_list = json_array();

				// Jump to the correct pointer
				unsigned int index = 0;
				while(pFileItem != NULL && index < result->mOffset && index < result->mNumItems) {
					pFileItem = pFileItem->pNext;
					index++;
				}

				// Write data
				index = 0;
				while(pFileItem != NULL) { // && index < result->mMaxItems) {
					json_t *fileObj = json_object();

					json_object_set_new(fileObj, CGI_JSON_KEY_FILEPROP_TYPE, json_string(mapTypeString(pFileItem->mType)));
					json_object_set_new(fileObj, CGI_JSON_KEY_FILEPROP_PATH, json_string(pFileItem->mPath));
					json_object_set_new(fileObj, CGI_JSON_KEY_FILEPROP_DISP_NAME, json_string(pFileItem->mDispName));
					json_object_set_new(fileObj, CGI_JSON_KEY_FILEPROP_SIZE, json_integer(pFileItem->mSize));
					json_object_set_new(fileObj, CGI_JSON_KEY_FILEPROP_MTIME, json_integer(pFileItem->mMTime));

					json_array_append_new(file_list, fileObj);

					pFileItem = pFileItem->pNext;
					index++;
				}
				json_object_set_new(obj, CGI_JSON_KEY_ITEMS, file_list);				
			}
		}
	}
	else {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_FAIL));
		json_object_set_new(obj, CGI_JSON_KEY_ERROR_DESC, json_string(result->mErrStr));
	}
	
	jsonBuf = json_dumps(obj, CGI_JSON_FMT);
	json_object_clear(obj);

    printf("Content-type: application/json\r\n"
           "\r\n"
           "%s\n",
           jsonBuf);

	free(jsonBuf);	
	return 0;	
}

/** The query result is directly from MedaiQueryService with JSON format
static int dumpCgiQueryDBResult(QUERY_DB_T result)
{
	char *jsonBuf 	= NULL;
	json_t *obj 	= json_object();

	if (result->mRet == CGI_OK) {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_OK));
		json_object_set_new(obj, CGI_JSON_KEY_TOTAL_NUM, json_integer(result->mTotal));
		
		FILE_ITEM_T *pFile = NULL;
		if ((result->mFileHead != NULL) && 
			((pFile = result->mFileHead->pNext) != NULL)) 
		{
			json_t *file_list = json_array();

			// Jump to the correct pointer
			unsigned int index = 0;
			while(pFile != NULL && index < result->mOffset && index < result->mTotal) {
				pFile = pFile->pNext;
				index++;
			}

			// Write data
			index = 0;
			while(pFile != NULL && index < result->mMaxItems) {
				json_t *file = json_object();

				json_object_set_new(file, CGI_JSON_KEY_FILEPROP_PATH, json_string(pFile->mPath));
				json_object_set_new(file, CGI_JSON_KEY_FILEPROP_DISP_NAME, json_string(pFile->mDispName));

				json_array_append_new(file_list, file);
			}
			json_object_set_new(obj, CGI_JSON_KEY_ITEMS, file_list);
		}		
	}
	else {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_FAIL));
		json_object_set_new(obj, CGI_JSON_KEY_ERROR_DESC, json_string(result->mErrStr));
	}

	jsonBuf = json_dumps(obj, CGI_JSON_FMT);
	json_object_clear(obj);

    printf("Content-type: application/json\r\n"
           "\r\n"
           "%s\n",
           jsonBuf);

	free(jsonBuf);	
	return 0;	
}
**/

/**
 * Using lighttpd X-Sendfile/X-Sendfile2 to handle file download
 */
static int dumpCgiDownloadResult(DOWNLOAD_RESULT_T *result)
{
	printf("Accept-Ranges: bytes\r\n");
	printf("Content-type: application/octet-stream\r\n");
	printf("Content-Disposition:attachment;filename=\"%s\"\r\n", result->name);
	if(result->isPartial) {
		// Send partial file contain
		printf("Status: 206 Partial Content\r\n");
		printf("Content-Range: bytes %lu-%lu/%lu\r\n", result->rangeStart, result->rangeEnd, result->size);
		printf("X-Sendfile2: %s %lu-%lu\r\n", result->fullPath, result->rangeStart, result->rangeEnd);
	} else {
		// Send the whold file
		printf("X-Sendfile: %s\r\n", result->fullPath); 
	}
	printf("\r\n");
	return 0;
}

static int dumpCgiTogoResult(TOGO_RESULT_T *result)
{
	char *jsonBuf 	= NULL;
	json_t *obj 	= json_object();

	if (result->mRet == CGI_OK) {
		json_object_set_new(obj, CGI_JSON_KEY_CGI_VERSION, json_integer(result->mVersion));
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_OK));
		
		if (result->mSrcCount > 0) {
			int count = result->mSrcCount;
			//json_object_set_new(obj, CGI_JSON_KEY_SOURCE_COUNT, json_integer(count));

			if (count > 0) {
				json_t *source_list = json_array();

				for (int i=0;i<count;i++) {
					json_t *source = json_object();

					json_object_set_new(source, CGI_JSON_KEY_SOURCE_NAME, json_string(result->mSrcName[i]));
					json_object_set_new(source, CGI_JSON_KEY_SOURCE_ID, json_integer(result->mSrcId[i]));
					json_object_set_new(source, CGI_JSON_KEY_SOURCE_AVAILABLE, json_integer(result->mSrcAvail[i]));

					json_array_append_new(source_list, source);
				}

				json_object_set_new(obj, CGI_JSON_KEY_SOURCE_LIST, source_list);
			}

			// Session List
			if( result->mSessionCount > 0) {
				json_object_set_new(obj, CGI_JSON_KEY_SESSION_MAX, json_integer(result->mSessionMax));
				json_t *session_list = json_array();
				for(unsigned int i=0; i<result->mSessionCount; i++) {
					json_t *session = json_object();
					json_object_set_new(session, CGI_JSON_KEY_SESSION_CLIENT_ID, json_integer(result->mSessionClientId[i]));
					json_object_set_new(session, CGI_JSON_KEY_SESSION_SOURCE, json_integer(result->mSessionSource[i]));
					json_object_set_new(session, CGI_JSON_KEY_SESSION_STATE, json_integer(result->mSessionState[i]));
					json_array_append_new(session_list, session);
				}
				json_object_set_new(obj, CGI_JSON_KEY_SESSION_LIST, session_list);
			}

		}

		
		if (result->mClientId != (-1)) {
			json_object_set_new(obj, CGI_JSON_KEY_CLIENT_ID, json_integer(result->mClientId));
		}
		
		if (strlen(result->mPreviewSize) > 0) {
			json_object_set_new(obj, CGI_JSON_KEY_PREVIEW_SIZE, json_string(result->mPreviewSize));
		}
		if (strlen(result->mSupportedPreviewSizes) > 0) {
			json_object_set_new(obj, CGI_JSON_KEY_SUPPORTED_PREVIEW_SIZES, json_string(result->mSupportedPreviewSizes));
		}
		if (strlen(result->mPreviewFrameRate) > 0) {
			//json_object_set_new(obj, CGI_JSON_KEY_PREVIEW_FRAME_RATE, json_string(result->mPreviewFrameRate));
			json_object_set_new(obj, CGI_JSON_KEY_PREVIEW_FRAME_RATE, json_integer(atoi(result->mPreviewFrameRate)));
		}
		if (strlen(result->mSupportedPreviewFrameRates) > 0) {
			//json_object_set_new(obj, CGI_JSON_KEY_SUPPORTED_PREVIEW_FRAME_RATES, json_string(result->mSupportedPreviewFrameRates));
			json_object_set_new(obj, CGI_JSON_KEY_SUPPORTED_PREVIEW_FRAME_RATES, json_integer(atoi(result->mSupportedPreviewFrameRates)));
		}		

		if (strlen(result->mLCNList) > 0) {
			json_object_set_new(obj, CGI_JSON_KEY_DTV_CH_NUMBER_LIST, json_string(result->mLCNList));
		}		

		if ((result->width != -1) && (result->height != -1)) {
			json_object_set_new(obj, CGI_JSON_KEY_FILE_WIDTH, json_integer(result->width));
			json_object_set_new(obj, CGI_JSON_KEY_FILE_HEIGHT, json_integer(result->height));
		}	

		if(result->duration > 0) {
			json_object_set_new(obj, CGI_JSON_KEY_FILE_DURATION, json_integer(result->duration/1000));
		}
	}
	else {
		json_object_set_new(obj, CGI_JSON_KEY_STATUS, json_integer(CGI_JSON_STATUS_FAIL));
		json_object_set_new(obj, CGI_JSON_KEY_ERROR_DESC, json_string(result->mErrStr));
	}

	jsonBuf = json_dumps(obj, CGI_JSON_FMT);
    printf("Content-type: application/json\r\n"
           "\r\n"
           "%s\n",
           jsonBuf);

	json_object_clear(obj);
	free(jsonBuf);
	return 0;
}

void responseGeneralResult(CGI_RESULT_T *pResult, RESPONSE_TYPE_E eType, CGI_RET status, const char *errFmt, ...)
{
	va_list args;

	pResult->type = CGI_RESULT_GENERAL;
    pResult->GeneralResult.mResponseType = eType;
	pResult->GeneralResult.mRet = status;

    if (status != CGI_OK) {
    	va_start(args, errFmt);	
    	vsnprintf(pResult->GeneralResult.mErrStr, MAX_SIZE_ERROR_STR, errFmt, args);
    	va_end(args);
    }
}

int dumpCgiResult(CGI_RESULT_T *out)
{
	switch (out->type)
	{
		case CGI_RESULT_REMOTE:
		{
			return dumpCgiGeneralResult(&out->RemoteResult);
		}
		case CGI_RESULT_INPUTSTRING:
		{
			return dumpCgiGeneralResult(&out->InputStringResult);
		}
		case CGI_RESULT_QUERYINFO:
		{
			return dumpCgiQueryInfoResult(&out->QueryInfoResult);
		}
		case CGI_RESULT_GETSETSETTINGITEM:
		{
			return dumpCgiGetSetSettingItemResult(&out->GetSetSettingItemResult);
		}
		case CGI_RESULT_QUERY_FS:
		{
			return dumpCgiQueryFSResult(&out->QueryFSResult);
		}
		/** The query result is directly from MedaiQueryService with JSON format		
		case CGI_RESULT_QUERY_DB:
		{
			return dumpCgiQueryDBResult(out->QueryDBResult);
		}
		The query result is directly from MedaiQueryService with JSON format **/		
		case CGI_RESULT_GENERAL:
		{
			return dumpCgiGeneralResult(&out->GeneralResult);
		}
		case CGI_RESULT_DOWNLOAD:
		{
			return dumpCgiDownloadResult(&out->DownloadResult);
		}
		case CGI_RESULT_TOGO:
		{
			return dumpCgiTogoResult(&out->TogoResult);
		}
		default:
		{
			printf("Not supported type: %d\n", out->type);
		}
			break;
	}
	
	return 0;
}
