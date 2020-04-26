#include "internal.h"
#include "file_db.h"
// #include <Util.h>
#include "dvd_path_define.h"
#include "OpenFile.h"

extern pthread_mutex_t g_iodevice_mutex[5];

gfsimple_t * gf_simple_open(const char *filename, int oflag) {
    int fd;
    gfsimple_t *simple_st;
    char resolve_name[PATH_MAX]; //MAX_PATH_LENGTH
    char *pReal;

    // open file
    fd = open64(filename, oflag);
    if (fd < 0) {
        printf("libfaccess: file %s open error\n", filename);
        return NULL;
    }

    // initialize internal data structure    
    simple_st = (gfsimple_t *)malloc(sizeof(gfsimple_t));
    if (!simple_st) {
        printf("libfaccess: malloc fail\n");
        return NULL;
    }

    memset(simple_st, 0, sizeof(gfsimple_t));
    sprintf(simple_st->file_name, "%s", filename);
    simple_st->file_pos = -1;
    simple_st->seek_pos = -1;
    simple_st->file_handle = fd;
    simple_st->file_size = lseek64(fd, 0, SEEK_END);
    lseek64(fd, 0, SEEK_SET);
    
    // prepare mutex
    memset(resolve_name, 0, sizeof(resolve_name));
    pReal = realpath(filename, resolve_name);

    if (strncmp(resolve_name, FILE_IODEVICE_USB1, FILE_IODEVICE_PATH_LENGTH) == 0)               
        simple_st->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB1]);            
    else if (strncmp(resolve_name, FILE_IODEVICE_USB2, FILE_IODEVICE_PATH_LENGTH) == 0) 
        simple_st->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB2]);                         
    else if (strncmp(resolve_name, FILE_IODEVICE_USB3, FILE_IODEVICE_PATH_LENGTH) == 0) 
        simple_st->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB3]);         
    else if (strncmp(resolve_name, FILE_IODEVICE_USB4, FILE_IODEVICE_PATH_LENGTH) == 0) 
        simple_st->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_USB4]);          
    else
        simple_st->pIO_mutex = &(g_iodevice_mutex[GFILE_IODEVICE_IDE]); 
        
    
    return simple_st;
}


int gf_simple_close(gfsimple_t *simple_st) {
    simple_st->pIO_mutex = NULL;
    if (close(simple_st->file_handle) < 0) 
        printf("libfaccess: close file error\n");
    
    free(simple_st);
    simple_st = NULL;    
    return 1;
}

ssize_t gf_simple_writebytes(gfsimple_t *simple_st, void *data, size_t nbyte) {

    // do write
    if (write(simple_st->file_handle, data, nbyte) != nbyte) {
        printf("libfaccess: file write %d bytes error\n", nbyte);              
        return -1;
    }
    
    simple_st->file_pos += nbyte;
    simple_st->file_size += nbyte;
    simple_st->seek_pos += nbyte;
    return nbyte;
}


ssize_t gf_simple_readbytes(gfsimple_t *simple_st, void *data, size_t nbyte) {
    int ret;
    
    // do read
    ret = read(simple_st->file_handle, data, nbyte);
    if (ret < 0) {
        printf("libfaccess: file pos %lld read %d bytes error\n", simple_st->seek_pos, nbyte);                    
        if ((errno == ENODEV) || (errno == EHOSTDOWN))
            return -4;
        else if (errno == EIO)
            return -5;
        else if ((errno == EBUSY) || (errno == EAGAIN))
            return -2;
        else 
            return -3;
    }

    if (ret != nbyte)
        printf("libfaccess: read bytes %d not enough %d, maybe reach file end\n", ret, nbyte);                                
        
    simple_st->seek_pos += ret;
    simple_st->file_pos += ret;    
    
    return ret;
}

off64_t gf_simple_seek(gfsimple_t *simple_st, off64_t bytes, int whence) {
    off64_t ret;
    // do seek    
    if ((ret = lseek64(simple_st->file_handle, bytes, whence)) != bytes) {
        printf("libfaccess: seek %lld error, ret: %lld, errno: %d\n", bytes, ret, errno);
        return -1;
    }
    
    if (whence == SEEK_SET) 
        simple_st->seek_pos = bytes;
    else if (whence == SEEK_CUR) 
        simple_st->seek_pos = simple_st->seek_pos + bytes;
    else if (whence == SEEK_END) 
        simple_st->seek_pos = simple_st->file_size + bytes;    
           
    return ret;       
}

off64_t gf_simple_getfilesize(gfsimple_t *simple_st) {
    return simple_st->file_size;
}

int64_t gf_simple_getfreespace(gfsimple_t *simple_st) {
    int i;
    int64_t totalFreeSize;
    struct statfs64 mystatfs;
    char full_path[MAX_PATH_LENGTH];
        
    for (i=strlen(simple_st->file_name); i>=0; i--) {
        if (simple_st->file_name[i] == '/') {
            memset(full_path, 0, MAX_PATH_LENGTH);
            snprintf(full_path, i+1, "%s", simple_st->file_name);
            break;
        }
    }  

//     if (statfs64(full_path, &mystatfs) < 0) {
//         printf("libfaccess: gf_simple_getfreespace call statfs error\n");
//         return -1;
//     }
    
    totalFreeSize = mystatfs.f_bfree * mystatfs.f_bsize;    
    return totalFreeSize;   
}

int gf_simple_lockresource(gfsimple_t *simple_st, int timeout) {
    if (simple_st->pIO_mutex) {
        if (timeout == -1) {                        
            pthread_mutex_lock(simple_st->pIO_mutex);            
        }
        else {
            return -1;
        }
    }
    return 1;
}

int gf_simple_unlockresource(gfsimple_t *simple_st) {
    if (simple_st->pIO_mutex) {                   
        pthread_mutex_unlock(simple_st->pIO_mutex);
    }
    return 1;
}
