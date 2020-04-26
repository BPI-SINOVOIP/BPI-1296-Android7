#ifndef __TSP_PROGRAM_H__
#define __TSP_PROGRAM_H__

#include "tsp_psi.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

typedef struct _tsp_pid_pusi_t{
    tsp_pid_t           pid;
    unsigned int        pusi;
} tsp_pid_pusi_t;

typedef struct _tsp_program_t{
    int                 pg_index;
    int                 pmt_index_array[stb_vm_stream_end_e];
    tsp_pid_pusi_t      pid_pusi_array[stb_vm_stream_end_e];
    tsp_table_pat_t     table_pat;
} tsp_program_t;

int
tsp_pg_init(
    tsp_program_t*      p_ts_program);

int
tsp_pg_reset(
    tsp_program_t*     p_ts_program);

// program id api
int
tsp_pg_set(
    tsp_program_t*      p_ts_program,
    tsp_pgid_t          pgid);

int
tsp_pg_set_default(
    tsp_program_t*      p_ts_program);

int
tsp_pg_is_current_invalid(
    tsp_program_t*      p_ts_program);

int
tsp_pg_next(
    tsp_program_t*      p_ts_program);

int
tsp_pg_prev(
    tsp_program_t*      p_ts_program);

// stream api in the program
int
tsp_pg_stream_is_current_invalid(
    tsp_program_t*      p_ts_program,
    stb_vm_stream_cat_t stream_cat);

int
tsp_pg_stream_set_default(
    tsp_program_t*      p_ts_program,
    stb_vm_stream_cat_t stream_cat);

uint32_t
tsp_pg_stream_descriptor_get(
    tsp_program_t*      p_ts_program,
    tsp_pid_t           pid);

// other api
tsp_pid_t
tsp_pg_pmt_pid_get(
    tsp_program_t*      p_ts_program);

tsp_table_pmt_t*
tsp_pg_table_pmt_get(
    tsp_program_t*      p_ts_program);

tsp_table_pat_t*
tsp_pg_table_pat_get(
    tsp_program_t*      p_ts_program);

int
tsp_pg_all_pid_get(
    tsp_program_t*      p_ts_program,
    tsp_pid_t*          p_pid_array,
    int*                p_n_pid);

int
tsp_pg_is_pid_ready(
    tsp_program_t*      p_ts_program,
    tsp_pid_t           pid);

int
tsp_pg_set_pid_ready(
    tsp_program_t*      p_ts_program,
    tsp_pid_t           pid);

/*
int
tsp_pg_reset_ready(
    tsp_program_t*      p_ts_program);
*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TSP_PROGRAM_H__
