#ifndef RTK_LIB_VOWRITEBACK_H
#define RTK_LIB_VOWRITEBACK_H

#include <vector>
#include "YUV_STATE.h"

class VOWriteBack {
    public:
        VOWriteBack(unsigned int fbCount);
        ~VOWriteBack();
        bool            lib_rtk_codec_vo_checkAvailable(int type);
        bool            lib_rtk_codec_vo_initICQRingBuffer();
        unsigned int    lib_rtk_process_InbandCmd(int cmd_type);
        unsigned int    lib_rtk_codec_vo_sendInbandCmd(void* p, void *cmd_endian_swapped, unsigned int cmd_size);

        bool            lib_rtk_codec_vo_initWriteBackRingBuffer();
        //bool            lib_rtk_codec_vo_initWriteBackBuffers(int width, int height);
        bool            lib_rtk_codec_vo_getWriteBackFrame(YUV_STATE *frame); // It must be a YUV_STATE pointer
        int             lib_rtk_codec_vo_getWriteBackFrameIfDone(YUV_STATE *frame);
        //bool            lib_rtk_codec_vo_SyncWriteBackBuffers(int width, int height);
        //void            lib_rtk_buffer_endian_swap_4bytes(void *addr, unsigned int byte_size);
        //void            _dbg_check_vo_inband_queue(OSAL_RTKALLOCATOR *alloc);
        bool            lib_rtk_codec_vo_prepareBuffer(int index, unsigned int yAddr, unsigned int uAddr, int width, int height, int pLockAddr);
        bool            lib_rtk_codec_vo_prepareBuffer_deinterlace(unsigned int yAddr, unsigned int uAddr, unsigned int yAddr_next, unsigned int uAddr_next, int width, int height, int pLockAddr, int pLockAddr_next, bool dInterlaceFlw);
        
        bool            lib_rtk_codec_vo_uninit();
        bool            lib_rtk_codec_vo_config();
        bool            lib_rtk_codec_vo_releasepLock(unsigned char *pLockphyAddr);
        unsigned int    lib_rtk_codec_vo_getpLockAddrWithIdx(int idx , unsigned char **pLockVirAddr);
        unsigned int    lib_rtk_codec_vo_getpLockphyAddr(int idx , int fieldidx , int dFBCount, int dIsInterlaceFrm);
        int             lib_rtk_codec_vo_rpc_ready();

        bool            lib_rtk_codec_vo_ConfigWriteBackBuffers(int width, int height,int dEnbSetFormat, int targetFormat);
        bool            lib_rtk_codec_vo_AddWriteBackBuffers(int idx, unsigned long phyAddress, unsigned char * virtAddress, int IsUseAllocBuff, bool Is10bit=false);
        bool            lib_rtk_codec_vo_ClearWriteBackBuffers(unsigned long phyAddress, unsigned char * virtAddress);
        bool            lib_rtk_codec_vo_LockWriteBackBuffers(unsigned long phyAddress, unsigned char * virtAddress);
        bool            lib_rtk_codec_vo_UnSetWriteBackBuffers(unsigned long phyAddress, unsigned char * virtAddress);
        bool            lib_rtk_codec_vo_ReleaseWriteBackBuffers();
        int             lib_rtk_codec_vo_GetWBNum();

        bool            lib_rtk_codec_vo_GetReleasedBufIdx(int *Idx, int eos);

        //rvsd_OSAL
        //void            lib_rtk_pli_IPCCopyMemory(BYTE* src, BYTE* des, unsigned long len);
        int             lib_rtk_codec_vo_GetDecLockInfo();
        //int             lib_rtk_codec_vo_getbuf(int wid,int hei);

    private:
        void *                          mVORPC;
        std::vector<YUV_STATE>          mSendQueue;
        //std::vector<int>                mIndexQueue;
};

#endif
