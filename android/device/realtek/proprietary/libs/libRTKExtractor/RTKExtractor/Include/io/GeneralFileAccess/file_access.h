#ifndef __H_GFILE_COMMON_API__
#define __H_GFILE_COMMON_API__

#include <Filters/config.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <unistd.h>
#include <OSAL.h>
#include <io/dvd_path_define.h>

 
#ifndef HAS_FLAG
 #define HAS_FLAG(flag, bit)    ((flag) & (bit))
 #define SET_FLAG(flag, bit)    ((flag)|= (bit))
 #define RESET_FLAG(flag, bit)  ((flag)&= (~(bit)))
#endif 

#define MAX_PATH_LENGTH              1024

#define METADATA_EXTFILENAME         ".meta"

//@FIXME: Array max file size R00~R99 = 100 files, used by AP
#define GFILE_FILE_COUNT_MAX         100

#ifdef __cplusplus
extern "C" {
#endif



typedef enum tag_enum_gfile_type {
    GFILE_TYPE_ARRAY,
    GFILE_TYPE_LIST,
    GFILE_TYPE_SIMPLE,
    GFILE_TYPE_UNKNOWN
} ENUM_GFILE_TYPE;


typedef struct tag_gfile_t {
    ENUM_GFILE_TYPE type;
    void *ptr;    
} gfile_t;

/* Edit result enum definition */
typedef enum tag_gfile_edit_ret_enum{
    GFILE_EDIT_SUCCESS,
    GFILE_ERR_NO_SPACE,
    GFILE_ERR_FILE_NOT_EXIST,
    GFILE_ERR_FILE_EXIST,
    GFILE_ERR_FILE_PROTECT,
    GFILE_ERR_EDIT_ERROR
} ENUM_GFILE_EDITRET;


// Basic functions
ENUM_GFILE_TYPE GgetFileType(gfile_t *file_st);
gfile_t * GFileOpen(const char *filename, int oflag, ENUM_GFILE_TYPE gfile_type);
gfile_t * GFileOpenRING(const char *filename, int oflag, int64_t buf_size, ENUM_GFILE_TYPE gfile_type);
//Clean the Livepause directory for the next time livepause operation. 1: success. 0: fail
int GFileCleanRing(const char *filename, int64_t buf_size, ENUM_GFILE_TYPE gfile_type);
int GFileClose(gfile_t *file_st);

ssize_t GFileWriteBytes(gfile_t *file_st, void *data, size_t nbyte);
ssize_t GFileReadBytes(gfile_t *file_st, void *data, size_t nbyte);
off64_t GFileSeek(gfile_t *file_st, off64_t bytes, int whence);
bool GFileTestExist(const char *file_name, ENUM_GFILE_TYPE gfile_type);
off64_t GgetFileSize(gfile_t *file_st);
off64_t GgetMaxWritableTotalFileSize(gfile_t *file_st);
int GFileSetForceWriteToNextFile(gfile_t *file_st, off64_t cut_pos);
int64_t GFileGetFreeSpace(gfile_t *file_st);
void GGetAllRealFileName(char *in_filepath, char *in_filename, int *cnt, char filenameAry[][MAX_PATH_LENGTH]);

ssize_t GFileIFOWriteBytes(gfile_t *file_st, void *data, size_t nbyte);
ssize_t GFileIFOReadBytes(gfile_t *file_st, void *data, size_t nbyte);
off64_t GFileIFOSeek(gfile_t *file_st, off64_t bytes, int whence);
int GFileIFOReset(gfile_t *file_st);

// Editing related functions
ENUM_GFILE_EDITRET GFileABCopy(gfile_t *list_src, int64_t start, int64_t end, gfile_t *list_dst);
ENUM_GFILE_EDITRET GFileABFastCopy(char *srcFileName, char *dstFileName, int64_t start, int64_t end);
int GFileCancelDeleteFile();
ENUM_GFILE_EDITRET GFileDelete(const char *file_name);
ENUM_GFILE_EDITRET GFileRename(const char *old_file_name, const char *new_file_name);
ENUM_GFILE_EDITRET GFileProtect(const char *file_name, int on);
void GGetFileName (char *in_filepath, char *in_filename, char *out_filename, int length, bool *isKeyFileName);
off64_t GGetFileSetSize(char *in_filename, int *out_iswriting);
ENUM_GFILE_EDITRET GFileMerge(gfile_t *file_st1, gfile_t *file_st2);
ENUM_GFILE_EDITRET GFileSplit(gfile_t *file_src, gfile_t *file_dst, off64_t split_pos);
ENUM_GFILE_EDITRET GFileABCut(gfile_t *file_st, int64_t pos_a, int64_t pos_b);

// related functions of settings exchange between Playback and Authoring 
int GGetFileIsWriting(gfile_t *file_st);
unsigned int GGetFileTimeLength(gfile_t *file_st);
int GSetFileTimeLength(gfile_t *file_st, unsigned int time_length);
int GGetFileTVFormat(gfile_t *file_st);
int GSetFileTVFormat(gfile_t *file_st, int tvformat);
int GGetFileBitRate(gfile_t *file_st);
int GSetFileBitrate(gfile_t *file_st, int bitrate);

// Context related functions
int GGetFileUserContext(gfile_t *file_st, void **context, osal_mutex_t **mutex, int *size);
int GSetFileUserContext(gfile_t *file_st, void *ptr, osal_mutex_t *mutex, int size);
int GCreateFileUserContext(gfile_t *file_st, int size, int need_mutex, void (*destructor)(void* context, int size));

// Resource Lock/Unlock related functions
int GLockResource(gfile_t *file_st, int timeout);  // timeout only support -1 now
int GUnlockResource(gfile_t *file_st);
int GExternalLockResouce(char *path, int timeout); // timeout only support -1 now
int GExternalUnLockResouce(char *path);

#ifdef __cplusplus
};
#endif


#endif // __H_GFILE_COMMON_API__
