#ifndef EMSCORE_H
#define EMSCORE_H

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

typedef int32 (*ems_event_listener)(int32 index, ems_event event); 
int32 ems_init( const char8 *config);

int32 ems_uninit(void);

int32 ems_event_listener_func(ems_event_listener listener);

int32 ems_add_task(char8 *url, int32 task_mode, char8 *save_path);

int32 ems_del_task(int32 index);

int32 ems_start_task(int32 index, const char8 *paycode);

int32 ems_stop_task(int32 index);

int32 ems_readdata(int32 index, uchar8 *buffer, uint32 length, uint64 position, struct timeval* time_out);

int32 ems_get_task_status(int32 index, ems_task_info *info);

int32 ems_payment(const char8* paycode, const char8* password);

int32 ems_get_account_info(int32* account_type, char8 *valid_date);

int32 ems_check_netstat(const char8* manufacturer, const char8* model_number,
		                const char8* unique_id, int32* netstat);

typedef struct __emscategory
{
	int    id;                       /* category or subcategory id */
	char  *name;                     /* category or subcategory num */
	int    type;                     /* subcategory type. 0: movie, 1: teleplay */
	int    subcatnum;                /* subcategory number under this category */
	struct __emscategory *psubcat;   /* subcategory array */

	struct __emscategory *next;
} emscategory;

typedef	struct __emsitem
{
	int    id;                       /* Item id */
	int    index;                    /* Item index */
	int    size;                     /* Item size (MB) */
	int    duration;                 /* Item duration (Minute)*/
	char  *format;                   /* file format (rm/rmvb/wmv) */
	int    bitrate;                  /* bitrate */
	char  *emsurl;                   /* ems vod url */
	int    vipflag;                  /* vip flags */
	struct __emsitem *next;
} emsitem;

typedef struct __emschannel
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
	struct __emsitem *pitems;       /* point to items list */
	
	struct __emschannel *next;
}emschannel;


int32 ems_plist_init(int lang);
int32 ems_plist_uninit(void);

emscategory *ems_plist_category(int *catnum, int lang);

emschannel *ems_plist_channel(int catid, int subcatid, int page_size, int page_num, int lang);

emschannel *ems_plist_search(char *keyword, int search_type, int search_order, 
		int page_size, int page_index, int *search_filmtotal_ret, int lang);

#ifdef __cplusplus
}
#endif

#endif /* ifndef EMSCORE_H */

