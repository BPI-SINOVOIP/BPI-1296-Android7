
#ifndef __TS_UTIL_RING_H__
#define __TS_UTIL_RING_H__

int
ts_util_ring_sync_byte(
    unsigned char**             pp_sync_byte,
    unsigned char*              p_lower,
    unsigned char*              p_upper,
    unsigned char*              p_start,
    unsigned char*              p_end,
    unsigned char               len_packet);

int
ts_util_ring_total_free_space(
    unsigned char*              p_lower,
    unsigned char*              p_upper,
    unsigned char*              p_read,
    unsigned char*              p_write);

int
ts_util_ring_total_size(
    unsigned char*              p_lower,
    unsigned char*              p_upper,
    unsigned char*              p_read,
    unsigned char*              p_write);

#endif
