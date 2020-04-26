#ifndef _H_DVD_PARSER_H_
#define _H_DVD_PARSER_H_


#include <Filters/config.h>
#include <sys/types.h>

#if defined(__ANDROID__)
typedef off64_t __off64_t;
#endif

#include <OSAL.h>
#include <io/dvd_device.h>
#include "dvdcp/dvdcp.h"

// Logical Block length
#define DVD_LB_LEN 2048

#ifndef PATH_MAX
  #define DVD_PATH_MAX   768
#else
  #define DVD_PATH_MAX   PATH_MAX
#endif


// Max length of filename in UDF.
#define MAX_UDF_FILE_NAME_LEN 2048

//cfyeh:2005/10/19 for reopen file
#define DVD_REOPENFILE(x) ((x)->b_reopen == 1)
#define DVD_REOPENFILE_ENABLE(x) ((x)->b_reopen = 1)
#define DVD_REOPENFILE_DISABLE(x) ((x)->b_reopen = 0)



typedef struct dvdcp_s *dvdcp_handle;

#define Input_to_Reader(dev) ((dvd_info_t *)((dev)->dvd))

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _dvd_info_t dvd_info_t;
typedef struct _dvd_file_t dvd_file_t;
typedef struct _dvd_handle_t *dvd_handle_t;

struct _dvd_info_t {
    /* Basic information. */
    int isReadOnly;

    // 0:file mode(HD) 1: DVD-VIDEO(disc)
    // 2:DVD+VR(disc), 3:DVD-VR(disc)
    int data_type;
	  
    // 0:NO disc, 1:CD 
    // 2:DVD-ROM, 3:DVD-RAM
    // 4:DVD+R(finalized), 5:DVD+R(unfinalized)
    // 6:DVD+RW(finalized), 7: DVD+RW(unfinalized)
    // 8:DVD-R(finalized), 9:DVD-R(unfinalized)
    // 10:DVD-RW(finalized), 11: DVD-RW(unfinalized)
    // 12:DVD+R DL(finalized), 13:DVD+R DL(unfinalized)
    int disc_type;

    //0:NONE, 1: use udf1.1 code 
    int b_udf_fs_11;

    /* Information required for a directory path drive. */
    char *root_dir;
    int    i_fd;			//device descriptor
    int    i_disc_fd;		//dvdDiscOpen handle

    /* Hack for keeping track of the css status. 
     * 0: no context protect, 1: CSS/CPPM, 2: CPRM */
    int cp_state;

    /* libdvdcss handle */
    dvdcp_t dvdcp;

	//cfyeh:2005/10/19 for reopen file
	int b_reopen;

	/* IOCache Handle*/
	int IOCacheHandle_read;
	int read_cache_buf_num;
	int IOCacheHandle_write;
	int write_cache_buf_num;
	int WriteQuery_seq;
	
    // The function pointers 
#ifdef DECSS_ONLY_ONE_TITLE_KEY_AT_VTS
    int         (*dvdinfo_open) (dvd_handle_t, char);
#else
    int         (*dvdinfo_open) (dvd_handle_t);
#endif /* DECSS_ONLY_ONE_TITLE_KEY_AT_VTS */
    int         (*dvdinfo_close) (dvd_handle_t);
    ssize_t     (*dvdinfo_seek)  (dvd_handle_t, ssize_t);
    int         (*dvdinfo_read)  (dvd_handle_t, void *, int);
    char *      (*dvdinfo_error) (dvd_handle_t);
    int         (*dvdinfo_write) (dvd_handle_t, void *, int);
    int         (*dvdinfo_writebytes) (dvd_handle_t, void *, int);
    int         (*dvdinfo_create) (dvd_handle_t);
    int         (*dvdinfo_reopen)  (dvd_handle_t dev, const char *, unsigned int startAddr);
    int         (*dvdinfo_createdirectory)  (const char *);
	int         (*dvdinfo_flush)   (dvd_handle_t);

};

struct _dvd_file_t {
    dvd_info_t *dvd;
    __off64_t seek_pos;      
    __off64_t filesize;

    __off64_t vobfile_size[ 9 ];
    dvd_handle_t vobfile_handle[ 9 ];
};



typedef enum {
  ENUM_DVD_IFO, //DVD_READ_INFO_FILE,        // VIDEO_TS.IFO  or VTS_XX_0.IFO 
  ENUM_DVD_IFOBUP, //DVD_READ_INFO_BACKUP_FILE, // VIDEO_TS.BUP  or VTS_XX_0.BUP 
  ENUM_DVD_MENUVOB, //DVD_READ_MENU_VOBS,        // VIDEO_TS.VOB  or VTS_XX_0.VOB
  ENUM_DVD_TITLEVOB, //DVD_READ_TITLE_VOBS,       // VTS_XX_[1-9].VOB 
  
  // For +VR
  ENUM_DVD_VRMI_IFO, //DVD_READ_VRMI_INFO_FILE,
  ENUM_DVD_VRMI_BUP,//DVD_READ_VRMI_INFO_BACKUP_FILE,
  ENUM_DVD_VRMIVIDEO_IFO, //DVD_READ_VRMIVIDEO_INFO_FILE,
  ENUM_DVD_VRMIVIDEO_BUP, //DVD_READ_VRMIVIDEO_INFO_BACKUP_FILE,

  // For -VR
  DVR_READ_INFO_FILE,		 // VR_MANGR.IFO 
  DVR_READ_INFO_BACKUP_FILE, // VR_MANGR.BUP
  DVR_READ_MOVIE_VRO,
  DVR_READ_STILL_VRO,
  DVR_READ_AUDIO_VRO
} ENUM_DVD_FILETYPE;




#ifdef VOB_CACHE_DATA
ssize_t RTDVDReadBlocksCache( dvd_file_t *, __off64_t, size_t, unsigned char ** );
#endif /* VOB_CACHE_DATA */


dvd_info_t*  RTDVDHandleOpen(const char *);
void         RTDVDHandleClose(dvd_info_t *);

ssize_t RTDVDFileReadBlocks(dvd_file_t *, __off64_t, size_t, unsigned char *);
ssize_t VRFileReadBlocks(dvd_file_t *, __off64_t, size_t, unsigned char *);

ssize_t RTDVDFileReadBytes(dvd_file_t *, void *, size_t);
ssize_t VRFileReadBytes(dvd_file_t *, void *, size_t);

dvd_file_t*    RTDVDFileOpen(dvd_info_t *, int, ENUM_DVD_FILETYPE);
void           RTDVDFileClose(dvd_file_t *);
__off64_t      RTDVDFileSeek(dvd_file_t *, __off64_t);
ssize_t        RTDVDGetFileSize(dvd_file_t *);
dvd_info_t*    RTDVDCreate(const char *);	

int RTDVDQueryVobDecrypt(dvd_file_t *dvd_file, __off64_t offset);
int RTDVDQueryVobCGMS(dvd_file_t *dvd_file, __off64_t offset);
int RTDVDQueryVobCGMSbyLSN(dvd_file_t *dvd_file, __off64_t offset);

int DVDQueryCopyRight(dvd_info_t *dvd);

// Added by Zack. We need a generic function to check if the folder is existed.
bool IsFolderExist(const char* pFolderPath);

#ifdef __cplusplus
};
#endif
#endif /* _H_DVD_PARSER_H_ */
