/*
 * collect statistics from HTTP
 */

#ifndef NE_STATS_H
#define NE_STATS_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void ne_stats_countbytes(unsigned int bytes);

unsigned int ne_stats_returnKB(void);

#ifdef __cplusplus
}
#endif

#endif
