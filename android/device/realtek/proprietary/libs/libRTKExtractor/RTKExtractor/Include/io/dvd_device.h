#ifndef __H_DVD_DEVICE_H__
#define __H_DVD_DEVICE_H__


#include <Filters/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <io/dvd_parse.h>
#include <io/dvd_path_define.h>

#ifdef WIN32
#define PDEBUG
#else
//#define DVDIO_DEBUG
#ifdef DVDIO_DEBUG
 #define PDEBUG(fmt, args...) printf(fmt, ## args)
#else
 #define PDEBUG(fmt, args...)  /*empty macro */
#endif
#endif

// The block size of a DVD
#define DVD_BLOCK_SIZE      2048


#ifdef IFO_CACHE_DATA
    #include <io/IOCache.h>
    #define IFO_ECC_SIZE IOCACHE_SIZE_2K
#endif /* IFO_CACHE_DATA */



struct _dvd_handle_t {    
    int    fd;		//udf11: dvdFile handle
    char * psz_device; // filename	        
    unsigned int lb_start;
    	            
    int decrypt;
    int cgms;
    
	int b_reopen; // for reopen file
	__off64_t size; //unit : block (2048 byte)
        __off64_t    i_pos;
	//pointer to struct dvd_reader
	void *dvd;
#ifdef DECSS_ONLY_ONE_TITLE_KEY_AT_VTS
	void *dvd_file;
#endif /* DECSS_ONLY_ONE_TITLE_KEY_AT_VTS */
    char b_check_dvdcp; 
    char vobfile; // 1: vob file; 2: vro file ;0:others
    unsigned char  p_key[5];
};



#endif /* __H_DVD_DEVICE_H__ */
