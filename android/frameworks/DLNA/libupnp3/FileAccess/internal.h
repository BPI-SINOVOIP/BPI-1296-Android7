#ifndef __H_GFILE_INTERNAL__
#define __H_GFILE_INTERNAL__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/sendfile.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
// #include "OSAL.h"
#include "file_util.h"
#include "file_access.h"

//#define GIO_DEBUG
#ifdef GIO_DEBUG
 #define PDEBUG(fmt, args...) printf(fmt, ## args)
#else
 #define PDEBUG(fmt, args...)  /*empty macro */
#endif

#define GFILE_EXTENSION_LENGTH  128
#define MAX_PATH_LENGTH         1024


//////////////////////////////////////////////////////////////////////////////
// (1) common
#define GFILE_FMODE_READ     (0x00000001)
#define GFILE_FMODE_WRITE    (0x00000002)
#define GFILE_FMODE_RINGBUF  (0x00000010)
#define GFILE_FMODE_EXT3     (0x00000100)



/////////////////////////////////////////////////////////////////////////////
// (2) gfarray related


/* R00~R99, the total file counts is 100*/
#define GFARRAY_COUNT_MAX       100
/* if file system is FAT, create 10 files each time */
#define GFARRAY_COUNT_INIT      10
/* if file system is FAT, create 1 files each time */
#define GFARRAY_COUNT_INIT_MIN  1
/* if file system is UDF/EXT, don't need to create too many files...*/
#define GFARRAY_COUNT_MIN       1


// max size of file = (4G-64K), this value count by bytes
#define GFARRAY_MAX_FILESIZE           0xFFFF0000
//#define MAX_GFILE_SIZE           0x3200000
// max size of file = 2T, this value count by bytes
#define GFARRAY_MAX_FILESIZE_UNLIMIT   0x20000000000LL



typedef struct tag_gdevs_t {
    int         fd;
    off64_t     pos;
} gdevs_t;


typedef struct tag_gfarray_t {    

    /* filename without extension, e.g: /mnt/smb/mydir/myFile */
    char           file_name[MAX_PATH_LENGTH];      
    /* filename extension, e.g: .mpg or .avi */
    char           file_name_extension[GFILE_EXTENSION_LENGTH];         

    /* file handles and current position */
    gdevs_t        *title_devs;    
    /* file sizes (count by bytes) */
    off64_t        *title_sizes;   
    /* current position (count by bytes) */
    off64_t        seek_pos;      

    /* file open mode flag.
       if opened by "w" mode, need to delete unused file handles after close()    
       the valuse use the definition above, 
       such as GFILE_FMODE_READ/GFILE_FMODE_WRITE/GFILE_FMODE_RINGBUF */
    int            file_mode; 
    /* if opened by "w" mode, remember the file open mode assign by user 
       it will be used when we create more file handles */
    int            file_create_mode;
    int            last_open_file_handle;

    /* if open by "w" mode, support writing to separate file and single big file 
       each file's size is depend on this value */
    off64_t        *max_eachfile_write_size; 
    /* If it used in GFILE_FMODE_RINGBUF case, this field means ring size
       but if it used in normal case, this field means max_write_size of filesystem */
    off64_t        max_ring_write_size;
    /* how many files are opened now, coule depend on partition type, 
       open by "w": if the type of filesystem is FAT32, file_count may increase dynamically 
                    if the type of filesystem is others, file_count maybe 1.
       open by "w": depend on playback only or timeshift 
       @NOTICE: attention the title_devs and title_sizes's array size*/
    int            file_count;
    /* database pointer, sharing the information between read/write Filters */
    int            timeshift_ref;  
} gfarray_t;


gfarray_t * gf_array_open(const char *filename, int oflag);
gfarray_t * gf_array_openring(const char *filename, int oflag, int64_t buf_size);
int gf_array_close(gfarray_t *file_st);
int64_t gf_array_getfreespace(gfarray_t *file_st);

ssize_t gf_array_writebytes(gfarray_t *file_st, void *data, size_t nbyte);
ssize_t gf_array_readbytes(gfarray_t *file_st, void *data, size_t nbyte);
off64_t gf_array_seek(gfarray_t *file_st, off64_t bytes, int whence);
bool gf_array_testfileexist(char *file_name_only, char *file_name_extension);
off64_t gf_array_getfilesize(gfarray_t *file_st);
off64_t gf_array_getmaxwtotalsize(gfarray_t *file_st);
int gf_array_setforcewritetonextfile(gfarray_t *file_st, off64_t cut_pos);

int gf_array_filedeletecancel();
ENUM_GFILE_EDITRET gf_array_filedelete(const char *file_name, char *file_name_only, char *file_name_extension);
ENUM_GFILE_EDITRET gf_array_filerename(const char *old_file_name, const char *new_file_name);
ENUM_GFILE_EDITRET gf_array_fileprotect(const char *file_name, int on);
void gf_array_getfilename(char *in_filepath, char *in_filename, char *out_filename, int length, bool *isKeyFileName);
off64_t gf_array_getfilesetsize(char *in_filename, int *out_iswriting);
void gf_array_getallrealfilename(char *in_filepath, char *in_filename, int *cnt, char filenameAry[][MAX_PATH_LENGTH]);
ENUM_GFILE_EDITRET gf_array_abfastcopy(char *srcFileName, char *dstFileName, int64_t start, int64_t end);





//////////////////////////////////////////////////////////////////////////////
// (3) gflist related

// max size of file = 64MB, this value count by bytes
#define MAX_GLIST_SIZE           0x4000000
// GFListCopy()'s copy unit = 64KB
#define GFLIST_COPY_UNIT 65536
// livepause using array to delete file in thread, this is array max size
#define DELETE_FILE_ARRAY_SIZE 16
// IFO FILENAME
#define IFOFILE_NAME   "1.ifo"

typedef struct tag_gflist_del_t {
    /* thread to delete files if the file is ring type. */
    int                threadExit;
    osal_thread_t      *pDeleteThread;    
    osal_sem_t         *pDeleteSem;
    int                delArray[DELETE_FILE_ARRAY_SIZE];
    char               delWr;
    char               delRd;
    char               dummy[2];
}gflist_del_t;

typedef struct tag_gflist_t {
    /* filename without extension, e.g: /mnt/smb/mydir/myFile */
    char           file_name[MAX_PATH_LENGTH];      
    /* filename extension, e.g: .mpg or .avi */
    char           file_name_extension[10]; 
    /* file open mode flag.
       if opened by "w" mode, need to delete unused file handles after close()    
       the valuse use the definition above, 
       such as GFILE_FMODE_READ/GFILE_FMODE_WRITE/GFILE_FMODE_RINGBUF */
    int            file_type;             
    /* if opened by "w" mode, remember the file open mode assign by user 
       it will be used when we create more file handles */
    int            file_open_mode;
    /* file handle using now */
    int            file_handle;
    /* file handle's actual position (count by bytes)*/
    off64_t        file_pos;
    /* file info's pointer (point to database)*/
    gfdb_info_list *file_info_now;
    /* current position (count by bytes) */
    off64_t        seek_pos;
    /* meta data's file handle */
    int            file_meta_handle;
    /* database pointer, sharing the information between read/write Filters */
    int            timeshift_ref;  
    /* ifo file's file handle */
    int            ifo_file_handle;    

} gflist_t;    

gflist_t * gf_list_open(const char *filename, int oflag);
gflist_t * gf_list_openring(const char *filename, int oflag, int64_t buf_size);
int gf_list_close(gflist_t *file_st);
int64_t gf_list_getfreespace(gflist_t *file_st);

ssize_t gf_list_writebytes(gflist_t *list_st, void *data, size_t nbyte);
ssize_t gf_list_readbytes(gflist_t *list_st, void *data, size_t nbyte);
off64_t gf_list_seek(gflist_t *list_st, off64_t bytes, int whence);
bool gf_list_testfileexist(char *file_name_only, char *file_name_extension);
off64_t gf_list_getfilesize(gflist_t *list_st);
off64_t gf_list_getfilesetsize(char *in_filename, int *out_iswriting);

ssize_t gf_list_ifo_writebytes(gflist_t *list_st, void *data, size_t nbyte);
ssize_t gf_list_ifo_readbytes(gflist_t *list_st, void *data, size_t nbyte);
off64_t gf_list_ifo_seek(gflist_t *list_st, off64_t bytes, int whence);
int gf_list_ifo_reset(gflist_t *list_st);

ENUM_GFILE_EDITRET gf_list_abcopy(gflist_t *list_src, int64_t start, int64_t end, gflist_t *list_dst);

ENUM_GFILE_EDITRET gf_list_filedelete(const char *file_name, char *file_name_only, char *file_name_extension);
ENUM_GFILE_EDITRET gf_list_filemerge(gflist_t *list_st1, gflist_t *list_st2);
ENUM_GFILE_EDITRET gf_list_filesplit(gflist_t *list_src, gflist_t *list_dst, off64_t split_pos);
ENUM_GFILE_EDITRET gf_list_abcut(gflist_t *list_st, int64_t pos_a, int64_t pos_b);



//////////////////////////////////////////////////////////////////////////////
// (3) gfsimple related

typedef struct tag_gfsimple_t {
    /* filename e.g: /mnt/smb/mydir/myFile */
    char           file_name[MAX_PATH_LENGTH];      
    /* file handle using now */
    int            file_handle;
    /* file handle's actual position (count by bytes)*/
    off64_t        file_pos;
    /* current position (count by bytes) */
    off64_t        seek_pos;
    /* total file size */
    off64_t        file_size;
    /* IO mutex, for streaming purpose only. Now only work for USB device*/
    pthread_mutex_t* pIO_mutex;
} gfsimple_t;    

gfsimple_t * gf_simple_open(const char *filename, int oflag);
int gf_simple_close(gfsimple_t *simple_st);

ssize_t gf_simple_writebytes(gfsimple_t *simple_st, void *data, size_t nbyte);
ssize_t gf_simple_readbytes(gfsimple_t *simple_st, void *data, size_t nbyte);
off64_t gf_simple_seek(gfsimple_t *simple_st, off64_t bytes, int whence);
off64_t gf_simple_getfilesize(gfsimple_t *simple_st);
int64_t gf_simple_getfreespace(gfsimple_t *simple_st);
int gf_simple_lockresource(gfsimple_t *simple_st, int timeout);
int gf_simple_unlockresource(gfsimple_t *simple_st);

#endif //__H_GFILE_INTERNAL__
