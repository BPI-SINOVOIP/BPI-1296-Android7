
#ifndef __TS_DB_H__
#define __TS_DB_H__

#include <OSAL/Types.h>

typedef struct _tsdb_t*         tsdb_t;

typedef enum{
    TS_DB_SYM_ID_BIT_RATE_E,
    TS_DB_SYM_ID_PACKET_LENGTH_E,
    TS_DB_SYM_ID_PACKET_OFFSET_E,
    TS_DB_SYM_ID_PACKET_PADDING_E,
    TS_DB_SYM_ID_PID_VIDEO_E,
    TS_DB_SYM_ID_PID_AUDIO_E,
    TS_DB_SYM_ID_PID_PMT_E,
    TS_DB_SYM_ID_STREAM_DESC_VIDEO_E,
    TS_DB_SYM_ID_STREAM_DESC_AUDIO_E,
} TS_DB_SYM_ID_E;

tsdb_t
ts_db_open(
    char*                       p_file_name_ts,
    int                         b_create);

tsdb_t
ts_db_create(
    char*                       p_file_name_ts);

int
ts_db_close(
    tsdb_t                      tsdb);

int
ts_db_is_sync(
    tsdb_t                      tsdb);

int
ts_db_sync(
    tsdb_t                      tsdb);

int
ts_db_read(
    tsdb_t                      tsdb,
    TS_DB_SYM_ID_E              sym_id,
    uint32_t*                   p_value);

int
ts_db_write(
    tsdb_t                      tsdb,
    TS_DB_SYM_ID_E              sym_id,
    uint32_t                    value);

#endif // #ifndef __TS_DB_H__
