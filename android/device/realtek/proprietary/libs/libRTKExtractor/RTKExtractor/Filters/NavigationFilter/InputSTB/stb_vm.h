#ifndef __STB_VM_H__
#define __STB_VM_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "OSAL.h"
#include "tsp.h"

typedef struct stb_vm_t*        stb_vm_id;

/*
// info type
// abcdefgh
#define STB_VM_BUF_VIDEO_MASK           0x80000000
#define STB_VM_BUF_AUDIO_MASK           0x40000000
#define STB_VM_BUF_SPU_MASK             0x20000000
#define STB_VM_BUF_PROG_MASK            0x10000000
*/

// stb_vm_buf_t info type
#define STB_VM_INFO_TYPE_NA             0x00000000
#define STB_VM_INFO_TYPE_PSI            0x00000001
#define STB_VM_INFO_TYPE_STREAM         0x00000002

// stb_vm_buf_t info
// (1) STB_VM_INFO_TYPE_PSI

// (2) STB_VM_INFO_TYPE_STREAM
//     this info corresponding to STB_VM_INFO_TYPE_STREAM is stream_descriptor
//     whose format is depicted in nav_content_src.h

// ======================================================================================
// data structure
// ======================================================================================
// a vm handle which all api require
typedef struct _stb_vm_buf_t{
    BYTE*               p_data;
    int                 len_data;
    int                 info_type;
    int32_t             info;
} stb_vm_buf_t;

// ======================================================================================
// stb_vm api collections
// ======================================================================================
int
stb_vm_is_psi(
    stb_vm_id           p_stb_vm,
    tsp_tid_t*          p_tid,
    tsp_pid_t           pid);

int
stb_vm_parse_psi(
    stb_vm_id           p_stb_vm,
    stb_vm_buf_t*       p_vm_buf,
    BYTE*               p_psi,
    int                 len_psi,
    tsp_tid_t           tid,
    unsigned int        pusi);

int
stb_vm_parse_stream(
    stb_vm_id           p_stb_vm,
    stb_vm_buf_t*       p_vm_buf,
    BYTE*               p_payload,
    int                 len_payload,
    tsp_pid_t           pid,
    unsigned int        pusi);

#if 0
int
stb_vm_parse_payload(
    stb_vm_id           p_stb_vm,
    stb_vm_buf_t*       p_vm_buf,
    BYTE*               p_payload,
    int                 len_payload,
    tsp_pid_t           pid,
    unsigned int        pusi);
#endif

stb_vm_id
stb_vm_create(
    char*               p_mrl);

int
stb_vm_delete(
    stb_vm_id           p_stb_vm);
    
int
stb_vm_read(
    stb_vm_id           p_stb_vm,
    BYTE**              pp_buffer,
    int*                p_length);

int
stb_vm_flush(
    stb_vm_id           p_stb_vm);

int
stb_vm_program_next(
    stb_vm_id           p_stb_vm);

int
stb_vm_program_prev(
    stb_vm_id           p_stb_vm);

/*
int
stb_vm_pid_filter(
    stb_vm_t*           p_stb_vm,
    tsp_pid_t*          p_pid_array,
    int                 len_pid_array);

int
stb_vm_pg_set(
    stb_vm_t*           p_stb_vm,
    tsp_pgid_t          pg_id);

int
stb_vm_pg_pid_set(
    stb_vm_t*           p_stb_vm,
    tsp_pid_t           pid,
    stb_vm_stream_cat_t stream_index);

tsp_pgid_t
stb_vm_pg_get(
    stb_vm_t*           p_stb_vm);

tsp_pid_t
stb_vm_pg_pid_get(
    stb_vm_t*           p_stb_vm,
    stb_vm_stream_cat_t stream_index);
*/


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __STB_VM_H__
