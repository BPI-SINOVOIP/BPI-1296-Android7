#include "internal.h"
#include "file_db.h"
// #include <Util.h>

//For UDF ABCUT
#include <sys/ioctl.h>
#include <linux/ioctl.h>
// #include <linux/udf_fs_i.h>
#include "dvd_path_define.h"
#include "stat.h"
#include "OpenFile.h"

// For AB Fast Copy
#include "fastcopy_wrapper.h"


// The Flag for canceling delete file
bool g_bCanDeleteFileInGFile = false;


#define _SAFE_FREE(file_st)                                            \
        {   if(file_st->title_devs) free(file_st->title_devs);         \
            if(file_st->title_sizes) free(file_st->title_sizes);       \
            if(file_st->max_eachfile_write_size) free(file_st->max_eachfile_write_size); \
            free(file_st);                                        }        

static off64_t _get_filesystem(gfarray_t *file_st, const char *filename) {
    int i;
    struct statfs mystatfs;
    char full_path[MAX_PATH_LENGTH];
    
    // max file size decision (2G or MAX ?)
    for (i=strlen(filename); i>=0; i--) {
        if (filename[i] == '/') {
            memset(full_path, 0, MAX_PATH_LENGTH);
            snprintf(full_path, i+1, "%s", filename);
            break;
        }
    }
    memset(&mystatfs, 0, sizeof(struct statfs));
    statfs(full_path, &mystatfs);

    if ((mystatfs.f_type == 0xEF53) ||      // EXT2/3
        (mystatfs.f_type == 0x5346544e) ||  // NTFS ?
        (mystatfs.f_type == 0x44534655) ||  // Paragon NTFS
        (mystatfs.f_type == 0x6969) ||      // NFS
        (mystatfs.f_type == 0x15013346) ||  // UDF_SUPER_MAGIC
        (strstr(file_st->file_name, VR_HDD_PATH))) {   // HDD's UDF2.0 partition        
        printf("file max size set to: 2TB\n");
        return GFARRAY_MAX_FILESIZE_UNLIMIT;
    }
    else {                
        printf("file max size set to: 4GB-1\n");
        return GFARRAY_MAX_FILESIZE;        
    }
    
}


/*
 *  local function, only used by GFileOpen() and GFileOpenRING()
 */
static gfarray_t * _init_gfarray(const char *filename) {    
    int bHasDot = 0, i, namelength;    
    char full_path[MAX_PATH_LENGTH];
    gfarray_t *file_st = NULL;

    if (!filename)
        return NULL;

    // create database if it has not been created.
    if (!gf_create_db()) {
        printf("gfile create db fail\n");
        return NULL;
    }

    // initialize some variables of gfarray_t
    file_st = (gfarray_t *)malloc(sizeof(gfarray_t));
    if (!file_st) {
        printf("libfaccess: malloc fail\n");
        return NULL;
    }

    memset(file_st, 0, sizeof(gfarray_t));
    memset(full_path, 0, MAX_PATH_LENGTH);
    
    file_st->seek_pos = 0;
    file_st->timeshift_ref = -1;
    file_st->title_devs = NULL;
    file_st->title_sizes = NULL; 
    file_st->max_eachfile_write_size = NULL;   

    // parse for file_name and file_name_extension
    sprintf(full_path, "%s", filename);
    namelength = strlen(full_path);
    for (i=namelength; i>=0; i--) {
        if (full_path[i] == '.') {
            bHasDot = 1;
            snprintf(file_st->file_name_extension, GFILE_EXTENSION_LENGTH, "%s", &(full_path[i]));
            snprintf(file_st->file_name, i+1, "%s", full_path);
            break;
        }
    }

    // if input filename doesn't have "." (extension filename).
    if (!bHasDot)        
        sprintf(file_st->file_name, "%s", full_path);

    return file_st;
}



/*
 *  local function, only used by GFileOpen() and GFileOpenRING()
 */
static int _init_gfarray_db(gfarray_t *file_st, const char *filename) {
    int i;

    // Database related:
    // if there is the same key in db, remember the pointer which is for file size update.
    // otherwise this is not timeshift case, create the db entry and remember the pointer
    if ((file_st->timeshift_ref = isexist_gfdb(filename)) != -1) {
        addref_gfdb(file_st->timeshift_ref);
        
        // if it is opened by "R" and "W" has already opened, then get file system type
        // ps: Ring File is setted in GFileOpenRING, so don't need to set again.
        // @HACK: statfs()'s speed maybe very slow... 
        //        LoadMedia() may call GFileOpen many times.
        //        So don't call get_filesystem() if the file is opened by "R"....
        //        The exception is timeshift case ....
        if ((!HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) &&
            (!HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF))) {
            off64_t max_file_size = _get_filesystem(file_st, filename);  
            file_st->max_eachfile_write_size = (off64_t *)malloc(sizeof(off64_t)*GFARRAY_COUNT_MAX);
            for (i=0; i<GFARRAY_COUNT_MAX; i++)
                file_st->max_eachfile_write_size[i] = max_file_size;
            file_st->max_ring_write_size = max_file_size;    
            printf("Read Catch Timeshift !!!!!!");
        }
    }
    else {
        //Need to add error handling code to deal with the no space case in the database.
        if ((file_st->timeshift_ref = gfdb_addkey(filename)) == -1) {
            printf("libfileaccess: file %s open error since there is no more space in the data spcae!\n", filename);
            for (i = 0; i<file_st->file_count; i++) {
                if (file_st->title_devs[i].fd)
                    close (file_st->title_devs[i].fd );
            }
            _SAFE_FREE(file_st);
            return -1;
        }
    }


    // update database information if 
    // (1) this is w mode file.
    // (2) this is r mode file, and this is the first time opened (not timeshift).
    if ((HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) ||
        ((!HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) && (getref_gfdb(file_st->timeshift_ref) == 1))) {
        gfdb_putvalue(file_st->timeshift_ref, file_st->title_sizes, 0, (file_st->file_count-1));
        gfdb_putfilecount(file_st->timeshift_ref, file_st->file_count);
    }


    // update write flag
    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) 
        gfdb_putwrite(file_st->timeshift_ref, 1);
        
    // update Ring Size info
    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
        if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) 
            gfdb_putRINGfilesize(file_st->timeshift_ref, file_st->max_ring_write_size);   
        else
            file_st->max_ring_write_size = gfdb_getRINGfilesize(file_st->timeshift_ref);
    } 

    return 1;
}


gfarray_t * gf_array_open(const char *filename, int oflag) {
    int fd, i;
    off64_t max_file_size;    
    gfarray_t *file_st = NULL;
    char full_path[MAX_PATH_LENGTH];

    file_st = _init_gfarray(filename);
    if (file_st == NULL)
        return NULL;

    // Open by "W" mode
    if ((oflag & O_RDWR) || (oflag & O_WRONLY)) {             
        SET_FLAG(file_st->file_mode, GFILE_FMODE_WRITE);
        file_st->file_create_mode = oflag;

        // Database related:
        // if this file has already opened by w/r mode, can't be opened again
        // otherwise add this key to database.
        if (isexist_gfdb(filename) != -1) {
            printf("libfaccess: this file %s has already opened by w mode, can't open again\n", filename);
            free(file_st);
            return NULL;
        }

        max_file_size = _get_filesystem(file_st, filename); 

        // initial file count decided by filesystem type
        file_st->title_devs = (gdevs_t *)malloc(sizeof(gdevs_t)*GFARRAY_COUNT_MAX);
        memset(file_st->title_devs, 0, sizeof(gdevs_t)*GFARRAY_COUNT_MAX);
        file_st->title_sizes = (off64_t *)malloc(sizeof(off64_t)*GFARRAY_COUNT_MAX);
        memset(file_st->title_sizes, 0, sizeof(off64_t)*GFARRAY_COUNT_MAX);
        file_st->max_eachfile_write_size = (off64_t *)malloc(sizeof(off64_t)*GFARRAY_COUNT_MAX);                    
        for (i=0; i<GFARRAY_COUNT_MAX; i++)
            file_st->max_eachfile_write_size[i] = max_file_size;    
        file_st->max_ring_write_size = max_file_size;        
        
        if (max_file_size == GFARRAY_MAX_FILESIZE_UNLIMIT)            
            file_st->file_count = GFARRAY_COUNT_MIN;           
        else 
            file_st->file_count = GFARRAY_COUNT_INIT;   


        // open all file handles.
        for (i = 0; i<file_st->file_count; i++) {
            memset(full_path, 0, MAX_PATH_LENGTH);
            sprintf(full_path, "%s_R%02d%s", file_st->file_name, i, file_st->file_name_extension);

            // if file doesn't exist, assign the permission when create it.
            fd = open64(full_path, oflag, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
            if (fd < 0) {
                // file open error, assume there is no other files anymore!!
                printf("libfaccess: file %s open error1\n", full_path);
                _SAFE_FREE(file_st);                                        
                return NULL;
            }
 
            // file open success, initialize some variables
            printf("libfaccess: open file %s success\n", full_path);
            file_st->title_devs[i].fd = fd;
            file_st->title_devs[i].pos = -1;
            file_st->title_sizes[i] = lseek64(fd, 0, SEEK_END);
            lseek64(fd, 0, SEEK_SET);  
        }

    }
    // Open by "R" mode
    else {
        int fd;
        SET_FLAG(file_st->file_mode, GFILE_FMODE_READ);
        file_st->file_create_mode = oflag;

        // Test if this filname is existed,
        // if existed, means normal file playback.
        if ((fd = open64(filename, O_RDONLY)) != -1) {
            printf("libfaccess: file %s exist, open directly!\n", filename);
            close(fd);
            fd = open64(filename, oflag);
            if (fd < 0) {
                printf("libfaccess: file %s open error!\n", filename);
                _SAFE_FREE(file_st);
                return NULL;
            }
            // only 1 file now
            file_st->title_devs = (gdevs_t *)malloc(sizeof(gdevs_t)*1);
            memset(file_st->title_devs, 0, sizeof(gdevs_t)*1);
            file_st->title_sizes = (off64_t *)malloc(sizeof(off64_t)*1);
            memset(file_st->title_sizes, 0, sizeof(off64_t)*1);
            file_st->file_count = 1;

            // file open success, initialize some variables
            file_st->title_devs[0].fd = fd;
            file_st->title_devs[0].pos = -1;
            file_st->title_sizes[0] = lseek64(fd, 0, SEEK_END);
            lseek64(fd, 0, SEEK_SET);

            // Database related: 
            if ((file_st->timeshift_ref = isexist_gfdb(filename)) != -1) 
                addref_gfdb(file_st->timeshift_ref);     
            else {
                if ((file_st->timeshift_ref = gfdb_addkey(filename)) == -1){
                    // error handling for no more space in the database.
                    printf("libfileaccess: file %s open error since there is no more space in the data spcae!\n", full_path);
                    close(fd);
                    _SAFE_FREE(file_st);
                    return NULL;
                }
            }

            gfdb_putvalue(file_st->timeshift_ref, file_st->title_sizes, 0, (file_st->file_count-1));
            gfdb_putfilecount(file_st->timeshift_ref, file_st->file_count);

            return file_st;
        }

        if (errno == EHOSTDOWN) {
             //The remote server is down, for example, the cable is unplugged
		     _SAFE_FREE(file_st);
             return NULL;
        } 
        
        file_st->title_devs = (gdevs_t *)malloc(sizeof(gdevs_t)*GFARRAY_COUNT_MAX);
        memset(file_st->title_devs, 0, sizeof(gdevs_t)*GFARRAY_COUNT_MAX);
        file_st->title_sizes = (off64_t *)malloc(sizeof(off64_t)*GFARRAY_COUNT_MAX);
        memset(file_st->title_sizes, 0, sizeof(off64_t)*GFARRAY_COUNT_MAX);
        // max_eachfile_write_size will be malloc in _init_gfarray_db() if this is timeshift case
                
        // open all file handles to remember all file's size
        for (i = 0; i<GFARRAY_COUNT_MAX; i++) {
            memset(full_path, 0, MAX_PATH_LENGTH);
            sprintf(full_path, "%s_R%02d%s", file_st->file_name, i, file_st->file_name_extension);
         
            // if file doesn't exist, assign the permission when create it.
            fd = open64(full_path, oflag);
            if (fd < 0) {
                // file open error, assume there is no other files anymore!!
                if (i == 0) {
                    _SAFE_FREE(file_st);
                    return NULL;               
                }
                else 
                    break;                                 
            }
         
            // file open success, initialize some variables      
            printf("libfaccess: open file %s success\n", full_path);          
            file_st->file_count++;
            file_st->title_devs[i].fd = fd;
            file_st->title_devs[i].pos = -1;
            file_st->title_sizes[i] = lseek64(fd, 0, SEEK_END);
            lseek64(fd, 0, SEEK_SET);  
        }            
            
        for (i = 1; i<GFARRAY_COUNT_MAX; i++) {
            if (file_st->title_devs[i].fd) {
                printf("libfaccess: close file handle %d\n", i);
                close(file_st->title_devs[i].fd);
                file_st->title_devs[i].fd = 0;
            }                
        }
                
        file_st->last_open_file_handle = 0;
    }

    if (_init_gfarray_db(file_st, filename) < 0)
        return NULL;
    
    return file_st;    
}



gfarray_t * gf_array_openring(const char *filename, int oflag, int64_t buf_size) {
    int fd, i;           
    off64_t max_file_size;
    gfarray_t *file_st = NULL;
    char full_path[MAX_PATH_LENGTH];

    file_st = _init_gfarray(filename);
    if (file_st == NULL)
        return NULL;
   
    max_file_size = _get_filesystem(file_st, filename);    
   
    // initial file count decided by filesystem type
    if (max_file_size == GFARRAY_MAX_FILESIZE_UNLIMIT) {
        // don't need to alloc max size...since filesystem can support 2T file,           
        file_st->title_devs = (gdevs_t *)malloc(sizeof(gdevs_t)*GFARRAY_COUNT_MIN);
        memset(file_st->title_devs, 0, sizeof(gdevs_t)*GFARRAY_COUNT_MIN);
        file_st->title_sizes = (off64_t *)malloc(sizeof(off64_t)*GFARRAY_COUNT_MIN);
        memset(file_st->title_sizes, 0, sizeof(off64_t)*GFARRAY_COUNT_MIN);
        file_st->file_count = GFARRAY_COUNT_MIN;            
        file_st->max_eachfile_write_size = (off64_t *)malloc(sizeof(off64_t)*GFARRAY_COUNT_MIN);
        for (i = 0; i<GFARRAY_COUNT_MIN; i++)
            file_st->max_eachfile_write_size[i] = max_file_size;
    }
    else {
        int elm;
        if (buf_size > GFARRAY_MAX_FILESIZE) {
            elm = buf_size / GFARRAY_MAX_FILESIZE;
            if ((buf_size % GFARRAY_MAX_FILESIZE) != 0) elm++;
        }
        else
            elm = 1;
        file_st->title_devs = (gdevs_t *)malloc(sizeof(gdevs_t)*elm);
        memset(file_st->title_devs, 0, sizeof(gdevs_t)*elm);
        file_st->title_sizes = (off64_t *)malloc(sizeof(off64_t)*elm);
        memset(file_st->title_sizes, 0, sizeof(off64_t)*elm);
        file_st->file_count = elm;           
        file_st->max_eachfile_write_size = (off64_t *)malloc(sizeof(off64_t)*elm); 
        for (i=0; i<elm; i++) 
            file_st->max_eachfile_write_size[i] = max_file_size;
    }  

    file_st->max_ring_write_size = buf_size;

    // Open by "W" mode
    if ((oflag & O_RDWR) || (oflag & O_WRONLY)) {        
        SET_FLAG(file_st->file_mode, GFILE_FMODE_WRITE);
        SET_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF);
        file_st->file_create_mode = oflag;       

        // Database related:
        // if this file has already opened by w/r mode, can't be opened again
        // otherwise add this key to database.
        if (isexist_gfdb(filename) != -1) {
            printf("libfaccess: this file %s has already opened by w mode, can't open again\n", filename);
            _SAFE_FREE(file_st);
            return NULL;
        }        
    }
    // Open by "R" mode
    else {
        int fd;
        SET_FLAG(file_st->file_mode, GFILE_FMODE_READ);
        SET_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF);


        // Test if this filname is existed,
        // if existed, means normal file playback.
        if ((fd = open64(filename, O_RDONLY)) != -1) {
            printf("libfaccess: file %s exist, but this is RING type!! error!!\n", filename);
            close(fd);
            _SAFE_FREE(file_st);
            return NULL;
        }
    
    }

    // open all file handles.
    for (i = 0; i<file_st->file_count; i++) {
        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s_R%02d%s", file_st->file_name, i, file_st->file_name_extension);

        // if file doesn't exist, assign the permission when create it.
        fd = open64(full_path, oflag);
        if (fd < 0) {
            // file open error, assume there is no other files anymore!!
            printf("libfaccess: file %s open error1\n", full_path);
            _SAFE_FREE(file_st);                                       
            return NULL;         
        }
 
        // file open success, initialize some variables
        printf("libfaccess: open file %s success\n", full_path);
        file_st->title_devs[i].fd = fd;
        file_st->title_devs[i].pos = -1;
        file_st->title_sizes[i] = lseek64(fd, 0, SEEK_END);
        lseek64(fd, 0, SEEK_SET);  
    }

    _init_gfarray_db(file_st, filename);
    return file_st;    
}


/*
 * close all opened file handles
 * If file handles are opened by write mode:
 *   (1) delete unused files.
 *   (2) if only one file "myfile_R00.xxx" file is recorded, rename it to "myfile.xxx"
 */
int gf_array_close(gfarray_t *file_st) { 
    int i, oldfcount;
    char full_path[MAX_PATH_LENGTH];

    if (!file_st)
        return false;

    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)){
        gfdb_putwrite(file_st->timeshift_ref, 0);
    }

    oldfcount = file_st->file_count;

    for (i = 0; i<file_st->file_count; i++) {
        // close file handles
        if (file_st->title_devs[i].fd) {
            if (close(file_st->title_devs[i].fd) < 0)
                printf("libfaccess: file handle %d close error!\n", i);            
        }

        // delete unused files automatically if it is opened by Authoring
        if ((file_st->title_sizes[i] == 0) && (HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE))) {
            memset(full_path, 0, MAX_PATH_LENGTH);
            sprintf(full_path, "%s_R%02d%s", file_st->file_name, i, file_st->file_name_extension);
            if (unlink(full_path) == -1) 
                printf("libfaccess: unlink file %s error\n", full_path);
            oldfcount--;    
        }
    }    

    if (oldfcount < file_st->file_count) 
        gfdb_putfilecount(file_st->timeshift_ref, oldfcount);        

    // rename first file if opened by Authoring and only one file is recorded.
    // ps: RINGBUF don't need to rename....
    if ((HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) &&
        (!HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF))) { 

        // case 1: many files, and only file 1's size != 0
        // case 2: only one file, and this file's size != 0
        if (((file_st->file_count > 1) &&
            ((file_st->title_sizes[0] != 0) && (file_st->title_sizes[1] == 0))) ||
            ((file_st->file_count == 1) &&
             (file_st->title_sizes[0] != 0))) {
            
            if (getref_gfdb(file_st->timeshift_ref) == 1) {        
                char des[MAX_PATH_LENGTH];
                memset(full_path, 0, MAX_PATH_LENGTH);
                memset(des, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s_R00%s", file_st->file_name, file_st->file_name_extension);
                sprintf(des, "%s%s", file_st->file_name, file_st->file_name_extension);
                if (rename(full_path, des) == -1)
                    printf("libfaccess: rename %s to %s fail\n", full_path, des);
            }
            else
                printf("libfaccess: there is file still referencing, don't rename file\n");
        }
    }
    // Database related
    gfdb_deletekey(file_st->timeshift_ref);  

    _SAFE_FREE(file_st);
    file_st = NULL;
    return true;
}

/*
 * Local functions: create more file handles......
 */
static int _create_file_handles(gfarray_t *file_st, int start) {
    int i, fd, ret=0, num_open_handles;
    char full_path[MAX_PATH_LENGTH];

    if (!HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE))
        return -1;

    // if filesystem is FAT32 like, open 10 files each time
    // if filesystem is NTFS like, open 1 file each time
    if (file_st->max_ring_write_size == GFARRAY_MAX_FILESIZE_UNLIMIT)
        num_open_handles = GFARRAY_COUNT_INIT_MIN;
    else    
        num_open_handles = GFARRAY_COUNT_INIT;
    
    // open file handles.    
    for (i = start; i< (start+num_open_handles); i++) {
        if ((i+1) > GFARRAY_COUNT_MAX) {
            if (i == start)
                return -1;
            else
                break;
        }

        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s_R%02d%s", file_st->file_name, i, file_st->file_name_extension);

        // if file doesn't exist, assign the permission when create it.
        fd = open64(full_path, file_st->file_create_mode, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
        if (fd < 0) {
            // file open error, assume there is no other files anymore!!
            if (i == start) {
                printf("libfaccess: open file %s fail\n", full_path);
                return -1;   
            }         
            else 
                break;                                 
        }
 
        // file open success, initialize some variables
        printf("libfaccess: open file %s success\n", full_path);
        ret++;
        file_st->file_count++;
        file_st->title_devs[i].fd = fd;
        file_st->title_devs[i].pos = -1;
        file_st->title_sizes[i] = lseek64(fd, 0, SEEK_END);
        lseek64(fd, 0, SEEK_SET);          
    }

    // update the value in database
    gfdb_putvalue(file_st->timeshift_ref, file_st->title_sizes, start, start+ret);
    gfdb_putfilecount(file_st->timeshift_ref, file_st->file_count);

    return ret;
}

int64_t gf_array_getfreespace(gfarray_t *file_st) {
    int i;
    int64_t totalFreeSize;
    struct statfs64 mystatfs;
    char full_path[MAX_PATH_LENGTH];
        
    for (i=strlen(file_st->file_name); i>=0; i--) {
        if (file_st->file_name[i] == '/') {
            memset(full_path, 0, MAX_PATH_LENGTH);
            snprintf(full_path, i+1, "%s", file_st->file_name);
            break;
        }
    }  

//     if (statfs64(full_path, &mystatfs) < 0) {
//         printf("libfaccess: gf_array_getfreespace call statfs error\n");
//         return -1;
//     }
    
    totalFreeSize = mystatfs.f_bfree * mystatfs.f_bsize;    
    return totalFreeSize;      
}

bool gf_array_testfileexist(char *file_name_only, char *file_name_extension) {
    int i;
    struct stat tmpstat;
    char full_name[MAX_PATH_LENGTH];
        
    // Try file with _Rxx
    for (i=0; i<GFARRAY_COUNT_MAX; i++) {
        memset(full_name, 0, MAX_PATH_LENGTH);
        sprintf(full_name,"%s_R%02d%s", file_name_only, i, file_name_extension);    
        
        if (stat(full_name, &tmpstat) == 0)
            return true;
    }
    
    return false;        
}


ssize_t gf_array_writebytes(gfarray_t *file_st, void *data, size_t nbyte) {
    int i, end_parse_file, need_update_db = 0; 
    off64_t offset;
    //int64_t tmpPTS;

    if ((!file_st) || (!data))
        return -1;

    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
        gfdb_putRINGfilepos(file_st->timeshift_ref, file_st->seek_pos);
        offset = file_st->seek_pos % file_st->max_ring_write_size;
        end_parse_file = file_st->file_count;
        if (file_st->seek_pos > file_st->max_ring_write_size)
            need_update_db = 0;
        else
            need_update_db = 1;
        PDEBUG("[Write] seek_pos: %lld, ring_size: %lld, fcount: %d\n", file_st->seek_pos, file_st->max_ring_write_size, file_st->file_count);
    }
    else {
        offset = file_st->seek_pos;
        end_parse_file = file_st->file_count+1;
        need_update_db = 1;
        PDEBUG("[Write] seek_pos: %lld, file_count: %d\n", file_st->seek_pos, file_st->file_count);
    }

    
    for (i = 0; i < end_parse_file; i++) {        
        if ((!(file_st->title_devs[i].fd)) &&
            (!HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF))) {            
            printf("libfaccess: file handle %d not exist, create more file handles!!\n", i);
            if (_create_file_handles(file_st, i) == -1)                 
                return -1;            
        }
                
        if (offset < file_st->max_eachfile_write_size[i]) {    // find correct file!

            // only need to write one file
            if ((offset + nbyte) <= file_st->max_eachfile_write_size[i]) {
                if (file_st->title_devs[i].pos != offset) {
                    if (lseek64(file_st->title_devs[i].fd, offset, SEEK_SET) != offset) {
                        printf("libfaccess: file handle %d seek %lld error\n", i, offset);                        
                        return -1;
                    }
                }                
                file_st->title_devs[i].pos = offset;

                //printf("write...."); 
                //tmpPTS = pli_getPTS();
                PDEBUG("       (1) write %d bytes to %lld, fd: %d\n", nbyte, offset, i);
                if (write(file_st->title_devs[i].fd, data, nbyte) != nbyte) {                
                    printf("libfaccess: file handle %d write %d bytes error\n", i, nbyte);                    
                    return -1;
                }
                //printf("back! pts: %lld\n", (pli_getPTS() - tmpPTS));

                // write success, update some variables.                
                file_st->title_devs[i].pos = offset + nbyte;
                file_st->seek_pos += nbyte;                
                                
                if (file_st->title_sizes[i] < (offset + nbyte)) 
                    file_st->title_sizes[i] = offset + nbyte;  

                // update database
                if (need_update_db)
                    gfdb_putvalue(file_st->timeshift_ref, file_st->title_sizes, i, i);   

                break;
            } // end of if ((offset + nbyte) <= max_eachfile_write_size[i])

            // need to write two files.
            else {

                int part1_size = file_st->max_eachfile_write_size[i] - offset;

                ////// write part1 //////
                if (file_st->title_devs[i].pos != offset) {
                    if (lseek64(file_st->title_devs[i].fd, offset, SEEK_SET) != offset) {
                        printf("libfaccess: file handle %d seek %lld error\n", i, offset); 
                        return -1;
                    }
                }
                file_st->title_devs[i].pos = offset; 

                PDEBUG("       (2) write %d bytes to %lld, fd: %d\n", part1_size, offset, i);
                if (write(file_st->title_devs[i].fd, data, part1_size) != part1_size) {
                    printf("libfaccess: file handle %d write %d bytes error\n", i, part1_size); 
                    return -1;
                }             

                // write success, update some variables.
                file_st->title_devs[i].pos = offset + part1_size;                
                file_st->seek_pos += part1_size;
                
                if (file_st->title_sizes[i] < (offset + part1_size)) 
                    file_st->title_sizes[i] = offset + part1_size;     


                ///// write part2 /////
                if (!(file_st->title_devs[i+1].fd)) {
                    printf("libfaccess: file handle %d not exist, write part2 error!\n", i+1);
                    if (_create_file_handles(file_st, i) == -1)         
                        return -1;                    
                }

                if (file_st->title_devs[i+1].pos != 0) {
                    if (lseek64(file_st->title_devs[i+1].fd, 0, SEEK_SET) != 0) {
                        printf("libfaccess: file handle %d seek 0 error\n", i+1);                        
                        return -1;
                    }
                }
                file_st->title_devs[i+1].pos = 0; 

                PDEBUG("       (3) write %d bytes to 0, fd: %d\n", (nbyte - part1_size), i+1);
                if (write(file_st->title_devs[i+1].fd, 
                          ((char *)data + part1_size), 
                          (nbyte - part1_size)) != (nbyte - part1_size)) {
                    printf("libfaccess: file handle %d write %d bytes error\n", i+1, nbyte - part1_size);                    
                    return -1;
                }    

                // write success, update some variables.  
                file_st->title_devs[i+1].pos = nbyte - part1_size;              
                file_st->seek_pos += (nbyte - part1_size);
                
                if (file_st->title_sizes[i+1] < (nbyte- part1_size))
                   file_st->title_sizes[i+1] = nbyte- part1_size;

                // update database
                if (need_update_db)
                    gfdb_putvalue(file_st->timeshift_ref, file_st->title_sizes, i, i+1);   
                break;   
            }
        } // end of  if (offset < max_eachfile_write_size[i])
        else {    
            //printf("(2) i: %d, title_size: %lld\n", i, file_st->title_sizes[i]);
            offset -= file_st->title_sizes[i];
        }
    } // end of for loop
    
    return nbyte;
}


/*
*   Read fail with -1: Read reaches the end of file and no more write for that file.
*                  -2: Read fail (EBUSY, EAGAIN)
*                  -3: Read fail(unknown reason).
*                  -4: NO_DEV/HOSTDOWN
*                  -5: file system error(EIO)
*                  -6: Ring File only. Data is overwitten
*                  -7: Read reaches the end of file and write continuous.
*                  -99: others
*/
ssize_t gf_array_readbytes(gfarray_t *file_st, void *data, size_t nbyte) {
    char full_path[MAX_PATH_LENGTH];
    int i, j, need_update_size = 1;
    off64_t offset, offr, total_size = 0;
    ssize_t ret=0, ret2=0;
    //int64_t tmpPTS;

    if ((!file_st) || (!data))
        return -1;
            
    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
        off64_t cur_writepos;
        // update total size and file count now
        offset = file_st->seek_pos % file_st->max_ring_write_size ;

        if (file_st->timeshift_ref != -1) {
            total_size = gfdb_gettotalfilesize(file_st->timeshift_ref);
            file_st->file_count = gfdb_getfilecount(file_st->timeshift_ref);
            cur_writepos = gfdb_getRINGfilepos(file_st->timeshift_ref);
            // since playback may start when authoring write buffer wrap around many times
            // so always set need_update_size = 1 to avoid this bug
            need_update_size = 1;
        }
        else
            return -1;

        PDEBUG("[Read] seek_pos: %lld, ring_size: %lld, total_size: %lld, file_count: %d\n", file_st->seek_pos, file_st->max_ring_write_size, total_size, file_st->file_count); 
        PDEBUG("       cur_writepos: %lld, need_update_size: %d\n", cur_writepos, need_update_size);

        // check if seek pos is over write position.
        if (file_st->seek_pos >= cur_writepos) {
            if (gfdb_getwrite(file_st->timeshift_ref) == 1) {
                //Read reaches write and write does not stop                
                return -7;
            }
            else {
                //Read reaches write and write does stop                
                return -1;
            }
        }
           
        // check seek pos is correct for reverse play
        if ((cur_writepos > file_st->max_ring_write_size) &&
            (file_st->seek_pos < (cur_writepos-file_st->max_ring_write_size)))             
            return -6;       
    }
    else {
        // update total size and file count now
        offset = file_st->seek_pos;

        if (file_st->timeshift_ref != -1) {
            total_size = gfdb_gettotalfilesize(file_st->timeshift_ref);
            file_st->file_count = gfdb_getfilecount(file_st->timeshift_ref);
            if (gfdb_getwrite(file_st->timeshift_ref) == 0)
                need_update_size = 0;
        }

        PDEBUG("[Read] seek_pos: %lld, total_size: %lld, file_count: %d\n", file_st->seek_pos, total_size, file_st->file_count);
        
        // offset is correct or now
        if (offset >= total_size) {
            if (file_st->timeshift_ref != -1) {
                if (gfdb_getwrite(file_st->timeshift_ref) == 1) {
                    //Read reaches write and write does not stop                    
                    return -7;
                }
                else {
                    //Read reaches write and write does stop                    
                    return -1;
                }
            }
            //reach the EOF            
            return -1;
        }
    }



    for (i = 0; i < file_st->file_count; i++) {
       
        // Update file size......... two conditions don't need to update single file size.
        // RING type: when seek_pos > max_ring_write_size
        // Normal type: not in timeshifting.
        if (need_update_size)
            file_st->title_sizes[i] = gfdb_getvalue(file_st->timeshift_ref, i);

        if (offset < file_st->title_sizes[i]) {  // find correct file !
            if (file_st->title_devs[i].fd == 0) {
                printf("close file handle %d\n", file_st->last_open_file_handle);                
                close(file_st->title_devs[file_st->last_open_file_handle].fd);
                file_st->title_devs[file_st->last_open_file_handle].fd = 0;
                file_st->title_devs[file_st->last_open_file_handle].pos = -1;
                memset(full_path, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s_R%02d%s", file_st->file_name, i, file_st->file_name_extension);                
                file_st->title_devs[i].fd = open64(full_path, file_st->file_create_mode);
                if (file_st->title_devs[i].fd < 0) {
                    printf("libfaccess: open file %s fail\n", full_path);
                    return -1;   
                }
                else
                    printf("libfaccess: open file %s success\n", full_path);
                file_st->last_open_file_handle = i;
            }
            
            // handling read two file case
            if (((offset + nbyte) > file_st->title_sizes[i]) &&
                (i != (file_st->file_count -1))) {

                int part1_size = file_st->title_sizes[i] - offset;

                ////// read part1 //////
                if (file_st->title_devs[i].pos != offset) {
                    offr = lseek64(file_st->title_devs[i].fd, offset, SEEK_SET);
                    if ((offr < 0) || (offr != offset)) {
                        printf("libfaccess: file handle %d seek %lld error\n", i, offset);                        
                        return ((offr < 0) ? offr : 0);
                    }
                }
                PDEBUG("       (2) read %d bytes from %lld, oldpos: %lld, fd: %d\n", part1_size, offset, file_st->title_devs[i].pos, i);
                file_st->title_devs[i].pos = offset;                
                
                //tmpPTS = pli_getPTS();                
                ret = read(file_st->title_devs[i].fd, data, part1_size);
                //printf(" pts: %lld\n", pli_getPTS()-tmpPTS);
                if (ret < 0) {
                    printf("libfileaccess: file handle %d pos %lld read %d bytes error\n", i, offset, part1_size);                    
                    if ((errno == ENODEV) || (errno == EHOSTDOWN))
                        return -4;
                    else if(errno == EIO)
                        return -5;
                    else if ((errno == EBUSY) || (errno == EAGAIN))
                        return -2;
                    else 
                        return -3;
                }

                // read success, update some variables
                if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
                    file_st->seek_pos += ret;
                    file_st->title_devs[i].pos = offset + ret;
                }
                else {
                    file_st->title_devs[i].pos = offset + ret;
                    file_st->seek_pos = offset + ret;
                    if (i > 0) {
                        for (j=0; j<i ; j++)
                            file_st->seek_pos += file_st->title_sizes[j];
                    }
                }

                if (file_st->title_devs[i+1].fd == 0) {      
                    printf("close file handle %d\n", file_st->last_open_file_handle);                          
                    close(file_st->title_devs[file_st->last_open_file_handle].fd);
                    file_st->title_devs[file_st->last_open_file_handle].fd = 0;
                    file_st->title_devs[file_st->last_open_file_handle].pos = -1;
                    memset(full_path, 0, MAX_PATH_LENGTH);
                    sprintf(full_path, "%s_R%02d%s", file_st->file_name, i+1, file_st->file_name_extension);                
                    file_st->title_devs[i+1].fd = open64(full_path, file_st->file_create_mode);
                    if (file_st->title_devs[i+1].fd < 0) {
                        printf("libfaccess: open file %s fail\n", full_path);
                        return -1;   
                    }
                    else
                        printf("libfaccess: open file %s success\n", full_path);
                    file_st->last_open_file_handle = i+1;
                }

                ///// read part2 /////
                if (!(file_st->title_devs[i+1].fd)) {
                    printf("libfaccess: file handle %d not exist, read part2 error!\n", i+1);
                    //return the first part, which is read successfully                    
                    return ret;
                }

                if (file_st->title_devs[i+1].pos != 0) {
                    offr = lseek64(file_st->title_devs[i+1].fd, 0, SEEK_SET);
                    if ((offr < 0) || (offr != 0)) {
                        printf("libfaccess: file handle %d seek 0 error\n", i);
                        //return the first part, which is read successfully                        
                        return ret;
                    }
                }
                PDEBUG("       (3) read %d bytes from 0, oldpos: %lld, fd: %d\n", (nbyte - part1_size), file_st->title_devs[i+1].pos, i+1);
                file_st->title_devs[i+1].pos = 0;
                
                ret2 = read(file_st->title_devs[i+1].fd,
                            ((char *)data + part1_size),
                            (nbyte - part1_size));
                if (ret2 < 0) {
                    printf("libfileaccess: file handle %d pos %lld read %d bytes error\n", i, offset, (nbyte-part1_size));
                    //return the first part, which is read successfully 
                    //Do not return fail since part of it success                   
                    return ret;
                }

                // read success, update some variables
                file_st->title_devs[i+1].pos = ret2;
                if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
                    file_st->seek_pos += ret2;
                }
                else {
                    file_st->seek_pos = ret2;
                    for (j=0; j<(i+1); j++)
                        file_st->seek_pos += file_st->title_sizes[j];
                }

                break;
            }
            // only need to read one file
            // will honor read size returned by read()
            // so don't need to concern about read too much
            else {
                if (file_st->title_devs[i].pos != offset) {
                    offr = lseek64(file_st->title_devs[i].fd, offset, SEEK_SET);
                    if ((offr < 0) || (offr != offset)) {
                        printf("libfaccess: file handle %d seek %lld error\n", i, offset);
                        //Other error                        
                        return -99;
                    }
                }
                PDEBUG("       (3) read %d bytes from %lld, oldpos: %lld, fd: %d\n", nbyte, offset, file_st->title_devs[i].pos, i);
                file_st->title_devs[i].pos = offset;                
                                
                //printf("read................");
                //tmpPTS = pli_getPTS();
                ret = read(file_st->title_devs[i].fd, data, nbyte);
                //printf("nbyte: %d, pts: %lld\n", nbyte, pli_getPTS()-tmpPTS);
                if (ret < 0) {
                    printf("libfileaccess: file handle %d pos %lld read %d bytes error\n", i, offset, nbyte);                    
                    if ((errno == ENODEV) || (errno == EHOSTDOWN))
                        return -4;
                    else if(errno == EIO)
                        return -5;
                    else if ((errno == EBUSY) || (errno == EAGAIN))
                        return -2;
                    else 
                        return -3;
                }

                if (ret != nbyte)
                    printf("libfaccess: read bytes %d not enough %d, maybe reach file end\n", ret, nbyte);

                // read success, update some variables
                file_st->title_devs[i].pos = offset + ret;
                if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
                    file_st->seek_pos += ret;
                } 
                else {
                    file_st->seek_pos = offset + ret;
                    if (i > 0) {
                        for (j=0; j<i ; j++)
                            file_st->seek_pos += file_st->title_sizes[j];
                    }
                }
                break;
            }
        } // end of if (offset < file_st->title_sizes[i])
        else {
            offset -= file_st->title_sizes[i];
        }
    }
    
    return ret + ret2;
}


/*
 * !NOTICE: 
 *   not really do seek operation now, but can be changed if someone needed in the future.
 */
off64_t gf_array_seek(gfarray_t *file_st, off64_t bytes, int whence) {
    off64_t old_seekpos;

    if ((file_st == NULL) || (bytes < 0))
        return -1;
        
    old_seekpos = file_st->seek_pos;

    if (whence == SEEK_SET) 
        file_st->seek_pos = bytes;
    else if (whence == SEEK_CUR) 
        file_st->seek_pos = file_st->seek_pos + bytes;
    else if (whence == SEEK_END) 
        file_st->seek_pos = gf_array_getfilesize(file_st) + bytes;    

    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_READ)) {
        int64_t total_size;
        if (!HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF))
            total_size = gfdb_gettotalfilesize(file_st->timeshift_ref);
        else
            total_size = gfdb_getRINGfilepos(file_st->timeshift_ref);

        if (file_st->seek_pos > total_size) {
            file_st->seek_pos = old_seekpos;
            return -1;
        }
    }

    return file_st->seek_pos;
}


off64_t gf_array_getfilesize(gfarray_t *file_st) {
    int i;
    off64_t filesize = 0;

    if (file_st == NULL)
        return -1;

    if (file_st->timeshift_ref != -1)
        filesize = gfdb_getvalue(file_st->timeshift_ref, -1);
    else {
        for (i=0; i<file_st->file_count; i++)
            filesize += file_st->title_sizes[i];
    }

    return filesize;    
}

off64_t gf_array_getmaxwtotalsize(gfarray_t *file_st) {
    if (file_st == NULL)
        return -1;

    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF)) {
        if (file_st->max_eachfile_write_size[0] == GFARRAY_MAX_FILESIZE_UNLIMIT) 
            return (off64_t)(file_st->max_eachfile_write_size[0]*GFARRAY_COUNT_MIN);
        else
            return file_st->max_ring_write_size;
    }

    if (file_st->max_ring_write_size == GFARRAY_MAX_FILESIZE_UNLIMIT)
        return (off64_t)(file_st->max_ring_write_size*GFARRAY_COUNT_MIN);

    return (off64_t)(file_st->max_ring_write_size*GFARRAY_COUNT_MAX);    
}

int gf_array_filedeletecancel() {
    g_bCanDeleteFileInGFile = false;
    return 1;
}


ENUM_GFILE_EDITRET gf_array_filedelete(const char *file_name, char *file_name_only, char *file_name_extension) {        
    int i, bProtected, ref_count;
    ENUM_GFILE_EDITRET ret;
    char full_name[MAX_PATH_LENGTH];
    
    g_bCanDeleteFileInGFile = true;
    
    printf("[gf_array_filedelete] file_name: %s, file_name_only: %s, file_name_extension: %s\n", file_name, file_name_only, file_name_extension);
    // Need to check if others are writing this files
    ref_count = isexist_gfdb(file_name);
    if (ref_count != -1) {
        if (gfdb_getwrite(ref_count) == 1) {
            printf("The targeted file is writing!!!!!\n");
            return GFILE_ERR_FILE_PROTECT;
        }
    }
            
    ret = GFILE_EDIT_SUCCESS;

    //Try file without _Rxx
    memset(full_name, 0, MAX_PATH_LENGTH);
    sprintf(full_name,"%s%s", file_name_only,file_name_extension);

    bProtected = 1;
    if (gfdb_isfileprotect(full_name, &bProtected) == 0) {
        if (!bProtected) {
            if (unlink(full_name) == 0) {
                return GFILE_EDIT_SUCCESS;
            }
        }
        else {
            //The file is protected
            return GFILE_ERR_FILE_PROTECT;
        }
    }

    // Try file with _Rxx
    // exam R00 is protected or not
    bProtected = 1;
    memset(full_name, 0, MAX_PATH_LENGTH);
    sprintf(full_name, "%s_R00%s", file_name_only, file_name_extension);
    if (gfdb_isfileprotect(full_name, &bProtected) == 0) {
        if (bProtected) {
            //The file is protected
            return GFILE_ERR_FILE_PROTECT;
        }
    }
    else {
        //The file does not exists
        return GFILE_ERR_FILE_NOT_EXIST;
    }    
    
    // for loop delete, from R99 -> R00
    for (i=(GFARRAY_COUNT_MAX-1); i>=0; i--) {
        memset(full_name, 0, MAX_PATH_LENGTH);
        sprintf(full_name, "%s_R%02d%s", file_name_only, i, file_name_extension);
        
        if (g_bCanDeleteFileInGFile == false) {
            printf("libfaccess: canceling file delete\n");
            break;
        }
            
        printf("unlink file %s....\n", full_name);    
        if (unlink(full_name) != 0) {
            if (i == 0)
                ret = GFILE_ERR_EDIT_ERROR;
        }
    }

    g_bCanDeleteFileInGFile = true;

    return ret;
}

ENUM_GFILE_EDITRET gf_array_filerename(const char *old_file_name, const char *new_file_name) {
    int bHasDot = 0, i, bProtected, ref_count;
    ENUM_GFILE_EDITRET ret;
    char old_file_name_only[MAX_PATH_LENGTH], old_file_name_extension[MAX_PATH_LENGTH], old_full_name[MAX_PATH_LENGTH];
    char new_file_name_only[MAX_PATH_LENGTH], new_file_name_extension[MAX_PATH_LENGTH], new_full_name[MAX_PATH_LENGTH];

    ret = GFILE_EDIT_SUCCESS;
    //@@fix me. May need to check if others are using this files

    // Need to check if others are writing this files
    ref_count = isexist_gfdb(old_file_name);
    if (ref_count != -1) {
        if (gfdb_getwrite(ref_count) == 1) {
            printf("The targeted file is writing!!!!!\n");
            return GFILE_ERR_FILE_PROTECT;
        }
    }    


    // parse for file_name and file_name_extension
    memset(old_file_name_only, 0, MAX_PATH_LENGTH);
    memset(old_file_name_extension, 0, MAX_PATH_LENGTH);
    memset(new_file_name_only, 0, MAX_PATH_LENGTH);
    memset(new_file_name_extension, 0, MAX_PATH_LENGTH);
    
    for (i=strlen(old_file_name); i>=0; i--) {
        if (old_file_name[i] == '.') {
            bHasDot = 1;
            sprintf(old_file_name_extension, "%s", &(old_file_name[i]));
            snprintf(old_file_name_only, i+1, "%s", old_file_name);
            break;
        }
    }

    bHasDot = 0;
    for (i=strlen(new_file_name); i>=0; i--) {
        if (new_file_name[i] == '.') {
            bHasDot = 1;
            sprintf(new_file_name_extension, "%s", &(new_file_name[i]));
            snprintf(new_file_name_only, i+1, "%s", new_file_name);
            break;
        }
    }

    //Try file without _Rxx
    memset(new_full_name, 0, MAX_PATH_LENGTH);
    memset(old_full_name, 0, MAX_PATH_LENGTH);
    sprintf(old_full_name,"%s%s", old_file_name_only,old_file_name_extension);
    sprintf(new_full_name,"%s%s", new_file_name_only,new_file_name_extension);

    bProtected = 1;
    if (gfdb_isfileprotect(old_full_name, &bProtected) == 0) {
        if (!bProtected) {
            int fd;
            
            if ((fd = open64(new_full_name, O_RDONLY)) != -1) {
                printf("target filename %s is already exist.\n", new_full_name);
                close(fd); 
                return GFILE_ERR_FILE_EXIST;        
            }
            
            if (rename(old_full_name, new_full_name) == 0) {
                return GFILE_EDIT_SUCCESS;
            }
        }
        else {
            //The file is protected
            return GFILE_ERR_FILE_PROTECT;
        }
    }

    // check if all the target file names are already exist.
    for (i = 0; i<GFARRAY_COUNT_MAX; i++) {
        int fd;
        memset(new_full_name, 0, MAX_PATH_LENGTH);
        sprintf(new_full_name, "%s_R%02d%s", new_file_name_only, i, new_file_name_extension);
        if ((fd = open64(new_full_name, O_RDONLY)) != -1) {
            close(fd); 
            memset(old_full_name, 0, MAX_PATH_LENGTH);
            sprintf(old_full_name, "%s_R%02d%s", old_file_name_only, i, old_file_name_extension);
            // only if old file name is exist, rename will fail....
            if ((fd = open64(old_full_name, O_RDONLY)) != -1) {
                close(fd);
                printf("target filename %s is already exist.\n", new_full_name);            
                return GFILE_ERR_FILE_EXIST;        
            }
        }
    }

    for (i = 0; i<GFARRAY_COUNT_MAX; i++) {
        memset(old_full_name, 0, MAX_PATH_LENGTH);
        sprintf(old_full_name, "%s_R%02d%s", old_file_name_only, i, old_file_name_extension);
        memset(new_full_name, 0, MAX_PATH_LENGTH);
        sprintf(new_full_name, "%s_R%02d%s", new_file_name_only, i, new_file_name_extension);

        if (i == 0) {
            bProtected = 1;
            if (gfdb_isfileprotect(old_full_name, &bProtected) == 0) {
                if (bProtected) {
                    //The file is protected  
                    return GFILE_ERR_FILE_PROTECT;
                }
            }
            else {
                //The file does not exists
                return GFILE_ERR_FILE_NOT_EXIST;
            }
        }

        if (rename(old_full_name, new_full_name) != 0) {
            if (i == 0)	            
	            ret = GFILE_EDIT_SUCCESS;
        }
    }

    return ret;    
}

ENUM_GFILE_EDITRET gf_array_fileprotect(const char *file_name, int on) {
    int bHasDot = 0, i;
    ENUM_GFILE_EDITRET ret;
    char file_name_only[MAX_PATH_LENGTH], file_name_extension[GFILE_EXTENSION_LENGTH], full_name[MAX_PATH_LENGTH];
    mode_t mode;

    //@@fix me. May need to check if others are using this files

    // parse for file_name and file_name_extension
    ret = GFILE_EDIT_SUCCESS;
    memset(file_name_only, 0, MAX_PATH_LENGTH);
    memset(file_name_extension, 0, GFILE_EXTENSION_LENGTH);
    
    for (i=strlen(file_name); i>=0; i--) {
        if (file_name[i] == '.') {
            bHasDot = 1;
            sprintf(file_name_extension, "%s", &(file_name[i]));
            snprintf(file_name_only, i+1, "%s", file_name);
            break;
        }
    }
    
    if (on)
        mode = S_IRUSR|S_IRGRP|S_IROTH;
    else 
        mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
        
    //Try file without _Rxx
    memset(full_name, 0, MAX_PATH_LENGTH);
    sprintf(full_name,"%s%s", file_name_only,file_name_extension);
    if (chmod(full_name, mode) == 0) {    
        return GFILE_EDIT_SUCCESS;
    }

    for (i = 0; i<GFARRAY_COUNT_MAX; i++) {
        memset(full_name, 0, MAX_PATH_LENGTH);
        sprintf(full_name, "%s_R%02d%s", file_name_only, i, file_name_extension);
            
        if (chmod(full_name, mode) != 0) {
            if (i == 0)
	            ret = GFILE_ERR_EDIT_ERROR;
        }
    }

    return ret;
}


void gf_array_getallrealfilename(char *in_filepath, char *in_filename, int *cnt, char filenameAry[][MAX_PATH_LENGTH]) {
    int bFlag, i, num, fd, length;    
    char file_path[MAX_PATH_LENGTH], tmp_path[MAX_PATH_LENGTH], f_ext[10];
    memset(file_path, 0, MAX_PATH_LENGTH);
    memset(f_ext, 0, 10);
     
    bFlag = 0; 
    length = strlen(in_filename);       
    for (i=length; i>=0; i--) {
        if (in_filename[i] == '.') {
            if ((length-i) > 10)
                break;            
            snprintf(file_path, i+1, "%s", in_filename);             
            sprintf(f_ext, "%s", &(in_filename[i]));            
            bFlag = 1;           
            break;
        }
    }    
    
    if (bFlag == 1) { // has extension name
        // scan all the possible file
        num = 0;
        for (i=0; i<GFARRAY_COUNT_MAX; i++) {
            memset(tmp_path, 0, MAX_PATH_LENGTH);
            sprintf(tmp_path, "%s%s_R%02d%s",  in_filepath, file_path, i, f_ext);

            if ((fd = open64(tmp_path, O_RDONLY)) == -1) {
                num = i;           
                break;
            }
            close(fd);
        }
        if (num > 0) {
            *cnt = num;
            for (i=0; i<= num; i++) {                
                memset(filenameAry[i], 0, MAX_PATH_LENGTH);                
                sprintf(filenameAry[i], "%s_R%02d%s",  file_path, i, f_ext);
            }
        }
        else {
            *cnt = 1;            
            sprintf(filenameAry[0], "%s", in_filename);
            return;
        }
    }
    else { // no extension name
        *cnt = 1;        
        sprintf(filenameAry[0], "%s", in_filename);
        return;
    }

}


// the global variables which gf_array_getfilename() needed
int g_gfileFileNum = -1;
char g_gfileFileName[MAX_PATH_LENGTH];
char g_gfileFileExtName[MAX_PATH_LENGTH];


/*
 *  AP input filepath and filename ex: "/mypath/" "test_R03.mpg"
 *  return parsed filename, ex: "test.mpg"
 *  @FIXME: maybe enhance in the future
 */
void gf_array_getfilename(char *in_filepath, char *in_filename, char *out_filename, int length, bool *isKeyFileName) {
    int bFlag, i, num, fd;
    char file_path[MAX_PATH_LENGTH], tmp_path[MAX_PATH_LENGTH], f_ext[10];

    bFlag = 0;
    memset(file_path, 0, MAX_PATH_LENGTH);
    memset(tmp_path, 0, MAX_PATH_LENGTH);
    memset(f_ext, 0, 10);
    *isKeyFileName = false;
    
/*
    // open first, check if file existed or not
    sprintf(tmp_path, "%s%s", in_filepath, in_filename);
    if ((fd = open64(tmp_path, O_RDONLY)) == -1) {
        printf("file %s not exist\n", tmp_path);
        *out_filename = 0;  
        return;        
    }
    close(fd); 
*/

    for (i=strlen(in_filename); i>=0; i--) {
        if (in_filename[i] == '.') {            
            snprintf(file_path, i+1, "%s", in_filename);             
            sprintf(f_ext, "%s", &(in_filename[i]));
            bFlag = 1;           
            break;
        }
    }    

    // if filename matchs rule: _Rxx.xxx
    // examination _R00~_Rxx existed or not
    // if there is one file doesn't exist, return full filename
    // if all files are exist, return fake filename
    if ((bFlag == 1) && 
        (((int)(file_path[strlen(file_path)-1]) >= 48) && ((int)(file_path[strlen(file_path)-1]) <= 57)) &&
        (((int)(file_path[strlen(file_path)-2]) >= 48) && ((int)(file_path[strlen(file_path)-2]) <= 57))  &&
        ((int)(file_path[strlen(file_path)-3]) == 'R') && 
        ((int)(file_path[strlen(file_path)-4]) == '_')) {
        
        num = (int)((file_path[strlen(file_path)-1]) - 48) + ((file_path[strlen(file_path)-2] - 48)*10);

        // check global variable to see if this file has already parsed
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        strncat(tmp_path, file_path, strlen(file_path)-2);

        // special case: all _R00 is key filename
        if (num == 0)
            *isKeyFileName = true; 
            
        if ((g_gfileFileNum != -1) &&
            (strncmp(tmp_path, g_gfileFileName, strlen(tmp_path)) == 0) &&
            (strncmp(f_ext, g_gfileFileExtName, strlen(f_ext)) == 0)) {            
            if (g_gfileFileNum >= num) 
                goto fake_return;       
            else {
                *isKeyFileName = true;
                goto normal_return;
            }
        }                

        // scan all the possible file
        for (i=0; i<GFARRAY_COUNT_MAX; i++) {
            memset(tmp_path, 0, MAX_PATH_LENGTH);
            sprintf(tmp_path, "%s", in_filepath);
            strncat(tmp_path, file_path, strlen(file_path)-2);
            sprintf(tmp_path, "%s%02d%s",  tmp_path, i, f_ext);
            if ((fd = open64(tmp_path, O_RDONLY)) == -1) 
                break;
            close(fd);
        }
        
        // save the information to global variables for future reusing
        g_gfileFileNum = (i-1);
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        strncat(tmp_path, file_path, strlen(file_path)-2);
        sprintf(g_gfileFileName, "%s", tmp_path);
        sprintf(g_gfileFileExtName, "%s", f_ext);
        
        // special case: means scan fail: R00 not exist 
        if (g_gfileFileNum == -1) {
            *isKeyFileName = true;
            goto normal_return;
        }        
            
        // if filename is _R00.xxx and only this file exist, check others is exist or not, 
        // if not exist and NTFS like filesystem, return fake filename        
        if ((num == 0) && (g_gfileFileNum == 0)) {
            struct statfs mystatfs;
            memset(&mystatfs, 0, sizeof(struct statfs));
            // Since in_path may only include part of path
            // so we use whole filename to call statfs
            memset(tmp_path, 0, MAX_PATH_LENGTH);
            sprintf(tmp_path, "%s%s", in_filepath, in_filename);
            statfs(tmp_path, &mystatfs);  
            *isKeyFileName = true; 
            
            if ((mystatfs.f_type == 0xEF53) ||      // EXT2/3
                (mystatfs.f_type == 0x5346544e) ||  // NTFS ?
                (mystatfs.f_type == 0x44534655) ||  // Paragon NTFS
                (mystatfs.f_type == 0x6969) ||      // NFS
                (mystatfs.f_type == 0x15013346) ||  // UDF_SUPER_MAGIC
                (strstr(in_filepath, VR_HDD_PATH))) {    // HDD's UDF2.0 partition  
                gfarray_t * tmpfd = NULL;
                printf("NTFS/NFS/EXT filesystem\n");
                // try to open fake filename to get if it is timeshifting
                memset(tmp_path, 0, MAX_PATH_LENGTH);
                sprintf(tmp_path, "%s", in_filepath);
                snprintf(tmp_path, 
                        ((strlen(file_path)-3) > length)? length+strlen(in_filepath): (strlen(file_path)-3+strlen(in_filepath)),
                        "%s%s", tmp_path, file_path);
                sprintf(tmp_path, "%s%s", tmp_path, f_ext);
                
                printf("[getfilename] --> try open %s\n", tmp_path);
                tmpfd = gf_array_open(tmp_path, O_RDONLY);
                if (tmpfd == NULL) {
                    printf("libfaccess: FATAL ERROR !!!!!!!\n");
                    //assert(0);
                }
                else {
                    // timeshifing retun fake filename
                    if (gfdb_getwrite(tmpfd->timeshift_ref) == 1) {
                        printf("libfaccess: file is writing\n");                        
                        gf_array_close(tmpfd);    
                        goto fake_return;
                    }
                    // not timeshifting return true filename
                    else {
                        printf("libfaccess: file is NOT writing\n");
                        gf_array_close(tmpfd); 
                        //goto normal_return;
                        goto fake_return;
                    }                    
                }
            }
            else {         
                // Other filesystem(FAT32)
                // Since GFileOpen by w, will open 10 files each time
                // so if only one file _R00.xxx exists
                // it must be not writing, we have to return true filename                
                goto normal_return;            
            }            
        }                            

        // pass the filename examination, return fake filename
        goto fake_return;
    }
    else
        *isKeyFileName = true;
    

normal_return:       
    // not pass filename examination, return real filename
    snprintf(out_filename, 
             ((strlen(in_filename)+1) > length)? length: strlen(in_filename)+1,
             "%s", in_filename);
    //printf("[ycyang] %s return !!! %s %d\n", in_filename, out_filename, *isKeyFileName);                          
    return;            

fake_return:
    // pass the filename examination, return fake filename
    snprintf(out_filename, 
             ((strlen(file_path)-3) > length)? length: (strlen(file_path)-3),
              "%s", file_path);
    sprintf(out_filename, "%s%s", out_filename, f_ext);
    //printf("[ycyang] %s return !!! %s %d\n", in_filename, out_filename, *isKeyFileName);                              
    return;
}

off64_t gf_array_getfilesetsize(char *in_filename, int *out_iswriting) {
    int sret;    
    struct stat mystat;
    sret = stat(in_filename, &mystat);

    // not find the file, try fileset's filename
    if (sret == -1) {

        int i, bFlag, db_count;
        off64_t total_size;
        char file_path[MAX_PATH_LENGTH], tmp_path[MAX_PATH_LENGTH], f_ext[GFILE_EXTENSION_LENGTH];

        bFlag = 0;
        memset(file_path, 0, MAX_PATH_LENGTH);
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        memset(f_ext, 0, GFILE_EXTENSION_LENGTH);
 

        for (i=strlen(in_filename); i>=0; i--) {
            if (in_filename[i] == '.') {            
                snprintf(file_path, i+1, "%s", in_filename);             
                sprintf(f_ext, "%s", &(in_filename[i]));
                bFlag = 1;           
                break;
            }
        }           

        // file not exist, and there is no Dot in filename
        if (bFlag == 0) {
            *out_iswriting = 0;
            return -1;
        }

        // see if it is already in db
        db_count = isexist_gfdb(in_filename);
        if (db_count != -1) { 
            // key exist in db            
            *out_iswriting = gfdb_getwrite(db_count);
            return gfdb_getvalue(db_count, -1); // get total size
        }

        // not exist in db, count by myself
        total_size = -1;
        *out_iswriting = 0;

        for (i = 0; i<GFARRAY_COUNT_MAX; i++) {
            memset(tmp_path, 0, MAX_PATH_LENGTH);
            sprintf(tmp_path, "%s_R%02d%s", file_path, i, f_ext);
            sret = stat(tmp_path, &mystat);
            if (sret == -1)
                return total_size;
            else
                total_size += mystat.st_size;
        }

    }

    // find the file, return the size.
    *out_iswriting = 0;
    return mystat.st_size;
}

// if user assign the cut_pos, then the writing function call after cut_pos will write to next file handle
// if cut_pos = -1, then cut from the seek_pos now.....
int gf_array_setforcewritetonextfile(gfarray_t *file_st, off64_t cut_pos) {
    int i;
    off64_t offset;
    
    if (!file_st)
        return -1;
        
    if ((!HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) ||
        (HAS_FLAG(file_st->file_mode, GFILE_FMODE_RINGBUF))) {
        printf("libfaccess: GFileSetWriteNextFile file mode error\n");
        return -1;
    }
    
    if (cut_pos == -1)
        offset = file_st->seek_pos;
    else
        offset = cut_pos;
    
    for (i=0; i<GFARRAY_COUNT_MAX; i++) {
        if (offset < file_st->max_eachfile_write_size[i]) {    // find correct file!
            file_st->max_eachfile_write_size[i] = offset;
            printf("libfaccess: set file handle %d's max_eachfile_write_size to %lld\n", i, offset);
            return 1;
        }
        else             
            offset -= file_st->title_sizes[i];        
    }
    
    return 0;
}


ENUM_GFILE_EDITRET gf_array_abcut(gfarray_t *file_st, int64_t pos_a, int64_t pos_b) {
    int tmpfd;
    char file_name[MAX_PATH_LENGTH];
    //In the unit of 2K
    uint32_t startSec, endSec;
    uint64_t abCutSector;

    if ((pos_a < 0) || (pos_b < 0) || (pos_b <= pos_a) ||
        ((pos_a%2048) != 0) || ((pos_b%2048) != 0) ||
        (pos_a > gfdb_gettotalfilesize(file_st->timeshift_ref)) ||
        (pos_b > gfdb_gettotalfilesize(file_st->timeshift_ref))) {
        printf("libfaccess: abcut fail! Incorrect input parameters!\n");
        return GFILE_ERR_EDIT_ERROR;
    }

    if (file_st->max_ring_write_size != GFARRAY_MAX_FILESIZE_UNLIMIT) {
        printf("libfaccess: abcut fail! Only UDF file system is supported!\n");
        return GFILE_ERR_EDIT_ERROR;
    }

    if (HAS_FLAG(file_st->file_mode, GFILE_FMODE_WRITE)) {
        printf("libfaccess: abcut fail! The source file must be opened as READ_ONLY!\n");
        return GFILE_ERR_EDIT_ERROR;
    }

    sprintf(file_name, "%s%s", file_st->file_name, file_st->file_name_extension);
    tmpfd = open(file_name, O_RDWR|O_DIRECT);
    if (!tmpfd) {
        printf("libfaccess: abcut fail! Can not re_opne source file with WRITE!\n");
        return GFILE_ERR_EDIT_ERROR;
    }

    if (file_st->title_devs[0].fd ) {
        close(file_st->title_devs[0].fd);
        file_st->title_devs[0].fd  = 0;
    }

    startSec = (uint32_t)(pos_a>>11);
    endSec = (uint32_t)(pos_b>>11);
    abCutSector = (uint64_t)(endSec - startSec);
    abCutSector = abCutSector<<32;
    abCutSector += startSec;

#ifndef UDF_ABCUT
#define UDF_ABCUT   _IOW('l', 0x44, long long)
#define UDF_ABCUT_INFO  _IO('l', 0x45)
#endif
	ioctl(tmpfd, UDF_ABCUT, &abCutSector);
	file_st->title_sizes[0] -= (pos_b - pos_a);
    
    close(tmpfd);
    file_st->title_devs[0].fd = open(file_name, O_RDONLY|O_DIRECT);
    if(!(file_st->title_devs[0].fd)) {
        file_st->title_devs[0].fd = open(file_name, O_RDONLY);
    }
    file_st->title_devs[0].pos = -1;

    gfdb_putvalue(file_st->timeshift_ref, file_st->title_sizes, 0, (file_st->file_count-1));

    return GFILE_EDIT_SUCCESS;     
}

/*
 * Use FastCopy library to copy date
 * @NOTE: srcFile is livepause filename, but remember write handle can't close in this time
 *        start/end position input virtual position for reading..
 */
ENUM_GFILE_EDITRET gf_array_abfastcopy(char *srcFileName, char *dstFileName, int64_t start, int64_t end) {
    printf("preparing doing FastCopy %s -> %s (%lld ~ %lld)\n", srcFileName, dstFileName, start, end);
    do_fastcopy(srcFileName, dstFileName, start, end);
    printf("FastCopy done\n");
    return GFILE_EDIT_SUCCESS;     
}
