#ifndef RVSD_COMMON_FUNC_H
#define RVSD_COMMON_FUNC_H

typedef struct
{
    volatile unsigned int   base   ;
    volatile unsigned int   limit  ;
    volatile unsigned int *pWrPtr ;
    volatile unsigned int *pRdPtr ;
    volatile unsigned int   write ;
    volatile unsigned int   phyBase;

} VP_INBAND_RING_BUF ;

int RVSD_GetExitLoopThread(void *p);
void ICQ_UpdateReadPtr_LE (VP_INBAND_RING_BUF *pVIRB, int size);
void *ICQ_GetCmd_LE (void *p, int p_size, VP_INBAND_RING_BUF *pVIRB, int bUpdateReadPtr);
int ICQ_PeekCmd_LE (VP_INBAND_RING_BUF *pVIRB, int numPeeks, INBAND_CMD_TYPE *pCmd);
int ICQ_SeekCmd_LE (void *p, VP_INBAND_RING_BUF *pVIRB, int cmdType);
unsigned int endian_swap(unsigned char *in);
void rvsd_adjust_DecOpenParam(void* p, int mapType);
bool IsFloatEqual(float a, float b);
void* my_memcpy(void *des, void *src, size_t num);

void StartElapseTime(struct timespec &start);
float EndElapseTime(struct timespec &start);

int IPC_GetRingBufFd(
    int client_sockfd,          // (I)
    unsigned char *pData,       // (I)
    unsigned int *pRingSize,    // (O)
    int *pHdrFd,                // (O)
    int *pRingBaseFd);           // (O)
int IPC_GetRingBufAddrByFd(
    int nRingHdrFd,
    int nRingBaseFd,
    int nBufSize,
    RINGBUFFER_HEADER **pHdr,
    unsigned char **pBase);

#endif // #ifdef RVSD_COMMON_FUNC_H

