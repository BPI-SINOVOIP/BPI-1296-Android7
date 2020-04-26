#ifndef __TSP_CRC32_H__
#define __TSP_CRC32_H__

#include "OSAL/Types.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

uint32_t
tsp_crc32_calc(
    uint8_t*            p_data,
    int                 length);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef __TSP_CRC32_H__
