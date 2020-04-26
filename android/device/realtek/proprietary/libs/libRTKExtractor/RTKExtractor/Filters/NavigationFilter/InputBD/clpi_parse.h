#if !defined(_CLPI_PARSE_H_)
#define _CLPI_PARSE_H_

typedef struct
{
    uint16_t         pcr_pid;
    uint32_t         spn_stc_start;
    uint32_t         presentation_start_time;
    uint32_t         presentation_end_time;
} bdt_stc_seq_t;

typedef struct
{
    uint32_t         spn_atc_start;
    uint8_t          num_stc_seq;
    uint8_t          offset_stc_id;
    bdt_stc_seq_t    *stc_seq;
} bdt_atc_seq_t;

typedef struct
{
    uint8_t          num_atc_seq;
    bdt_atc_seq_t    *atc_seq;
} bdt_seq_info_t;

typedef struct
{
    uint8_t          validity;
    uint8_t          format_id[4];
} bdt_ts_type_t;

typedef struct
{
    uint32_t         unknown;
    uint8_t          file_id[5];
    uint8_t          file_code[4];
} bdt_cc5_info_t;

typedef struct
{
    uint8_t          clip_stream_type;
    uint8_t          application_type;
    uint8_t          is_cc5;
    uint32_t         ts_recording_rate;
    uint32_t         num_source_packets;
    bdt_ts_type_t    ts_type_info;
    uint8_t          cc5_thingy_count;
    bdt_cc5_info_t   *cc5_thingy;
} bdt_clip_info_t;

typedef struct
{
    uint16_t         pid;
    uint8_t          coding_type;
    uint8_t          format;
    uint8_t          rate;
    uint8_t          aspect;
    uint8_t          oc_flag;
    uint8_t          char_code;
    uint8_t          lang[3];
} bdt_prog_stream_t;

typedef struct
{
    uint32_t         spn_program_sequence_start;
    uint16_t         program_map_pid;
    uint8_t          num_streams;
    uint8_t          num_groups;
    bdt_prog_stream_t *streams;
} bdt_prog_t;

typedef struct
{
    uint8_t          num_prog;
    bdt_prog_t       *progs;
} bdt_prog_info_t;

typedef struct
{
    int              ref_ep_fine_id;
    int              pts_ep;
    uint32_t         spn_ep;
} bdt_ep_coarse_t;

typedef struct
{
    uint8_t          is_angle_change_point;
    uint8_t          i_end_position_offset;
    int              pts_ep;
    int              spn_ep;
} bdt_ep_fine_t;

typedef struct
{
    uint16_t         pid;
    uint8_t          ep_stream_type;
    int              num_ep_coarse;
    int              num_ep_fine;
    uint32_t         ep_map_stream_start_addr;
    bdt_ep_coarse_t  *coarse;
    bdt_ep_fine_t    *fine;
} bdt_ep_map_entry_t;

typedef struct
{
    uint8_t         type;
    // ep_map
    uint8_t         num_stream_pid;
    bdt_ep_map_entry_t *entry;
} bdt_cpi_t;

typedef struct
{
    uint32_t         type_indicator;
    uint32_t         type_indicator2;
    uint32_t         sequence_info_start_addr;
    uint32_t         program_info_start_addr;
    uint32_t         cpi_start_addr;
    uint32_t         clip_mark_start_addr;
    uint32_t         ext_data_start_addr;
    bdt_clip_info_t  clip;
    bdt_seq_info_t   sequence;
    bdt_prog_info_t  program;
    bdt_cpi_t        cpi;
    // skip clip mark & extension data
} bdt_cl_t;

bool parse_cpi(FILE *stream, uint32_t start_addr, bdt_cpi_t *cpi, bool bCPI_SS);
bool parse_programinfo(FILE *stream, uint32_t start_addr, bdt_prog_info_t *program, bool bProgInfo_SS);

#endif // _CLPI_PARSE_H_
