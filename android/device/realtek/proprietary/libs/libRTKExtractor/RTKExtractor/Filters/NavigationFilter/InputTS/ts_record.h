#ifndef __TS_RECORD_H__
#define __TS_RECORD_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "ts_io_handle.h"

/*************************************************************************/
/*  The below is public function declaration                             */
/*************************************************************************/
typedef struct _ts_rec_handle_t*        ts_rec_handle_t;

ts_rec_handle_t
ts_record_thread_create(
    tsio_handle_t       ts_io_handle,
    tspg_sm_t           ts_pg_sm);

int
ts_record_thread_destroy(
    ts_rec_handle_t     ts_rec_handle);

int
ts_record_start(
    ts_rec_handle_t     ts_rec_handle,
    ts_status_rec_t*    p_status_rec,
    int64_t             lp_size);

int
ts_record_stop(
    ts_rec_handle_t     ts_rec_handle);

unsigned int
ts_record_time_elapse(
    ts_rec_handle_t     ts_rec_handle);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef __TS_RECORD_H__
