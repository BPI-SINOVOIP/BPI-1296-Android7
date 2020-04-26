#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdarg.h>
#include "libcgi.h"

//================DEFINITION=====================

#define MAX_SIZE_ERROR_STR	128
#define MAX_SIZE_FILE_PATH	256
#define MAX_SIZE_FILE_NAME	256

#define RESPONSE_TYPE_STR_REMOTE "remote"
#define RESPONSE_TYPE_STR_INFO "info"
#define RESPONSE_TYPE_STR_SETTING "setting"

#define RESPONSE_TYPE_STR_FILELIST "filelist"
#define RESPONSE_TYPE_STR_PHOTOLIST "photolist"
#define RESPONSE_TYPE_STR_MUSICLIST "musiclist"
#define RESPONSE_TYPE_STR_ALBUMLIST "albumlist"
#define RESPONSE_TYPE_STR_ARTISTLIST "artistlist"
#define RESPONSE_TYPE_STR_GENRELIST "genrelist"
#define RESPONSE_TYPE_STR_VIDEOLIST "videolist"
#define RESPONSE_TYPE_STR_VIEWCONTENT "viewcontent"
#define RESPONSE_TYPE_STR_WIFI "wifi"
#define RESPONSE_TYPE_STR_NONE "none"

#define FILE_ITEM_TYPE_STR_SOURCE "source"
#define FILE_ITEM_TYPE_STR_MEDIA "media";
#define FILE_ITEM_TYPE_STR_HDD	"hdd";
#define FILE_ITEM_TYPE_STR_USB	"usb";
#define FILE_ITEM_TYPE_STR_CARD	"card";

#define FILE_ITEM_TYPE_STR_FOLDER "folder";
#define FILE_ITEM_TYPE_STR_PHOTO "photo"
#define FILE_ITEM_TYPE_STR_MUSIC "music"
#define FILE_ITEM_TYPE_STR_VIDEO "video"
#define FILE_ITEM_TYPE_STR_FILE "file"
//==================MACRO========================


#define SAFE_FREE(p) \
{ \
    if (NULL != p) \
    { \
        free(p) ; \
        p = NULL ; \
    } \
}

typedef enum
{
    RESPONSE_TYPE_REMOTE,
    RESPONSE_TYPE_INFO,
    RESPONSE_TYPE_SETTING,
    
    RESPONSE_TYPE_FILELIST,
    RESPONSE_TYPE_PHOTOLIST,
    RESPONSE_TYPE_MUSICLIST,
    RESPONSE_TYPE_ALBUMLIST,
    RESPONSE_TYPE_ARTISTLIST,
    RESPONSE_TYPE_GENRELIST,
    RESPONSE_TYPE_VIDEOLIST,
    
    RESPONSE_TYPE_VIEWCONTENT,
	
    RESPONSE_TYPE_WIFI,
    RESPONSE_TYPE_NONE
} RESPONSE_TYPE_E;

/************************************************/
/*		General									*/
/************************************************/
typedef struct _GENERAL_RESULT_T
{
    RESPONSE_TYPE_E mResponseType;
	CGI_RET	mRet;
	char	mErrStr[MAX_SIZE_ERROR_STR];
} GENERAL_RESULT_T;


/************************************************/
/*		Query Info								*/
/************************************************/

typedef struct _SOFTWARE_INTO_T
{
	char *mVendor;
	char *mProduct;
	char *mAndroidVersion; // e.g. 4.2.2
	char *mKernelVersion;  
	char *mAndroidVersionDetail; //
	char *mCgiVersion;
	char *mDvdPlayerVersion;
	char *mAudioVersion;
	char *mVideoVersion;	
} SOFTWARE_INFO_T;
	
#define SAFE_FREE_SOFTWARE_INFO(p) \
	if (NULL != p) { \
		SAFE_FREE(p->mVendor) \
		SAFE_FREE(p->mProduct) \
		SAFE_FREE(p->mAndroidVersion) \
		SAFE_FREE(p->mKernelVersion) \
		SAFE_FREE(p->mAndroidVersionDetail) \
		SAFE_FREE(p->mCgiVersion) \
		SAFE_FREE(p->mDvdPlayerVersion) \
		SAFE_FREE(p->mAudioVersion) \
		SAFE_FREE(p->mVideoVersion) \
		SAFE_FREE(p) \
	}
	
typedef struct _HARDWARE_INFO_T {
	char *mChip;
	char *mVendor;
}HARDWARE_INFO_T;

#define SAFE_FREE_HARDWARE_INFO(p) \
	if (NULL != p) { \
		SAFE_FREE(p->mChip) \
		SAFE_FREE(p->mVendor) \
		SAFE_FREE(p) \
	} 

typedef struct _QUERY_INFO_RESULT_T
{
	CGI_RET	mRet;
	char	mErrStr[MAX_SIZE_ERROR_STR];
	
	char 		   	*mDeviceName;
	
	SOFTWARE_INFO_T	*mSwInfo;
	HARDWARE_INFO_T	*mHwInfo;
	
} QUERY_INFO_RESULT_T;

#define SAFE_FREE_QUERY_INFO_RESULT_T(s) \
	SAFE_FREE(s.mDeviceName) \
	SAFE_FREE_SOFTWARE_INFO(s.mSwInfo) \
	SAFE_FREE_HARDWARE_INFO(s.mHwInfo)


/************************************************/
/*		Get/Set Setting Items					*/
/************************************************/

typedef struct _GETSET_SETTING_ITEM_T
{
	CGI_RET mRet;
	char	mErrStr[MAX_SIZE_ERROR_STR];
	
	char	*mValue;
} GETSET_SETTING_ITEM_T;

#define SAFE_FREE_GETSET_SETTING_ITEM_T(s) \
	SAFE_FREE(s.mValue)

/************************************************/
/*		File System Traverse					*/
/************************************************/

typedef enum
{
	FILE_ITEM_TYPE_SOURCE, 	// for File entry in Home screen
	FILE_ITEM_TYPE_MEDIA, 	// for media entry of photo/music/video
	FILE_ITEM_TYPE_FOLDER,	// for directory
	FILE_ITEM_TYPE_PHOTO,	// for photo
	FILE_ITEM_TYPE_MUSIC,	// for music
	FILE_ITEM_TYPE_VIDEO,	// for video
	FILE_ITEM_TYPE_SUBTITLE, // for subtitle 
	FILE_ITEM_TYPE_HDD,
	FILE_ITEM_TYPE_USB,
	FILE_ITEM_TYPE_CARD,
	FILE_ITEM_TYPE_UNKNOWN
} FILE_ITEM_TYPE_E;

typedef struct _FILE_ITEM_T
{
	bool bHidden;

	FILE_ITEM_TYPE_E mType;
	char mPath[MAX_SIZE_FILE_PATH+MAX_SIZE_FILE_NAME];
	char mDispName[MAX_SIZE_FILE_NAME];
	unsigned long long mSize;
	time_t mMTime;
	_FILE_ITEM_T *pNext;
} FILE_ITEM_T;

typedef struct _QUERY_FS_T
{
	CGI_RET	mRet;
	char	mErrStr[MAX_SIZE_ERROR_STR];

	FILE_ITEM_T		*mParent;
	FILE_ITEM_T		*mCurrent;

	unsigned int	mNumItems;
	FILE_ITEM_T		*mHead;

	unsigned int	mOffset;	// It indicates the offset of total queried results
	unsigned int	mMaxItems;	// It indicates the max number of items in one response.

} QUERY_FS_T;

#define SAFE_FREE_QUERY_FS_T(s) \
	SAFE_FREE(s.mParent) \
	SAFE_FREE(s.mCurrent) \
	FILE_ITEM_T* p = s.mHead; \
	while (p != NULL) { \
		FILE_ITEM_T *tmp = (FILE_ITEM_T*)p; \
		p = tmp->pNext; \
		SAFE_FREE(tmp); \
	}

typedef struct _QUERY_DB_T
{
	CGI_RET mRet;
	char	mErrStr[MAX_SIZE_ERROR_STR];

	unsigned int	mTotal;
	FILE_ITEM_T		*mFileHead;

	unsigned int 	mOffset;	// It indicates the offset of total queried results
	unsigned int	mMaxItems;	// It indicates the max number of items in one response.
	
} QUERY_DB_T;

#define SAFE_FREE_QUERY_DB_T(s) \
	void *p = (void*) s.mFileHead; \
	while (p != NULL) { \
		FILE_ITEM_T *tmp = (FILE_ITEM_T*)p; \
		p = (void*) tmp->pNext; \
		SAFE_FREE(tmp); \
	}


typedef struct _DOWNLOAD_RESULT_T
{
	bool isPartial;
	char *name;
	char *fullPath;
	off_t size;
	off_t rangeStart;
	off_t rangeEnd;
}DOWNLOAD_RESULT_T;
	

#define MAX_SHORT_SINGLE_PARAM_STRING_BYTES	16
#define MAX_LONG_SINGLE_PARAM_STRING_BYTES	256
#define MAX_SOURCE_NAME_LENGTH	16

typedef struct _TOGO_RESULT_T
{
	CGI_RET	mRet;
	char	mErrStr[MAX_SIZE_ERROR_STR+1];

	unsigned int mVersion;
	// for query all
	unsigned int mSrcCount;
	char **mSrcName;
	int *mSrcId;
	int *mSrcAvail;

	// session list
	unsigned int mSessionMax;
	unsigned int mSessionCount;
	int *mSessionClientId;
	int *mSessionSource;
	int *mSessionState;

	// for init Camera
	int mClientId;
	char mPreviewSize[MAX_SHORT_SINGLE_PARAM_STRING_BYTES+1];
	char mSupportedPreviewSizes[MAX_LONG_SINGLE_PARAM_STRING_BYTES+1];
	char mPreviewFrameRate[MAX_SHORT_SINGLE_PARAM_STRING_BYTES+1];
	char mSupportedPreviewFrameRates[MAX_LONG_SINGLE_PARAM_STRING_BYTES+1];

	// for init DTV
	char mLCNList[MAX_LONG_SINGLE_PARAM_STRING_BYTES+1];

	// for init FILE
	int width;
	int height;
	int duration;
} TOGO_RESULT_T;

/*************************************************/


typedef enum {
	CGI_RESULT_REMOTE,
	CGI_RESULT_INPUTSTRING,

	CGI_RESULT_QUERYINFO,
	CGI_RESULT_GETSETSETTINGITEM,

	CGI_RESULT_QUERY_FS,
	/** The query result is directly from MedaiQueryService with JSON format
	CGI_RESULT_QUERY_DB,
	**/
	CGI_RESULT_DOWNLOAD,
	
	CGI_RESULT_TOGO,
	CGI_RESULT_GENERAL
} CGI_RESULT_E;

typedef struct _CGI_RESULT_T
{
	CGI_RESULT_E type;    

	union {
		GENERAL_RESULT_T		GeneralResult;
		GENERAL_RESULT_T		RemoteResult;
		QUERY_INFO_RESULT_T		QueryInfoResult;
		GETSET_SETTING_ITEM_T	GetSetSettingItemResult;
		GENERAL_RESULT_T		InputStringResult;

		QUERY_FS_T				QueryFSResult;
		QUERY_DB_T				QueryDBResult;
		DOWNLOAD_RESULT_T		DownloadResult;
		TOGO_RESULT_T			TogoResult;
	};
} CGI_RESULT_T;


void responseGeneralResult(CGI_RESULT_T *pResult, RESPONSE_TYPE_E eType, CGI_RET status, const char *errFmt, ...);
int dumpCgiResult(CGI_RESULT_T *out);

#endif // RESPONSE_H
