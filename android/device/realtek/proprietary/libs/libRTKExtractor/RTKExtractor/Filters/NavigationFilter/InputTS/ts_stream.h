
#ifndef __TS_STREAM_H__
#define __TS_STREAM_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

int
ts_stream_read(
    ts_nav_t*                   p_ts_nav,
    int*                        p_b_pb_catch_rec);

/*
int
ts_stream_release_buffer(
    void*                  mem_handle,
    pes_buf_t*             p_pes_buf);
*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
