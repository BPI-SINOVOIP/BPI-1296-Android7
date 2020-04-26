#ifndef __PPS_H__
#define __PPS_H__


#ifndef HASH_VALUE_LENGTH
#define HASH_VALUE_LENGTH  20
#endif

#define PPS_EXTRA_MSG_LEN  64
#define PPS_MAX_MAP_SIZE   65536

typedef unsigned long long uint64_t;
typedef long long          int64_t;

typedef enum pps_event_id
{
	EPPS_SERVER_TIMEOUT,            // 0,  连接服务器超时,停止播放
	EPPS_NO_SERVER,                 // 1,  找不到服务器,停止播放
	EPPS_SERVER_NO_RESPONSE,        // 2,  服务器未响应,停止播放
	EPPS_SERVER_INVALID,            // 3,  解析服务器域名失败.
	EPPS_VERSION_LOW,               // 4,  客户端版本号太低,退出网络
	EPPS_CLIENT_NAME,               // 5,  客户端名称或引用页不正确,退出播放
	EPPS_URL_INVALID,               // 6,  播放的文件不存在或网址错误
	EPPS_NO_SERVICE,                // 7,  服务不存在,停止播放
	EPPS_OUT_OF_SERVICE,            // 8,  不在服务区.    
	EPPS_MEDIA_INFO_ERROR,          // 9,  媒体信息加载时错误..
	EPPS_INDEX_INFO_ERROR,          // 10, 媒体索引信息错误
	EPPS_NO_INDEX,                  // 11, 没有获取到媒体索引信息,不能拖动播放位置
	EPPS_NO_MEIDIA,                 // 12, 找不到媒体
	EPPS_MULTI_INSTANCE,            // 13, 请确定是否有另一个实例正在播放此文件
	EPPS_PREPARE_MEDIA_INFO,        // 14, 正在准备媒体信息
	EPPS_GETTING_MEDIA_INFO,        // 15, 正在获取媒体信息
	EPPS_GETTING_INDEX_INFO,        // 16, 正在获取媒体索引信息
	EPPS_PLAYING,                   // 17, 正在播放
	EPPS_BUFFERING,                 // 18, 正在缓冲数据
	EPPS_CONNECTING,                // 19, 正在连入网络
	EPPS_MEDIA_READY,               // 20, 准备媒体就绪
	EPPS_PARSING_SERVER,            // 21, 正在解析服务器域名..
	EPPS_GET_MEDIA_INFO,            // 22, 已成功获取到媒体信息.
	EPPS_BUFFERED,                  // 23, 数据缓冲完毕.
	EPPS_PREPARE_MEDIA,             // 24, 准备媒体
	EPPS_STORAGE_ERROR,             // 25. 访问存储器失败
	EPPS_QUIT,                      // 26. 程序退出

	EPPS_DATA_TIMEOUT,              // 27, 等待数据超时

	EPPS_UPDATING,                  // 28, 正在升级
	EPPS_UPDATE_FAILED,             // 29, 升级失败
	EPPS_UPDATE_OK,                 // 30, 升级成功
	EPPS_AUTH_UNPASSED,				// 31, 验证成功，非付费用户
	EPPS_AUTH_PASSED,				// 32, 验证成功，付费用户
	EPPS_AUTH_INVALID,				// 34, 单部点播，验证未通过
	EPPS_AUTH_FAILED,				// 33, 验证失败
	EPPS_ALLOCATE_PGF,                      //34, 预先分配缓存文件空间失败
}pps_event_id;

#define	ERR_PPS_SUCCESS	0	        // No error
#define	ERR_PPS_COMMON	-1	        // common error
#define	ERR_PPS_SERVER_FAILED	-2	// server timeout or transmit error
#define	ERR_PPS_AUTH_UNPASSED	-3	// auth unpassed
#define	ERR_PPS_AUTH_INVALID	-4	// auth invalid

typedef struct pps_event
{
	pps_event_id id;
	unsigned int wparam;
	unsigned int lparam;
	char         extra[PPS_EXTRA_MSG_LEN];
}pps_event;

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*pps_callback)(int index, pps_event event); 

#ifdef __cplusplus
}
#endif


typedef struct ppsvod_iteminfo
{
	int          index;
	char         hash_id[HASH_VALUE_LENGTH];        // task hash id
	char        *file_name;                         // save name
	unsigned int file_size;                         // total file size
	int          progress;                          // download percent 
	int          down_speed;                        // download speed (byte/s)
	int          up_speed;                          // upload speed (byte/s)
	int          peer_count;                        // peer count has connected
	int          status;                            // has the task been started?
}ppsvod_iteminfo;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Description: ppsvod initialization, must be called before all.
 * param:       Datatype               Param name      In/Out     Comment
 *              const char *           oem_name        in
 *              const char *           terminal_type   in
 *              const char *           device_id       in
 *              pps_callback           pcallback       in         callback function pointer
 * return:      int    return 0 if success; -1 if failed.
 */
int ppsvod_create(const char *oem_name, 
                const char * terminal_type, 
                const char *device_id, 
                pps_callback pcallback);

/*
 * Description: uninitialize ppsvod, call this fuction when you want to exit ppsvod.
 * param:       Datatype     Param name      In/Out     Comment
 *              void
 * return:      int    return 0 if success; -1 if failed
 */
int ppsvod_destroy(void);

/*
 * Description: Add an item into ppsvod internal list.
 * param:       Datatype     Param name      In/Out     Comment
 *              char *       url             In         url to add
 *              int          vip_flag        In         flag for vip items
 * return:      int    return a file descriptor if success; -1 if failed.
 */
int ppsvod_add_item(char *url, int vipflag);


/*
 * Description: Remove an item from ppsvod internal list.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      int    return 0 if success; -1 if failed.
 */
int ppsvod_remove_item(int fd);

/*
 * Description: start to play a file.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 *              const char*  validcode       In         
 * return:      int    return 0 if success; -1 if failed.
 */
int  ppsvod_play_item(int fd, const char * validcode);

/*
 * Description: stop the playing url.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      int    return 0 if success; -1 if failed; -3 means validcode error.
 */
int ppsvod_stop_item(int fd);

/*
 * Description: read data.
 * param:       Datatype     Param name      In/Out     Comment
 *              int            fd              In       file descriptor
 *              char *         buffer          Out      buffer to store data
 *              unsigned int   length          In       data length
 *              unsigned int   position        In       from where to read.
 *              struct timeval* wait_time      In       time length that read() can block
 * return:      int    return length of data if success; -1 if failed(errno
 *                     is set to EAGAIN if data is temporarily unavailable; etherwise,
 *                     some error happens or EOF reaches). 
 */
int ppsvod_read(int fd, char *buffer, unsigned int length, unsigned int position, struct      timeval *wait_time);

/*
 * Description: query buffer status
 * param:       Datatype     Param name      In/Out     Comment
 *              int            fd              In       file descriptor
 *              unsigned int   length          In       data length
 *              unsigned int   position        In       from where to read.
 * return:      int    return available data size if success; -1 if no data found.
 *              This is a non-block function.
 */
int ppsvod_query_read(int fd, unsigned int length, unsigned int position);

/*
 * Description: Drop read operation
 * param:       Datatype     Param name      In/Out     Comment
 *              int            fd              In       file descriptor
 */
void ppsvod_drop_read(int fd);

/*
 * Description: seeking within a playing url.
 * param:       Datatype     Param name      In/Out     Comment
 *              int           fd              In        file descriptor
 *              unsigned int  position        In        seeking destination
 * return:      int    return 0 if success; -1 if failed.
 */
int  ppsvod_seek(int fd, unsigned int position);

/*
 * Description: get total file length
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      int    return file length if success; -1 if failed.
 */
int  ppsvod_get_file_length(int fd);

/*
 * Description: validate url.
 * param:       Datatype     Param name      In/Out     Comment
 *              const char*    url            In        url to check 
 * return:      int    return 0 if success; -1 if failed.
 */
int  ppsvod_check_url(const char* url);

/*
 * Description: Get information for a playing file.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      ppsvod_iteminfo*   return a pointer to ppsvod_iteminfo if success; 
 *                                 NULL if failed.
 */
ppsvod_iteminfo *ppsvod_get_item_info(int fd);

/*
 * Description: Free memory of a ppsvod_iteminfo* pointer
 * param:       Datatype         Param name      In/Out     Comment
 *              ppsvod_iteminfo* piteminfo       In         pointer returned by ppsvod_get_item_info
 * return:      void
 */
void ppsvod_free_item_info(ppsvod_iteminfo *piteminfo);

/*
 * APIs for playlist operation
 */

/* 
 * Define download, unzip and parse vodlist file status.
 * vodlist status use highest 4 bits to express stage, use 
 * lowest 8 bits to express every stage process 
 */
#define VODLIST_IDLE            0x0
#define VODLIST_DOWNLOADING     0x10000000
#define VODLIST_UNCOMPRESSING   0x20000000
#define VODLIST_PARSING         0x40000000
#define VODLIST_UPDATE_SUCCESS  0x80000000
#define VODLIST_UPDATE_FAIL     0xff000000

typedef struct __ppscategory
{
	int    id;                       /* category or subcategory id */
	char  *name;                     /* category or subcategory num */
	int    type;                     /* subcategory type. 0: movie, 1: teleplay */
	int    subcatnum;                /* subcategory number under this category */
	struct __ppscategory *psubcat;   /* subcategory array */

	struct __ppscategory *next;
}ppscategory;


typedef	struct __ppsitem
{
	int    id;                       /* Item id */
	int    index;                    /* Item index */
	int    size;                     /* Item size (MB) */
	int    duration;                 /* Item duration (Minute)*/
	char  *format;                   /* file format (rm/rmvb/wmv) */
	int    bitrate;                  /* bitrate */
	char  *ppsurl;                   /* pps vod url */
	int    vipflag;                  /* vip flags */
	struct __ppsitem *next;
}ppsitem;

typedef struct __ppschannel
{
	char *name;                     /* channel name */
	char *director;
	char *actor;
	char *area;
	int   size;                     /* all items' total size under this channel (MB) */
	char *pubtime;                  /* publish time */
	int   duration;                 /* all items' total duration time under this channel (Min) */
	char *lang;                     /* channel language */
	char *desc;                     /* channel description */
	char *BImgUrl;                  /* Big Image Url */
	char *SImgUrl;                  /* Small Image Url */
	int   itemnum;                  /* num of items */
	struct __ppsitem *pitems;       /* point to items list */
	
	struct __ppschannel *next;
}ppschannel;


/*
 *  description: vod list initialization, must be called before all.
 *  param:       datatype     param name      in/out     comment
 *               void
 *  return:      int    return 0 if success; -1 if failed.
*/
int pps_vodlist_init(void);

/*
 *  description: vod list uninitialize, call this fuction when you want to exit pps vodlist.
 *  param:       datatype     param name      in/out     comment
 *               void
 *  return:      int    return 0 if success; -1 if failed.
*/
int pps_vodlist_uninit(void);

/*
 *  description: get all categories and sub categories list.
 *  param:       datatype     param name      in/out     comment
 *               void
 *  return:      ppscategory* pointer of all categories, null if failed.
*/
ppscategory *pps_vodlist_getcat(int *catnum);

/*
 *  description: get channels under pointed catogory and sub category.
 *  param:       datatype     param name      in/out     comment
 *               int          catid           in         class id
 *               int          subcatid        in         sub class id
 *               int          page_size       in         channels one page have
 *               int          page_num        in         page num which want to query
 *  return:      ppschannel pointer of channels, null if failed.
*/
ppschannel *pps_vodlist_getchannel(int catid, int subcatid, int page_size, int page_num);

/*
 * Description: configure directory for store cache, config, data and logs.
 * param:       Datatype     Param name      In/Out     Comment
 *              const char*  root_path       In         root path used to store
 * return:      int          return 0 if success, -1 if failed.
 * comment:     
 */
int ppsvod_save_config_dir(const char * root_path);

/*
 * Description: load configure directory for store cache, config, data and logs.
 * param:       Datatype     Param name      In/Out     Comment
 *              void
 * return:      char*        return root path if success, NULL if failed.
 * comment:     
 */
char *ppsvod_get_config_dir(void);

/*
 * Description: load default configure directory for store cache, config, data and logs.
 * param:       Datatype     Param name      In/Out     Comment
 *              void
 * return:      int          return 0 if success, -1 if failed.
 * comment:     
 */
int ppsvod_default_config_dir(void);

/*
 * Description: Deposit money for PPS VIP account
 * param:       Datatype     Param name      In/Out     Comment
 *              const char*  password        In         17 numbers sold by pps
 * return:      int          return 0 if success, -1 if failed.
 * comment:     
 */
int ppsvod_charge(const char* password);

/*
 * Description: Get account type of current user
 * param:       Datatype     Param name      In/Out     Comment
 *              void
 * return:      int          return -1 if failed;
 *                           return 0: Non-VIP user
 *                           return 1: VIP user
 * comment:     
 */
int ppsvod_get_account_type(void);

/*
 * Description: Get expire time of vip qualification
 * param:       Datatype     Param name      In/Out     Comment
 *              char*        time            Out        memory allocated by caller
 * return:      int          -1: failed
 *                            0: OK
 */
int ppsvod_get_vip_expire_time(char *time);

/*
 * Description: Get time from network
 * param:       Datatype     Param name      In/Out     Comment
 *              struct timeval  tv           Out        time since 1970
 * return:      int          return 0 if success, -1 if failed.
 * comment:     
 */
int ppsvod_get_time(struct timeval *tv);


/**************************************************/
/***************PPS Downloading API****************/
/**************************************************/

/*
 * Description: Add an item into ppsdwn internal list.
 * param:       Datatype     Param name      In/Out     Comment
 *              char *       url             In         url to add
 *              char *       save_path       In         local file path
 * return:      int    return a file descriptor if success; -1 if failed.
 */
int ppsdwn_add_item(char *url, char *save_path);

/*
 * Description: Remove an item from ppsdwn internal list.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      int    return 0 if success; -1 if failed.
 */
int ppsdwn_remove_item(int fd);

/*
 * Description: start to play a file.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      int    return 0 if success; -1 if failed.
 */
int  ppsdwn_download_item(int fd);

/*
 * Description: stop the playing url.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      int    return 0 if success; -1 if failed; -3 means validcode error.
 */
int ppsdwn_stop_item(int fd);

/*
 * Description: Get information for a playing file.
 * param:       Datatype     Param name      In/Out     Comment
 *              int          fd              In         file descriptor
 * return:      ppsvod_iteminfo*   return a pointer to ppsvod_iteminfo if success; 
 *                                 NULL if failed.
 */
ppsvod_iteminfo *ppsdwn_get_item_info(int fd);

/*
 * Description: Free memory of a ppsvod_iteminfo* pointer
 * param:       Datatype         Param name      In/Out     Comment
 *              ppsvod_iteminfo* piteminfo       In         pointer returned by ppsvod_get_item_info
 * return:      void
 */
void ppsdwn_free_item_info(ppsvod_iteminfo *piteminfo);


/**************************************************/
/*************PPS Film Searching API***************/
/**************************************************/
/* search type */
#define PPS_SEARCH_NAME      0x00000001
#define PPS_SEARCH_ACTOR     0x00000002
#define PPS_SEARCH_DIRECTOR  0x00000003
#define PPS_SEARCH_AREA      0x00000004
#define PPS_SEARCH_ALL       0x00000005

/* search order */
#define PPS_SEARCH_BITRATE_DOWN 0x00000001
#define PPS_SEARCH_BITRATE_UP   0x00000002
#define PPS_SEARCH_SCORE_DOWN   0x00000003
#define PPS_SEARCH_SCORE_UP     0x00000004
#define PPS_SEARCH_DATE_DOWN    0x00000005
#define PPS_SEARCH_DATE_UP      0x00000006
#define PPS_SEARCH_PINYIN_DOWN  0x00000007
#define PPS_SEARCH_PINYIN_UP    0x00000008
#define PPS_SEARCH_HOT_DOWN     0x00000009
#define PPS_SEARCH_HOT_UP       0x0000000A

/*
 *  description: search pps channels.
 *  param:       datatype     param name      in/out     comment
 *               char*        keyword         in         keyword
 *               int          search_type     in         see macros above
 *               int          search_order    in         see macros above
 *               int          page_size       in         channels per page
 *               int          page_index      in         which page to get(start from 1)
 *  return:      ppschannel pointer of channels, null if failed.
 *  		 int*         search_filmtotal_ret  out  return value for search film total
*/
ppschannel *ppsvod_search(char *keyword, int search_type, int search_order, int page_size, int page_index, 
			   int* search_filmtotal_ret);

typedef enum PPS_NETINFO_ID
{
	EPPS_NETINFO_ALLPASS = 0,
	EPPS_NETCABLE_UNLINK,		//1, Netcard cable is un-connect
	EPPS_NETCARD_DOWN,		//2, Netcard is DOWN
	EPPS_UNSET_IP,			//3, IP Un-Allocated
	EPPS_UNSET_GATEWAY,		//4, GATEWAY Un-Settting
	EPPS_UNSET_DNS,			//5, DNS Un-Setting
	EPPS_VERIFY_FAILD,		//6, Verify Domain Failed
	EPPS_NETINFO_ERROR = -1,	//-1, NetInfo Internal ERROR
} PPS_NETINFO_ID;

/*
 *  description: search network informations.
 *  param:       datatype     	       param name      in/out     comment
 *              const char *           oem_name        in
 *              const char *           terminal_type   in
 *              const char *           device_id       in
 *  return:      Network status as enum type pps_netinfo_id, EPPS_NETINFO_ERROR if internal failed.
*/
PPS_NETINFO_ID ppsnet_get_ipinfo(const char* oem_name, const char* terminal_type, const char* device_id);

#ifdef __cplusplus
}
#endif
#endif	// __PPS_H__
