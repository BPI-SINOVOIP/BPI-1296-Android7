#if !defined(_MPLS_PARSE_H_)
#define _MPLS_PARSE_H_

#include <stdio.h>
#include <stdint.h>
#include "subpath_parse.h"

typedef struct
{
    uint8_t         stream_type;
    uint8_t         coding_type;
    uint16_t        pid;
    uint8_t         subpath_id;
    uint8_t         subclip_id;
    uint8_t         format;
    uint8_t         rate;
    uint8_t         char_code;
    uint8_t         lang[3];
} bdt_stream_t;

typedef struct
{
    uint8_t         num_video;
    uint8_t         num_audio;
    uint8_t         num_pg;
    uint8_t         num_ig;
    uint8_t         num_secondary_audio;
    uint8_t         num_secondary_video;
    uint8_t         num_pip_pg;
    bdt_stream_t    *video;
    bdt_stream_t    *audio;
    bdt_stream_t    *pg;
} bdt_pl_stn_t;

typedef struct
{
    char            clip_id[6];
    uint8_t         connection_condition;
    uint8_t         stc_id;
    uint32_t        in_time;
    uint32_t        out_time;
    bdt_pl_stn_t    stn;

    // Extrapolated items
    uint32_t        abs_start;
    uint32_t        abs_end;
} bdt_pi_t;

typedef struct
{
    uint8_t         mark_id;
    uint8_t         mark_type;
    uint16_t        play_item_ref;
    uint32_t        time;
    uint16_t        entry_es_pid;
    uint32_t        duration;

    // Extrapolated items
    uint32_t        abs_start;
} bdt_plm_t;

typedef struct
{
    uint32_t        type_indicator;
    uint32_t        type_indicator2;
    uint32_t        list_pos;
    uint32_t        mark_pos;
    uint32_t        ext_pos;
    uint16_t        list_count;
    uint16_t        sub_count;
    uint16_t        mark_count;
    bdt_pi_t        *play_item;
    bdt_plm_t       *play_mark;
    bdt_sub_path_t  *sub_path;
	bdt_stn_ss_t    *stn_ss;

    // Extrapolated items
    uint64_t        duration;
} bdt_pl_t;

#endif // _MPLS_PARSE_H_
