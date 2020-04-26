#ifndef RPC_GLOBAL_H
#define RPC_GLOBAL_H

class VORPCGlobal {
    public:    
        unsigned char bVOReady;
        unsigned int out_id;
        int ion_fd;
        CLNT_STRUCT clnt;

        SystemMemory        *mVOMemory;
        VOWB_RTKALLOCATOR   mwbpLk;
        VOWB_RTKALLOCATOR   mScaleBuf;
        VOWB_RTKALLOCATOR   mpLock_ion;

        VOWB_RTKRINGBUFFER  mICQRingBuffer;
        VOWB_RTKRINGBUFFER  mWBRingBuffer;

        VOWBbuf_Struct      wbBufSt[WRITEBACK_BUFNUM];

        int                 wbIonSize;
        int                 wbBufNum;
        int                 wbBufIdx;
        unsigned int        wbInit;             //0:uninit 1:tvsystem 2:ringbuffer 3:wbbuffers
        int                 wbWidth;
        int                 wbHeight;
        struct timeval      timeStamp;
        unsigned int        wbFCnt;
        struct timeval      DectimeStamp;
        unsigned int        DecFCnt;

        pLockList           *stpLockList;
        pLockList           *stpLockphyList;
        int                 dpLockNumIdx;
        unsigned char       *pLockvirAddr;
        int                 FbCount;
        DataQueue           *pLockR_queue;
        int                 dInterlaceNeedFirstFlg;

        VOWBbuf_Struct      stVPUFrmIdx[WRITEBACK_BUFNUM];
        int                 dstRdptr;
        int                 dstWtptr;

        int                 dEnbSetFormat;
        int                 targetFormat; //'rtk1'  bit 0=>NV21, 1:NV21, 0:NV12; bit 1=>422, 1:422, 0:420
        int                 wbStride;
    private:
};

#endif
