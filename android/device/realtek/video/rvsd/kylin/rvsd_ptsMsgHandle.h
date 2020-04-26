#ifndef _RVSD_PTS_MESSAGE_HANDLE_H_
#define _RVSD_PTS_MESSAGE_HANDLE_H_

#include "rvsd_InBandAPI.h"
class RVSD_INFO;

typedef struct PtsQueue
{
    struct PtsQueue *next;
    struct PtsQueue *prev;
    unsigned int wPtr ;
    unsigned int PTSH ;
    unsigned int PTSL ;
} PtsQueue_t;

void PQ_flush(RVSD_INFO* pRvsdInfo);
void PQ_append(RVSD_INFO* pRvsdInfo, PTS_INFO *cmd);
void PQ_append(RVSD_INFO* pRvsdInfo, EOS *cmd);
void PQ_remove_head(RVSD_INFO* pRvsdInfo);
void APQ_append(RVSD_INFO* pRvsdInfo,PTS_INFO *cmd);
void APQ_remove_head(RVSD_INFO* pRvsdInfo);
void APQ_flush(RVSD_INFO* pRvsdInfo);

#endif // _RVSD_PTS_MESSAGE_HANDLE_H_
