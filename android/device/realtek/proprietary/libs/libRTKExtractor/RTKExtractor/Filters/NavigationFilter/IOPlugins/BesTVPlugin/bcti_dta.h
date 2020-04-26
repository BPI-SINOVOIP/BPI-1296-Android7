/*
 * bcti_dta.h
 *
 *  Created on: 2011-3-22
 *      Author: li.zhaoping
 */

#ifndef BCTI_DTA_H__
#define BCTI_DTA_H__

/* base type define*/
typedef char				int8_dta;
typedef unsigned char		uint8_dta;
typedef short				int16_dta;
typedef unsigned short 		uint16_dta;
typedef int					int32_dta;
typedef unsigned int		uint32_dta;
typedef long long			int64_dta;
typedef unsigned long long	uint64_dta;
typedef int32_dta			bcti_dta_handle;
#ifndef BOOL
typedef int					BOOL;
#endif
#ifndef TRUE
#define TRUE				(BOOL)1
#endif
#ifndef FALSE
#define FALSE				(BOOL)0
#endif

/*
 * 负数表示必须处理的错误
 */
typedef enum {
	BCTI_DTA_STATUS_UNEXPECTED = -5,//播放过程中某个还未播放的子项打开失败，只会在NextMovie中返回
	BCTI_DTA_STATUS_TIMEOUT = -4,	//超时
	BCTI_DTA_STATUS_REOPEN = -3,	//重复打开
	BCTI_DTA_STATUS_CANCLE = -2,	//被Close打断
	BCTI_DTA_STATUS_FAILURE = -1,	//通用失败
	BCTI_DTA_STATUS_SUCCESS = 0,	//成功
	BCTI_DTA_STATUS_RESERVED = 1,	//保留
} bcti_dta_status_e;

/*
 * Read函数返回类型
 * 负数表示必须处理的错误
 */
typedef enum {
	BCTI_DTA_READ_ERR = -1,				//错误
	BCTI_DTA_READ_SUC = 0,				//成功 非下面的情况
	BCTI_DTA_READ_SUC_MOVIEEND = 1,		//读完一个MOVIE
	BCTI_DTA_READ_SUC_ALLEND = 2,		//所有数据读完
	BCTI_DTA_READ_SUC_CLIPEND = 3,		//切片模式中，其中一个切片读完
	BCTI_DTA_READ_CANCLE = 4,			//被close打断
	BCTI_DTA_READ_TIMEOUT = 5,			//超时,返回超时也可能读到部分数据,只是没有读到所需要的数据
} bcti_dta_read_e;

/*
 * SeekByTime函数返回类型
 * 负数表示必须处理的错误
 */
typedef enum {
	BCTI_DTA_SEEKTIME_ERR = -1,			//出错
	BCTI_DTA_SEEKTIME_FAIL = 0,			//播放器要自己做seek（直接调SeekBySize）
	BCTI_DTA_SEEKTIME_CLIP = 1,			//切片方式下的seek
	BCTI_DTA_SEEKTIME_SUC_TOPOS = 2,	//Dta转化时间到Pos，并通过参数返回该Pos，播放器通过该Pos进行seek
	BCTI_DTA_SEEKTIME_SUC_NEWFILE = 3,	//Dta转化后获取到的是一个新的文件，播放器需要重新codec过程//TODO
} bcti_dta_seek_e;

typedef enum {
	BCTI_DTA_SERVER_TIMEOUT = 0,	// 0,  连接服务器超时,停止播放
	BCTI_DTA_NO_SERVER, // 1,  找不到服务器,停止播放
	BCTI_DTA_SERVER_NO_RESPONSE, // 2,  服务器未响应,停止播放
	BCTI_DTA_SERVER_INVALID, // 3,  解析服务器域名失败.
	BCTI_DTA_VERSION_LOW, // 4,  客户端版本号太低,退出网络
	BCTI_DTA_CLIENT_NAME, // 5,  客户端名称或引用页不正确,退出播放
	BCTI_DTA_URL_INVALID, // 6,  播放的文件不存在或网址错误
	BCTI_DTA_NO_SERVICE, // 7,  服务不存在,停止播放
	BCTI_DTA_OUT_OF_SERVICE, // 8,  不在服务区.
	BCTI_DTA_MEDIA_INFO_ERROR, // 9,  媒体信息加载时错误..
	BCTI_DTA_INDEX_INFO_ERROR, // 10, 媒体索引信息错误
	BCTI_DTA_NO_INDEX, // 11, 没有获取到媒体索引信息,不能拖动播放位置
	BCTI_DTA_NO_MEIDIA, // 12, 找不到媒体
	BCTI_DTA_MULTI_INSTANCE, // 13, 请确定是否有另一个实例正在播放此文件
	BCTI_DTA_PREPARE_MEDIA_INFO, // 14, 正在准备媒体信息
	BCTI_DTA_GETTING_MEDIA_INFO, // 15, 正在获取媒体信息
	BCTI_DTA_GETTING_INDEX_INFO, // 16, 正在获取媒体索引信息
	BCTI_DTA_PLAYING, // 17, 正在播放
	BCTI_DTA_BUFFERING, // 18, 正在缓冲数据  [param1为缓冲百分比0 - 100]
	BCTI_DTA_CONNECTING, // 19, 正在连入网络
	BCTI_DTA_MEDIA_READY, // 20, 准备媒体就绪
	BCTI_DTA_PARSING_SERVER, // 21, 正在解析服务器域名..
	BCTI_DTA_GET_MEDIA_INFO, // 22, 已成功获取到媒体信息.
	BCTI_DTA_BUFFERED, // 23, 数据缓冲完毕.[param1为100]
	BCTI_DTA_PREPARE_MEDIA, // 24, 准备媒体
	BCTI_DTA_STORAGE_ERROR, // 25. 访问存储器失败
	BCTI_DTA_QUIT, // 26. 程序退出

	BCTI_DTA_DATA_TIMEOUT, // 27, 等待数据超时

	BCTI_DTA_UPDATING, // 28, 正在升级
	BCTI_DTA_UPDATE_FAILED, // 29, 升级失败
	BCTI_DTA_UPDATE_OK, // 30, 升级成功
	BCTI_DTA_AUTH_UNPASSED, // 31, 验证成功，非付费用户
	BCTI_DTA_AUTH_PASSED, // 32, 验证成功，付费用户
	BCTI_DTA_AUTH_INVALID, // 33, 单部点播，验证未通过
	BCTI_DTA_AUTH_FAILED, // 34, 验证失败
	BCTI_DTA_ALLOCATE_PGF,//35 预先分配缓存文件空间失败

} bcti_dta_event_e;

/* 回调函数类型
 * handle 		[播放过程的句柄，即CreateSession返回的句柄]
 * id 			[事件id，参考bcti_dta_event_e类型]
 * param1 		[根据id的不同而不同]
 * param2 		[根据id的不同而不同]
 * message		[事件提示信息,可能会为NULL]
 * 备注			param1和param2的值请参考bcti_dta_event_e的注释，如无说明，默认为0
 */
typedef int32_dta (*bcti_dta_event_handler)(bcti_dta_handle handle,
		bcti_dta_event_e id, uint32_dta param1, uint32_dta param2,
		const char* message);

typedef enum {
	DTA_SEEK_BEGIN,	//从文件头
	DTA_SEEK_CURR,	//从当前位置
	DTA_SEEK_END,	//从文件尾，此时offset应该为负数
} bcti_dta_seek_direction_e;

//影片类型
typedef enum{
	MovieType_UnKnown = -1,
	MovieType_Prevue = 0,	//片花
	MovieType_Main = 1,		//正片
}MovieType_e;

//影片文件格式
typedef enum{
	MovieFileType_UnKnown = -1,
	MovieFileType_RMVB = 0,
	MOVIEFILETYPE_MP4 = 1,
	MovieFileType_TS = 2,
	MovieFileType_FLV = 3,
	MovieFileType_F4V = 4,
	MovieFileType_FLV_PFV = 5,
}MovieFileType_e;


//切片信息
typedef struct ClipInfo{
	int32_dta	duration;	//时间长度，单位毫秒
	int64_dta	length;		//长度，单位字节
}ClipInfo_t;

//影片信息
typedef struct {
	MovieType_e 		movieType;	//影片类型
	MovieFileType_e		fileType;	//影片文件格式
	BOOL				bLive;		//是否直播
	BOOL				bIsClip;	//是否是切片
	int64_dta	   		length;		//总长度，单位字节
	int32_dta			duration;	//总时间长度，单位毫秒
	int32_dta			clipCount;	//切片个数,为0或者bIsClip为false的话可不理会clipList
	ClipInfo_t*			clipList;	//ClipInfo_t数组指针,clipCount>0 表示切片信息数组
}bcti_movie_info;

//播放过程信息
typedef struct {
	int32_dta 				count;			//影片个数
	bcti_movie_info**		movieList;		//movie info 指针数组
} bcti_play_info;

class CBctiDta {
public:
	static const int8_dta* 		GetVesion();		//获取当前dta版本信息
	static CBctiDta* 			GetInstance();		//创建dta全局实例
	static void					Destroy();			//析构dta全局实例

	/* Init
	 * [功能] 初始化
	 * [参数] work_dir,配置文件dtaCfg.ini所在的目录。为NULL dta将使用默认配置.
	 * [返回值] BCTI_DTA_STATUS_SUCCESS：成功
	 * 			BCTI_DTA_STATUS_FAILURE：出错
	 */
	virtual bcti_dta_status_e 	Init(const int8_dta *work_dir) = 0;

	/* UnInit
	 * [功能] 反初始化
	 * [备注]  调用该函数之后不能再调其它函数，除非重新Init
	 * [返回值]	BCTI_DTA_STATUS_SUCCESS：成功
	 * 			BCTI_DTA_STATUS_FAILURE：出错
	 */
	virtual bcti_dta_status_e 	UnInit() = 0;

	/* SetEventHandler
	 * [功能] 设置消息回调函数
	 * [备注]  只用和Init一起调用一次即可
	 * [参数] event_handler,消息回调函数
	 * [返回值]	BCTI_DTA_STATUS_SUCCESS：成功
	 * 			BCTI_DTA_STATUS_FAILURE：出错
	 */
	virtual bcti_dta_status_e 	SetEventHandler(bcti_dta_event_handler event_handler) = 0;

	/* Open
	 * [功能] 打开一个播放链接
	 * [备注]  url不能为空，并且函数会阻塞到获取文件长度才返回
	 * [返回值]	BCTI_DTA_STATUS_TIMEOUT,	超时
	 *			BCTI_DTA_STATUS_REOPEN,	重复打开
	 *			BCTI_DTA_STATUS_CANCLE = -2,	//被Close打断
	 *			BCTI_DTA_STATUS_FAILURE = -1,	//通用失败
	 *			BCTI_DTA_STATUS_SUCCESS = 0,	//成功
	 */
	virtual bcti_dta_status_e 	Open(const int8_dta *url) = 0;


	/* Close
	 * [功能] 关闭一个播放链接
	 * [备注]  可异步调用该函数来打断open的阻塞
	 * [返回值] BCTI_DTA_STATUS_SUCCESS：成功
	 * 			BCTI_DTA_STATUS_FAILURE：出错
	 */
	virtual bcti_dta_status_e 	Close() = 0;

	/* GetPlayInfo
	* [功能]	获取整个播放过程的信息
	* [参数]	play_info返回的播放过程信息
	* [返回值]	取到播放信息指针, NULL表示失败					
	*/
	virtual const bcti_play_info* 	GetPlayInfo() = 0;

	/* GetMovieInfo
	 * [功能]	获取影片信息
	 * [备注]	刚刚Open之后只能获取第一个movie的信息,之后的movie需要在每次NextMovie
				之后调用该函数
	 * [参数]	movie_index,影片index
	 * [返回值]	movie信息, NULL表示获取失败
	 */
	virtual const bcti_movie_info* 	GetMovieInfo(int32_dta movie_index) = 0;

	/* NextMovie
	 * [功能]		设置播放下一个
	 * [返回值]		BCTI_DTA_STATUS_SUCCESS：成功
	 * 					BCTI_DTA_STATUS_FAILURE：失败
	 */
	virtual bcti_dta_status_e 	NextMovie(int32_dta nStep=1) = 0;

	/* Read
	 * [功能] 读取数据
	 * [参数]	buffer, 读取数据所存放的缓冲区
	 *			size,[in,out] 需要读取的长度, 已经读取的长度也通过该参数返回
	 *			wait_timeMs,	等待时间,0表示立即返回(unblock),-1表示一直等待直到读到所需数据 其他大于的值
							表示在该毫秒数内不管读到多少数据必须返回。默认为0。
	 * [返回值] BCTI_DTA_READ_SUC：数据正常读取
	 * 			BCTI_DTA_READ_SUC_MOVIE_END：数据正常读取，但是已读完该段影片，下次调用将是新影片的数据，播放器在得到该值后可能需要重置
	 * 			BCTI_DTA_READ_SUC_ALLEND：数据正常读取，而且已读完该所有影片
	 * 			BCTI_DTA_READ_SUC_CLIP_END：切片模式下，一个切片段读完
	 *			BCTI_DTA_READ_CANCLE:	被close打断
	 *			BCTI_DTA_READ_TIMEOUT:	超时。返回超时也有可能读取到数据
	 * 			BCTI_DTA_READ_ERR：出错
	 */
	virtual bcti_dta_read_e  Read(int8_dta* buffer, int64_dta *size, int32_dta wait_timeMs = 0) = 0;


	/* SeekByTime
	 * [功能] 用于判断UI指定的time offset是否需要额外处理
	 * [参数] time_offset：要定位的时间，单位毫秒
	 * 			toPos：DTA转化时间到长度的输出值，单位字节
	 * 			time_offset_inner：切片模式下，跨段seek后位于新切片段的偏移时间，单位毫秒
	 * 			clip_index：切片模式下，跨段seek后位于新切片段的索引
	 * [备注] 播放器不支持切片的，可以不考虑time_offset_inner和clip_index
	 * 			toPos用于播放器不能timeSeek的情况，由Dta来做时间转化的工作
	 * [返回值] BCTI_DTA_SEEKTIME_FAIL：播放器要自己做seek,此时任何输出参数无效
	 *			BCTI_DTA_SEEKTIME_CLIP:切片模式下的seek,如果clip_index>=0,则
	 * 					clip_index为跨切片段的索引，time_offset_inner为定位到新的切片段的时间偏移
	 * 			BCTI_DTA_SEEKTIME_SUC_TOPOS：Dta转化时间到Pos，并通过参数返回该Pos，播放器通过该Pos进行seek
	 * 			BCTI_DTA_SEEKTIME_SUC_NEWFILE：Dta转化后获取到的是一个新的文件，播放器需要重新codec过程
	 * 			BCTI_DTA_SEEKTIME_ERR：出错
	 */
	virtual bcti_dta_seek_e		SeekByTime(uint32_dta	time_offset,
											int64_dta*	toPos,
											uint32_dta *time_offset_inner,
											int32_dta *clip_index) = 0;

	/* SeekBySize
	 * [功能] 根据长度进行定位
	 * [参数] off会返回实际定位的pos，单位字节
	 * [返回值] BCTI_DTA_STATUS_SUCCESS：成功
	 * 			BCTI_DTA_STATUS_FAILURE：出错
	 */
	virtual bcti_dta_status_e 	SeekBySize(bcti_dta_seek_direction_e from, int64_dta *off) = 0;


	/* Percent
	 * [功能] 获取数据缓冲状态的百分比,一般情况下返回100之后才能读到数据
	 * [备注] 如果链接打开失败，函数返回0
	 * [返回值] 缓冲的百分比
	 */
	virtual uint32_dta 			Percent() = 0;

	/* ~CBctiDta
	* 析构函数
	*/
	virtual ~CBctiDta(){};
};

#endif	// BCTI_DTA_H__
