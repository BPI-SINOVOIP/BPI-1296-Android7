#ifndef __H_GENERAL_FILE_INFO_LIST__
#define __H_GENERAL_FILE_INFO_LIST__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include "OSAL.h"


/*
 *  MetaData file related
 */
// max files in one metadata file 
#define GF_MAX_FILE_COUNT             9999



///////////////////////////////////////////////
// The Content definition in metadata file

// (1) sync word  (start: 0, length: 11 bytes)
#define METADATA_HEADER_FIRSTWORD    "RTKMetaData"
#define METADATA_HEADER_FIRSTWORD_LEN   11
// (2) file count (start: 16, length: 4 bytes)
#define METADATA_FILECOUNT_START_ADDR   16
#define METADATA_FILECOUNT_LEN           4
// (3) title name (start: 32, length: 128 bytes)
#define METADATA_TITLENAME_START_ADDR   32
#define METADATA_TITLENAME_LEN         128
// (4) record time (start: 160, length: 12 bytes)
#define METADATA_RECORDTIME_START_ADDR 160
#define METADATA_RECORDTIME_LEN         12
// (5) filename (start: 1024, length: 1024 bytes for each !)
#define METADATA_FILENAME_START_ADDR  1024
#define METADATA_FILENAME_LEN         1024

/*
 *  Double Link List:
 *   used to keep the file list information
 */
struct tar_gfdb_info_list_t;
typedef struct tag_gfdb_info_list_t gfdb_info_list;
struct tag_gfdb_info_list_t {
    /* doublely link node pointer */
    gfdb_info_list *next, *prev;
    /* file index */
    int filelist_idx;
    /* file size */
    int64_t file_size;       
    /* virtual file start address*/ 
    off64_t start_addr;
};


gfdb_info_list * gfdb_info_create_node();
int gf_info_getidx(gfdb_info_list *node);
off64_t gf_info_getstartaddr(gfdb_info_list *node);
int64_t gf_info_getfilesize(gfdb_info_list *node);

int gf_metafile_checkvalid(int fd);
int gf_metafile_initialize(int fd);

int gf_metafile_getfilecount(int fd);
int gf_metafile_putfilecount(int fd, int c);

int gf_metafile_gettitlename(int fd, char **name);
int gf_metafile_puttitlename(int fd, char *name, int length);

int gf_metafile_getrecordtime(int fd, int *hour, int *min, int *sec);
int gf_metafile_putrecordtime(int fd, int hour, int min, int sec);

int gf_metafile_getfilename(int fd, int fcount, char **name);
int gf_metafile_putfilename(int fd, int fcount, char *name);


#endif //__H_GENERAL_FILE_INFO_LIST__
