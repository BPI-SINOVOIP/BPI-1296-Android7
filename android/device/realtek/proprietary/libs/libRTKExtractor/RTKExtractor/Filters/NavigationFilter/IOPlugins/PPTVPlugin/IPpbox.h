// IPpbox.h

#ifndef _PPBOX_PPBOX_I_PPBOX_H_
#define _PPBOX_PPBOX_I_PPBOX_H_

#ifdef WIN32
# define PPBOX_DECL __declspec(dllimport)
#else
# define PPBOX_DECL
#endif

typedef char PP_char;
typedef bool PP_bool;
typedef unsigned char PP_uchar;
typedef long PP_int32;
typedef unsigned short PP_uint16;
typedef unsigned long PP_uint32;
typedef unsigned long long PP_uint64;

enum PPBOX_ErrorEnum
{
    ppbox_success = 0, 
    ppbox_not_start, 
    ppbox_already_start, 
    ppbox_not_open, 
    ppbox_already_open, 
    ppbox_operation_canceled, 
    ppbox_would_block, 
    ppbox_stream_end, 
    ppbox_logic_error, 
    ppbox_network_error, 
    ppbox_demux_error, 
    ppbox_certify_error, 
    ppbox_other_error = 1024, 
};

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_int32 PPBOX_StartP2PEngine(
        PP_char const * gid, 
        PP_char const * pid, 
        PP_char const * auth);

    PPBOX_DECL void PPBOX_StopP2PEngine();

    //获得错误码
    //返回值: 错误码
    PPBOX_DECL PP_int32 PPBOX_GetLastError();

    PPBOX_DECL PP_char const * PPBOX_GetLastErrorMsg();

    PPBOX_DECL PP_char const * PPBOX_GetVersion();

    PPBOX_DECL void PPBOX_SetConfig(
        PP_char const * module, 
        PP_char const * section, 
        PP_char const * key, 
        PP_char const * value);

    PPBOX_DECL void PPBOX_DebugMode(
        PP_bool mode);

    typedef struct tag_DialogMessage
    {
        PP_uint32 time;         // time(NULL)返回的值
        PP_char const * module; // \0结尾
        PP_uint32 level;        // 日志等级
        PP_uint32 size;         // msg的长度，不包括\0结尾
        PP_char const * msg;    // \0结尾
    } DialogMessage;

    // vector:保存日志信息的buffer
    // size:期望获取日志的条数
    // module:获取日志的模块名称，（NULL 获取任意）
    // level <= 日志等级
    // 返回值：实际获取日志的条数
    // 返回0表示没有获取到任何日志。
    PPBOX_DECL PP_uint32 PPBOX_DialogMessage(
        DialogMessage * vector, 
        PP_int32 size, 
        PP_char const * module, 
        PP_int32 level);

    PPBOX_DECL void PPBOX_SubmitMessage(
        PP_char const * msg, 
        PP_int32 size);

    typedef void (*PPBOX_Callback)(void *, PP_int32);

    typedef void (*PPBOX_OnLogDump)( PP_char const *, PP_int32 );

    PPBOX_DECL void PPBOX_LogDump(
        PPBOX_OnLogDump callback,
        PP_int32 level);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_PPBOX_H_
