
#ifndef __TS_PG_SM_H__
#define __TS_PG_SM_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "tsp.h"
#include "frontend.h"

typedef enum _TSPG_STREAM_TYPE_E{
    TSPG_STREAM_TYPE_VIDEO_E=           0,
    TSPG_STREAM_TYPE_AUDIO_E=           1,
    TSPG_STREAM_TYPE_SPU_E=             2,
    TSPG_STREAM_TYPE_NA=                3
} TSPG_STREAM_TYPE_E;

typedef struct _tspg_sm_t*              tspg_sm_t;

// transport program state machine create/destroy function

#ifdef TS_DEBUG
void
tspg_cscan_dump(
    tspg_sm_t                           tspg_sm);

void
tspg_epg_dump(
    tspg_sm_t                           tspg_sm);
#endif

tspg_sm_t
tspg_create(void);

int
tspg_destroy(
    tspg_sm_t                           tspg_sm);

// transport program state machine bit stream

#ifdef TS_AP_NAV_IF
int
tspg_pat_parse_init(
    tspg_sm_t                           tspg_sm,
    tsio_param_t*                       p_tsio_param);
#else
int
tspg_pat_parse_init(
    tspg_sm_t                           tspg_sm,
    uint32_t                            frequency);
#endif

int
tspg_pat_parse(
    tspg_sm_t                           tspg_sm,
    unsigned char*                      p_buffer,
    int                                 len_buffer);

int
tspg_pmt_parse_init(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t                           pid_target_pmt);

int
tspg_pmt_parse(
    tspg_sm_t                           tspg_sm,
    unsigned char*                      p_buffer,
    int                                 len_buffer);

int
tspg_eit_parse_init(
    tspg_sm_t                           tspg_sm);

int
tspg_eit_parse(
    tspg_sm_t                           tspg_sm,
    unsigned char*                      p_buffer,
    int                                 len_buffer);

int
tspg_sdt_parse_init(
    tspg_sm_t                           tspg_sm);

int
tspg_sdt_parse(
    tspg_sm_t                           tspg_sm,
    unsigned char*                      p_buffer,
    int                                 len_buffer);

int
tspg_tdt_parse_init(
    tspg_sm_t                           tspg_sm);

int
tspg_tdt_parse(
    tspg_sm_t                           tspg_sm,
    unsigned char*                      p_buffer,
    int                                 len_buffer);

// transport program state machine program information stuff
int
tspg_pginfo_rehash(
    tspg_sm_t                           tspg_sm);

int
tspg_pginfo_stream_descriptor(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t                           pid);

int
tspg_pginfo_cur_pgindex(
    tspg_sm_t                           tspg_sm);

int
tspg_pginfo_pg_select(
    tspg_sm_t                           tspg_sm,
    int                                 pg_index);

int
tspg_pginfo_pg_total(
    tspg_sm_t                           tspg_sm);

#ifdef TS_AP_NAV_IF
int
tspg_pginfo_pg_front_param_get(
    tspg_sm_t                           tspg_sm,
    tsio_param_t*                       p_tsio_param,
    int                                 channel_idx);
#else
int
tspg_pginfo_pg_front_param_set(
    tspg_sm_t                           tspg_sm,
    struct dvb_frontend_parameters*     p_front_param);

int
tspg_pginfo_pg_front_param_get(
    tspg_sm_t                           tspg_sm,
    struct dvb_frontend_parameters*     p_front_param,
    int                                 channel_idx);
#endif

// @FIXME: refine this API later
int
tspg_pginfo_subtitle_info(
    tspg_sm_t                           tspg_sm,
    unsigned short*                     p_page_id_composition,
    unsigned short*                     p_page_id_ancillary);

// transport program state machine pid filter infomation get
int
tspg_pid_is_av(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t                           pid);

// @FIXME: the below API needs to be refine
int
tspg_pid_pmt_get_all(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t*                          pmt_pid_list,
    int*                                p_n_pmt_pid,
    uint32_t                            freq);

int
tspg_pid_cur_pmt_get(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t*                          p_pmt_pid);

int
tspg_pid_cur_pid_by_type(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t*                          p_pid,
    TSPG_STREAM_TYPE_E                  tspg_stream_type);

int
tspg_pid_cur_descriptor_by_type(
    tspg_sm_t                           tspg_sm,
    uint32_t*                           p_descptor_stream,
    TSPG_STREAM_TYPE_E                  tspg_stream_type);

// @FIXME: the below API needs to be refine
int
tspg_pid_cur_pg_get(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t*                          pid_list,
    int*                                p_n_pid);

int
tspg_pid_cur_pg_get_all(
    tspg_sm_t                           tspg_sm,
    tsp_pid_t*                          pid_list,
    int*                                p_n_pid);

// transport program state machine channel scan stuff
#ifdef TS_AP_NAV_IF
int
tspg_cscan_get(
    tspg_sm_t                           tspg_sm,
    void*                               p_channel_guide,
    int*                                p_channel_guide_size,
    void*                               p_user_data,
    int*                                p_user_data_size);

int
tspg_cscan_set(
    tspg_sm_t                           tspg_sm,
    void*                               p_channel_guide,
    int                                 channel_guide_size,
    void*                               p_user_data,
    int                                 user_data_size);

int
tspg_cscan_set_stream(
    tspg_sm_t           tspg_sm,
    tsp_pid_t           pid_video,
    uint32_t            stream_desc_video,
    tsp_pid_t           pid_audio,
    uint32_t            stream_desc_audio);

// The below interface has to be refine later
int
tspg_cgd_service_get(
    tspg_sm_t                   tspg_sm,
    char*                       p_name,
    int*                        p_name_size,
    int                         pg_index,
    TS_CGD_SERVICE_TYPE_E       name_type);

int
tspg_cgd_freq_get(
    tspg_sm_t           tspg_sm,
    int*                p_freq,
    int                 pg_index);

int
tspg_cgd_cid_get(
    tspg_sm_t                   tspg_sm,
    int*                        p_cid_major,
    int*                        p_cid_minor,
    int                         pg_index);

#else
int
tspg_cscan_get(
    tspg_sm_t                           tspg_sm,
    ts_channel_guide_t*                 p_channel_guide);

int
tspg_cscan_set(
    tspg_sm_t                           tspg_sm,
    ts_channel_guide_t*                 p_channel_guide);
#endif

// transport program state machine channel epg stuff
int
tspg_epg_event_get(
    tspg_sm_t                           tspg_sm,
    ts_guide_epg_event_t**              pp_event_list,
    int*                                p_n_event,
    int                                 n_channel_idx);

int
tspg_epg_range_set(
    tspg_sm_t                           tspg_sm,
    ts_guide_epg_time_t                 time_start,
    ts_guide_epg_time_t                 time_end);

int
tspg_epg_range_rehash(
    tspg_sm_t                           tspg_sm);

int
tspg_epg_event_clear(
    tspg_sm_t                           tspg_sm,
    int                                 n_channel_idx);

// transport program state machine channel epg stuff
int
tspg_time_utc_get(
    tspg_sm_t                           tspg_sm,
    ts_guide_epg_time_t*                p_time_utc);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
