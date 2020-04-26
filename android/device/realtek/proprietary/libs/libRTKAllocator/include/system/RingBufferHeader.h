#ifndef RTK_LIBRTKALLOCATOR_RINGBUFFER_HEADER_H
#define RTK_LIBRTKALLOCATOR_RINGBUFFER_HEADER_H

#include <allocator/Allocator.h>
#include <allocator/IonAllocator.h>

#ifdef __cplusplus
class RingBufferHeader : public IonAllocator {
public:
    void * const PHYADDR_ERROR = AllocatorDef::PHYADDR_ERROR;
    void * const VIRADDR_ERROR = AllocatorDef::VIRADDR_ERROR;

    /* Default:
     *      Heap Mask : AudioHeapMask
     *      Heap Flag : NONCACHED|SCPUACC|HWIPACC|ACPUACC
     */
    RingBufferHeader(
            enum IonAllocator::IonOper oper = IonAllocator::eNoneBuffer,
            long oper_data = 0,
            unsigned int heap_mask = IonAllocator::AudioHeapMask,
            unsigned int flag_mask = (
                IonAllocator::NONCACHED |
                IonAllocator::SCPUACC   |
                IonAllocator::ACPUACC   |
                IonAllocator::HWIPACC));

    virtual ~RingBufferHeader() {};

    /* Base API of IonAllocator */
    virtual int                 Alloc           (unsigned long extSize = 0);

    /* Spec API of RingBufferHeader op */
    virtual unsigned int        GetMsgMagic         (void);
    virtual void                SetMsgMagic         (unsigned int magic = 0);
    virtual unsigned long       GetMsgBeginPhyAddr  (void);
    virtual void                SetMsgBeginPhyAddr  (unsigned long phyAddr);
    virtual unsigned long       GetMsgSize          (void);
    virtual void                SetMsgSize          (unsigned long size);
    virtual unsigned long       GetMsgBufferId      (void);
    virtual void                SetMsgBufferId      (unsigned long bufferId = 0);
    virtual unsigned long       GetMsgWritePhyAddr  (void);
    virtual int                 SetMsgWritePhyAddr  (unsigned long phyAddr);
    virtual unsigned long       GetMsgReadPhyAddr   (void);
    virtual int                 SetMsgReadPhyAddr   (unsigned long phyAddr);
    virtual unsigned long       GetMsgWriteOffset   (void);
    virtual int                 SetMsgWriteOffset   (unsigned long offset);
    virtual unsigned long       GetMsgReadOffset    (void);
    virtual int                 SetMsgReadOffset    (unsigned long offset);
    virtual unsigned long       GetMsgWritableSize  (void);
    virtual unsigned long       GetMsgReadableSize  (void);
    virtual int                 PutMsgWriteSize     (unsigned long size);
    virtual int                 PutMsgReadSize      (unsigned long size);

    /* Spec API for Multiple Reader */
    virtual unsigned long       GetMsgWritableSizeBySlot    (unsigned int slot = 0);
    virtual unsigned long       GetMsgReadableSizeBySlot    (unsigned int slot = 0);
    virtual unsigned long       GetMsgReadPhyAddrBySlot     (unsigned int slot = 0);
    virtual int                 SetMsgReadPhyAddrBySlot     (unsigned int slot = 0, unsigned long phyAddr = 0);
    virtual unsigned long       GetMsgLastReadPhyAddr       (void);
    virtual unsigned int        GetReadreSlot       (void);
    virtual void                SetReadreSlot       (unsigned int slot = 0); /* 0 ~ 3 */
    virtual unsigned int        GetMsgNumOfReadPtr  (void);
    virtual void                SetMsgNumOfReadPtr  (unsigned int num = 1); /* 1 ~ 4 */
private:
    unsigned int mReaderSlot;
};
#else
#define RINGBUFFERHEADER_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define RINGBUFFERHEADER_VIRADDR_ERROR DEF_VIRADDR_ERROR;
typedef struct RingBufferHeader RingBufferHeader;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    RingBufferHeader *  RingBufferHeader_new();
    void                RingBufferHeader_delete                 (RingBufferHeader * pInstance);

    int                 RingBufferHeader_Alloc                  (RingBufferHeader * pInstance, unsigned long extSize);
    unsigned int        RingBufferHeader_GetMsgMagic            (RingBufferHeader * pInstance);
    void                RingBufferHeader_SetMsgMagic            (RingBufferHeader * pInstance, unsigned int magic);
    unsigned long       RingBufferHeader_GetMsgBeginPhyAddr     (RingBufferHeader * pInstance);
    void                RingBufferHeader_SetMsgBeginPhyAddr     (RingBufferHeader * pInstance, unsigned long phyAddr);
    unsigned long       RingBufferHeader_GetMsgSize             (RingBufferHeader * pInstance);
    void                RingBufferHeader_SetMsgSize             (RingBufferHeader * pInstance, unsigned long size);
    unsigned long       RingBufferHeader_GetMsgBufferId         (RingBufferHeader * pInstance);
    void                RingBufferHeader_SetMsgBufferId         (RingBufferHeader * pInstance, unsigned long bufferId);
    unsigned long       RingBufferHeader_GetMsgWritePhyAddr     (RingBufferHeader * pInstance);
    int                 RingBufferHeader_SetMsgWritePhyAddr     (RingBufferHeader * pInstance, unsigned long phyAddr);
    unsigned long       RingBufferHeader_GetMsgReadPhyAddr      (RingBufferHeader * pInstance);
    int                 RingBufferHeader_SetMsgReadPhyAddr      (RingBufferHeader * pInstance, unsigned long phyAddr);
    unsigned long       RingBufferHeader_GetMsgWriteOffset      (RingBufferHeader * pInstance);
    int                 RingBufferHeader_SetMsgWriteOffset      (RingBufferHeader * pInstance, unsigned long offset);
    unsigned long       RingBufferHeader_GetMsgReadOffset       (RingBufferHeader * pInstance);
    int                 RingBufferHeader_SetMsgReadOffset       (RingBufferHeader * pInstance, unsigned long offset);
    unsigned long       RingBufferHeader_GetMsgWritableSize     (RingBufferHeader * pInstance);
    unsigned long       RingBufferHeader_GetMsgReadableSize     (RingBufferHeader * pInstance);
    int                 RingBufferHeader_PutMsgWriteSize        (RingBufferHeader * pInstance, unsigned long size);
    int                 RingBufferHeader_PutMsgReadSize         (RingBufferHeader * pInstance, unsigned long size);

    unsigned long       RingBufferHeader_GetMsgWritableSizeBySlot    (RingBufferHeader * pInstance, unsigned int slot);
    unsigned long       RingBufferHeader_GetMsgReadableSizeBySlot    (RingBufferHeader * pInstance, unsigned int slot);
    unsigned long       RingBufferHeader_GetMsgReadPhyAddrBySlot     (RingBufferHeader * pInstance, unsigned int slot);
    int                 RingBufferHeader_SetMsgReadPhyAddrBySlot     (RingBufferHeader * pInstance, unsigned int slot, unsigned long phyAddr);
    unsigned long       RingBufferHeader_GetMsgLastReadPhyAddr       (RingBufferHeader * pInstance);
    unsigned int        RingBufferHeader_GetReadreSlot          (RingBufferHeader * pInstance);
    void                RingBufferHeader_SetReadreSlot          (RingBufferHeader * pInstance, unsigned int slot); /* 0 ~ 3 */
    unsigned int        RingBufferHeader_GetMsgNumOfReadPtr     (RingBufferHeader * pInstance);
    void                RingBufferHeader_SetMsgNumOfReadPtr     (RingBufferHeader * pInstance, unsigned int num); /* 1 ~ 4 */
#ifdef __cplusplus
}
#endif


#endif /* End of RTK_LIBRTKALLOCATOR_RINGBUFFER_HEADER_HH */
