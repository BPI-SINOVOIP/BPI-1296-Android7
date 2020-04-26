#ifndef __TS_IO_PRIV_H__
#define __TS_IO_PRIV_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "ts_io_handle.h"
#include "tsp.h"

typedef struct tsio_ops_t{
    // pid filter api
    int                    (*tsio_ops_pid_filters)(
                                void*                               handle,
                                tsp_pid_t*                          pid_array, 
                                int                                 n_pid,
                                TSIO_PID_FILTER_TYPE_E              pid_filter_type);
    // tuner api
#ifdef TS_AP_NAV_IF
    int                    (*tsio_ops_tuner_set)(
                                void*                               handle,
                                tsio_param_t*                       p_ts_param);
#else
    int                    (*tsio_ops_tuner_set)(
                                void*                               handle,
                                struct dvb_frontend_parameters*     p_dvb_arg);
#endif

    int                    (*tsio_ops_tuner_is_ready)(
                                void*           handle);

    // data api
    int                    (*tsio_ops_data_flush)(
                                void*           handle,
                                int             index_read);
    int                    (*tsio_ops_data_start)(
                                void*           handle);
    int                    (*tsio_ops_data_stop)(
                                void*           handle);
    int                    (*tsio_ops_data_is_start)(
                                void*           handle);
    int                    (*tsio_ops_data_read_set_status)(
                                void*           handle,
                                int             index_read,
                                int             on_off);
    int                    (*tsio_ops_data_read_get_status)(
                                void*           handle,
                                int             index_read);
    int                    (*tsio_ops_data_read)(
                                void*           handle,
                                unsigned char** pp_buffer,
                                int*            p_len_buffer,
                                int             index_read,
                                int             b_sync);
/*
    int                    (*tsio_ops_data_read_raw)(
                                void*           handle,
                                unsigned char** pp_buffer,
                                int*            p_len_buffer,
                                int             index_read);
*/
    int                    (*tsio_ops_data_put)(
                                void*           handle,
                                unsigned char*  p_buffer,
                                int             len_buffer,
                                int             index_read);
    int                    (*tsio_ops_data_release)(
                                void*           handle,
                                unsigned char*  p_buffer,
                                int             len_buffer,
                                int             index_read);
    int                    (*tsio_ops_data_format)(
                                void*           tsio_handle,
                                unsigned char*  p_len_packet,
                                unsigned char*  p_offset_packet,
                                unsigned char*  p_padding_packet);
    off64_t                (*tsio_ops_data_seek)(
                                void*           tsio_handle,
                                off64_t         offset,
                                int             whence);
    int                    (*tsio_ops_data_seek_cap)(
                                void*                   tsio_handle,
                                tsio_seek_cap_t*        p_seek_cap);
    int                    (*tsio_ops_data_write_unit)(
                                void*           tsio_handle,
                                int*            p_write_unit);
    int                    (*tsio_ops_data_write_address)(
                                void*           tsio_handle,
                                unsigned char** pp_writeable_buffer,
                                unsigned char*  p_buffer,
                                int             len_buffer);
} tsio_ops_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
