
#ifndef __INPUT_TS_PRIV_H__
#define __INPUT_TS_PRIV_H__

#include <pthread.h>
#include <Filters/PrivateInfo.h>
#include "Util.h"
#include "debug.h"
#include "mpeg_pes.h"
#include "frontend.h"
#include "ts_io_handle.h"
#include "ts_pg_sm.h"
#include "tsp.h"
#include "ts_time.h"
#include "ts_record.h"

/*************************************************************************/
/*  The below is global variable declaration                             */
/*************************************************************************/
extern ts_tshift_t             g_tshift;

/*************************************************************************/
/*  The below is some constant definition                                */
/*************************************************************************/
#define TS_STREAM_READ_INDEX            0
#define TS_SI_READ_INDEX                1
#define TS_STREAM_RECORD_INDEX          2

#define _TS_STATE_NONE                  (0x00000000)
#define _TS_STATE_STREAM                (0x00000001)
#define _TS_STATE_CSCAN                 (0x00000002)
#define _TS_STATE_EPG                   (0x00000004)
#define _TS_STATE_REC                   (0x00000008)
#define _TS_STATE_REPEAT                (0x00000010)
#define _TS_STATE_REPEAT_CHAPTER        (0x00000020 | _TS_STATE_REPEAT)
#define _TS_STATE_REPEAT_TITLE          (0x00000040 | _TS_STATE_REPEAT)
#define _TS_STATE_TIME_UTC              (0x00000100)
#define _TS_STATE_NEW_SEG               (0x00000200)
#define _TS_STATE_KEYFRAME              (0x00000400)

#define _TS_EX_CMD_NEW_SEG              (0x00000001)
#define _TS_EX_CMD_DMX_NEW_SEG          (0x00000002)
#define _TS_EX_CMD_DECODE_CMD_VIDEO     (0x00000004)
#define _TS_EX_CMD_VOUT_MIXER_RPTS      (0x00000008)
#define _TS_EX_CMD_VOUT_MIXER_BLEND     (0x00000010)
#define _TS_EX_CMD_DATA_START           (0x00000020)
// #define _TS_EX_CMD_END_SEQ              (0x00000040)
#define _TS_EX_CMD_END_SEG              (0x00000080)
#define _TS_EX_CMD_STOP_FLOW            (0x00000100)
#define _TS_EX_CMD_SPU_CONFIG           (0x00000200)
#define _TS_EX_CMD_PAGE_ID              (0x00000400)
#define _TS_EX_CMD_PB_CATCH_REC         (0x00000800)
#define _TS_EX_CMD_MPEG_VOB_INFO        (0x00001000)
#define _TS_EX_CMD_VDECODE_TO_BUFFER    (0x00002000)
#define _TS_EX_CMD_FLUSH                (0x00004000)
#define _TS_EX_CMD_RESET_SPEED          (0x00008000)
#define _TS_EX_CMD_VIDEO_ERROR_CONCEAL  (0x00010000)

#define _TS_EX_CMD_CHANNEL_SELECT       (_TS_EX_CMD_NEW_SEG | \
                                         _TS_EX_CMD_DECODE_CMD_VIDEO | \
                                         _TS_EX_CMD_VOUT_MIXER_RPTS | \
                                         _TS_EX_CMD_VOUT_MIXER_BLEND | \
                                         _TS_EX_CMD_DATA_START )

                                         // _TS_EX_CMD_DMX_FLUSH |

#define _TS_IN_CMD_WAIT_FINISH_RENDER   (0x00000001)
#define _TS_IN_CMD_SEEK_BEGIN           (0x00000002)
#define _TS_IN_CMD_MEDIA_CHANGE         (0x00000004)
#define _TS_IN_CMD_RENDER_POS_SYNC      (0x00000008)

/*************************************************************************/
/*  Some useful macros                                                   */
/*************************************************************************/
#define _TS_STATE_IS_INITAL(ts_state)    (_TS_STATE_NONE== (ts_state))

#define _TS_STATE_HAS_STREAM(ts_state)   HAS_FLAG((ts_state), _TS_STATE_STREAM)
#define _TS_STATE_HAS_CSCAN(ts_state)    HAS_FLAG((ts_state), _TS_STATE_CSCAN)
#define _TS_STATE_HAS_EPG(ts_state)      HAS_FLAG((ts_state), _TS_STATE_EPG)
#define _TS_STATE_HAS_REC(ts_state)      HAS_FLAG((ts_state), _TS_STATE_REC)
#define _TS_STATE_HAS_REPEAT(ts_state)   HAS_FLAG((ts_state), _TS_STATE_REPEAT)
#define _TS_STATE_HAS_REPEAT_CHAPTER(ts_state)  HAS_FLAG((ts_state), _TS_STATE_REPEAT_CHAPTER)
#define _TS_STATE_HAS_REPEAT_TITLE(ts_state)    HAS_FLAG((ts_state), _TS_STATE_REPEAT_TITLE)
#define _TS_STATE_HAS_TIME_UTC(ts_state) HAS_FLAG((ts_state), _TS_STATE_TIME_UTC)
#define _TS_STATE_HAS_NEW_SEG(ts_state)  HAS_FLAG((ts_state), _TS_STATE_NEW_SEG)
#define _TS_STATE_HAS_KEYFRAME(ts_state) HAS_FLAG((ts_state), _TS_STATE_KEYFRAME)

#define _TS_STATE_SET_STREAM(ts_state)   SET_FLAG((ts_state), _TS_STATE_STREAM)
#define _TS_STATE_SET_CSCAN(ts_state)    SET_FLAG((ts_state), _TS_STATE_CSCAN)
#define _TS_STATE_SET_EPG(ts_state)      SET_FLAG((ts_state), _TS_STATE_EPG)
#define _TS_STATE_SET_REC(ts_state)      SET_FLAG((ts_state), _TS_STATE_REC)
#define _TS_STATE_SET_REPEAT_CHAPTER(ts_state)  SET_FLAG((ts_state), _TS_STATE_REPEAT_CHAPTER)
#define _TS_STATE_SET_REPEAT_TITLE(ts_state)    SET_FLAG((ts_state), _TS_STATE_REPEAT_TITLE)
#define _TS_STATE_SET_TIME_UTC(ts_state) SET_FLAG((ts_state), _TS_STATE_TIME_UTC)
#define _TS_STATE_SET_NEW_SEG(ts_state)  SET_FLAG((ts_state), _TS_STATE_NEW_SEG)
#define _TS_STATE_SET_KEYFRAME(ts_state) SET_FLAG((ts_state), _TS_STATE_KEYFRAME)

#define _TS_STATE_RESET_STREAM(ts_state) RESET_FLAG((ts_state), _TS_STATE_STREAM)
#define _TS_STATE_RESET_CSCAN(ts_state)  RESET_FLAG((ts_state), _TS_STATE_CSCAN)
#define _TS_STATE_RESET_EPG(ts_state)    RESET_FLAG((ts_state), _TS_STATE_EPG)
#define _TS_STATE_RESET_REC(ts_state)    RESET_FLAG((ts_state), _TS_STATE_REC)
#define _TS_STATE_RESET_REPEAT(ts_state) RESET_FLAG((ts_state), _TS_STATE_REPEAT_CHAPTER | _TS_STATE_REPEAT_TITLE)
#define _TS_STATE_RESET_TIME_UTC(ts_state)      RESET_FLAG((ts_state), _TS_STATE_TIME_UTC)
#define _TS_STATE_RESET_NEW_SEG(ts_state)       RESET_FLAG((ts_state), _TS_STATE_NEW_SEG)
#define _TS_STATE_RESET_KEYFRAME(ts_state)      RESET_FLAG((ts_state), _TS_STATE_KEYFRAME)

#if 1
#define _CS_LOCK(p_mutex) \
    pthread_mutex_lock((p_mutex))

#define _CS_UNLOCK(p_mutex) \
    pthread_mutex_unlock((p_mutex))
#else
#define _CS_LOCK(p_mutex) \
    printf(">>>>>>>>>>>>>>>>> %s:%d: 0x%08x Lock\n", __FUNCTION__, __LINE__, (unsigned int)(p_mutex)); \
    fflush(stdout); \
    pthread_mutex_lock((p_mutex))

#define _CS_UNLOCK(p_mutex) \
    fflush(stdout); \
    pthread_mutex_unlock((p_mutex)); \
    printf("<<<<<<<<<<<<<<<<< %s:%d: 0x%08x UnLock\n", __FUNCTION__, __LINE__, (unsigned int)(p_mutex))
#endif

/*************************************************************************/
/*  The below is the real coding parts                                   */
/*************************************************************************/
/*
                            ---------------------
                            |     ts_nav_t      |
                            ---------------------
                               |              |
               ---------------------     ---------------------
               |   ts_io_handle_t  |     |    ts_pg_sm_t     |
               ---------------------     ---------------------
*/

typedef struct _ts_media_thumbnail_t{
    NAVPROP_TS_CMD_ARG_DECODE_MKEYFRAME_S       arg_mkeyframe;
    int                                         cur_file_idx;
} ts_media_thumbnail_t;

typedef enum _ts_media_spec_type_e{
    TS_MEDIA_SPEC_NA,
    TS_MEDIA_SPEC_THUMBNAIL_E,
} ts_media_spec_type_e;

typedef union _ts_media_spec_arg_t{
    ts_media_thumbnail_t        arg_thumbnail;
} ts_media_spec_arg_t;

typedef struct _ts_media_spec_t{
    ts_media_spec_type_e        media_type;
    ts_media_spec_arg_t         media_arg;
} ts_media_spec_t;

typedef struct _ts_ex_cmd_set_t{
    int64_t                     rpts;
    VIDEODECODECOMMAND          cmd_decoder_video;
    VIDEOMIXERBLEND             cmd_vout_blend;
    VIDEODVDVOBUINFO            cmd_vobu_info;
    VIDEODECODETOBUFFER         cmd_decode_to_buffer_video;
    SPUCONFIG                   spu_config_curr;
    SPUPAGEID                   spu_page_curr;
} ts_ex_cmd_storage_t;

typedef struct _ts_nav_t{
    tspg_sm_t                   ts_pg_sm;
    tsio_handle_t               ts_io_handle;
    pes_buf_list_t              pes_buf_list;
    uint32_t                    ts_state;
    uint32_t                    ts_ex_cmd;
    ts_ex_cmd_storage_t         ts_ex_cmd_storage;
    uint32_t                    ts_in_cmd;
    void*                       p_ts_si_handle;
    ts_rec_handle_t             ts_rec_handle;
    tsp_format_ts_packet_t      format_ts;
    ts_time_set_t               time_set;
    int                         b_seek;
    pthread_mutex_t             ts_state_cmd_mutex;
    void*                       p_plugin_io;
    ts_media_spec_t             media_spec;
    int                         cscan_progress;
    unsigned int                video_conceal_level;

    // Just a workaround for un-reasonable mutex locking in nav filter.
    // It means nothing to input plugin
    osal_mutex_t*               p_mutex;
    // @FIXME: should country code be added here? or ? Need to be clarified
    // ts_country_e        country_code;
} ts_nav_t;

/*************************************************************************/
/*  The below is the real coding parts                                   */
/*************************************************************************/
int
ts_media_load(
    ts_nav_t*           p_ts_nav,
    char*               p_path);

int
ts_media_unload(
    ts_nav_t*           p_ts_nav);

int
ts_media_change(
    ts_nav_t*           p_ts_nav,
    char*               p_path);

void
ts_cscan_seek(
    ts_nav_t*           p_ts_nav);

int
ts_channel_jump(
    ts_nav_t*           p_ts_nav,
    unsigned int*       pIsFlushRequired,
    int                 pg_index,
    int                 b_streaming);

int
ts_channel_stop(
    ts_nav_t*           p_ts_nav);

void
ts_status_bit_rate_init(
    ts_nav_t*           p_ts_nav);

int
ts_speed(
    ts_nav_t*           p_ts_nav,
    int                 speed,
    int                 skip);

int
ts_speed_normal(
    ts_nav_t*           p_ts_nav);

#endif // #ifndef __INPUT_TS_PRIV_H_
