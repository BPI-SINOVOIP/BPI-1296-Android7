#ifndef EMS_H
#define EMS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char               char8;
typedef unsigned char      uchar8;
typedef int                int32;
typedef unsigned int       uint32;
typedef long long          int64;
typedef unsigned long long uint64;

typedef struct ems_event
{
    uint32         event_id;
    uint32         param;
    uint32         param_extra;
    uchar8         payload[8];
} ems_event;

typedef struct ems_task_info
{
    uint32          down_speed;      // download speed (B/s)
    uint32          up_speed;        // upload speed (B/s)
    uint64          file_size;       // total file size
    uint32          percent;         // download percent 
} ems_task_info;

/***********************
 *  Playback API       *
 ***********************/
typedef int32 (*ems_event_listener)(int32 index, ems_event event); 

/*
 * Description: Init ems library.
 * param:       Datatype            Param name      In/Out     Comment
 *              const char8*        manufacturer    In         manufacturer name
 *              const char8*        model_number    In         platform model name
 *              const char8*        unique_id       In         unique id
 *
 * return:      int32    
 *              return  0    if success; 
 *                      <0   if failed.
 */
int32 ems_init(const char8 *manufacturer,
            const char8 *model_number,
            const char8 *unique_id);


/*
 * Description: Uninit ems library.
 * param:       Datatype     Param name      In/Out     Comment
 *              none
 *
 * return:      int32    
 *              return  0    if success; 
 *                      <0   if failed.
 */
int32 ems_uninit(void);


/*
 * Description: Registe ems library event listener
 * param:       Datatype              Param name      In/Out     Comment
 *              ems_event_listener    listener        In         Function to deal with ems event
 *
 * return:      int32    
 *              return  0     if success; 
 *                      <0    if failed.
 */
int32 ems_event_listener_func(ems_event_listener listener);


/*
 * Description: Add a url as a task 
 * param:       Datatype         Param name      In/Out     Comment
 *              char8*           url             In         Url add as task 
 *              int32            task_mode       In         0: VOD mode, 1: Download mode, 2: VOD + Download
 *              char8*           save_path       In         When in Download mode, it's the path to save file.
 *
 * return:      int32    
 *              return the task index, which can be use by ems_del_task, ems_start_task, ems_stop_task... 
 *                     <0  failed
 */
int32 ems_add_task(char8 *url, int32 task_mode, char8 *save_path);



/*
 * Description: Del  a task by index
 * param:       Datatype         Param name      In/Out     Comment
 *              int32            index           In         task index 
 *
 * return:      int32    
 *              return  0    if success; 
 *                      <0   if failed.
 */
int32 ems_del_task(int32 index);


/*
 * Description: Start play the index's task.
 * param:       Datatype           Param name      In/Out     Comment
 *              int32              index           In         task index
 *              const char8*       paycode         In         pay valid code 
 *
 * return:      int32
 *              return  0    if success; 
 *                      -1   generic error
 *                      -2   can not access to server
 *                      -3   authentication failed
 */
int32 ems_start_task(int32 index, const char8 *paycode);


/*
 * Description: Stop the playing the index's task. 
 * param:       Datatype     Param name      In/Out     Comment
 *              int32        index           In         task index 
 *
 * return:      int32
 *              return  0    if success; 
 *                      <0   if failed.
 */
int32 ems_stop_task(int32 index);


/*
 * Description: read data.
 * param:       Datatype         Param name      In/Out     Comment
 *              int32            index             In       task index
 *              uchar *          buffer            Out      buffer to store data
 *              uint32           length            In       data length
 *              uint64           position          In       from where to read.
 *              struct timeval*  time_out          In       time length that read() can block
 *
 * return:      int32    
 *              return length of data if success; 
 *                      0: EOF;
 *                     -1: read failed(errno is set to EAGAIN if data is temporarily unavailable; 
 *                     etherwise, some error happens or EOF reaches). 
 */
int32 ems_readdata(int32 index, 
                uchar8 *buffer, 
                uint32 length, 
                uint64 position, 
                struct timeval *time_out);

/*
 * Description: transfer timestamp to file offset.
 * param:       Datatype        Param name      In/Out     Comment
 *              int32           index             In       task index
 *              int64           timestamp         In       timestamp need to transfer(milliseconds).
 *              int64           offset            Out      file offset after transfer.
 *
 * return:      int32
 *              return  0    if success; 
 *                     -1    if failed
 */
int32 ems_pfv_timestamp2offset(int32 index, int64 timestamp, int64 *offset);

/*
 * Description: Use to quit readdata function immediately. 
 * param:       Datatype     Param name      In/Out     Comment
 *              int32        index           In         task index 
 *
 * return:      int32
 *              return  0    if success; 
 *                     -1     if failed.
 */
int32 ems_drop_read(int32 index);


/*
 * Description: Get information for a task.
 * param:       Datatype         Param name      In/Out     Comment
 *              int32            index           In         task index 
 *              ems_task_info*   info            Out        task info [memory should alloced by caller]
 *
 * return:      int32 
 *              return  0    if success; 
 *                     -1    if failed.
 */
int32 ems_get_task_status(int32 index, ems_task_info *info);

/*
 * Description: Get error string. This API is used when ems_start_task returns -8.
 * param:       Datatype         Param name      In/Out     Comment
 *              none
 *
 * return:      const char*
 *              return  error string if there is error; 
 *                      empty string if everything is OK
 *                      NULL if failed
 */
const char *ems_get_err_string(void);


/***********************
 *  Payment API        *
 ***********************/
/*
 * Description: Check the permission to play the film
 * param:       Datatype          Param name      In/Out     Comment
 *              const char8*      manufacturer     In         
 *              const char8*      model_number     In        
 *              const char8*      unique_id        In
 *              uint32            film_id          In        
 *
 * return:      int32        
 *              return 0 success, 
 *                    -1 generic error
 *                    -2 invalid user
 *                    -3 film not found
 *                    -4 not enough points in user's account
 *                    -5 server error
 * comment:     
 */
int32 ems_task_verify (const char8 *manufacturer, 
                  const char8 *model_number, 
                  const char8 *unique_id, 
                  uint32 film_id);


/*
 * Description: Deposit money for EMS VIP account
 * param:       Datatype          Param name      In/Out     Comment
 *              const char8*      manufacturer     In         
 *              const char8*      model_number     In        
 *              const char8*      unique_id        In
 *              const char8*      paycode          In        17 card numbers
 *              const char8*      password         In        17 card password
 *
 * return:      int32        
 *              return 0 success, 
 *                    -1 generic error
 *                    -2 invalid card
 *                    -3 card already used
 *                    -4 card beyond the expire date
 * comment:     
 */
int32 ems_payment(const char8 *manufacturer, 
                  const char8 *model_number, 
                  const char8 *unique_id,
                  const char8 *paycode, 
                  const char8 *password);


/*
 * Description: Get account info of current user
 * param:       Datatype          Param name      In/Out     Comment
 *              const char8*      manufacturer     In         
 *              const char8*      model_number     In        
 *              const char8*      unique_id        In
 *              int32*            account_points   Out       remaining points in user's account
 *              char8*            valid_date       Out       If user buy period-paid packages, this 
 *                                                           param indicates the valid date
 *                                                           [memory should alloced by caller]
 *
 * return:      int32        
 *              return 0  success, 
 *                     <0 generic error
 * comment:     
 */
int32 ems_get_account_info(const char8 *manufacturer, 
                  const char8 *model_number, 
                  const char8 *unique_id,
                  int32 *account_points, 
                  char8 *valid_date);


/*
 * Description: Get package info from server
 * param:       Datatype          Param name      In/Out     Comment
 *              const char8*      manufacturer     In         
 *              const char8*      model_number     In        
 *              const char8*      unique_id        In
 *              char8*            package_info     Out       returned package info, in the following format:
 *                                                           "month|quarter|half-year|year".eg:"100|300|600|1200"
 *                                                           [memory should alloced by caller]
 *
 * return:      int32        
 *              return 0  success, 
 *                     <0 generic error
 * comment:     
 */
int32 ems_get_package_info(const char8 *manufacturer, 
                  const char8 *model_number, 
                  const char8 *unique_id, 
                  char8 *package_info);


/*
 * Description: buy a package for some period service
 * param:       Datatype          Param name      In/Out     Comment
 *              const char8*      manufacturer     In         
 *              const char8*      model_number     In        
 *              const char8*      unique_id        In
 *              int32             package_type     In        package type: 1: month; 2: quarter; 3: half year
 *                                                           4: year
 *
 * return:      int32        
 *              return 0  success, 
 *                     -1 generic error
 *                     -2 user invalid
 *                     -3 package invalid
 *                     -4 not enough points in user's account
 *                     -5 server error
 * comment:     
 */
int32 ems_set_package (const char8 *manufacturer, 
                  const char8 *model_number, 
                  const char8 *unique_id, 
                  int32 package_type);


/************************
 * Network Checking API *
 ************************/
typedef enum EMS_NETINFO_ID
{
    EMS_NETINFO_ALLPASS = 0,
    EMS_NETIF_INVALID   = 0x80000001,        //1, Net Interface is invalid
    EMS_NETCABLE_UNLINK,                     //2, Netcard cable is un-connect
    EMS_NETCARD_DOWN,                        //3, Netcard is DOWN
    EMS_UNSET_IP,                            //4, IP Un-Allocated
    EMS_UNSET_GATEWAY,                       //5, GATEWAY Un-Settting
    EMS_UNSET_DNS,                           //6, DNS Un-Setting
    EMS_SERVER_UNREACHABLE1,                 //7, XML server unreachable
    EMS_SERVER_UNREACHABLE2,                 //8, Verify server unreachable
    EMS_NETINFO_ERROR = -1,                  //-1, NetInfo Internal ERROR
} EMS_NETSTAT_ID;

/*
 * Description: Get network status
 * param:       Datatype          Param name      In/Out     Comment
 *              const char8*      manufacturer     In         
 *              const char8*      model_number     In        
 *              const char8*      unique_id        In
 *              const char8*      ifname           In        interface name
 *              int32*            netstat          Out       memory alloced by caller,defined by EMS_NETSTAT_ID
 *
 * return:      int32        
 *              return 0  success, 
 *                     <0 generic error
 * comment:     
 */
int32 ems_check_netstat(const char8 *manufacturer, 
                    const char8 *model_number, 
                    const char8 *unique_id, 
                    const char8 *ifname, 
                    int32 *netstat);


/*************************
 * Playlist Retrieve API *
 *************************/
typedef struct __emscategory
{
        int32    id;                       /* category or subcategory id */
        char8   *name;                     /* category or subcategory name */
        int32    type;                     /* subcategory type. 0: movie, 1: teleplay */
        int32    subcatnum;                /* subcategory number under this category */
        struct __emscategory *psubcat;     /* subcategory array */

        struct __emscategory *next;
} emscategory;

typedef struct __emsitem
{
        int32    id;                       /* Item id */
        int32    index;                    /* Item index */
        int32    size;                     /* Item size (MB) */
        int32    duration;                 /* Item duration (Minute)*/
        char8   *format;                   /* file format (rm/rmvb/wmv) */
        int32    bitrate;                  /* bitrate */
        char8   *emsurl;                   /* ems vod url */
        int32    vipflag;                  /* vip flags */
	int32    points;		   /* charge points*/
        struct __emsitem *next;
} emsitem;

typedef struct __emschannel
{
        char8 *name;                     /* channel name */
        char8 *director;
        char8 *actor;
        char8 *area;
        int32  size;                     /* all items' total size under this channel (MB) */
        char8 *pubtime;                  /* publish time */
        int32  duration;                 /* all items' total duration time under this channel (Min) */
        char8 *lang;                     /* channel language */
        char8 *desc;                     /* channel description */
        char8 *BImgUrl;                  /* Big Image Url */
        char8 *SImgUrl;                  /* Small Image Url */
        int32  itemnum;                  /* num of items */
        struct __emsitem *pitems;        /* point32 to items list */

        struct __emschannel *next;
}emschannel;

/*
 *  description: vod list initialization, must be called before all.
 *  param:       datatype     param name      in/out     comment
 *               void
 *  return:      int    return 0 if success; -1 if failed.
*/
int32 ems_plist_init(int lang);

/*
 *  description: vod list uninitialize, call this fuction when you want to exit pps vodlist.
 *  param:       datatype     param name      in/out     comment
 *               void
 *  return:      int    return 0 if success; -1 if failed.
*/
int ems_plist_uninit(void);

/*
 *  description: get all categories and sub categories list.
 *  param:       datatype     param name      in/out     comment
 *               void
 *  return:      ppscategory* pointer of all categories, null if failed.
*/
emscategory *ems_plist_category(int *catnum, int lang);

/*
 *  description: get channels under pointed catogory and sub category.
 *  param:       datatype     param name      in/out     comment
 *               int            catid            in        class id
 *               int            subcatid        in        sub class id
 *               int            page_size        in        channels one page have
 *               int            page_num        in        page num which want to query
 *  return:      ppschannel        pointer of channels, null if failed.
*/
emschannel *ems_plist_channel(int catid, int subcatid, int page_size, int page_num, int lang);

/**************************************************/
/*************TVOD Film Searching API***************/
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
#
emschannel *ems_plist_search(char *keyword, int search_type, int search_order, 
        int page_size, int page_index, int *search_filmtotal_ret, int lang);


#ifdef __cplusplus
}
#endif

#endif /* ifndef EMS_H */

