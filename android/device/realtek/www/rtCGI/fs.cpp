#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#include <set>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <dirent.h>

#include <Application/AppClass/MediaQuery.h>
#include "fs.h"
#include "response.h"



#define MAX_BUFFER_SIZE	        512
#define MAX_STORAGE_LIST_NUM    16

STORAGE_ITEM_T g_StorageList[MAX_STORAGE_LIST_NUM] = {
	{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, 
	{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}		
};

struct ConstCharStarComparator
{
  bool operator()(const char *s1, const char *s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};


static int handleRootRequest(char *path, CGI_RESULT_T *pCgiResult)
{
	FILE_ITEM_T *pItem = NULL, *pInsertPoint = NULL;
	// It always report File/Photo/Music/Video
	do 
	{
		pCgiResult->type = CGI_RESULT_QUERY_FS;		
		{	
			pCgiResult->QueryFSResult.mRet = CGI_OK;

			pCgiResult->QueryFSResult.mCurrent = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
			memset(pCgiResult->QueryFSResult.mCurrent, 0, sizeof(FILE_ITEM_T));
			snprintf(pCgiResult->QueryFSResult.mCurrent->mPath, MAX_SIZE_FILE_PATH, "%s", ROOT_TAG);
			snprintf(pCgiResult->QueryFSResult.mCurrent->mDispName, MAX_SIZE_FILE_NAME, "Home");
			
			pCgiResult->QueryFSResult.mHead= (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
			memset(pCgiResult->QueryFSResult.mHead, 0, sizeof(FILE_ITEM_T));

			pInsertPoint = pCgiResult->QueryFSResult.mHead;				
			for (unsigned int i=0; i < SIZEOF_ARRAY(categoryList); i++) 
			{					
				pCgiResult->QueryFSResult.mNumItems++;
				
				pItem = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
				memset(pItem, 0, sizeof(FILE_ITEM_T));

				pItem->mType = categoryList[i].type;
				snprintf(pItem->mPath, MAX_SIZE_FILE_PATH, "%s", categoryList[i].pPath);
				snprintf(pItem->mDispName, MAX_SIZE_FILE_NAME, "%s", categoryList[i].pDispName);

				pInsertPoint->pNext = pItem;
				pInsertPoint = pItem;
			}
		}	
	} while(false);

	dumpCgiResult(pCgiResult);
	SAFE_FREE_QUERY_FS_T(pCgiResult->QueryFSResult);
	
    return CGI_OK;	
}

int queryStorage(char *path, CGI_RESULT_T *pCgiResult)
{
	int nIndex=0, nOffset=0, nMax=DEFAULT_MAX_ITEMS_PER_QUERY;
	FILE_ITEM_T *pInsertPoint = NULL;

	cgi_get_ivalue("offset", &nOffset);
	cgi_get_ivalue("max", &nMax);
	
	// Query all storage devices
	FILE *f = NULL;  
	f = fopen("/proc/mounts","r"); //open file for describing the mounted filesystems  
	if(!f) {
		responseGeneralResult(pCgiResult, RESPONSE_TYPE_FILELIST, CGI_FAIL, "Can not open /proc/mounts");
		return CGI_FAIL;
	}
	
	pCgiResult->QueryFSResult.mRet = CGI_OK;
    for (int i = 0; i < MAX_STORAGE_LIST_NUM; i++) {        
        SAFE_FREE(g_StorageList[i].pRootPath);
        SAFE_FREE(g_StorageList[i].pDispName);
    }
    memset(g_StorageList, 0, sizeof(STORAGE_ITEM_T) * MAX_STORAGE_LIST_NUM);
    
	// Parent dir
	pCgiResult->QueryFSResult.mParent= (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
	memset(pCgiResult->QueryFSResult.mParent, 0, sizeof(FILE_ITEM_T));
	snprintf(pCgiResult->QueryFSResult.mParent->mPath, MAX_SIZE_FILE_PATH, "%s", ROOT_TAG);
	snprintf(pCgiResult->QueryFSResult.mParent->mDispName, MAX_SIZE_FILE_NAME, "Home");	
	
	// Current dir
	pCgiResult->QueryFSResult.mCurrent = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
	memset(pCgiResult->QueryFSResult.mCurrent, 0, sizeof(FILE_ITEM_T));
	snprintf(pCgiResult->QueryFSResult.mCurrent->mPath, MAX_SIZE_FILE_PATH, "%s", FS_TAG);
	snprintf(pCgiResult->QueryFSResult.mCurrent->mDispName, MAX_SIZE_FILE_NAME, "Devices");	

	pCgiResult->QueryFSResult.mOffset = nOffset;
	pCgiResult->QueryFSResult.mMaxItems = nMax;
	
	pCgiResult->QueryFSResult.mHead = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
	memset(pCgiResult->QueryFSResult.mHead, 0, sizeof(FILE_ITEM_T));
	pInsertPoint = pCgiResult->QueryFSResult.mHead;
	
	char buf[MAX_BUFFER_SIZE] = {0};
	char mount_point[MAX_SIZE_FILE_PATH]={0},dev_root[MAX_SIZE_FILE_PATH]={0},fs_type[MAX_SIZE_FS_TYPE]={0},fs_option[MAX_SIZE_FS_OPTION]={0};
	int dummy0, dummy1;
	int numUsb=0, numHdd=0, numCard=0;
	
	while ( fgets(buf, MAX_BUFFER_SIZE, f) != NULL) 
	{	
		FILE_ITEM_T *pItem = NULL;
		if (strstr(buf, USB_MOUNT_PREFIX)) {
			if (nIndex++ < nOffset)
				continue;			
	
			pItem = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
			memset(pItem, 0, sizeof(FILE_ITEM_T));
	
			sscanf(buf, "%s %s %s %s %d %d", mount_point, dev_root, fs_type, fs_option, &dummy0, &dummy1);
			pItem->mType = FILE_ITEM_TYPE_USB;
			snprintf(pItem->mPath, MAX_SIZE_FILE_PATH, "%s:%s", FS_TAG, dev_root);
			snprintf(pItem->mDispName, MAX_SIZE_FILE_NAME, "Storage%d", pCgiResult->QueryFSResult.mNumItems);

            numUsb++;            
		}		
		else
		{
			continue;
		}

        g_StorageList[pCgiResult->QueryFSResult.mNumItems].pRootPath = strdup(dev_root);
        g_StorageList[pCgiResult->QueryFSResult.mNumItems].pDispName= strdup(pItem->mDispName);
        pCgiResult->QueryFSResult.mNumItems++;

		pInsertPoint->pNext = pItem;
		pInsertPoint = pItem;
	}
	fclose(f);	 //close file for describing the mounted filesystems

	return CGI_OK;
}

// the returned means if it is the root of device
static bool getRelativeFsInfo(char *path, FILE_ITEM_T *pParentInfo, FILE_ITEM_T *pCurrentInfo)
{
	char *pTmp = NULL, *pRelativePath = NULL;
	char indexStr[MAX_SIZE_FILE_NAME] = {0};
	char dispName[MAX_SIZE_FILE_NAME] = {0};

    for (int i =0; i < MAX_STORAGE_LIST_NUM; i++) {
        if (g_StorageList[i].pRootPath == NULL)
            break;

        if (!strncmp(path, g_StorageList[i].pRootPath, strlen(g_StorageList[i].pRootPath))) {
            strcpy(dispName, g_StorageList[i].pDispName);
            pRelativePath = path + strlen(g_StorageList[i].pRootPath) + 1;
            break;
        }            
    }
    
    if (pRelativePath == NULL)
        return false;

	if (strlen(pRelativePath) == 0) {
		// The root folder of some device
		snprintf(pParentInfo->mPath, MAX_SIZE_FILE_PATH, "%s", FS_TAG);
		snprintf(pParentInfo->mDispName, MAX_SIZE_FILE_NAME, "%s", "Devices");	


		snprintf(pCurrentInfo->mPath, MAX_SIZE_FILE_PATH, "%s:%s", FS_TAG, path);
		snprintf(pCurrentInfo->mDispName, MAX_SIZE_FILE_NAME, "%s", dispName);	
		
		return true;
	}	

	char *pParent=dispName, *pCurrent=pRelativePath;
	char *pEnd = strrchr (pRelativePath, '/');
	pTmp = strchr(pRelativePath, '/');
	bool bFirstLevel = true;
	while (pTmp != pEnd)
	{
		bFirstLevel = false;
		pRelativePath = pTmp + 1;
		pParent = pCurrent;
		pCurrent = pRelativePath;
		
		pTmp = strchr (pRelativePath, '/');
	}

	snprintf(pParentInfo->mPath, MAX_SIZE_FILE_PATH, "%s:", FS_TAG);
	strncat(pParentInfo->mPath, path, pCurrent-path);
	if (bFirstLevel)
		snprintf(pParentInfo->mDispName, MAX_SIZE_FILE_NAME, "%s", pParent);		
	else
		strncpy(pParentInfo->mDispName, pParent, pCurrent-pParent-1);
	
	snprintf(pCurrentInfo->mPath, MAX_SIZE_FILE_PATH, "%s:%s", FS_TAG, path);
	strncpy(pCurrentInfo->mDispName, pRelativePath, pEnd - pRelativePath);

	return false;
}

static FILE_ITEM_TYPE_E getSuggestedMediaType(const char *pathname) {
	static const char *imageExts[] = {
		"png",	"gif",	"jpeg",	"jpg",	"bmp" 
	};
  	static const char *audioExts[] = {
  		"mp3",	"wma",	"asf",	"wav",	"aac",	"mp4",	
		"m4a",	"flac",	"ogg",	"pls",	"m3u"
	};
  	static const char *videoExts[] = {
	  	"mp4",	"mov",	"xvid",	"avi",	"iso",	"wmv",	
		"flv",	"f4v",	"mkv",	"ts",	"tp",	"trp",	
		"m2ts",	"mts",	"dat",	"mpg",	"mpeg",	"m2v",	
		"vob",	"avs",	"asm",	"avsts",	"ps",	"3gp",	
		"webm",	"ogm"
	};
  	static const char *subtitleExts[] = {
		"srt",	"aqt",	"cvd",	"dks",	
		"jss",	"sub",	"ttxt",	"mpl",	"pjs",	"psb",	
		"rt",	"smi",	"ssa",	"svcd","usf",	"idx",	
		"txt"
	};
	static const std::set<const char*, ConstCharStarComparator> imageExtSet(imageExts, imageExts+sizeof(imageExts)/sizeof(char*));
	static const std::set<const char*, ConstCharStarComparator> audioExtSet(audioExts, audioExts+sizeof(audioExts)/sizeof(char*));
	static const std::set<const char*, ConstCharStarComparator> videoExtSet(videoExts, videoExts+sizeof(videoExts)/sizeof(char*));
	static const std::set<const char*, ConstCharStarComparator> subtitleExtSet(subtitleExts, subtitleExts+sizeof(subtitleExts)/sizeof(char*));
	
    if(pathname == NULL)
		return FILE_ITEM_TYPE_UNKNOWN;

	/* initialization */
	const char *ext = strrchr(pathname, '.');
	if(ext == NULL)
		return FILE_ITEM_TYPE_UNKNOWN;

	ext++;	// skip "."
	if(imageExtSet.count(ext) == 1)
		return FILE_ITEM_TYPE_PHOTO;
	if(audioExtSet.count(ext) == 1)
		return FILE_ITEM_TYPE_MUSIC;
	if(videoExtSet.count(ext) == 1)
		return FILE_ITEM_TYPE_VIDEO;
	if(subtitleExtSet.count(ext) == 1)
		return FILE_ITEM_TYPE_SUBTITLE;
	
	return FILE_ITEM_TYPE_UNKNOWN;
}


/* NOTE : Please must check if path ends with '/' */
static int queryDir(char *path, CGI_RESULT_T *pCgiResult)
{
    DIR                *dir   = NULL;	
    struct dirent      *entry = NULL;
	struct stat 		info;	
	FILE_ITEM_T			*pItem = NULL;
	char				*currentDispTitle, filePath[MAX_SIZE_FILE_PATH+MAX_SIZE_FILE_NAME];
	int 				nIndex=0, nOffset=0, nMax=DEFAULT_MAX_ITEMS_PER_QUERY;
	FILE_ITEM_T			*pInsertPoint = NULL;

	cgi_get_ivalue("offset", &nOffset);
	cgi_get_ivalue("max", &nMax);	

	if((dir = opendir(path)) == NULL)
	{
		responseGeneralResult(pCgiResult, RESPONSE_TYPE_FILELIST, CGI_FAIL, "Can not open dir %s, err: %s", path, strerror(errno));
		return CGI_FAIL;
	}

	pCgiResult->QueryFSResult.mRet = CGI_OK;


	pCgiResult->QueryFSResult.mParent = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
	memset(pCgiResult->QueryFSResult.mParent, 0, sizeof(FILE_ITEM_T));
	pCgiResult->QueryFSResult.mCurrent = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
	memset(pCgiResult->QueryFSResult.mCurrent, 0, sizeof(FILE_ITEM_T));

	getRelativeFsInfo(path, pCgiResult->QueryFSResult.mParent, pCgiResult->QueryFSResult.mCurrent);
	
	pCgiResult->QueryFSResult.mOffset = nOffset;
	pCgiResult->QueryFSResult.mMaxItems = nMax;

	pCgiResult->QueryFSResult.mHead = (FILE_ITEM_T*)malloc(sizeof(FILE_ITEM_T));
	memset(pCgiResult->QueryFSResult.mHead, 0, sizeof(FILE_ITEM_T));
	pInsertPoint = pCgiResult->QueryFSResult.mHead;
	
	while((entry = readdir(dir)) != NULL)
	{		
		if (0 != strcmp (".", entry->d_name) && 0 != strcmp ("..", entry->d_name))
		{
			if (nIndex < nOffset)
				continue;
			
			pItem = (FILE_ITEM_T*) malloc(sizeof(FILE_ITEM_T));
			memset(pItem, 0, sizeof(FILE_ITEM_T));

			snprintf(filePath, MAX_SIZE_FILE_PATH+MAX_SIZE_FILE_NAME, "%s%s", path, entry->d_name);
			snprintf(pItem->mPath, MAX_SIZE_FILE_PATH+MAX_SIZE_FILE_NAME, "%s:%s%s", FS_TAG, path, entry->d_name);
			
			memset(&info, 0, sizeof(info));
			if (stat(filePath, &info) == 0)
			{
			
				pItem->mMTime = info.st_mtime;
				pItem->mSize = info.st_size;
				if(S_ISDIR(info.st_mode)) {
					strcat(pItem->mPath, "/");
					pItem->mType = FILE_ITEM_TYPE_FOLDER;
				}
				else 
				{
					pItem->mType = getSuggestedMediaType(entry->d_name);
                    if(pItem->mType == FILE_ITEM_TYPE_UNKNOWN) {
                        SAFE_FREE(pItem);
                        continue;
                    }
				}
			}
			snprintf(pItem->mDispName, MAX_SIZE_FILE_NAME, "%s", entry->d_name);
			pInsertPoint->pNext = pItem;
			pInsertPoint = pItem;
			pCgiResult->QueryFSResult.mNumItems++;
		}
	}

	closedir(dir);

	return CGI_OK;
}

static CGI_RET play(char *path)
{
    MediaQuery *pMediaQuery = new MediaQuery();

    int ret = pMediaQuery->play(path);
    delete pMediaQuery;

    return (ret == 0)? CGI_OK:CGI_FAIL;
}

static CGI_RET play(MEDIA_TYPE_E type, int nId)
{
    MediaQuery *pMediaQuery = new MediaQuery();

    int ret = pMediaQuery->play(type, nId);
    delete pMediaQuery;

    return (ret == 0)? CGI_OK:CGI_FAIL;
}

static int handleGetPropertyRequest()
{
    int contentId;
    char *fs_type = NULL, *param = NULL;
    char* result = NULL;
    
    MediaQuery *pMediaQuery = new MediaQuery();

    cgi_get_svalue("url", &fs_type);
        
    if (!strncasecmp(fs_type, MUSIC_TAG, strlen(MUSIC_TAG)))
    {
        param = fs_type + strlen(MUSIC_TAG);
        while(param != NULL && *param == ':') param++;
    
        if ( (contentId = atoi(param)) != 0)
        {
            result = pMediaQuery->getContentProperty(MEDIA_TYPE_MUSIC, contentId);
        }                
    }
    else if (!strncasecmp(fs_type, PHOTO_TAG, strlen(PHOTO_TAG)))
    {
        param = fs_type + strlen(PHOTO_TAG);
        while(param != NULL && *param == ':') param++;
    
        if ( (contentId = atoi(param)) != 0)
        {
            result = pMediaQuery->getContentProperty(MEDIA_TYPE_PHOTO, contentId);
        }                
    }
    else if (!strncasecmp(fs_type, VIDEO_TAG, strlen(VIDEO_TAG)))
    {
        param = fs_type + strlen(VIDEO_TAG);
        while(param != NULL && *param == ':') param++;
    
        if ( (contentId = atoi(param)) != 0)
        {
            result = pMediaQuery->getContentProperty(MEDIA_TYPE_VIDEO, contentId);
        }                
    }
    
	if (result != NULL && strlen(result) > 0) 
	{
		printf("Content-type: application/json\r\n"
			   "\r\n"
			   "%s\n",
			   result);        
	}

    delete pMediaQuery;
    SAFE_FREE(fs_type);
    SAFE_FREE(result);

    return CGI_OK;
}

static int handleFSRequest(char *path, CGI_RESULT_T *pCgiResult)
{
	int ret = CGI_FAIL;
	char *rpath;
	
	path += strlen(FS_TAG);

	while(path != NULL && *path == ':')
		path++;

	rpath = realpath(path, NULL);		// rpath should be freed

	do
	{
		pCgiResult->type = CGI_RESULT_QUERY_FS;
		if (path == NULL || strlen(path) == 0 || !strncasecmp(path, FS_ROOT, strlen(FS_ROOT)))
		{
			ret = queryStorage(path, pCgiResult);			
		}
		else // the path points to some file, and there might action appended
		{ 
			struct stat info;
			bool bIsDir = false;
			
			int fs_action = FS_ACTION_OPEN;
			char *fs_action_str = NULL;
			
			if (cgi_get_svalue("action", &fs_action_str) == CGI_OK) {
				if(strncasecmp(fs_action_str, FS_ACTION_DEL_TAG, strlen(FS_ACTION_DEL_TAG))==0) {
					fs_action = FS_ACTION_DELETE;
				} else if(strncasecmp(fs_action_str, FS_ACTION_DOWNLOAD_TAG, strlen(FS_ACTION_DOWNLOAD_TAG))==0) {
					fs_action = FS_ACTION_DOWNLOAD;
				}
			}

			memset(&info, 0, sizeof(info));
			if(stat(path, &info) != 0) {
				responseGeneralResult(pCgiResult, RESPONSE_TYPE_FILELIST, CGI_FAIL, "Can not stat on %s, err = %s", path, strerror(errno));
				break;
			}
	
#ifdef SKIP_HIDDEN_FILE
			if (IsHiddenFile(path, fsType))
			{
				responseGeneralResult(pCgiResult, RESPONSE_TYPE_FILELIST, CGI_FAIL, "The folder %s is hidden", path);
				break;
			}
#endif
			if(S_ISDIR(info.st_mode))
				bIsDir = true;

			if (bIsDir) {
				switch (fs_action) {
					case FS_ACTION_GET_PROPERTY:
						// Not implemented
						break;
					case FS_ACTION_DELETE:
						// Not implemented
						break;
					case FS_ACTION_OPEN:
					default:
					{
						char dirPath[MAX_SIZE_FILE_PATH];
						strcpy(dirPath, path);
						if (path[strlen(path)-1] != '/')
							strcat(dirPath, "/");
						ret = queryDir(dirPath, pCgiResult);
					}
						break;
				}
			}
			else {
				switch (fs_action) {
					case FS_ACTION_GET_PROPERTY:
						break;
					case FS_ACTION_DELETE:
						// Not implemented
						break;
					case FS_ACTION_DOWNLOAD: 
						{
							CGI_RET ret = CGI_OK;
							FILE_ITEM_TYPE_E mType;
							char *httpRange = getenv("HTTP_RANGE");
							char *bname;
							int rangeRet = 0;
							off_t rangeStart = 0, 
							      rangeEnd = info.st_size-1, 
							      size = info.st_size;
							
							if(rpath == NULL || strstr(rpath, USB_MOUNT_PREFIX) == NULL) {
								responseGeneralResult(pCgiResult, RESPONSE_TYPE_VIEWCONTENT, CGI_FAIL, "Unsupported file path");
								break;
							}

							// check media types (only media file can be downloaed)
							bname = basename(rpath);	// notice: basename() may modify rpath
							mType = getSuggestedMediaType(bname);
							if(mType != FILE_ITEM_TYPE_PHOTO 
								&& mType != FILE_ITEM_TYPE_MUSIC 
								&& mType != FILE_ITEM_TYPE_VIDEO
								&& mType != FILE_ITEM_TYPE_SUBTITLE) {
								responseGeneralResult(pCgiResult, RESPONSE_TYPE_VIEWCONTENT, CGI_FAIL, "Unsupported file type");
								break;
							}
							
							// Check for http range header
							if(httpRange != NULL) {
								rangeRet = sscanf(httpRange, "bytes=%lu - %lu", &rangeStart, &rangeEnd);
							}

							pCgiResult->type = CGI_RESULT_DOWNLOAD;
							pCgiResult->DownloadResult.isPartial = (rangeRet > 0);
							pCgiResult->DownloadResult.rangeStart = rangeStart;
							pCgiResult->DownloadResult.rangeEnd = rangeEnd;
							pCgiResult->DownloadResult.size = size;
							pCgiResult->DownloadResult.fullPath = rpath;
							pCgiResult->DownloadResult.name = bname;
						}
						break;
					case FS_ACTION_OPEN:
					default:
					{
						// try to play this file
                        CGI_RET ret = play(path);
                        responseGeneralResult(pCgiResult, RESPONSE_TYPE_VIEWCONTENT, ret, ((ret==CGI_OK)?NULL:"It can only play photo, music and video"));
					}
						break;
				}
			}			
		}
	} while(0);	

	dumpCgiResult(pCgiResult);
	SAFE_FREE_QUERY_FS_T(pCgiResult->QueryFSResult);
	if(rpath != NULL)
		free(rpath);
    return CGI_OK;
}


/*	
	IMPORTTANT !!! IMPORTANT !!!

	Because the database query is handled by MediaQueryServcie, and 
	the returned value is the JSON formatted string. 
	handleDBRequest only needs to print out the returned string from 
	MediaQuery. It should not pass to response again.
*/

static int handleDBRequest(MEDIA_TYPE_E mediaType)
{
	int nOffset=0, nMax=DEFAULT_MAX_ITEMS_PER_QUERY;
	MediaQuery *pMediaQuery = new MediaQuery();
	char* result = NULL;

	cgi_get_ivalue("offset", &nOffset);
	cgi_get_ivalue("max", &nMax);

	int target = QUERY_FILTER_NONE, id=0;
	if (cgi_get_ivalue("album", &id) == CGI_OK)
		target = QUERY_FILTER_ALBUM;
	else if (cgi_get_ivalue("artist", &id) == CGI_OK)
		target = QUERY_FILTER_ARTIST;
	else if (cgi_get_ivalue("genre", &id) == CGI_OK)
		target = QUERY_FILTER_GENRE;
    
	switch (mediaType)
	{
		case MEDIA_TYPE_MUSIC:
			result = pMediaQuery->queryMusic(nOffset, nMax, target, id);			
			break;
		case MEDIA_TYPE_ALBUM:
			result = pMediaQuery->queryAlbum(nOffset, nMax, target, id);			
			break;
        case MEDIA_TYPE_ALBUM_ART:
        {
            int nWidth, nHeight, albumArtSize=0;
            uint8_t* albumArt = NULL;

            if (cgi_get_ivalue("music", &id) == CGI_OK) {
            
                if(cgi_get_ivalue("width", &nWidth) != CGI_OK)
                    nWidth = DEFAULT_ALBUM_ART_WIDTH;
                if(cgi_get_ivalue("height", &nHeight) != CGI_OK)
                    nHeight = DEFAULT_ALBUM_ART_HEIGHT;            
                
                albumArt = pMediaQuery->queryAlbumArt(id, nWidth, nHeight, &albumArt, &albumArtSize);

                if (albumArt != NULL && albumArtSize > 0)  {
                    printf("Content-Length: %ld\r\n"
                           "Content-type: image/png\r\n"
                           "\r\n", albumArtSize);
                    
                    fwrite(albumArt, albumArtSize, sizeof(uint8_t), stdout);
                    
    				SAFE_FREE(albumArt);
                }
            }
            break;
        }
		case MEDIA_TYPE_ARTIST:
			result = pMediaQuery->queryArtist(nOffset, nMax);			
			break;
		case MEDIA_TYPE_GENRE:
			result = pMediaQuery->queryGenre(nOffset, nMax);			
			break;
		case MEDIA_TYPE_PHOTO:
			result = pMediaQuery->queryPhoto(nOffset, nMax);			
			break;
		case MEDIA_TYPE_VIDEO:
			result = pMediaQuery->queryVideo(nOffset, nMax);
			break;
		default:
			break;
	}

	if (result != NULL && strlen(result) > 0) 
	{
		printf("Content-type: application/json\r\n"
			   "\r\n"
			   "%s\n",
			   result);        
        
	}
    
	delete pMediaQuery;
    SAFE_FREE(result);

    return CGI_OK;
}

int handleFSDBRequst()
{
    int contentId = 0;
	char *fs_type = NULL, *fs_action = NULL;
	CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));
	memset(pCgiRet, 0, sizeof(CGI_RESULT_T));

	do 
	{	    
        cgi_get_svalue("action", &fs_action);

        if (fs_action == NULL
			|| !strcasecmp(fs_action, FS_ACTION_OPEN_TAG)
			|| !strcasecmp(fs_action, FS_ACTION_DOWNLOAD_TAG))
        {
            if (cgi_get_svalue("url", &fs_type) != CGI_OK)
            {
                responseGeneralResult(pCgiRet, RESPONSE_TYPE_NONE, CGI_FAIL, "Invalid parameters");
                dumpCgiResult(pCgiRet);
                break;  
            }

    		if (!strncasecmp(fs_type, ROOT_TAG, strlen(ROOT_TAG)))
    		{
    			handleRootRequest(fs_type, pCgiRet);
    		}
    		else if (!strncasecmp(fs_type, FS_TAG, strlen(FS_TAG)))
    		{
    			handleFSRequest(fs_type, pCgiRet);
    		}
    		else if (!strncasecmp(fs_type, MUSIC_TAG, strlen(MUSIC_TAG)))
    		{
    			char *param = fs_type + strlen(MUSIC_TAG);			
    			while(param != NULL && *param == ':') param++;

    			if (param == NULL || strlen(param) == 0 || !strncasecmp(param, DOMAIN_ALL, strlen(DOMAIN_ALL)))
    			{
    				handleDBRequest(MEDIA_TYPE_MUSIC);
    			}
                else if (!strncasecmp(param, DOMAIN_MUSIC_ALBUMART, strlen(DOMAIN_MUSIC_ALBUMART)))
                {
                    handleDBRequest(MEDIA_TYPE_ALBUM_ART);
                }
    			else if (!strncasecmp(param, DOMAIN_MUSIC_ALBUM, strlen(DOMAIN_MUSIC_ALBUM)))
    			{
    				handleDBRequest(MEDIA_TYPE_ALBUM);
    			}
    			else if (!strncasecmp(param, DOMAIN_MUSIC_ARTIST, strlen(DOMAIN_MUSIC_ARTIST)))
    			{
    				handleDBRequest(MEDIA_TYPE_ARTIST);
    			}
    			else if (!strncasecmp(param, DOMAIN_MUSIC_GENRE, strlen(DOMAIN_MUSIC_GENRE)))
    			{
    				handleDBRequest(MEDIA_TYPE_GENRE);
    			}
                else if ( (contentId = atoi(param)) != 0)
                {
                    // Play the music
                    CGI_RET ret = play(MEDIA_TYPE_MUSIC, contentId);
                    responseGeneralResult(pCgiRet, RESPONSE_TYPE_VIEWCONTENT, ret, ((ret==CGI_OK)?NULL:"It can only play photo, music and video"));
                    dumpCgiResult(pCgiRet);
                }
    		}
    		else if (!strncasecmp(fs_type, PHOTO_TAG, strlen(PHOTO_TAG)))
    		{
    			char *param = fs_type + strlen(PHOTO_TAG);			
    			while(param != NULL && *param == ':') param++;

    		    if (param == NULL || strlen(param) == 0 || !strncasecmp(param, DOMAIN_ALL, strlen(DOMAIN_ALL)))
                {
    			    handleDBRequest(MEDIA_TYPE_PHOTO);
                }
                else if ( (contentId = atoi(param)) != 0)
                {
                    CGI_RET ret = play(MEDIA_TYPE_PHOTO, contentId);
                    responseGeneralResult(pCgiRet, RESPONSE_TYPE_VIEWCONTENT, ret, ((ret==CGI_OK)?NULL:"It can only play photo, music and video"));
                    dumpCgiResult(pCgiRet);
                }
    		}
    		else if (!strncasecmp(fs_type, VIDEO_TAG, strlen(VIDEO_TAG)))
    		{
    			char *param = fs_type + strlen(VIDEO_TAG);
    			while(param != NULL && *param == ':') param++;

    		    if (param == NULL || strlen(param) == 0 || !strncasecmp(param, DOMAIN_ALL, strlen(DOMAIN_ALL)))
                {
                    handleDBRequest(MEDIA_TYPE_VIDEO);
                }
                else if ( (contentId = atoi(param)) != 0)
                {
                    CGI_RET ret = play(MEDIA_TYPE_VIDEO, contentId);                
                    responseGeneralResult(pCgiRet, RESPONSE_TYPE_VIEWCONTENT, ret, ((ret == CGI_OK)?NULL:"It can only play photo, music and video"));
                    dumpCgiResult(pCgiRet);
                }
    		}		
        }
        else if (!strcasecmp(fs_action, FS_ACTION_GET_PROPERTY_TAG)){
            handleGetPropertyRequest();
        }
	} while(0);

	SAFE_FREE(pCgiRet);
	SAFE_FREE(fs_type);
    SAFE_FREE(fs_action);

	return CGI_OK;
}
