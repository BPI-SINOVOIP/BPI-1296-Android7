// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_DEMUXER_H_
#define _PPBOX_PPBOX_I_DEMUXER_H_

#include "IPpbox.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //打开一个视频
    PPBOX_DECL PP_int32 PPBOX_Open(
        PP_char const * playlink);

    typedef void (*PPBOX_Open_Callback)(PP_int32);

    PPBOX_DECL void PPBOX_AsyncOpen(
        PP_char const * playlink, 
        PPBOX_Open_Callback callback);

    //暂停
    PPBOX_DECL PP_int32 PPBOX_Pause();

    //强制结束
    PPBOX_DECL void PPBOX_Close();

    //获得有多少流
    PPBOX_DECL PP_uint32 PPBOX_GetStreamCount();

    enum PPBOX_StreamTypeEnum
    {
        ppbox_video = 1, 
        ppbox_audio = 2, 
    };

    enum PPBOX_StreamSubTypeEnum
    {
        ppbox_video_avc = 1, 
        ppbox_audio_aac = 2, 
        ppbox_audio_mp3 = 3, 
        ppbox_audio_wma = 4,
    };

    enum PPBOX_StreamFormatTypeEnum
    {
        ppbox_video_avc_packet = 1, 
        ppbox_video_avc_byte_stream = 2, 
        ppbox_audio_microsoft_wave = 3, 
        ppbox_audio_iso_mp4 = 4, 
    };

    typedef struct tag_PPBOX_StreamInfo
    {
        PP_int32 type;          // TypeEnum
        PP_int32 sub_type;      // SubTypeEnum
        PP_int32 format_type;   // 格式说明的类型
        PP_uint32 format_size;  // 格式说明的大小
        PP_uchar const * format_buffer;     // 不同的解码不同的结构体，格式说明的的内容
    } PPBOX_StreamInfo;

    //获得流的详细信息
    PPBOX_DECL PP_int32 PPBOX_GetStreamInfo(
        PP_uint32 index, 
        PPBOX_StreamInfo * info);

    typedef struct tag_PPBOX_VideoInfo
    {
        PP_uint32 width;
        PP_uint32 height;
        PP_uint32 frame_rate;
    } PPBOX_VideoInfo;

    typedef struct tag_PPBOX_AudioInfo
    {
        PP_uint32 channel_count;
        PP_uint32 sample_size;
        PP_uint32 sample_rate;
    } PPBOX_AudioInfo;

    typedef struct tag_PPBOX_StreamInfoEx
    {
        PP_int32 type;          // TypeEnum
        PP_int32 sub_type;      // SubTypeEnum
        union 
        {
            PPBOX_VideoInfo video_format;
            PPBOX_AudioInfo audio_format;
        };
        PP_int32 format_type;   // 格式说明的类型
        PP_uint32 format_size;  // 格式说明的大小
        PP_uchar const * format_buffer;   // 不同的解码不同的结构体，格式说明的的内容
    } PPBOX_StreamInfoEx;

    //获得流的详细扩展信息
    PPBOX_DECL PP_int32 PPBOX_GetStreamInfoEx(
        PP_uint32 index,
        PPBOX_StreamInfoEx *info);

    //获得总时长
    PPBOX_DECL PP_uint32 PPBOX_GetDuration();

    //获得播放长宽，像素
    PPBOX_DECL PP_int32 PPBOX_GetWidthHeight(
        PP_uint32 * pwidth, 
        PP_uint32 * pheight);

    //跳到某个时刻开始播放
    PPBOX_DECL PP_int32 PPBOX_Seek(
        PP_uint32 start_time);

    //获得AVC音频编码的AVCDecoderConfigurationRecord参数
    PPBOX_DECL PP_int32 PPBOX_GetAvcConfig(
        PP_uchar const * * buffer, 
        PP_uint32 * length);

#ifdef PPBOX_DEMUX_RETURN_SEGMENT_INFO

    PPBOX_DECL PP_int32 PPBOX_GetFirstSegHeader(
        PP_uchar const * * buffer, 
        PP_uint32 * length);

    PPBOX_DECL PP_int32 PPBOX_GetSegHeader(
        PP_uint32 index, 
        PP_uchar const * * buffer, 
        PP_uint32 * length);

    PPBOX_DECL PP_uint32 PPBOX_GetSegDataSize(
        PP_uint32 index);

    typedef struct tag_PPBOX_SegmentInfo
    {
        PP_uint32 index;
        PP_uint32 duration;
        PP_uint32 duration_offset;
        PP_uint32 head_length;
        PP_uint64 file_length;
        PP_uchar const * head_buffer;
    } PPBOX_SegmentInfo;

    PPBOX_DECL PP_uint32 PPBOX_GetSegmentCount();

    PPBOX_DECL PP_int32 PPBOX_GetSegmentInfo(
        PPBOX_SegmentInfo * segment_info);

#endif

    typedef struct tag_PPBOX_Sample
    {
        PP_uint32 stream_index;     // 流的编号
        PP_uint32 start_time;       // Sample对应的时间戳, 单位是毫秒
        PP_uint32 buffer_length;    // Sample的大小
        PP_uchar const * buffer;    // Sample的内容
        //当调用本函数的后，我们申请内存然后给他Sample
        //当调用下一个ReadSample函数的时候，或者Stop的时候，内存释放掉
    } PPBOX_Sample;

    //同步读取Sample接口，不阻塞
    PPBOX_DECL PP_int32 PPBOX_ReadSample(
        PPBOX_Sample * sample);

    typedef struct tag_PPBOX_SampleEx
    {
        PP_uint32 stream_index;     // 流的编号
        PP_uint32 start_time;       // Sample对应的时间戳, 单位是毫秒
        PP_uint64 offset_in_file;
        PP_uint32 buffer_length;    // Sample的大小
        PP_uint32 duration;
        PP_uint32 desc_index;
        PP_uint64 decode_time;
        PP_uint32 composite_time_delta;
        PP_bool is_sync;
        PP_uchar const * buffer;    // Sample的内容
        //当调用本函数的后，我们申请内存然后给他Sample
        //当调用下一个ReadSample函数的时候，或者Stop的时候，内存释放掉
    } PPBOX_SampleEx;

    PPBOX_DECL PP_int32 PPBOX_ReadSampleEx(
        PPBOX_SampleEx * sample);

    typedef struct tag_PPBOX_SampleEx2
    {
        PP_uint32 stream_index;     // 流的编号
        PP_uint64 start_time;       // Sample对应的时间戳, 单位是微妙
        PP_uint32 buffer_length;    // Sample的大小
        PP_uint32 duration;
        PP_uint32 desc_index;
        PP_uint64 decode_time;
        PP_uint32 composite_time_delta;
        PP_bool is_sync;
        PP_bool is_discontinuity;   // 该位标识此帧与上一帧之间是否是连续的sample; 如果该sample是seek后的sample或者前面丢失了sample,则该位置1，否则为0。
        PP_uchar const * buffer;    // Sample的内容
        //当调用本函数的后，我们申请内存然后给他Sample
        //当调用下一个ReadSample函数的时候，或者Stop的时候，内存释放掉
    } PPBOX_SampleEx2;

    PPBOX_DECL PP_int32 PPBOX_ReadSampleEx2(
        PPBOX_SampleEx2 * sample);

    //设置下载缓冲区大小 （只能在Open前调用）
    // 主要用于控制内存，如果下载速度大于ReadSample的速度，那么下载的数据将存放
    // 于内存之中，当内存中的下载缓冲大于这个预设值，那么将停止下载。直到被调用了
    // ReadSample，少了一些内存占用后，再继续下载，
    // length: 预设的下载内存缓冲的大小
    PPBOX_DECL void PPBOX_SetDownloadBufferSize(
        PP_uint32 length);

    PPBOX_DECL void PPBOX_SetHttpProxy(
        PP_char const * addr);

    PPBOX_DECL void PPBOX_SetDownloadMaxSpeed(
        PP_uint32 speed);

    //设置播放缓冲区的缓冲时间 (随时可以调用)
    // 主要用于计算播放状态，如果不调用这个函数，默认3s
    // 如果 下载缓冲区数据的总时间 < 播放缓冲时间 则 处于 buffering 状态
    // 如果 下载缓冲区数据的总时间 >=播放缓冲时间 则 处于 playing 状态
    // 如果 人为调用了 Pause 使之暂停的，则处于 Pausing 状态
    // 如果处于buffering状态，下载缓冲区数据的总时间/播放缓冲时间*100%就是缓冲百分比
    PPBOX_DECL void PPBOX_SetPlayBufferTime(
        PP_uint32 time);

    enum PPBOX_PlayStatusEnum
    {
        ppbox_closed = 0, 
        ppbox_playing, 
        ppbox_buffering, 
        ppbox_paused, 
    };

    typedef struct tag_PPBOX_PlayStatistic
    {
        PP_uint32 length;           //本结构体的长度
        PP_int32 play_status;       //播放状态 0-未启动 1-playing态 2-buffering态 3-Pausing态
        PP_uint32 buffering_present;//播放缓冲百分比 10 表示 10%
        PP_uint32 buffer_time;      //下载缓冲区数据的总时间
    } PPBOX_PlayStatistic;

    //获得播放信息
    //返回值: 错误码
    //    ppbox_success      表示成功
    //    其他数值表示失败
    PPBOX_DECL PP_int32 PPBOX_GetPlayMsg(
        PPBOX_PlayStatistic * statistic_Msg);

    typedef struct tag_PPBOX_DownloadMsg
    {
        PP_uint32 length;                   // 本结构体的长度
        PP_uint32 start_time;               // 开始时刻
        PP_uint32 total_download_bytes;     // 总共下载的字节数
        PP_uint32 total_upload_bytes;       // 总共上传时字节数
        PP_uint32 http_download_bytes;      // Http下载的字节数
        PP_uint32 http_downloader_count;    // Http下载个数
        PP_uint32 p2p_download_bytes;       // P2P下载的字节数
        PP_uint32 p2p_upload_bytes;         // P2P上传的字节数
        PP_uint32 p2p_downloader_count;     // P2P下载个数
        PP_uint32 p2p_downloader_count_ext; // 候补P2P下载的资源个数
        PP_uint32 total_upload_cache_request_count; // 总共的上传Cache请求数
        PP_uint32 total_upload_cache_hit_count;     // 总共的上传Cache命中数
        PP_uint32 download_duration_in_sec; // 下载总共持续时长(秒)
        PP_uint32 local_peer_version;       // 自己内核版本号
    } PPBOX_DownloadMsg;

    //获得下载信息
    // download_Msg: 调用这个接口后，用于获取的下载信息数据
    //返回值: 错误码
    //    ppbox_success      表示成功
    //    其他数值表示失败
    PPBOX_DECL PP_int32 PPBOX_GetDownMsg(
        PPBOX_DownloadMsg* download_Msg);

    typedef struct tag_PPBOX_DownloadSpeedMsg
    {
        PP_uint32 length;                   // 本结构体的长度
        PP_uint32 now_download_speed;       // 当前下载速度 <5s统计>
        PP_uint32 now_upload_speed;         // 当前上传速度 <5s统计>
        PP_uint32 minute_download_speed;    // 最近一分钟平均下载速度 <60s统计>
        PP_uint32 minute_upload_speed;      // 最近一分钟平均上传速度 <60s统计>
        PP_uint32 avg_download_speed;       // 历史平均下载速度
        PP_uint32 avg_upload_speed;         // 历史平均上传速度

        PP_uint32 recent_download_speed;    // 当前下载速度 <20s统计>
        PP_uint32 recent_upload_speed;      // 当前上传速度 <20s统计>
        PP_uint32 second_download_speed;    // 当前1s的下载速度
        PP_uint32 second_upload_speed;      // 当前1s的上传速度
    } PPBOX_DownloadSpeedMsg;

    //获得下载速度信息
    // download_Msg: 调用这个接口后，用于获取的下载信息数据
    //返回值: 错误码
    //    ppbox_success      表示成功
    //    其他数值表示失败
    PPBOX_DECL PP_int32 PPBOX_GetDownSedMsg(
        PPBOX_DownloadSpeedMsg* download_spped_Msg);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
