#ifndef __TSP_PSI_H__
#define __TSP_PSI_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// ======================================================================================
// pat table structure
// ======================================================================================
#define MAX_PAT_SIZE                    40
#define MAX_PMT_SIZE                    40

typedef struct _tsp_prev_buf_psi{
    BYTE*                               p_buf_psi[TSP_MAX_TABLE_SECTION_SIZE];
    int                                 len_buf;
} tsp_prev_buf_psi;

typedef struct _tsp_element_pmt_t{
    tsp_stream_type_t                   stream_type;
    stb_vm_stream_cat_t                 stream_cat;
    tsp_pid_t                           es_pid;
    unsigned int                        flag;
    uint32_t                            stream_descriptor;
    char                                audio_lang_code[4]; // ISO 639 language code
                                                      // 3 letters ended up with '\0' denoting end of string
} tsp_element_pmt_t;

typedef struct _tsp_table_pmt_t{
    tsp_element_pmt_t                   p_pmt_array[MAX_PMT_SIZE];
    tsp_pgid_t                          pgid;
    // tsp_pid_t                           pmt_pid;
    int                                 n_es_pid;
    tsp_prev_buf_psi                    buf_pmt;
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
    tsp_prev_buf_psi                    buf_pat;
} tsp_table_pat_t;

typedef struct _tsp_table_cat_t{
} tsp_table_cat_t;

int
tsp_parse_pat(
    tsp_table_pat_t*            p_table,
    int*                        p_b_cot,
    int*                        p_b_eot,
    BYTE*                       p_next_data,
    int                         len_next_data,
    unsigned int                pusi);

int
tsp_parse_pmt(
    tsp_table_pmt_t*            p_table,
    int*                        p_b_cot,
    int*                        p_b_eot,
    BYTE*                       p_next_data,
    int                         len_next_data,
    unsigned int                pusi);

int
tsp_parse_cat(
    tsp_table_cat_t*            p_table,
    int*                        p_b_cot,
    int*                        p_b_eot,
    BYTE*                       p_next_data,
    int                         len_next_data,
    unsigned int                pusi);

int
tsp_table_pmt_reset(
    tsp_table_pmt_t*            p_table);

int
tsp_table_rehash_pmt(
    tsp_table_pmt_t*                    p_table_pat);

int
tsp_table_rehash_pat(
    tsp_table_pat_t*                    p_table_pat);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TSP_PSI_H__

