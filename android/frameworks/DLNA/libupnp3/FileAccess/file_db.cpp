

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif 
#include <pthread.h>
// #include <Util.h>
#include "dvd_path_define.h"
#include "file_db.h"


pthread_mutex_t g_iodevice_mutex[5] =  
   {PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP, PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP,
    PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP, PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP,
    PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP};

static gfdb_t* g_filedb = NULL;
static pthread_mutex_t g_filedb_mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

char* gfdb_realpath(const char *path, char *resolved_path);


/*
 * Use global array as database
 * This function will crate database if it has not been created
 */
bool gf_create_db() {
    int i;
   
    pthread_mutex_lock(&(g_filedb_mutex));
    if (g_filedb) {
        pthread_mutex_unlock(&(g_filedb_mutex));
        return g_filedb != NULL;
    }

    g_filedb = (gfdb_t *)malloc(sizeof(gfdb_t));
    if (!g_filedb) {
        printf("gfiledb malloc fail\n");
        assert(0);
    }
    memset(g_filedb, 0, sizeof(sizeof(gfdb_t)));
    for (i=0; i< MAX_GFDB_ELEM; i++)
        g_filedb->elem[i] = NULL;    

    pthread_mutex_unlock(&(g_filedb_mutex));
    return g_filedb != NULL;
}

/*
 *  return -1 means the key not exist in database
 *  otherwise return db_count;
 */
int isexist_gfdb(const char *keyname) {
    int i;
    char local_path[MAX_PATH_LENGTH], *pReal;
    
    if (!g_filedb) 
        return -1;
        
    //printf("isexist_gfdb::The input path is %s\n", keyname);
    pReal = gfdb_realpath(keyname, &(local_path[0]));
    if (pReal == NULL) {
        printf("The target file %s does not exist in the system!\n", keyname);
        return -1;
    }
    else {
        printf("isexist_gfdb::The resolved path is %s\n", pReal);
    }

    for (i=0; i < MAX_GFDB_ELEM; i++) {
        if ((g_filedb->elem[i] != NULL) &&
            (strcmp(pReal, g_filedb->elem[i]->key_name) == 0) &&
            (g_filedb->elem[i]->ref_count != 0)) 
            return i;                
    }
    return -1;
}

/*
 *  return -1 if add key fail
 *  otherwire retun db_count;
 */
int gfdb_addkey(const char *keyname) {
    int i, bhasEntry, db_count; 
    pthread_mutexattr_t attr;       
    char local_path[MAX_PATH_LENGTH], *pReal;

    pReal = gfdb_realpath(keyname, &(local_path[0]));
    //printf("gfdb_addkey::The input path is %s\n", keyname);
    if (pReal == NULL) {
        printf("The target file %s does not exist in the system!\n", keyname);
        return -1;
    }
    else {
        printf("gfdb_addkey::The resolved path is %s\n", pReal);
    }

    pthread_mutex_lock(&(g_filedb_mutex));
        
    if (!g_filedb)
        assert(0);        
        
    // search empty entry
    bhasEntry = 0;
    for (i=0; i<MAX_GFDB_ELEM; i++) {
        if (g_filedb->elem[i] == NULL) {        
            db_count = i;
            bhasEntry = 1;
            break;
        }
    }

    if (bhasEntry == 0) {
        printf("gfiledb add key fail, exceed Max elements\n");
        pthread_mutex_unlock(&(g_filedb_mutex));
        return -1;
    }

    // add key and inititlize all the fields
    g_filedb->elem[db_count] = (gfdb_elm_t*)malloc(sizeof(gfdb_elm_t));
    if (g_filedb->elem[db_count] == NULL) {
        printf("gfiledb malloc fail\n");
        pthread_mutex_unlock(&(g_filedb_mutex));
        return -1;
    }
    memset(g_filedb->elem[db_count], 0, sizeof(gfdb_elm_t));  
    
    snprintf(g_filedb->elem[db_count]->key_name, strlen(pReal)+1, "%s", pReal);
    g_filedb->elem[db_count]->ref_count = 1;
    memset(g_filedb->elem[db_count]->file_size, 0, sizeof(off64_t)*GFARRAY_COUNT_MAX);       
    g_filedb->elem[db_count]->TV_format = -1;
    // belows are special usage by GFList sharing
    g_filedb->elem[db_count]->info_list_head = gfdb_info_create_node();
    if (g_filedb->elem[db_count]->info_list_head == NULL) {
        printf("gfiledb malloc fail\n");
        pthread_mutex_unlock(&(g_filedb_mutex));
        return -1;
    }
    g_filedb->elem[db_count]->info_list_end = g_filedb->elem[db_count]->info_list_head;
      
    
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
    pthread_mutex_init(&(g_filedb->elem[db_count]->elm_mutex), &attr);
    pthread_mutexattr_destroy(&attr);  

    if (strncmp(keyname, FILE_IODEVICE_USB1, FILE_IODEVICE_PATH_LENGTH) == 0) {
        g_filedb->elem[db_count]->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB1]);
    } else if (strncmp(keyname, FILE_IODEVICE_USB2, FILE_IODEVICE_PATH_LENGTH) == 0) {
        g_filedb->elem[db_count]->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB2]);
    } else if (strncmp(keyname, FILE_IODEVICE_USB3, FILE_IODEVICE_PATH_LENGTH) == 0) {
        g_filedb->elem[db_count]->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB3]);
    } else if (strncmp(keyname, FILE_IODEVICE_USB4, FILE_IODEVICE_PATH_LENGTH) == 0) {
        g_filedb->elem[db_count]->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB4]);
    } else {
        g_filedb->elem[db_count]->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_IDE]);
    }

    pthread_mutex_unlock(&(g_filedb_mutex));

    return db_count;
}



int gfdb_deletekey(int db_count) {
    int i, bHasEntry;

    pthread_mutex_lock(&(g_filedb_mutex));
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)) {
        printf("gfiledb add ref fail\n");
        assert(0);
    }
    
    g_filedb->elem[db_count]->ref_count--;
    
    if (g_filedb->elem[db_count]->ref_count == 0) {
        pthread_mutex_destroy(&(g_filedb->elem[db_count]->elm_mutex));
        if (g_filedb->elem[db_count]->user_context) {
            if (g_filedb->elem[db_count]->user_context->p_context) {
                if (g_filedb->elem[db_count]->user_context->destructor) {
                    g_filedb->elem[db_count]->user_context->destructor(g_filedb->elem[db_count]->user_context->p_context, 
                                                                       g_filedb->elem[db_count]->user_context->context_size);
                }
                free(g_filedb->elem[db_count]->user_context->p_context);
            }
            if (g_filedb->elem[db_count]->user_context->mutex) {
                osal_MutexDestroy(g_filedb->elem[db_count]->user_context->mutex);
                free(g_filedb->elem[db_count]->user_context->mutex);
            }
            free(g_filedb->elem[db_count]->user_context);
        }
        if (g_filedb->elem[db_count]->info_list_head) {
            gfdb_info_list *tmpnode = g_filedb->elem[db_count]->info_list_end;
            
            // go to the end of the info list
            while(tmpnode->prev != NULL) {
                tmpnode = tmpnode->prev;
                if ((tmpnode) && (tmpnode->next))
                    free(tmpnode->next);
            }
            if (tmpnode) free(tmpnode);
            g_filedb->elem[db_count]->info_list_head = NULL;
            g_filedb->elem[db_count]->info_list_end = NULL;
        }
        free(g_filedb->elem[db_count]);
        g_filedb->elem[db_count] = NULL;

        // check if the database elems are all free
        // if yes, free database.
        bHasEntry = 0;
        for (i=0; i<MAX_GFDB_ELEM; i++) {
            if (g_filedb->elem[i] != NULL) {
                bHasEntry = 1;
                break;
            }
        }
#if 0
        if (bHasEntry == 0) {          
            free(g_filedb);
            g_filedb = NULL;
            pthread_mutex_unlock(&(g_filedb_mutex));
            return 1;
        }
#endif
    }

    pthread_mutex_unlock(&(g_filedb_mutex));

    return 1;
}


/*
 *  Add reference count
 */
int addref_gfdb(int db_count) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)) {
        printf("gfiledb add ref fail\n");
        assert(0);
    }


    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
    g_filedb->elem[db_count]->ref_count++;
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}


/*
 *  Get Reference count
 */
int getref_gfdb(int db_count) {
    if ((!g_filedb) || (db_count < 0) || (g_filedb->elem[db_count] == NULL))
        assert(0);
    return g_filedb->elem[db_count]->ref_count;
}


/*
 *  Update file size:
 *  update file_size[start] ~ file_size[end]
 *  @NOTICE: must be continuous... if you only need to update just one entry,
 *           just let start=end
 */
int gfdb_putvalue(int db_count, off64_t *fsize_ary, int start, int end) {
    int i;
    off64_t old_filesize = 0, new_filesize = 0;

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (!fsize_ary)) {
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    for (i=start; i<=end; i++) {       
        old_filesize += g_filedb->elem[db_count]->file_size[i];
        new_filesize += fsize_ary[i];
    }
 
    memcpy(&(g_filedb->elem[db_count]->file_size[start]), &(fsize_ary[start]), (end-start+1)*sizeof(off64_t));

    g_filedb->elem[db_count]->total_file_size += (new_filesize - old_filesize);

    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  get file size[i]
 *  if i = -1, means get total file sizes
 */
off64_t gfdb_getvalue(int db_count, int i) {
    int j;
    off64_t res;

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    res = 0;
    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
    if (i == -1) {                
        for (j=0; j<(g_filedb->elem[db_count]->file_count); j++)
            res += g_filedb->elem[db_count]->file_size[j];
    }
    else {
        if (i < g_filedb->elem[db_count]->file_count)
            res = g_filedb->elem[db_count]->file_size[i];
        else
            assert(0);
    }
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return res;
}


off64_t gfdb_gettotalfilesize(int db_count)
{
    off64_t res;

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
    res = g_filedb->elem[db_count]->total_file_size;
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return res;    
}

int gfdb_puttotalfilesize(int db_count, off64_t size) 
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->total_file_size = size;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;   
}


int gfdb_getfilecount(int db_count)
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    return (int)(g_filedb->elem[db_count]->file_count);
}

int gfdb_putfilecount(int db_count, int fcount) 
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->file_count = fcount;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}


off64_t gfdb_getRINGfilepos(int db_count)
{
    off64_t res;

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    res = (g_filedb->elem[db_count]->ringwrite_pos_now);

    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return res;
}

int gfdb_putRINGfilepos(int db_count, off64_t pos) 
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->ringwrite_pos_now = pos;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

off64_t gfdb_getRINGfilesize(int db_count)
{
    off64_t res;

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    res = (g_filedb->elem[db_count]->max_ring_size);

    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return res;
}

int gfdb_putRINGfilesize(int db_count, off64_t size) 
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->max_ring_size = size;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  get if the writing of the file is going on 
 */
int gfdb_getwrite(int db_count)
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    return (int)(g_filedb->elem[db_count]->write_file);
}

/*
 *  update if the writing of the file is going on 
 */
int gfdb_putwrite(int db_count, int write_file)
{

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->write_file = (char)write_file;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  get the total time length of the file 
 */
unsigned int gfdb_getlengthtime(int db_count) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    return g_filedb->elem[db_count]->file_length_time;
}

/*
 *  update the total time length of the file
 */
int gfdb_putlengthtime(int db_count, int time_length) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)) {
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->file_length_time = time_length;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  get the tv format of the current file 
 */
int gfdb_gettvformat(int db_count)
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    return (int)(g_filedb->elem[db_count]->TV_format);
}

/*
 *  set the tv format of the current file 
 */
int gfdb_puttvformat(int db_count, int tvformat)
{
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->TV_format = (char)tvformat;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  get the total stream bitrate 
 */
int gfdb_getbitrate(int db_count) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    return g_filedb->elem[db_count]->bitrate;
}

/*
 *  update the total stream bitrate
 */
int gfdb_putbitrate(int db_count, int bitrate) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)) {
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    g_filedb->elem[db_count]->bitrate = bitrate;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  get the user context pointer and mutex
 */
int gfdb_getcontext(int db_count, void **context, osal_mutex_t **mutex, int *size) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    if (g_filedb->elem[db_count]->user_context == NULL)
        return -1;

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
    *context = g_filedb->elem[db_count]->user_context->p_context;
    if (mutex) *mutex = g_filedb->elem[db_count]->user_context->mutex;
    if (size) *size = g_filedb->elem[db_count]->user_context->context_size;
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
    return 1; 
}

int gfdb_putcontext(int db_count, void *ptr, osal_mutex_t *mutex, int size) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->user_context == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    //memcpy(g_filedb->elem[db_count]->user_context->p_context, ptr, size);
    g_filedb->elem[db_count]->user_context->p_context = ptr;
    g_filedb->elem[db_count]->user_context->mutex = mutex;
    g_filedb->elem[db_count]->user_context->context_size = size;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;
}

/*
 *  malloc user context memory according to the size field
 *  and initialize the mutex which will be used by user.
 */
int gfdb_createcontext(int db_count, int size, int need_mutex, void (*destructor)(void* context, int size)) {

    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (g_filedb->elem[db_count]->ref_count == 0)) {
        printf("gfiledb get value fail\n");
        assert(0);
    }

    if (g_filedb->elem[db_count]->user_context != NULL) 
        return -1;
    
    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));     

    g_filedb->elem[db_count]->user_context = (gfdb_context_t *)malloc(sizeof(gfdb_context_t));
    if (g_filedb->elem[db_count]->user_context == NULL) {
        pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
        return -1;
    }
    memset(g_filedb->elem[db_count]->user_context, 0, sizeof(gfdb_context_t));
             
    if (need_mutex) {  
        g_filedb->elem[db_count]->user_context->mutex = (osal_mutex_t*)malloc(sizeof(osal_mutex_t));
        if (g_filedb->elem[db_count]->user_context->mutex == NULL) {
            printf("Can not alloc the memory for context mutex.\n");
            free(g_filedb->elem[db_count]->user_context);
            pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
            return -1; 
        }   
        if (osal_MutexCreate(g_filedb->elem[db_count]->user_context->mutex) != true) {
            printf("Can not create context mutex.\n");
            free(g_filedb->elem[db_count]->user_context);
            pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
            return -1; 
        }
        //printf("Mutex is 0x%x\n", (int)(g_filedb->elem[db_count]->user_context->mutex));
    }
  
    g_filedb->elem[db_count]->user_context->p_context = (void *)malloc(size);
    if (g_filedb->elem[db_count]->user_context->p_context == NULL) {
        osal_MutexDestroy(g_filedb->elem[db_count]->user_context->mutex);
        free(g_filedb->elem[db_count]->user_context->mutex);
        free(g_filedb->elem[db_count]->user_context);
        pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
        return -1;
    }
    //printf("Context is 0x%x and size %d\n", (int)(g_filedb->elem[db_count]->user_context->p_context), size);
    g_filedb->elem[db_count]->user_context->context_size = size;
    g_filedb->elem[db_count]->user_context->destructor = destructor;
      
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1; 
}

int gfdb_lockresource(int db_count, int timeout)
{
    if (g_filedb->elem[db_count]->pIO_mutex) {
        if (timeout == -1) {                        
            pthread_mutex_lock(g_filedb->elem[db_count]->pIO_mutex);            
        }
        else {
            return -1;
        }
    }
    return 1;
}

int gfdb_unlockresource(int db_count)
{
    if (g_filedb->elem[db_count]->pIO_mutex) {                   
        pthread_mutex_unlock(g_filedb->elem[db_count]->pIO_mutex);
    }
    return 1;
}

int gfdb_external_lockresource(char *path, int timeout) 
{
    if (strncmp(path, FILE_IODEVICE_USB1, FILE_IODEVICE_PATH_LENGTH) == 0)               
        pthread_mutex_lock(&(g_iodevice_mutex[GFILE_IODEVICE_USB1]));            
    else if (strncmp(path, FILE_IODEVICE_USB2, FILE_IODEVICE_PATH_LENGTH) == 0) 
        pthread_mutex_lock(&(g_iodevice_mutex[GFILE_IODEVICE_USB2]));                        
    else if (strncmp(path, FILE_IODEVICE_USB3, FILE_IODEVICE_PATH_LENGTH) == 0) 
        pthread_mutex_lock(&(g_iodevice_mutex[GFILE_IODEVICE_USB3]));            
    else if (strncmp(path, FILE_IODEVICE_USB4, FILE_IODEVICE_PATH_LENGTH) == 0) 
        pthread_mutex_lock(&(g_iodevice_mutex[GFILE_IODEVICE_USB4]));            
    else 
        return -1;
    
    return 1;    
}

int gfdb_external_unlockresource(char *path)
{
    if (strncmp(path, FILE_IODEVICE_USB1, FILE_IODEVICE_PATH_LENGTH) == 0)                 
        pthread_mutex_unlock(&(g_iodevice_mutex[GFILE_IODEVICE_USB1]));    
    else if (strncmp(path, FILE_IODEVICE_USB2, FILE_IODEVICE_PATH_LENGTH) == 0)         
        pthread_mutex_unlock(&(g_iodevice_mutex[GFILE_IODEVICE_USB2]));       
    else if (strncmp(path, FILE_IODEVICE_USB3, FILE_IODEVICE_PATH_LENGTH) == 0)         
        pthread_mutex_unlock(&(g_iodevice_mutex[GFILE_IODEVICE_USB3]));    
    else if (strncmp(path, FILE_IODEVICE_USB4, FILE_IODEVICE_PATH_LENGTH) == 0)         
        pthread_mutex_unlock(&(g_iodevice_mutex[GFILE_IODEVICE_USB4]));            
    else 
        return -1;
    
    return 1;        
}

char* gfdb_realpath(const char *path, char *resolved_path)
{
    char directory_name[MAX_PATH_LENGTH], file_name[MAX_PATH_LENGTH], *pReal;
    int i;
    
    // parse for file directory
    for (i=strlen(path); i>=0; i--) {
        if (path[i] == '/') {
            sprintf(file_name, "%s", &(path[i]));
            snprintf(directory_name, i+1, "%s", path);
            break;
        }
    }
    //printf("The filedirectory is %s\n", directory_name);

    pReal = realpath(directory_name, resolved_path);

    if (pReal == NULL) {
        printf("gfdb_realpath::The target file directory %s does not exist!\n", directory_name);
        return NULL;
    }
    
    sprintf(resolved_path, "%s%s", resolved_path, file_name);
    
    //printf("The resolved path is %s\n", resolved_path);
    
    return pReal;
}


int gfdb_isfileprotect(const char *file_name, int* bProtected)
{
    struct stat buffer;
    int         status;

    status = stat(file_name, &buffer);

    if (status == 0) {
        if (buffer.st_mode & S_IWUSR)
            *bProtected = 0;
        else
            *bProtected = 1;
    }
    else {
        //Since something wrong with that file. Forbit any operation
        *bProtected = 1;
    }
    return status;
}


////////////////////////////////////////////////////////////////////////////

/*
 *  Get the infolist node now;
 */
gfdb_info_list * gfdb_getinfonode(int db_count, ENUM_GFDB_GETINFOLIST type) {
    if ((!g_filedb) || (db_count < 0) || (g_filedb->elem[db_count] == NULL))
        assert(0);
    if (type == GFDB_GETINFOLIST_HEAD)
        return g_filedb->elem[db_count]->info_list_head;
    else if (type == GFDB_GETINFOLIST_END)    
        return g_filedb->elem[db_count]->info_list_end;
    else if (type == GFDB_GETINFOLIST_FOR_READ)        
        return g_filedb->elem[db_count]->info_list_for_read;
    else
        return NULL;
}


int gfdb_putinfonode(int db_count, ENUM_GFDB_GETINFOLIST type, gfdb_info_list *node) {
    if ((!g_filedb) || (db_count < 0) || (g_filedb->elem[db_count] == NULL))
        assert(0);
    if (type == GFDB_GETINFOLIST_HEAD)
        g_filedb->elem[db_count]->info_list_head = node;
    else if (type == GFDB_GETINFOLIST_END)    
        g_filedb->elem[db_count]->info_list_end = node;
    else if (type == GFDB_GETINFOLIST_FOR_READ)
        g_filedb->elem[db_count]->info_list_for_read = node;
    else 
        return -1;
    return 1;        
}


int gfdb_resetinfonode(int db_count, int create_new_node) {
    gfdb_info_list *tmpnode;
    
    if ((!g_filedb) || (db_count < 0) || (g_filedb->elem[db_count] == NULL))
        assert(0);
    
    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
    
    tmpnode = g_filedb->elem[db_count]->info_list_end;
    while(tmpnode) {
        tmpnode = tmpnode->prev;
        if ((tmpnode) && (tmpnode->next)) free(tmpnode->next);
    }
    
    if (tmpnode) free(tmpnode);
    g_filedb->elem[db_count]->info_list_head = NULL;
    g_filedb->elem[db_count]->info_list_end = NULL;    
    g_filedb->elem[db_count]->info_list_for_read = NULL;    
    
    
    if (create_new_node == 1) {
        g_filedb->elem[db_count]->info_list_head = gfdb_info_create_node();
        if (g_filedb->elem[db_count]->info_list_head < 0) {
            printf("gfdb_resetinfonode: create node error\n");
            pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
            return -1;
        }
    
        g_filedb->elem[db_count]->info_list_end = g_filedb->elem[db_count]->info_list_head;
        g_filedb->elem[db_count]->info_list_for_read = g_filedb->elem[db_count]->info_list_head;
    }
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
    return 1;   
}


int gfdb_info_putfilesize(int db_count, gfdb_info_list* node, int64_t fsize) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (!node)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    node->file_size = fsize;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;    
}

int gfdb_info_putfileindex(int db_count, gfdb_info_list* node, int index) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (!node)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    node->filelist_idx = index;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;    
}

int gfdb_info_putstartaddr(int db_count, gfdb_info_list* node, off64_t start_addr) {
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL) ||
        (!node)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    

    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));

    node->start_addr = start_addr;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));

    return 1;    
}


/*
 * append one new node to the last of the link list
 */
gfdb_info_list * gfdb_info_append_node(int db_count) {
    gfdb_info_list *newnode, *tmpnode;
    
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)){
        printf("gfiledb put value fail\n");
        assert(0);
    }    
    
    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
    tmpnode = g_filedb->elem[db_count]->info_list_end;
    
    newnode = (gfdb_info_list *)malloc(sizeof(gfdb_info_list));
    if (newnode == NULL) {
        printf("gfile_info_list malloc error\n");
        pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
        return NULL;
    }
    
    memset(newnode, 0, sizeof(gfdb_info_list));
    tmpnode->next = newnode;
    newnode->prev = tmpnode;   
    newnode->filelist_idx = (gf_info_getidx(tmpnode) + 1);
    // index wrap around
    if (g_filedb->elem[db_count]->max_ring_size != 0) {
        if (newnode->filelist_idx == ((g_filedb->elem[db_count]->max_ring_size/MAX_GLIST_SIZE) + 2)) {
            printf("libfaccess: WARNING: file index warp around to 0 !\n");
            newnode->filelist_idx = 0;
        }            
    }
    newnode->file_size = 0;
    g_filedb->elem[db_count]->info_list_end = newnode;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
    return newnode;    
}

/*
 * append one new node to the last of the link list
 */
int gfdb_info_removeheadnode(int db_count) {
    gfdb_info_list *node = NULL;
    
    if ((!g_filedb) ||
        (db_count > MAX_GFDB_ELEM) || 
        (db_count < 0) || 
        (g_filedb->elem[db_count] == NULL)){
        printf("gfiledb put value fail\n");
        assert(0);
    }
     
    pthread_mutex_lock(&(g_filedb->elem[db_count]->elm_mutex));
      
    if (!g_filedb->elem[db_count]->info_list_head->next) {
        printf("libfaccess: WARNING: gfdb_info_removeheadnode only one node exist\n");
        pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
        return -1;
    }
    node = g_filedb->elem[db_count]->info_list_head->next;
    node->prev = NULL;
    free(g_filedb->elem[db_count]->info_list_head);
    g_filedb->elem[db_count]->info_list_head = node;
    
    pthread_mutex_unlock(&(g_filedb->elem[db_count]->elm_mutex));
    return 1;
}
