
#ifndef __TS_SI_H__
#define __TS_SI_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// @FIXME: remove this later
#include "Filters/NavigationFilter/ts_guide.h" // only for TS_AP_NAV_IF

/*************************************************************************/
/*  The below is about constant declaration                              */
/*************************************************************************/
#define MAX_NUM_FREQ                    64

typedef enum{
    TS_SI_CMD_NONE,

    // General stop
    TS_SI_CMD_STOP_E,
    TS_SI_CMD_EXIT_E,

    // About channel scan
    TS_SI_CMD_CSCAN_START_E,
    TS_SI_CMD_CSCAN_STOP_E,

    // About EPG: the tuner must start receiving data before TS_SI_CMD_EPG_START_E
    TS_SI_CMD_EPG_START_E,
    TS_SI_CMD_EPG_STOP_E,

    // About utc time
    TS_SI_CMD_TIME_UTC_START_E,
    TS_SI_CMD_TIME_UTC_STOP_E,
} TS_SI_CMD_TYPE_E;

typedef enum{
    TS_SI_STATE_IDLE_E,
    TS_SI_STATE_CSCAN_E,
    TS_SI_STATE_EPG_E,
    TS_SI_STATE_TIME_UTC_E,
    TS_SI_STATE_EXIT_E
} TS_SI_STATE_E;

typedef enum{
    // About channel scan
    TS_SI_EVENT_CSCAN_COMPLETE_E,
    TS_SI_EVENT_CSCAN_USER_BREAK_E,

    // About utc time
    TS_SI_EVENT_TIME_UTC_COMPLETE_E,
    TS_SI_EVENT_TIME_UTC_USER_BREAK_E,
} TS_SI_EVENT_ID_E;

/*************************************************************************/
/*  The below is about event notifier                                    */
/*************************************************************************/
typedef int (*ts_si_event_handle_t)(
        void*                   p_cb_data,
        void*                   p_si_handle,
        TS_SI_EVENT_ID_E        ts_si_evid);

/*************************************************************************/
/*  The below is about local data structure declaration                  */
/*************************************************************************/
typedef struct{
} TS_SI_CMD_ARG_STOP_S;

typedef struct{
} TS_SI_CMD_ARG_EXIT_S;

typedef struct{
#ifdef TS_AP_NAV_IF
    char                            dtv_param[256];
    int                             dtv_param_len;
#else
    struct dvb_frontend_parameters  fe_param;
    int                             n_freq;
    uint32_t                        freq_list[MAX_NUM_FREQ];
#endif
} TS_SI_CMD_ARG_CSCAN_START_S;

typedef struct{
} TS_SI_CMD_ARG_CSCAN_STOP_S;

typedef struct{
} TS_SI_CMD_ARG_EPG_START_S;

typedef struct{
} TS_SI_CMD_ARG_EPG_STOP_S;

typedef struct{
} TS_SI_CMD_ARG_TIME_UTC_START_S;

typedef struct{
} TS_SI_CMD_ARG_TIME_UTC_STOP_S;

typedef union _TS_SI_CMD_ARG_U{
    TS_SI_CMD_ARG_STOP_S            arg_stop;
    TS_SI_CMD_ARG_EXIT_S            arg_exit;
    TS_SI_CMD_ARG_CSCAN_START_S     arg_cscan_start;
    TS_SI_CMD_ARG_CSCAN_STOP_S      arg_cscan_stop;
    TS_SI_CMD_ARG_EPG_START_S       arg_epg_start;
    TS_SI_CMD_ARG_EPG_STOP_S        arg_epg_stop;
    TS_SI_CMD_ARG_TIME_UTC_START_S  arg_time_utc_start;
    TS_SI_CMD_ARG_TIME_UTC_STOP_S   arg_time_utc_stop;
} TS_SI_CMD_ARG_U;

typedef struct{
    TS_SI_CMD_TYPE_E                    cmd_type;
    TS_SI_CMD_ARG_U                     cmd_arg;
} TS_SI_CMD_ARG_S;

/*************************************************************************/
/*  The below is public function declaration                             */
/*************************************************************************/
void*
ts_si_thread_create(
    ts_nav_t*                   p_ts_nav,
    ts_si_event_handle_t        p_cb_ev_handle,
    void*                       p_cb_ev_data);

int
ts_si_thread_destroy(
    void*                       p_arg);

int
ts_si_cmd(
    void*                       p_handle,
    TS_SI_CMD_ARG_S*            p_cmd_arg);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
