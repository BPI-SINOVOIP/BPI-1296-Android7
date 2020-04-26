#ifndef ET_TASK_MANAGER_H_200909081912
#define ET_TASK_MANAGER_H_200909081912
/*-------------------------------------------------------*/
/*                       IDENTIFICATION                             	*/
/*-------------------------------------------------------*/
/*     Filename  : et_task_manager.h                                	*/
/*     Author    : Zeng Yuqing                                      	*/
/*     Project   : EmbedThunderManager                              	*/
/*     Version   : 1.5 								    	*/
/*-------------------------------------------------------*/
/*                  Shenzhen XunLei Networks			          	*/
/*-------------------------------------------------------*/
/*                  - C (copyright) - www.xunlei.com -		    	*/
/*                                                                          		      	*/
/*      This document is the proprietary of XunLei                  	*/
/*                                                                          			*/
/*      All rights reserved. Integral or partial reproduction         */
/*      prohibited without a written authorization from the           */
/*      permanent of the author rights.                               */
/*                                                                          			*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*                  FUNCTIONAL DESCRIPTION                           */
/*-------------------------------------------------------*/
/* This file contains the interfaces of EmbedThunderTaskManager       */
/*-------------------------------------------------------*/

/*-------------------------------------------------------*/
/*                       HISTORY                                     */
/*-------------------------------------------------------*/
/*   Date     |    Author   | Modification                            */
/*-------------------------------------------------------*/
/* 2009.09.08 |ZengYuqing  | Creation                                 */
/* 2010.11.11 |ZengYuqing  | Update to 1.5                                 */
/*-------------------------------------------------------*/

#ifdef __cplusplus
extern "C" 
{
#endif

/************************************************************************/
/*                    TYPE DEFINE                    */
/************************************************************************/
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef char				int8;
typedef short				int16;
typedef int					int32;
#if defined(LINUX) || defined( __SYMBIAN32__)
	typedef unsigned long long	uint64;
	typedef long long			int64;
#else
	#ifdef  NOT_SUPPORT_LARGE_INT_64
		typedef unsigned int	uint64;
		typedef int			__int64;
	#else
        #if defined(WINCE)
		typedef unsigned __int64 uint64;
		//typedef long long			__int64;
         #else
		typedef unsigned long	long uint64;
		typedef long long			__int64;
         #endif
	#endif
#endif

#if defined(LINUX) || defined( __SYMBIAN32__)
#ifndef NULL
#define NULL	((void*)(0))
#endif
#endif

#if defined( __SYMBIAN32__)
#include <e32def.h>
#ifdef ETDLL_API
#undef ETDLL_API
#endif
#define ETDLL_API IMPORT_C
#elif defined( WIN32)
  #ifdef WINCE
	#define ETDLL_API
  #else
	#ifdef ETDLL_EXPORTS
	#define ETDLL_API __declspec(dllexport)
	#else
	#define ETDLL_API __declspec(dllimport)
	#endif
  #endif
#else
	#define ETDLL_API
#endif


#ifndef TRUE
typedef int32 Bool;
typedef int32 BOOL;
#define TRUE	(1)
#define FALSE	(0)
#else
typedef int32 Bool;
#endif


/************************************************************************/
/*                    STRUCT DEFINE                 */
/************************************************************************/

#define ETM_MAX_FILE_NAME_LEN (256)		/* 文件名最大长度 */
#define ETM_MAX_FILE_PATH_LEN (256)		/* 文件路径(不包括文件名)最大长度 */
#define ETM_MAX_TD_CFG_SUFFIX_LEN (8)		/* 迅雷下载临时文件的后缀最大长度 */

/************************************************************************/

/* 任务类型*/
typedef enum t_etm_task_type
{
	ETT_URL = 0, 				/* 用URL 创建的任务,支持协议: http://,https://,ftp://,thunder:// */
	ETT_BT, 					/* 用.torrent 文件创建的任务 */
	ETT_TCID,  					/* 用迅雷的TCID 创建的任务  */
	ETT_KANKAN, 				/* 用迅雷的TCID,File_Size,和GCID一起创建的任务 ,注意 这种类型的任务不会自动停止，需要调用etm_stop_vod_task才会停止*/
	ETT_EMULE, 					/* 用电驴链接创建的任务  								 */
	ETT_FILE 
} ETM_TASK_TYPE;

/* 任务状态 */
typedef enum t_etm_task_state
{
	ETS_TASK_WAITING=0, 
	ETS_TASK_RUNNING, 
	ETS_TASK_PAUSED,
	ETS_TASK_SUCCESS, 
	ETS_TASK_FAILED, 
	ETS_TASK_DELETED 
} ETM_TASK_STATE;

/* BT 任务的字符编码设置 */
typedef enum t_etm_encoding_mode
{ 
	EEM_ENCODING_NONE = 0, 				/*  返回原始字段*/
	EEM_ENCODING_GBK = 1,					/*  返回GBK格式编码 */
	EEM_ENCODING_UTF8 = 2,				/*  返回UTF-8格式编码 */
	EEM_ENCODING_BIG5 = 3,				/*  返回BIG5格式编码  */
	EEM_ENCODING_UTF8_PROTO_MODE = 4,	/*  返回种子文件中的utf-8字段  */
	EEM_ENCODING_DEFAULT = 5				/*  未设置输出格式(使用etm_set_default_encoding_mode的全局输出设置)  */
}ETM_ENCODING_MODE ;

/* 正在运行中的任务的实时状况 */
typedef struct t_etm_running_status
{
	ETM_TASK_TYPE  _type;   
	ETM_TASK_STATE  _state;   
	uint64 _file_size; 					/* 任务文件大小*/  
	uint64 _downloaded_data_size; 	/* 已下载的数据大小*/  
	u32 _dl_speed;  					/* 实时下载速率*/  
	u32 _ul_speed;  					/* 实时上传速率*/  
	u32 _downloading_pipe_num; 			/* 正在下载数据的socket 连接数 */
	u32 _connecting_pipe_num; 		/* 正在连接中的socket 连接数 */
}ETM_RUNNING_STATUS;

/* 任务信息 */
typedef struct t_etm_task_info
{
	u32  _task_id;		
	ETM_TASK_STATE  _state;   
	ETM_TASK_TYPE  _type;   

	char _file_name[ETM_MAX_FILE_NAME_LEN];
	char _file_path[ETM_MAX_FILE_PATH_LEN]; 
	uint64 _file_size; 
	uint64 _downloaded_data_size; 		
	 
	u32 _start_time;				/* 任务开始下载的时间，自1970年1月1日开始的秒数 */
	u32 _finished_time;			/* 任务完成(成功或失败)的时间，自1970年1月1日开始的秒数 */

	u32  _failed_code;			/* 如果任务失败的话,这里会有失败码 */
	u32  _bt_total_file_num; 		/* 如果该任务的_type== ETT_BT, 这里是torrent文件中包含的子文件总个数*/
	Bool _is_deleted;			/* 当任务被成功调用etm_delete_task 接口后,它的_state不会为ETS_TASK_DELETED，还是保持原来的值以便它可以被正确还原(etm_recover_task)，用这个值标识任务此时状态为ETS_TASK_DELETED */
	Bool _check_data;			/* 点播时是否需要验证数据 */	
	Bool _is_no_disk;			/* 是否为无盘点播  */
}ETM_TASK_INFO;

/* BT 任务子文件的下载状态 */
typedef enum t_etm_file_status
{
	EFS_IDLE = 0, 
	EFS_DOWNLOADING, 
	EFS_SUCCESS, 
	EFS_FAILED 
} ETM_FILE_STATUS;

/* BT 任务子文件的下载信息 */
typedef struct  t_etm_bt_file
{
	u32 _file_index;
	Bool _need_download;			/* 是否需要下载 */
	ETM_FILE_STATUS _status;
	uint64 _file_size;	
	uint64 _downloaded_data_size;
	u32 _failed_code;
}ETM_BT_FILE;

/* BT 任务子文件的信息 */
typedef struct t_etm_torrent_file_info
{
	u32 _file_index;
	char *_file_name;
	u32 _file_name_len;
	char *_file_path;
	u32 _file_path_len;
	uint64 _file_offset;		/* 该子文件在整个BT 文件中的偏移 */
	uint64 _file_size;
} ETM_TORRENT_FILE_INFO;

/* torrent 文件的信息 */
typedef struct t_etm_torrent_seed_info
{
	char _title_name[ETM_MAX_FILE_NAME_LEN-ETM_MAX_TD_CFG_SUFFIX_LEN];
	u32 _title_name_len;
	uint64 _file_total_size;						/* 所有子文件的总大小*/
	u32 _file_num;								/* 所有子文件的总个数*/
	u32 _encoding;								/* 种子文件编码格式，GBK = 0, UTF_8 = 1, BIG5 = 2 */
	unsigned char _info_hash[20];					/* 种子文件的特征值 */
	ETM_TORRENT_FILE_INFO *file_info_array_ptr;	/* 子文件信息列表 */
} ETM_TORRENT_SEED_INFO;


/* E-Mule链接中包含的文件信息 */
typedef struct tagETM_ED2K_LINK_INFO
{
	char		_file_name[256];
	uint64		_file_size;
	u8		_file_id[16];
	u8		_root_hash[20];
	char		_http_url[512];
}ETM_ED2K_LINK_INFO;


/************************************************************************/

#define	ETM_INVALID_TASK_ID			0


/* 创建下载任务时的输入参数 */
typedef struct t_etm_create_task
{
	ETM_TASK_TYPE _type;				/* 任务类型，必须准确填写*/
	
	char* _file_path; 						/* 任务文件存放路径，必须已存在且可写；如果为NULL ，则表示存放于默认路径中(由etm_set_download_path事先设置) */
	u32 _file_path_len;					/* 任务文件存放路径的长度，必须小于 ETM_MAX_FILE_PATH_LEN */
	char* _file_name;					/* 任务文件名字 */
	u32 _file_name_len; 					/* 任务文件名字的长度，必须小于 ETM_MAX_FILE_NAME_LEN-ETM_MAX_TD_CFG_SUFFIX_LEN */
	
	char* _url;							/* _type== ETT_URL 或者_type== ETT_EMULE 时，这里就是任务的URL */
	u32 _url_len;						/* URL 的长度，必须小于512 */
	char* _ref_url;						/* _type== ETT_URL 时，这里就是任务的引用页面URL ,可为NULL */
	u32 _ref_url_len;						/* 引用页面URL 的长度，必须小于512 */
	
	char* _seed_file_full_path; 			/* _type== ETT_BT 时，这里就是任务的种子文件(*.torrent) 全路径 */
	u32 _seed_file_full_path_len; 		/* 种子文件全路径的长度，必须小于ETM_MAX_FILE_PATH_LEN+ETM_MAX_FILE_NAME_LEN */
	u32* _download_file_index_array; 	/* _type== ETT_BT 时，这里就是需要下载的子文件序号列表，为NULL 表示下载所有有效的子文件*/
	u32 _file_num;						/* _type== ETT_BT 时，这里就是需要下载的子文件个数*/
	
	char *_tcid;							/* _type== ETT_TCID 或_type== ETT_KANKAN  时，这里就是任务文件的迅雷TCID ,为40字节的十六进制数字字符串*/
	uint64 _file_size;						/* _type== ETT_KANKAN  时，这里就是任务文件的大小 */
	char *_gcid;							/* _type== ETT_KANKAN  时，这里就是任务文件的迅雷GCID ,为40字节的十六进制数字字符串*/
	
	Bool _check_data;					/* 任务边下边播时是否需要校验数据  */
	Bool _manual_start;					/* 是否手动开始下载?  TRUE 为用户手动开始下载,FALSE为ETM自动开始下载*/
	
	void * _user_data;					/* 用于存放与该任务相关的用户附加信息，如任务描述，海报，cookie等 。如果是cookie，coockie部分必须以"Cookie:"开头，并以"\r\n" 结束 */
	u32  _user_data_len;					/* 用户数据的长度,必须小于65535 */
} ETM_CREATE_TASK;

/* 创建点播任务时的输入参数 */
typedef struct t_etm_create_vod_task
{
	ETM_TASK_TYPE _type;

	char* _url;
	u32 _url_len;
	char* _ref_url;
	u32 _ref_url_len;

	char* _seed_file_full_path; 
	u32 _seed_file_full_path_len; 
	u32* _download_file_index_array; 	
	u32 _file_num;						/* _type== ETT_BT 或_type== ETT_KANKAN  时，这里就是需要点播的子文件个数										[ 暂不支持ETT_KANKAN 多个文件] */

	char *_tcid;							/* _type== ETT_KANKAN  时，这里是一个或多个子文件(一部电影有可能被分成n个文件) TCID 的数组		[ 暂不支持ETT_KANKAN 多个文件] */
	uint64 _file_size;						/* _type== ETT_KANKAN  时，这里是所有(一个或多个)需要点播的文件的总大小								[ 暂不支持ETT_KANKAN 多个文件] */
	uint64 * _segment_file_size_array;	/* _type== ETT_KANKAN  时，这里可以是多个子文件(一部电影有可能被分成n个文件) file_size 的数组		[ 暂不支持ETT_KANKAN 多个文件] */
	char *_gcid;							/* _type== ETT_KANKAN  时，这里是一个或多个子文件(一部电影有可能被分成n个文件) GCID 的数组		[ 暂不支持ETT_KANKAN 多个文件] */

	Bool _check_data;					/* 是否需要校验数据  */
	Bool _is_no_disk;					/* 是否为无盘点播  */

	void * _user_data;					/* 用于存放与该任务相关的用户附加信息，如任务描述，海报，cookie等 。如果是cookie，coockie部分必须以"Cookie:"开头，并以"\r\n" 结束 */
	u32  _user_data_len;					/* 用户数据的长度,必须小于65535 */
} ETM_CREATE_VOD_TASK;

/* 创建微型任务时的输入参数 (比如上传或下载几KB 的一块数据，一个小文件，一张小图片或网页等)*/
typedef struct t_etm_mini_task
{
	char* _url;					/* 只支持"http://" 开头的url  */
	u32 _url_len;
	
	Bool _is_file;					/* 是否为文件，TRUE为_file_path,_file_path_len,_file_name,_file_name_len有效,_data和_data_len无效；FALSE则相反*/
	
	char _file_path[ETM_MAX_FILE_PATH_LEN]; 				/* _is_file=TRUE时, 需要上传或下载的文件存储路径，必须真实存在 */
	u32 _file_path_len;			/* _is_file=TRUE时, 需要上传或下载的文件存储路径的长度*/
	char _file_name[ETM_MAX_FILE_NAME_LEN];			/* _is_file=TRUE时, 需要上传或下载的文件名*/
	u32 _file_name_len; 			/* _is_file=TRUE时, 需要上传或下载的文件名长度*/
	
	u8 * _send_data;			/* _is_file=FALSE时,指向需要上传的数据*/
	u32  _send_data_len;			/* _is_file=FALSE时,需要上传的数据长度 */
	
	void * _recv_buffer;			/* 用于接收服务器返回的响应数据的缓存，下载时如果_is_file=TRUE,此参数无效*/
	u32  _recv_buffer_size;		/* _recv_buffer 缓存大小,至少要16384 byte !下载时如果_is_file=TRUE,此参数无效 */
	
	char * _send_header;			/* 用于发送http头部 */
	u32  _send_header_len;		/* _send_header的大小	 */
	
	void *_callback_fun;			/* 任务完成后的回调函数 : typedef int32 ( *ETM_NOTIFY_MINI_TASK_FINISHED)(void * user_data, int32 result,void * recv_buffer,u32  recvd_len,void * p_header,u8 * send_data); */
	void * _user_data;			/* 用户参数 */
	u32  _timeout;				/* 超时,单位秒,等于0时180秒超时*/
	u32  _mini_id;				/* id */
	Bool  _gzip;					/* 是否接受或发送压缩文件 */
} ETM_MINI_TASK;

/************************************************************************/
/*  Interfaces provided by EmbedThunderTaskManager	*/
/************************************************************************/
/////1 初始化与反初始化相关接口

/////1.1 初始化，etm_system_path 为ETM 用于存放系统数据的目录，必须已存在且可写，最小要有1M 的空间，不能在可移动的存储设备中；path_len 必须小于ETM_MAX_FILE_PATH_LEN
ETDLL_API int32 etm_init(const char *etm_system_path,u32  path_len);

/////1.2 反初始化
ETDLL_API int32 etm_uninit(void);

/////1.1 有外部设备插入时可调用该接口初始化下载任务，etm_system_path 为ETM 用于存放系统数据的目录，必须已存在且可写，最小要有1M 的空间，不能在可移动的存储设备中；path_len 必须小于ETM_MAX_FILE_PATH_LEN
ETDLL_API int32 etm_load_tasks(const char *etm_system_path,u32  path_len);

/////U盘拔出时调用该接口卸载掉所有任务，但是保留无盘点播任务
ETDLL_API int32 etm_unload_tasks(void);


/////1.3 网络相关接口
/*
网络连接类型：
(低16位) */
#define UNKNOWN_NET   	0x00000000
#define CHINA_MOBILE  	0x00000001
#define CHINA_UNICOM  	0x00000002
#define CHINA_TELECOM 	0x00000004
#define OTHER_NET     	0x00008000
/* (高16位) */
#define NT_GPRS_WAP   	0x00010000
#define NT_GPRS_NET   	0x00020000
#define NT_3G         		0x00040000
#define NT_WLAN       		0x00080000   // wifi and lan ...

#define NT_CMWAP 		(NT_GPRS_WAP|CHINA_MOBILE)
#define NT_CMNET 		(NT_GPRS_NET|CHINA_MOBILE)

#define NT_CUIWAP 		(NT_GPRS_WAP|CHINA_UNICOM)
#define NT_CUINET 		(NT_GPRS_NET|CHINA_UNICOM)

typedef int32 ( *ETM_NOTIFY_NET_CONNECT_RESULT)(u32 iap_id,int32 result,u32 net_type);
typedef enum t_etm_net_status
{
	ENS_DISCNT = 0, 
	ENS_CNTING, 
	ENS_CNTED 
} ETM_NET_STATUS;

ETDLL_API int32 etm_set_network_cnt_notify_callback( ETM_NOTIFY_NET_CONNECT_RESULT callback_function_ptr);
ETDLL_API int32 etm_init_network(u32 iap_id);
ETDLL_API int32 etm_uninit_network(void);
ETDLL_API ETM_NET_STATUS etm_get_network_status(void);
ETDLL_API int32 etm_get_network_iap(u32 *iap_id);
ETDLL_API int32 etm_get_network_iap_from_ui(u32 *iap_id);
ETDLL_API const char* etm_get_network_iap_name(void);
ETDLL_API int32 etm_get_network_flow(u32 * download,u32 * upload);
ETDLL_API const char* etm_get_peerid(void);

///////////////////////////////////////////////////////////////
/////2 系统设置相关接口

/////2.1  设置用户自定义的底层接口,这个接口必须在调用etm_init 之前调用!
#define ET_FS_IDX_OPEN           (0)
#define ET_FS_IDX_ENLARGE_FILE   (1)
#define ET_FS_IDX_CLOSE          (2)
#define ET_FS_IDX_READ           (3)
#define ET_FS_IDX_WRITE          (4)
#define ET_FS_IDX_PREAD          (5)
#define ET_FS_IDX_PWRITE         (6)
#define ET_FS_IDX_FILEPOS        (7)
#define ET_FS_IDX_SETFILEPOS     (8)
#define ET_FS_IDX_FILESIZE       (9)
#define ET_FS_IDX_FREE_DISK      (10)
#define ET_SOCKET_IDX_SET_SOCKOPT (11)
#define ET_MEM_IDX_GET_MEM           (12)
#define ET_MEM_IDX_FREE_MEM          (13)
#define ET_ZLIB_UNCOMPRESS          (14)

/* 参数列表说明：
 * 	int32 fun_idx    接口函数的序号
 * 	void *fun_ptr    接口函数指针
 *
 *
 *  目前支持的接口函数序号以及相对应的函数类型说明：
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		0 (ET_FS_IDX_OPEN)      
 *  类型定义：	typedef int32 (*et_fs_open)(char *filepath, int32 flag, u32 *file_id);
 *  说明：		打开文件，需要以读写方式打开。成功时返回0，否则返回错误码
 *  参数说明：
 *	 filepath：需要打开文件的全路径； 
 *	 flag：    当(flag & 0x01) == 0x01时，文件不存在则创建文件，否则文件不存在时打开失败
                                                                       如果文件存在则以读写权限打开文件
                      当(flag & 0x02) == 0x02时，表示打开只读文件
                      当(flag & 0x04) == 0x04时，表示打开只写
                      当(flag ) = 0x 0时，表示读写权限打开文件
 *	 file_id： 文件打开成功，返回文件句柄
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		1 (ET_FS_IDX_ENLARGE_FILE)   
 *  类型定义：	typedef int32 (*et_fs_enlarge_file)(u32 file_id, uint64 expect_filesize, uint64 *cur_filesize);
 *  说明：		重新改变文件大小（目前只需要变大）。一般用于打开文件后，进行预创建文件。 成功返回0，否则返回错误码
 *  参数说明：
 *   file_id：需要更改大小的文件句柄
 *   expect_filesize： 希望更改到的文件大小
 *   cur_filesize： 实际更改后的文件大小（注意：当设置大小成功后，一定要正确设置此参数的值!）
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		2 (ET_FS_IDX_CLOSE)   
 *  类型定义：	typedef int32 (*et_fs_close)(u32 file_id);
 *  说明：		关闭文件。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id：需要关闭的文件句柄
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		3 (ET_FS_IDX_READ)   
 *  类型定义：	typedef int32 (*et_fs_read)(u32 file_id, char *buffer, int32 size, u32 *readsize);
 *  说明：		读取当前文件指针文件内容。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id： 需要读取的文件句柄
 *   buffer：  存放读取内容的buffer指针
 *   size：    需要读取的数据大小（调用者可以保证不会超过buffer的大小）
 *   readsize：实际读取的文件大小（注意：文件读取成功后，一定要正确设置此参数的值!）
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		4 (ET_FS_IDX_WRITE)   
 *  类型定义：	typedef int32 (*et_fs_write)(u32 file_id, char *buffer, int32 size, u32 *writesize);
 *  说明：		从当前文件指针处写入内容。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id：  需要写入的文件句柄
 *   buffer：   存放写入内容的buffer指针
 *   size：     需要写入的数据大小（调用者可以保证不会超过buffer的大小）
 *   writesize：实际写入的文件大小（注意：文件写入成功后，一定要正确设置此参数的值!）
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		5 (ET_FS_IDX_PREAD)   
 *  类型定义：	typedef int32 (*et_fs_pread)(u32 file_id, char *buffer, int32 size, uint64 filepos, u32 *readsize);
 *  说明：		读取指定偏移处的文件内容。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id： 需要读取的文件句柄
 *   buffer：  存放读取内容的buffer指针
 *   size：    需要读取的数据大小（调用者可以保证不会超过buffer的大小）
 *   filepos： 需要读取的文件偏移
 *   readsize：实际读取的文件大小（注意：文件读取成功后，一定要正确设置此参数的值!）
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		6 (ET_FS_IDX_PWRITE)   
 *  类型定义：	typedef int32 (*et_fs_pwrite)(u32 file_id, char *buffer, int32 size, uint64 filepos, u32 *writesize);
 *  说明：		从指定偏移处写入内容。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id：  需要写入的文件句柄
 *   buffer：   存放写入内容的buffer指针
 *   size：     需要写入的数据大小（调用者可以保证不会超过buffer的大小）
 *   filepos：  需要读取的文件偏移
 *   writesize：实际写入的文件大小（注意：文件写入成功后，一定要正确设置此参数的值!）
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		7 (ET_FS_IDX_FILEPOS)   
 *  类型定义：	typedef int32 (*et_fs_filepos)(u32 file_id, uint64 *filepos);
 *  说明：		获得当前文件指针位置。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id： 文件句柄
 *   filepos： 从文件头开始计算的文件偏移
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		8 (ET_FS_IDX_SETFILEPOS)   
 *  类型定义：	typedef int32 (*et_fs_setfilepos)(u32 file_id, uint64 filepos);
 *  说明：		设置当前文件指针位置。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id： 文件句柄
 *   filepos： 从文件头开始计算的文件偏移
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		9 (ET_FS_IDX_FILESIZE)   
 *  类型定义：	typedef int32 (*et_fs_filesize)(u32 file_id, uint64 *filesize);
 *  说明：		获得当前文件大小。成功返回0，否则返回错误码
 *  参数说明：
 *   file_id： 文件句柄
 *   filepos： 当前文件大小
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		10 (ET_FS_IDX_FREE_DISK)   
 *  类型定义：	typedef int32 (*et_fs_get_free_disk)(const char *path, u32 *free_size);
 *  说明：		获得path路径所在磁盘的剩余空间，一般用作是否可以创建文件的判断依据。成功返回0，否则返回错误码
 *  参数说明：
 *   path：     需要获取剩余磁盘空间磁盘上的任意路径
 *   free_size：指定路径所在磁盘的当前剩余磁盘空间（注意：此参数值单位是 KB(1024 bytes) !）
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		11 (ET_SOCKET_IDX_SET_SOCKOPT)   
 *  类型定义：	typedef int32 (*et_socket_set_sockopt)(u32 socket, int32 socket_type); 
 *  说明：		设置socket的相关参数，目前只支持协议簇PF_INET。成功返回0，否则返回错误码
 *  参数说明：
 *   socket：     需要设置的socket句柄
 *   socket_type：此socket的类型，目前有效的值有2个：SOCK_STREAM  SOCK_DGRAM。这2个宏的取值和所在的OS一致。
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		12 (ET_MEM_IDX_GET_MEM)  
 *  类型定义：	typedef int32 (*et_mem_get_mem)(u32 memsize, void **mem);
 *  说明：		从操作系统分配固定大小的连续内存块。成功返回0，否则返回错误码
 *  参数说明：
 *   memsize：     需要分配的内存大小
 *   mem： 成功分配之后，内存块首地址放在*mem中返回。
 *------------------------------------------------------------------------------------------------------------------
 *  序号：		13 (ET_MEM_IDX_FREE_MEM)   
 *  类型定义：	typedef int32 (*et_mem_free_mem)(void* mem, u32 memsize);
 *  说明：		释放指定内存块给操作系统。成功返回0，否则返回错误码
 *  参数说明：
 *   mem：     需要释放的内存块首地址
 *   memsize：需要释放的内存块的大小
 *------------------------------------------------------------------------------------------------------------------ 
 *  序号：		14 (ET_ZLIB_UNCOMPRESS)   
 *  类型定义：	typedef _int32 (*et_zlib_uncompress)( unsigned char *p_out_buffer, int *p_out_len, const unsigned char *p_in_buffer, int in_len );
 *  说明：		指定zlib库的解压缩函数进来,便于kad网络中找源包的解压,提高emule找源数量
 *  参数说明：
 *   p_out_buffer：解压后数据缓冲区
 *   p_out_len：   解压后数据长度
 *   p_in_buffer： 待解压数据缓冲区
 *   in_len：      待解压数据长度
 *------------------------------------------------------------------------------------------------------------------ */
ETDLL_API int32 etm_set_customed_interface(int32 fun_idx, void *fun_ptr);
	
/////2.2 获取版本号,其中前部分为ETM 的版本号，后一部分为下载库ET 的版本号
ETDLL_API const char* etm_get_version(void);

/////2.3 注册license
/*目前提供仅用于测试的免费license:
*				License						    |  序号 |		有效期		| 状态
*	09072400010000000000009y4us41bxk5nsno35569 | 0000000 |2009-07-24~2010-07-24	| 00
*/
ETDLL_API int32 	etm_set_license(const char *license, int32 license_size);
ETDLL_API const char* etm_get_license(void);

/////2.4 获取license 的验证结果,接口返回码和接口参数*result 均等于0 为验证通过,其他值解释如下

/*  如果该接口返回值为102406，表示ETM正在与迅雷服务器交互中，需要在稍后再调用此接口。
*    在接口返回值为0的前提下,接口参数*result的值解释如下:
*    4096   表示通讯错误，原因可能是网络问题或服务器坏掉了。
*    4118   表示ETM在尝试了12次（每次间隔1小时）license上报之后，均告失败，原因同4096。
*    21000 表示 LICENSE 验证未通过，原因是 LICENSE 被冻结。
*    21001 表示 LICENSE 验证未通过，原因是 LICENSE 过期。
*    21002 表示 LICENSE 验证未通过，原因是 LICENSE 被回收。
*    21003 表示 LICENSE 验证未通协，原因是 LICENSE 被重复使用。
*    21004 表示 LICENSE 验证未通协，原因是 LICENSE 是虚假的。
*    21005 表示 服务器繁忙，ETM 会在一小时后自动重试,界面程序不必理会。
*/
ETDLL_API int32 	etm_get_license_result(u32 * result);

/////2.5 设置解释.torrent文件时系统默认字符编码格式，encoding_mode 不能为EEM_ENCODING_DEFAULT
ETDLL_API int32 etm_set_default_encoding_mode( ETM_ENCODING_MODE encoding_mode);
ETDLL_API ETM_ENCODING_MODE etm_get_default_encoding_mode(void);

/////2.6 设置默认下载存储路径,必须已存在且可写,path_len 必须小于ETM_MAX_FILE_PATH_LEN
ETDLL_API int32 etm_set_download_path(const char *path,u32  path_len);
ETDLL_API const char* etm_get_download_path(void);

/////2.7 设置任务状态转换通知回调函数,此函数必须在创建任务之前调用
// 注意回调函数的代码应尽量简洁，且万不可以在回调函数中调用任何ETM的接口函数，否则会导致死锁!
typedef int32 ( *ETM_NOTIFY_TASK_STATE_CHANGED)(u32 task_id, ETM_TASK_INFO * p_task_info);
ETDLL_API int32 	etm_set_task_state_changed_callback( ETM_NOTIFY_TASK_STATE_CHANGED callback_function_ptr);

/////2.8 设置有盘点播的默认临时文件缓存路径,必须已存在且可写,path_len 要小于ETM_MAX_FILE_PATH_LEN,此函数必须在创建有盘点播任务之前调用
ETDLL_API int32 etm_set_vod_cache_path(const char *path,u32  path_len);
ETDLL_API const char* etm_get_vod_cache_path(void);

/////2.9 设置有盘点播的缓存区大小，单位KB，即接口etm_set_vod_cache_path 所设进来的目录的最大可写空间，建议3GB 以上,此函数必须在调用etm_set_vod_cache_path之后,创建有盘点播任务之前调用
ETDLL_API int32 etm_set_vod_cache_size(u32  cache_size);
ETDLL_API u32  etm_get_vod_cache_size(void);

///// 强制清空vod 缓存
ETDLL_API int32  etm_clear_vod_cache(void);

/////设置缓冲时间，单位秒,默认为30秒缓冲,不建议设置该值,以保证播放流畅
ETDLL_API int32 etm_set_vod_buffer_time(u32 buffer_time);

/////2.10 设置点播的专用内存大小,单位KB，建议2MB 以上,此函数必须在创建点播任务之前调用
ETDLL_API int32 etm_set_vod_buffer_size(u32  buffer_size);
ETDLL_API u32  etm_get_vod_buffer_size(void);

/////2.11 查询vod 专用内存是否已经分配
ETDLL_API Bool etm_is_vod_buffer_allocated(void);

/////2.12 手工释放vod 专用内存,ETM 本身在反初始化时会自动释放这块内存，但如果界面程序想尽早腾出这块内存的话，可调用此接口，注意调用之前要确认无点播任务在运行
ETDLL_API int32  etm_free_vod_buffer(void);

/////2.13 重置以下各系统参数:(10M内存版本时)max_tasks=5,download_limit_speed=-1,upload_limit_speed=-1,auto_limit_speed=FALSE,max_task_connection=128,task_auto_start=FALSE
ETDLL_API int32 	etm_load_default_settings(void);

/////2.14 设置最大同时运行的任务个数,最小为1,最大为15
ETDLL_API int32 etm_set_max_tasks(u32 task_num);
ETDLL_API u32 etm_get_max_tasks(void);

/////2.15 设置下载限速,以KB 为单位,等于-1为不限速,不能为0
ETDLL_API int32 etm_set_download_limit_speed(u32 max_speed);
ETDLL_API u32 etm_get_download_limit_speed(void);

/////2.16 设置上传限速,以KB 为单位,等于-1为不限速,不能为0,建议max_download_limit_speed:max_upload_limit_speed=3:1
ETDLL_API int32 etm_set_upload_limit_speed(u32 max_speed);
ETDLL_API u32 etm_get_upload_limit_speed(void);

/////2.17 设置是否启动智能限速(auto_limit=TRUE为启动)，linux下必须用root 权限运行ETM 的时候才能生效
ETDLL_API int32 etm_set_auto_limit_speed(Bool auto_limit);
ETDLL_API Bool etm_get_auto_limit_speed(void);

/////2.18 设置最大连接数,10M 内存版本取值范围为[1~200]
ETDLL_API int32 etm_set_max_task_connection(u32 connection_num);
ETDLL_API u32 etm_get_max_task_connection(void);

/////2.19 设置ETM 启动后是否自动开始下载未完成的任务(auto_start=TRUE为是)
ETDLL_API int32 etm_set_task_auto_start(Bool auto_start);
ETDLL_API Bool etm_get_task_auto_start(void);

/////2.20 获取当前全局下载总速度,以Byte 为单位
ETDLL_API u32 etm_get_current_download_speed(void);

/////2.21 获取当前全局上传总速度,以Byte 为单位
ETDLL_API u32 etm_get_current_upload_speed(void);

/////2.22 设置下载分段大小,[100~1000]，单位KB
ETDLL_API int32 etm_set_download_piece_size(u32 piece_size);
ETDLL_API u32 etm_get_download_piece_size(void);

/////2.23 设置下载完成提示音开关类型:0 -关闭,1-打开,3-震动,4-根据情景模式
ETDLL_API int32 etm_set_prompt_tone_mode(u32 mode);
ETDLL_API u32 etm_get_prompt_tone_mode(void);

/////2.24 设置p2p 下载模式:0 -打开,1-在WIFI网络下打开,2-关闭
ETDLL_API int32 etm_set_p2p_mode(u32 mode);
ETDLL_API u32 etm_get_p2p_mode(void);

ETDLL_API int32 etm_set_ui_version(const char * version);
/* 以下三组接口用于界面保存和获取配置项
	item_name 和item_value最长为63字节
	调用get接口时,若找不到与item_name 对应的项则以item_value 为初始值生成新项 
*/
ETDLL_API int32  etm_settings_set_str_item(char * item_name,char *item_value);
ETDLL_API int32  etm_settings_get_str_item(char * item_name,char *item_value);

ETDLL_API int32  etm_settings_set_int_item(char * item_name,int32 item_value);
ETDLL_API int32  etm_settings_get_int_item(char * item_name,int32 *item_value);

ETDLL_API int32  etm_settings_set_bool_item(char * item_name,Bool item_value);
ETDLL_API int32  etm_settings_get_bool_item(char * item_name,Bool *item_value);
///////////////////////////////////////////////////////////////
/////3下载任务(包括纯下载和边下边播)相关接口

/////3.1 创建下载任务
ETDLL_API int32 etm_create_task(ETM_CREATE_TASK * p_param,u32* p_task_id );

/////3.2 暂停任务
ETDLL_API int32 etm_pause_task (u32 task_id);

/////3.3 开始被暂停的任务
ETDLL_API int32 etm_resume_task(u32 task_id);

/////3.4 将任务放入回收站
ETDLL_API int32 etm_delete_task(u32 task_id);

/////3.5 还原已删除的任务
ETDLL_API int32 etm_recover_task(u32 task_id);

/////3.6 永久删除任务,当delete_file为TRUE时表示删除任务所有信息的同时也删除任务对应的文件，但是注意delete_file 只对普通任务(ETT_URL,ETT_TCID,ETT_KANKAN,ETT_EMULE)有效,对bt 任务不会删除任何文件
ETDLL_API int32 etm_destroy_task(u32 task_id,Bool delete_file);

/////3.7 获取任务下载优先级task_id 列表,任务状态为ETS_TASK_WAITING,ETS_TASK_RUNNING,ETS_TASK_PAUSED,ETS_TASK_FAILED的都会在这个列表中,*buffer_len的单位是sizeof(u32)
ETDLL_API int32 etm_get_task_pri_id_list (u32 * id_array_buffer,u32 *buffer_len);

/////3.8 将任务下载优先级上移
ETDLL_API int32 etm_task_pri_level_up(u32 task_id,u32 steps);
	
/////3.9 将任务下载优先级下移
ETDLL_API int32 etm_task_pri_level_down (u32 task_id,u32 steps);

/////3.10 重命名任务文件，任务状态必须为ETS_TASK_SUCCESS，且只对普通任务(ETT_URL,ETT_TCID,ETT_KANKAN,ETT_EMULE)有效，不能用于bt任务，new_name_len必须小于ETM_MAX_FILE_NAME_LEN-ETM_MAX_TD_CFG_SUFFIX_LEN
ETDLL_API int32 etm_rename_task(u32 task_id,const char * new_name,u32 new_name_len);

/////3.11 获取任务的基本信息,由etm_create_vod_task接口创建的任务也可以调用这个接口
ETDLL_API int32 etm_get_task_info(u32 task_id,ETM_TASK_INFO *p_task_info);

/////3.12 获取正在运行的 任务的运行状况,由etm_create_vod_task接口创建的任务也可以调用这个接口
ETDLL_API int32 etm_get_task_running_status(u32 task_id,ETM_RUNNING_STATUS *p_status);

/////3.13 获取类型为ETT_URL 或者ETT_EMULE的 任务的URL
ETDLL_API const char* etm_get_task_url(u32 task_id);

/////3.14 获取类型为ETT_URL 的 任务的引用页面URL(如果有的话)
ETDLL_API const char* etm_get_task_ref_url(u32 task_id);

/////3.15 获取类型为ETT_TCID  或 ETT_KANKAN 的 任务的TCID
ETDLL_API const char* etm_get_task_tcid(u32 task_id);

/////3.16 获取类型为ETT_KANKAN 的 任务的GCID
ETDLL_API const char* etm_get_task_gcid(u32 task_id);

/////3.17 获取类型为ETT_BT 的 任务的种子文件全路径
ETDLL_API const char* etm_get_bt_task_seed_file(u32 task_id);

/////3.18 获取BT任务中某个子文件的信息,file_index取值范围为>=0。由etm_create_vod_task接口创建的bt 任务也可以调用这个接口
ETDLL_API int32 etm_get_bt_file_info(u32 task_id, u32 file_index, ETM_BT_FILE *file_info);

/////3.19 修改BT任务中要下载的文件序号列表,如果之前需要下的文件仍然需要下载的话，也要放在new_file_index_array里
ETDLL_API int32 etm_set_bt_need_download_file_index(u32 task_id, u32* new_file_index_array,	u32 new_file_num);

/////3.20 获取任务的用户附加数据,*buffer_len的单位是byte
ETDLL_API int32 etm_get_task_user_data(u32 task_id,void * data_buffer,u32 * buffer_len);

/////3.21 根据任务状态获取任务id 列表(包括远程控制创建的任务和本地创建的任务),*buffer_len的单位是sizeof(u32)
ETDLL_API int32 	etm_get_task_id_by_state(ETM_TASK_STATE state,u32 * id_array_buffer,u32 *buffer_len);

/////3.22 根据任务状态获取本地创建的任务id 列表(不包括远程控制创建的任务),*buffer_len的单位是sizeof(u32)
ETDLL_API int32 	etm_get_local_task_id_by_state(ETM_TASK_STATE state,u32 * id_array_buffer,u32 *buffer_len);

/////3.23 获取所有的任务id 列表,*buffer_len的单位是sizeof(u32)
ETDLL_API int32 	etm_get_all_task_ids(u32 * id_array_buffer,u32 *buffer_len);

///////////////////////////////////////////////////////////////
/////离线下载相关接口
/////3.23 根据URL ,cid,gicd 或BT种子的info_hash 查找对应的任务id
typedef struct t_etm_eigenvalue
{
	ETM_TASK_TYPE _type;				/* 任务类型，必须准确填写*/
	
	char* _url;							/* _type== ETT_URL 或者_type== ETT_EMULE 时，这里就是任务的URL */
	u32 _url_len;						/* URL 的长度，必须小于512 */
	char _eigenvalue[40];					/* _type== ETT_TCID 时,这里放CID,_type== ETT_KANKAN 时,这里放GCID, _type== ETT_BT 时,这里放info_hash   */
}ETM_EIGENVALUE;
ETDLL_API int32 	etm_get_task_id_by_eigenvalue(ETM_EIGENVALUE * p_eigenvalue,u32 * task_id);

/////3.24 把任务设置为离线任务
ETDLL_API int32 etm_set_task_lixian_mode(u32 task_id,Bool is_lixian);

/////3.25 查询任务是否为离线任务
ETDLL_API Bool etm_is_lixian_task(u32 task_id);

/////3.26 增加server 资源,file_index只在BT任务中用到,普通任务file_index可忽略
typedef struct t_etm_server_res
{
	u32 _file_index;
	u32 _resource_priority;
	char* _url;							
	u32 _url_len;					
	char* _ref_url;				
	u32 _ref_url_len;		
	char* _cookie;				
	u32 _cookie_len;		
}ETM_SERVER_RES;
ETDLL_API int32 etm_add_server_resource( u32 task_id, ETM_SERVER_RES  * p_resource );

/////3.27 增加peer 资源,file_index只在BT任务中用到,普通任务file_index可忽略
#define ETM_PEER_ID_SIZE 16
typedef struct t_etm_peer_res
{
	u32 _file_index;
	char _peer_id[ETM_PEER_ID_SIZE];							
	u32 _peer_capability;					
	u32 _res_level;
	u32 _host_ip;
	u16 _tcp_port;
	u16 _udp_port;
}ETM_PEER_RES;
ETDLL_API int32 etm_add_peer_resource( u32 task_id, ETM_PEER_RES  * p_resource );


///////////////////////////////////////////////////////////////
/////4 torrent 文件解析和E-Mule链接解析相关接口,其中torrent文件解析的两个接口必须成对使用!

/////4.1 从种子文件解析出种子信息
ETDLL_API int32 etm_get_torrent_seed_info (const char *seed_path, ETM_ENCODING_MODE encoding_mode,ETM_TORRENT_SEED_INFO **pp_seed_info );

/////4.2 释放种子信息
ETDLL_API int32 etm_release_torrent_seed_info( ETM_TORRENT_SEED_INFO *p_seed_info );

/////4.3 从E-Mule链接中解析出文件信息
ETDLL_API int32 etm_extract_ed2k_url(char* ed2k, ETM_ED2K_LINK_INFO* info);

///////////////////////////////////////////////////////////////
/////5 VOD点播相关接口

/////5.1 创建点播任务
ETDLL_API int32 etm_create_vod_task(ETM_CREATE_VOD_TASK * p_param,u32* p_task_id );

/////5.2 停止点播,如果这个任务是由etm_create_vod_task接口创建的，那么这个任务将被删除,如果任务由etm_create_task接口创建，则只停止点播
ETDLL_API int32 etm_stop_vod_task (u32 task_id);

/////5.3 读取文件数据，由etm_create_task接口创建的任务也可调用这个接口以实现边下载边播放
///// 建议len(一次获取数据的长度,单位Byte)为16*1024 以达到最优获取速度；block_time 单位为毫秒,  取值500,1000均可
ETDLL_API int32 etm_read_vod_file(u32 task_id, uint64 start_pos, uint64 len, char *buf, u32 block_time );
ETDLL_API int32 etm_read_bt_vod_file(u32 task_id, u32 file_index, uint64 start_pos, uint64 len, char *buf, u32 block_time );

/////5.4 获取缓冲(码率乘与30秒的数据长度)百分比，由etm_create_task接口创建的任务在边下载边播放时也可调用这个接口
ETDLL_API int32 etm_vod_get_buffer_percent(u32 task_id,  u32 * percent );

/////5.5 查询点播任务的剩余数据是否已经下载完成，一般用于判断是否应该开始下载下一个电影或同一部电影的下一片段。由etm_create_task接口创建的任务在边下载边播放时也可调用这个接口
ETDLL_API int32 etm_vod_is_download_finished(u32 task_id, Bool* finished );

/////5.6 获取点播任务文件的码率
ETDLL_API int32 etm_vod_get_bitrate(u32 task_id, u32 file_index, u32* bitrate );

/////5.7 获取当前下载到的连续位置
ETDLL_API int32 etm_vod_get_download_position(u32 task_id, uint64* position );

/////5.8 将点播任务转为下载任务,file_retain_time为任务文件在磁盘的保留时间(等于0为永久保存),单位秒
ETDLL_API int32 etm_vod_set_download_mode(u32 task_id, Bool download,u32 file_retain_time );

/////5.9 查询点播任务是否转为下载任务
ETDLL_API int32 etm_vod_get_download_mode(u32 task_id, Bool * download,u32 * file_retain_time );

/////5.10 点播统计上报
typedef struct t_etm_vod_report
{
       u32	_vod_play_time_len;//点播时长,单位均为秒
       u32  _vod_play_begin_time;//开始点播时间
	u32  _vod_first_buffer_time_len; //首缓冲时长
	u32  _vod_play_drag_num;//拖动次数
	u32  _vod_play_total_drag_wait_time;//拖动后等待总时长
	u32  _vod_play_max_drag_wait_time;//最大拖动等待时间
	u32  _vod_play_min_drag_wait_time;//最小拖动等待时间
	u32  _vod_play_interrupt_times;//中断次数
	u32  _vod_play_total_buffer_time_len;//中断等待总时长
	u32  _vod_play_max_buffer_time_len;//最大中断等待时间
	u32  _vod_play_min_buffer_time_len;//最小中断等到时间
	
	u32 _play_interrupt_1;         // 1分钟内中断
	u32 _play_interrupt_2;	        // 2分钟内中断
	u32 _play_interrupt_3;		    //6分钟内中断
	u32 _play_interrupt_4;		    //10分钟内中断
	u32 _play_interrupt_5;		    //15分钟内中断
	u32 _play_interrupt_6;		    //15分钟以上中断
}ETM_VOD_REPORT;
ETDLL_API int32 etm_vod_report(u32 task_id, ETM_VOD_REPORT * p_report );


///////////////////////////////////////////////////////////////
/// 6. 任务存储树相关接口,注意这棵树与下载任务或点播任务无任何关系，一般仅供界面程序存放树状菜单用

#define ETM_INVALID_NODE_ID	0

/* whether if create tree while it not exist. */
#define ETM_TF_CREATE		(0x1)
/* read and write (default) */
#define ETM_TF_RDWR		(0x0)
/* read only. */
#define ETM_TF_RDONLY		(0x2)
/* write only. */
#define ETM_TF_WRONLY		(0x4)
#define ETM_TF_MASK       (0xFF)


// 创建或打开一棵树(flag=ETM_TF_CREATE|ETM_TF_RDWR),如果成功*p_tree_id返回树根节点id
ETDLL_API int32 	etm_open_tree(const char * file_path,int32 flag ,u32 * p_tree_id); 

// 关闭一棵树
ETDLL_API int32 	etm_close_tree(u32 tree_id); 

// 删除一棵树
ETDLL_API int32 	etm_destroy_tree(const char * file_path); 

// 判断数是否存在
ETDLL_API Bool 	etm_tree_exist(const char * file_path); 

// 复制一棵树
ETDLL_API int32 	etm_copy_tree(const char * file_path,const char * new_file_path); 


// 创建一个节点,data_len(单位Byte)大小无限制,但建议不大于256
ETDLL_API int32 	etm_create_node(u32 tree_id,u32 parent_id,const char * name, u32 name_len,void * data, u32 data_len,u32 * p_node_id); 

// 删除一个节点。注意:如果这个node_id是树枝，会把里面的所有叶节点同时删除;node_id不能等于tree_id,删除树要用etm_destroy_tree
ETDLL_API int32 	etm_delete_node(u32 tree_id,u32 node_id);

// 设置节点名字(或重命名),name_len必须小于256 bytes;node_id不能等于tree_id
ETDLL_API int32 	etm_set_node_name(u32 tree_id,u32 node_id,const char * name, u32 name_len);

// 获取节点名字;node_id等于tree_id时返回这棵树的名字
ETDLL_API  const char * etm_get_node_name(u32 tree_id,u32 node_id);

// 设置或修改节点数据,new_data_len(单位Byte)大小无限制,但建议不大于256;
ETDLL_API int32 	etm_set_node_data(u32 tree_id,u32 node_id,void * new_data, u32 new_data_len);

// 获取节点数据,*buffer_len的单位是Byte;
ETDLL_API int32	etm_get_node_data(u32 tree_id,u32 node_id,void * data_buffer,u32 * buffer_len);

// 移动节点
///移往不同分类;node_id不能等于tree_id
ETDLL_API int32 	etm_set_node_parent(u32 tree_id,u32 node_id,u32 parent_id);

///同一分类中移动;node_id不能等于tree_id
ETDLL_API int32 	etm_node_move_up(u32 tree_id,u32 node_id,u32 steps);
ETDLL_API int32 	etm_node_move_down(u32 tree_id,u32 node_id,u32 steps);


// 获取父节点id;node_id不能等于tree_id
ETDLL_API u32 	etm_get_node_parent(u32 tree_id,u32 node_id);

// 获取所有子节点id 列表,*buffer_len的单位是sizeof(u32)
ETDLL_API int32	etm_get_node_children(u32 tree_id,u32 node_id,u32 * id_buffer,u32 * buffer_len);

// 获取第一个子节点id,找不到返回ETM_INVALID_NODE_ID
ETDLL_API u32	etm_get_first_child(u32 tree_id,u32 parent_id);

// 获取兄弟节点id;node_id不能等于tree_id,找不到返回ETM_INVALID_NODE_ID
ETDLL_API u32	etm_get_next_brother(u32 tree_id,u32 node_id);
ETDLL_API u32	etm_get_prev_brother(u32 tree_id,u32 node_id);

// 根据名字查找节点id，【名字暂不支持分层式的写法，如aaa.bbb.ccc】
// 返回第一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID
ETDLL_API u32	etm_find_first_node_by_name(u32 tree_id,u32 parent_id,const char * name, u32 name_len);

// 返回下一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID;node_id不能等于tree_id
ETDLL_API u32	etm_find_next_node_by_name(u32 tree_id,u32 parent_id,u32 node_id,const char * name, u32 name_len); //注意node_id与parent_id的对应关系
// 返回上一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID;node_id不能等于tree_id
ETDLL_API u32	etm_find_prev_node_by_name(u32 tree_id,u32 parent_id,u32 node_id,const char * name, u32 name_len); //注意node_id与parent_id的对应关系


// 根据节点数据查找节点,data_len单位Byte.

// 返回第一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID
ETDLL_API u32	etm_find_first_node_by_data(u32 tree_id,u32 parent_id,void * data, u32 data_len);

// 返回下一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID;node_id不能等于tree_id
ETDLL_API u32	etm_find_next_node_by_data(u32 tree_id,u32 parent_id,u32 node_id,void * data, u32 data_len);
// 返回上一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID;node_id不能等于tree_id
ETDLL_API u32	etm_find_prev_node_by_data(u32 tree_id,u32 parent_id,u32 node_id,void * data, u32 data_len);

// 根据节点名字和数据查找合适的节点,data_len单位Byte.
// 返回第一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID
ETDLL_API u32	etm_find_first_node(u32 tree_id,u32 parent_id,const char * name, u32 name_len,void * data, u32 data_len);

// 返回下一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID;node_id不能等于tree_id
ETDLL_API u32	etm_find_next_node(u32 tree_id,u32 parent_id,u32 node_id,const char * name, u32 name_len,void * data, u32 data_len);//注意node_id与parent_id的对应关系
// 返回上一个匹配的节点id，找不到返回ETM_INVALID_NODE_ID;node_id不能等于tree_id
ETDLL_API u32	etm_find_prev_node(u32 tree_id,u32 parent_id,u32 node_id,const char * name, u32 name_len,void * data, u32 data_len);//注意node_id与parent_id的对应关系


//////////////////////////////////////////////////////////////
/////7 其它

/////7.1 从URL 中获得文件名和文件大小，如果不能从URL 中直接解析得到，ETM 将会查询迅雷服务器获得		[ 暂不支持获取文件大小] 
/// 注意: 目前只支持http://,https://,ftp://和thunder://,ed2k://开头的URL, url_len必须小于512
///               如果只想得到文件名，file_size可为 NULL ；如果只想得到文件大小，name_buffer可为 NULL
///               由于该接口为同步(线程阻塞)接口,因此需要设置block_time,单位为毫秒
ETDLL_API int32 etm_get_file_name_and_size_from_url(const char* url,u32 url_len,char *name_buffer,u32 *name_buffer_len,uint64 *file_size, int32 block_time );

/////7.2 创建微型下载任务,回调函数中result为0表示成功【此接口已被etm_http_get 和etm_http_get_file替代,请别再使用!】
// typedef int32 ( *ETM_NOTIFY_MINI_TASK_FINISHED)(void * user_data, int32 result,void * recv_buffer,u32  recvd_len,void * p_header,u8 * send_data);
ETDLL_API int32 etm_get_mini_file_from_url(ETM_MINI_TASK * p_mini_param );

/////7.3 创建微型上传任务,回调函数中result为0表示成功【此接口已被etm_http_post替代,请别再使用!】
// typedef int32 ( *ETM_NOTIFY_MINI_TASK_FINISHED)(void * user_data, int32 result,void * recv_buffer,u32  recvd_len,void * p_header,u8 * send_data);
ETDLL_API int32 etm_post_mini_file_to_url(ETM_MINI_TASK * p_mini_param );

/////7.4 强制取消微型任务【此接口已被etm_http_cancel替代,请别再使用!】
ETDLL_API int32 etm_cancel_mini_task(u32 mini_id );

//////////////////////////////////////////////////////////////
/* HTTP会话接口的回调函数参数*/
typedef enum t_etm_http_cb_type
{
	EHCT_NOTIFY_RESPN=0, 
	EHCT_GET_SEND_DATA, 	// Just for POST
	EHCT_NOTIFY_SENT_DATA, 	// Just for POST
	EHCT_GET_RECV_BUFFER,
	EHCT_PUT_RECVED_DATA,
	EHCT_NOTIFY_FINISHED
} ETM_HTTP_CB_TYPE;
typedef struct t_etm_http_call_back
{
	u32 _http_id;
	void * _user_data;			/* 用户参数 */
	ETM_HTTP_CB_TYPE _type;
	void * _header;				/* _type==EHCT_NOTIFY_RESPN时有效,指向http响应头,可用etm_get_http_header_value获取里面的详细信息*/
	
	u8 ** _send_data;			/* _type==EHCT_GET_SEND_DATA时有效, 数据分步上传时,指向需要上传的数据*/
	u32  * _send_data_len;		/* 需要上传的数据长度 */
	
	u8 * _sent_data;			/* _type==EHCT_NOTIFY_SENT_DATA时有效, 数据分步上传时,指向已经上传的数据,切记无论_sent_data_len是否为零,界面都要负责释放该内存*/
	u32   _sent_data_len;		/* 已经上传的数据长度 */
	
	void ** _recv_buffer;			/* _type==EHCT_GET_RECV_BUFFER时有效, 数据分步下载时,指向用于接收数据的缓存*/
	u32  * _recv_buffer_len;		/* 缓存大小 ,至少要16384 byte !*/
	
	u8 * _recved_data;			/* _type==EHCT_PUT_RECVED_DATA时有效, 数据分步下载时,指向已经收到的数据,切记无论_recved_data_len是否为零,界面都要负责释放该内存*/
	u32  _recved_data_len;		/* 已经收到的数据长度 */

	int32 _result;					/* _type==EHCT_NOTIFY_FINISHED时有效, 0为成功*/
} ETM_HTTP_CALL_BACK;
typedef int32 ( *ETM_HTTP_CALL_BACK_FUNCTION)(ETM_HTTP_CALL_BACK * p_http_call_back_param); 
/* HTTP会话接口的输入参数 */
typedef struct t_etm_http_get
{
	char* _url;					/* 只支持"http://" 开头的url  */
	u32 _url_len;
	
	char * _ref_url;				/* 引用页面URL*/
	u32  _ref_url_len;		
	
	char * _cookie;			
	u32  _cookie_len;		
	
	uint64  _range_from;			/* RANGE 起始位置,【暂不支持】*/
	uint64  _range_to;			/* RANGE 结束位置,【暂不支持】*/
	
	Bool  _accept_gzip;			/* 是否接受压缩文件 */
	
	void * _recv_buffer;			/* 用于接收服务器返回的响应数据的缓存*/
	u32  _recv_buffer_size;		/* _recv_buffer 缓存大小,至少要16384 byte !*/
	
	ETM_HTTP_CALL_BACK_FUNCTION _callback_fun;			/* 任务完成后的回调函数 : typedef int32 ( *ETM_HTTP_CALL_BACK_FUNCTION)(ETM_HTTP_CALL_BACK * p_http_call_back_param); */
	void * _user_data;			/* 用户参数 */
	u32  _timeout;				/* 超时,单位秒,等于0时180秒超时*/
} ETM_HTTP_GET;
typedef struct t_etm_http_post
{
	char* _url;					/* 只支持"http://" 开头的url  */
	u32 _url_len;
	
	char * _ref_url;				/* 引用页面URL*/
	u32  _ref_url_len;		
	
	char * _cookie;			
	u32  _cookie_len;		
	
	uint64  _content_len;			/* Content-Length:*/
	
	Bool  _send_gzip;			/* 是否发送压缩数据*/
	Bool  _accept_gzip;			/* 是否接受压缩数据*/
	
	u8 * _send_data;				/* 指向需要上传的数据*/
	u32  _send_data_len;			/* 需要上传的数据大小*/
	
	void * _recv_buffer;			/* 用于接收服务器返回的响应数据的缓存*/
	u32  _recv_buffer_size;		/* _recv_buffer 缓存大小,至少要16384 byte !*/

	ETM_HTTP_CALL_BACK_FUNCTION _callback_fun;			/* 任务完成后的回调函数 : typedef int32 ( *ETM_HTTP_CALL_BACK_FUNCTION)(ETM_HTTP_CALL_BACK * p_http_call_back_param); */
	void * _user_data;			/* 用户参数 */
	u32  _timeout;				/* 超时,单位秒,等于0时180秒超时*/
} ETM_HTTP_POST;
typedef struct t_etm_http_get_file
{
	char* _url;					/* 只支持"http://" 开头的url  */
	u32 _url_len;
	
	char * _ref_url;				/* 引用页面URL*/
	u32  _ref_url_len;		
	
	char * _cookie;			
	u32  _cookie_len;		
	
	uint64  _range_from;			/* RANGE 起始位置,【暂不支持】*/
	uint64  _range_to;			/* RANGE 结束位置,【暂不支持】*/
	
	Bool  _accept_gzip;			/* 是否接受压缩文件 */
	
	char _file_path[ETM_MAX_FILE_PATH_LEN]; 				/* _is_file=TRUE时, 需要上传或下载的文件存储路径，必须真实存在 */
	u32 _file_path_len;			/* _is_file=TRUE时, 需要上传或下载的文件存储路径的长度*/
	char _file_name[ETM_MAX_FILE_NAME_LEN];			/* _is_file=TRUE时, 需要上传或下载的文件名*/
	u32 _file_name_len; 			/* _is_file=TRUE时, 需要上传或下载的文件名长度*/
	
	ETM_HTTP_CALL_BACK_FUNCTION _callback_fun;			/* 任务完成后的回调函数 : typedef int32 ( *ETM_HTTP_CALL_BACK_FUNCTION)(ETM_HTTP_CALL_BACK * p_http_call_back_param); */
	void * _user_data;			/* 用户参数 */
	u32  _timeout;				/* 超时,单位秒,等于0时180秒超时*/
} ETM_HTTP_GET_FILE;


/////7.6 创建http get 会话
// typedef int32 ( *ETM_HTTP_CALL_BACK_FUNCTION)(ETM_HTTP_CALL_BACK * p_http_call_back_param);
ETDLL_API int32 etm_http_get(ETM_HTTP_GET * p_http_get ,u32 * http_id);
ETDLL_API int32 etm_http_get_file(ETM_HTTP_GET_FILE * p_http_get_file ,u32 * http_id);

/////7.7 创建http post 会话
// typedef int32 ( *ETM_HTTP_CALL_BACK_FUNCTION)(ETM_HTTP_CALL_BACK * p_http_call_back_param);
ETDLL_API int32 etm_http_post(ETM_HTTP_POST * p_http_post ,u32 * http_id );

/////7.8 强制取消http会话,这个函数不是必须的，只有UI想在回调函数被执行之前取消该会话时才调用
ETDLL_API int32 etm_http_cancel(u32 http_id );

/////7.9 获取http头的字段值,该函数可以直接在回调函数里调用
/* http头字段*/
typedef enum t_etm_http_header_field
{
	EHHV_STATUS_CODE=0, 
	EHHV_LAST_MODIFY_TIME, 
	EHHV_COOKIE,
	EHHV_CONTENT_ENCODING,
	EHHV_CONTENT_LENGTH
} ETM_HTTP_HEADER_FIELD;
ETDLL_API char * etm_get_http_header_value(void * p_header,ETM_HTTP_HEADER_FIELD header_field );

//////////////////////////////////////////////////////////////
/////8 远程控制

/*消息回调函数(etm_remote_control_start返回成功后,会通过这个回调函数通知远程控制内部信息)*/
/* 注意:不能在此回调函数中直接调用ETM的接口, 否则会死锁*/
/* msg_id 消息
0: 激活码   //第一次使用远程控制,界面把激活码显示给用户,需要用户去web绑定激活码.
1: 受控成功 //用户使用一个有效的已经绑定激活码的远程控制.
2: 错误消息 //当界面收到这个消息,需要去在自己的线程里调用etm_remote_control_stop接口,或者直接调用etm_remote_control_start重启远程控制.
* msg消息
* msg _len消息长度
*/
typedef int32 ( *ET_RT_CTRL_MSG_NOTIFY)(u32 msg_id,  char * msg, u32 msg_len);

typedef struct t_etm_rc_path
{
	char _data_path[ETM_MAX_FILE_PATH_LEN]; 
} ETM_RC_PATH;


/*  开始使用远程控制
*注意:由于web编码展示为utf-8,所以使用远程控制功能需要调用etm_set_default_encoding_mode( EEM_ENCODING_UTF8);
* data_path_array:默认存储路径数组,路径长度小于256.(会在web端显示),不同路径必须在不同磁盘。
* path_num: 存储路径个数(小于等于3个)
* 104449 表示调用此接口时机不对.此时任务处于非stop状态(注意在调用stop后马上调用start , 因为服务器尚未发freebind命令过来,所以盒子返回104450错误码) 
* 104451 path个数非法( 正确个数: 0<path_num<=3 )
* 104452 path长度非法( 正确路径长度: 0<path_length<ETM_MAX_FILE_PATH_LEN )
* 104453 path非法,path不存在,或path不可写
* 102406 表示liscense不通过,或者ETM正在与迅雷服务器交互中,需要稍等liscense成功后再调用此接口.或者liscense非法
*/
ETDLL_API int32 etm_remote_control_start( ETM_RC_PATH data_path_array[], u32 path_num, ET_RT_CTRL_MSG_NOTIFY call_back );


/* 停止使用远程控制 
 * 104450 表示调用此接口时机不对.任务处于stop状态或connecting状态,比如没用调用start就直接调用stop ((注意在start后马上调用stop ,因为服务器尚未受控成功,盒子返回104450错误码)
*/

ETDLL_API int32 etm_remote_control_stop(void);


//////////////////////////////////////////////////////////////
/////8 drm


/* 
 *  设置drm证书的存放路径
 *  certificate_path:  drm证书路径
 * 返回值: 0     成功
           4201  certificate_path非法(路径不存在或路径长度大于255)
 */
ETDLL_API int32 etm_set_certificate_path(const char *certificate_path);


/* 
 *  打开xlmv文件,获取操作文件的drm_id
 *  p_file_full_path:  xlmv文件全路径
 *  p_drm_id: 返回的drm_id(一个文件对应一个,且不会重复)
 *  p_origin_file_size: 返回的drm原始文件的大小
 * 返回值: 0     成功
           22529 drm文件不存在.
           22530 drm文件打开失败.
           22531 drm文件读取失败.
           22532 drm文件格式错误.
           22537 drm证书格式错误,或者非法证书文件.
           22538 不支持openssl库,无法解密文件
 */
ETDLL_API int32 etm_open_drm_file(const char *p_file_full_path, u32 *p_drm_id, uint64 *p_origin_file_size);


/* 
 *  xlmv文件对应证书文件是否合法可用
 *  drm_id:  xlmv文件对应的drm_id
 *  p_is_ok: 是否证书文件合法可用
 * 返回值: 0     成功
           22534 无效的drm_id.
           22535 证书下载失败.
           22536 drm内部逻辑错误.
           22537 drm证书格式错误,或者非法证书文件.
           22538 不支持openssl库,无法解密文件
           
           (以下错误码需要返回给界面让用户知道)
           22628 盒子的pid绑定的用户名不存在
           22629 用户没有购买指定的影片
           22630 商品已经下架，用户将不能再购买；如果是在已经购买之后下架的，用户还是可以播放
           22631 绑定的盒子数超出限制
           22632 密钥未找到，或者是证书生成过程中出现了异常
           22633 服务器忙，一般是系统压力过大，导致查询或者绑定存在异常。如果出现这个异常，客户端可以考虑重试。
 */

ETDLL_API int32 etm_is_certificate_ok(u32 drm_id, Bool *p_is_ok);


/* 
 *  xlmv文件读取数据
 *  drm_id:      xlmv文件对应的drm_id
 *  p_buffer:    读取数据buffer
 *  size:        读取数据buffer的大小
 *  file_pos:    读取数据的起始位置
 *  p_read_size: 实际读取数据大小
 * 返回值: 0     成功
           22533 证书尚未成功
           22534 无效的drm_id.
           22536 drm内部逻辑错误.
           22537 drm证书格式错误,或者非法证书文件.
 */
ETDLL_API int32 etm_read_drm_file(u32 drm_id, char *p_buffer, u32 size, 
    uint64 file_pos, u32 *p_read_size );


/* 
 *  关闭xlmv文件
 *  drm_id:      xlmv文件对应的drm_id
 * 返回值: 0     成功
           22534 无效的drm_id.
 */
ETDLL_API int32 etm_close_drm_file(u32 drm_id);


#define ETM_OPENSSL_IDX_COUNT			    (7)

/* function index */
#define ETM_OPENSSL_D2I_RSAPUBLICKEY_IDX    (0)
#define ETM_OPENSSL_RSA_SIZE_IDX            (1)
#define ETM_OPENSSL_BIO_NEW_MEM_BUF_IDX     (2)
#define ETM_OPENSSL_D2I_RSA_PUBKEY_BIO_IDX  (3)
#define ETM_OPENSSL_RSA_PUBLIC_DECRYPT_IDX  (4)
#define ETM_OPENSSL_RSA_FREE_IDX            (5)
#define ETM_OPENSSL_BIO_FREE_IDX            (6)

/* 
 *  drm需要用到openssl库里面rsa解密相关的函数,需要外界设置对应的回调函数进来.(必须在下载库启动前调用)
 *  fun_count:    需要的回调函数个数,必须等于ET_OPENSSL_IDX_COUNT
 *  fun_ptr_table:    回调函数数组,可以是一个void*的数组
 * 返回值: 0     成功
           3273  参数不对,fun_count不为ET_OPENSSL_IDX_COUNT或fun_ptr_table为NULL或数组元素为NULL

 *  序号：0 (ET_OPENSSL_D2I_RSAPUBLICKEY_IDX)  函数原型：typedef RSA * (*et_func_d2i_RSAPublicKey)(RSA **a, _u8 **pp, _int32 length);
 *  说明：对应openssl库的d2i_RSAPublicKey函数

 *  序号：1 (ET_OPENSSL_RSA_SIZE_IDX)  函数原型：typedef _int32 (*et_func_openssli_RSA_size)(const RSA *rsa);
 *  说明：对应openssl库的RSA_size函数

 *  序号：2 (ET_OPENSSL_BIO_NEW_MEM_BUF_IDX)  函数原型：typedef BIO *(*et_func_BIO_new_mem_buf)(void *buf, _int32 len);
 *  说明：对应openssl库的BIO_new_mem_buf函数

 *  序号：3 (ET_OPENSSL_D2I_RSA_PUBKEY_BIO_IDX)  函数原型：typedef RSA *(*et_func_d2i_RSA_PUBKEY_bio)(BIO *bp,RSA **rsa);
 *  说明：对应openssl库的d2i_RSA_PUBKEY_bio函数

 *  序号：4 (ET_OPENSSL_RSA_PUBLIC_DECRYPT_IDX)  函数原型：typedef _int32(*et_func_RSA_public_decrypt)(_int32 flen, const _u8 *from, _u8 *to, RSA *rsa, _int32 padding);
 *  说明：对应openssl库的RSA_public_decrypt函数

 *  序号：5 (ET_OPENSSL_RSA_FREE_IDX)  函数原型：typedef void(*et_func_RSA_free)( RSA *r );
 *  说明：对应openssl库的RSA_free函数

 *  序号：6 (ET_OPENSSL_BIO_FREE_IDX)  函数原型：typedef void(*et_func_BIO_free)( BIO *a );
 *  说明：对应openssl库的BIO_free函数

 */

ETDLL_API int32 etm_set_openssl_rsa_interface(int32 fun_count, void *fun_ptr_table);


//////////////////////////////////////////////////////////////
/////9 查询错误码对应的简要描述。注意1~1024的描述在不同的平台可能有所不同
ETDLL_API const char * etm_get_error_code_description(int32 error_code);





#ifdef __cplusplus
}
#endif
#endif /* ET_TASK_MANAGER_H_200909081912 */

