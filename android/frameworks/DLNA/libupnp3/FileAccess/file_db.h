#ifndef __H_GENERAL_FILE_DATA_BASE__
#define __H_GENERAL_FILE_DATA_BASE__

#include <pthread.h>
#include <assert.h>
#include <unistd.h>
// #include <hresult.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
// #include "OSAL.h"
#include "internal.h"

//#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP PTHREAD_ERRORCHECK_MUTEX_INITIALIZER

typedef enum _ENUM_GFILE_IODEVICE_TYPE {
    GFILE_IODEVICE_USB1,
    GFILE_IODEVICE_USB2,
    GFILE_IODEVICE_USB3,
    GFILE_IODEVICE_USB4,
    GFILE_IODEVICE_IDE,
    GFILE_IODEVICE_TOTAL
}ENUM_GFILE_IODEVICE_TYPE;

typedef enum tag_gfdb_getinfolist_e {
    GFDB_GETINFOLIST_HEAD = 0,
    GFDB_GETINFOLIST_END = 1,
    GFDB_GETINFOLIST_FOR_READ = 2
}ENUM_GFDB_GETINFOLIST;
    
    
    
/* database table's max entry */
#define MAX_GFDB_ELEM   16


typedef struct tag_gfdb_context_t {
    /* Filters can use this mutex to access the data in p_context freely*/
    osal_mutex_t*  mutex;
    /* user space */   
    void *            p_context;
    /* the alloc size of p_context */
    int               context_size;    
    /* the destructor of p_context */
    void              (*destructor)(void* context, int size);
}gfdb_context_t;

/*
 * This structure which compose database is used to share file information ....
 */
typedef struct tag_gfdb_elm_t {
    /* key name, here we use file name as key */
    char             key_name[MAX_PATH_LENGTH];
    /* reference count */
    int              ref_count;    
    /* mutex */
    pthread_mutex_t  elm_mutex;
    /* file size now, 
       in timeshift case, recording have to update these values,
       and playback will update its size by these value 
       in playback only case, no special usage....
       @NOTICE: use the value "file_count" can know how many files are available now
    */
    off64_t          file_size[GFARRAY_COUNT_MAX]; 
    /* total file size now 
       For easily get total file size by GFileReadBytes() */
    off64_t          total_file_size;
    /* used to acccess file_size array, means how many file are opened now */
    int              file_count;
    /* only for RING type, 
       since "r" have to know where "w" is writing exactly now,
       "w" update this value, and "r" use it to decide if the read offset is correct or not */
    off64_t          ringwrite_pos_now;
    /* only for RING type, 
       since "r" have to know the ring file size, which needs "w" to update during open  */
    off64_t          max_ring_size;
 
    /* recording bit rate */
    int              bitrate;
    /* total file time length now */
    unsigned int     file_length_time; 
    /* is writing file 
       0: Not recording now, 1: recording now */
    char             write_file;   
    /* TV System Format
       0: NTSC, 1: PAL, -1: unknown */
    char             TV_format;    

    /* dummy payload */
    char             dummy[2];

    /* IO mutex, for streaming purpose only. Now only work for USB device*/
    pthread_mutex_t* pIO_mutex;
  
    /* User define context:
     *    for special information change
     *    can use put/get/createcontext to set this area    
     */
    gfdb_context_t*  user_context; 

    ////////////////////////////////////////////////////////
    // Belows are special usage in GFList's sharing
    
    /* double link list's head */
    gfdb_info_list  *info_list_head; 
    /* double link list's end */
    gfdb_info_list  *info_list_end; 
    /* remember read's info list pointer when livepause
     *   If WRITE need to delete overdue nodes, and READ is using it, 
     *   WRITE can delete the node directly, just setting this field to NULL.
     *   When READ found this field is NULL, it knows the position is unavailable,
     *   and it will recount the info list pointer.
     */
    gfdb_info_list  *info_list_for_read;    
} gfdb_elm_t;

// global database
typedef struct tag_gfdb_t {
    gfdb_elm_t       *elem[MAX_GFDB_ELEM];    
    //pthread_mutex_t  db_mutex;    
} gfdb_t;


bool gf_create_db();
int isexist_gfdb(const char *keyname);
int addref_gfdb(int db_count);
int getref_gfdb(int db_count);

int gfdb_addkey(const char *keyname);
int gfdb_deletekey(int db_count);

off64_t gfdb_getvalue(int db_count, int i);
int gfdb_putvalue(int db_count, off64_t *fsize_ary, int start, int end);

off64_t gfdb_gettotalfilesize(int db_count);
int gfdb_puttotalfilesize(int db_count, off64_t size);

int gfdb_getfilecount(int db_count);
int gfdb_putfilecount(int db_count, int fcount);

off64_t gfdb_getRINGfilepos(int db_count);
int gfdb_putRINGfilepos(int db_count, off64_t pos);

off64_t gfdb_getRINGfilesize(int db_count);
int gfdb_putRINGfilesize(int db_count, off64_t size);

int gfdb_getwrite(int db_count);
int gfdb_putwrite(int db_count, int write_file);

unsigned int gfdb_getlengthtime(int db_count);
int gfdb_putlengthtime(int db_count, int time_length);

int gfdb_gettvformat(int db_count);
int gfdb_puttvformat(int db_count, int tvformat);

int gfdb_getbitrate(int db_count);
int gfdb_putbitrate(int db_count, int bitrate);

int gfdb_getcontext(int db_count, void **context, osal_mutex_t **mutex, int *size);
int gfdb_putcontext(int db_count, void *ptr, osal_mutex_t *mutex, int size);
int gfdb_createcontext(int db_count, int size, int need_mutex, void (*destructor)(void* context, int size));

int gfdb_lockresource(int db_count, int timeout);
int gfdb_unlockresource(int db_count);

int gfdb_external_lockresource(char *path, int timeout);
int gfdb_external_unlockresource(char *path);

int gfdb_isfileprotect(const char *file_name, int* bProtected);

//////////////////////////////////////////////////////////
// Belows are special usage in GFList's sharing

int gfdb_putinfonode(int db_count, ENUM_GFDB_GETINFOLIST type, gfdb_info_list *node);
gfdb_info_list * gfdb_getinfonode(int db_count, ENUM_GFDB_GETINFOLIST type);
int gfdb_resetinfonode(int db_count, int create_new_node);

int gfdb_info_putfilesize(int db_count, gfdb_info_list* node, int64_t fsize);
gfdb_info_list * gfdb_info_append_node(int db_count);
int gfdb_info_removeheadnode(int db_count);

int gfdb_info_putstartaddr(int db_count, gfdb_info_list* node, off64_t start_addr);
int gfdb_info_putfileindex(int db_count, gfdb_info_list* node, int index);


#endif // __H_GENERAL_FILE_DATA_BASE__
