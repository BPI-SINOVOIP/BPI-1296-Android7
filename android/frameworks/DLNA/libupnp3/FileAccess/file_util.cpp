#include "file_util.h"
// #include <Util.h>

gfdb_info_list * gfdb_info_create_node() {
    gfdb_info_list *node = NULL;
    
    node = (gfdb_info_list *)malloc(sizeof(gfdb_info_list));
    if (node == NULL) {
        printf("gfile_info_list malloc error\n");
        return NULL;
    }
    
    memset(node, 0, sizeof(gfdb_info_list));
    return node;    
}


int gf_info_getidx(gfdb_info_list *node) {
    if (node == NULL) 
        return -1;
    
    return node->filelist_idx;
}

off64_t gf_info_getstartaddr(gfdb_info_list *node) {
    if (node == NULL)
        return -1;
    return node->start_addr;
}

int64_t gf_info_getfilesize(gfdb_info_list *node) {
    if (node == NULL)
        return -1;
    return node->file_size;
}


//////////////////////////////////////////////////////////////////
// meta data related

int gf_metafile_checkvalid(int fd) {
    int c;
    int64_t len;
    char tmp[30];
    
    if (fd < 0)
        return -1;
        
    // check header's firstword
    memset(tmp, 0, 30);
    lseek64(fd, 0, SEEK_SET);    
    if (read(fd, &tmp[0], METADATA_HEADER_FIRSTWORD_LEN) != METADATA_HEADER_FIRSTWORD_LEN)
        return -1;        
    if (strncmp(tmp, METADATA_HEADER_FIRSTWORD, METADATA_HEADER_FIRSTWORD_LEN) != 0)
        return -1;
    
    // check file count
    lseek64(fd, METADATA_FILECOUNT_START_ADDR, SEEK_SET);    
    if (read(fd, &c, METADATA_FILECOUNT_LEN) != METADATA_FILECOUNT_LEN)
        return -1;
    if ((c > GF_MAX_FILE_COUNT) || (c <= 0))
        return -1;
    
    //@FIXME: need to check file names ??    
    
    // check file size
    len = lseek64(fd, 0, SEEK_END);
    // after Split, since filecount decrease, metadata filesize maybe large than expect.
    if (len < METADATA_FILENAME_START_ADDR + (c*METADATA_FILENAME_LEN))
        return -1;
    
    return 1;    
}

int gf_metafile_initialize(int fd) {
    int c;
    char tmp[20];
       
    if (fd < 0)
        return -1;
    
    // Wirte Sync Word
    lseek64(fd, 0, SEEK_SET);    
    memset(tmp, 0, 20);        
    snprintf(tmp, METADATA_HEADER_FIRSTWORD_LEN+1, "%s", METADATA_HEADER_FIRSTWORD);
    
    if (write(fd, &tmp, METADATA_HEADER_FIRSTWORD_LEN) != METADATA_HEADER_FIRSTWORD_LEN)
        return -1;
    
    // write file count = 0    
    c = 0;
    lseek64(fd, METADATA_FILECOUNT_START_ADDR, SEEK_SET);
    if (write(fd, &c, METADATA_FILECOUNT_LEN) != METADATA_FILECOUNT_LEN)
        return -1;    
    return 1;        
}

int gf_metafile_getfilecount(int fd) {
    int c = 0;
    if (fd < 0)
        return -1;
        
    lseek64(fd, METADATA_FILECOUNT_START_ADDR, SEEK_SET);
    if (read(fd, &c, METADATA_FILECOUNT_LEN) != METADATA_FILECOUNT_LEN)
        return -1;
    
    return c;    
}

int gf_metafile_putfilecount(int fd, int c) {
    if (fd < 0)
        return -1;
    lseek64(fd, METADATA_FILECOUNT_START_ADDR, SEEK_SET);
    if (write(fd, &c, METADATA_FILECOUNT_LEN) != METADATA_FILECOUNT_LEN)
        return -1;
        
    return 1;    
}


int gf_metafile_gettitlename(int fd, char **name) {
    if (fd < 0)
        return -1;
    
    lseek64(fd, METADATA_TITLENAME_START_ADDR, SEEK_SET);
    if (read(fd, *name, METADATA_TITLENAME_LEN) != METADATA_TITLENAME_LEN)
        return -1;
    return 1;
}


int gf_metafile_puttitlename(int fd, char *name, int length) {
    if ((fd < 0) || (length > METADATA_TITLENAME_LEN))
        return -1;
    
    lseek64(fd, METADATA_TITLENAME_START_ADDR, SEEK_SET);
    if (write(fd, name, length) != length)
        return -1;
    return 1;    
}


int gf_metafile_getrecordtime(int fd, int *hour, int *min, int *sec) {
    if (fd < 0)
        return -1;
    
    lseek64(fd, METADATA_RECORDTIME_START_ADDR, SEEK_SET);
    if ((hour) && (read(fd, hour, 4) != 4))
        return -1;
    lseek64(fd, METADATA_RECORDTIME_START_ADDR+4, SEEK_SET);    
    if ((min) && (read(fd, min, 4) != 4))
        return -1;
    lseek64(fd, METADATA_RECORDTIME_START_ADDR+8, SEEK_SET);    
    if ((sec) && (read(fd, sec, 4) != 4))
        return -1;    
    
    return 1;    
}


int gf_metafile_putrecordtime(int fd, int hour, int min, int sec) {
    if (fd < 0)
        return -1;
    
    lseek64(fd, METADATA_RECORDTIME_START_ADDR, SEEK_SET);    
    if (write(fd, &hour, 4) != 4)
        return -1;
    lseek64(fd, METADATA_RECORDTIME_START_ADDR+4, SEEK_SET);    
    if (write(fd, &min, 4) != 4)
        return -1;
    lseek64(fd, METADATA_RECORDTIME_START_ADDR+8, SEEK_SET);
    if (write(fd, &sec, 4) != 4)
        return -1;   
        
    return 1;         
}


int gf_metafile_getfilename(int fd, int fcount, char **name) {
    if (fd < 0)
        return -1;
        
    lseek64(fd, METADATA_FILENAME_START_ADDR+((fcount-1)*METADATA_FILENAME_LEN), SEEK_SET);    
    if (read(fd, *name, METADATA_FILENAME_LEN) != METADATA_FILENAME_LEN)
        return -1;
    return 1;
}


int gf_metafile_putfilename(int fd, int fcount, char *name) {
    int ret;
    if (fd < 0)
        return -1;
         
    lseek64(fd, METADATA_FILENAME_START_ADDR+((fcount-1)*METADATA_FILENAME_LEN), SEEK_SET);
    if ((ret = write(fd, name, METADATA_FILENAME_LEN)) != METADATA_FILENAME_LEN)        
        return -1;    
    
    return 1;
}


