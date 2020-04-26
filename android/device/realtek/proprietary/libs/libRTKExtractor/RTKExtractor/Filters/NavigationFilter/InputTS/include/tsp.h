#ifndef __TSP_H__
#define __TSP_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


#if 1
#include "OSAL/Types.h"
#else
#include <linux/types.h>
#endif
#include <limits.h>
#include "Filters/NavigationFilter/ts_guide.h"

#define TSP_PACKET_SIZE                 188

#if 0
typedef uint32_t                        tsp_pid_t;
typedef uint32_t                        tsp_sid_t;
#else
typedef uint16_t                        tsp_pid_t;
typedef uint16_t                        tsp_sid_t;
#endif
typedef uint8_t                         tsp_tid_t;
typedef uint32_t                        tsp_pgid_t;

typedef enum{
    tsp_stream_na_e             = -1,
    tsp_stream_start_e          = 0,
    tsp_stream_video_e          = 0,
    tsp_stream_audio_e          = 1,
    tsp_stream_spu_e            = 2,
    tsp_stream_end_e            = 3
} stb_vm_stream_cat_t;

// @FIXME: temparary fix
#ifdef TS_AP_NAV_IF
#define TSIO_PARAM_NA           (0x00)
#define TSIO_PARAM_BW           (0x01)
#define TSIO_PARAM_FREQ         (0x02)

typedef struct _tsio_param_t{
    unsigned char               flag;
    unsigned char               bandwidth;
    unsigned int                frequency;
} tsio_param_t;
#endif

// packet id list
#define TSP_PID_PAT                     (tsp_pid_t)(0x0000)
#define TSP_PID_CAT                     (tsp_pid_t)(0x0001)
#define TSP_PID_TSDT                    (tsp_pid_t)(0x0002)
#define TSP_PID_SDT                     (tsp_pid_t)(0x0011)
#define TSP_PID_EIT                     (tsp_pid_t)(0x0012)
#define TSP_PID_TDT                     (tsp_pid_t)(0x0014)
// based on spec, the null packet should be filtered by hardware.
// i.e. from the software's viewpoint, we never see null packets.
// consequently, no null packet need to be handled by software.
#define TSP_PID_NULL                    (tsp_pid_t)(0x1fff)
#define TSP_PID_INVALID                 (tsp_pid_t)(USHRT_MAX)

// section id list
#define TSP_SID_PAT                     (tsp_sid_t)(0x0000)
#define TSP_SID_CAT                     (tsp_sid_t)(0x0001)
#define TSP_SID_TSDT                    (tsp_sid_t)(0x0002)
#define TSP_SID_SDT                     (tsp_sid_t)(0x0011)
#define TSP_SID_EIT                     (tsp_sid_t)(0x0012)
#define TSP_SID_TDT                     (tsp_sid_t)(0x0014)
#define TSP_SID_PMT                     (tsp_sid_t)(0xfffe)
#define TSP_SID_INVALID                 (tsp_sid_t)(UINT_MAX)

// Table id list
#define TSP_TID_PAT                     (tsp_tid_t)(0x00)
#define TSP_TID_CAT                     (tsp_tid_t)(0x01)
#define TSP_TID_PMT                     (tsp_tid_t)(0x02)
#define TSP_TID_TSDT                    (tsp_tid_t)(0x03)
#define TSP_TID_SDT_ACTUAL              (tsp_tid_t)(0x42)
#define TSP_TID_SDT_OTHER               (tsp_tid_t)(0x46)
#define TSP_TID_EIT_ACTUAL              (tsp_tid_t)(0x4e)
#define TSP_TID_EIT_OTHER               (tsp_tid_t)(0x4f)
#define TSP_TID_TDT                     (tsp_tid_t)(0x70)
#define TSP_TID_TOT                     (tsp_tid_t)(0x73)
#define TSP_TID_INVALID                 (tsp_tid_t)(UCHAR_MAX)

// descriptor tag
#define TSP_DESC_TAG_REGISTRATION       (0x05)
#define TSP_DESC_TAG_AUDIO_LANG         (0x0a)
#define TSP_DESC_TAG_SERVICE            (0x48)
#define TSP_DESC_TAG_SHORT_EVENT        (0x4d)
#define TSP_DESC_TAG_EXTENTED_EVENT     (0x4e)
#define TSP_DESC_TAG_CONTENT            (0x54)
#define TSP_DESC_TAG_SUBTITLE           (0x59)


#define TSP_STYPE_ISO_11172_VIDEO               0x01            /* ISO/IEC 11172 Video */
#define TSP_STYPE_ISO_13818_VIDEO               0x02            /* ISO/IEC 13818-2 Video */
#define TSP_STYPE_ISO_11172_AUDIO               0x03            /* ISO/IEC 11172 Audio */
#define TSP_STYPE_ISO_13818_AUDIO               0x04            /* ISO/IEC 13818-3 Audio */
#define TSP_STYPE_ISO_13818_PRIVATE             0x05            /* ISO/IEC 13818-1 private sections */
#define TSP_STYPE_ISO_13818_PES_PRIVATE         0x06            /* ISO/IEC 13818-1 PES packets containing private data */
#define TSP_STYPE_ISO_13522_MHEG                0x07            /* ISO/IEC 13512 MHEG */
#define TSP_STYPE_ISO_13818_DSMCC               0x08            /* ISO/IEC 13818-1 Annex A  DSM CC */
#define TSP_STYPE_ISO_13818_TYPE_A              0x0a            /* ISO/IEC 13818-6 Multiprotocol encapsulation */
#define TSP_STYPE_ISO_13818_TYPE_B              0x0b            /* ISO/IEC 13818-6 DSM-CC U-N Messages */
#define TSP_STYPE_ISO_13818_TYPE_C              0x0c            /* ISO/IEC 13818-6 Stream Descriptors */
#define TSP_STYPE_ISO_13818_TYPE_D              0x0d            /* ISO/IEC 13818-6 Sections (any type, including private data) */
#define TSP_STYPE_ISO_13818_AUX                 0x0e            /* ISO/IEC 13818-1 auxiliary */
#define TSP_STYPE_ISO_13818_PART7_AUDIO         0x0f            /* ISO/IEC 13818-7 Audio with ADTS transport sytax */
#define TSP_STYPE_ISO_14496_PART2_VIDEO         0x10            /* ISO/IEC 14496-2 Visual (MPEG-4) */
#define TSP_STYPE_ISO_14496_PART3_AUDIO         0x11            /* ISO/IEC 14496-3 Audio with LATM transport syntax */
#define TSP_STYPE_ISO_14496_PART10_VIDEO        0x1b            /* ISO/IEC 14496-10 Video (MPEG-4 part 10/AVC, aka H.264) */
#define TSP_STYPE_ATSC_AC3_AUDIO                0x81            /* ISO/IEC 14496-10 Video (MPEG-4 part 10/AVC, aka H.264) */

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

#define MAX_PREVIOUS_DATA_LEN           4096
#define MAX_OUT_ARG                     16

// #define IS_DATA_PARSE_READY(p_arg_out)  (0== ((tsp_arg_pat*)(p_arg_out))->prev_data.len_buffer)
#define TSP_PARSE_READY                 0x01
#define TSP_DATA_PARSE_IS_READY(p_arg_out) \
    (TSP_PARSE_READY & *((uint32_t*)p_arg_out))
#define TSP_DATA_PARSE_SET_READY(p_arg_out) \
    (*((uint32_t*)p_arg_out)|= TSP_PARSE_READY)
#define TSP_DATA_PARSE_RESET_READY(p_arg_out) \
    (*((uint32_t*)p_arg_out)&= ~TSP_PARSE_READY)

// ======================================================================================
// some data structure definition                
// ======================================================================================
typedef struct tsp_previous_data_t{
    int                                 len_buffer;
    unsigned char*                      p_buffer;
} tsp_previous_data_t;

#define TSP_MAX_TS_STRING_LENGTH        256

typedef struct tsp_arg_desc_service_t{
    unsigned char*                      p_name_service_provider;
    unsigned char*                      p_name_service;
    unsigned char                       len_name_service_provider;
    unsigned char                       len_name_service;
    unsigned char                       service_type;
} tsp_arg_desc_service_t;

typedef struct tsp_arg_desc_short_event_t{
    unsigned char*                      p_event_name;
    unsigned char*                      p_event_text;
    unsigned char                       len_event_name;
    unsigned char                       len_event_text;
    TS_ISO_639_t                        lang_code;
} tsp_arg_desc_short_event_t;

typedef struct tsp_arg_desc_content_t{
    unsigned short                      content_nibble;
} tsp_arg_desc_content_t;

typedef struct tsp_arg_desc_subtitle_t{
    TS_ISO_639_t                        lang_code;
    unsigned short                      subtitle_page_id_composition;
    unsigned short                      subtitle_page_id_ancillary;
    unsigned char                       subtitle_type;
} tsp_arg_desc_subtitle_t;

typedef struct tsp_arg_desc_audio_lang_t{
    TS_ISO_639_t                        lang_code;
    unsigned char                       audio_type;
} tsp_arg_desc_audio_lang_t;

typedef struct tsp_arg_desc_registraction_t{
    uint32_t                            RID;
} tsp_arg_desc_registraction_t;

typedef union tsp_desc_u{
    tsp_arg_desc_service_t              arg_desc_service;               // TSP_DESC_TAG_SERVICE
    tsp_arg_desc_short_event_t          arg_desc_short_event;           // TSP_DESC_TAG_SHORT_EVENT
    tsp_arg_desc_content_t              arg_desc_content;               // TSP_DESC_TAG_CONTENT
    tsp_arg_desc_subtitle_t             arg_desc_subtitle;              // TSP_DESC_TAG_SUBTITLE
    tsp_arg_desc_audio_lang_t           arg_desc_audio_lang;            // TSP_DESC_TAG_AUDIO_LANG
    tsp_arg_desc_registraction_t        arg_desc_registration;          // TSP_DESC_TAG_REGISTRATION
    // tsp_arg_desc_extented_event_t       arg_desc_extended_event;        // TSP_DESC_TAG_EXTENTED_EVENT
} tsp_desc_u;

typedef struct tsp_arg_desc_t{
    unsigned char                       desc_tag;
    tsp_desc_u                          desc;
} tsp_arg_desc_t;

typedef int
(*tsp_pat_event_process_t)(
    void*                               p_handle,
#ifdef TS_AP_NAV_IF
    tsio_param_t*                       p_tsio_param,
#else
    uint32_t                            frequency,
#endif
    tsp_pid_t                           pid_pmt,
    tsp_pgid_t                          pgid);

// Richard: don't change the memory layout if you know how flag works
typedef struct tsp_arg_pat_t{
    uint32_t                            flag;
    tsp_previous_data_t                 prev_data;
#ifdef TS_AP_NAV_IF
    tsio_param_t                        tsio_param;
#else
    uint32_t                            frequency;
#endif
    void*                               p_cb_handle;
    tsp_pat_event_process_t             p_cb_pat_process;
} tsp_arg_pat_t;

typedef int
(*tsp_pmt_event_process_t)(
    void*                               p_handle,
    tsp_pid_t                           pid_target_pmt,
    tsp_pid_t                           pid_stream,
    // uint32_t                            descriptor_stream,
    uint8_t                             stream_type,
    tsp_arg_desc_t*                     arg_desc_list,
    int                                 n_arg_desc);

// Richard: don't change the memory layout if you know how flag works
typedef struct tsp_arg_pmt_t{
    uint32_t                            flag;
    tsp_previous_data_t                 prev_data;
    tsp_pid_t                           pid_target_pmt;
    void*                               p_cb_handle;
    tsp_pmt_event_process_t             p_cb_pmt_process;
} tsp_arg_pmt_t;

typedef struct tsp_arg_cat_t{
    // not implement yet
} tsp_arg_cat_t;

typedef int
(*tsp_eit_event_process_t)(
    void*                               p_handle,
    tsp_pgid_t                          service_id,
    unsigned short                      event_id,
    ts_guide_epg_time_t*                p_time_start,
    ts_guide_epg_duration_t*            p_time_duragtion,
    unsigned char                       status_running,
    unsigned char                       free_CA_mode,
    tsp_arg_desc_t*                     arg_desc_list,
    int                                 n_arg_desc,
    unsigned char                       version_number);

// Richard: don't change the memory layout if you know how flag works
typedef struct tsp_arg_eit_t{
    uint32_t                            flag;
    tsp_previous_data_t                 prev_data;
    void*                               p_cb_handle;
    tsp_eit_event_process_t             p_cb_ev_process;
} tsp_arg_eit_t;

typedef int
(*tsp_sdt_desc_process_t)(
    void*                               p_handle,
    tsp_pgid_t                          service_id,
    unsigned char                       status_running,
    unsigned char                       free_CA_mode,
    unsigned char                       EIT_schedule,
    unsigned char                       EIT_present_following,
    tsp_arg_desc_t*                     arg_desc_list,
    int                                 n_arg_desc);

// Richard: don't change the memory layout if you know how flag works
typedef struct tsp_arg_sdt_t{
    uint32_t                            flag;
    tsp_previous_data_t                 prev_data;
    void*                               p_cb_handle;
    tsp_sdt_desc_process_t              p_cb_sdt_desc_process;
} tsp_arg_sdt_t;

typedef int
(*tsp_tdt_process_t)(
    void*                               p_handle,
    ts_guide_epg_time_t*                p_time_utc);

// Richard: don't change the memory layout if you know how flag works
typedef struct tsp_arg_tdt_t{
    uint32_t                            flag;
    tsp_previous_data_t                 prev_data;
    void*                               p_cb_handle;
    tsp_tdt_process_t                   p_cb_tdt_process;
} tsp_arg_tdt_t;

typedef struct _tsp_format_ts_packet_t{
    unsigned char   len_packet;
    unsigned char   offset_packet;
    unsigned char   padding_packet;
} tsp_format_ts_packet_t;

#define TSP_PACKET_SIZE_TABLE_NUMBER    3
extern tsp_format_ts_packet_t           ts_packet_size_table[TSP_PACKET_SIZE_TABLE_NUMBER];

// ======================================================================================
// some useful macros for header information
// ======================================================================================
// #define tsp_parse_header(pTS)              ((tsp_header_t)((htonl(*((u_long*)(pTS))))))
#define _swap32(d)                              \
            ((((d) & 0xff) << 24)       |       \
             ((((d)>> 8) & 0xff) << 16) |       \
             ((((d)>> 16) & 0xff) << 8) |       \
             (((d)>> 24) & 0xff))

#define _swap24(d)                              \
            ((((d) & 0xff) << 16)       |       \
             ((((d)>>  8) & 0xff) << 8) |       \
             ((((d)>> 16) & 0xff))

#define _swap16(d)                              \
            ((((d) & 0xff) << 8)       |        \
             ((((d)>> 8) & 0xff)))

#define tsp_header(ts_header, pTS)         (*((u_long*)(void*)&(ts_header))= _swap32(*((u_long*)(pTS))))
// #define tsp_header(ts_header, pTS)         { (*((u_long*)(void*)&(ts_header))= _swap32(*((u_long*)(pTS)))); assert(0x47== (ts_header).sync_byte); }
// #define tsp_header(ts_header, pTS)         { (*((u_long*)(void*)&(ts_header))= _swap32(*((u_long*)(pTS)))); db_warning("TS header is 0x%02x (NOT 0x47)\n", (ts_header).sync_byte); }

#define tsp_has_adaptation(ts_header)      (((tsp_header_t)(ts_header)).adaptation_field_control & 0x2)
#define tsp_has_payload(ts_header)         (((tsp_header_t)(ts_header)).adaptation_field_control & 0x1)
#define tsp_adaptation_length(p_ts)        (*(p_ts)+ 1)

int
tsp_payload(
    unsigned char**     pp_payload,
    int*                p_len_payload,
    unsigned char*      p_ts_packet,
    int                 len_ts_packet,
    tsp_header_t        ts_header);

int
tsp_parse_pat(
    tsp_arg_pat_t*              p_arg_pat,
    unsigned char*              p_payload,
    int                         len_payload,
    unsigned int                pusi);

int
tsp_parse_pmt(
    tsp_arg_pmt_t*              p_arg_pmt,
    unsigned char*              p_payload,
    int                         len_payload,
    unsigned int                pusi);

int
tsp_parse_cat(
    tsp_arg_cat_t*              p_arg_cat,
    unsigned char*              p_payload,
    int                         len_payload,
    unsigned int                pusi);

int
tsp_parse_eit(
    tsp_arg_eit_t*              p_arg_eit,
    unsigned char*              p_payload,
    int                         len_payload,
    unsigned int                pusi);

int
tsp_parse_sdt(
    tsp_arg_sdt_t*              p_arg_sdt,
    unsigned char*              p_payload,
    int                         len_payload,
    unsigned int                pusi);

int
tsp_parse_tdt(
    tsp_arg_tdt_t*              p_arg_tdt,
    unsigned char*              p_payload,
    int                         len_payload,
    unsigned int                pusi);

int
tsp_is_ts_stream(
    unsigned char*              p_buffer,
    int                         len_buffer);

int
tsp_sync_byte(
    unsigned char**             pp_sync_byte,
    unsigned char*              p_start,
    int                         len_buffer,
    int                         len_packet);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
