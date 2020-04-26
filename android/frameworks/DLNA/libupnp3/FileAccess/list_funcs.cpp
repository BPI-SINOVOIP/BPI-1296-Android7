#include "internal.h"
#include "file_db.h"
// #include <Util.h>
#include "OpenFile.h"

#include <sys/ioctl.h>
#define EXT3_IOC_XFLAGS              _IOR('f', 10, long)
#define EXT3_RTL_PARET              0x00000001
#define EXT3_RTL_CHILD               0x00000002
#define EXT3_RTL_LINK                0x00000004


#define _SAFE_FREE(list_st)                                                       \
        {   if (list_st->file_handle)  close(list_st->file_handle);               \
            if (list_st->file_meta_handle)  close(list_st->file_meta_handle);     \
            if (list_st) free(list_st);                               }

static gflist_t * _init_gflist(const char *filename) {       
    int bHasDot = 0, i;    
    char full_path[MAX_PATH_LENGTH];
    gflist_t *list_st = NULL;

    if (!filename)
        return NULL;

    // create database if it has not been created.
    if (!gf_create_db()) {
        printf("gfile create db fail\n");
        return NULL;
    }

    // initialize some variables of gfile_t
    list_st = (gflist_t *)malloc(sizeof(gflist_t));
    if (!list_st) {
        printf("libfaccess: malloc fail\n");
        return NULL;
    }

    memset(list_st, 0, sizeof(gflist_t));

    // parse for file_name and file_name_extension
    sprintf(full_path, "%s", filename);
    for (i=strlen(full_path); i>=0; i--) {
        if (full_path[i] == '.') {
            bHasDot = i;
            sprintf(list_st->file_name_extension, "%s", &(full_path[i]));
            snprintf(list_st->file_name, i+1, "%s", full_path);            
            break;
        }
    }

    // if input filename doesn't have "."  (extension filename).
    if (!bHasDot) {
        printf("input filename has no '.' , automatically add '.ts'\n");
        sprintf(list_st->file_name, "%s", full_path);
        sprintf(list_st->file_name_extension, ".ts");        
    }

    return list_st;
}


static gflist_t * _init_gflist_db(gflist_t *list_st, const char *filename) {
    char full_path[MAX_PATH_LENGTH];
    
    // Database related:
    // if there is the same key in db, remember the pointer which is for file size update.
    // otherwise this is not timeshift case, create the db entry and remember the pointer
    if ((list_st->timeshift_ref = isexist_gfdb(list_st->file_name)) != -1) {
        addref_gfdb(list_st->timeshift_ref);
    }
    else {
        // Need to add error handling code to deal with the no space case in the database.
        if ((list_st->timeshift_ref = gfdb_addkey(list_st->file_name)) == -1) {
            printf("libfileaccess: file %s open error since there is no more space in the database!\n", filename);
            _SAFE_FREE(list_st);
            return NULL;
        }
    }

    // get info node in database
    list_st->file_info_now = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_HEAD);
    
    // initialize infolist node
    if (HAS_FLAG(list_st->file_type, GFILE_FMODE_WRITE)) {
        gfdb_putwrite(list_st->timeshift_ref, 1);
        gfdb_puttotalfilesize(list_st->timeshift_ref, 0);
        list_st->file_info_now->file_size = 0;
        list_st->file_info_now->filelist_idx = 0;
        list_st->file_info_now->start_addr = 0;
    }
    // READ
    else {    
        // timeshift case
        if (getref_gfdb(list_st->timeshift_ref) > 1) {         
            // Don't need to initialize all fields in info list,
            // since WRITE has already maintained it.
            printf("libfaccess: read CATCH timeshift !!!!\n");                        
            list_st->file_handle = -1;
            list_st->file_pos = -1;
        }
        // not timeshift case    
        else {
            // metadata file exist
            if (list_st->file_meta_handle > 0) {                                   
                int f_count, i, fd, final_opened;
                int64_t total_size;
                char *ptr = NULL;
                gfdb_info_list *node = NULL;
        
                ptr = &(full_path[0]);
        
                // put file count        
                f_count = gf_metafile_getfilecount(list_st->file_meta_handle);
                gfdb_putfilecount(list_st->timeshift_ref, f_count);
                                 
                total_size = 0;                                                                                
                final_opened = 0;                
                node = list_st->file_info_now;
                
                // for each file                
                for (i=0; i<f_count; i++) {                    
                    memset(full_path, 0, MAX_PATH_LENGTH);
                    // append one new node to info list                    
                    if ((i>0) && ((node = gfdb_info_append_node(list_st->timeshift_ref)) == NULL)) {
                        _SAFE_FREE(list_st);
                        return NULL;
                    }
                    // get filename from metadata
                    gf_metafile_getfilename(list_st->file_meta_handle, i+1, &ptr);
                    printf("name: %s  ", ptr);
                    fd = open64(full_path, list_st->file_open_mode);
                    if (fd > 0) {
                        int64_t tmp_size;
                        final_opened = final_opened + 1;
                        tmp_size = lseek64(fd, 0, SEEK_END);
                        gfdb_info_putfilesize(list_st->timeshift_ref, node, tmp_size);
                        gfdb_info_putstartaddr(list_st->timeshift_ref, node, total_size);
                        total_size += tmp_size;
 
                        // Since the filename in metadata file may not obey the naming rule
                        // I have to re-read metadata file when GFListReadBytes() need to change file handle,
                        // but metadata file can't always trust.
                        // Therefore I use file_index to indicate the actual position in medata file.
                        //    prefix 0xF: means this is metadata file position, not filname index
                        //       0xF0000001:  the first filename in metadata file
                        //       0xF000000A:  the 10th filename in metadata file
                        printf("idx: 0x%x\n", (0xF0000000 + (i+1)));
                        gfdb_info_putfileindex(list_st->timeshift_ref, node, (0xF0000000 + (i+1)));                    
                        close(fd);    
                    }                    
                }
                
                // update toal file size                
                gfdb_puttotalfilesize(list_st->timeshift_ref, total_size);
                
                // reassign total file handles
                if (f_count != final_opened) {
                    printf("libfaccess: file number %d in metadata not match actual num: %d\n", f_count, final_opened);
                    gfdb_putfilecount(list_st->timeshift_ref, final_opened);
                }    
                
                list_st->file_handle = -1;
                list_st->file_pos = -1;            
            }
            // Metadata file not exist, this is normal read case.
            // File handle has already opened in _open_list(),
            // don't need to close it now, since this is not timeshift case,
            // open file early will not cause some wrong situations anyway.
            else {
                int64_t file_size = 0;
                gfdb_putfilecount(list_st->timeshift_ref, 1);
                file_size = lseek64(list_st->file_handle, 0, SEEK_END);
                lseek64(list_st->file_handle, 0, SEEK_SET);
                gfdb_puttotalfilesize(list_st->timeshift_ref, file_size);
                list_st->file_info_now->file_size = file_size;
                list_st->file_info_now->filelist_idx = 0;                
                list_st->file_info_now->start_addr = 0;          
            }
        } // end of  not timeshift case
        
        // remember info_list_for_read
        gfdb_putinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_FOR_READ, list_st->file_info_now);
        
    } // end of READ
    
    return list_st;
}

// Used by editing function:
// Since editing function's input parameter is opened gfile_t *,
// may call ReadBytes() after editing...
// so I have to reset list nodes and some related fields.
static int _reset_list(gflist_t *list_st) {
    char *ptr;
    gfdb_info_list *node;
    int i, f_count, fd, final_opened, total_size;        
    char full_path[MAX_PATH_LENGTH];
    
    ptr = &full_path[0];
    
    // clear info node list
    gfdb_resetinfonode(list_st->timeshift_ref, 1);
    
    
    list_st->file_info_now = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_HEAD);
    if (list_st->file_info_now < 0) {
        printf("libfaccess: _reset_list() create node error\n");
        return -1;
    }    
    
    f_count = gf_metafile_getfilecount(list_st->file_meta_handle);    
    gfdb_putfilecount(list_st->timeshift_ref, f_count);
    node = list_st->file_info_now;
    
    total_size = 0;                                                                                
    final_opened = 0; 

    // for each file                
    for (i=0; i<f_count; i++) {                    
        memset(full_path, 0, MAX_PATH_LENGTH);
        // append one new node to info list                    
        if ((i>0) && ((node = gfdb_info_append_node(list_st->timeshift_ref)) == NULL)) {
            _SAFE_FREE(list_st);
            return -1;
        }
        
        // get filename from metadata
        gf_metafile_getfilename(list_st->file_meta_handle, i+1, &ptr);
        printf("name: %s  ", ptr);
        fd = open64(full_path, list_st->file_open_mode);
        if (fd > 0) {
            int64_t tmp_size;
            final_opened = final_opened + 1;
            tmp_size = lseek64(fd, 0, SEEK_END);
            gfdb_info_putfilesize(list_st->timeshift_ref, node, tmp_size);
            gfdb_info_putstartaddr(list_st->timeshift_ref, node, total_size);
            total_size += tmp_size;
 
            // Since the filename in metadata file may not obey the naming rule
            // I have to re-read metadata file when GFListReadBytes() need to change file handle,
            // but metadata file can't always trust.
            // Therefore I use file_index to indicate the actual position in medata file.
            //    prefix 0xF: means this is metadata file position, not filname index
            //       0xF0000001:  the first filename in metadata file
            //       0xF000000A:  the 10th filename in metadata file
            printf("idx: 0x%x\n", (0xF0000000 + (i+1)));
            gfdb_info_putfileindex(list_st->timeshift_ref, node, (0xF0000000 + (i+1)));                    
            close(fd);    
        }                    
    }
                
    // update toal file size                
    gfdb_puttotalfilesize(list_st->timeshift_ref, total_size);
                
    // reassign total file handles
    if (f_count != final_opened) {
        printf("libfaccess: file number %d in metadata not match actual num: %d\n", f_count, final_opened);
        gfdb_putfilecount(list_st->timeshift_ref, final_opened);
    }    
    
    list_st->file_handle = -1;
    list_st->file_pos = -1;    
    //@FIXME: Do I need to clean seek_pos ?
    //list_st->seek_pos = 0; 
    
    return 1;     
}



#if 0
static int _force_delete_file(gflist_t *list_st, int64_t ringsize) {
    int i, fileIndex, fd;
    struct stat buf;
    struct statfs64 mystatfs;
    char full_path[MAX_PATH_LENGTH], new_file[MAX_PATH_LENGTH];
    int64_t reuse_filesize, totalFreeSize;     
    
    //Check if the lock file exist.
    //If lock file exists, no need to do the force deleting
    memset(full_path, 0, MAX_PATH_LENGTH);                                 
    sprintf(full_path, "%s/%s", list_st->file_name, _GLIST_LOCK_FILE);     
    fd = open(full_path, O_RDONLY);                         

    if (fd >= 0) {
        printf("The directory %s is already clean!\n", list_st->file_name);
        close(fd);
        return 1;
    }

    reuse_filesize = 0; 
    memset(full_path, 0, MAX_PATH_LENGTH);
    
    // Test if directory exist
    printf("(1) test directory exist: %s\n", list_st->file_name);
    if (stat(list_st->file_name, &buf) == 0) {
        int bNeedDelete;
        DIR *mydir = NULL;
        unsigned long flags;
        struct dirent *entry = NULL;
        // delete all files in directory and directory itself
        
        printf("(2) open dir\n");
        mydir = opendir(list_st->file_name);
        if (mydir == NULL) {
            printf("libfaccess: open directory %s error\n", list_st->file_name);
            return -1;
        }
        
        // EXT3 filesystem have to decide delete file or not according to the file type
        // But Other filesystem only just delete file directly.
        if (HAS_FLAG(list_st->file_type, GFILE_FMODE_EXT3)) {
            printf("libfaccess: INFO: FileSystem is EXT3\n");
            fileIndex = 0;
            while((entry = readdir(mydir)) != NULL) {            
                if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
                    continue;
                
                bNeedDelete = 1;
                
                memset(full_path, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s/%s", list_st->file_name, entry->d_name);
                fd = open(full_path, O_RDONLY);
                if (fd < 0) {
                    printf("try to open file: %s fail, fd: %d, errno: %d\n", full_path, fd, errno);                    
                    assert(0);
                }
                if (ioctl(fd, EXT3_IOC_XFLAGS, &flags) < 0) {
                    printf("libfaccess: ioctl error, cannot get EXT3_IOC_XFLAGS\n");
                    close(fd);
                    return -1;
                }
                                    
                if (flags == 0) {
                    int64_t tmp_size;
                    printf("file %s is regular file\n", full_path);                            
                    bNeedDelete = 0;
                    tmp_size = lseek64(fd, 0, SEEK_END);
                    if (tmp_size != -1)
                        reuse_filesize += tmp_size;
                }
                if (flags & EXT3_RTL_PARET) 
                    printf("file %s is a AB copy parent\n", full_path);
                if (flags & EXT3_RTL_CHILD) 
                    printf("file %s is a AB copy child\n", full_path);
                if (flags & EXT3_RTL_LINK) 
                    printf("file %s is hard linked\n", full_path);                             
                
                close(fd);
                
                if (bNeedDelete) {
                    int64_t tmpPTS = pli_getPTS();                               
                    printf("delete file %s...", full_path);
                    if (unlink(full_path) < 0) {
                        printf("libfaccess: unlink file %s error\n", entry->d_name);
                    }
                    printf("pts: %lld\n", pli_getPTS()-tmpPTS);                
                } 
                else {
                    //Rename files, starting from R0000.ts
                    memset(new_file, 0, MAX_PATH_LENGTH);
                    sprintf(new_file, "%s/R%04d%s", list_st->file_name, fileIndex, list_st->file_name_extension);
                    printf("new file name is %s\n", new_file);
                    rename(full_path, new_file);
                }         
                fileIndex++;  
            } // end of while
        }        
        // Other Filesystem 
        else {
            printf("libfaccess: INFO: FileSystem is NOT EXT3\n");
            while((entry = readdir(mydir)) != NULL) {            
                if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
                    continue;
                memset(full_path, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s/%s", list_st->file_name, entry->d_name);
                printf("delete file %s...\n", full_path);
                if (unlink(full_path) < 0) {
                    printf("libfaccess: unlink file %s error\n", entry->d_name);
                }
            }
        }
        
        closedir(mydir);
        printf("rmdir directory %s\n", list_st->file_name);
        if (rmdir(list_st->file_name) < 0) 
            printf("libfaccess: rmdir %s not success, still has file to reuse\n", list_st->file_name);
    }
    
    // Test if metadata exist
    sprintf(full_path, "%s%s", list_st->file_name, METADATA_EXTFILENAME);
    if (stat(full_path, &buf) == 0) {
        // delete metadata file
        printf("delete metadata file %s\n", full_path);
        if (unlink(full_path) < 0) {
            printf("unlink metadata %s error\n", full_path);
            return -1;
        }
    }
        
    // Check if the filesystem still has enough space to do livepause    
    for (i=strlen(list_st->file_name); i>=0; i--) {
        if (list_st->file_name[i] == '/') {
            memset(full_path, 0, MAX_PATH_LENGTH);
            snprintf(full_path, i+1, "%s", list_st->file_name);
            break;
        }
    }       
    
    if (statfs64(full_path, &mystatfs) < 0) {
        printf("libfaccess: _force_delete_file call statfs error\n");
        return -1;
    }
    
    totalFreeSize = mystatfs.f_bfree * mystatfs.f_bsize;
    if (totalFreeSize < (ringsize - reuse_filesize)) {
        printf("libfaccess: Disc Free size %lld is too small to do livepause, ringsize: %lld, reuse_filesize: %lld\n", totalFreeSize, ringsize, reuse_filesize);
        return -1;
    }
    
    //Create the cleaned lock file so that no need to clean for the next time livepause
    memset(full_path, 0, MAX_PATH_LENGTH);                                 
    sprintf(full_path, "%s/%s", list_st->file_name, _GLIST_LOCK_FILE);     
    fd = open(full_path, O_CREAT | O_WRONLY );                         
    if (fd < 0) {
        printf("libfaccess: Can not create cleaned lock file for %s!\n", list_st->file_name);
    }
    else {
        close(fd);
    }

    return 1;
}
#endif

/*
 *  Accept Rule:
 *      AP always issue open "/usr/local/etc/hdd/dvdvr/title1.mpg"  !!!!!
 *        WRITE will create /usr/local/etc/hdd/dvdvr/title1/R0000.mpg ~ R0512.mpg
 *        READ will open /usr/local/etc/hdd/dvdvr/title1.metadata in normal case
 *             but if /usr/local/etc/hdd/dvdvr/title1.mpg exist, will open it first
 *      The keyname recorded in database is directory name:
 *           "/usr/local/etc/hdd/dvdvr/title1/"   (of course, tranfer from real_path)
 */
static gflist_t * _open_list(const char *filename, int oflag, int isRingType, int64_t ringsize) {
    int fd;
    gflist_t *list_st = NULL;
    char full_path[MAX_PATH_LENGTH];

    list_st = _init_gflist(filename);
    if (list_st == NULL)
        return NULL;    

    // Open by "W" mode
    if ((oflag & O_RDWR) || (oflag & O_WRONLY)) { 
        int i;
        struct statfs mystatfs;
                   
        SET_FLAG(list_st->file_type, GFILE_FMODE_WRITE);
        list_st->file_open_mode = oflag;
        
        // Database related:
        // if this file has already opened by w/r mode, can't be opened again
        // otherwise add this key to database.
        if (isexist_gfdb(list_st->file_name) != -1) {
            printf("libfaccess: this file %s has already opened by w mode, can't open again\n", filename);
            _SAFE_FREE(list_st);
            return NULL;
        }               
       
        // Get FileSystem type:
        // Since EXT3 has some extra functions can be used, others don't.        
        for (i=strlen(list_st->file_name); i>=0; i--) {
            if (filename[i] == '/') {
                memset(full_path, 0, MAX_PATH_LENGTH);
                snprintf(full_path, i+1, "%s", list_st->file_name);
                break;
            }
        }        
        memset(&mystatfs, 0, sizeof(struct statfs));
        statfs(full_path, &mystatfs);
        if (mystatfs.f_type == 0xEF53)  // is EXT2/3
            SET_FLAG(list_st->file_type, GFILE_FMODE_EXT3);                          
        
        // create subdirectory
        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s", list_st->file_name);
        printf("mkdir %s\n", full_path);
        if (mkdir(full_path, 0644) < 0)
            printf("mkdir error\n");

        // open metadata file handle only if it's normal WRITE
        if (!isRingType) {            
            memset(full_path, 0, MAX_PATH_LENGTH);
            sprintf(full_path, "%s%s", list_st->file_name, METADATA_EXTFILENAME);
        
            list_st->file_meta_handle = open64(full_path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
            if (list_st->file_meta_handle < 0) {            
                printf("libfaccess: file %s open error\n", full_path);
                _SAFE_FREE(list_st);                                        
                return NULL;
            }        
            // write metadata header
            gf_metafile_initialize(list_st->file_meta_handle);             
        }
        else 
            list_st->file_meta_handle = -1;
                
        list_st->file_handle = -1;
        list_st->file_pos = -1;    
        list_st->ifo_file_handle = -1;      
        
    }   
    // Open by "R" mode
    else {                       
        SET_FLAG(list_st->file_type, GFILE_FMODE_READ);
        list_st->file_open_mode = oflag;

        // RING_BUF:
        //      metadata file doesn't exist, 
        //      need to open file handle in db later in GFListReadBytes()
        // not RING_BUF:
        //      open priority:
        //         (1) input filename (both metadata and normal filename are OK)     
        //               /usr/local/etc/title1.meta
        //               /usr/local/etc/title1.mpg
        //         (2) open the metadata file in the same directory, 
        //             if input filename doesn't exist
        //               /usr/local/etc/title2.mpg not exist
        //               will try to open /usr/local/etc/title2.meta
        if (!isRingType) {
            // Try if input filename exist or not
            fd = open64(filename, oflag);

            if (fd < 0) {
                // try to open metadata under this directory
                memset(full_path, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s%s", list_st->file_name, METADATA_EXTFILENAME);
            
                fd = open64(full_path, O_RDWR);
             
                // metadata not exist
                if (fd < 0) {                
                    printf("libfaccess: metadata file %s not exist\n", full_path);
                    _SAFE_FREE(list_st);                                        
                    return NULL;
                }
                // metadata file exist
                else {
                    // check if metadata is valid
                    if (gf_metafile_checkvalid(fd) < 0) {
                        printf("libfacccess: metadata content is invalid, open fail\n");
                        _SAFE_FREE(list_st);
                        return NULL;
                    }
                                
                    list_st->file_meta_handle = fd;    
                    list_st->file_handle = -1;
                    list_st->file_pos = -1;            
                }
            }
            // input filename exist
            else {
                // see if this file is metadata
                if (strcmp(list_st->file_name_extension, METADATA_EXTFILENAME) == 0) {
                    // is metadata                
                    list_st->file_meta_handle = fd;
                    list_st->file_handle = -1;
                    list_st->file_pos = -1;
                }
                // input file is not metadata file
                else {                
                    list_st->file_handle = fd;
                    list_st->file_meta_handle = -1;
                    list_st->file_pos = -1;
                }
            }                        
        } // end of if(!isRingType)
        
        // try open ifo file handle
        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s/%s", list_st->file_name, IFOFILE_NAME);
        
        //@HACK: Since our gfile not support Open "W" a exist writed file, 
        //       some editing features have to opened by "R",
        //       but IFO file need to edit... so we opened IFO file to O_RDWR here
        fd = open64(full_path, O_RDWR); 
        if (fd < 0) {
            printf("libfaccess: IFO file does not exist\n");
            list_st->ifo_file_handle = -1;            
        }
        else {
            printf("libfaccess: IFO file exist\n");
            list_st->ifo_file_handle = fd;           
        }
        
    } // end of Open by "R" mode
                    
    return _init_gflist_db(list_st, filename);    
}



gflist_t * gf_list_open(const char *filename, int oflag) {
    printf("[GFListOpen] filename: %s\n", filename);    
    return _open_list(filename, oflag, 0, 0);     
}


// Will check disc's free space automatically
// If free space is not enough, return NULL
gflist_t * gf_list_openring(const char *filename, int oflag, int64_t buf_size) {
    int64_t new_buf_size;
    gflist_t *list_st = NULL;

    // re-count new ringsize to multiple of MAX_GLIST_SIZE
    if ((buf_size%MAX_GLIST_SIZE) != 0) 
        new_buf_size = (((buf_size/MAX_GLIST_SIZE) + 1) * MAX_GLIST_SIZE);            
    else
        new_buf_size = buf_size;        
     
    list_st = _open_list(filename, oflag, 1, new_buf_size); 
    if (list_st == NULL)
        return NULL;
          
    SET_FLAG(list_st->file_type, GFILE_FMODE_RINGBUF);
    
    // if buf_size not multiple of MAX_GLIST_SIZE
    // automatically increase it
    //@HACK: only write mode set ring buffer size
    if (HAS_FLAG(list_st->file_type, GFILE_FMODE_WRITE)) 
        gfdb_putRINGfilesize(list_st->timeshift_ref, new_buf_size);   
        
    printf("GFListOpenRING, ring_size: %lld\n", new_buf_size);
        
    return list_st;
}


int gf_list_close(gflist_t *list_st) {
    int bDeleteFile = 0;
    char full_path[MAX_PATH_LENGTH];
    
    if (!list_st)
        return false;

    if (HAS_FLAG(list_st->file_type, GFILE_FMODE_WRITE))
        gfdb_putwrite(list_st->timeshift_ref, 0);
    
    if (list_st->file_handle > 0) {
        if (close(list_st->file_handle) < 0)
            printf("libfaccess: file handle close error\n");
    } 
    
    if (list_st->file_meta_handle > 0) {
        // If there is no any file in metadata file.
        if (gf_metafile_getfilecount(list_st->file_meta_handle) == 0)
            bDeleteFile = 1;
        if (close(list_st->file_meta_handle) < 0) 
            printf("libfaccess: metadata file handle close error\n");
    }

    if (list_st->ifo_file_handle > 0) {
        if (close(list_st->ifo_file_handle) < 0) 
            printf("libfaccess: ifo file handle close error\n");        
        //@HACK for GFileMerge:
        // since Navigation may using IFO file after calling GFileMerge()
        // but only IFO file and directory are existed after GFileMerge()
        // so I have to delete the directory and ifo file here.
        // (1) open for READ
        // (2) not livepause
        // (3) ifo file exist and open
        // (4) meta data not exist
        if ((HAS_FLAG(list_st->file_type, GFILE_FMODE_READ)) &&
            (!HAS_FLAG(list_st->file_type, GFILE_FMODE_RINGBUF))) {
            int tmpfd;
            memset(full_path, 0, MAX_PATH_LENGTH);
            sprintf(full_path, "%s%s", list_st->file_name, METADATA_EXTFILENAME);
            // test if meta data exist
            if ((tmpfd = open(full_path, O_RDONLY)) > 0) {
                close(tmpfd);                
            }
            else {
                printf("calling GFileClose() after GFileMerge()....\n");
            
                memset(full_path, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s/%s", list_st->file_name, IFOFILE_NAME);
                printf("delete IFO file %s !!!\n", full_path);
                if (unlink(full_path) < 0) 
                    printf("delete ifo file %s error\n", full_path);
                printf("delete directory %s !!\n", list_st->file_name);
                if (rmdir(list_st->file_name) < 0)
                    printf("delete directory %s error\n", list_st->file_name);
            }            
        }
    }
    
    if ((HAS_FLAG(list_st->file_type, GFILE_FMODE_WRITE)) &&
        (gf_info_getfilesize(list_st->file_info_now) == 0) &&
        (bDeleteFile == 1)) {                
        printf("[GFileClose] no file write success, delete all files and directory\n"); 
        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s", list_st->file_name);
        printf("delete directory %s\n", full_path);
        if (rmdir(full_path) < 0)
            printf("libfaccess: delete directory %s fail\n", full_path);
                
        memset(full_path, 0, MAX_PATH_LENGTH);        
        sprintf(full_path, "%s%s", list_st->file_name, METADATA_EXTFILENAME);
        printf("delete metadata file %s\n", full_path);
        if (unlink(full_path) < 0)
            printf("libfaccess: delete file %s fail\n", full_path);
    }
    
    // Database related
    gfdb_deletekey(list_st->timeshift_ref);  

    _SAFE_FREE(list_st);
    list_st = NULL;    
    return 1;
}

int64_t gf_list_getfreespace(gflist_t *file_st) {
    int64_t totalFreeSize;
    struct statfs64 mystatfs;
    
//     if (statfs64(file_st->file_name, &mystatfs) < 0) {
//         printf("libfaccess: _force_delete_file call statfs error\n");
//         return -1;
//     }
    
    totalFreeSize = mystatfs.f_bfree * mystatfs.f_bsize;    
    return totalFreeSize;
}

static void _delete_reference_file(char *full_path) {
    int fd;
    
    fd = open(full_path, O_RDONLY);
    if (fd > 0) {
        unsigned long flags;
        int bNeedDelete = 1;
        
        if (ioctl(fd, EXT3_IOC_XFLAGS, &flags) < 0) {
            printf("libfaccess: ioctl get EXT3_IOC_XFLAGS error\n");            
            assert(0);
        }
        if (flags == 0) {
            printf("file %s is regular file !!\n", full_path);                                                
            bNeedDelete = 0;
        }
        if (flags & EXT3_RTL_PARET) 
            printf("file %s is a AB copy parent\n", full_path);  
        if (flags & EXT3_RTL_CHILD) 
            printf("file %s is a AB copy child\n", full_path);  
        if (flags & EXT3_RTL_LINK)
            printf("file %s is hard linked, delete it!\n", full_path); 
        if (bNeedDelete) {
//             int64_t tmpPTS = pli_getPTS();
            printf("delete file %s.....", full_path);
            if (unlink(full_path) < 0) 
                printf("libfaccess: unlink %s fail ERROR !!!!!!!\n", full_path);                    
//             printf("pts: %lld\n", pli_getPTS()-tmpPTS);
        }
                    
        close(fd);                
    }
}

ssize_t gf_list_writebytes(gflist_t *list_st, void *data, size_t nbyte) {
    off64_t offset;    
    int64_t oldsize;
    int fd, f_count;
    gfdb_info_list * newnode;
    char full_path[MAX_PATH_LENGTH];    
                      
    // The first GFListWriteBytes() after GFlistOpen()
    // have to create the first file handle
    // ps: although file handle not create, but we have set the first nod in info list
    if ((list_st->file_handle == -1) &&         
        (gfdb_gettotalfilesize(list_st->timeshift_ref) == 0)) {
        // open new file handle
        memset(full_path, 0, MAX_PATH_LENGTH);
              
        sprintf(full_path, "%s/R%04d%s", list_st->file_name, gf_info_getidx(list_st->file_info_now), list_st->file_name_extension);
        // check if this file is ABCopy parent, otherwise don't need to delete        
        _delete_reference_file(full_path);
        
        // open new file handle
        printf("create file %s\n", full_path);
        fd = open64(full_path, list_st->file_open_mode, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
        if (fd < 0) {
            printf("libfaccess: file %s open error\n", full_path);
            return -1;
        }

        list_st->file_handle = fd;
        list_st->file_pos = -1;
        gfdb_putfilecount(list_st->timeshift_ref, 1); 
        printf("f_count: 1, file_meta_handle: %d\n", list_st->file_meta_handle);
        // refresh metadata file  
        if (list_st->file_meta_handle != -1) {
            printf("refresh file_meta_handle\n");
            gf_metafile_putfilecount(list_st->file_meta_handle, 1);
            gf_metafile_putfilename(list_st->file_meta_handle, 1, full_path);
        }        
    }
    
    // See if need to change file handle and info node list
    if ((list_st->seek_pos + nbyte) > (gf_info_getstartaddr(list_st->file_info_now) + MAX_GLIST_SIZE)) {        
                
        // change file handle and info node list //
        
        // append new node 
        if ((newnode = gfdb_info_append_node(list_st->timeshift_ref)) == NULL)
            printf("libfaccess: append node error");
        
        newnode->start_addr = (list_st->file_info_now->start_addr + MAX_GLIST_SIZE);
        
        // If not write 2 part case, fill old file_handle to max size, close file handle then
        if (list_st->seek_pos >= (gf_info_getstartaddr(list_st->file_info_now) + MAX_GLIST_SIZE)) {            
            
            if (list_st->file_pos < MAX_GLIST_SIZE) 
                lseek64(list_st->file_handle, MAX_GLIST_SIZE, SEEK_SET);           
            if (close(list_st->file_handle) < 0)
                printf("libfaccess: close fd error\n");

            list_st->file_pos = -1;        
            
            memset(full_path, 0, MAX_PATH_LENGTH);                                                                      
            sprintf(full_path, "%s/R%04d%s", list_st->file_name, gf_info_getidx(newnode), list_st->file_name_extension);
            // check if need to delete new file before open
            _delete_reference_file(full_path);
            // open new file handle                        
            printf( "create file %s\n", full_path);
            fd = open64(full_path, list_st->file_open_mode, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
            if (fd < 0) {
                printf("libfaccess: file %s open error\n", full_path);
                return -1;
            }

            list_st->file_handle = fd;
            list_st->file_info_now = newnode;
            f_count = gfdb_getfilecount(list_st->timeshift_ref)+1;
            gfdb_putfilecount(list_st->timeshift_ref, f_count); 

            // refresh metadata file  
            printf("f_count: %d, file_meta_handle: %d\n", f_count, list_st->file_meta_handle);
            if (list_st->file_meta_handle != -1) {
                printf("refresh metadata file\n");
                gf_metafile_putfilecount(list_st->file_meta_handle, f_count);
                gf_metafile_putfilename(list_st->file_meta_handle, f_count, full_path);
            }                    
        }                
    }
    
    offset = list_st->seek_pos - gf_info_getstartaddr(list_st->file_info_now);
     
    // seek   
    if (list_st->file_pos != offset) {
        //printf("seek to %lld\n", offset);
        if (lseek64(list_st->file_handle, offset, SEEK_SET) != offset) {
            printf("libfaccess: file %d seek %lld error\n", list_st->file_handle, offset);                        
            return -1;
        }
        list_st->file_pos = offset;
    }     
    
    // only need to write once
    if ((offset + nbyte) <= MAX_GLIST_SIZE) {
#ifdef GIO_DEBUG        
        int64_t tmpPTS = pli_getPTS();                        
#endif        
        PDEBUG("write %d bytes, seek_pos: %lld, offset: %lld.....", nbyte, list_st->seek_pos, offset);
        if (write(list_st->file_handle, data, nbyte) != nbyte) {
            printf("libfaccess: file write %d bytes error\n", nbyte);              
            return -1;
        }
        PDEBUG("pts: %lld\n", pli_getPTS()-tmpPTS);
        list_st->file_pos += nbyte;
        gfdb_info_putfilesize(list_st->timeshift_ref, list_st->file_info_now, list_st->file_pos);
        if ((oldsize = gfdb_gettotalfilesize(list_st->timeshift_ref)) <= list_st->seek_pos) {
            //printf("    oldtotalsize: %lld, file_pos: %lld\n", oldsize, list_st->file_pos);
            gfdb_puttotalfilesize(list_st->timeshift_ref, (oldsize+nbyte));
        }
        list_st->seek_pos += nbyte;
    }
    // need to write two part
    else {
        int first_part = MAX_GLIST_SIZE - offset;
        
        printf("WRITE 2 Part !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        // write first part        
        printf("write %d bytes, seek_pos: %lld\n", first_part, list_st->seek_pos);
        if (write(list_st->file_handle, data, first_part) != first_part) {
            printf("libfaccess: file write %d bytes error\n", first_part);              
            return -1;
        }
        list_st->file_pos += first_part;
        gfdb_info_putfilesize(list_st->timeshift_ref, list_st->file_info_now, list_st->file_pos);        
        if ((oldsize = gfdb_gettotalfilesize(list_st->timeshift_ref)) <= list_st->seek_pos) {
            printf("    oldtotalsize: %lld, file_pos: %lld\n", oldsize, list_st->file_pos);
            gfdb_puttotalfilesize(list_st->timeshift_ref, (oldsize+first_part));
            printf("    newtotalsize: %lld\n", oldsize+first_part);
        }        
        list_st->seek_pos += first_part;
        
        // change file handle
        if (close(list_st->file_handle) < 0)
            printf("libfaccess: close fd error\n");
            
        list_st->file_pos = -1;        
                        
        memset(full_path, 0, MAX_PATH_LENGTH);
        newnode = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_END);
        sprintf(full_path, "%s/R%04d%s", list_st->file_name, gf_info_getidx(newnode), list_st->file_name_extension);
        // check if need to delete new file before open
        _delete_reference_file(full_path);        
        printf("create file %s\n", full_path);
        fd = open64(full_path, list_st->file_open_mode, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
        if (fd < 0) {
            printf("libfaccess: file %s open error\n", full_path);
            return -1;
        }
        
        list_st->file_handle = fd;
        list_st->file_info_now = newnode;
        
        f_count = gfdb_getfilecount(list_st->timeshift_ref)+1;
        gfdb_putfilecount(list_st->timeshift_ref, f_count); 
        
        // refresh metadata file  
        printf("f_count: %d, file_meta_handle: %d\n", f_count, list_st->file_meta_handle);
        if (list_st->file_meta_handle != -1) {
            printf("refresh metadata file\n");
            gf_metafile_putfilecount(list_st->file_meta_handle, f_count);
            gf_metafile_putfilename(list_st->file_meta_handle, f_count, full_path);
        }   
        
        // seek
        printf("seek to 0\n");
        if (lseek64(list_st->file_handle, 0, SEEK_SET) != 0) {
            printf("libfaccess: file seek 0 error\n");                        
            return -1;
        }
        list_st->file_pos = 0;
        
        // write second part
        printf("write %d bytes, seek_pos: %lld\n", (nbyte-first_part), list_st->seek_pos);
        if (write(list_st->file_handle, ((char *)data+first_part), (nbyte-first_part)) != (nbyte-first_part)) {
            printf("libfaccess: file write %d bytes error\n", (nbyte-first_part));              
            return -1;
        }
        list_st->file_pos += (nbyte-first_part);
        gfdb_info_putfilesize(list_st->timeshift_ref, list_st->file_info_now, list_st->file_pos);        
        if ((oldsize = gfdb_gettotalfilesize(list_st->timeshift_ref)) <= list_st->seek_pos) {
            printf("    oldtotalsize: %lld, file_pos: %lld\n", oldsize, list_st->file_pos);
            gfdb_puttotalfilesize(list_st->timeshift_ref, (oldsize+nbyte-first_part));
            printf("new totalfilesize: %lld\n", oldsize+nbyte-first_part);
        }
        list_st->seek_pos += (nbyte - first_part);                       
    } // end of write two parts

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
ssize_t gf_list_readbytes(gflist_t *list_st, void *data, size_t nbyte) {
    off64_t offset;      
    int bResetNode = 0;    
    gfdb_info_list *tmpnode;
    int fd, ret, f_count;    
    char full_path[MAX_PATH_LENGTH];


    // (1) Check if seek_pos is valid
    //      we have to check it no matter need to reset node or not !!!!
    
    // RING type check start address is in ring_size's range
    if (HAS_FLAG(list_st->file_type, GFILE_FMODE_RINGBUF)) {
        if (list_st->seek_pos < (gfdb_gettotalfilesize(list_st->timeshift_ref) - gfdb_getRINGfilesize(list_st->timeshift_ref))) {
            printf("libfaccess: READ fail, seek pos %lld too small than ring's start_addr: %lld\n", 
                     list_st->seek_pos, (gfdb_gettotalfilesize(list_st->timeshift_ref) - gfdb_getRINGfilesize(list_st->timeshift_ref)));
            return -6;
        }        
    }
    else {
        // seek_pos < Listfile's start pos ?    
        tmpnode = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_HEAD);
        if (list_st->seek_pos < (gf_info_getstartaddr(tmpnode))) {
            printf("libfaccess: READ fail, seek pos %lld too small, %lld\n", list_st->seek_pos, gf_info_getstartaddr(tmpnode));
            return -6;
        }        
    }
        
    // seek_pos > Listfile's end pos ?
    tmpnode = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_END);
    if ((list_st->seek_pos + nbyte) > (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode))) {
        printf("libfaccess: READ fail, seek pos %lld too large\n", list_st->seek_pos);
        if (gfdb_getwrite(list_st->timeshift_ref) == 1) {
            //Read reaches write and write does not stop
            return -7;
        }
        else {
            //Read reaches write and write does stop      
            return -1;
        }
    }
            

    // (2) see if need to reset file_info_now
    if (gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_FOR_READ) == NULL) {
        // info_list_for_read == NULL, need to reset node
        bResetNode = 1;
    }
    else {
        // First time calling GFListReadBytes(), and GFListOpen() didn't open file handle
        // Of course, file_info_now and file_info_for_read have to exist !!!
        if (list_st->file_handle == -1) {
            memset(full_path, 0, MAX_PATH_LENGTH);      
            
            // get filename from metadata file
            printf("index result: 0x%x\n", gf_info_getidx(list_st->file_info_now));
            if (gf_info_getidx(list_st->file_info_now) & 0xF0000000) {
                char *ptr = &(full_path[0]);
                
                if (list_st->file_meta_handle == -1) {
                    printf("libfaccess: ERROR state metadata file doesn't exist.\n");
                    return -3;
                }
                
                f_count = (gf_info_getidx(list_st->file_info_now) & 0x0FFFFFFF);
                gf_metafile_getfilename(list_st->file_meta_handle, f_count, &ptr);                
            }
            // get filename from database node index
            else {                                
                sprintf(full_path, "%s/R%04d%s", list_st->file_name, gf_info_getidx(list_st->file_info_now), list_st->file_name_extension);                
            }
                    
            printf("open file %s\n", full_path);
            fd = open64(full_path, list_st->file_open_mode);
            if (fd < 0) {
                printf("libfaccess: file %s open error\n", full_path);
                return -1;
            }
            list_st->file_handle = fd;
        } // end of if (list_st->file_handle == -1)
        
        //printf("compare: seek_pos: %lld, start_addr: %lld, file_size: %lld\n", list_st->seek_pos, gf_info_getstartaddr(list_st->file_info_now), gf_info_getfilesize(list_st->file_info_now));
        
        if ((list_st->seek_pos < gf_info_getstartaddr(list_st->file_info_now)) ||
            (list_st->seek_pos >= (gf_info_getstartaddr(list_st->file_info_now) + gf_info_getfilesize(list_st->file_info_now))))
            bResetNode = 1;
            
    } // end of GFDB_GETINFOLIST_FOR_READ != NULL
           
               
    // (3) Do ResetNode if needed
    if (bResetNode) {
        int bSearchForward = 0, bSearchOK = 0;        
        // Check if seek_pos is out of ring_size
        
        printf("I am ResetNode !!!!!!!!!!!!!!!!!!!!!!\n");
                
        // Since the info list may be very large...
        // decide the sequence of searching node is forward or backward
        if (gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_FOR_READ) == NULL)
            tmpnode = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_HEAD);
        else
            tmpnode = list_st->file_info_now;
            
        if (list_st->seek_pos >= gf_info_getstartaddr(tmpnode))
            bSearchForward = 1;
        else    
            bSearchForward = 0;
        
        printf("bSearchForward: %d\n", bSearchForward);
        
        // do search node
        while (tmpnode) {
            printf("in while loop, seek_pos: %lld, start_addr: %lld, file_size: %lld\n", list_st->seek_pos, gf_info_getstartaddr(tmpnode), gf_info_getfilesize(tmpnode));
            if ((list_st->seek_pos >= gf_info_getstartaddr(tmpnode)) && 
                (list_st->seek_pos < (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {                
                // find it !
                
                // close old file handle
                printf("close file handle\n");
                if (close(list_st->file_handle) < 0)
                    printf("libfaccess: close fd error\n");
                list_st->file_pos = -1;
                        
                // open new file handle
                memset(full_path, 0, MAX_PATH_LENGTH);
                
                // get filename from metadata file
                if (gf_info_getidx(tmpnode) & 0xF0000000) {
                    char *ptr = &(full_path[0]);
                
                    if (list_st->file_meta_handle == -1) {
                        printf("libfaccess: ERROR state metadata file doesn't exist.\n");
                        return -3;
                    }
                
                    f_count = (gf_info_getidx(tmpnode) & 0x0FFFFFFF);
                    
                    gf_metafile_getfilename(list_st->file_meta_handle, f_count, &ptr);                
                    printf("f_count: %d, ptr: %s\n", f_count, ptr);
                }
                // get filename from database node index
                else {                
                    sprintf(full_path, "%s/R%04d%s", list_st->file_name, gf_info_getidx(tmpnode), list_st->file_name_extension);                    
                }
                
                printf("open file %s\n", full_path);
                fd = open64(full_path, list_st->file_open_mode);
                if (fd < 0) {
                    printf("libfaccess: file %s open error\n", full_path);
                    return -1;
                }
        
                // update some fields
                list_st->file_info_now = tmpnode;
                list_st->file_handle = fd;
                gfdb_putinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_FOR_READ, tmpnode);
                
                bSearchOK = 1;
                break;
            } // end of if
            
            if (bSearchForward)
                tmpnode = tmpnode->next;
            else
                tmpnode = tmpnode->prev;            
        } // end of  while (tmpnode)                

        if (bSearchOK == 0) {
            printf("libfaccess: FATAL ERROR... :(\n");
            assert(0);
        }
            
    } // end of if (bResetNode)
    
    
    offset = list_st->seek_pos - gf_info_getstartaddr(list_st->file_info_now);
    
    // do seek
    if (list_st->file_pos != offset) {
        if (lseek64(list_st->file_handle, offset, SEEK_SET) != offset) {                    
            printf("libfaccess: file seek %lld error\n", offset);
            // Other error                        
            return -99;
        }
        list_st->file_pos = offset;
    }    
    
    // do read
    
    // may need to read many times... (write bytes exceed this file, and next file exist)
    if (((offset + nbyte) > gf_info_getfilesize(list_st->file_info_now)) &&
        (list_st->file_info_now->next)) {
        int remain_bytes = nbyte;
        
        while(remain_bytes) {
            int this_part;
            
            if (remain_bytes > (gf_info_getfilesize(list_st->file_info_now) - list_st->file_pos))
                this_part = (gf_info_getfilesize(list_st->file_info_now) - list_st->file_pos);
            else
                this_part = remain_bytes;
                
            // read this part
            printf("     -> read %d bytes, nbyte: %d, remain_bytes: %d, file_st->pos: %lld\n", this_part, nbyte, remain_bytes, list_st->file_pos); 
            ret = read(list_st->file_handle, ((char *)data + nbyte - remain_bytes), this_part);
            if (ret < 0) {
                printf( "libfileaccess: file pos %lld read %d bytes error\n", offset, this_part);
                if ((errno == ENODEV) || (errno == EHOSTDOWN))
                    return -4;
                else if(errno == EIO)
                    return -5;
                else if ((errno == EBUSY) || (errno == EAGAIN))
                    return -2;
                else 
                    return -3;
            }              
                
            if (ret != this_part)
                printf("libfaccess: read bytes %d not enouth %d, maybe reach file end\n", ret, this_part);
            
            list_st->seek_pos += ret;
            list_st->file_pos += ret;
            remain_bytes -= ret;
            
            if (remain_bytes == 0)
                break;
                
            // still not finish.... 
            
            // refresh file handle and some fields
            if (!list_st->file_info_now->next) {
                printf("libfaccess: no next file, maybe reach file end, ret: %d\n", ret);
                return ret;
            }            
            printf("close fd\n");
            if (close(list_st->file_handle) < 0) 
                printf("libfaccess: close fd error\n");                    
        
            tmpnode = list_st->file_info_now->next;
            memset(full_path, 0, MAX_PATH_LENGTH);            
            
            // get filename from metadata file
            if (gf_info_getidx(tmpnode) & 0xF0000000) {
                char *ptr = &(full_path[0]);
            
                if (list_st->file_meta_handle == -1) {
                    printf("libfaccess: ERROR state metadata file doesn't exist.\n");
                    return -3;
                }
            
                f_count = (gf_info_getidx(tmpnode) & 0x0FFFFFFF);
                gf_metafile_getfilename(list_st->file_meta_handle, f_count, &ptr);                
            }
            // get filename from database node index
            else {                
                sprintf(full_path, "%s/R%04d%s", list_st->file_name, gf_info_getidx(tmpnode), list_st->file_name_extension);                
            }           
            
            printf("open file %s\n", full_path);
            fd = open64(full_path, list_st->file_open_mode);
            if (fd < 0) {
                printf("libfaccess: file %s open error\n", full_path);
                return -1;
            }            
            
            list_st->file_handle = fd;
            list_st->file_pos = -1;
            list_st->file_info_now = tmpnode;
            gfdb_putinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_FOR_READ, tmpnode);   
             
            // seek new file
            if (lseek64(list_st->file_handle, 0, SEEK_SET) != 0) {                    
                printf("libfaccess: file seek %lld error\n", offset);
                // Other error                        
                return -99;
            }
            list_st->file_pos = 0; 
        } // end of while         
        
        ret = nbyte - remain_bytes;                
    }
    // only need to read once
    else {
#ifdef GIO_DEBUG
        int64_t tmpPTS = pli_getPTS();
#endif        
        PDEBUG("    read from seek: %lld, pos: %lld, nbyte: %d....", list_st->seek_pos, list_st->file_pos, nbyte);
        ret = read(list_st->file_handle, data, nbyte);
        PDEBUG("pts: %lld\n", pli_getPTS()-tmpPTS);
        if (ret < 0) {
            printf("libfileaccess: file pos %lld read %d bytes error\n", offset, nbyte);                    
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
        
        list_st->seek_pos += ret;
        list_st->file_pos += ret;        
    }
        
    return ret;
}


/*
 * !NOTICE: 
 *   not really do seek operation now, but can be changed if someone needed in the future.
 */
off64_t gf_list_seek(gflist_t *list_st, off64_t bytes, int whence) { 
    if ((list_st == NULL) || (bytes < 0))
        return -1;
        
    if (whence == SEEK_SET) 
        list_st->seek_pos = bytes;
    else if (whence == SEEK_CUR) 
        list_st->seek_pos = list_st->seek_pos + bytes;
    else if (whence == SEEK_END) 
        list_st->seek_pos = gf_list_getfilesize(list_st) + bytes;    

    return list_st->seek_pos;
}


bool gf_list_testfileexist(char *file_name_only, char *file_name_extension) {
    struct stat tmpstat;    
    
    // Test If directory is exist
    if (stat(file_name_only, &tmpstat) == 0)
        return true;
        
    return false;    
}


off64_t gf_list_getfilesize(gflist_t *list_st) {
    if (list_st == NULL)
        return -1;
   
    return gfdb_gettotalfilesize(list_st->timeshift_ref);    
}


off64_t gf_list_getfilesetsize(char *in_filename, int *out_iswriting) {
    int sret;   
    DIR *mydir = NULL; 
    struct stat mystat;
    struct dirent *entry = NULL;
    sret = stat(in_filename, &mystat);

    // not find the file, try fileset's filename
    if (sret == -1) {

        int i, bFlag, db_count;
        off64_t total_size;
        char file_path[MAX_PATH_LENGTH], tmp_path[MAX_PATH_LENGTH], f_ext[10];

        bFlag = 0;
        memset(file_path, 0, MAX_PATH_LENGTH);
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        memset(f_ext, 0, 10);
 

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
        db_count = isexist_gfdb(file_path);
        if (db_count != -1) { 
            // key exist in db            
            *out_iswriting = gfdb_getwrite(db_count);
            return gfdb_gettotalfilesize(db_count); // get total size
        }

        // not exist in db, count by myself
        total_size = 0;
        *out_iswriting = 0;

        mydir = opendir(file_path);
        if (mydir == NULL) {
            printf("libfaccess: open directory %s error\n", file_path);
            return -1;
        }
        
        while((entry = readdir(mydir)) != NULL) {            
            if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
                continue;                                
                
            memset(tmp_path, 0, MAX_PATH_LENGTH);
            sprintf(tmp_path, "%s/%s", file_path, entry->d_name);            
            sret = stat(tmp_path, &mystat);            
            if (sret == -1)
                return total_size;
            else
                total_size += mystat.st_size;
        }
        
        return total_size;
    }
    else {
        // find the file, return the size.
        *out_iswriting = 0;
        return mystat.st_size;        
    }        
}


ssize_t gf_list_ifo_writebytes(gflist_t *list_st, void *data, size_t nbyte) {

    if ((list_st->ifo_file_handle == -1) && (HAS_FLAG(list_st->file_type, GFILE_FMODE_WRITE))) {
        char full_path[MAX_PATH_LENGTH];
                
        memset(full_path, 0, MAX_PATH_LENGTH);
        sprintf(full_path, "%s/%s", list_st->file_name, IFOFILE_NAME);
        list_st->ifo_file_handle = open64(full_path,  O_CREAT | O_RDWR);
        if (list_st->ifo_file_handle < 0) {
            printf("libfaccess: IFO file %s create error!\n", full_path);
            return -1;
        }        
    }   

    if (write(list_st->ifo_file_handle, data, nbyte) != nbyte) {
        printf("libfaccess, write %d bytes error, errno: %d\n", nbyte, errno);
        return -1;
    }
    
    return nbyte;
}

ssize_t gf_list_ifo_readbytes(gflist_t *list_st, void *data, size_t nbyte) {
    if (list_st->ifo_file_handle == -1)
        return -1;
      
    return read(list_st->ifo_file_handle, data, nbyte);    
}

off64_t gf_list_ifo_seek(gflist_t *list_st, off64_t bytes, int whence) {
    // create ifo file if first time write.
    if (list_st->ifo_file_handle == -1) {
        if (HAS_FLAG(list_st->file_type, GFILE_FMODE_WRITE)) {
            char full_path[MAX_PATH_LENGTH];
                
            memset(full_path, 0, MAX_PATH_LENGTH);
            sprintf(full_path, "%s/%s", list_st->file_name, IFOFILE_NAME);
            list_st->ifo_file_handle = open64(full_path,  O_CREAT | O_RDWR);
            if (list_st->ifo_file_handle < 0) {
                printf("libfaccess: IFO file %s create error!\n", full_path);
                return -1;
            }
        }
        else
            return -1;        
    }   
        
    return lseek64(list_st->ifo_file_handle, bytes, whence);
}


// purpose: set ifo file's size to zero
int gf_list_ifo_reset(gflist_t *list_st) {        
    if (list_st->ifo_file_handle == -1)
        return -1;
        
    if (ftruncate(list_st->ifo_file_handle, 0) < 0)
        printf("libfaccess: gf_list_ifo_reset ftruncate file error, errno: %d\n", errno);
    else {
        printf("libfaccess: ftruncate success\n");    
        lseek64(list_st->ifo_file_handle, 0, SEEK_SET);
    }
    
    return 1;
}



//@FIXME:
//   (1) Can AP make sure the copy unit is multiple of 4K ??? (O_DIRECT....)
//   (2) The responsibility of setting thread priority
#if 1
static int _copy_gflist_file(int src_fd, gflist_t *list_dst, int64_t start, int64_t end) {    
    off_t s_start;
    int dst_f_count, fd, ret;
    char dst_path[MAX_PATH_LENGTH];
    gfdb_info_list *newnode = NULL;
    

    memset(dst_path, 0, MAX_PATH_LENGTH);

    // the first write after GFileOpen()
    // NOTICE: GFileOpen() has already initialize one info node (but not file handle)
    // so there is no need to append a new node here
    // (1) decide append new node or not
    if ((gf_info_getidx(list_dst->file_info_now) == 0) &&
        (gf_info_getfilesize(list_dst->file_info_now) == 0)) {
        dst_f_count = 0;        
    }
    else {    
        if ((newnode = gfdb_info_append_node(list_dst->timeshift_ref)) == NULL)
            printf("libfaccess: append node error");

        dst_f_count = gf_info_getidx(newnode);
        newnode->start_addr = gfdb_gettotalfilesize(list_dst->timeshift_ref);
        list_dst->file_info_now = newnode;        
    }    

    sprintf(dst_path, "%s/R%04d%s", list_dst->file_name, dst_f_count, list_dst->file_name_extension);
        
    // (2) create new file 
    printf("create dst file: %s\n", dst_path);   
    fd = open64(dst_path, list_dst->file_open_mode, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
    if (fd < 0) {
        printf("libfaccess: file %s open error\n", dst_path);
        return -1;
    }

    printf("prepare to sendfile:  %d ~ %lld\n", (int)start, end);
    // (3) using sendfile to copy data (have to be 2KB align)
    s_start = (off_t)start;
    if ((ret = sendfile(fd, src_fd, &s_start, (end-start))) != (end-start)) {    
        printf("libfaccess: _copy_gflist_file sendfile() fail, ret: %d !!!\n", ret);
    }
    
    close(fd);

    // (4) set dst node's file size and file count
    gfdb_info_putfilesize(list_dst->timeshift_ref, list_dst->file_info_now, (end-start));
    gfdb_puttotalfilesize(list_dst->timeshift_ref, list_dst->file_info_now->start_addr + (end - start));
    gfdb_putfilecount(list_dst->timeshift_ref, (dst_f_count+1));

    // (5) update dst file's meta data
    if (list_dst->file_meta_handle != -1) {
        gf_metafile_putfilecount(list_dst->file_meta_handle, (dst_f_count+1));
        gf_metafile_putfilename(list_dst->file_meta_handle, (dst_f_count+1), dst_path);
    }

    return 1;
}

static int _move_gflist_file(gflist_t *list_src, gfdb_info_list *srcnode, char *src_path, gflist_t *list_dst) {
    int dst_f_count;
    char dst_path[MAX_PATH_LENGTH];
    gfdb_info_list *newnode;

    memset(dst_path, 0, MAX_PATH_LENGTH);
    
    // src and dst filename decision
    if ((gf_info_getidx(list_dst->file_info_now) == 0) &&
        (gf_info_getfilesize(list_dst->file_info_now) == 0)) {
        dst_f_count = 0;    
    }
    else {    
        if ((newnode = gfdb_info_append_node(list_dst->timeshift_ref)) == NULL)
            printf("libfaccess: append node error");

        dst_f_count = gf_info_getidx(newnode);
        newnode->start_addr = (list_dst->file_info_now->start_addr + MAX_GLIST_SIZE);
        list_dst->file_info_now = newnode;
    }
    
    sprintf(dst_path, "%s/R%04d%s", list_dst->file_name, dst_f_count, list_dst->file_name_extension);    

    gfdb_lockresource(list_src->timeshift_ref, -1); // GLockResource
    
    // (1) Create hard link
    printf("link: %s -> %s\n", dst_path, src_path);
    if (link(src_path, dst_path) == -1)
        printf("libfaccess: link %s to %s fail\n", dst_path, src_path);

    // (2) set dst node's file size and file count
    gfdb_info_putfilesize(list_dst->timeshift_ref, list_dst->file_info_now, MAX_GLIST_SIZE);    
    gfdb_puttotalfilesize(list_dst->timeshift_ref, (dst_f_count+1)*MAX_GLIST_SIZE);    
    gfdb_putfilecount(list_dst->timeshift_ref, (dst_f_count+1));

    // (3) update dst file's meta data
    if (list_dst->file_meta_handle != -1) {
        gf_metafile_putfilecount(list_dst->file_meta_handle, (dst_f_count+1));
        gf_metafile_putfilename(list_dst->file_meta_handle, (dst_f_count+1), dst_path);
    }

    gfdb_unlockresource(list_src->timeshift_ref); // GUnlockResource

    return 1;
}


ENUM_GFILE_EDITRET gf_list_abcopy(gflist_t *list_src, int64_t start, int64_t end, gflist_t *list_dst) {    
    int bSet, src_fd, oldPriority;    
    struct statfs64 mystatfs;
//     int64_t tmpPTS
    int64_t totalFreeSize;
    gfdb_info_list *tmpnode, *startnode, *endnode;
    char src_path[MAX_PATH_LENGTH];        
    
//     tmpPTS = pli_getPTS();
    printf("START gf_list_abcopy..... start: %lld, end: %lld\n", start, end);
        
    // check if start and end pos is valid
    if ((start >= end) || (start < 0) || (end < 0)) {
        printf("libfaccess: GFileABCopy position error, start: %lld, end: %lld\n", start, end);
        return GFILE_ERR_EDIT_ERROR;
    }

    if ((start%2048) || (end%2048) || ((end-start)%2048)) {
        printf("libfaccess: GFileABCopy position error, not 2KB's multiple\n");
        return GFILE_ERR_EDIT_ERROR;
    }

    // check if free space is enough or not
    // FreeSpace = partition's free space - (ring_size - used_size)
//     if (statfs64(list_src->file_name, &mystatfs) < 0) {
//         printf("libfaccess: _force_delete_file call statfs error\n");
//         return GFILE_ERR_EDIT_ERROR;
//     }
    
    totalFreeSize = mystatfs.f_bfree * mystatfs.f_bsize;
    if ((end-start) > (totalFreeSize - gfdb_getRINGfilesize(list_src->timeshift_ref) + gfdb_gettotalfilesize(list_src->timeshift_ref))) {
        printf("libfaccess: GFileABCopy destination size %lld > disc free space %lld\n", (end-start), (totalFreeSize - gfdb_getRINGfilesize(list_src->timeshift_ref) + gfdb_gettotalfilesize(list_src->timeshift_ref)));
        printf("totalFreeSize: %lld, ring_size: %lld, ring_size_now: %lld\n", totalFreeSize, gfdb_getRINGfilesize(list_src->timeshift_ref), gfdb_gettotalfilesize(list_src->timeshift_ref));
        return GFILE_ERR_NO_SPACE;
    }

    tmpnode = gfdb_getinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_END);
    if (end > (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode))) {     
        printf("libfaccess: GFileABCopy end position error, end: %lld, node_start: %lld, node_filesize: %lld\n", end, gf_info_getstartaddr(tmpnode), gf_info_getfilesize(tmpnode));
        return GFILE_ERR_EDIT_ERROR;    
    }
            
    tmpnode = gfdb_getinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_HEAD);
    if (start < (gf_info_getstartaddr(tmpnode))) {       
        printf("libfaccess: GFileABCopy start position error, start: %lld, node_start: %lld\n", start, gf_info_getstartaddr(tmpnode));
        return GFILE_ERR_EDIT_ERROR;    
    }
    
    // locate the start and end info node
    bSet = 0;
    while (tmpnode) {
        if ((start >= gf_info_getstartaddr(tmpnode)) &&
            (start < (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            startnode = tmpnode;
            bSet++;
        }
        if ((end >= gf_info_getstartaddr(tmpnode)) &&
            (end <= (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            endnode = tmpnode;
            bSet++;
            break;
        }
        tmpnode = tmpnode->next;
    }
    
    if (bSet < 2) {
        printf("libfaccess: can't find start and end node\n");
        return GFILE_ERR_EDIT_ERROR;
    }
    
    // lower the priority
    oldPriority = getpriority(PRIO_PROCESS, 0);
    setpriority(PRIO_PROCESS, 0, 10);
    
    // process each node (for each file)
    tmpnode = startnode;
    while(tmpnode) {                               
        
        memset(src_path, 0, MAX_PATH_LENGTH);  
        
        if (gf_info_getidx(tmpnode) & 0xF0000000) {
            int f_count;
            char *ptr = &(src_path[0]);
                
            if (list_src->file_meta_handle == -1) {
                printf("libfaccess: metadata file doesn't exist.\n");
                return GFILE_ERR_EDIT_ERROR;
            }
                
            f_count = (gf_info_getidx(tmpnode) & 0x0FFFFFFF);
                    
            gf_metafile_getfilename(list_src->file_meta_handle, f_count, &ptr);                        
        }
        // get filename from database node index
        else {                
            sprintf(src_path, "%s/R%04d%s", list_src->file_name, gf_info_getidx(tmpnode), list_src->file_name_extension);                    
        }
                      
                
        // start node need to do file copy
        if ((tmpnode == startnode) &&
            (start > gf_info_getstartaddr(startnode))) {
            printf("open source file %s\n", src_path);            
            src_fd = open(src_path, O_RDONLY | O_DIRECT);
            if (src_fd < 0) {
                printf("libfaccess: GFileABCopy open src fd error\n");
                return GFILE_ERR_EDIT_ERROR;            
            }
            
            if (tmpnode == endnode) {
                // copy part of this file
                printf("COPY case 1\n");
                _copy_gflist_file(src_fd, list_dst, (start - gf_info_getstartaddr(startnode)), (end - gf_info_getstartaddr(startnode)));
            }
            else {
                // copy until the end of this file
                printf("COPY case 2\n");
                printf("startadr: %lld, filesize: %lld\n", gf_info_getstartaddr(startnode), gf_info_getfilesize(tmpnode));                                
                _copy_gflist_file(src_fd, list_dst, (start - gf_info_getstartaddr(startnode)), gf_info_getfilesize(tmpnode));
            }
                        
            if (close(src_fd) < 0) 
                printf("libfaccess: GFileABCopy close src_fd error\n");            
            
            // if start node and end node are the same
            if (tmpnode == endnode)
                break;
        }
        
        // end node need to do file copy also if not copy all node
        else if ((tmpnode == endnode) &&
                 (end < (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
                
            printf("open source file %s\n", src_path);
            src_fd = open(src_path, O_RDONLY | O_DIRECT);
            if (src_fd < 0) {
                printf("libfaccess: GFileABCopy open src fd error\n");
                return GFILE_ERR_EDIT_ERROR;            
            }
            
            _copy_gflist_file(src_fd, list_dst, 0, (end - gf_info_getstartaddr(tmpnode)));
            
            if (close(src_fd) < 0) 
                printf("libfaccess: GFileABCopy close src_fd error\n");            
                
            break;            
        }
        
        // Remain cases just need to do file rename and move to new directory~
        else {
            _move_gflist_file(list_src, tmpnode, &(src_path[0]), list_dst);
        
            if (tmpnode == endnode)
                break;
        }
            
        tmpnode = tmpnode->next;
    }
    
    // get back the priority
    setpriority(PRIO_PROCESS, 0, oldPriority);
    
//     printf("END gf_list_abcopy, pts: %lld !!!\n", pli_getPTS()-tmpPTS);    
    
    return GFILE_EDIT_SUCCESS;    
}


#else



static int _copy_gflist_file(int src_fd, gflist_t *list_dst, int64_t start, int64_t end) {
    int i, ret, bLock, total, remain;  
    int64_t tmpPTS;  
    unsigned char *ptr = NULL;   
    BYTE *ptrNonCached = NULL;

    ptr = pli_allocContinuousMemory(GFLIST_COPY_UNIT, &ptrNonCached, NULL);
    if (ptr == NULL) {
        printf("libfaccess: gflist_copy_file alloc memory fail\n");
        return -1;
    }

    memset(ptrNonCached, 0, GFLIST_COPY_UNIT);
    
    total = ((end - start)/GFLIST_COPY_UNIT);
    remain = ((end - start)%GFLIST_COPY_UNIT);
    printf("total: %d, remain: %d\n", total, remain);
    for(i=0; i<total; i++) {        
        
        if ((i%5) == 0) {
            PDEBUG("[gflist_copy_file] GListLockResource...\n");
            gfdb_lockresource(list_dst->timeshift_ref, -1); // GLockResource
            bLock = 1;
        }
        
        if (lseek64(src_fd, ((i*65536)+start), SEEK_SET) != ((i*65536)+start)) {
            printf("gflist_copy_file seek error\n");
            return -1;
        }
        PDEBUG("----------------------> read 65536 bytes from pos: %lld....", ((i*65536)+start));
        tmpPTS = pli_getPTS();
        if ((ret = read(src_fd, ptrNonCached, 65536)) != 65536) {
            printf("gflist_copy_file read error, ret: %d, errno: %d\n", ret, errno);
            return -1;
        }
        PDEBUG("pts: %lld\n", pli_getPTS()-tmpPTS);
        
        if (gf_list_writebytes(list_dst, ptrNonCached, 65536) != 65536) {
            printf("gflist_copy_file write error\n");
            return -1;
        }
        
        if ((i%5) == 4) {
            PDEBUG("[gflist_copy_file] GListUnlockResource\n");
            gfdb_unlockresource(list_dst->timeshift_ref); 
            bLock = 0;
            usleep(100000); // 600 ms
        }
    }
    
    if (bLock == 1) {
        PDEBUG("[gflist_copy_file] GListUnlockResource\n");
        gfdb_unlockresource(list_dst->timeshift_ref); // GUnlockResource
    }
        
    if (remain != 0) {
        gfdb_lockresource(list_dst->timeshift_ref, -1); // GLockResource
        
        if (lseek64(src_fd, (end-remain), SEEK_SET) != (end-remain)) {
            printf("gflist_copy_file seek error\n");
            return -1;
        }
        PDEBUG("----------------------> read %d bytes from pos: %lld....", remain, (end-remain));
        tmpPTS = pli_getPTS();
        if ((ret = read(src_fd, ptrNonCached, remain)) != remain) {
            printf("gflist_copy_file read error, ret: %d, errno: %d\n", ret, errno);
            return -1;
        }
        PDEBUG("pts: %lld\n", pli_getPTS()-tmpPTS);
                
        if (gf_list_writebytes(list_dst, ptrNonCached, remain) != remain) {
            printf("[gflist_copy_file write error\n");
            return -1;
        }
                
        gfdb_unlockresource(list_dst->timeshift_ref); // GUnlockResource
    }    
        
    pli_freeContinuousMemory(ptr);
    
    return 1;
}


static int _move_gflist_file(gflist_t *list_src, gfdb_info_list *srcnode, char *src_path, gflist_t *list_dst) {
    int dst_f_count;
    char dst_path[MAX_PATH_LENGTH];
    gfdb_info_list *newnode;
    
    // src and dst filename decision    
    memset(dst_path, 0, MAX_PATH_LENGTH);
    if ((newnode = gfdb_info_append_node(list_dst->timeshift_ref)) == NULL)
        printf("libfaccess: append node error");
        
    dst_f_count = gf_info_getidx(newnode);            
    newnode->start_addr = (list_dst->file_info_now->start_addr + MAX_GLIST_SIZE);
    
    sprintf(dst_path, "%s/R%04d%s", list_dst->file_name, dst_f_count, list_dst->file_name_extension);      
    
    list_dst->file_info_now = newnode;
    
    gfdb_lockresource(list_src->timeshift_ref, -1); // GLockResource
    
    // (1) rename
    printf("rename: %s -> %s\n", src_path, dst_path);
    if (rename(src_path, dst_path) == -1)
        printf("libfaccess: rename %s to %s fail\n", src_path, dst_path);
        
    // (2) crete a hard link
    printf("link: %s -> %s\n", dst_path, src_path);
    if (link(dst_path, src_path) == -1)
        printf("libfaccess: link %s to %s fail\n", dst_path, src_path);
        
    // (3) set src node's info_list_for_read to NULL if READ is using it
    if (srcnode == gfdb_getinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_FOR_READ)) 
        gfdb_putinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_FOR_READ, NULL);
        
    // (4) set dst node's file size and file count
    gfdb_puttotalfilesize(list_dst->timeshift_ref, (dst_f_count+1)*MAX_GLIST_SIZE);
    gfdb_putfilecount(list_dst->timeshift_ref, (dst_f_count+1));
        
    // (5) update dst file's meta data
    if (list_dst->file_meta_handle != -1) {
        gf_metafile_putfilecount(list_dst->file_meta_handle, (dst_f_count+1));
        gf_metafile_putfilename(list_dst->file_meta_handle, (dst_f_count+1), dst_path);        
    }
    
    gfdb_unlockresource(list_src->timeshift_ref); // GUnlockResource
    
    return 1;
}



int gf_list_abcopy(gflist_t *list_src, int64_t start, int64_t end, gflist_t *list_dst) {
    int64_t tmpPTS;
    int bSet, src_fd;
    gfdb_info_list *tmpnode, *startnode, *endnode;
    char src_path[MAX_PATH_LENGTH];        
    
    tmpPTS = pli_getPTS();
    printf("START gf_list_abcopy..... start: %lld, end: %lld\n", start, end);
        
    // check if start and end pos is valid
    if ((start >= end) || (start < 0) || (end < 0)) {
        printf("libfaccess: GFListCopy position error, start: %lld, end: %lld\n", start, end);
        return -1;
    }

    if ((start%2048) || (end%2048) || ((end-start)%2048)) {
        printf("libfaccess: GFListCopy position error, not 2KB's multiple\n");
        return -1;
    }

    tmpnode = gfdb_getinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_END);
    if (end > (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode))) {     
        printf("libfaccess: GFListCopy end position error\n");
        return -1;    
    }
            
    tmpnode = gfdb_getinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_HEAD);
    if (start < (gf_info_getstartaddr(tmpnode))) {       
        printf("libfaccess: GFListCopy start position error\n");
        return -1;    
    }
    
    // locate the start and end info node
    bSet = 0;
    while (tmpnode) {
        if ((start >= gf_info_getstartaddr(tmpnode)) &&
            (start < (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            startnode = tmpnode;
            bSet++;
        }
        if ((end >= gf_info_getstartaddr(tmpnode)) &&
            (end <= (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            endnode = tmpnode;
            bSet++;
            break;
        }
        tmpnode = tmpnode->next;
    }
    
    if (bSet < 2) {
        printf("libfaccess: can't find start and end node\n");
        return -1;
    }
    
    // process each node (for each file)
    tmpnode = startnode;
    while(tmpnode) {               
        
        setpriority(PRIO_PROCESS, 0, 10);
        
        memset(src_path, 0, MAX_PATH_LENGTH);
        
        sprintf(src_path, "%s/R%04d%s", list_src->file_name, gf_info_getidx(tmpnode), list_src->file_name_extension);

        PDEBUG("[GFlistCopy] process, start: %lld, end: %lld, start_addr: %lld, file_size: %lld\n", start, end, gf_info_getstartaddr(tmpnode), gf_info_getfilesize(tmpnode));       
        
        // start node need to do file copy
        if ((tmpnode == startnode) &&
            (start > gf_info_getstartaddr(startnode))) {
            printf("open file %s\n", src_path);            
            src_fd = open(src_path, O_RDONLY | O_DIRECT);
            if (src_fd < 0) {
                printf("libfaccess: GFListCopy open src fd error\n");
                return -1;            
            }
            
            if (tmpnode == endnode) {
                // copy part of this file
                _copy_gflist_file(src_fd, list_dst, start, end);
            }
            else {
                // copy until the end of this file
                _copy_gflist_file(src_fd, list_dst, start, (gf_info_getstartaddr(startnode) + gf_info_getfilesize(tmpnode)));
            }
            
            gfdb_puttotalfilesize(list_dst->timeshift_ref, MAX_GLIST_SIZE); // must be 1st file
            if (close(src_fd) < 0) 
                printf("libfaccess: GFListCopy close src_fd error\n");            
            
            // if start node and end node are the same
            if (tmpnode == endnode)
                break;
        }
        
        // end node need to do file copy also if not copy all node
        else if ((tmpnode == endnode) &&
            (end < (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            printf("open file %s\n", src_path);
            src_fd = open(src_path, O_RDONLY | O_DIRECT);
            if (src_fd < 0) {
                printf("libfaccess: GFListCopy open src fd error\n");
                return -1;            
            }
            gf_list_seek(list_dst, 0, SEEK_END);
            _copy_gflist_file(src_fd, list_dst, 0, (end - gf_info_getstartaddr(tmpnode)));
            
            if (close(src_fd) < 0) 
                printf("libfaccess: GFListCopy close src_fd error\n");            
                
            break;            
        }
        
        // Remain cases just need to do file rename and move to new directory~
        else {
            _move_gflist_file(list_src, tmpnode, &(src_path[0]), list_dst);
        
            if (tmpnode == endnode)
                break;
        }
            
        tmpnode = tmpnode->next;
    }
    
    printf("END gf_list_abcopy, pts: %lld !!!\n", pli_getPTS()-tmpPTS);    
    
    return 1;    
}

#endif


ENUM_GFILE_EDITRET gf_list_filedelete(const char *file_name, char *file_name_only, char *file_name_extension) {
    int i, ref_count, meta_fd, f_count, bProtected;
    char full_name[MAX_PATH_LENGTH], *ptr;   
    
    ptr = &(full_name[0]);

    // check if others is using
    ref_count = isexist_gfdb(file_name_only);
    if (ref_count != -1) {
        if (gfdb_getwrite(ref_count) == 1) {
            printf("The targeted file is writing!!!!!\n");
            return GFILE_ERR_FILE_PROTECT;
        }
    }
      
    // check if protected      
    if (gfdb_isfileprotect(file_name, &bProtected) == 0) {
        if (bProtected) 
            return GFILE_ERR_FILE_PROTECT;        
    }      
         
    meta_fd = open64(file_name, O_RDONLY);
    if (meta_fd < 0) {
        printf("gf_list_filedelete open metadata file %s error\n", file_name);
        return GFILE_ERR_EDIT_ERROR;
    }
    
    f_count = gf_metafile_getfilecount(meta_fd);          
    
    // delete all files
    for (i=1; i<=f_count; i++) {
        memset(full_name, 0, MAX_PATH_LENGTH); 
        gf_metafile_getfilename(meta_fd, i, &ptr);
        if (unlink(full_name) == -1)
            printf("libfaccess: unlink file %s error\n", full_name);            
    }
    
    // delete the directory
    if (rmdir(file_name_only) == -1)
        printf("libfaccess: rmdir directory %s error, maynot empty\n", file_name_only);
    
    // close and delete metadata file
    if (meta_fd) {
        if (close(meta_fd) < 0)
            printf("libfaccess: close meta data file error\n");
    }
    
    if (unlink(file_name) == -1)
        printf("libfaccess: unlink directory %s error\n", file_name);
        
    return GFILE_EDIT_SUCCESS;
}


ENUM_GFILE_EDITRET gf_list_filemerge(gflist_t *list_st1, gflist_t *list_st2) {
    int i, fcount1, fcount2;    
    char full_path[MAX_PATH_LENGTH], *ptr;
    char des_path[MAX_PATH_LENGTH];
    
    ptr = &(full_path[0]);

    printf("merge file %s -> %s\n", list_st2->file_name, list_st1->file_name);
    
    if (list_st1->file_handle > 0) {
        if (close(list_st1->file_handle) < 0)    
            printf("close file handle error\n");
    }   
    if (list_st2->file_handle > 0) {
        if (close(list_st2->file_handle) < 0)    
            printf("close file handle error\n");
    }   
        
    if ((list_st1->file_meta_handle < 0) || (list_st2->file_meta_handle < 0))
        assert(0);
        
    fcount1 = gf_metafile_getfilecount(list_st1->file_meta_handle);
    fcount2 = gf_metafile_getfilecount(list_st2->file_meta_handle);

    printf("file1's fcount: %d, file2's fcount: %d\n", fcount1, fcount2);
  
    for (i=1; i<=fcount2; i++) {
        memset(full_path, 0, MAX_PATH_LENGTH);
        memset(des_path, 0, MAX_PATH_LENGTH);     

        gf_metafile_getfilename(list_st2->file_meta_handle, i, &ptr);

        sprintf(des_path, "%s/R%04d%s", list_st1->file_name, fcount1, list_st1->file_name_extension);

        printf("rename %s -> %s\n", full_path, des_path);        
        if (rename(full_path, des_path) < 0) {
            printf("libfaccess: rename %s -> %s error\n", full_path, des_path);
            continue;
        }
        
        ++fcount1;

        gf_metafile_putfilename(list_st1->file_meta_handle, fcount1, des_path);
        gf_metafile_putfilecount(list_st1->file_meta_handle, fcount1);
    }
    
    close(list_st1->file_meta_handle);
    close(list_st2->file_meta_handle);
    list_st1->file_meta_handle = -1;
    list_st2->file_meta_handle = -1;
    
    // delete file2
    memset(full_path, 0, MAX_PATH_LENGTH);
    sprintf(full_path, "%s%s", list_st2->file_name, METADATA_EXTFILENAME);

    printf("delete metadata file %s\n", full_path);    
    if (unlink(full_path) < 0)
        printf("unlink file %s error\n", full_path);
        
    if (list_st2->ifo_file_handle < 0) {
        printf("delete directory %s\n", list_st2->file_name);        
        if (rmdir(list_st2->file_name) < 0)
            printf("rmdir directory %s error\n", list_st2->file_name);    
    }
    
    // clear info list node 
    gfdb_resetinfonode(list_st1->timeshift_ref, 0);  
    list_st1->file_info_now = NULL;
    
    return GFILE_EDIT_SUCCESS;
}


/*
 * Usage:  gfile_t * file_src = GFileOpen("xx.ts", O_RDONLY);
 *         gfile_t * file_dst = GFileOpen("new.ts", O_RDWR | O_CREAT);
 *         GFileSplit(file_src, file_dst, 2048);
 */
ENUM_GFILE_EDITRET gf_list_filesplit(gflist_t *list_src, gflist_t *list_dst, off64_t split_pos) {    
    int f_count;
    gfdb_info_list *tmpnode;
    char full_path[MAX_PATH_LENGTH], dst_path[MAX_PATH_LENGTH];        
    
    if (split_pos > gfdb_gettotalfilesize(list_src->timeshift_ref)) {
        printf("libfaccess: Split pos %lld > total file size %lld\n", split_pos, gfdb_gettotalfilesize(list_src->timeshift_ref));
        return GFILE_ERR_EDIT_ERROR;
    }   
    
    if ((split_pos == gfdb_gettotalfilesize(list_src->timeshift_ref)) ||
        (split_pos == 0)) {
        printf("libfaccess: Split pos %lld is the end or the start of file\n", split_pos);
        return GFILE_ERR_EDIT_ERROR;
    }

    if (list_src->file_handle > 0) {
        if (close(list_src->file_handle) < 0)    
            printf("close file handle error\n");
    }   

    // (1) copy 2nd part to new file
    gf_list_abcopy(list_src, split_pos, gfdb_gettotalfilesize(list_src->timeshift_ref), list_dst);    
 
    // (2) delete unused part of 1st part.
    tmpnode = gfdb_getinfonode(list_src->timeshift_ref, GFDB_GETINFOLIST_END);    
        
    f_count = gf_metafile_getfilecount(list_src->file_meta_handle);
    
    // (3) process from the end node...
    while (tmpnode) {        
        if (gf_info_getstartaddr(tmpnode) >= split_pos) {            
            // delete unused file
            memset(full_path, 0, MAX_PATH_LENGTH);
            if (gf_info_getidx(tmpnode) & 0xF0000000) {
                int tmp_f_count;
                char *ptr = &(full_path[0]);
                
                if (list_src->file_meta_handle == -1) {
                    printf("libfaccess: metadata file doesn't exist.\n");
                    return GFILE_ERR_EDIT_ERROR;
                }
                
                tmp_f_count = (gf_info_getidx(tmpnode) & 0x0FFFFFFF);
                    
                gf_metafile_getfilename(list_src->file_meta_handle, tmp_f_count, &ptr);                                        
            }
            // get filename from database node index
            else {                
                sprintf(full_path, "%s/R%04d%s", list_src->file_name, gf_info_getidx(tmpnode), list_src->file_name_extension);                                
            }       
                  
            printf("delete file %s\n", full_path);                        
            if (unlink(full_path) < 0) {
                printf("libfaccess: gf_list_filesplit unlink file %s error\n", full_path);
                return GFILE_ERR_EDIT_ERROR;
            }
            
            // update meta data file
            memset(full_path, 0, MAX_PATH_LENGTH);
            gf_metafile_putfilename(list_src->file_meta_handle, f_count, full_path);
            f_count--;
            gf_metafile_putfilecount(list_src->file_meta_handle, f_count);
            
            tmpnode = tmpnode->prev;
        }
        else if ((gf_info_getstartaddr(tmpnode) < split_pos) && 
                 ((gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)) >= split_pos)) {
            off_t start;
            int fd, src_fd, ret;
                        
            // copy useful part to a tmp file
            memset(dst_path, 0, MAX_PATH_LENGTH);                                    
            memset(full_path, 0, MAX_PATH_LENGTH);
            // (1) get filename
            if (gf_info_getidx(tmpnode) & 0xF0000000) {
                int tmp_f_count;
                char *ptr = &(full_path[0]);
                
                if (list_src->file_meta_handle == -1) {
                    printf("libfaccess: metadata file doesn't exist.\n");
                    return GFILE_ERR_EDIT_ERROR;
                }
                
                tmp_f_count = (gf_info_getidx(tmpnode) & 0x0FFFFFFF);
                    
                gf_metafile_getfilename(list_src->file_meta_handle, tmp_f_count, &ptr);                        
                sprintf(dst_path, "%s/R%04d.tmp", list_src->file_name, tmp_f_count);            
            }
            // get filename from database node index
            else {                
                sprintf(full_path, "%s/R%04d%s", list_src->file_name, gf_info_getidx(tmpnode), list_src->file_name_extension);                    
                sprintf(dst_path, "%s/R%04d.tmp", list_src->file_name, gf_info_getidx(tmpnode));            
            }            
                   
            // (2) open destination file (tmp file)
            fd = open64(dst_path, list_dst->file_open_mode, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);                        
            if (fd < 0) {
                printf("libfaccess: gf_list_splitfile() tmp file %s open error\n", dst_path);
                return GFILE_ERR_EDIT_ERROR;
            }
                                                                                
            // (3) open src file                                                                  
            src_fd = open64(full_path, O_RDONLY);
            if (src_fd < 0) {
                printf("libfaccess: gf_list_splitfile() file %s open error\n", full_path);
                close(fd);
                return GFILE_ERR_EDIT_ERROR;
            }
            
            // (4) do AB Record
            start = 0;            
            if ((ret = sendfile(fd, src_fd, &start, (split_pos-gf_info_getstartaddr(tmpnode)))) != (split_pos-gf_info_getstartaddr(tmpnode))) {                            
                printf("libfaccess: gf_list_splitfile sendfile() fail, start: 0, offset: %lld, ret: %d !!!\n", (split_pos-gf_info_getstartaddr(tmpnode)), ret);            
                close(fd);
                close(src_fd);
                return GFILE_ERR_EDIT_ERROR;
            }
            
            // (5) close filehandle
            close(fd);
            close(src_fd);
            
            // (6) delete old file
            if (unlink(full_path) < 0) {
                printf("libfaccess: gf_list_splitfile unlink file %s error\n", full_path);
                return GFILE_ERR_EDIT_ERROR;
            }
            
            // (7) rename tmp file to new file (orginal filename)
            if (rename(dst_path, full_path) < 0) {
                printf("libfaccess: gf_list_splitfile rename file %s -> %s error\n", dst_path, full_path);
                return GFILE_ERR_EDIT_ERROR;
            }
                        
            break;
        }
        else {
            printf("filesplit ERROR\n");    
            break;
        }    
    }
    
    if (_reset_list(list_src) == -1) {
        printf("libfaccess: gf_list_abcut() reset_list error\n");
        return GFILE_ERR_EDIT_ERROR;
    }
    
    return GFILE_EDIT_SUCCESS;
}



static int _cut_file(char *filename, char *filename_ext, int64_t file_size, int64_t start, int64_t end) {
    off_t s_start;
    int org_fd, dst_fd, mycase, ret;    
    char full_path[MAX_PATH_LENGTH], tmp_path[MAX_PATH_LENGTH];
    
    // (1) check which case we have to process..
    if ((start == 0) && (end == file_size))
        mycase = 1;   // only just delete this file
    else if ((start == 0) || (end == file_size))   
        mycase = 2;   // simple ABCopy
    else
        mycase = 3;   // need to call ABCopy twice

    memset(full_path, 0, MAX_PATH_LENGTH);
    sprintf(full_path, "%s%s", filename, filename_ext);
    
    // <1> only just delete this file
    if (mycase == 1) {        
        printf("_cut_file(): delete %s\n", full_path);
        if (unlink(full_path) < 0) {
            printf("libfaccess: unlink file %s error\n", full_path);
            return -1;
        }        
        return 1;
    }
   
    // open original file......    
    org_fd = open64(full_path, O_RDONLY);
    if (org_fd < 0) {
        printf("libfaccess: file %s open error\n", full_path);
        return -1;
    }
   
   
    // <2> Simple ABCopy
    if (mycase == 2) {
        off_t s_end;
        
        // create tmp file
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        sprintf(tmp_path, "%s.tmp", filename);
        dst_fd = open64(tmp_path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
        if (dst_fd < 0) {
            printf("libfaccess: file %s open error\n", tmp_path);
            return -1;
        }
        
        // using sendfile to copy data
        if (start == 0)
            s_start = (off_t)end;
        else
            s_start = 0;
        
        if (end == file_size)
            s_end = (off_t)start;
        else
            s_end = (off_t)file_size;   
                
        printf("prepare to sendfile: %lld ~ %lld (diff: %d)\n", s_start, s_end, (size_t)(s_end-s_start));
        if ((ret = sendfile(dst_fd, org_fd, &s_start, (size_t)(s_end-s_start))) != ((size_t)(s_end-s_start))) {
            printf("libfaccess: _cut_file sendfile() fail, ret: %d !!\n", ret);
            return -1;
        }
        
        close(org_fd);
        close(dst_fd);
        
        // rename tmp filename to original filename
        if (unlink(full_path) < 0) {
            printf("libfaccess: unlink file %s error\n", full_path);
            return -1;
        }
            
        if (rename(tmp_path, full_path) < 0) {
            printf("libfaccess: rename file %s -> %s error\n", tmp_path, full_path);
            return -1;
        }
        
        return 1;
    }
    
    // <3> need to call ABCopy twice
    if (mycase == 3) {
        // create tmp file (part2)
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        sprintf(tmp_path, "%s.part2", filename);
        dst_fd = open64(tmp_path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
        if (dst_fd < 0) {
            printf("libfaccess: file %s open error\n", tmp_path);
            return -1;
        }
        
        // using sendfile to copy data to part2
        s_start = (off_t)end;
        printf("prepare to sendfile: %lld ~ %lld\n", s_start, (s_start+file_size-end));
        if ((ret = sendfile(dst_fd, org_fd, &s_start, (file_size-end))) != (file_size-end)) {
            printf("libfaccess: _cut_file sendfile() fail, ret: %d !!\n", ret);
            return -1;
        }
            
        close(dst_fd);     
        
        // create tmp file (part1)   
        memset(tmp_path, 0, MAX_PATH_LENGTH);
        sprintf(tmp_path, "%s.part1", filename);
        dst_fd = open64(tmp_path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
        if (dst_fd < 0) {
            printf("libfaccess: file %s open error\n", tmp_path);
            return -1;
        }
        
        // using sendfile to copy data to part1
        s_start = (off_t)0;
        printf("prepare to sendfile: 0 ~ %lld\n", start);
        if ((ret = sendfile(dst_fd, org_fd, &s_start, start)) != start) {
            printf("libfaccess: _cut_file sendfile() fail, ret: %d !!\n", ret);
            return -1;
        }        
        
        close(dst_fd);
        close(org_fd);
        
        // rename part1 file to original filename
        if (unlink(full_path) < 0) {
            printf("libfaccess: unlink file %s error\n", full_path);
            return -1;
        }
        
        if (rename(tmp_path, full_path) < 0) {
            printf("libfaccess: rename file %s -> %s error\n", tmp_path, full_path);
            return -1;
        }
    }
    
    return 1;
}




ENUM_GFILE_EDITRET gf_list_abcut(gflist_t *list_st, int64_t pos_a, int64_t pos_b) {
    int i, bSet, mycase;
    int fcount_a, fcount_b, tmp_f_count;
    char full_path[MAX_PATH_LENGTH], dst_path[MAX_PATH_LENGTH];
    gfdb_info_list *tmpnode, *node_a, *node_b;
    
    fcount_a = 0;
    fcount_b = 0;
    
    printf("start AB Cut.....pos_a: %lld, pos_b: %lld\n", pos_a, pos_b);    
    
    if ((pos_a < 0) || (pos_b < 0) || (pos_b <= pos_a) ||
        ((pos_a%2048) != 0) || ((pos_b%2048) != 0) ||
        (pos_a > gfdb_gettotalfilesize(list_st->timeshift_ref)) ||
        (pos_b > gfdb_gettotalfilesize(list_st->timeshift_ref)))
        return GFILE_ERR_EDIT_ERROR;
        
    if (list_st->file_handle > 0) {
        if (close(list_st->file_handle) < 0)    
            printf("close file handle error\n");
    }    
    
    // locate node_a and node_b   
    bSet = 0;
    tmpnode = gfdb_getinfonode(list_st->timeshift_ref, GFDB_GETINFOLIST_HEAD);
    while (tmpnode) {
        if ((pos_a >= gf_info_getstartaddr(tmpnode)) &&
            (pos_a < (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            node_a = tmpnode;
            bSet++;
        }
        if ((pos_b >= gf_info_getstartaddr(tmpnode)) &&
            (pos_b <= (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode)))) {
            node_b = tmpnode;
            bSet++;
            break;
        }
        tmpnode = tmpnode->next;
    }
    
    if (bSet != 2) {
        printf("libfaccess: can't find pos_a and pos_b's node\n");
        return GFILE_ERR_EDIT_ERROR;
    }    
    
    // Check which method used to re-organize the metadata and all filenames
    // @NOTE:  the rule of mycase
    //      mycase >= 0:  the number of files need to be deleted
    //      mycase = -1:  don't need to do anything
    //      mycase = -2:  need to insert one "part2" file
    if (node_a == node_b) {
        if ((pos_a == gf_info_getstartaddr(node_a)) &&
            ((pos_b-pos_a) == gf_info_getfilesize(node_a)))
            mycase = 1;   // need to delete one unused file
        else
            mycase = -2;   // need to insert part2 file
    }
    else {
        if (((node_a->next) == node_b) &&
            (pos_a > gf_info_getstartaddr(node_a)) &&
            (pos_b < (gf_info_getstartaddr(node_b) + gf_info_getfilesize(node_b))))
            mycase = -1;   // lucky!, don't need to modify filname!
        else                
            mycase = 0;   // need to delete unused files        
    }

    printf("gf_list_abcut() before cut: mycase: %d\n", mycase);

    // Do Copy things...
    tmpnode = node_a;
    while (tmpnode) {        
        // (1) get filename from node
        memset(full_path, 0, MAX_PATH_LENGTH);
        memset(dst_path, 0, MAX_PATH_LENGTH);
            
        if (gf_info_getidx(tmpnode) & 0xF0000000) {        
            tmp_f_count = (gf_info_getidx(tmpnode) & 0x0FFFFFFF);
            tmp_f_count = tmp_f_count - 1;                        
        }
        // get filename from database node index
        else 
            tmp_f_count = gf_info_getidx(tmpnode);
        
        sprintf(full_path, "%s/R%04d", list_st->file_name, tmp_f_count);  
        printf("in while loop.... before cut: full_path: %s, idx: 0x%x\n", full_path, gf_info_getidx(tmpnode));

        // (2) Main Process....
        
        //  process start node
        if (tmpnode == node_a) {      
            fcount_a = tmp_f_count;          
           
            // if start node == end node
            if (node_a == node_b) {
                _cut_file(full_path, list_st->file_name_extension, gf_info_getfilesize(tmpnode), (pos_a-gf_info_getstartaddr(tmpnode)), (pos_b-gf_info_getstartaddr(tmpnode)));
                if (mycase == 1) {
                    fcount_a = (tmp_f_count - 1);
                    fcount_b = (tmp_f_count + 1);
                }
                    
                break;
            }
            // if start node != end node
            else {            
                _cut_file(full_path, list_st->file_name_extension, gf_info_getfilesize(tmpnode), (pos_a-gf_info_getstartaddr(tmpnode)), gf_info_getfilesize(tmpnode));
                if (mycase >= 0) {
                    if (pos_a == gf_info_getstartaddr(tmpnode)) {
                        mycase++;
                        fcount_a = (tmp_f_count -1);
                    }
                }
            }                      
        }        
        // process end node
        else if (tmpnode == node_b) {        
            fcount_b = tmp_f_count;   
            _cut_file(full_path, list_st->file_name_extension, gf_info_getfilesize(tmpnode), 0, (pos_b-gf_info_getstartaddr(tmpnode)));
            if (mycase >= 0) {
                if (pos_b == (gf_info_getstartaddr(tmpnode) + gf_info_getfilesize(tmpnode))) {
                    mycase++;
                    fcount_b = (tmp_f_count+1);   
                }
            }
            break;
        }
        // process the node between node_a and node_b
        else {    
            _cut_file(full_path, list_st->file_name_extension, gf_info_getfilesize(tmpnode), 0, gf_info_getfilesize(tmpnode));
            if (mycase >= 0) 
                mycase++;                
        }
        
        tmpnode = tmpnode->next;
    } // end of while (tmpnode)
 
    // re-organize metadata and all filenames 

    printf("gf_list_abcut(): after cut: mycase: %d\n", mycase);
    
    // need to insert one "part2" file
    if (mycase == -2) {
        int total_f_count = gf_metafile_getfilecount(list_st->file_meta_handle);                

        for (i=(total_f_count-1); i>=fcount_a; i--) {
            memset(full_path, 0, MAX_PATH_LENGTH);
            memset(dst_path, 0, MAX_PATH_LENGTH);
            if (i == fcount_a)
                sprintf(full_path, "%s/R%04d.part2", list_st->file_name, i);
            else
                sprintf(full_path, "%s/R%04d%s", list_st->file_name, i, list_st->file_name_extension);
                
            sprintf(dst_path, "%s/R%04d%s", list_st->file_name, (i+1), list_st->file_name_extension);
            
            if (rename(full_path, dst_path) < 0) {
                printf("libfaccess: rename file %s -> %s error\n", full_path, dst_path);
                return GFILE_ERR_EDIT_ERROR;
            }
            gf_metafile_putfilename(list_st->file_meta_handle, i+2, dst_path);
        }
                
        gf_metafile_putfilecount(list_st->file_meta_handle, (total_f_count+1));
    }
    else if (mycase > 0) {
        int total_f_count = gf_metafile_getfilecount(list_st->file_meta_handle);  
        
        // need to rename files and update metadata file
        if (total_f_count > fcount_b) {
            
            // set delete filename entry in metadata to empty
            for (i=0; i<mycase;i++) {
                memset(dst_path, 0, MAX_PATH_LENGTH);                
                gf_metafile_putfilename(list_st->file_meta_handle, (fcount_a+i+2), dst_path);
            }
            
            for (i=fcount_b; i<=(total_f_count-1); i++) {
                memset(full_path, 0, MAX_PATH_LENGTH);
                memset(dst_path, 0, MAX_PATH_LENGTH);
                sprintf(full_path, "%s/R%04d%s", list_st->file_name, i, list_st->file_name_extension);
                sprintf(dst_path, "%s/R%04d%s", list_st->file_name, (i-mycase), list_st->file_name_extension);

                if (rename(full_path, dst_path) < 0) {
                    printf("libfaccess: rename file %s -> %s error\n", full_path, dst_path);
                    return GFILE_ERR_EDIT_ERROR;
                }
                gf_metafile_putfilename(list_st->file_meta_handle, (i-mycase)+1, dst_path);
                memset(dst_path, 0, MAX_PATH_LENGTH);
                gf_metafile_putfilename(list_st->file_meta_handle, i+1, dst_path);
            }

            gf_metafile_putfilecount(list_st->file_meta_handle, (total_f_count-mycase));
        }
        // only need to update metadata file.
        // ex: the last file has already been deleted.......
        else {
            for (i=(fcount_a+1);i<=(total_f_count-1);i++) {
                memset(full_path, 0, MAX_PATH_LENGTH);
                gf_metafile_putfilename(list_st->file_meta_handle, i+1, dst_path);                
            }
            gf_metafile_putfilecount(list_st->file_meta_handle, (total_f_count-mycase));
        }
    }
 
    if (_reset_list(list_st) == -1) {
        printf("libfaccess: gf_list_abcut() reset_list error\n");
        return GFILE_ERR_EDIT_ERROR;
    }
 
    return GFILE_EDIT_SUCCESS;     
}

