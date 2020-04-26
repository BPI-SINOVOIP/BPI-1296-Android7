#include <cstring>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <cutils/compiler.h>

#include <RPCProxy.h>
#include <ion.h>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <cutils/compiler.h>

#include "RingBufferBase.h"

#define ATRACE_TAG (ATRACE_TAG_VIDEO)
#include <utils/Timers.h>
#include <utils/Trace.h>

#include <hresult.h>
#define RingBufferPrint ALOGD

/***************************************************************************************************/
typedef struct
{
    INBAND_CMD_PKT_HEADER header ;
    int packet[50];//may need to modify later
} VIDEO_VO_INBAND_DUMMY;
/***************************************************************************************************/

RingBufferBase::RingBufferBase(unsigned int instanceID, CLNT_STRUCT clnt)
    : mClientLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mClnt(clnt),
    mInstanceID(instanceID),
    mICQBufferSize(128*1024), //128K
    mLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mLowDelayMode(LowDelayDef),
    mLowDelayDepth(-1),
    mInitFrame(-1),
    mDeintFlag(-1)
{
    //initRPCProxy();

    OSAL_ERRORTYPE err = OSAL_ERRORNONE;
    int bufferSize;
    HRESULT* h;
    pICQRingBuffer = (OSAL_RTKALLOCATOR *)OSAL_Malloc(sizeof(OSAL_RTKALLOCATOR));
    memset(pICQRingBuffer, 0, sizeof(OSAL_RTKALLOCATOR));
    OSAL_RTKConstructor(pICQRingBuffer, STYLE_IPC, RTK_PHOENIX_ION_HEAP_AUDIO_MASK, ION_FLAG_NONCACHED |ION_FLAG_SCPUACC | ION_FLAG_ACPUACC);

    err = OSAL_RTKSetBuffer(pICQRingBuffer, mICQBufferSize);
    if(err != OSAL_ERRORNONE)
    {
        RingBufferPrint("[OSAL_RTKSetBuffer]fail!\n");
        return;
    }

    err = OSAL_RTKCommit(pICQRingBuffer, RTK_PHOENIX_ION_HEAP_AUDIO_MASK, /*ION_FLAG_NONCACHED |*/ION_FLAG_SCPUACC | ION_FLAG_ACPUACC);
    if(err != OSAL_ERRORNONE)
    {
        RingBufferPrint("[OSAL_RTKCommit]fail!\n");
        return;
    }

    RPC_RINGBUFFER ringbuffer = {
        .instanceID = (uint32_t) mInstanceID,
        .pinID = 0,
        .readPtrIndex = 0,
        .pRINGBUFF_HEADER = 0
    };

    OSAL_RTKGetBufferHeader(pICQRingBuffer, &(m_pICQBufferHeader), (OSAL_U32 *)(&ringbuffer.pRINGBUFF_HEADER));
    ALOGD("[%s] InstanceID 0x%08x RINGBUFF_HEADER 0x%08x",__FUNCTION__,
            (unsigned int) ringbuffer.instanceID,
            (unsigned int) ringbuffer.pRINGBUFF_HEADER);

    //OSAL_RTKWriteULONG((unsigned char *)&(m_pICQBufferHeader->bufferID), RINGBUFFER_COMMAND);
    OSAL_RTKWriteUINT32(m_pICQBufferHeader->bufferID, RINGBUFFER_COMMAND);

    ATRACE_NAME("VIDEO_RPC_ToAgent_InitRingBuffer_0");
    h = VIDEO_RPC_ToAgent_InitRingBuffer_0(&ringbuffer, &mClnt);

    if(h)
    {
        if (*h == S_OK)
            RingBufferPrint("[VIDEO_RPC_ToAgent_InitRingBuffer_0]RPC S_OK!\n");
        else
            RingBufferPrint("[VIDEO_RPC_ToAgent_InitRingBuffer_0]RPC fail! (0x%x)\n",* (unsigned int *) h);

        free(h);
    }
    else
    {
        RingBufferPrint("[VIDEO_RPC_ToAgent_InitRingBuffer_0]RPC return Error\n!\n");
    }

    OSAL_RTKGetBuffer(pICQRingBuffer, &m_pICQBufferLower, (OSAL_S32 *)&bufferSize);
    m_pICQBufferUpper = m_pICQBufferLower + bufferSize;

    RingBufferPrint("%s ready. (0x%lx-0x%lx size:%d)\n",__FUNCTION__, (long) m_pICQBufferLower, (long)m_pICQBufferUpper, bufferSize);
    return;
}

RingBufferBase::~RingBufferBase()
{
    ConfigLowDelayMode(LowDelayOff);
    OSAL_RTKDestory(pICQRingBuffer);
    OSAL_Free(pICQRingBuffer);
}

void RingBufferBase::registerClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (!found) {
        if (client != NULL) {
            mClientList.add(client);
            client->mService = this;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void RingBufferBase::removeClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            mClientList.removeAt(i);
            client->mService = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void RingBufferBase::endianSwap(void *addr, unsigned int byte_size)
{
    unsigned int i, data;
    unsigned int *p = (unsigned int*)addr;

    if(byte_size%4 != 0)
        printf("[buffer_endian_swap size warnning]\n");

    for(i=0; i<(byte_size>>2); i++)
    {
        unsigned int b0 = *p & 0x000000ff;
        unsigned int b1 = (*p & 0x0000ff00) >> 8;
        unsigned int b2 = (*p & 0x00ff0000) >> 16;
        unsigned int b3 = (*p & 0xff000000) >> 24;
        *p = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
        p++;
    }
}

void RingBufferBase::endianSwapMemoryCopy(unsigned char* src, unsigned char* des, unsigned long len)
{
    unsigned int i;
    unsigned int *psrc, *pdes;
    unsigned int b0, b1, b2, b3;
#if 0
    if ((((int)src & 0x3) != 0) || (((int)des & 0x3) != 0) || ((len & 0x3) != 0))
        printf("error in endianSwapMemoryCopy()...\n");
#endif

    for (i = 0; i < len; i+=4) {
        psrc = (unsigned int *)&src[i];
        pdes = (unsigned int *)&des[i];
        b0 = *psrc & 0x000000ff;
        b1 = (*psrc & 0x0000ff00) >> 8;
        b2 = (*psrc & 0x00ff0000) >> 16;
        b3 = (*psrc & 0xff000000) >> 24;
        *pdes = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    }
}

unsigned int RingBufferBase::InbandCmd(Client * client, int cmd_type, void *packet, unsigned int packet_size_in_bytes)
{
    bool found = false;
    unsigned int ret = 0;
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
    //pthread_mutex_lock(&mLock);
    if (found)
        ret = InbandCmd(cmd_type, packet, packet_size_in_bytes);
    //pthread_mutex_unlock(&mLock);
    return ret;

}
int RingBufferBase::ConfigLowDelay(Client * client, enum LowDelayMode mode, int depth, int init_frame)
{
    bool found = false;
    int ret = -1;
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
    //pthread_mutex_lock(&mLock);
    if (found)
        ret = ConfigLowDelay(mode, depth, init_frame);
    //pthread_mutex_unlock(&mLock);
    //ALOGI("[%s] -",__FUNCTION__);
    return ret;

}

unsigned int RingBufferBase::InbandCmd(int cmd_type, void *packet, unsigned int packet_size_in_bytes)
{
    //ALOGI("[%s] +", __FUNCTION__);
    unsigned int ret;

    VIDEO_VO_INBAND_DUMMY cmd;
    unsigned int cmd_size = sizeof(INBAND_CMD_PKT_HEADER) + packet_size_in_bytes;

    //OSAL_RTKWriteULONG((OSAL_U8 *)&(cmd.header.type), (OSAL_U32)cmd_type);
    //OSAL_RTKWriteULONG((OSAL_U8 *)&(cmd.header.size), (OSAL_U32)cmd_size);
    cmd.header.type = (INBAND_CMD_TYPE)htonl(cmd_type);
    cmd.header.size = htonl(cmd_size);

    endianSwap((void *)packet, packet_size_in_bytes);
    memcpy((void *)cmd.packet, (void *)packet, packet_size_in_bytes);

    ret = sendInbandCmd(&cmd, cmd_size);

    if(!ret)
        RingBufferPrint("rvsd_vo_sendInbandCmd size %d\n",ret);

    //ALOGI("[%s] -", __FUNCTION__);
    return ret;
}

unsigned int RingBufferBase::sendInbandCmd(void *cmd_endian_swapped, unsigned int cmd_size)
{
    //ALOGI("[%s]", __FUNCTION__);
    pthread_mutex_lock(&mLock);
    unsigned int res;
    int writableSize;

    unsigned char* pBufferCurr;
    unsigned char* pBufferNext;

    OSAL_RTKALLOCATOR* alloc;
    unsigned char* pBufferLower;
    unsigned char* pBufferUpper;

    unsigned char* Data;
    unsigned int Size = cmd_size;
    int WaitCount = 30;

    //==============================================================
    //copy to pVORPC->pICQRingBuffer(ICQ ring buffer)(= alloc)
    //==============================================================
    alloc = pICQRingBuffer;
    pBufferLower = m_pICQBufferLower;
    pBufferUpper = m_pICQBufferUpper;
    Data = (unsigned char *)cmd_endian_swapped;

    while((res =OSAL_RTKRequestWriteSpace(alloc, &pBufferCurr, (OSAL_S32 *)&writableSize, Size, 0)) == OSAL_ERROR_NOT_READY) {
#if defined(__LP64__)
        RingBufferPrint("NOT READY, writableSize = 0x%x, cmd_size = 0x%x, [0x%p - 0x%p] \n",
                writableSize, Size, pBufferLower, m_pICQBufferUpper);
#else
        RingBufferPrint("NOT READY, writableSize = 0x%x, cmd_size = 0x%x, [0x%08x - 0x%08x] \n",
                writableSize, Size, (unsigned int) pBufferLower, (unsigned int) m_pICQBufferUpper);
#endif
        usleep(1000);

        if (WaitCount < 0)
            goto err;

        WaitCount--;
    }

#if 0
    RingBufferPrint("[%s] %p-%p , pBufferCurr:%p Size:%d res:%d", __FUNCTION__, m_pICQBufferLower, m_pICQBufferUpper,
            pBufferCurr, Size, res);
#endif

    if(pBufferCurr)
    {
#if defined(__LP64__)
        if( ((unsigned long) pBufferCurr > (unsigned long) m_pICQBufferUpper) || ((unsigned long) pBufferCurr < (unsigned long) m_pICQBufferLower) )
            goto err;
#else
        if( ((unsigned int) pBufferCurr > (unsigned int) m_pICQBufferUpper) || ((unsigned int) pBufferCurr < (unsigned int) m_pICQBufferLower) )
            goto err;
#endif

        if(res == OSAL_ERROR_ALLOC_RING_UPPER_BOUND)
        {
            unsigned int space = pBufferUpper - pBufferCurr;
            memcpy(pBufferCurr, Data, space);
            if(Size > space)
            {
                memcpy(pBufferLower, Data + space, Size - space);
            }
            pBufferNext = pBufferLower + Size - space;
        }
        else
        {
            memcpy(pBufferCurr, Data, Size);
            pBufferNext = pBufferCurr + Size;
        }

        //==============================================================
        //update write pointer
        //==============================================================
#if 0
        RingBufferPrint("[%s] update write pointer :0x%lx", __FUNCTION__, (unsigned long)pBufferNext);
#endif
        OSAL_RTKSyncRingBuffer(alloc);

        OSAL_RTKSetWritePtr(alloc, pBufferNext);
        //RingBufferPrint("v = %x, p= %x, write %x\n",pBufferNext,(int)(pBufferNext+alloc->m_headerPhyAddrDiff),&(alloc->m_pBufferHeader->writePtr));
        pthread_mutex_unlock(&mLock);
        return Size;
    }

    RingBufferPrint("pBufferCurr=NULL!\n");
    pthread_mutex_unlock(&mLock);
    return 0;

err:
#if defined(__LP64__)
    if (!(WaitCount < 0))
        RingBufferPrint("[ERROR] pBufferCurr:0x%p, Size:0x%x [0x%p - 0x%p]\n",
                pBufferCurr, Size, pBufferLower, m_pICQBufferUpper);
#else
    if (!(WaitCount < 0))
        RingBufferPrint("[ERROR] pBufferCurr:0x%08x, Size:0x%x [0x%08x - 0x%08x]\n",
                (unsigned int) pBufferCurr, Size, (unsigned int) pBufferLower, (unsigned int) m_pICQBufferUpper);
#endif

    pthread_mutex_unlock(&mLock);
    return 0;
}

//==============================================================
// receive writeback INBAND COMMAND from VO
//==============================================================
unsigned int RingBufferBase::recvInbandCmd(void *data, unsigned int size)
{
    OSAL_RTKALLOCATOR* alloc = pICQRingBuffer;
    OSAL_U8 *lower, *upper, *rptr, *wptr; //virtual addr
    OSAL_S32 bufsize;
    OSAL_RTKGetBuffer(alloc, (unsigned char **)&lower, &bufsize);
    //ALOGD("alloc:%p lower:%p bufsize:%d", alloc, lower, bufsize);
    upper = lower + bufsize;
    rptr = OSAL_RTKGetReadPtr(alloc, 0);
    wptr = OSAL_RTKGetWritePtr(alloc);

    OSAL_RTKSyncRingBuffer(alloc);

    OSAL_S32 avail = (bufsize + wptr - rptr) % bufsize;
    //ALOGD("rptr:%p wptr:%p avail:%u size:%u", rptr, wptr, avail, size);
    if(avail < (OSAL_S32)size){
        return 0;
    }

    OSAL_S32 nbyte = upper - rptr;
    if(nbyte >= (OSAL_S32)size){
        endianSwapMemoryCopy(rptr, (unsigned char*)data, size);
        rptr += size;
        if(rptr >= upper)
            rptr = lower;
    }else{
        OSAL_S32 remain = size - nbyte;
        endianSwapMemoryCopy(rptr, (OSAL_U8 *)data, nbyte);
        endianSwapMemoryCopy(lower, (OSAL_U8 *)data + nbyte, remain);
        rptr = lower + remain;
    }

    OSAL_RTKSetReadPtr(alloc, 0, rptr);
    //ALOGD("update rptr:%p", rptr);
    return size;
}

int RingBufferBase::ConfigLowDelay(enum LowDelayMode mode, int depth, int init_frame)
{
    int ret;
    ret = ConfigLowDelayMode(mode);

    if ((ret < 0) || (mode == LowDelayOff))
        return ret;

    ret = ConfigLowDelayDepth(depth, init_frame);

    return ret;
}

int RingBufferBase::ConfigLowDelayDepth(int depth, int init_frame)
{
    HRESULT* h;
    VIDEO_RPC_VOUT_SET_Q_PARAMETER info = {
        .depth = (unsigned int) depth,
        .init_frame = (unsigned int) ((init_frame <= 0) ? 1 : init_frame),
        .jitter = 0,
    };

    if (depth < 0)
        return -EINVAL;

    if (depth == mLowDelayDepth && init_frame == mInitFrame)
        return 0;

    ATRACE_NAME("VIDEO_RPC_VOUT_ToAgent_Set_Q_Parameter_0");
    h = VIDEO_RPC_VOUT_ToAgent_Set_Q_Parameter_0(&info, &mClnt);

    if(h)
    {
        if (*h == S_OK) {
            mLowDelayDepth = depth;
            mInitFrame = init_frame;
            RingBufferPrint("[VIDEO_RPC_VOUT_ToAgent_Set_Q_Parameter_0]RPC S_OK! mLowDelayDepth = %d mInitFrame = %d\n", mLowDelayDepth, mInitFrame);
        } else {
            RingBufferPrint("[VIDEO_RPC_VOUT_ToAgent_Set_Q_Parameter_0]RPC fail! (0x%x)\n",* (unsigned int *) h);
        }

        free(h);
    }
    else
    {
        RingBufferPrint("[VIDEO_RPC_VOUT_ToAgent_Set_Q_Parameter_0]RPC return Error\n!\n");
    }

    return 0;

}

int RingBufferBase::ConfigLowDelayMode(enum LowDelayMode mode)
{
    HRESULT* h;
    VIDEO_RPC_LOW_DELAY info;
    info.mode = (VIDEO_LOW_DELAY) mode;

    if (mLowDelayMode == mode)
        return 0;

    ATRACE_NAME("VIDEO_RPC_ToAgent_ConfigLowDelay_0");
    h = VIDEO_RPC_ToAgent_ConfigLowDelay_0(&info, &mClnt);

    if(h)
    {
        if (*h == S_OK) {
            mLowDelayMode = mode;
            RingBufferPrint("[VIDEO_RPC_ToAgent_ConfigLowDelay_0]RPC S_OK! mLowDelayMode = %d\n", mLowDelayMode);
        } else {
            RingBufferPrint("[VIDEO_RPC_ToAgent_ConfigLowDelay_0]RPC fail! (0x%x)\n",* (unsigned int *) h);
        }

        free(h);
    }
    else
    {
        RingBufferPrint("[VIDEO_RPC_ToAgent_ConfigLowDelay_0]RPC return Error\n!\n");
    }

    return 0;
}

int RingBufferBase::SetDeintFlag(Client * client, int flag) {
    bool found = false;
    int ret = -1;
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
    //pthread_mutex_lock(&mLock);
    if (found)
        ret = SetDeintFlag(flag);
    //pthread_mutex_unlock(&mLock);
    return ret;
}

int RingBufferBase::SetDeintFlag(int flag) {
    HRESULT* h;
    VIDEO_RPC_VOUT_SET_DIFLAG set_diflag;

    if (flag == mDeintFlag)
        return 0;

    mDeintFlag = flag;

    if (mDeintFlag < 0) {
        ALOGE("[%s] flag is %d !", __FUNCTION__, flag);
        return -1;
    }

    set_diflag.flag = (ENUM_DEINTFLAG) mDeintFlag;

    ATRACE_NAME("VIDEO_RPC_VOUT_ToAgent_Set_DeintFlag_0");
    h = VIDEO_RPC_VOUT_ToAgent_Set_DeintFlag_0((VIDEO_RPC_VOUT_SET_DIFLAG *)&set_diflag, &mClnt);

    if (h && (*h == S_OK)) {
        ALOGD("[%s] VIDEO_RPC_VOUT_ToAgent_Set_DeintFlag_0 return OK!\n",__func__);
        free(h);
        return 0;
    } 

    if (h)
        free(h);

    ALOGE("[%s] return ERROR!\n",__func__);
    return -1;
}
