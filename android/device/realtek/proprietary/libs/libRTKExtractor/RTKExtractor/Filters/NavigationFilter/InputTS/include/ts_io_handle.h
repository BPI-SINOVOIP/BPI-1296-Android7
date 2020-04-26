#ifndef __TS_IO_HANDLE_H__
#define __TS_IO_HANDLE_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include <unistd.h>
#include <sys/types.h>
#include "frontend.h"
#include "tsp.h"
#include "Filters/NavigationFilter/NavPlugins.h"

/**********************************************************************/
/*        ts_io_handle data structure definition                      */
/**********************************************************************/
typedef struct _tsio_handle_t*  tsio_handle_t;

/*
typedef struct _tsio_format_ts_packet_t{
    unsigned char   len_packet;
    unsigned char   offset_packet;
    unsigned char   padding_packet;
} tsio_format_ts_packet_t;
*/

typedef struct _tsio_seek_cap{
    unsigned char   b_seek;
} tsio_seek_cap_t;

/**********************************************************************/
/*        ts_io_handle constant declaration                           */
/**********************************************************************/
typedef enum{
    TSIO_PID_FILTER_EMPTY_E,
    TSIO_PID_FILTER_INSERT_E,
    TSIO_PID_FILTER_REMOVE_E,
    TSIO_PID_FILTER_SET_E
} TSIO_PID_FILTER_TYPE_E;

/**********************************************************************/
/*        return flag from tsio_data_read                             */
/**********************************************************************/
#define TSIO_DATA_READ_WRAP     0x00000001      // ring buffer wrap around
#define TSIO_DATA_READ_EOS      0x00000002      // end of stream

/**********************************************************************/
/*        input seeking mode for tsio_data_seek                       */
/**********************************************************************/
#define TSIO_DATA_SEEK_SET      0x00000001
#define TSIO_DATA_SEEK_CUR      0x00000002
#define TSIO_DATA_SEEK_END      0x00000004

/**********************************************************************/
/*        ts_io_handle public APIs                                    */
/**********************************************************************/
// transport stream i/o handle create/destroy function
tsio_handle_t
tsio_create(
    char*                       p_mrl,
    char*                       p_file_name,
    IOPLUGIN*                   p_plugin_io);

int
tsio_destroy(
    tsio_handle_t               tsio_handle);

// transport stream pid filter api
int
tsio_pid_filter(
    tsio_handle_t               tsio_handle,
    tsp_pid_t                   pid,
    TSIO_PID_FILTER_TYPE_E      pid_filter_type);

int
tsio_pid_filters(
    tsio_handle_t               tsio_handle,
    tsp_pid_t*                  pids,
    int                         n_pid,
    TSIO_PID_FILTER_TYPE_E      pid_filter_type);

// transport stream tuner api
// the below tsio_tuner_set api is just a temporary interface.
// Need to be refined later
#ifdef TS_AP_NAV_IF
int
tsio_tuner_set(
    tsio_handle_t                       tsio_handle,
    tsio_param_t*                       p_ts_param);

#else
int
tsio_tuner_set(
    tsio_handle_t                       tsio_handle,
    struct dvb_frontend_parameters*     p_dvb_arg);
#endif
    
int
tsio_tuner_is_ready(
    tsio_handle_t               tsio_handle);

// transport stream data api
int
tsio_data_flush(
    tsio_handle_t               tsio_handle,
    int                         index_read);

int
tsio_data_start(
    tsio_handle_t               tsio_handle);

int
tsio_data_stop(
    tsio_handle_t               tsio_handle);

int
tsio_data_is_start(
    tsio_handle_t               tsio_handle);

int
tsio_data_read_set_status(
    tsio_handle_t               tsio_handle,
    int                         index_read,
    int                         on_off);

int
tsio_data_read_get_status(
    tsio_handle_t               tsio_handle,
    int                         index_read);

int
tsio_data_read(
    tsio_handle_t               tsio_handle,
    unsigned char**             pp_buffer,
    int*                        p_len_buffer,
    int                         index_read,
    int                         b_sync);

/*
int
tsio_data_read_raw(
    tsio_handle_t               tsio_handle,
    unsigned char**             pp_buffer,
    int*                        p_len_buffer,
    int                         index_read);
*/

int
tsio_data_put(
    tsio_handle_t               tsio_handle,
    unsigned char*              p_buffer,
    int                         len_buffer,
    int                         index_read);

int
tsio_data_release(
    tsio_handle_t               tsio_handle,
    unsigned char*              p_buffer,
    int                         len_buffer,
    int                         index_read);

int
tsio_data_format(
    tsio_handle_t               tsio_handle,
    unsigned char*              p_len_packet,
    unsigned char*              p_offset_packet,
    unsigned char*              p_padding_packet);

off64_t
tsio_data_seek(
    tsio_handle_t               tsio_handle,
    off64_t                     offset,
    int                         whence);

int
tsio_data_seek_cap(
    tsio_handle_t               tsio_handle,
    tsio_seek_cap_t*            p_seek_cap);

int
tsio_data_write_unit(
    void*                       tsio_handle,
    int*                        p_write_unit);

int
tsio_data_write_address(
    void*                       tsio_handle,
    unsigned char**             pp_writeable_buffer,
    unsigned char*              p_buffer,
    int                         len_buffer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TS_IO_HANDLE_H__
