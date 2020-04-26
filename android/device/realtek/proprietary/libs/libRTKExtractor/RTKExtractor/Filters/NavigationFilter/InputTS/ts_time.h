#ifndef __TS_TIME_H__
#define __TS_TIME_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/**********************************************************************/
/*        trick play constant definition                              */
/**********************************************************************/
////////////////////////////////////////////////////////////////////////
#define TS_SPEED_NORMAL         256
#define TS_SPEED_MIN_SCAN       7680            // 30* 256
#define TS_INIT_FORWARD_SCAN_PTS        1000000
#define TS_INIT_REVERSE_SCAN_PTS        100000000000000

////////////////////////////////////////////////////////////////////////
#define MAX_FILE_LENGTH         128

/**********************************************************************/
/*        trick play dsta structure definition                        */
/**********************************************************************/
/*
typedef struct _ts_trick_t{
    int                         speed;
    int                         skip;
} ts_trick_t;
*/

// #define TS_LP

typedef struct _ts_io_position_t{
    off64_t                     pos_start;
    off64_t                     pos_current;
    off64_t                     pos_end;
#ifdef TS_LP
    off64_t                     pos_lower;
    off64_t                     pos_upper;
#endif // #ifdef TS_LP
} ts_io_position_t;

typedef struct _ts_status_pb_t{
    void*                       p_owner;
    unsigned char               p_file_name[MAX_FILE_LENGTH];
    ts_io_position_t            position_pb;
    ts_io_position_t*           p_position;
    int                         bit_rate; // bytes/second
    int                         threshold_empty;
    int                         speed;
    int                         skip;
    int                         skip_size_in_bytes;
    int                         read_packet_remain;
    int                         read_packet_total;
    unsigned int                n_frame_to_decode;
    int                         pts_delta;
    unsigned char               b_I_only;
    unsigned char               b_audio;
    unsigned char               b_subtitle;
} ts_status_pb_t;

typedef struct _ts_status_rec_t{
    void*                       p_owner;
    unsigned char               p_file_name[MAX_FILE_LENGTH];
    ts_io_position_t            position;
    int                         fd_rec;
} ts_status_rec_t;

typedef struct{
    int64_t                     pts_global;
    int64_t                     pts_last_render;
    int64_t                     pts_since_render;
    int64_t                     pts_last_demux;
    int64_t                     pts_first_frame;
    int64_t                     pts_skip;
} ts_time_render_t;

typedef struct{
    ts_status_pb_t              time_status_pb;
    ts_status_rec_t             time_status_rec;
    ts_time_render_t            time_render;
    // ts_trick_t                  time_trick;
} ts_time_set_t;

#define TS_TSHIFT_LP            (0x01)

typedef struct _ts_tshift_t{
    unsigned char               flag;
    unsigned char               b_enable;
    ts_status_pb_t*             p_status_pb;
    ts_status_rec_t*            p_status_rec;
} ts_tshift_t;

/**********************************************************************/
/*        ts time information api declaration                         */
/**********************************************************************/
int
ts_tshift_enable_check(
    ts_tshift_t*        p_tshift);

int
ts_status_pb_init(
    ts_status_pb_t*     p_status_pb,
    void*               p_owner);

void
ts_status_pb_set_bit_rate_stuff(
    ts_status_pb_t*     p_status_pb,
    int                 bit_rate,
    int                 len_packet);

int
ts_status_rec_init(
    ts_status_rec_t*    p_status_rec,
    void*               p_owner);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TS_TIME_H__
