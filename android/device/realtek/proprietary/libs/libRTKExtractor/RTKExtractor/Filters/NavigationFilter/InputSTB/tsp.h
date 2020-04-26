#ifndef __TSP_H__
#define __TSP_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "OSAL.h"
#include <netinet/in.h>

// some switchs
#define DMA_FILE_FAKE

// some constant
#define TSP_MAX_TABLE_SECTION_SIZE      1024
#define TSP_PACKET_SIZE                 188

typedef uint32_t                        tsp_pid_t;
typedef uint8_t                         tsp_tid_t;
typedef uint32_t                        tsp_pgid_t;
typedef uint8_t                         tsp_stream_type_t;

typedef enum{
    stb_vm_stream_na_e          = -1,
    stb_vm_stream_start_e       = 0,
    stb_vm_stream_video_e       = 0,
    stb_vm_stream_audio_e       = 1,
    stb_vm_stream_spu_e         = 2,
    stb_vm_stream_end_e         = 3
} stb_vm_stream_cat_t;

// packet id list
#define TSP_PID_PAT                     (tsp_pid_t)(0x0000)
#define TSP_PID_CAT                     (tsp_pid_t)(0x0001)
#define TSP_PID_TSDT                    (tsp_pid_t)(0x0002)
// based on spec, the null packet should be filtered by hardware.
// i.e. from the software's viewpoint, we never see null packets.
// consequently, no null packet need to be handled by software.
#define TSP_PID_NULL                    (tsp_pid_t)(0x1fff)
#define TSP_PID_INVALID                 (tsp_pid_t)(UINT_MAX)

// Table id list
#define TSP_TID_PAT                     (tsp_tid_t)(0x00)
#define TSP_TID_CAT                     (tsp_tid_t)(0x01)
#define TSP_TID_PMT                     (tsp_tid_t)(0x02)
#define TSP_TID_TSDT                    (tsp_tid_t)(0x03)
#define TSP_TID_INVALID                 (tsp_tid_t)(UCHAR_MAX)

// PG id list
#define TSP_PGID_NIT                    (tsp_pgid_t)(0x0)
#define TSP_PGID_INVALID                (tsp_pgid_t)(UINT_MAX)

// ======================================================================================
// 
// ======================================================================================
typedef struct{
    uint32_t                            continuity_counter              :  4;
    uint32_t                            adaptation_field_control        :  2;
    uint32_t                            transport_scrambling_control    :  2;
    uint32_t                            PID                             : 13;
    uint32_t                            transport_priority              :  1;
    uint32_t                            payload_unit_start_indicator    :  1;
    uint32_t                            transport_error_indicator       :  1;
    uint32_t                            sync_byte                       :  8;
} tsp_header_t;

#if 0
// ======================================================================================
// pat table structure
// ======================================================================================
#define MAX_PAT_SIZE                    40
#define MAX_PMT_SIZE                    40

typedef struct _tsp_element_pmt_t{
    tsp_stream_type_t                   stream_type;
    // @FIXME: is it a good org for putting stb_vm_stream_cat_t in stb_vm.h
    stb_vm_stream_cat_t                 stream_cat;
    tsp_pid_t                           es_pid;
    unsigned int                        flag;
} tsp_element_pmt_t;

typedef struct _tsp_table_pmt_t{
    tsp_element_pmt_t                   p_pmt_array[MAX_PMT_SIZE];
    tsp_pgid_t                          pgid;
    int                                 n_es_pid;
} tsp_table_pmt_t;

typedef struct _tsp_element_pat_t{
    tsp_pgid_t                          pgid;
    tsp_pid_t                           pmt_pid;
    tsp_table_pmt_t*                    p_table_pmt;
    unsigned int                        flag;
} tsp_element_pat_t;
 
typedef struct _tsp_table_pat_t{
    tsp_element_pat_t                   p_pat_array[MAX_PAT_SIZE];
    int                                 n_pg;
} tsp_table_pat_t;
#endif

// ======================================================================================
// some useful macros for header information
// ======================================================================================
// #define tsp_parse_header(pTS)              ((tsp_header_t)((htonl(*((u_long*)(pTS))))))
#define tsp_parse_header(ts_header, pTS)   (*((u_long*)(void*)&(ts_header))= htonl(*((u_long*)(pTS))))
#define tsp_has_adaptation(ts_header)      (((tsp_header_t)(ts_header)).adaptation_field_control & 0x2)
#define tsp_has_payload(ts_header)         (((tsp_header_t)(ts_header)).adaptation_field_control & 0x1)
#define tsp_adaptation_length(p_ts)        (*(p_ts)+ 1)

// #define tsp_has_1394_time_stamp()          (192== TSP_PACKET_SIZE)

#if 0
// ======================================================================================
// ts parser api                            
// ======================================================================================
// PAT table api
int
tsp_table_add_pat(
    tsp_table_pat_t*                    p_table_pat,
    int*                                p_b_cot,
    tsp_pgid_t                          pgid,
    tsp_pid_t                           pmt_pid);

int
tsp_reset_element_pat(
    tsp_element_pat_t*                  p_element_pat);

int
tsp_table_rehash_pat(
    tsp_table_pat_t*                    p_table_pat);
    
int
tsp_is_pg_in_table_pat(
    tsp_table_pat_t*                    p_table_pat,
    tsp_pgid_t                          pg_id);

// PMT table api
int
tsp_table_add_pmt(
    tsp_table_pmt_t*                    p_table_pmt,
    int*                                p_b_cot,
    tsp_stream_type_t                   stream_type,
    tsp_pid_t                           es_pid,
    BYTE*                               p_descriptor,
    int                                 len_descriptor);

int
tsp_reset_element_pmt(
    tsp_element_pmt_t*                  p_element_pmt);

int
tsp_table_rehash_pmt(
    tsp_table_pat_t*                    p_table_pat,
    int                                 pg_index);
#endif

#if 0
int
tsp_parse_pat(
    void*                       p_table,
    int*                        p_b_cot,
    int*                        p_b_eot,
    BYTE*                       p_next_data,
    int                         len_next_data,
    unsigned int                pusi);

int
tsp_parse_pmt(
    void*                       p_table,
    int*                        p_b_cot,
    int*                        p_b_eot,
    BYTE*                       p_next_data,
    int                         len_next_data,
    unsigned int                pusi);

int
tsp_parse_cat(
    void*                       p_table,
    int*                        p_b_cot,
    int*                        p_b_eot,
    BYTE*                       p_next_data,
    int                         len_next_data,
    unsigned int                pusi);
#endif

/*
tsp_pid_t
tsp_table_pmt_find_valid_pid(
    tsp_table_pat_t*                    p_table_pat,
    int                                 pg_index,
    stb_vm_stream_cat_t                 stream_cat);
*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TSP_H__
