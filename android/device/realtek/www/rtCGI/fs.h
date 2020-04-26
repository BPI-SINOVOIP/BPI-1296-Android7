#ifndef FS_H
#define FS_H

#include "libcgi.h"
#include "response.h"

#define FS_ACTION_OPEN_TAG          "open"
#define FS_ACTION_DOWNLOAD_TAG      "download"
#define FS_ACTION_DEL_TAG	        "del"
#define FS_ACTION_GET_PROPERTY_TAG  "get_property"

#define MUSIC_TAG	"music"
#define PHOTO_TAG	"photo"
#define VIDEO_TAG	"video"
#define FS_TAG		"fs"
#define ROOT_TAG	"root"

// Music related sub-parameters
#define DOMAIN_ALL		        "all"
#define DOMAIN_MUSIC_ALBUM		"album"
#define DOMAIN_MUSIC_ALBUMART   "albumart"
#define DOMAIN_MUSIC_ARTIST	    "artist"
#define DOMAIN_MUSIC_GENRE		"genre"

#define DEFAULT_ALBUM_ART_WIDTH     280
#define DEFAULT_ALBUM_ART_HEIGHT    240

// FS related sub-parameters
#define FS_ROOT		"rootFs"

#define USB_MOUNT_PREFIX	"/storage/sd"

#define MAX_SIZE_FS_TYPE	16
#define MAX_SIZE_FS_OPTION	256

#define DEFAULT_MAX_ITEMS_PER_QUERY 20

typedef struct _CATEGORY_ITEM_T
{
	FILE_ITEM_TYPE_E type;
	const char *pPath;
	const char *pDispName;
} CATEGORY_ITEM_T;

static const CATEGORY_ITEM_T categoryList[] = 
{
	{FILE_ITEM_TYPE_SOURCE, 	FS_TAG,		"File"},
	{FILE_ITEM_TYPE_MEDIA, 		PHOTO_TAG,	"Photo"},
	{FILE_ITEM_TYPE_MEDIA, 		MUSIC_TAG, 	"Music"},
	{FILE_ITEM_TYPE_MEDIA,		VIDEO_TAG, 	"Video"}
};

typedef struct _STORAGE_ITEM_T
{
    char *pRootPath;
    char *pDispName;
} STORAGE_ITEM_T;

enum
{
	FS_ACTION_OPEN,
	FS_ACTION_DELETE,
	FS_ACTION_GET_PROPERTY,
	FS_ACTION_DOWNLOAD,
	FS_ACTION_NUM
};

int handleFSDBRequst(void);


#endif // FS_H
