#include "file_access.h"
#include <sys/stat.h>
#include "internal.h"
#include "file_db.h"
#include "file_util.h"
// #include <Util.h>
#include "OpenFile.h"

//////////////////////////////////////////////////////////////////
// Basic functions

ENUM_GFILE_TYPE GgetFileType(gfile_t *file_st) {
    if (file_st == NULL)
        return GFILE_TYPE_UNKNOWN;
    return file_st->type;    
}


gfile_t * GFileOpen(const char *filename, int oflag, ENUM_GFILE_TYPE gfile_type) {
    gfile_t *file_st = (gfile_t *)malloc(sizeof(gfile_t));
    if (file_st == NULL)
        return NULL;
       
    switch(gfile_type) {
        case GFILE_TYPE_ARRAY:
            file_st->type = GFILE_TYPE_ARRAY;
            file_st->ptr = (void *)gf_array_open(filename, oflag);
            break;
        case GFILE_TYPE_LIST:    
            file_st->type = GFILE_TYPE_LIST;
            file_st->ptr = (void *)gf_list_open(filename, oflag);
            break;
        case GFILE_TYPE_SIMPLE:  
            file_st->type = GFILE_TYPE_SIMPLE;
            file_st->ptr = (void *)gf_simple_open(filename, oflag);   
            break;
        default:
            assert(0);   
    }           
    
    if (!file_st->ptr) {
        free(file_st);
        return NULL;
    }
    return file_st;
}


gfile_t * GFileOpenRING(const char *filename, int oflag, int64_t buf_size, ENUM_GFILE_TYPE gfile_type) {
    gfile_t *file_st = (gfile_t *)malloc(sizeof(gfile_t));
    if (file_st == NULL)
        return NULL;
        
    if (gfile_type == GFILE_TYPE_ARRAY) {
        file_st->type = GFILE_TYPE_ARRAY;
        file_st->ptr = (void *)gf_array_openring(filename, oflag, buf_size);
    }
    if (gfile_type == GFILE_TYPE_LIST) {
        file_st->type = GFILE_TYPE_LIST;
        file_st->ptr = (void *)gf_list_openring(filename, oflag, buf_size);
    }
    
    if (!file_st->ptr) {
        free(file_st);
        return NULL;
    }
    return file_st;    
}

int GFileClose(gfile_t *file_st) {
    if (!file_st)
        return -1;
        
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            gf_array_close((gfarray_t *)file_st->ptr);
            break;
        case GFILE_TYPE_LIST:    
            gf_list_close((gflist_t *)file_st->ptr);
            break;
        case GFILE_TYPE_SIMPLE:  
            gf_simple_close((gfsimple_t *)file_st->ptr);   
            break;
        default:
            assert(0);   
    }        
        
    free(file_st);
    file_st = NULL;
    return 1;
}


ssize_t GFileWriteBytes(gfile_t *file_st, void *data, size_t nbyte) {
    if ((!file_st) || (!data))
        return -1;
    
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            return gf_array_writebytes((gfarray_t *)file_st->ptr, data, nbyte);
        case GFILE_TYPE_LIST:    
            return gf_list_writebytes((gflist_t *)file_st->ptr, data, nbyte);
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_writebytes((gfsimple_t *)file_st->ptr, data, nbyte);
        default:
            return -1;   
    }
}

ssize_t GFileReadBytes(gfile_t *file_st, void *data, size_t nbyte) {
    if ((!file_st) || (!data))
        return -1;
    
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            return gf_array_readbytes((gfarray_t *)file_st->ptr, data, nbyte);
        case GFILE_TYPE_LIST:    
            return gf_list_readbytes((gflist_t *)file_st->ptr, data, nbyte);
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_readbytes((gfsimple_t *)file_st->ptr, data, nbyte);
        default:
            return -1;   
    }                
}

off64_t GFileSeek(gfile_t *file_st, off64_t bytes, int whence) {
    if (!file_st)
        return -1;
    
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            return gf_array_seek((gfarray_t *)file_st->ptr, bytes, whence);
        case GFILE_TYPE_LIST:    
            return gf_list_seek((gflist_t *)file_st->ptr, bytes, whence);
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_seek((gfsimple_t *)file_st->ptr, bytes, whence);
        default:
            return -1;   
    }               
}

bool GFileTestExist(const char *file_name, ENUM_GFILE_TYPE gfile_type) {    
    int i, bHasDot;
    struct stat tmpstat;
    char file_name_only[MAX_PATH_LENGTH], file_name_extension[MAX_PATH_LENGTH];    
        
    // Test if this file is exist.
    if (stat(file_name, &tmpstat) == 0)
        return true;
        
    bHasDot = 0;
    memset(file_name_only, 0, MAX_PATH_LENGTH);
    memset(file_name_extension, 0, MAX_PATH_LENGTH);
    
    // parse for file_name and file_name_extension
    for (i=strlen(file_name); i>=0; i--) {
        if (file_name[i] == '.') {
            bHasDot = 1;
            sprintf(file_name_extension, "%s", &(file_name[i]));
            snprintf(file_name_only, i+1, "%s", file_name);
            break;
        }
    }
    if(!bHasDot) {
        printf("libfacess: GFileTestExist filename %s format error\n", file_name);
        return GFILE_ERR_FILE_NOT_EXIST;
    }    
        
        
    if (gfile_type == GFILE_TYPE_ARRAY)
        return gf_array_testfileexist(file_name_only, file_name_extension);
    else if (gfile_type == GFILE_TYPE_LIST)
        return gf_list_testfileexist(file_name_only, file_name_extension);
    
    return false;       
}

off64_t GgetFileSize(gfile_t *file_st) {
    if (!file_st)
        return -1;
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            return gf_array_getfilesize((gfarray_t *)file_st->ptr);
        case GFILE_TYPE_LIST:    
            return gf_list_getfilesize((gflist_t *)file_st->ptr);
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_getfilesize((gfsimple_t *)file_st->ptr);
        default:
            return -1;   
    }        
}

off64_t GgetMaxWritableTotalFileSize(gfile_t *file_st) {
    if (!file_st)
        return -1;
        
    if (file_st->type == GFILE_TYPE_ARRAY)
        return gf_array_getmaxwtotalsize((gfarray_t *)file_st->ptr);
    else
        printf("libfaccess: GFILE_TYPE_LIST/GFILE_TYPE_SIMPLE not implement GgetMaxWritableTotalFileSize() \n");
        
    return -1;    
}

int GFileSetForceWriteToNextFile(gfile_t *file_st, off64_t cut_pos) {
    if (!file_st)
        return -1;
        
    if (file_st->type == GFILE_TYPE_ARRAY)
        return gf_array_setforcewritetonextfile((gfarray_t *)file_st->ptr, cut_pos);
    else
        printf("libfaccess: GFILE_TYPE_LIST/GFILE_TYPE_SIMPLE not implement GgetMaxWritableTotalFileSize() \n");
        
    return -1;    
}


//@NOTE: DO not use this before LivePause recording or ABrecord
//       only can call before normal recording
int64_t GFileGetFreeSpace(gfile_t *file_st) {
    if (!file_st)
        return -1;
       
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            return gf_array_getfreespace((gfarray_t *)file_st->ptr);
        case GFILE_TYPE_LIST:    
            return gf_list_getfreespace((gflist_t *)file_st->ptr);
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_getfreespace((gfsimple_t *)file_st->ptr);
        default:
            return -1;   
    }      
}


ssize_t GFileIFOWriteBytes(gfile_t *file_st, void *data, size_t nbyte) {
    if (!file_st)
        return -1;
            
    if (file_st->type == GFILE_TYPE_LIST)
        return gf_list_ifo_writebytes((gflist_t *)file_st->ptr, data, nbyte);
        
    return -1;    
}

ssize_t GFileIFOReadBytes(gfile_t *file_st, void *data, size_t nbyte) {
    if (!file_st)
        return -1;
        
    if (file_st->type == GFILE_TYPE_LIST)
        return gf_list_ifo_readbytes((gflist_t *)file_st->ptr, data, nbyte);
        
    return -1;        
}

off64_t GFileIFOSeek(gfile_t *file_st, off64_t bytes, int whence) {
    if (!file_st)
        return -1;
        
    if (file_st->type == GFILE_TYPE_LIST)
        return gf_list_ifo_seek((gflist_t *)file_st->ptr, bytes, whence);
        
    return -1;          
}


/*
 *  purpose: set file's size to zero
 */
int GFileIFOReset(gfile_t *file_st) {
    if (!file_st)
        return -1;
    
    if (file_st->type == GFILE_TYPE_LIST)
        return gf_list_ifo_reset((gflist_t *)file_st->ptr);
        
    return -1;         
}

//////////////////////////////////////////////////////////////////
// Editing related functions

ENUM_GFILE_EDITRET GFileABCopy(gfile_t *file_src, int64_t start, int64_t end, gfile_t *file_dst) {    
    if (!file_src)
        return GFILE_ERR_EDIT_ERROR;
            
    if (file_src->type == GFILE_TYPE_LIST)        
        return gf_list_abcopy((gflist_t *)file_src->ptr, start, end, (gflist_t *)file_dst->ptr);
        
    return GFILE_ERR_EDIT_ERROR;     
}

ENUM_GFILE_EDITRET GFileABFastCopy(char *srcFileName, char *dstFileName, int64_t start, int64_t end) {           
    return gf_array_abfastcopy(srcFileName, dstFileName, start, end);            
}

int GFileCancelDeleteFile() {
    return gf_array_filedeletecancel();
}

ENUM_GFILE_EDITRET GFileDelete(const char *file_name) {
    int i, bHasDot;
    char file_name_only[MAX_PATH_LENGTH], file_name_extension[GFILE_EXTENSION_LENGTH];
    
    bHasDot = 0;
    memset(file_name_only, 0, MAX_PATH_LENGTH);
    memset(file_name_extension, 0, GFILE_EXTENSION_LENGTH);
    
    // parse for file_name and file_name_extension
    for (i=strlen(file_name); i>=0; i--) {
        if (file_name[i] == '.') {
            bHasDot = 1;
            sprintf(file_name_extension, "%s", &(file_name[i]));
            snprintf(file_name_only, i+1, "%s", file_name);
            break;
        }
    }
    if(!bHasDot) {
        printf("libfacess: GFileDelete filename %s has no '.', delete directly !\n", file_name);
        if (unlink(file_name) == 0)
            return GFILE_EDIT_SUCCESS;
        else    
            return GFILE_ERR_FILE_NOT_EXIST;
    }    
    
    // If extension filename is METADATA
    if (strcmp(file_name_extension, METADATA_EXTFILENAME) == 0) {
        printf("input name is metadata, try list delete\n");
        return gf_list_filedelete(file_name, file_name_only, file_name_extension);
    }
    // input filename is not METADATA file, start trying all cases.
    else {                
        int fd; 
        char full_path[MAX_PATH_LENGTH];
        
        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s%s", file_name_only, METADATA_EXTFILENAME);
        
        // try rename to METADATA filename
        fd = open64(full_path, O_RDONLY);
        if (fd > 0) {
            // METADATA filename exist, 
            printf("meta file is exist, try list delete\n");
            close(fd);            
            memset(file_name_extension, 0, 10);
            sprintf(file_name_extension, "%s", METADATA_EXTFILENAME);
            return gf_list_filedelete(full_path, file_name_only, file_name_extension);
        }
        
        // Other cases go here.
        printf("try array delete\n");
        return gf_array_filedelete(file_name, file_name_only, file_name_extension);
    }
}


ENUM_GFILE_EDITRET GFileRename(const char *old_file_name, const char *new_file_name) {
    printf("[GFileRename] old_file_name: %s, new_file_name: %s\n", old_file_name, new_file_name);
    return gf_array_filerename(old_file_name, new_file_name);
}

ENUM_GFILE_EDITRET GFileProtect(const char *file_name, int on) {
    return gf_array_fileprotect(file_name, on);
}

void GGetFileName (char *in_filepath, char *in_filename, char *out_filename, int length, bool *isKeyFileName) {
    return gf_array_getfilename(in_filepath, in_filename, out_filename, length, isKeyFileName);
}

void GGetAllRealFileName(char *in_filepath, char *in_filename, int *cnt, char filenameAry[][MAX_PATH_LENGTH]) {
    return gf_array_getallrealfilename(in_filepath, in_filename, cnt, filenameAry);
}

off64_t GGetFileSetSize(char *in_filename, int *out_iswriting) {
    off64_t ret;
    if ((ret = gf_array_getfilesetsize(in_filename, out_iswriting)) == -1)         
        return gf_list_getfilesetsize(in_filename, out_iswriting);

    return ret;    
}

ENUM_GFILE_EDITRET GFileMerge(gfile_t *file_st1, gfile_t *file_st2) {
    if ((!file_st1) || (!file_st2)) {
        printf("libfaccess: GFileMerge open file error\n");
        return GFILE_ERR_EDIT_ERROR;
    }    
    
    return gf_list_filemerge((gflist_t *)file_st1->ptr, (gflist_t *)file_st2->ptr);
}


// Use example:
//   file_src = GFileOpen(filename1, O_RDONLY, GFILE_TYPE_LIST);
//   file_dst = GFileOpen(filename2, O_CREAT | O_RDWR, GFILE_TYPE_LIST);  
//   GFileSplit(file_src, file_dst, 2048);
// @NOTICE: file_src must open by O_RDONLY !!!!!
ENUM_GFILE_EDITRET GFileSplit(gfile_t *file_src, gfile_t *file_dst, off64_t split_pos) {
    if ((!file_src) || (!file_dst)) {
        printf("libfaccess: GFileSplit open file error\n");
        return GFILE_ERR_EDIT_ERROR;
    }
   
    return gf_list_filesplit((gflist_t *)file_src->ptr, (gflist_t *)file_dst->ptr, split_pos);     
}


ENUM_GFILE_EDITRET GFileABCut(gfile_t *file_st, int64_t pos_a, int64_t pos_b) {
    if (!file_st)
        return GFILE_ERR_EDIT_ERROR;
    
    return gf_list_abcut((gflist_t *)file_st->ptr, pos_a, pos_b);    
}


////////////////////////////////////////////////////////////////////////////
// related functions of settings exchange between Playback and Authoring 

int GGetFileIsWriting(gfile_t *file_st) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_getwrite(timeshift_ref);
 
    return -1;      
}

unsigned int GGetFileTimeLength(gfile_t *file_st) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_getlengthtime(timeshift_ref);
 
    return 0;      
}

int GSetFileTimeLength(gfile_t *file_st, unsigned int time_length) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_putlengthtime(timeshift_ref, time_length);
 
    return -1;            
}

int GGetFileTVFormat(gfile_t *file_st) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_gettvformat(timeshift_ref);
 
    return -1;            
}

int GSetFileTVFormat(gfile_t *file_st, int tvformat) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_puttvformat(timeshift_ref, tvformat);
 
    return -1;           
}

int GGetFileBitRate(gfile_t *file_st) {
    int timeshift_ref = -1;    

    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_getbitrate(timeshift_ref);
 
    return -1; 
}

int GSetFileBitrate(gfile_t *file_st, int bitrate) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_putbitrate(timeshift_ref, bitrate);
 
    return -1; 
}

int GGetFileUserContext(gfile_t *file_st, void **context, osal_mutex_t **mutex, int *size) {
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return gfdb_getcontext(timeshift_ref, context, mutex, size);
 
    return -1;
}

int GSetFileUserContext(gfile_t *file_st, void *ptr, osal_mutex_t *mutex, int size) 
{
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return (gfdb_putcontext(timeshift_ref, ptr, mutex, size));
 
    return -1;
}

int GCreateFileUserContext(gfile_t *file_st, int size, int need_mutex, void (*destructor)(void* context, int size)) 
{
    int timeshift_ref = -1;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    if (file_st->type == GFILE_TYPE_ARRAY)
        timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
    else if (file_st->type == GFILE_TYPE_LIST)
        timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;

    if (timeshift_ref != -1)
        return (gfdb_createcontext(timeshift_ref, size, need_mutex, destructor));
 
    return -1;
}

int GLockResource(gfile_t *file_st, int timeout)
{
    int timeshift_ref;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;

    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
            break;
        case GFILE_TYPE_LIST:    
            timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;
            break;
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_lockresource((gfsimple_t *)file_st->ptr, timeout);
        default:
            return -1;   
    }        

    if (timeshift_ref != -1)
        return (gfdb_lockresource(timeshift_ref, timeout));
 
    return -1;
}

int GUnlockResource(gfile_t *file_st)
{
    int timeshift_ref;
    
    if ((file_st == NULL) || (file_st->ptr == NULL))
        return -1;
        
    switch(file_st->type) {
        case GFILE_TYPE_ARRAY:
            timeshift_ref = ((gfarray_t *)(file_st->ptr))->timeshift_ref;
            break;
        case GFILE_TYPE_LIST:    
            timeshift_ref = ((gflist_t *)(file_st->ptr))->timeshift_ref;
            break;
        case GFILE_TYPE_SIMPLE:  
            return gf_simple_unlockresource((gfsimple_t *)file_st->ptr);
        default:
            return -1;   
    }        

    if (timeshift_ref != -1)
        return (gfdb_unlockresource(timeshift_ref));
 
    return -1;
}


int GExternalLockResouce(char *path, int timeout)
{    
    return gfdb_external_lockresource(path, timeout);
}

int GExternalUnLockResouce(char *path)
{    
    return gfdb_external_unlockresource(path);
}
